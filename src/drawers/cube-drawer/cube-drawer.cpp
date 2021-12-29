#include "cube-drawer.hpp"

#include <GLES2/gl2.h>
#include <emscripten/html5_webgl.h>
#include <emscripten/val.h>
#include <webgl/webgl1.h>

#include <algorithm>
#include <string>
#include <tuple>

#include "../../helpers/assert.hpp"
#include "../../helpers/opengl.hpp"
#include "../../helpers/utils.hpp"
#include "geometry/cube-texture-coords.hpp"
#include "geometry/cube-vertex-coords.hpp"

constexpr Radians FIELD_OF_VIEW = degToRad(60);

// using GLES2 API to draw 3D since it's basically the same as webgl API.
// alternatively emscripten has static bindings for webgl (too long func names
// due to "emscripten_" prefix) or SDL (totally different API)
void initCubeDrawer(GameState* state) {
  auto& scene = state->scene;
  ASSERT(scene.canvas.has_value());

  EmscriptenWebGLContextAttributes attrs{
      .alpha = GL_TRUE,
      .depth = GL_TRUE,
      .stencil = GL_FALSE,
      .antialias = GL_TRUE,
      .premultipliedAlpha = GL_TRUE,
      .preserveDrawingBuffer = GL_FALSE,
      .powerPreference = EM_WEBGL_POWER_PREFERENCE_DEFAULT,
      .failIfMajorPerformanceCaveat = GL_FALSE};

  auto ctx_handle = emscripten_webgl_create_context("canvas", &attrs);
  emscripten_webgl_make_context_current(ctx_handle);

  // getting webgl context second time. this time in a form of embind handle, so
  // later we can use it for dynamic binding. this is ugly. ideally we would use
  // GLES2 API for everything, but turns out GLES2 API is not suitable for all
  // possible scenarious. eg. see comment below about using another canvas as a
  // data source for texture
  //
  // anyway getting context multiple times on canvas shouldn't cause any
  // problems, since web API doc explicitly states that "Later calls to this
  // method [getContext] on the same canvas element, with the same contextType
  // argument, will always return the same drawing context instance as was
  // returned the first time the method was invoked."
  auto ctx =
      scene.canvas->call<emscripten::val, std::string>("getContext", "webgl");
  scene.ctx = ctx;

  auto& cube = scene.cube;

  // compile GLSL shaders for cube
  const auto vertex_shader_src =
      read_file_to_string("src/drawers/cube-drawer/shaders/vertex.glsl");
  const auto fragment_shader_src =
      read_file_to_string("src/drawers/cube-drawer/shaders/fragment.glsl");

  const auto vertex_shader = initShader(GL_VERTEX_SHADER, vertex_shader_src);
  const auto fragment_shader =
      initShader(GL_FRAGMENT_SHADER, fragment_shader_src);

  auto program = initProgram({vertex_shader, fragment_shader});
  scene.cube.program = program;

  glUseProgram(program);

  // lookup locations for attributes/uniforms
  auto cube_vertex_coord_attr_location =
      getAttributeLocation(program, "a_cube_vertex_coord");
  auto cube_vertex_side_attr_location =
      getAttributeLocation(program, "a_cube_vertex_side");
  auto cube_texture_coord_attr_location =
      getAttributeLocation(program, "a_cube_texture_coord");
  cube.matrix_uniform_location = getUniformLocation(program, "u_matrix");

  // pass buffer with vertex coordinates
  GLuint cube_vertex_coords_buffer{};
  glGenBuffers(1, &cube_vertex_coords_buffer);
  glBindBuffer(GL_ARRAY_BUFFER, cube_vertex_coords_buffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertex_coords),
               cube_vertex_coords.data(), GL_STATIC_DRAW);

  // pass buffer with texture coordinates
  GLuint cube_texture_coords_buffer{};
  glGenBuffers(1, &cube_texture_coords_buffer);
  glBindBuffer(GL_ARRAY_BUFFER, cube_texture_coords_buffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(cube_texture_coords),
               cube_texture_coords.data(), GL_STATIC_DRAW);

  // define how to extract coordinates from vertex buffer
  glEnableVertexAttribArray(cube_vertex_coord_attr_location);
  glBindBuffer(GL_ARRAY_BUFFER, cube_vertex_coords_buffer);

  glVertexAttribPointer(cube_vertex_coord_attr_location,  // index
                        3,         // size. 3 components per iteration
                        GL_FLOAT,  // type. the data is 32bit floats
                        GL_FALSE,  // normalize. don't normalize the data
                        16,       // stride. (bytes) each vertex consists of 4 x
                                  // 4-byte floats (side, x, y, z)
                        (void*)4  // NOLINT (bytes) offset. skip side float
  );

  // define how to extract cube side index from vertex buffer
  glEnableVertexAttribArray(cube_vertex_side_attr_location);
  glBindBuffer(GL_ARRAY_BUFFER, cube_vertex_coords_buffer);

  glVertexAttribPointer(cube_vertex_side_attr_location,  // index
                        1,                               // size
                        GL_FLOAT,                        // type
                        GL_FALSE,                        // normalize
                        16,                              // stride
                        nullptr                          // offset
  );

  // define how to extract coordinates from texture coordinates buffer
  glEnableVertexAttribArray(cube_texture_coord_attr_location);
  glBindBuffer(GL_ARRAY_BUFFER, cube_texture_coords_buffer);

  glVertexAttribPointer(cube_texture_coord_attr_location,  // index
                        2,                                 // size
                        GL_FLOAT,                          // type
                        GL_FALSE,                          // normalize
                        0,                                 // stride
                        nullptr                            // offset
  );

  // create textures for cube sides
  std::vector<GLuint> cube_textures;
  cube_textures.reserve(cube.sides.size());
  for (const auto& [side_type, side] : cube.sides) {
    GLuint texture{};
    glGenTextures(1, &texture);
    cube_textures.push_back(texture);

    // bind uniform with texture unit
    const auto side_type_idx = static_cast<int>(side_type);
    const auto uniform_name =
        "u_cube_texture_side_" + std::to_string(side_type_idx);
    GLint cube_texture_side_uniform_location =
        getUniformLocation(program, uniform_name.c_str());

    glUniform1i(cube_texture_side_uniform_location, side_type_idx);
  }

  cube.textures = std::move(cube_textures);

  // pass texture data for the first time (update later in draw loop)
  for (auto& [side_type, side] : cube.sides) {
    ASSERT(side.canvas.has_value());
    ASSERT(side.ctx.has_value());
    const auto& canvas = side.canvas.value();

    const auto side_type_idx = static_cast<int>(side_type);
    glActiveTexture(GL_TEXTURE0 + side_type_idx);  // select texture unit
    glBindTexture(GL_TEXTURE_2D, cube.textures[side_type_idx]);

    // use dynamic binding to webgl API instead of GLES2 API here, because
    // GLES2 "glTexImage2D" expects pointer to data array "const void *pixels":
    // (a) it's impossible to get pointer to canvas data and pass it to wasm
    // function, since canvas data is located in js memory,
    // (b) js glue expectes pointer to wasm memory only
    ctx.call<void>("texImage2D",
                   ctx["TEXTURE_2D"],     // target
                   0,                     // level
                   ctx["RGBA"],           // internal format
                   ctx["RGBA"],           // format
                   ctx["UNSIGNED_BYTE"],  // type
                   canvas                 // source
    );

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    side.needs_update_on_cube = false;
  }
}

auto shouldRedrawCube(const Cube& cube) -> bool {
  return cube.needs_redraw ||
         std::any_of(cube.sides.begin(), cube.sides.end(),
                     [](const std::pair<ECubeSide, CubeSide>& p) {
                       return p.second.needs_update_on_cube;
                     });
}

void drawCubeLoop(GameState* state) {
  ASSERT(state != nullptr);
  ASSERT(state->scene.canvas.has_value());

  const auto& scene = state->scene;
  const auto& canvas = scene.canvas.value();
  const auto& cube = scene.cube;

  if (!shouldRedrawCube(cube)) {
    return;
  }

  // define how to convert from clip space to canvas pixels
  glViewport(0, 0, canvas["width"].as<int>(), canvas["height"].as<int>());

  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);

  // clear the canvas and the depth buffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // calculate transformation matrix
  const auto aspect =
      canvas["clientWidth"].as<double>() / canvas["clientHeight"].as<double>();
  const auto projection_matrix = perspective(FIELD_OF_VIEW, aspect, 1, 2000);

  static const Vec3 cameraPosition{0, 0, 2};
  static const Vec3 up{0, 1, 0};
  static const Vec3 target{0, 0, 0};

  static const auto camera_matrix = lookAt(cameraPosition, target, up);
  static const auto view_matrix = inverse(camera_matrix);
  const auto view_projection_matrix = multiply(projection_matrix, view_matrix);

  auto matrix =
      xRotate(view_projection_matrix, degToRad(cube.current_rotation.x));
  matrix = yRotate(matrix, degToRad(cube.current_rotation.y));

  drawCube(state, matrix);
}

void drawCube(GameState* state, const Matrix4& matrix) {
  ASSERT(state != nullptr);
  ASSERT(state->scene.canvas.has_value());
  ASSERT(state->scene.ctx.has_value());

  auto& scene = state->scene;
  auto& cube = scene.cube;
  const auto& canvas = scene.canvas.value();
  const auto& ctx = scene.ctx.value();

  ASSERT(cube.program.has_value());
  ASSERT(cube.textures.size() == cube.sides.size());

  glUseProgram(cube.program.value());

  // update texture data if needed
  for (auto& [side_type, side] : cube.sides) {
    if (side.needs_update_on_cube) {
      ASSERT(side.canvas.has_value());

      const auto side_type_index = static_cast<int>(side_type);
      glActiveTexture(GL_TEXTURE0 + side_type_index);  // select texture unit
      glBindTexture(GL_TEXTURE_2D, cube.textures[side_type_index]);

      ctx.call<void>("texSubImage2D",
                     ctx["TEXTURE_2D"],     // target
                     0,                     // level
                     0,                     // offset x
                     0,                     // offset y
                     ctx["RGBA"],           // format
                     ctx["UNSIGNED_BYTE"],  // type
                     side.canvas.value()    // source
      );

      side.needs_update_on_cube = false;
    }
  }

  // pass transformation matrix
  ASSERT(cube.matrix_uniform_location.has_value());
  glUniformMatrix4fv(cube.matrix_uniform_location.value(), 1, GL_FALSE,
                     matrix.data());

  // draw the geometry
  constexpr int CUBE_VERTICES_COUNT = 6     // cube sides
                                      * 2   // triangles per cube side
                                      * 3;  // vertices per triangle
  glDrawArrays(GL_TRIANGLES, 0, CUBE_VERTICES_COUNT);

  cube.needs_redraw = false;
}

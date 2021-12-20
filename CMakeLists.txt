cmake_minimum_required(VERSION 3.16.3)

project(Snake3D)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -g")

set(MAIN_SOURCE_DIR "src")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build)

include_directories(/emsdk/upstream/emscripten/system/include)

file(GLOB_RECURSE CPP_HEADERS ${MAIN_SOURCE_DIR}/*.hpp)
file(GLOB_RECURSE CPP_SOURCES ${MAIN_SOURCE_DIR}/*.cpp)

add_executable(
    main
    ${CPP_HEADERS}
    ${CPP_SOURCES}
)

set_target_properties(
    main
    PROPERTIES
    LINK_FLAGS
    # emcc options:
    # - resulting glue js code should target browser, not nodejs (eg. do not "require 'fs'")
    # - pack all files been read in c++ code into '.data' file next to '.wasm'
    # - support embind feature (eg. emscripten::val)
    "-s ENVIRONMENT='web' \
     --preload-file src/shaders/vertex.glsl \
     --preload-file src/shaders/fragment.glsl \
     --bind"
)
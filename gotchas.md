1. emscripten doesn't provide complete static bindings to web API (like `wasm-bindgen` does in rust)  
  it only has limited hand-made bindings (events/timers/console/throw/webgl)  
  https://emscripten.org/docs/api_reference/html5.h.html  

1. emscripten supports dynamic binding (embind), which allows to call any JS func/class/etc from c++.  
  it means that entire web API can be used from c++ without any static bindings.  
  https://emscripten.org/docs/porting/connecting_cpp_and_javascript/embind.html#embind-val-guide  

    downsides of dynamic binding compared to static binding:  
    - no static validation (no typecheck, autocomplete, docs, etc)
    - slower, because it first needs to copy method name string from wasm memory to js memory.  
      eg. wasm calls `__emval_call_void_method(caller, handle, methodName, args)` where `methodName`
      and `args` are pointers to wasm memory.  
      it first copies method name to js memory char by char converting it from UTF-8 to UTF-16 along the way.  
      `args` points to arguments object located in wasm memory, which should be extracted/copied one by one too.  
      `handle` is index of js object previously saved to handle array to call methods on it, which is the same as with static binding.  

      `readLatin1String` js glue function performs this copy/conversion for method name strings.  
      it takes 8% of total js/wasm execution time, and is second hottest func after `texSubImage2D` (which updates cube textures) with 9%


1. emscripten provides several static binding options to work with canvas's webgl/2D context:  
    - SDL/OpenGL/OpenGLES/webgl translates to webgl context
    - SDL translates to 2D context

    in this project:  
    - for 3D I'm using OpenGLES2 API because it basicaly the same as webgl
    - for 2D only static bindings option I see is SDL which is far different from 2D context web API, and I want to stick to web API as close as possible

1. emscripten translates OpenGL to webgl API calls entirely inside js glue code.  
  e.g. wasm calls imported `glCreateShader` func and js glue calls `ctx.createShader`  

1. emscripten's OpenGLES2 (I believe webgl too) bindings does't allow using another canvas as a data source for texture.  
  `glTexImage2D` expects pointer to pixel array `const void *pixels`.  
  it's easy to get pointer to pixel array when it's located inside wasm memory, but in order to pass `canvas/ctx.getImageData().data` you have to  
  (a) get js memory pointer and pass it to wasm function, which is impossible,  
  (b) and js-glue should be able to translate this pointer back to typed array, which is impossible too,
      because js-glue `_glTexImage2D` expectes wasm memory pointer only  
  this is why only solution here is to use dynamic binding (embind) to webgl API

1. emscripten doesn't allow to natively `throw` errors from c++ to browser console  
  have to use special `emscripten_throw_string` binding  
  TODO: try add `-fexceptions` flag to compile command

1. clang-tidy doesn't support ignoring for blocks/files, only for single lines.  
  so had to add tons on NOLINT's in matrix math to ignore "magic number" warnings.  
  https://stackoverflow.com/questions/55165840/clang-tidy-nolint-for-multiple-lines

1. clang-tidy linter is poorly documented  
    - pain to find doc on some checks. some of them lead to private google docs
    - pain to find doc on `.clang-tidy` format. eg. complete list of supported options

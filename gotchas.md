1. emscripten doesn't provide complete HTML5/DOM bindings (as `wasm-bindgen` does in rust)  
  it only has limited hand-made bindings (events/timers/console/throw)  
  https://emscripten.org/docs/api_reference/html5.h.html  

1. emscripten doesn't allow to `throw` error message from c++ to browser console  
  have to use special `emscripten_throw_string` 
  TODO: try add `-fexceptions` flag to compile command

1. clang-tidy doesn't support ignoring for blocks/files, only for single lines.  
  so had to add tons on NOLINT's in matrix math to ignore "magic number" warnings.  
  https://stackoverflow.com/questions/55165840/clang-tidy-nolint-for-multiple-lines

1. emscripten doesn't provide any bindings directly to webgl/2d canvas contexts web APIs.  
  they are targets for translation from graphic APIs popular in c++ 
  - webgl context is target for translation from c++ opengl/SDL 
  - 2d context is target for translation from c++ SDL

1. emscripten translates opengl to webgl API calls entirely inside js glue code.  
  e.g. wasm calls imported `glCreateShader` func and js glue translates it to `ctx.createShader`  

1. emscripten supports dynamic binding (embind), which allows to call any JS func/class/etc from c++  
  which means entire web API can be used from c++ without any predefined bindings.  
  downside is that it's not statically validatable (no typecheck / autocomplete / name validation / func param check, etc)
  https://emscripten.org/docs/porting/connecting_cpp_and_javascript/embind.html#embind-val-guide

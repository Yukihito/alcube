cmake_minimum_required(VERSION 3.8)
project(alcube)

set(CMAKE_CXX_STANDARD 17)

#### Libs
### Utils
set(ALCUBE_UTILS_SOURCE_FILES
        src/cpp/utils/Profiler.cpp src/cpp/utils/Profiler.h
        src/cpp/utils/FileUtil.cpp src/cpp/utils/FileUtil.h
        src/cpp/utils/opencl/Memory.cpp src/cpp/utils/opencl/Memory.h
        src/cpp/utils/opencl/conversions.h src/cpp/utils/opencl/conversions.cpp
        src/cpp/utils/opencl/Resources.cpp src/cpp/utils/opencl/Resources.h
        src/cpp/utils/opencl/MemoryManager.cpp src/cpp/utils/opencl/MemoryManager.h
        src/cpp/utils/opencl/CommandQueue.cpp src/cpp/utils/opencl/CommandQueue.h
        src/cpp/utils/opencl/ProgramFactory.cpp src/cpp/utils/opencl/ProgramFactory.h
        src/cpp/utils/opencl/KernelFactory.cpp src/cpp/utils/opencl/KernelFactory.h
        src/cpp/utils/alcubemath.h src/cpp/utils/alcubemath.cpp
        src/cpp/utils/app/Application.cpp src/cpp/utils/app/Application.h
        src/cpp/utils/app/OpenGLApplication.cpp src/cpp/utils/app/OpenGLApplication.h
        src/cpp/utils/Mouse.cpp src/cpp/utils/Mouse.h
        src/cpp/utils/Keyboard.cpp src/cpp/utils/Keyboard.h
        src/cpp/utils/opencl/ResourcesProvider.cpp src/cpp/utils/opencl/ResourcesProvider.h
        )
add_library(alcube_utils_lib STATIC ${ALCUBE_UTILS_SOURCE_FILES})

### GPU Accessor
set(ALCUBE_GPU_ACCESSOR_SOURCE_FILES
        src/cpp/gpu/dtos.h src/cpp/gpu/GPUAccessor.cpp src/cpp/gpu/GPUAccessor.h
        )
add_library(alcube_gpu_accessor_lib STATIC ${ALCUBE_GPU_ACCESSOR_SOURCE_FILES})

### Drawing
set(ALCUBE_DRAWING_SOURCE_FILES
        src/cpp/drawing/Shader.cpp src/cpp/drawing/Shader.h
        src/cpp/drawing/Buffer.cpp src/cpp/drawing/Buffer.h
        src/cpp/drawing/Camera.cpp src/cpp/drawing/Camera.h
        src/cpp/drawing/Shape.cpp src/cpp/drawing/Shape.h
        src/cpp/drawing/Drawable.cpp src/cpp/drawing/Drawable.h
        src/cpp/drawing/DrawerWithProfiler.cpp src/cpp/drawing/DrawerWithProfiler.h
        src/cpp/drawing/Drawer.cpp src/cpp/drawing/Drawer.h
        src/cpp/drawing/Context.cpp src/cpp/drawing/Context.h
        src/cpp/drawing/Material.cpp src/cpp/drawing/Material.h
        src/cpp/drawing/shaders/Shaders.cpp src/cpp/drawing/shaders/Shaders.h
        src/cpp/drawing/shaders/DirectionalLightShader.cpp src/cpp/drawing/shaders/DirectionalLightShader.h
        src/cpp/drawing/shapes/triangles/Sphere.cpp src/cpp/drawing/shapes/triangles/Sphere.h
        )
add_library(alcube_drawing_lib STATIC ${ALCUBE_DRAWING_SOURCE_FILES})
target_link_libraries(alcube_drawing_lib
        alcube_utils_lib
        )

### Physics
set(ALCUBE_PHYSICS_SOURCE_FILES
        src/cpp/physics/SoftBodyActor.cpp src/cpp/physics/SoftBodyActor.h
        src/cpp/physics/Simulator.cpp src/cpp/physics/Simulator.h
        src/cpp/physics/Spring.cpp src/cpp/physics/Spring.h
        src/cpp/physics/FluidActor.cpp src/cpp/physics/FluidActor.h
        src/cpp/physics/Actor.cpp src/cpp/physics/Actor.h)
add_library(alcube_physics_lib STATIC ${ALCUBE_PHYSICS_SOURCE_FILES})
target_link_libraries(alcube_physics_lib
        alcube_utils_lib
        alcube_gpu_accessor_lib
        )

### Scripting
set(ALCUBE_SCRIPTING_SOURCE_FILES
        src/cpp/scripting/Evaluator.cpp src/cpp/scripting/Evaluator.h
        )
add_library(alcube_scripting_lib STATIC ${ALCUBE_SCRIPTING_SOURCE_FILES})
target_link_libraries(alcube_scripting_lib
        alcube_utils_lib
        alcube_drawing_lib
        alcube_physics_lib
        )

### Samples
set(ALCUBE_SAMPLES_SOURCE_FILES
        src/cpp/samples/SphereDrawable.cpp src/cpp/samples/SphereDrawable.h
        src/cpp/samples/ApplicationBase.cpp src/cpp/samples/ApplicationBase.h
        src/cpp/samples/SampleApplication.cpp src/cpp/samples/SampleApplication.h
        )

add_library(alcube_samples_lib STATIC ${ALCUBE_SAMPLES_SOURCE_FILES})
target_link_libraries(alcube_samples_lib
        alcube_physics_lib
        alcube_utils_lib
        alcube_gpu_accessor_lib
        alcube_drawing_lib
        )


### Extra libs
find_package(OpenGL REQUIRED)
find_package(OpenCL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)
set(V8_INCLUDE_DIR ext/v8/v8/include)
link_directories(ext/v8/v8/out.gn/x64.release/obj)
link_directories(ext/v8/v8/out.gn/x64.release/obj/third_party)
link_directories(ext/v8/v8/out.gn/x64.release/obj/third_party/icu)
link_directories(ext/v8/v8/out.gn/x64.release/obj/src/inspector)

include_directories(${OpenGL_gl_INCLUDE_DIR} ${OpenCL_INCLUDE_DIR} ${GLEW_INCLUDE_DIRS} ${V8_INCLUDE_DIR})
set (EXTRA_LIBS ${OPENGL_gl_LIBRARY} ${OPENGL_glu_LIBRARY} ${OpenCL_LIBRARY} ${GLEW_LIBRARIES} glfw
        _wasm_fuzzer_common
        fuzzer_support
        json_fuzzer
        multi_return_fuzzer
        parser_fuzzer
        regexp_builtins_fuzzer
        regexp_fuzzer
        v8_libplatform
        v8_base
        v8_external_snapshot
        v8_init
        v8_initializers
        v8_libbase
        v8_libsampler
        v8_nosnapshot
        v8_snapshot
        wasm_async_fuzzer
        wasm_code_fuzzer
        wasm_compile_fuzzer
        wasm_data_section_fuzzer
        wasm_function_sigs_section_fuzzer
        wasm_fuzzer
        wasm_globals_section_fuzzer
        wasm_imports_section_fuzzer
        wasm_memory_section_fuzzer
        wasm_module_runner
        wasm_names_section_fuzzer
        wasm_types_section_fuzzer
        icui18n
        icuuc
        inspector
        pthread
        #icuuc icui18n v8_libbase v8_libplatform
        )

### Aggregated libs
add_library(alcube_lib STATIC)

add_executable(kernel_compiler src/cpp/tools/kernelcompiler/main.cpp)
target_link_libraries(alcube_lib
        ${EXTRA_LIBS}
        alcube_utils_lib
        alcube_drawing_lib
        alcube_physics_lib
        )

#### Executables
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wno-deprecated-declarations -Wc++1z-extensions -stdlib=libc++")

### Main executable
# add_executable(alcube src/cpp/main.cpp)
# target_link_libraries(alcube alcube_lib)

### Sample executable
add_executable(alcube_samples src/cpp/samples/main.cpp)
target_link_libraries(alcube_samples
        alcube_lib
        alcube_samples_lib
        )

### Kernel compiler executable
target_link_libraries(kernel_compiler alcube_lib)

### Simple v8 sample
add_executable(v8_sample src/cpp/samples/v8/main.cpp)
target_link_libraries(v8_sample alcube_lib)

### Tests executable
# gtest
# Download and unpack googletest at configure time
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )
if(result)
    message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )
if(result)
    message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_BINARY_DIR}/googletest-src
        ${CMAKE_BINARY_DIR}/googletest-build
        EXCLUDE_FROM_ALL)

# The gtest/gtest_main targets carry header search path
# dependencies automatically when using CMake 2.8.11 or
# later. Otherwise we have to add them here ourselves.
if (CMAKE_VERSION VERSION_LESS 2.8.11)
    include_directories("${gtest_SOURCE_DIR}/include")
endif()

add_executable(tests test/cpp/physics/SimulatorTest.cpp)
target_link_libraries(tests gtest_main alcube_lib)
add_test(NAME test COMMAND tests)
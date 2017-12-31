cmake_minimum_required(VERSION 3.8)
project(alcube)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES src/cpp/main.cpp src/cpp/utils/Profiler.cpp src/cpp/utils/Profiler.h src/cpp/drawing/Shader.cpp src/cpp/drawing/Shader.h src/cpp/drawing/Buffer.cpp src/cpp/drawing/Buffer.h src/cpp/drawing/Camera.cpp src/cpp/drawing/Camera.h src/cpp/drawing/Shape.cpp src/cpp/drawing/Shape.h src/cpp/drawing/Drawable.cpp src/cpp/drawing/Drawable.h src/cpp/drawing/shapes/triangles/Sphere.cpp src/cpp/drawing/shapes/triangles/Sphere.h src/cpp/drawing/Drawer.cpp src/cpp/drawing/Drawer.h src/cpp/utils/FileUtil.cpp src/cpp/utils/FileUtil.h src/cpp/drawing/samples/SimpleSphereSample.cpp src/cpp/drawing/samples/SimpleSphereSample.h src/cpp/drawing/Context.cpp src/cpp/drawing/Context.h src/cpp/drawing/shaders/SimpleShader.cpp src/cpp/drawing/shaders/SimpleShader.h src/cpp/drawing/shaders/StandardShader.cpp src/cpp/drawing/shaders/StandardShader.h src/cpp/drawing/shapes/triangles/TrianglesProvider.cpp src/cpp/drawing/shapes/triangles/TrianglesProvider.h src/cpp/drawing/shapes/Shapes.cpp src/cpp/drawing/shapes/Shapes.h src/cpp/drawing/shaders/Shaders.cpp src/cpp/drawing/shaders/Shaders.h src/cpp/drawing/shapes/points/Sphere.cpp src/cpp/drawing/shapes/points/Sphere.h src/cpp/drawing/shapes/points/PointsProvider.cpp src/cpp/drawing/shapes/points/PointsProvider.h src/cpp/physics/opencl/dtos.h src/cpp/utils/opencl/Memory.cpp src/cpp/utils/opencl/Memory.h src/cpp/physics/Cell.cpp src/cpp/physics/Cell.h src/cpp/physics/rigidbody/RigidBody.cpp src/cpp/physics/rigidbody/RigidBody.h src/cpp/physics/rigidbody/Sphere.cpp src/cpp/physics/rigidbody/Sphere.h src/cpp/utils/opencl/conversions.h src/cpp/utils/opencl/conversions.cpp src/cpp/utils/opencl/Resources.cpp src/cpp/utils/opencl/Resources.h src/cpp/utils/opencl/MemoryManager.cpp src/cpp/utils/opencl/MemoryManager.h src/cpp/utils/opencl/CommandQueue.cpp src/cpp/utils/opencl/CommandQueue.h src/cpp/utils/opencl/ProgramFactory.cpp src/cpp/utils/opencl/ProgramFactory.h src/cpp/utils/opencl/KernelFactory.cpp src/cpp/utils/opencl/KernelFactory.h src/cpp/physics/Simulator.cpp src/cpp/physics/Simulator.h src/cpp/physics/samples/BenchmarkSample.cpp src/cpp/physics/samples/BenchmarkSample.h src/cpp/utils/alcubemath.h src/cpp/utils/alcubemath.cpp src/cpp/physics/samples/balls/BallsSample.cpp src/cpp/physics/samples/balls/BallsSample.h src/cpp/utils/app/Application.cpp src/cpp/utils/app/Application.h src/cpp/utils/app/OpenGLApplication.cpp src/cpp/utils/app/OpenGLApplication.h src/cpp/utils/Mouse.cpp src/cpp/utils/Mouse.h src/cpp/utils/Keyboard.cpp src/cpp/utils/Keyboard.h src/cpp/physics/samples/particles/ParticlesSample.cpp src/cpp/physics/samples/particles/ParticlesSample.h src/cpp/drawing/shapes/points/Particles.cpp src/cpp/drawing/shapes/points/Particles.h src/cpp/drawing/shaders/ParticleShader.cpp src/cpp/drawing/shaders/ParticleShader.h src/cpp/drawing/Material.cpp src/cpp/drawing/Material.h src/cpp/physics/samples/balls/ManyBallsSample.cpp src/cpp/physics/samples/balls/ManyBallsSample.h src/cpp/drawing/shaders/TileShader.cpp src/cpp/drawing/shaders/TileShader.h src/cpp/physics/Spring.cpp src/cpp/physics/Spring.h)
add_executable(alcube ${SOURCE_FILES})
add_library(alcube_lib STATIC ${SOURCE_FILES})

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

IF (APPLE)
    include_directories (/System/Library/Frameworks)
    find_library (GLUT_LIBRARY GLUT)
    find_library (OpenGL_LIBRARY OpenGL)
    find_library (GLM_LIBRARY NAMES GLM)
    mark_as_advanced (
            GLUT_LIBRARY
            OpenGL_LIBRARY
            GLM_LIBRARY)
    set (EXTRA_LIBS ${GLUT_LIBRARY} ${OpenGL_LIBRARY})
ENDIF (APPLE)

target_link_libraries(alcube ${EXTRA_LIBS})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wno-deprecated-declarations -framework OpenCL -lyaml-cpp")

add_executable(tests test/cpp/physics/SimulatorTest.cpp)
target_link_libraries(tests gtest_main alcube_lib)
add_test(NAME test COMMAND tests)
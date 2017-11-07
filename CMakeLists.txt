cmake_minimum_required(VERSION 3.8)
project(alcube)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES src/cpp/main.cpp src/cpp/utils/Profiler.cpp src/cpp/utils/Profiler.h src/cpp/drawing/Shader.cpp src/cpp/drawing/Shader.h src/cpp/drawing/Buffer.cpp src/cpp/drawing/Buffer.h src/cpp/drawing/Camera.cpp src/cpp/drawing/Camera.h src/cpp/drawing/Shape.cpp src/cpp/drawing/Shape.h src/cpp/drawing/Drawable.cpp src/cpp/drawing/Drawable.h src/cpp/drawing/shapes/triangles/Sphere.cpp src/cpp/drawing/shapes/triangles/Sphere.h src/cpp/drawing/Drawer.cpp src/cpp/drawing/Drawer.h src/cpp/utils/FileUtil.cpp src/cpp/utils/FileUtil.h src/cpp/drawing/samples/SimpleSphereSample.cpp src/cpp/drawing/samples/SimpleSphereSample.h src/cpp/drawing/Context.cpp src/cpp/drawing/Context.h src/cpp/drawing/shaders/SimpleShader.cpp src/cpp/drawing/shaders/SimpleShader.h src/cpp/drawing/shaders/StandardShader.cpp src/cpp/drawing/shaders/StandardShader.h src/cpp/drawing/shapes/triangles/TrianglesProvider.cpp src/cpp/drawing/shapes/triangles/TrianglesProvider.h src/cpp/drawing/shapes/Shapes.cpp src/cpp/drawing/shapes/Shapes.h src/cpp/drawing/shaders/Shaders.cpp src/cpp/drawing/shaders/Shaders.h src/cpp/drawing/shapes/points/Sphere.cpp src/cpp/drawing/shapes/points/Sphere.h src/cpp/drawing/shapes/points/PointsProvider.cpp src/cpp/drawing/shapes/points/PointsProvider.h src/cpp/physics/opencl/dtos.h src/cpp/utils/opencl/MemAllocator.cpp src/cpp/utils/opencl/MemAllocator.h)
add_executable(alcube ${SOURCE_FILES})

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

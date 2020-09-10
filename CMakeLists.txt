cmake_minimum_required(VERSION 3.0.0)
project(raytracer VERSION 0.1.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include(CTest)
enable_testing()

add_executable(raytracer main.cpp Vector.cpp Point.cpp Util.cpp Color.cpp Canvas.cpp Matrix.cpp Tuple.cpp Ray.cpp Sphere.cpp Intersection.cpp Object.cpp Material.cpp Light.cpp World.cpp Transformations.cpp Camera.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

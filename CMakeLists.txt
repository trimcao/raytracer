cmake_minimum_required(VERSION 3.0.0)
project(raytracer VERSION 0.1.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include(CTest)
enable_testing()

set(SOURCES
        main.cpp
        Vector.cpp
        Point.cpp
        Util.cpp
        Color.cpp
        Canvas.cpp
        Matrix.cpp
        Tuple.cpp
        Ray.cpp
        Object.cpp
        Sphere.cpp
        Material.cpp
        Light.cpp
        World.cpp
        Transformations.cpp
        Camera.cpp
        Plane.cpp
        Pattern.cpp
        Functions.cpp
        Cubes.cpp
        Cylinders.cpp
        Cones.cpp
        Groups.cpp
        Triangles.cpp
        ObjParser.cpp
        CSG.cpp
        )

set(HEADERS
        include/Vector.h
        include/Point.h
        include/Util.h
        include/Color.h
        include/Canvas.h
        include/Matrix.h
        include/Tuple.h
        include/Ray.h
        include/Object.h
        include/Sphere.h
        include/Material.h
        include/Light.h
        include/World.h
        include/Transformations.h
        include/Camera.h
        include/Plane.h
        include/Pattern.h
        include/Functions.h
        include/Cubes.h
        include/Cylinders.h
        include/Cones.h
        include/Groups.h
        include/Triangles.h
        include/ObjParser.h
        include/CSG.h
        include/Intersection.h
        )

add_executable(raytracer ${SOURCES} ${HEADERS})

target_include_directories(raytracer
        PRIVATE
        ${PROJECT_SOURCE_DIR}/include
        )

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

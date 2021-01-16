cmake_minimum_required(VERSION 3.14.0)
project(raycmd VERSION 0.1.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include(CTest)
enable_testing()


include(FetchContent)

# add yaml-cpp
FetchContent_Declare(yaml-cpp
        GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
        GIT_TAG yaml-cpp-0.6.3)

FetchContent_GetProperties(yaml-cpp)
if(NOT yaml-cpp_POPULATED)
    FetchContent_Populate(yaml-cpp)
    add_subdirectory(${yaml-cpp_SOURCE_DIR} ${yaml-cpp_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

get_filename_component(PARENT_DIR ../raytracer/ ABSOLUTE)

add_executable(raycmd main.cpp
                scene.cpp
                scene.h

                ${PARENT_DIR}/Vector.cpp
                ${PARENT_DIR}/Matrix.cpp
                ${PARENT_DIR}/Point.cpp
                ${PARENT_DIR}/Util.cpp
                ${PARENT_DIR}/Color.cpp
                ${PARENT_DIR}/Canvas.cpp
                ${PARENT_DIR}/Tuple.cpp
                ${PARENT_DIR}/Sphere.cpp
                ${PARENT_DIR}/Object.cpp
                ${PARENT_DIR}/Ray.cpp
                ${PARENT_DIR}/Light.cpp
                ${PARENT_DIR}/Material.cpp
                ${PARENT_DIR}/World.cpp
                ${PARENT_DIR}/Transformations.cpp
                ${PARENT_DIR}/Camera.cpp
                ${PARENT_DIR}/Plane.cpp
                ${PARENT_DIR}/Pattern.cpp
                ${PARENT_DIR}/Functions.cpp
                ${PARENT_DIR}/Cubes.cpp
                ${PARENT_DIR}/Cylinders.cpp
                ${PARENT_DIR}/Cones.cpp
                ${PARENT_DIR}/Groups.cpp
                ${PARENT_DIR}/Triangles.cpp
                ${PARENT_DIR}/ObjParser.cpp
                ${PARENT_DIR}/BoundingBoxes.cpp

                ${PARENT_DIR}/include/Vector.h
                ${PARENT_DIR}/include/Matrix.h
                ${PARENT_DIR}/include/Point.h
                ${PARENT_DIR}/include/Util.h
                ${PARENT_DIR}/include/Color.h
                ${PARENT_DIR}/include/Canvas.h
                ${PARENT_DIR}/include/Tuple.h
                ${PARENT_DIR}/include/Sphere.h
                ${PARENT_DIR}/include/Object.h
                ${PARENT_DIR}/include/Ray.h
                ${PARENT_DIR}/include/Light.h
                ${PARENT_DIR}/include/Material.h
                ${PARENT_DIR}/include/World.h
                ${PARENT_DIR}/include/Transformations.h
                ${PARENT_DIR}/include/Camera.h
                ${PARENT_DIR}/include/Plane.h
                ${PARENT_DIR}/include/Pattern.h
                ${PARENT_DIR}/include/Functions.h
                ${PARENT_DIR}/include/Cubes.h
                ${PARENT_DIR}/include/Cylinders.h
                ${PARENT_DIR}/include/Cones.h
                ${PARENT_DIR}/include/Groups.h
                ${PARENT_DIR}/include/Triangles.h
                ${PARENT_DIR}/include/ObjParser.h
                ${PARENT_DIR}/include/BoundingBoxes.h
                ${PARENT_DIR}/include/Intersection.h
                ${PARENT_DIR}/include/TRay.h
                ${PARENT_DIR}/threadpool/threadpool.h
)

target_include_directories(raycmd
        PRIVATE
        ${PROJECT_SOURCE_DIR}/../raytracer/include
        ${PROJECT_SOURCE_DIR}/../raytracer/threadpool
)

target_link_libraries(raycmd PRIVATE yaml-cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

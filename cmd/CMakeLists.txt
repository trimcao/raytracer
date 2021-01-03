cmake_minimum_required(VERSION 3.14.0)
project(raycmd VERSION 0.1.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include(CTest)
enable_testing()


include(FetchContent)

# add json
FetchContent_Declare(json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG v3.9.1)

FetchContent_GetProperties(json)
if(NOT json_POPULATED)
    FetchContent_Populate(json)
    add_subdirectory(${json_SOURCE_DIR} ${json_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

get_filename_component(PARENT_DIR ../raytracer/ ABSOLUTE)

add_executable(raycmd main.cpp
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
)

target_include_directories(raycmd
        PRIVATE
        ${PROJECT_SOURCE_DIR}/../raytracer/include
        ${PROJECT_SOURCE_DIR}/../raytracer/threadpool
)

target_link_libraries(raycmd PRIVATE nlohmann_json::nlohmann_json)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

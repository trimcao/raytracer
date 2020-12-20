cmake_minimum_required(VERSION 3.14.0)
project(raytracer VERSION 0.1.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include(CTest)
enable_testing()

# Download and unpack googletest at configure time
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
                 ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
                 EXCLUDE_FROM_ALL)

set(SOURCES
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
        BoundingBoxes.cpp
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
        include/BoundingBoxes.h
        )

set(TESTS
        test/Camera_Test.cpp
        test/BoundingBoxes_Test.cpp
        test/Groups_Test.cpp
        test/CSG_Test.cpp
        )

add_executable(raytracer ${SOURCES} ${HEADERS} ${TESTS})

target_include_directories(raytracer
        PRIVATE
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/threadpool
        )

add_subdirectory(threadpool)

# Now simply link against gtest or gtest_main as needed. Eg
target_link_libraries(raytracer gtest_main threadpool)
# add_test(NAME example_test COMMAND example)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

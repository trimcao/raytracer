cmake_minimum_required(VERSION 3.0.0)
project(projectile VERSION 0.1.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include(CTest)
enable_testing()

get_filename_component(PARENT_DIR ../../ ABSOLUTE)

add_executable(projectile main.cpp ${PARENT_DIR}/Matrix.cpp ${PARENT_DIR}/Vector.cpp ${PARENT_DIR}/Point.cpp ${PARENT_DIR}/Util.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

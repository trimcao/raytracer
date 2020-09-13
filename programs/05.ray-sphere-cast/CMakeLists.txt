cmake_minimum_required(VERSION 3.0.0)
project(canvas VERSION 0.1.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include(CTest)
enable_testing()

get_filename_component(PARENT_DIR ../../ ABSOLUTE)

add_executable(raysphere main.cpp ${PARENT_DIR}/Vector.cpp ${PARENT_DIR}/Matrix.cpp ${PARENT_DIR}/Point.cpp ${PARENT_DIR}/Util.cpp ${PARENT_DIR}/Color.cpp ${PARENT_DIR}/Canvas.cpp ${PARENT_DIR}/Tuple.cpp ${PARENT_DIR}/Sphere.cpp ${PARENT_DIR}/Object.cpp ${PARENT_DIR}/Ray.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

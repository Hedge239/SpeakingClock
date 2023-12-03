cmake_minimum_required(VERSION 3.12)
project(StressProject VERSION 0.1.0 LANGUAGES C CXX)

add_executable(StressProject main.cpp)

target_link_libraries(${PROJECT_NAME} sapi)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

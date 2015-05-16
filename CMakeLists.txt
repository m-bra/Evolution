cmake_minimum_required(VERSION 2.8)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR})

#set(CMAKE_CXX_COMPILER "clang")
set(CMAKE_CXX_FLAGS "--std=c++11 -ggdb -O0")

add_subdirectory(external/nnlib)
include_directories(${NNLIB_INCLUDE_PATHS} ${CMAKE_CURRENT_SOURCE_DIR}/src)

find_package(SFML COMPONENTS system window graphics REQUIRED)
include_directories(${SFML_INCLUDE_DIR})

file(GLOB EVOLUTION_SOURCE_FILES "src/*.cpp")

add_executable(evolution ${EVOLUTION_SOURCE_FILES})
target_link_libraries(evolution ${NNLIB_LIBRARIES} ${SFML_LIBRARIES})

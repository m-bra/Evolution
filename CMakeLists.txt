add_subdirectory(external/nnlib)
include_directories(${NNLIB_INCLUDE_PATHS} ${CMAKE_CURRENT_SOURCE_DIR}/src)
add_executable(evolution)
target_link_libraries(evolution ${NNLIB_LIBRARIES})

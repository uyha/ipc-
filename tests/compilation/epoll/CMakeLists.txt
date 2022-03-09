lpipp_add_executable(epoll-invalid-exclusive-combination invalid_exclusive_combination.cpp)
target_link_libraries(epoll-invalid-exclusive-combination PRIVATE lpipp::epoll tl::expected rt)

lpipp_add_executable(epoll-modify-exclusive modify_exclusive.cpp)
target_link_libraries(epoll-modify-exclusive PRIVATE lpipp::epoll tl::expected rt)
if (CMAKE_CXX_COMPILER_ID STREQUAL GNU)
  target_compile_options(epoll-modify-exclusive PRIVATE -fconcepts)
endif()


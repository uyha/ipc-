lpipp_add_executable(shm-invalid-mode-combination invalid_mode_combination.cpp)
target_link_libraries(shm-invalid-mode-combination PRIVATE lpipp::shm tl::expected)

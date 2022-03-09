lpipp_add_executable(mq-invalid-mode-combination invalid_mode_combination.cpp)
target_link_libraries(mq-invalid-mode-combination PRIVATE lpipp::mq tl::expected)

lpipp_add_executable(mq-copy-assignment-disabled copy_assignment_disabled.cpp)
target_link_libraries(mq-copy-assignment-disabled PRIVATE lpipp::mq tl::expected rt)

lpipp_add_executable(mq-copy-constructor-disabled copy_constructor_disabled.cpp)
target_link_libraries(mq-copy-constructor-disabled PRIVATE lpipp::mq tl::expected rt)


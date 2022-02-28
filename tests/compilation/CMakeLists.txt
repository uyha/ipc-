cmake_minimum_required(VERSION 3.12)
project(lpipp-test)

find_package(tl-expected)

add_library(lpipp:mq STATIC IMPORTED GLOBAL)
set_target_properties(lpipp:mq
  PROPERTIES
    IMPORTED_LOCATION "${IPCPP_MQ}"
    INTERFACE_INCLUDE_DIRECTORIES "${IPCPP_MQ_INCLUDE}"
)

add_executable(mq-invalid-mode-combination mq/invalid_mode_combination.cpp)
target_link_libraries(mq-invalid-mode-combination PRIVATE lpipp:mq tl::expected)
target_compile_features(mq-invalid-mode-combination PRIVATE cxx_std_20)

add_executable(mq-copy-assignment-disabled mq/copy_assignment_disabled.cpp)
target_link_libraries(mq-copy-assignment-disabled PRIVATE lpipp:mq tl::expected rt)
target_compile_features(mq-copy-assignment-disabled PRIVATE cxx_std_20)

add_executable(mq-copy-constructor-disabled mq/copy_constructor_disabled.cpp)
target_link_libraries(mq-copy-constructor-disabled PRIVATE lpipp:mq tl::expected rt)
target_compile_features(mq-copy-constructor-disabled PRIVATE cxx_std_20)


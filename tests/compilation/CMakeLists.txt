cmake_minimum_required(VERSION 3.12)
project(ipcpp-test)

find_package(tl-expected)

add_library(ipcpp-mq STATIC IMPORTED GLOBAL)
set_target_properties(ipcpp-mq
  PROPERTIES
    IMPORTED_LOCATION "${IPCPP_MQ}"
    INTERFACE_INCLUDE_DIRECTORIES "${IPCPP_MQ_INCLUDE}"
)

add_executable(mq-compilation mq.cpp)
target_link_libraries(mq-compilation PRIVATE ipcpp-mq tl::expected)
target_compile_features(mq-compilation PRIVATE cxx_std_20)

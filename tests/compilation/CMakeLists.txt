cmake_minimum_required(VERSION 3.12)
project(ipcpp-test)

find_package(tl-expected)

add_library(ipcpp::mp STATIC IMPORTED GLOBAL)
set_target_properties(ipcpp::mp
  PROPERTIES
    IMPORTED_LOCATION "${IPCPP_MQ}"
    INTERFACE_INCLUDE_DIRECTORIES "${IPCPP_MQ_INCLUDE}"
)

add_executable(mq-mode mq.cpp)
target_link_libraries(mq-mode PRIVATE ipcpp::mp tl::expected)
target_compile_features(mq-mode PRIVATE cxx_std_20)

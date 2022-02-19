include(build_options)
include(use_conan)

find_package(tl-expected REQUIRED)

add_library(ipcpp-mq mq.cpp)
add_library(ipcpp::mq ALIAS ipcpp-mq)
target_include_directories(ipcpp-mq PUBLIC
  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)
target_link_libraries(ipcpp-mq PUBLIC tl::expected rt)
target_compile_features(ipcpp-mq PRIVATE cxx_std_20)
target_compile_options(ipcpp-mq PRIVATE ${warnings} ${wae})

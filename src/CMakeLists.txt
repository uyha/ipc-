include(build_options)
include(use_conan)

find_package(tl-expected REQUIRED)

add_custom_command(OUTPUT "${CMAKE_BINARY_DIR}/include/ipcpp/sig_rt.hpp"
  COMMAND "${CMAKE_COMMAND}" -E echo make_directory "${CMAKE_BINARY_DIR}/include/ipcpp"
  COMMAND gensigrt "${CMAKE_BINARY_DIR}/include/ipcpp/sig_rt.hpp"
  DEPENDS gensigrt
)

add_library(ipcpp-mq
  mq.cpp
  "${CMAKE_BINARY_DIR}/include/ipcpp/sig_rt.hpp"
)
add_library(ipcpp::mq ALIAS ipcpp-mq)
target_include_directories(ipcpp-mq PUBLIC
  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
  $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/include>
  $<INSTALL_INTERFACE:include>
)
target_link_libraries(ipcpp-mq PUBLIC tl::expected rt)
target_compile_features(ipcpp-mq PRIVATE cxx_std_20)
target_compile_options(ipcpp-mq PRIVATE ${warnings} ${wae})

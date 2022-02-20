include(build_options)
include(use_conan)

find_package(tl-expected REQUIRED)

add_custom_command(OUTPUT "${CMAKE_BINARY_DIR}/include/lpipp/sig_rt.hpp"
  COMMAND "${CMAKE_COMMAND}" -E make_directory "${CMAKE_BINARY_DIR}/include/lpipp"
  COMMAND gensigrt "${CMAKE_BINARY_DIR}/include/lpipp/sig_rt.hpp"
  DEPENDS gensigrt
)

add_library(lpipp-mq
  mq.cpp
  "${CMAKE_BINARY_DIR}/include/lpipp/sig_rt.hpp"
)
add_library(lpipp::mq ALIAS lpipp-mq)
target_include_directories(lpipp-mq PUBLIC
  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
  $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/include>
  $<INSTALL_INTERFACE:include>
)
target_link_libraries(lpipp-mq PUBLIC tl::expected rt)
target_compile_features(lpipp-mq PRIVATE cxx_std_20)
target_compile_options(lpipp-mq PRIVATE ${warnings} ${wae})

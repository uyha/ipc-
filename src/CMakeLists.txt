include(build_options)
include(use_conan)

find_package(tl-expected REQUIRED)

add_library(lpipp-include INTERFACE)
add_library(lpipp::include ALIAS lpipp-include)
target_include_directories(lpipp-include INTERFACE
  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
  $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

function(lpipp_add_library name)
  add_library(${name} ${ARGN})
  target_link_libraries(${name} PUBLIC lpipp::include tl::expected)
  target_compile_features(${name} PRIVATE cxx_std_20)
  target_compile_options(${name} PRIVATE ${warnings} ${wae})
endfunction()

lpipp_add_library(lpipp-detail-file-descriptor file_descriptor.cpp)
add_library(lpipp::detail::file-descriptor ALIAS lpipp-detail-file-descriptor)

lpipp_add_library(lpipp-mq mq.cpp)
add_library(lpipp::mq ALIAS lpipp-mq)
target_link_libraries(lpipp-mq PRIVATE rt lpipp::detail::file-descriptor)

lpipp_add_library(lpipp-epoll epoll.cpp)
add_library(lpipp::epoll ALIAS lpipp-epoll)
target_link_libraries(lpipp-epoll PRIVATE rt lpipp::detail::file-descriptor)
target_compile_options(lpipp-epoll PUBLIC ${LPIPP_CONCEPT})

lpipp_add_library(lpipp-shm shm.cpp)
add_library(lpipp::shm ALIAS lpipp-shm)
target_link_libraries(lpipp-shm PRIVATE rt lpipp::detail::file-descriptor)

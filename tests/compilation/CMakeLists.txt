cmake_minimum_required(VERSION 3.12)
project(lpipp-test)

find_package(tl-expected)

foreach(var IN ITEMS LPIPP_INCLUDE LPIPP_MQ LPIPP_EPOLL)
  if(NOT DEFINED "${var}")
    list(APPEND undef_vars ${var})
  endif()
endforeach()
list(LENGTH undef_vars len)
if(len GREATER 0)
  list(JOIN undef_vars ", " undef_vars_str)
  message(FATAL_ERROR "There following variables are required to be defined: ${undef_vars_str}")
endif()

add_library(lpipp:mq STATIC IMPORTED GLOBAL)
set_target_properties(lpipp:mq
  PROPERTIES
    IMPORTED_LOCATION "${LPIPP_MQ}"
    INTERFACE_INCLUDE_DIRECTORIES "${LPIPP_INCLUDE}"
)

add_library(lpipp:epoll STATIC IMPORTED GLOBAL)
set_target_properties(lpipp:epoll
  PROPERTIES
    IMPORTED_LOCATION "${LPIPP_EPOLL}"
    INTERFACE_INCLUDE_DIRECTORIES "${LPIPP_INCLUDE}"
)

function(lpipp_add_executable name)
  add_executable("${name}" ${ARGN})
  target_compile_features("${name}" PRIVATE cxx_std_20)
endfunction()

lpipp_add_executable(mq-invalid-mode-combination mq/invalid_mode_combination.cpp)
target_link_libraries(mq-invalid-mode-combination PRIVATE lpipp:mq tl::expected)

lpipp_add_executable(mq-copy-assignment-disabled mq/copy_assignment_disabled.cpp)
target_link_libraries(mq-copy-assignment-disabled PRIVATE lpipp:mq tl::expected rt)

lpipp_add_executable(mq-copy-constructor-disabled mq/copy_constructor_disabled.cpp)
target_link_libraries(mq-copy-constructor-disabled PRIVATE lpipp:mq tl::expected rt)

lpipp_add_executable(epoll-invalid-exclusive-combination epoll/invalid_exclusive_combination.cpp)
target_link_libraries(epoll-invalid-exclusive-combination PRIVATE lpipp:epoll tl::expected rt)

lpipp_add_executable(epoll-modify-exclusive epoll/modify_exclusive.cpp)
target_link_libraries(epoll-modify-exclusive PRIVATE lpipp:epoll tl::expected rt)
if (CMAKE_CXX_COMPILER_ID STREQUAL GNU)
  target_compile_options(epoll-modify-exclusive PRIVATE -fconcepts)
endif()


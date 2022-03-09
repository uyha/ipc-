cmake_minimum_required(VERSION 3.12)
project(lpipp-test)

find_package(tl-expected)

foreach(var IN ITEMS LPIPP_INCLUDE LPIPP_MQ LPIPP_EPOLL LPIPP_SHM)
  if(NOT DEFINED "${var}")
    list(APPEND undef_vars ${var})
  endif()
endforeach()
list(LENGTH undef_vars len)
if(len GREATER 0)
  list(JOIN undef_vars ", " undef_vars_str)
  message(FATAL_ERROR "There following variables are required to be defined: ${undef_vars_str}")
endif()

add_library(lpipp::mq STATIC IMPORTED GLOBAL)
set_target_properties(lpipp::mq
  PROPERTIES
    IMPORTED_LOCATION "${LPIPP_MQ}"
    INTERFACE_INCLUDE_DIRECTORIES "${LPIPP_INCLUDE}"
)

add_library(lpipp::epoll STATIC IMPORTED GLOBAL)
set_target_properties(lpipp::epoll
  PROPERTIES
    IMPORTED_LOCATION "${LPIPP_EPOLL}"
    INTERFACE_INCLUDE_DIRECTORIES "${LPIPP_INCLUDE}"
)

add_library(lpipp::shm STATIC IMPORTED GLOBAL)
set_target_properties(lpipp::shm
  PROPERTIES
    IMPORTED_LOCATION "${LPIPP_SHM}"
    INTERFACE_INCLUDE_DIRECTORIES "${LPIPP_INCLUDE}"
)

function(lpipp_add_executable name)
  add_executable("${name}" ${ARGN})
  target_compile_features("${name}" PRIVATE cxx_std_20)
endfunction()

add_subdirectory(epoll)
add_subdirectory(mq)
add_subdirectory(shm)

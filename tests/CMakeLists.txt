cmake_policy(SET CMP0110 NEW)

include(use_conan)
include(build_options)

find_package(Catch2 REQUIRED)

include(CTest)
include(Catch)

if(NOT BUILD_TESTING)
  return()
endif()

add_library(lpipp-test-main STATIC EXCLUDE_FROM_ALL main.cpp)
add_library(lpipp::test::main ALIAS lpipp-test-main)
target_link_libraries(lpipp-test-main PUBLIC Catch2::Catch2)

function(lpipp_add_test name)
  add_executable(${name} EXCLUDE_FROM_ALL ${ARGN})
  target_link_libraries(${name} PRIVATE lpipp::test::main)
  target_compile_features(${name} PRIVATE cxx_std_20)
  target_compile_options(${name} PRIVATE ${warnings})
  catch_discover_tests(${name})
endfunction()

add_subdirectory(mq)
add_subdirectory(epoll)

lpipp_add_test(lpipp-chrono-test chrono.test.cpp)
target_link_libraries(lpipp-chrono-test PRIVATE lpipp::include)

add_test(NAME [[mq invalid mode combination]]
  COMMAND "${CMAKE_CTEST_COMMAND}"
          --build-and-test ${CMAKE_CURRENT_LIST_DIR}/compilation ${CMAKE_CURRENT_BINARY_DIR}/compilation
          --build-generator ${CMAKE_GENERATOR}
          --build-target mq-invalid-mode-combination
          --build-options "-DCMAKE_MODULE_PATH=${CMAKE_BINARY_DIR}/src"
                          "-DIPCPP_MQ=$<TARGET_FILE:lpipp::mq>"
                          "-DIPCPP_MQ_INCLUDE=${CMAKE_SOURCE_DIR}/include"
)
set_tests_properties([[mq invalid mode combination]]
  PROPERTIES
    PASS_REGULAR_EXPRESSION [[
no match for ‘operator|’ (operand types are ‘const lpipp::mq::OpenMode’ and ‘const lpipp::mq::OpenMode’)
]]
)

add_test(NAME [[mq copy assignment disabled]]
  COMMAND "${CMAKE_CTEST_COMMAND}"
          --build-and-test ${CMAKE_CURRENT_LIST_DIR}/compilation ${CMAKE_CURRENT_BINARY_DIR}/compilation
          --build-generator ${CMAKE_GENERATOR}
          --build-target mq-copy-assignment-disabled
          --build-options "-DCMAKE_MODULE_PATH=${CMAKE_BINARY_DIR}/src"
                          "-DIPCPP_MQ=$<TARGET_FILE:lpipp::mq>"
                          "-DIPCPP_MQ_INCLUDE=${CMAKE_SOURCE_DIR}/include"
)
set_tests_properties([[mq copy assignment disabled]]
  PROPERTIES
    PASS_REGULAR_EXPRESSION [[
use of deleted function ‘lpipp::mq::mq\(const lpipp::mq&\)’
]]
)

add_test(NAME [[mq copy constructor disabled]]
  COMMAND "${CMAKE_CTEST_COMMAND}"
          --build-and-test ${CMAKE_CURRENT_LIST_DIR}/compilation ${CMAKE_CURRENT_BINARY_DIR}/compilation
          --build-generator ${CMAKE_GENERATOR}
          --build-target mq-copy-constructor-disabled
          --build-options "-DCMAKE_MODULE_PATH=${CMAKE_BINARY_DIR}/src"
                          "-DIPCPP_MQ=$<TARGET_FILE:lpipp::mq>"
                          "-DIPCPP_MQ_INCLUDE=${CMAKE_SOURCE_DIR}/include"
)
set_tests_properties([[mq copy constructor disabled]]
  PROPERTIES
    PASS_REGULAR_EXPRESSION [[
use of deleted function ‘lpipp::mq& lpipp::mq::operator=\(const lpipp::mq&\)’
]]
)



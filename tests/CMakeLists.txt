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

macro(construct_test name)
    add_executable(${name} ${ARGN})
    target_link_libraries(${name} PRIVATE lpipp::test::main)
    target_compile_features(${name} PRIVATE cxx_std_20)
    target_compile_options(${name} PRIVATE ${warnings})
    catch_discover_tests(${name})
endmacro()

add_subdirectory(mq)

construct_test(lpipp-chrono-test)
target_sources(lpipp-chrono-test PRIVATE chrono.test.cpp)
target_link_libraries(lpipp-chrono-test PRIVATE lpipp::mq)

add_test(NAME [[mq invalid mode combination]]
  COMMAND "${CMAKE_CTEST_COMMAND}"
          --build-and-test ${CMAKE_CURRENT_LIST_DIR}/compilation ${CMAKE_CURRENT_BINARY_DIR}/compilation
          --build-generator ${CMAKE_GENERATOR}
          --build-target mq-mode
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



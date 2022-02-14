cmake_policy(SET CMP0110 NEW)

include(use_conan)
include(build_options)

find_package(Catch2 REQUIRED)
find_package(fmt)

include(CTest)
include(Catch)

add_library(ipcpp-test-main STATIC EXCLUDE_FROM_ALL main.cpp)
add_library(ipcpp::test::main ALIAS ipcpp-test-main)
target_link_libraries(ipcpp-test-main PUBLIC Catch2::Catch2)

macro(construct_test name)
    add_executable(${name} ${ARGN})
    target_link_libraries(${name} PRIVATE ipcpp::test::main fmt::fmt)
    target_compile_features(${name} PRIVATE cxx_std_20)
    target_compile_options(${name} PRIVATE ${warnings})
    catch_discover_tests(${name})
endmacro()

add_subdirectory(mq)

add_test(NAME [[ipcpp mode combination]]
  COMMAND "${CMAKE_CTEST_COMMAND}"
          --build-and-test ${CMAKE_CURRENT_LIST_DIR}/compilation ${CMAKE_CURRENT_BINARY_DIR}/compilation
          --build-generator ${CMAKE_GENERATOR}
          --build-target mq-mode
          --build-options "-DCMAKE_MODULE_PATH=${CMAKE_BINARY_DIR}/src"
                          "-DIPCPP_MQ=$<TARGET_FILE:ipcpp::mq>"
                          "-DIPCPP_MQ_INCLUDE=${CMAKE_SOURCE_DIR}/include"
)
set_tests_properties([[ipcpp mode combination]]
  PROPERTIES
    PASS_REGULAR_EXPRESSION [[
no match for ‘operator|’ (operand types are ‘const ipcpp::mq::OpenMode’ and ‘const ipcpp::mq::OpenMode’)
]]
)



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



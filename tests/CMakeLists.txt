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

add_custom_target(lpipp-tests)

function(lpipp_add_test name)
  add_executable(${name} EXCLUDE_FROM_ALL ${ARGN})
  target_link_libraries(${name} PRIVATE lpipp::test::main)
  target_compile_features(${name} PRIVATE cxx_std_20)
  target_compile_options(${name} PRIVATE ${warnings})
  catch_discover_tests(${name}
    PROPERTIES TIMEOUT 1
    )
  add_dependencies(lpipp-tests ${name})
endfunction()

add_subdirectory(mq)
add_subdirectory(epoll)
add_subdirectory(shm)

lpipp_add_test(lpipp-chrono-test chrono.test.cpp)
target_link_libraries(lpipp-chrono-test PRIVATE lpipp::include)

add_test(NAME [[configure compilation tests]]
  COMMAND "${CMAKE_COMMAND}"
            -B "${CMAKE_CURRENT_BINARY_DIR}/compilation"
            -S "${CMAKE_CURRENT_SOURCE_DIR}/compilation"
            -G "${CMAKE_GENERATOR}"
            "-DCMAKE_MODULE_PATH=${CMAKE_BINARY_DIR}/src"
            "-DLPIPP_INCLUDE=${CMAKE_SOURCE_DIR}/include"
            "-DLPIPP_MQ=$<TARGET_FILE:lpipp::mq>"
            "-DLPIPP_EPOLL=$<TARGET_FILE:lpipp::epoll>"
)
set_tests_properties([[configure compilation tests]] PROPERTIES FIXTURE_SETUP compilation_tests)

function(lpipp_add_compilation_test)
  cmake_parse_arguments("" "" "NAME;TARGET" "PROPERTIES" ${ARGN})
  add_test(NAME "${_NAME}"
    COMMAND "${CMAKE_COMMAND}"
              --build "${CMAKE_CURRENT_BINARY_DIR}/compilation"
              --target "${_TARGET}"
  )
  set_tests_properties("${_NAME}"
    PROPERTIES
      FIXTURE_REQUIRED compilation_tests
      DEPENDS [[configure compilation tests]]
      ${_PROPERTIES}
  )
endfunction()


lpipp_add_compilation_test(
  NAME [[mq invalid mode combination]]
  TARGET mq-invalid-mode-combination
  PROPERTIES
    PASS_REGULAR_EXPRESSION [[no match for ‘operator|’ \(.*OpenMode.*OpenMode.*\)]]
)
lpipp_add_compilation_test(
  NAME [[mq copy assignment disabled]]
  TARGET mq-copy-assignment-disabled
  PROPERTIES
    PASS_REGULAR_EXPRESSION [[use of deleted function ‘lpipp::mq::mq\(const lpipp::mq&\)’]]
)
lpipp_add_compilation_test(
  NAME [[mq copy constructor disabled]]
  TARGET "mq-copy-constructor-disabled"
  PROPERTIES
    PASS_REGULAR_EXPRESSION [[use of deleted function ‘lpipp::mq& lpipp::mq::operator=\(const lpipp::mq&\)’]]
)
lpipp_add_compilation_test(
  NAME [[epoll invalid exclusive combination]]
  TARGET "epoll-invalid-exclusive-combination"
  PROPERTIES
    PASS_REGULAR_EXPRESSION [[no match for ‘operator|’ \(.*OneShot.*ExclusiveEvent.*\)]]
)
lpipp_add_compilation_test(
  NAME [[epoll modify exclusive]]
  TARGET "epoll-modify-exclusive"
  PROPERTIES
    PASS_REGULAR_EXPRESSION [[no matching function for call to ‘lpipp::epoll::modify\(.*ExclusiveEvent.*\)’]]
)

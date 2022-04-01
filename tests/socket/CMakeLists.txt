lpipp_add_test(lpipp-socket-test
  option.test.cpp
)

target_link_libraries(lpipp-socket-test PRIVATE lpipp::un)

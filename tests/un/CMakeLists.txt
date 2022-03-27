lpipp_add_test( lpipp-un-test
  bind.test.cpp
  create.test.cpp
  socket_type.test.cpp
)

target_link_libraries(lpipp-un-test PRIVATE lpipp::un)

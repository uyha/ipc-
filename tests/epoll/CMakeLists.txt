lpipp_add_test(lpipp-epoll-test
  create.test.cpp
  add.test.cpp
)
target_link_libraries(lpipp-epoll-test PRIVATE lpipp::epoll)

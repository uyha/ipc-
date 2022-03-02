lpipp_add_test(lpipp-epoll-test
  add.test.cpp
  create.test.cpp
  event.test.cpp
  modify.test.cpp
  remove.test.cpp
)
target_link_libraries(lpipp-epoll-test PRIVATE lpipp::epoll lpipp::mq)

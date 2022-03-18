lpipp_add_test(lpipp-fcntl-test
  duplicate.test.cpp
  stat.test.cpp
)
target_link_libraries(lpipp-fcntl-test PRIVATE lpipp::epoll lpipp::mq lpipp::shm)

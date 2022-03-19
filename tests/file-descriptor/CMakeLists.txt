lpipp_add_test(lpipp-file-descriptor-test
  duplicate.test.cpp
  stat.test.cpp
)
target_link_libraries(lpipp-file-descriptor-test PRIVATE lpipp::epoll lpipp::mq lpipp::shm)

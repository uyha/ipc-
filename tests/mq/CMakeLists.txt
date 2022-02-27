find_package(Threads REQUIRED)

lpipp_add_test(lpipp-mq-test
  attribute.test.cpp
  mode.test.cpp
  notify.test.cpp
  open.test.cpp
  send_receive.test.cpp
  timed_send_receive.test.cpp
  unlink.test.cpp
  move.test.cpp
)
target_link_libraries(lpipp-mq-test PRIVATE lpipp::mq Threads::Threads)

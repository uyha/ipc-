find_package(Threads REQUIRED)

construct_test(lpipp-mq-test)
target_sources(lpipp-mq-test PRIVATE
  attribute.test.cpp
  mode.test.cpp
  notify.test.cpp
  open.test.cpp
  send_receive.test.cpp
  timed_send_receive.test.cpp
  unlink.test.cpp
)
target_link_libraries(lpipp-mq-test PRIVATE lpipp::mq Threads::Threads)

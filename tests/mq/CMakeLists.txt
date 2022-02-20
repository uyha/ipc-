find_package(Threads REQUIRED)

construct_test(ipcpp-mq-test)
target_sources(ipcpp-mq-test PRIVATE
  attribute.test.cpp
  mode.test.cpp
  notify.test.cpp
  open.test.cpp
  send_receive.test.cpp
  timed_send_receive.test.cpp
  unlink.test.cpp
)
target_link_libraries(ipcpp-mq-test PRIVATE ipcpp::mq Threads::Threads)

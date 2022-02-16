construct_test(ipcpp-mq-test)
target_sources(ipcpp-mq-test PRIVATE
  open.test.cpp
  mode.test.cpp
  unlink.test.cpp
  attribute.test.cpp
  send_receive.test.cpp
  timed_send_receive.test.cpp
)
target_link_libraries(ipcpp-mq-test PRIVATE ipcpp::mq)

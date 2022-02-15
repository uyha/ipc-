construct_test(ipcpp-mq-test)
target_sources(ipcpp-mq-test PRIVATE open.test.cpp mode.test.cpp unlink.test.cpp)
target_link_libraries(ipcpp-mq-test PRIVATE ipcpp::mq)

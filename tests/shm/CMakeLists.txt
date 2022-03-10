find_package(Threads REQUIRED)

lpipp_add_test(lpipp-shm-test
  mode.test.cpp
  unlink.test.cpp
)
target_link_libraries(lpipp-shm-test PRIVATE lpipp::shm)

#include <catch2/catch.hpp>
#include <iostream>
#include <ipcpp/mq.hpp>
#include <thread>
#include <unistd.h>

using namespace ipcpp;
using namespace mq_constants;
using namespace std::chrono_literals;

bool mq_thread_callback_called = false;

TEST_CASE("register notification of a message queue") {
  auto name   = "/notification";
  auto queue  = mq::open(name, create | read_write, 0666, {.max_messages = 1, .max_message_size = 1});
  auto buffer = std::array<char, 1>{};
  REQUIRE(queue);

  SECTION("register then unregister") {
    CHECK(queue->register_notification());
    CHECK(queue->unregister_notification());
  }

  SECTION("register twice should create an error") {
    auto pid = ::fork();
    if (pid == 0) {
      (void)queue->register_notification();
      std::this_thread::sleep_for(50ms);
      std::exit(0);
    }
    std::this_thread::sleep_for(10ms);
    REQUIRE_FALSE(queue->register_notification());
    CHECK(queue->register_notification().error() == mq::NotiRegError::registration_existed);
  }

  SECTION("register with thread callback") {
    auto sending_thread   = std::thread([&] {
      std::this_thread::sleep_for(10ms);
      (void)queue->send(name, 1);
    });
    auto const attributes = queue->get_attributes();
    REQUIRE(attributes.mq_curmsgs == 0);
    REQUIRE(queue->register_notification([](::sigval) { mq_thread_callback_called = true; }));
    std::this_thread::sleep_for(20ms);
    auto result = queue->receive(std::data(buffer), std::size(buffer));
    REQUIRE(result);
    CHECK(std::get<0>(buffer) == '/');
    CHECK(mq_thread_callback_called);
    sending_thread.join();
  }

  REQUIRE(mq::unlink(name));
}

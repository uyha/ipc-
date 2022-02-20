#include <catch2/catch.hpp>
#include <iostream>
#include <ipcpp/mq.hpp>
#include <thread>
#include <unistd.h>

using namespace ipcpp;
using namespace mq_constants;
using namespace std::chrono_literals;

bool mq_thread_callback_called   = false;
int mq_thread_callback_int       = 0;
void *mq_thread_callback_pointer = nullptr;

TEST_CASE("register notification of a message queue") {
  auto name                  = "/notification";
  auto queue                 = mq::open(name, create | read_write, 0666, {.max_messages = 1, .max_message_size = 1});
  mq_thread_callback_called  = false;
  mq_thread_callback_int     = 0;
  mq_thread_callback_pointer = nullptr;

  REQUIRE(queue);

  SECTION("register then unregister") {
    CHECK(queue->notify());
    CHECK(queue->unnotify());
  }

  SECTION("register twice should create an error") {
    auto pid = ::fork();
    if (pid == 0) {
      (void)queue->notify();
      std::this_thread::sleep_for(50ms);
      std::exit(0);
    }
    std::this_thread::sleep_for(10ms);
    REQUIRE_FALSE(queue->notify());
    CHECK(queue->notify().error() == mq::NotifyError::registration_existed);
  }

  SECTION("register with thread callback") {
    auto sending_thread   = std::thread([&] {
      std::this_thread::sleep_for(10ms);
      (void)queue->send(name, 1);
    });
    auto const attributes = queue->get_attributes();
    REQUIRE(attributes.mq_curmsgs == 0);
    REQUIRE(queue->notify([](::sigval) { mq_thread_callback_called = true; }));
    std::this_thread::sleep_for(20ms);
    CHECK(mq_thread_callback_called);
    sending_thread.join();
  }
  SECTION("register with thread callback and having value in it") {
    auto sending_thread   = std::thread([&] {
      std::this_thread::sleep_for(10ms);
      (void)queue->send(name, 1);
    });
    auto const attributes = queue->get_attributes();
    REQUIRE(attributes.mq_curmsgs == 0);
    REQUIRE(queue->notify([](::sigval val) { mq_thread_callback_int = val.sival_int; }, 1));
    std::this_thread::sleep_for(20ms);
    CHECK(mq_thread_callback_int == 1);
    sending_thread.join();
  }
  SECTION("register with thread callback and having pointer in it") {
    auto sending_thread   = std::thread([&] {
      std::this_thread::sleep_for(10ms);
      (void)queue->send(name, 1);
    });
    auto const attributes = queue->get_attributes();
    REQUIRE(attributes.mq_curmsgs == 0);
    REQUIRE(
        queue->notify([](::sigval val) { mq_thread_callback_pointer = val.sival_ptr; }, (void *)0xFF));
    std::this_thread::sleep_for(20ms);
    CHECK(mq_thread_callback_pointer == (void *)0xFF);
    sending_thread.join();
  }

  REQUIRE(mq::unlink(name));
}

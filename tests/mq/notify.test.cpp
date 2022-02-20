#include <catch2/catch.hpp>
#include <csignal>
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
bool mq_signal_callback_called   = false;

TEST_CASE("register notification of a message queue") {
  auto name                  = "/notification";
  auto queue                 = mq::open(name, create | read_write, 0666, {.max_messages = 1, .max_message_size = 1});
  mq_thread_callback_called  = false;
  mq_thread_callback_int     = 0;
  mq_thread_callback_pointer = nullptr;
  mq_signal_callback_called  = false;

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
    auto const attributes = queue->get_attributes();
    REQUIRE(attributes.mq_curmsgs == 0);
    REQUIRE(queue->notify([](::sigval) { mq_thread_callback_called = true; }));
    CHECK(queue->send(name, 1));
    std::this_thread::sleep_for(1ms);
    CHECK(mq_thread_callback_called);
  }
  SECTION("register with thread callback and having value in it") {
    auto const attributes = queue->get_attributes();
    REQUIRE(attributes.mq_curmsgs == 0);
    REQUIRE(queue->notify([](::sigval val) { mq_thread_callback_int = val.sival_int; }, 1));
    CHECK(queue->send(name, 1));
    std::this_thread::sleep_for(1ms);
    CHECK(mq_thread_callback_int == 1);
  }
  SECTION("register with thread callback and having pointer in it") {
    auto const attributes = queue->get_attributes();
    REQUIRE(attributes.mq_curmsgs == 0);
    REQUIRE(queue->notify([](::sigval val) { mq_thread_callback_pointer = val.sival_ptr; }, (void *)0xFF));
    CHECK(queue->send(name, 1));
    std::this_thread::sleep_for(1ms);
    CHECK(mq_thread_callback_pointer == (void *)0xFF);
  }
  SECTION("register with invalid signal should fail") {
    auto result = queue->notify(-1);
    REQUIRE_FALSE(result);
    REQUIRE(result.error() == mq::SignalNotifyError::signal_invalid);
  }
  SECTION("register and wait for signal to come") {
    std::signal(SIGUSR1, [](int) { mq_signal_callback_called = true; });
    REQUIRE(queue->notify(SIGUSR1));
    REQUIRE(queue->send(name, 1));
    REQUIRE(mq_signal_callback_called);
  }

  REQUIRE(mq::unlink(name));
}

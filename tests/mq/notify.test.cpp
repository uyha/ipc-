#include <catch2/catch.hpp>
#include <csignal>
#include <iostream>
#include <lpipp/mq.hpp>
#include <thread>
#include <unistd.h>

using namespace lpipp;
using namespace mq_constants;
using namespace std::chrono_literals;

bool mq_thread_callback_called   = false;
int mq_thread_callback_int       = 0;
void *mq_thread_callback_pointer = nullptr;
bool mq_signal_callback_called   = false;

#define QUOTE(s) #s
#define NAME_IMPL(base, line) base QUOTE(line)
#define NAME NAME_IMPL("/mq.notify.test", __LINE__)

TEST_CASE("register notification of a message queue") {
  mq_thread_callback_called  = false;
  mq_thread_callback_int     = 0;
  mq_thread_callback_pointer = nullptr;
  mq_signal_callback_called  = false;

  SECTION("register then unregister") {
    auto name  = NAME;
    auto queue = mq::open(name, create | read_write, 0666, {.max_messages = 1, .max_message_size = 1});
    REQUIRE(queue);
    CHECK(queue->notify());
    CHECK(queue->unnotify());
    CHECK(mq::unlink(name));
  }

  SECTION("register twice should create an error") {
    auto name  = NAME;
    auto queue = mq::open(name, create | read_write, 0666, {.max_messages = 1, .max_message_size = 1});
    REQUIRE(queue);
    auto pid = ::fork();
    if (pid == 0) {
      (void)queue->notify();
      std::this_thread::sleep_for(50ms);
      std::exit(0);
    }
    std::this_thread::sleep_for(10ms);
    REQUIRE_FALSE(queue->notify());
    CHECK(queue->notify().error() == mq::NotifyError::registration_existed);
    CHECK(mq::unlink(name));
  }

  SECTION("register with thread callback") {
    auto name  = NAME;
    auto queue = mq::open(name, create | read_write, 0666, {.max_messages = 1, .max_message_size = 1});
    auto const attributes = queue->get_attributes();
    REQUIRE(attributes.mq_curmsgs == 0);
    REQUIRE(queue->notify([](::sigval) { mq_thread_callback_called = true; }));
    CHECK(queue->send(name, 1));
    std::this_thread::sleep_for(1ms);
    CHECK(mq_thread_callback_called);
    CHECK(mq::unlink(name));
  }
  SECTION("register with thread callback and having value in it") {
    auto name  = NAME;
    auto queue = mq::open(name, create | read_write, 0666, {.max_messages = 1, .max_message_size = 1});
    auto const attributes = queue->get_attributes();
    REQUIRE(attributes.mq_curmsgs == 0);
    REQUIRE(queue->notify([](::sigval val) { mq_thread_callback_int = val.sival_int; }, 1));
    CHECK(queue->send(name, 1));
    std::this_thread::sleep_for(1ms);
    CHECK(mq_thread_callback_int == 1);
    CHECK(mq::unlink(name));
  }
  SECTION("register with thread callback and having pointer in it") {
    auto name  = NAME;
    auto queue = mq::open(name, create | read_write, 0666, {.max_messages = 1, .max_message_size = 1});
    auto const attributes = queue->get_attributes();
    REQUIRE(attributes.mq_curmsgs == 0);
    REQUIRE(queue->notify([](::sigval val) { mq_thread_callback_pointer = val.sival_ptr; }, (void *)0xFF));
    CHECK(queue->send(name, 1));
    std::this_thread::sleep_for(1ms);
    CHECK(mq_thread_callback_pointer == (void *)0xFF);
    CHECK(mq::unlink(name));
  }
  SECTION("register with invalid signal should fail") {
    auto name  = NAME;
    auto queue = mq::open(name, create | read_write, 0666, {.max_messages = 1, .max_message_size = 1});
    auto result = queue->notify(-1);
    REQUIRE_FALSE(result);
    REQUIRE(result.error() == mq::SignalNotifyError::signal_invalid);
    CHECK(mq::unlink(name));
  }
  SECTION("register and wait for signal to come") {
    auto name  = NAME;
    auto queue = mq::open(name, create | read_write, 0666, {.max_messages = 1, .max_message_size = 1});
    std::signal(SIGUSR1, [](int) { mq_signal_callback_called = true; });
    REQUIRE(queue->notify(SIGUSR1));
    REQUIRE(queue->send(name, 1));
    REQUIRE(mq_signal_callback_called);
    CHECK(mq::unlink(name));
  }
}

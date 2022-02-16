#define CATCH_CONFIG_ENABLE_CHRONO_STRINGMAKER

#include <catch2/catch.hpp>
#include <ipcpp/mq.hpp>

using namespace ipcpp;
using namespace mq_constants;
using namespace std::chrono_literals;

TEST_CASE("timed sending nonblocking message queue") {
  auto name  = "/time-send-nonblock";
  auto queue = mq::open(name, write_only | create | nonblock, 0666, {.max_messages = 1, .max_message_size = 1});
  REQUIRE(queue);
  {
    auto result = queue->send(name, 1, 2s);
    CHECK(result);
  }
  {
    auto result = queue->send(name, 1, 2s);
    CHECK_FALSE(result);
    CHECK(result.error() == mq::TimedSendError::queue_full);
  }

  mq::unlink(name);
}

TEST_CASE("timed sending blocking message queue") {
  auto name  = "/time-send";
  auto queue = mq::open(name, write_only | create, 0666, {.max_messages = 1, .max_message_size = 1});
  REQUIRE(queue);
  {
    auto result = queue->send(name, 1, 2s);
    CHECK(result);
  }
  {
    auto send_time    = std::chrono::steady_clock::now();
    auto result       = queue->send(name, 1, 0.1005s);
    auto timeout_time = std::chrono::steady_clock::now();
    CHECK_FALSE(result);
    CHECK(result.error() == mq::TimedSendError::timedout);
    CHECK(timeout_time - send_time >= 0.1005s);
  }

  mq::unlink(name);
}

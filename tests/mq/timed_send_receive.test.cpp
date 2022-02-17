#define CATCH_CONFIG_ENABLE_CHRONO_STRINGMAKER

#include <catch2/catch.hpp>
#include <ipcpp/mq.hpp>
#include <thread>

using namespace ipcpp;
using namespace mq_constants;
using namespace std::chrono;
using namespace std::chrono_literals;

TEST_CASE("timed sending nonblocking message queue") {
  auto name  = "/mq.timed-send-nonblock";
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
  auto name  = "/mq.timed-send-block";
  auto queue = mq::open(name, write_only | create, 0666, {.max_messages = 1, .max_message_size = 1});
  REQUIRE(queue);
  {
    auto result = queue->send(name, 1, 2s);
    CHECK(result);
  }
  {
    auto send_time    = steady_clock::now();
    auto result       = queue->send(name, 1, 0.1005s);
    auto timeout_time = steady_clock::now();
    CHECK_FALSE(result);
    CHECK(result.error() == mq::TimedSendError::timedout);
    CHECK(timeout_time - send_time >= 0.1005s);
  }
  {
    auto result = queue->send(name, 1, -(system_clock::now().time_since_epoch() + 1s));
    CHECK_FALSE(result);
    CHECK(result.error() == mq::TimedSendError::timeout_invalid);
  }

  mq::unlink(name);
}

TEST_CASE("timed receiving nonblocking message queue") {
  auto name   = "/mq.timed-receive-nonblock";
  auto queue  = mq::open(name, read_only | create | nonblock, 0666, {.max_messages = 1, .max_message_size = 1});
  auto result = queue->receive(nullptr, 1, 2s);
  CHECK_FALSE(result);
  CHECK(result.error() == mq::TimedReceiveError::queue_empty);
  mq::unlink(name);
}

TEST_CASE("timed receiving blocking message queue") {
  auto name  = "/mq.timed-receive-block";
  auto queue = mq::open(name, read_only | create, 0666, {.max_messages = 1, .max_message_size = 1});
  REQUIRE(queue);
  {
    auto result = queue->receive(nullptr, 1, 0.1s);
    CHECK_FALSE(result);
    CHECK(result.error() == mq::TimedReceiveError::timedout);
  }
  {
    auto start  = steady_clock::now();
    auto result = queue->receive(nullptr, 1, 0.1005s);
    auto end    = steady_clock::now();
    CHECK_FALSE(result);
    CHECK(result.error() == mq::TimedReceiveError::timedout);
    CHECK(end - start >= 0.1005s);
  }
  {
    auto result = queue->receive(nullptr, 1, -(system_clock::now().time_since_epoch() + 1s));
    CHECK_FALSE(result);
    CHECK(result.error() == mq::TimedReceiveError::timeout_invalid);
  }
  mq::unlink(name);
}

TEST_CASE("timed sending and receiving") {
  auto name   = "/mq.timed-sending-receiving-block";
  auto queue  = mq::open(name, create | read_write, 0666, {.max_messages = 1, .max_message_size = 1});
  auto buffer = std::array<char, 1>{};
  REQUIRE(queue);
  {
    auto send_thread = std::thread{[&] {
      std::this_thread::sleep_for(0.1s);
      queue->send(name, 1);
    }};
    {
      auto result = queue->receive(std::data(buffer), std::size(buffer), 0.2s);
      CHECK(result);
      CHECK(std::get<0>(buffer) == '/');
    }
    send_thread.join();
  }
  {
    auto receive_thread = std::thread{[&] {
      std::this_thread::sleep_for(0.1s);
      queue->receive(std::data(buffer), std::size(buffer));
    }};
    {
      queue->send(name, 1);
      auto result = queue->send(name, 1, 0.2s);
      CHECK(result);
    }
    receive_thread.join();
  }
  REQUIRE(mq::unlink(name));
}

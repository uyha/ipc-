#include <catch2/catch.hpp>
#include <lpipp/mq.hpp>

using namespace lpipp;
using namespace mq_constants;

TEST_CASE("sending and reading attributes") {
  SECTION("blocking then nonblocking") {
    auto name  = "/send";
    auto queue = mq::open(name, Create | WriteOnly, 0666, {.max_messages = 1, .max_message_size = 8});
    REQUIRE(queue);
    CHECK(queue->send(name, 0));
    {
      auto attributes = queue->get_attributes();
      CHECK(attributes.mq_curmsgs == 1);
    }
    {
      (void)queue->set_nonblock();
      auto result = queue->send(name, 0);
      CHECK_FALSE(result);
      CHECK(result.error() == mq::SendError::queue_full);
    }
    CHECK(mq::unlink(name));
  }
  SECTION("nonblocking") {
    auto name  = "/send";
    auto queue = mq::open(name, Create | WriteOnly | Nonblock, 0666, {.max_messages = 1, .max_message_size = 8});
    REQUIRE(queue);
    CHECK(queue->send(name, 0));
    {
      auto attributes = queue->get_attributes();
      CHECK(attributes.mq_curmsgs == 1);
    }
    {
      auto result = queue->send(name, 0);
      CHECK_FALSE(result);
      CHECK(result.error() == mq::SendError::queue_full);
      auto attributes = queue->get_attributes();
      CHECK(attributes.mq_curmsgs == 1);
    }
    CHECK(mq::unlink(name));
  }
}
TEST_CASE("reading empty nonblocking queue") {
  auto name  = "/receive";
  auto queue = mq::open(name, Create | ReadOnly | Nonblock, 0666, {.max_messages = 1, .max_message_size = 1});
  REQUIRE(queue);
  auto result = queue->receive(nullptr, 1);
  CHECK_FALSE(result);
  CHECK(result.error() == mq::ReceiveError::queue_empty);

  CHECK(mq::unlink(name));
}
TEST_CASE("sending and reading from the same queue") {
  auto name     = "/send-receive";
  auto buffer   = std::array<char, 1>{'\0'};
  auto queue    = mq::open(name, Create | ReadWrite, 0666, {.max_messages = 2, .max_message_size = 1});
  auto priority = 1000u;
  REQUIRE(queue);
  SECTION("send then receive") {
    auto send_result = queue->send(name, 1);
    CHECK(send_result);
    auto receive_result = queue->receive(std::data(buffer), std::size(buffer), &priority);
    CHECK(receive_result);
    CHECK(priority == 0);
    CHECK(std::get<0>(buffer) == '/');
  }
  SECTION("send then receive with difference priority") {
    {
      auto send_result = queue->send(name, 1);
      CHECK(send_result);
    }
    {
      auto send_result = queue->send(name + 1, 1, 1);
      CHECK(send_result);
    }
    {
      auto receive_result = queue->receive(std::data(buffer), std::size(buffer), &priority);
      CHECK(receive_result);
      CHECK(priority == 1);
      CHECK(std::get<0>(buffer) == 's');
    }
    {
      auto receive_result = queue->receive(std::data(buffer), std::size(buffer), &priority);
      CHECK(receive_result);
      CHECK(priority == 0);
      CHECK(std::get<0>(buffer) == '/');
    }
  }

  CHECK(mq::unlink(name));
}
TEST_CASE("sending/receiving from read/write only queues") {
  SECTION("send to readonly queue") {
    auto name  = "/mq.send-readonly-queue";
    auto queue = mq::open(name, Create | ReadOnly, 0666);
    REQUIRE(queue);
    auto result = queue->send(name, 1);
    CHECK_FALSE(result);
    CHECK(result.error() == mq::SendError::queue_read_only);
    CHECK(mq::unlink(name));
  }
  SECTION("read from writeonly queue") {
    auto name  = "/mq.read-writeonly-queue";
    auto queue = mq::open(name, Create | WriteOnly, 0666);
    REQUIRE(queue);
    auto result = queue->receive(nullptr, 1);
    CHECK_FALSE(result);
    CHECK(result.error() == mq::ReceiveError::queue_write_only);
    CHECK(mq::unlink(name));
  }
}
TEST_CASE("duplicate then send and receive") {
  SECTION("send from original and receive from duplicated") {
    auto name           = "/mq.dup.send-receive";
    auto buffer         = std::array<char, 5>{};
    auto original_queue = mq::open(name, Create | ReadWrite, 0666, {.max_messages = 1, .max_message_size = 1});
    REQUIRE(original_queue);
    auto dup_queue = original_queue->duplicate();
    REQUIRE(dup_queue);
    REQUIRE(original_queue->send("s", 1));
    auto result = dup_queue->receive(std::data(buffer), std::size(buffer));
    CHECK(result);
    CHECK(std::get<0>(buffer) == 's');

    CHECK(mq::unlink(name));
  }
  SECTION("send from original and receive from duplicated and check file descriptor") {
    auto name           = "/mq.dup-at-least.send-receive";
    auto buffer         = std::array<char, 5>{};
    auto original_queue = mq::open(name, Create | ReadWrite, 0666, {.max_messages = 1, .max_message_size = 1});
    REQUIRE(original_queue);
    auto dup_queue = original_queue->duplicate_at_least(10);
    REQUIRE(dup_queue);
    CHECK(dup_queue->get_handle() >= 10);
    REQUIRE(original_queue->send("s", 1));
    auto result = dup_queue->receive(std::data(buffer), std::size(buffer));
    CHECK(result);
    CHECK(std::get<0>(buffer) == 's');

    CHECK(mq::unlink(name));
  }
}

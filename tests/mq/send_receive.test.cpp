#include <catch2/catch.hpp>
#include <ipcpp/mq.hpp>

using namespace ipcpp;
using namespace mq_constants;

TEST_CASE("sending and reading attributes") {
  SECTION("blocking then nonblocking") {
    auto name  = "/send";
    auto queue = mq::open(name, create | write_only, 0666, {.max_messages = 1, .max_message_size = 8});
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
    auto queue = mq::open(name, create | write_only | nonblock, 0666, {.max_messages = 1, .max_message_size = 8});
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
  auto queue = mq::open(name, create | read_only | nonblock, 0666, {.max_messages = 1, .max_message_size = 1});
  REQUIRE(queue);
  auto result = queue->receive(nullptr, 1);
  CHECK_FALSE(result);
  CHECK(result.error() == mq::ReceiveError::queue_empty);

  CHECK(mq::unlink(name));
}
TEST_CASE("sending and reading from the same queue") {
  auto name     = "/send-receive";
  auto buffer   = std::array<char, 1>{'\0'};
  auto queue    = mq::open(name, create | read_write, 0666, {.max_messages = 2, .max_message_size = 1});
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

#include <catch2/catch.hpp>
#include <lpipp/epoll.hpp>
#include <lpipp/mq.hpp>
#include <string_view>
#include <thread>

using namespace lpipp;
using namespace epoll_constants;

TEST_CASE("wait on epoll") {
  using namespace std::chrono_literals;

  auto queue_name = "/epoll.wait";
  (void)mq::unlink(queue_name);
  auto epoll_instance = epoll::create();
  auto queue          = mq::open(queue_name,
                        mq_constants::read_write | mq_constants::create,
                        0666,
                        {.max_messages = 1, .max_message_size = 1});
  auto buffer         = std::array<::epoll_event, 1>{};
  auto receive_buffer = std::array<char, 1>{};
  REQUIRE(epoll_instance);
  REQUIRE(queue);
  REQUIRE(epoll_instance->add(queue->get_handle(), In, queue->get_handle()));
  REQUIRE(queue->get_attributes().mq_curmsgs == 0);
  SECTION("wait and receive") {
    auto send_result = queue->send(queue_name, 1);
    REQUIRE(send_result);
    REQUIRE(queue->get_attributes().mq_curmsgs == 1);
    auto available = epoll_instance->wait(std::data(buffer), std::size(buffer), 1ms);
    REQUIRE(available);
    REQUIRE(*available == 1);
    CHECK(buffer[0].data.fd == queue->get_handle());
    auto receive_result = queue->receive(std::data(receive_buffer), std::size(receive_buffer), 1ms);
    REQUIRE(receive_result);
    CHECK(receive_buffer[0] == '/');
    CHECK(queue->get_attributes().mq_curmsgs == 0);
  }
  SECTION("invalid size should fail") {
    auto available = epoll_instance->wait(nullptr, 0);
    REQUIRE_FALSE(available);
    CHECK(available.error() == epoll::WaitError::max_events_invalid);
  }
  SECTION("invalid buffer should fail") {
    auto send_result = queue->send(queue_name, 1);
    REQUIRE(send_result);
    REQUIRE(queue->get_attributes().mq_curmsgs == 1);
    auto available = epoll_instance->wait(nullptr, 1);
    REQUIRE_FALSE(available);
    CHECK(available.error() == epoll::WaitError::events_buffer_not_accessible);
  }
  SECTION("interrupted with timeout") {
    using namespace std::chrono_literals;
    auto available = epoll_instance->wait(std::data(buffer), std::size(buffer), 1ms);
    REQUIRE_FALSE(available);
    CHECK(available.error() == epoll::WaitError::timeout);
  }

  CHECK(mq::unlink(queue_name));
}

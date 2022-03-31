#include <catch2/catch.hpp>
#include <lpipp/epoll.hpp>
#include <lpipp/mq.hpp>

using namespace lpipp;
using namespace epoll_constants;

TEST_CASE("modify epoll") {
  auto queue_name     = "/epoll.add";
  auto epoll_instance = epoll::create();
  auto queue          = mq::open(queue_name, mq_constants::ReadOnly | mq_constants::Create, 0666);
  REQUIRE(epoll_instance);
  REQUIRE(queue);
  SECTION("add then modify") {
    auto add_result = epoll_instance->add(queue->get_handle(), In, 0);
    REQUIRE(add_result);
    auto modify_result = epoll_instance->modify(queue->get_handle(), Out, 0);
    REQUIRE(modify_result);
  }
  SECTION("modify exlusively registered fd should fail") {
    auto add_result = epoll_instance->add(queue->get_handle(), In | Exclusive, 0);
    REQUIRE(add_result);
    auto modify_result = epoll_instance->modify(queue->get_handle(), Out, 0);
    REQUIRE_FALSE(modify_result);
    REQUIRE(modify_result.error() == epoll::ModifyError::exclusive_previously_specified);
  }
  SECTION("modify invalid file descriptor should fail") {
    auto modify_result = epoll_instance->modify(-1, In, 0);
    REQUIRE_FALSE(modify_result);
    REQUIRE(modify_result.error() == epoll::ModifyError::file_descriptor_invalid);
  }
  SECTION("modify file descriptor same as epoll should fail") {
    auto modify_result = epoll_instance->modify(epoll_instance->get_handle(), In, 0);
    REQUIRE_FALSE(modify_result);
    REQUIRE(modify_result.error() == epoll::ModifyError::file_descriptor_same_with_epoll);
  }
  (void)mq::unlink(queue_name);
}

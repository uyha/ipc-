#include <catch2/catch.hpp>
#include <lpipp/epoll.hpp>
#include <lpipp/mq.hpp>

using namespace lpipp;
using namespace epoll_constants;

TEST_CASE("add to epoll") {
  auto queue_name     = "/epoll.add";
  auto epoll_instance = epoll::create();
  auto queue          = mq::open(queue_name, mq_constants::ReadOnly | mq_constants::Create, 0666);
  REQUIRE(epoll_instance);
  SECTION("add an mq") {
    auto result = epoll_instance->add(queue->get_handle(), In, 0);
    REQUIRE(result);
  }
  SECTION("add invalid file descriptor") {
    auto result = epoll_instance->add(-1, In, 0);
    REQUIRE_FALSE(result);
    CHECK(result.error() == epoll::AddError::file_descriptor_invalid);
  }
  SECTION("add file descriptor twice should fail") {
    auto result = epoll_instance->add(queue->get_handle(), In, 0);
    REQUIRE(result);
    auto second_result = epoll_instance->add(queue->get_handle(), In, 0);
    REQUIRE_FALSE(second_result);
    CHECK(second_result.error() == epoll::AddError::file_descriptor_existed);
  }
  SECTION("add invalid file descriptor") {
    auto result = epoll_instance->add(epoll_instance->get_handle(), In, 0);
    REQUIRE_FALSE(result);
    CHECK(result.error() == epoll::AddError::file_descriptor_same_with_epoll);
  }
  SECTION("add file descriptor for directory should fail") {
    auto dir = ::open(".", O_DIRECTORY);
    REQUIRE(dir != -1);
    auto result = epoll_instance->add(dir, In, 0);
    REQUIRE_FALSE(result);
    CHECK(result.error() == epoll::AddError::file_descriptor_not_supported);
    CHECK(::close(dir) != -1);
  }
  SECTION("exclusive on epoll should fail") {
    auto another_epoll = epoll::create();
    REQUIRE(another_epoll);
    auto result = epoll_instance->add(another_epoll->get_handle(), In | Exclusive, 0);
    REQUIRE_FALSE(result);
    CHECK(result.error() == epoll::AddError::exclusive_on_epoll_error);
  }
  SECTION("loop induced") {
    auto another_epoll = epoll::create();
    REQUIRE(another_epoll);
    auto first_add = epoll_instance->add(another_epoll->get_handle(), In, 0);
    REQUIRE(first_add);
    auto second_add = another_epoll->add(epoll_instance->get_handle(), In, 0);
    REQUIRE_FALSE(second_add);
    CHECK(second_add.error() == epoll::AddError::loop_detected);
  }
  (void)mq::unlink(queue_name);
}

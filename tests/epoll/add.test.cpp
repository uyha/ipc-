#include <catch2/catch.hpp>
#include <lpipp/epoll.hpp>
#include <lpipp/mq.hpp>

using namespace lpipp;
using namespace epoll_constants;
using namespace mq_constants;

TEST_CASE("add to epoll") {
  auto queue_name     = "/epoll.queue";
  auto epoll_instance = epoll::create();
  auto queue          = mq::open(queue_name, read_only | create, 0666);
  REQUIRE(epoll_instance);
  SECTION("add an mq") {
    auto result = epoll_instance->add(queue->get_handle(), in, 0);
    REQUIRE(result);
  }
  SECTION("add invalid file descriptor") {
    auto result = epoll_instance->add(-1, in, 0);
    REQUIRE_FALSE(result);
    CHECK(result.error() == epoll::AddError::file_descriptor_invalid);
  }
  SECTION("add file descriptor twice should fail") {
    auto result = epoll_instance->add(queue->get_handle(), in, 0);
    REQUIRE(result);
    auto second_result = epoll_instance->add(queue->get_handle(), in, 0);
    REQUIRE_FALSE(second_result);
    CHECK(second_result.error() == epoll::AddError::file_descriptor_existed);
  }
  SECTION("add invalid file descriptor") {
    auto result = epoll_instance->add(epoll_instance->get_handle(), in, 0);
    REQUIRE_FALSE(result);
    CHECK(result.error() == epoll::AddError::file_descriptor_same_with_epoll);
  }
  SECTION("add file descriptor for directory should fail") {
    auto dir = ::open(".", O_DIRECTORY);
    REQUIRE(dir != -1);
    auto result = epoll_instance->add(dir, in, 0);
    REQUIRE_FALSE(result);
    CHECK(result.error() == epoll::AddError::file_descriptor_not_supported);
    CHECK(::close(dir) != -1);
  }
  SECTION("loop induced") {
    auto another_epoll = epoll::create();
    REQUIRE(another_epoll);
    auto first_add = epoll_instance->add(another_epoll->get_handle(), in, 0);
    REQUIRE(first_add);
    auto second_add = another_epoll->add(epoll_instance->get_handle(), in, 0);
    REQUIRE_FALSE(second_add);
    CHECK(second_add.error() == epoll::AddError::loop_detected);
  }
  CHECK(mq::unlink(queue_name));
}

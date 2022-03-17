#include <catch2/catch.hpp>
#include <lpipp/epoll.hpp>
#include <lpipp/mq.hpp>

using namespace lpipp;
using namespace epoll_constants;
using namespace mq_constants;

TEST_CASE("remove from epoll") {
  auto epoll_instance = epoll::create();
  REQUIRE(epoll_instance);
  SECTION("remove registered instance") {
    auto second_epoll = epoll::create();
    REQUIRE(second_epoll);
    REQUIRE(epoll_instance->add(second_epoll->get_handle(), In, 0));
    REQUIRE(epoll_instance->remove(second_epoll->get_handle()));
  }
  SECTION("remove unregistered epoll") {
    auto second_epoll = epoll::create();
    REQUIRE(second_epoll);
    auto result = epoll_instance->remove(second_epoll->get_handle());
    REQUIRE_FALSE(result);
    REQUIRE(result.error() == epoll::RemoveError::file_descriptor_not_registered);
  }
}

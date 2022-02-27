#include <catch2/catch.hpp>
#include <lpipp/epoll.hpp>

using namespace lpipp;

TEST_CASE("creating an epoll") {
  auto epoll_instance = epoll::create();
  REQUIRE(epoll_instance);
}

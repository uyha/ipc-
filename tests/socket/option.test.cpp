#include <catch2/catch.hpp>
#include <lpipp/un.hpp>
#include <net/if.h>

using namespace lpipp;
using namespace un_constants;
using namespace socket_options;

TEST_CASE("getting socket option from a dgram unix socket") {
  auto socket = un::create(Dgram);
  REQUIRE(socket);
  auto option = socket->get_option<AcceptConnection>();
  CHECK(option);
  CHECK_FALSE(option->value);
}

TEST_CASE("setting socket option of a dgram unix socket") {
  auto socket = un::create(Dgram);
  REQUIRE(socket);
  auto result = socket->set_option(KeepAlive{true});
  CHECK(result);
}

TEST_CASE("setting socket option of a stream unix socket") {
  using namespace std::string_view_literals;

  auto socket = un::create(Stream);
  REQUIRE(socket);
  auto set_result = socket->set_option(BindToDevice{"eth0"});
  if (set_result) {
    auto get_result = socket->get_option<BindToDevice<IFNAMSIZ>>();
    CHECK(get_result);
    CHECK(get_result->value == "eth0"sv);
    CHECK(get_result->size == 5);
  }
}

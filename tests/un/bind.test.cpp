#include <catch2/catch.hpp>
#include <lpipp-test/macros.hpp>
#include <lpipp/un.hpp>

#define NAME UNIQUE_STR_IMPL("/tmp/un.bind", __LINE__)

using namespace lpipp;
using namespace un_constants;

TEST_CASE("binding a unix socket") {
  auto socket = un::create(Dgram);
  REQUIRE(socket);

  SECTION("normal binding") {
    auto result = socket->bind(NAME);
    CHECK(result);
  }
  SECTION("binding to non existing directory should fail") {
    auto result = socket->bind("/tmp/definitely-non-exisiting-directory/lpipp-un-binding");
    CHECK_FALSE(result);
    CHECK(result.error() == un::BindError::directory_not_exist);
  }
  SECTION("binding to root should fail") {
    auto result = socket->bind("/root/lpipp-un-binding");
    CHECK_FALSE(result);
    CHECK(result.error() == un::BindError::access_denied);
  }
  SECTION("binding twice should fail") {
    CHECK(socket->bind(NAME));
    auto result = socket->bind(NAME);
    CHECK_FALSE(result);
    CHECK(result.error() == un::BindError::invalid);
  }
  SECTION("binding to the same address should fail") {
    auto const address = NAME;
    CHECK(socket->bind(address));
    auto result = socket->bind(address);
    CHECK_FALSE(result);
    CHECK(result.error() == un::BindError::address_in_use);
  }
  SECTION("binding to a address which is too long should fail") {
    auto result = socket->bind("/aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
                               "aaaaaaaaaaaaaaaaaaaaa");
    CHECK_FALSE(result);
    CHECK(result.error() == un::BindError::name_too_long);
  }
  SECTION("binding to a address with prefix being a file should fail") {
    auto result = socket->bind("/dev/null" NAME);
    CHECK_FALSE(result);
    CHECK(result.error() == un::BindError::not_directory);
  }
}

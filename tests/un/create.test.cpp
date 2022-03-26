#include <catch2/catch.hpp>
#include <lpipp/un.h>

using namespace lpipp;
using namespace un_constants;

TEST_CASE("creating a unix socket") {
  SECTION("stream socket") {
    auto create_result = un::create(Stream);
    REQUIRE(create_result);
  }
  SECTION("dgram socket") {
    auto create_result = un::create(Dgram);
    REQUIRE(create_result);
  }
  SECTION("seqpacket socket") {
    auto create_result = un::create(SeqPacket);
    REQUIRE(create_result);
  }
  SECTION("invalid socket should fail") {
    auto create_result = un::create(static_cast<un::SocketType>(0));
    REQUIRE_FALSE(create_result);
  }
}

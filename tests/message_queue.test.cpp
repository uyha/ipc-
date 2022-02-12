#include <catch2/catch.hpp>
#include <ipcpp/mq.hpp>

namespace fs = std::filesystem;
using namespace ipcpp::mq_constants;

TEST_CASE("opening a message queue") {
  auto const name = "/ipcpp-open-mq-test";

  SECTION("creating a message queue while at it") {
    auto permissions   = fs::perms::owner_read;
    auto mode          = read_only | create;
    auto message_queue = ipcpp::mq::open(name, mode, permissions);
    REQUIRE(message_queue.has_value());
  }
  SECTION("opening non existing queue without creating it") {
    auto mode          = read_only;
    auto message_queue = ipcpp::mq::open(name, mode);
    REQUIRE(not message_queue.has_value());
  }

  ipcpp::mq::unlink(name);
}

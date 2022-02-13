#include <catch2/catch.hpp>
#include <ipcpp/mq.hpp>

namespace fs = std::filesystem;
using namespace ipcpp;
using namespace mq_constants;

TEST_CASE("opening a message queue") {
  auto const name = "/ipcpp-open-mq-test";

  SECTION("creating a message queue while at it") {
    auto permissions   = fs::perms::owner_read;
    auto mode          = read_only | create;
    auto message_queue = ipcpp::mq::open(name, mode, permissions);
    REQUIRE(message_queue);
  }
  SECTION("opening non existing queue without creating it") {
    auto mode          = read_only;
    auto message_queue = ipcpp::mq::open(name, mode);
    REQUIRE(not message_queue);
    REQUIRE(message_queue.error() == mq::OpenError::QueueMissing);
  }

  ipcpp::mq::unlink(name);
}

TEST_CASE("opening with invalid name") {
  auto mode = read_only;

  auto empty_name = mq::open("", mode);
  REQUIRE(not empty_name);
  REQUIRE(empty_name.error() == mq::OpenError::NameInvalid);

  auto only_slash = mq::open("/", mode);
  REQUIRE(not only_slash);
  REQUIRE(only_slash.error() == mq::OpenError::NameInvalid);

  auto adjacent_slashes = mq::open("//", mode);
  REQUIRE(not adjacent_slashes);
  REQUIRE(adjacent_slashes.error() == mq::OpenError::NameInvalid);

  auto distanced_end_slash = mq::open("/name/", mode);
  REQUIRE(not distanced_end_slash);
  REQUIRE(distanced_end_slash.error() == mq::OpenError::NameInvalid);

  auto distanced_middle_slash = mq::open("/na/me", mode);
  REQUIRE(not distanced_middle_slash);
  REQUIRE(distanced_middle_slash.error() == mq::OpenError::NameInvalid);
}

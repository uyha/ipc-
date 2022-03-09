#include <catch2/catch.hpp>
#include <lpipp/mq.hpp>

namespace fs = std::filesystem;
using namespace lpipp;
using namespace mq_constants;

TEST_CASE("opening a message queue") {
  auto const name = "/lpipp-open-mq-test";

  SECTION("creating a message queue while at it") {
    auto permissions   = fs::perms::owner_read;
    auto mode          = read_only | create;
    auto message_queue = lpipp::mq::open(name, mode, permissions);
    CHECK(message_queue);
    CHECK(mq::unlink(name).has_value());
  }
  SECTION("creating a message queue with permissions being octet") {
    auto mode          = read_only | create;
    auto message_queue = lpipp::mq::open(name, mode, 0666);
    CHECK(message_queue);
    CHECK(mq::unlink(name).has_value());
  }
  SECTION("creating an already existing queue") {
    auto mode          = read_only | create;
    auto message_queue = lpipp::mq::open(name, mode, 0666);
    CHECK(message_queue);
    auto dup_message_queue = lpipp::mq::open(name, read_only | create | exclusive, 0666);
    CHECK_FALSE(dup_message_queue);
    CHECK(dup_message_queue.error() == mq::OpenError::queue_existed);
    CHECK(mq::unlink(name).has_value());
  }
  SECTION("opening non existing queue without creating it") {
    auto mode          = read_only;
    auto message_queue = lpipp::mq::open(name, mode);
    CHECK_FALSE(message_queue);
    CHECK(message_queue.error() == mq::OpenError::queue_missing);
  }
  SECTION("with invalid max messages") {
    auto permissions          = fs::perms::owner_read;
    auto mode                 = read_only | create;
    auto invalid_max_messages = mq::CreateAttributes{.max_messages = 0, .max_message_size = 1};
    auto message_queue        = lpipp::mq::open(name, mode, permissions, invalid_max_messages);
    CHECK_FALSE(message_queue);
    CHECK(message_queue.error() == mq::OpenError::attribute_invalid);
  }
  SECTION("with invalid max messages") {
    auto permissions          = fs::perms::owner_read;
    auto mode                 = read_only | create;
    auto invalid_message_size = mq::CreateAttributes{.max_messages = 1, .max_message_size = 0};
    auto message_queue        = lpipp::mq::open(name, mode, permissions, invalid_message_size);
    CHECK_FALSE(message_queue);
    CHECK(message_queue.error() == mq::OpenError::attribute_invalid);
  }
}

TEST_CASE("opening message queue with invalid name") {
  auto mode = read_only;

  auto empty_name = mq::open("", mode);
  CHECK_FALSE(empty_name);
  CHECK(empty_name.error() == mq::OpenError::name_invalid);

  auto only_slash = mq::open("/", mode);
  CHECK_FALSE(only_slash);
  CHECK(only_slash.error() == mq::OpenError::name_invalid);

  auto adjacent_slashes = mq::open("//", mode);
  CHECK_FALSE(adjacent_slashes);
  CHECK(adjacent_slashes.error() == mq::OpenError::name_invalid);

  auto distanced_end_slash = mq::open("/name/", mode);
  CHECK_FALSE(distanced_end_slash);
  CHECK(distanced_end_slash.error() == mq::OpenError::name_invalid);

  auto distanced_middle_slash = mq::open("/na/me", mode);
  CHECK_FALSE(distanced_middle_slash);
  CHECK(distanced_middle_slash.error() == mq::OpenError::name_invalid);

  auto too_long_name =
      mq::open("/aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
               "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
               "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
               mode);
  CHECK_FALSE(too_long_name);
  CHECK(too_long_name.error() == mq::OpenError::name_too_long);
}

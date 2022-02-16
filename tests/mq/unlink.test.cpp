#include <catch2/catch.hpp>
#include <ipcpp/mq.hpp>

using namespace ipcpp;
using namespace mq_constants;

TEST_CASE("unlinking non existing queue") {
  auto result = mq::unlink("/unlink");
  CHECK_FALSE( result);
  CHECK(result.error() == mq::UnlinkError::queue_missing);
}
TEST_CASE("creating and unlinking queue") {
  auto name   = "/unlink";
  auto queue  = mq::open(name, read_only | create, 0666);
  auto result = mq::unlink(name);
  CHECK(result);
}
TEST_CASE("creating unsuccessfully and unlinking queue") {
  auto name =
      "/aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
      "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
      "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
  auto queue  = mq::open(name, read_only | create, 0666);
  auto result = mq::unlink(name);
  CHECK_FALSE( result);
  CHECK(result.error() == mq::UnlinkError::name_too_long);
}

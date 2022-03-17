#include <catch2/catch.hpp>
#include <lpipp/mq.hpp>

using namespace lpipp;
using namespace mq_constants;

TEST_CASE("unlinking message queue") {
  SECTION("unlinking non-existing queue should fail") {
    auto result = mq::unlink("/unlink");
    CHECK_FALSE(result);
    CHECK(result.error() == mq::UnlinkError::queue_missing);
  }
  SECTION("creating and unlinking queue") {
    auto name   = "/unlink";
    auto queue  = mq::open(name, ReadOnly | Create, 0666);
    auto result = mq::unlink(name);
    CHECK(result);
  }
  SECTION("creating unsuccessfully and unlinking queue") {
    auto name   = "/aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
                  "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
                  "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
    auto queue  = mq::open(name, ReadOnly | Create, 0666);
    auto result = mq::unlink(name);
    CHECK_FALSE(result);
    CHECK(result.error() == mq::UnlinkError::name_too_long);
  }
}

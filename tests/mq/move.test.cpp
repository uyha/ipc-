#include <asm-generic/errno.h>
#include <catch2/catch.hpp>
#include <lpipp/mq.hpp>

using namespace lpipp;
using namespace mq_constants;

TEST_CASE("move construction") {
  auto name = "/mq.move-construction";
  SECTION("interacting with moved from object will give bad file descriptor error") {
    auto move_queue = mq::open(name, create | write_only | nonblock, 0666);
    REQUIRE(move_queue);
    auto queue  = std::move(*move_queue);
    auto result = move_queue->send(name, 1);
    REQUIRE_FALSE(result);
    // This is a false error due to the invariant of the `mq` class is not guaranteed for moved from objects
    CHECK(result.error() == mq::SendError::queue_read_only);
    CHECK(mq::unlink(name));
  }
  SECTION("move constructed queue behaves normally") {
    auto move_queue = mq::open(name, create | write_only | nonblock, 0666);
    REQUIRE(move_queue);
    REQUIRE(move_queue);
    auto queue  = std::move(*move_queue);
    auto result = queue.send(name, 1);
    CHECK(result);
    CHECK(queue.get_attributes().mq_curmsgs == 1);
    CHECK(mq::unlink(name));
  }
}

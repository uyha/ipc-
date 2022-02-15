#include <catch2/catch.hpp>
#include <ipcpp/mq.hpp>

using namespace ipcpp;
using namespace mq_constants;

TEST_CASE("getting attribute of a queue") {
  auto name = "/queue";
  SECTION("blocking queue") {
    auto queue = mq::open(name, read_only | create, 0666);
    CHECK(queue);
    auto attributes = queue->get_attributes();
    CHECK(not(attributes.mq_flags & O_NONBLOCK));
  }
  SECTION("getting attribute of a nonblocking queue") {
    auto queue = mq::open(name, read_only | create | nonblock, 0666);
    CHECK(queue);
    auto attributes = queue->get_attributes();
    CHECK(attributes.mq_flags & O_NONBLOCK);
  }
  CHECK(mq::unlink(name));
}

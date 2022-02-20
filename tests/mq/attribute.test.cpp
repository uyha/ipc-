#include <catch2/catch.hpp>
#include <lpipp/mq.hpp>

using namespace lpipp;
using namespace mq_constants;

TEST_CASE("getting attribute of a queue") {
  auto name = "/queue";
  SECTION("blocking queue") {
    auto queue = mq::open(name, read_only | create, 0666);
    CHECK(queue);
    auto attributes = queue->get_attributes();
    CHECK_FALSE(attributes.mq_flags & O_NONBLOCK);
  }
  SECTION("getting attribute of a nonblocking queue") {
    auto queue = mq::open(name, read_only | create | nonblock, 0666);
    CHECK(queue);
    auto attributes = queue->get_attributes();
    CHECK(attributes.mq_flags & O_NONBLOCK);
  }
  CHECK(mq::unlink(name));
}

TEST_CASE("setting/getting attribute of a queue") {
  auto name = "/queue";
  SECTION("blocking queue") {
    auto queue = mq::open(name, read_only | create, 0666);
    CHECK(queue);

    auto attributes = queue->get_attributes();
    CHECK_FALSE(attributes.mq_flags & O_NONBLOCK);

    auto old_attributes = queue->set_nonblock();
    CHECK(attributes.mq_flags == old_attributes.mq_flags);
    CHECK(attributes.mq_maxmsg == old_attributes.mq_maxmsg);
    CHECK(attributes.mq_msgsize == old_attributes.mq_msgsize);
    CHECK(attributes.mq_curmsgs == old_attributes.mq_curmsgs);

    auto new_attributes = queue->get_attributes();
    CHECK(new_attributes.mq_flags & O_NONBLOCK);
  }
  SECTION("nonblocking queue") {
    auto queue = mq::open(name, read_only | create | nonblock, 0666);
    CHECK(queue);

    auto attributes = queue->get_attributes();
    CHECK(attributes.mq_flags & O_NONBLOCK);

    auto old_attributes = queue->set_block();
    CHECK(attributes.mq_flags == old_attributes.mq_flags);
    CHECK(attributes.mq_maxmsg == old_attributes.mq_maxmsg);
    CHECK(attributes.mq_msgsize == old_attributes.mq_msgsize);
    CHECK(attributes.mq_curmsgs == old_attributes.mq_curmsgs);

    auto new_attributes = queue->get_attributes();
    CHECK_FALSE(new_attributes.mq_flags & O_NONBLOCK);
  }
  CHECK(mq::unlink(name));
}

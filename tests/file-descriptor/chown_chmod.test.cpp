#include <catch2/catch.hpp>
#include <lpipp/epoll.hpp>

using namespace lpipp;
using namespace epoll_constants;

TEST_CASE("file_descriptor chown") {
  auto epoll = epoll::create();
  REQUIRE(epoll);

  SECTION("setting uid and gid to current") {
    auto uid = ::getuid();
    auto gid = ::getgid();

    auto result = epoll->chown(uid, gid);
    CHECK(result);

    auto stat = epoll->stat();
    CHECK(stat);
    CHECK(stat->st_uid == uid);
    CHECK(stat->st_gid == gid);
  }

  SECTION("setting uid and gid to root should fail") {
    auto result = epoll->chown(0, 0);
    CHECK_FALSE(result);
    CHECK(result.error() == ChownError::permission_denied);
  }
}

TEST_CASE("file_descriptor chmod") {
  auto epoll = epoll::create();
  REQUIRE(epoll);
  CHECK(epoll->chmod(0666));
}

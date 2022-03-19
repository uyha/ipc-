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
    CHECKED_IF(result) {
      auto stat = epoll->stat();
      CHECK(stat);
      CHECK(stat->st_uid == uid);
      CHECK(stat->st_gid == gid);
    }
    CHECKED_ELSE(result) {
      CHECK(result.error() == ChownError::permission_denied);
    }
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
  auto result = epoll->chmod(0666);
  CHECKED_IF(result) {
    auto stat = epoll->stat();
    CHECK(stat);
    CHECK(stat->st_mode == 0666);
  }
  CHECKED_ELSE(result) {
    CHECK(result.error() == ChmodError::permission_denied);
  }
}

#include <catch2/catch.hpp>
#include <lpipp/epoll.hpp>

using namespace lpipp;
using namespace epoll_constants;

TEST_CASE("file_descriptor chown") {
  auto epoll = epoll::create();
  REQUIRE(epoll);

  auto uid = ::getuid();
  auto gid = ::getgid();

  auto result = epoll->chown(uid, gid);
  CHECK(result);

  auto stat = epoll->stat();
  CHECK(stat);
  CHECK(stat->st_uid == uid);
  CHECK(stat->st_gid == gid);
}

#include <catch2/catch.hpp>
#include <lpipp-test/macros.hpp>
#include <lpipp/shm.hpp>

#define NAME UNIQUE_STR_IMPL("/fcntl.duplicate", __LINE__)

using namespace lpipp;
using namespace shm_constants;

TEST_CASE("file_descriptor duplicate") {
  auto const name = NAME;

  auto const shared_memory = shm::open(name, ReadWrite | Create, 600);
  REQUIRE(shared_memory);
  auto const shm_stat = shared_memory->stat();
  CHECK(shm_stat);

  auto const dup_memory = shared_memory->duplicate();
  CHECK(dup_memory);
  auto const dup_stat = dup_memory->stat();
  CHECK(dup_stat);
  CHECK(dup_stat->st_ino == shm_stat->st_ino);

  CHECK(shm::unlink(name));
}
TEST_CASE("file_descriptor duplicate at least") {
  auto const name = NAME;

  auto const shared_memory = shm::open(name, ReadWrite | Create, 600);
  REQUIRE(shared_memory);
  auto const shm_stat = shared_memory->stat();
  CHECK(shm_stat);

  auto const dup_memory = shared_memory->duplicate_at_least(15);
  CHECK(dup_memory);
  CHECK(dup_memory->get_handle() >= 15);
  auto const dup_stat = dup_memory->stat();
  CHECK(dup_stat);
  CHECK(dup_stat->st_ino == shm_stat->st_ino);

  CHECK(shm::unlink(name));
}

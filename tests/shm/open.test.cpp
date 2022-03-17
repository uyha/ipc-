#include <catch2/catch.hpp>
#include <filesystem>
#include <lpipp/shm.hpp>

#define QUOTE(s) #s
#define NAME "/shm.open" QUOTE(__LINE__)

namespace fs = std::filesystem;
using namespace lpipp;
using namespace shm_constants;

TEST_CASE("opening shared memory") {
  SECTION("creating a message queue while at it") {
    auto const name    = NAME;
    auto shared_memory = shm::open(name, ReadOnly | Create, fs::perms::all);
    CHECK(shared_memory);
    CHECK(shm::unlink(name));
  }
  SECTION("creating a message queue with permissions being octet") {
    auto const name    = NAME;
    auto shared_memory = shm::open(name, ReadOnly | Create, 0666);
    CHECK(shared_memory);
    CHECK(shm::unlink(name).has_value());
  }
  SECTION("creating an already existing queue") {
    auto const name    = NAME;
    auto shared_memory = shm::open(name, ReadOnly | Create, 0666);
    REQUIRE(shared_memory);
    auto dup = shm::open(name, ReadOnly | Create | Exclusive, 0666);
    CHECK_FALSE(dup);
    CHECK(dup.error() == shm::OpenError::shared_memory_existed);
    CHECK(shm::unlink(name).has_value());
  }
  SECTION("opening non existing queue without creating it") {
    auto const name    = NAME;
    auto shared_memory = shm::open(name, ReadOnly);
    CHECK_FALSE(shared_memory);
    CHECK(shared_memory.error() == shm::OpenError::shared_memory_missing);
  }
}

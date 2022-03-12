#include <catch2/catch.hpp>
#include <filesystem>
#include <lpipp/shm.hpp>

#define QUOTE(s) #s
#define NAME "/shm.open" QUOTE(__LINE__)

using namespace lpipp;
using namespace shm_constants;

TEST_CASE("truncating shared memory") {
  SECTION("truncate a writable shared memory") {
    auto const name    = NAME;
    auto shared_memory = shm::open(name, read_write | create, 0666);
    REQUIRE(shared_memory);

    CHECK(shared_memory->truncate(100));

    CHECK(shm::unlink(name));
  }
  SECTION("truncate a readonly shared memory should fail") {
    auto const name          = NAME;
    auto const shared_memory = shm::open(name, read_only | create, 0666);
    REQUIRE(shared_memory);

    auto const result = shared_memory->truncate(100);
    CHECK_FALSE(result);
    CHECK(result.error() == shm::TruncateError::unwritable);

    CHECK(shm::unlink(name));
  }
}

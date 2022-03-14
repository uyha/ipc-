#include <catch2/catch.hpp>
#include <filesystem>
#include <lpipp/shm.hpp>

#define QUOTE(s) #s
#define NAME "/shm.map" QUOTE(__LINE__)

using namespace lpipp;
using namespace shm_constants;

TEST_CASE("mapping shared memory") {
  SECTION("map a read only shared memory") {
    auto const name    = NAME;
    auto shared_memory = shm::open(name, read_only | create, 0444);
    REQUIRE(shared_memory);

    auto truncate_result = shared_memory->truncate(8);
    CHECK_FALSE(truncate_result);
    CHECK(truncate_result.error() == shm::TruncateError::unwritable);

    auto map_address = shared_memory->map(8, read, map_private);
    CHECK(map_address);
    CHECK(shm::unlink(name));
  }
}

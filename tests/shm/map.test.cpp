#include <catch2/catch.hpp>
#include <filesystem>
#include <lpipp-test/macros.hpp>
#include <lpipp/shm.hpp>

#define NAME UNIQUE_STR_IMPL("/shm.map", __LINE__)

using namespace lpipp;
using namespace shm_constants;

TEST_CASE("mapping shared memory") {
  SECTION("map a read only shared memory") {
    auto const name    = NAME;
    auto shared_memory = shm::open(name, ReadWrite | Create, 0444);
    REQUIRE(shared_memory);

    auto truncate_result = shared_memory->truncate(8);
    CHECK(truncate_result);

    auto map_address = shared_memory->map(8, Read, MapPrivate);
    CHECK(map_address);
    CHECK(shm::unlink(name));
  }
  SECTION("map with 0 length should fail") {
    auto const name    = NAME;
    auto shared_memory = shm::open(name, ReadOnly | Create, 0444);
    REQUIRE(shared_memory);

    auto map_address = shared_memory->map(0, Read, MapPrivate);
    CHECK_FALSE(map_address);
    CHECK(map_address.error() == shm::MapError::argument_invalid);
    CHECK(shm::unlink(name));
  }
}

TEST_CASE("unmapping shared memory") {
  SECTION("unmap a mapped shared memory") {
    auto const name    = NAME;
    auto shared_memory = shm::open(name, ReadWrite | Create, 0444);
    REQUIRE(shared_memory);

    auto truncate_result = shared_memory->truncate(8);
    CHECK(truncate_result);

    auto map_address = shared_memory->map(8, Read, MapPrivate);
    CHECK(map_address);
    auto unmap_result = shm::unmap(*map_address, 8);
    CHECK(unmap_result);
    CHECK(shm::unlink(name));
  }
  SECTION("unmap invalid address should fail") {
    auto unmap_result = shm::unmap(MAP_FAILED, 8);
    CHECK_FALSE(unmap_result);
    CHECK(unmap_result.error() == shm::UnmapError::argument_invalid);
  }
  SECTION("unmap length 0 should fail") {
    auto unmap_result = shm::unmap(nullptr, 0);
    CHECK_FALSE(unmap_result);
    CHECK(unmap_result.error() == shm::UnmapError::argument_invalid);
  }
}

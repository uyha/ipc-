#include <catch2/catch.hpp>
#include <lpipp/shm.hpp>

using namespace lpipp;
using namespace shm_constants;

TEST_CASE("unlinking shared memory") {
  SECTION("unlinking non-existing queue should fail") {
    auto result = shm::unlink("/shm.unlink");
    CHECK_FALSE(result);
    CHECK(result.error() == shm::UnlinkError::shared_memory_missing);
  }
  SECTION("creating and unlinking queue") {
    auto name          = "/shm.unlink.open";
    auto shared_memory = shm::open(name, ReadOnly | Create, 0666);
    auto result        = shm::unlink(name);
    CHECK(result);
  }
  SECTION("creating unsuccessfully and unlinking queue") {
    auto name   = "/aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
                  "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
                  "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
    auto queue  = shm::open(name, ReadOnly | Create, 0666);
    auto result = shm::unlink(name);
    CHECK_FALSE(result);
    CHECK(result.error() == shm::UnlinkError::shared_memory_missing);
  }
}

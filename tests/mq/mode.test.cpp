#include <catch2/catch.hpp>
#include <ipcpp/mq.hpp>

using namespace ipcpp::mq_constants;

TEST_CASE("mode combinations") {
  REQUIRE(static_cast<int>(read_only) == O_RDONLY);
}

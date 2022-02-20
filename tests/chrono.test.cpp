#include <catch2/catch.hpp>
#include <lpipp/chrono.hpp>
#include <ostream>

constexpr auto operator==(::timespec lhs, ::timespec rhs) noexcept -> bool {
  return lhs.tv_sec == rhs.tv_sec and lhs.tv_nsec == rhs.tv_nsec;
}

auto operator<<(std::ostream &os, ::timespec const &timespec) -> std::ostream & {
  return os << "{seconds = " << timespec.tv_sec << ", nanoseconds = " << timespec.tv_nsec << "}";
}

using namespace lpipp;
using namespace std::chrono;
using namespace std::chrono_literals;

TEST_CASE("to_timespec") {
  CHECK(lpipp::to_timespec(system_clock::time_point{}) == ::timespec{});
  CHECK(lpipp::to_timespec(system_clock::time_point{} + 2s) == ::timespec{.tv_sec = 2, .tv_nsec = 0});
  CHECK(lpipp::to_timespec(system_clock::time_point{} + 2s + 2ns) == ::timespec{.tv_sec = 2, .tv_nsec = 2});
  CHECK(lpipp::to_timespec(system_clock::time_point{} + 2s + 999'999'999ns)
        == ::timespec{.tv_sec = 2, .tv_nsec = 999'999'999});
  CHECK(lpipp::to_timespec(system_clock::time_point{} + 2s + 1'999'999'999ns)
        == ::timespec{.tv_sec = 3, .tv_nsec = 999'999'999});
}

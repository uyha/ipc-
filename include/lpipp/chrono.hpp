#pragma once

#include <bits/types/struct_timespec.h>
#include <chrono>

namespace lpipp {
template <typename Duration>
constexpr auto to_timespec(std::chrono::time_point<std::chrono::system_clock, Duration> const &timepoint) noexcept
    -> ::timespec {
  using namespace std::chrono;
  auto timepoint_s  = time_point_cast<seconds>(timepoint);
  auto timepoint_ns = time_point_cast<nanoseconds>(timepoint);

  auto sec  = timepoint_s.time_since_epoch().count();
  auto nsec = timepoint_ns.time_since_epoch().count() % 1'000'000'000;

  return {.tv_sec = sec, .tv_nsec = nsec};
}
constexpr auto to_timepoint(::timespec value) noexcept -> std::chrono::system_clock::time_point {
  using namespace std::chrono;
  auto const duration = seconds{value.tv_sec} + nanoseconds{value.tv_nsec};
  return system_clock::time_point{duration_cast<system_clock::duration>(duration)};
}
} // namespace lpipp

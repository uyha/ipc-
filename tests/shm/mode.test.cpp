#include <catch2/catch.hpp>
#include <lpipp/shm.hpp>

using namespace lpipp;
using namespace shm_constants;

TEST_CASE("shared memeory open mode bitor operations") {
  CHECK(static_cast<std::uint32_t>(read_only) == static_cast<std::uint32_t>(O_RDONLY));
  CHECK(static_cast<std::uint32_t>(read_only | create) == static_cast<std::uint32_t>(O_RDONLY | O_CREAT));
  CHECK(static_cast<std::uint32_t>(create | read_only) == static_cast<std::uint32_t>(O_RDONLY | O_CREAT));
  CHECK(static_cast<std::uint32_t>(read_only | create | exclusive) == static_cast<std::uint32_t>(O_RDONLY | O_CREAT | O_EXCL));
  CHECK(static_cast<std::uint32_t>(read_only | exclusive | create) == static_cast<std::uint32_t>(O_RDONLY | O_CREAT | O_EXCL));
  CHECK(static_cast<std::uint32_t>(create | read_only | exclusive) == static_cast<std::uint32_t>(O_RDONLY | O_CREAT | O_EXCL));
  CHECK(static_cast<std::uint32_t>(create | exclusive | read_only) == static_cast<std::uint32_t>(O_RDONLY | O_CREAT | O_EXCL));
  CHECK(static_cast<std::uint32_t>(exclusive | read_only | create) == static_cast<std::uint32_t>(O_RDONLY | O_CREAT | O_EXCL));
  CHECK(static_cast<std::uint32_t>(exclusive | create | read_only) == static_cast<std::uint32_t>(O_RDONLY | O_CREAT | O_EXCL));
  CHECK(static_cast<std::uint32_t>(read_write) == static_cast<std::uint32_t>(O_RDWR));
  CHECK(static_cast<std::uint32_t>(read_write | create) == static_cast<std::uint32_t>(O_RDWR | O_CREAT));
  CHECK(static_cast<std::uint32_t>(create | read_write) == static_cast<std::uint32_t>(O_RDWR | O_CREAT));
  CHECK(static_cast<std::uint32_t>(read_write | create | exclusive) == static_cast<std::uint32_t>(O_RDWR | O_CREAT | O_EXCL));
  CHECK(static_cast<std::uint32_t>(read_write | exclusive | create) == static_cast<std::uint32_t>(O_RDWR | O_CREAT | O_EXCL));
  CHECK(static_cast<std::uint32_t>(create | read_write | exclusive) == static_cast<std::uint32_t>(O_RDWR | O_CREAT | O_EXCL));
  CHECK(static_cast<std::uint32_t>(create | exclusive | read_write) == static_cast<std::uint32_t>(O_RDWR | O_CREAT | O_EXCL));
  CHECK(static_cast<std::uint32_t>(exclusive | read_write | create) == static_cast<std::uint32_t>(O_RDWR | O_CREAT | O_EXCL));
  CHECK(static_cast<std::uint32_t>(exclusive | create | read_write) == static_cast<std::uint32_t>(O_RDWR | O_CREAT | O_EXCL));
  CHECK(static_cast<std::uint32_t>(read_write | create | exclusive | truncate) == static_cast<std::uint32_t>(O_RDWR | O_CREAT | O_EXCL | O_TRUNC));
  CHECK(static_cast<std::uint32_t>(read_write | create | truncate | exclusive) == static_cast<std::uint32_t>(O_RDWR | O_CREAT | O_EXCL | O_TRUNC));
  CHECK(static_cast<std::uint32_t>(read_write | exclusive | create | truncate) == static_cast<std::uint32_t>(O_RDWR | O_CREAT | O_EXCL | O_TRUNC));
  CHECK(static_cast<std::uint32_t>(read_write | exclusive | truncate | create) == static_cast<std::uint32_t>(O_RDWR | O_CREAT | O_EXCL | O_TRUNC));
  CHECK(static_cast<std::uint32_t>(read_write | truncate | create | exclusive) == static_cast<std::uint32_t>(O_RDWR | O_CREAT | O_EXCL | O_TRUNC));
  CHECK(static_cast<std::uint32_t>(read_write | truncate | exclusive | create) == static_cast<std::uint32_t>(O_RDWR | O_CREAT | O_EXCL | O_TRUNC));
  CHECK(static_cast<std::uint32_t>(create | read_write | exclusive | truncate) == static_cast<std::uint32_t>(O_RDWR | O_CREAT | O_EXCL | O_TRUNC));
  CHECK(static_cast<std::uint32_t>(create | read_write | truncate | exclusive) == static_cast<std::uint32_t>(O_RDWR | O_CREAT | O_EXCL | O_TRUNC));
  CHECK(static_cast<std::uint32_t>(create | exclusive | read_write | truncate) == static_cast<std::uint32_t>(O_RDWR | O_CREAT | O_EXCL | O_TRUNC));
  CHECK(static_cast<std::uint32_t>(create | exclusive | truncate | read_write) == static_cast<std::uint32_t>(O_RDWR | O_CREAT | O_EXCL | O_TRUNC));
  CHECK(static_cast<std::uint32_t>(create | truncate | read_write | exclusive) == static_cast<std::uint32_t>(O_RDWR | O_CREAT | O_EXCL | O_TRUNC));
  CHECK(static_cast<std::uint32_t>(create | truncate | exclusive | read_write) == static_cast<std::uint32_t>(O_RDWR | O_CREAT | O_EXCL | O_TRUNC));
  CHECK(static_cast<std::uint32_t>(exclusive | read_write | create | truncate) == static_cast<std::uint32_t>(O_RDWR | O_CREAT | O_EXCL | O_TRUNC));
  CHECK(static_cast<std::uint32_t>(exclusive | read_write | truncate | create) == static_cast<std::uint32_t>(O_RDWR | O_CREAT | O_EXCL | O_TRUNC));
  CHECK(static_cast<std::uint32_t>(exclusive | create | read_write | truncate) == static_cast<std::uint32_t>(O_RDWR | O_CREAT | O_EXCL | O_TRUNC));
  CHECK(static_cast<std::uint32_t>(exclusive | create | truncate | read_write) == static_cast<std::uint32_t>(O_RDWR | O_CREAT | O_EXCL | O_TRUNC));
  CHECK(static_cast<std::uint32_t>(exclusive | truncate | read_write | create) == static_cast<std::uint32_t>(O_RDWR | O_CREAT | O_EXCL | O_TRUNC));
  CHECK(static_cast<std::uint32_t>(exclusive | truncate | create | read_write) == static_cast<std::uint32_t>(O_RDWR | O_CREAT | O_EXCL | O_TRUNC));
  CHECK(static_cast<std::uint32_t>(truncate | read_write | create | exclusive) == static_cast<std::uint32_t>(O_RDWR | O_CREAT | O_EXCL | O_TRUNC));
  CHECK(static_cast<std::uint32_t>(truncate | read_write | exclusive | create) == static_cast<std::uint32_t>(O_RDWR | O_CREAT | O_EXCL | O_TRUNC));
  CHECK(static_cast<std::uint32_t>(truncate | create | read_write | exclusive) == static_cast<std::uint32_t>(O_RDWR | O_CREAT | O_EXCL | O_TRUNC));
  CHECK(static_cast<std::uint32_t>(truncate | create | exclusive | read_write) == static_cast<std::uint32_t>(O_RDWR | O_CREAT | O_EXCL | O_TRUNC));
  CHECK(static_cast<std::uint32_t>(truncate | exclusive | read_write | create) == static_cast<std::uint32_t>(O_RDWR | O_CREAT | O_EXCL | O_TRUNC));
  CHECK(static_cast<std::uint32_t>(truncate | exclusive | create | read_write) == static_cast<std::uint32_t>(O_RDWR | O_CREAT | O_EXCL | O_TRUNC));
}

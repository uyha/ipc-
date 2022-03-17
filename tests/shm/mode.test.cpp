#include <catch2/catch.hpp>
#include <lpipp/shm.hpp>

using namespace lpipp;
using namespace shm_constants;

TEST_CASE("shared memeory open mode bitor operations") {
  CHECK(static_cast<std::uint32_t>(ReadOnly) == static_cast<std::uint32_t>(O_RDONLY));
  CHECK(static_cast<std::uint32_t>(ReadOnly | Create) == static_cast<std::uint32_t>(O_RDONLY | O_CREAT));
  CHECK(static_cast<std::uint32_t>(Create | ReadOnly) == static_cast<std::uint32_t>(O_RDONLY | O_CREAT));
  CHECK(static_cast<std::uint32_t>(ReadOnly | Create | Exclusive) == static_cast<std::uint32_t>(O_RDONLY | O_CREAT | O_EXCL));
  CHECK(static_cast<std::uint32_t>(ReadOnly | Exclusive | Create) == static_cast<std::uint32_t>(O_RDONLY | O_CREAT | O_EXCL));
  CHECK(static_cast<std::uint32_t>(Create | ReadOnly | Exclusive) == static_cast<std::uint32_t>(O_RDONLY | O_CREAT | O_EXCL));
  CHECK(static_cast<std::uint32_t>(Create | Exclusive | ReadOnly) == static_cast<std::uint32_t>(O_RDONLY | O_CREAT | O_EXCL));
  CHECK(static_cast<std::uint32_t>(Exclusive | ReadOnly | Create) == static_cast<std::uint32_t>(O_RDONLY | O_CREAT | O_EXCL));
  CHECK(static_cast<std::uint32_t>(Exclusive | Create | ReadOnly) == static_cast<std::uint32_t>(O_RDONLY | O_CREAT | O_EXCL));
  CHECK(static_cast<std::uint32_t>(ReadWrite) == static_cast<std::uint32_t>(O_RDWR));
  CHECK(static_cast<std::uint32_t>(ReadWrite | Create) == static_cast<std::uint32_t>(O_RDWR | O_CREAT));
  CHECK(static_cast<std::uint32_t>(Create | ReadWrite) == static_cast<std::uint32_t>(O_RDWR | O_CREAT));
  CHECK(static_cast<std::uint32_t>(ReadWrite | Create | Exclusive) == static_cast<std::uint32_t>(O_RDWR | O_CREAT | O_EXCL));
  CHECK(static_cast<std::uint32_t>(ReadWrite | Exclusive | Create) == static_cast<std::uint32_t>(O_RDWR | O_CREAT | O_EXCL));
  CHECK(static_cast<std::uint32_t>(Create | ReadWrite | Exclusive) == static_cast<std::uint32_t>(O_RDWR | O_CREAT | O_EXCL));
  CHECK(static_cast<std::uint32_t>(Create | Exclusive | ReadWrite) == static_cast<std::uint32_t>(O_RDWR | O_CREAT | O_EXCL));
  CHECK(static_cast<std::uint32_t>(Exclusive | ReadWrite | Create) == static_cast<std::uint32_t>(O_RDWR | O_CREAT | O_EXCL));
  CHECK(static_cast<std::uint32_t>(Exclusive | Create | ReadWrite) == static_cast<std::uint32_t>(O_RDWR | O_CREAT | O_EXCL));
  CHECK(static_cast<std::uint32_t>(ReadWrite | Create | Exclusive | Truncate) == static_cast<std::uint32_t>(O_RDWR | O_CREAT | O_EXCL | O_TRUNC));
  CHECK(static_cast<std::uint32_t>(ReadWrite | Create | Truncate | Exclusive) == static_cast<std::uint32_t>(O_RDWR | O_CREAT | O_EXCL | O_TRUNC));
  CHECK(static_cast<std::uint32_t>(ReadWrite | Exclusive | Create | Truncate) == static_cast<std::uint32_t>(O_RDWR | O_CREAT | O_EXCL | O_TRUNC));
  CHECK(static_cast<std::uint32_t>(ReadWrite | Exclusive | Truncate | Create) == static_cast<std::uint32_t>(O_RDWR | O_CREAT | O_EXCL | O_TRUNC));
  CHECK(static_cast<std::uint32_t>(ReadWrite | Truncate | Create | Exclusive) == static_cast<std::uint32_t>(O_RDWR | O_CREAT | O_EXCL | O_TRUNC));
  CHECK(static_cast<std::uint32_t>(ReadWrite | Truncate | Exclusive | Create) == static_cast<std::uint32_t>(O_RDWR | O_CREAT | O_EXCL | O_TRUNC));
  CHECK(static_cast<std::uint32_t>(Create | ReadWrite | Exclusive | Truncate) == static_cast<std::uint32_t>(O_RDWR | O_CREAT | O_EXCL | O_TRUNC));
  CHECK(static_cast<std::uint32_t>(Create | ReadWrite | Truncate | Exclusive) == static_cast<std::uint32_t>(O_RDWR | O_CREAT | O_EXCL | O_TRUNC));
  CHECK(static_cast<std::uint32_t>(Create | Exclusive | ReadWrite | Truncate) == static_cast<std::uint32_t>(O_RDWR | O_CREAT | O_EXCL | O_TRUNC));
  CHECK(static_cast<std::uint32_t>(Create | Exclusive | Truncate | ReadWrite) == static_cast<std::uint32_t>(O_RDWR | O_CREAT | O_EXCL | O_TRUNC));
  CHECK(static_cast<std::uint32_t>(Create | Truncate | ReadWrite | Exclusive) == static_cast<std::uint32_t>(O_RDWR | O_CREAT | O_EXCL | O_TRUNC));
  CHECK(static_cast<std::uint32_t>(Create | Truncate | Exclusive | ReadWrite) == static_cast<std::uint32_t>(O_RDWR | O_CREAT | O_EXCL | O_TRUNC));
  CHECK(static_cast<std::uint32_t>(Exclusive | ReadWrite | Create | Truncate) == static_cast<std::uint32_t>(O_RDWR | O_CREAT | O_EXCL | O_TRUNC));
  CHECK(static_cast<std::uint32_t>(Exclusive | ReadWrite | Truncate | Create) == static_cast<std::uint32_t>(O_RDWR | O_CREAT | O_EXCL | O_TRUNC));
  CHECK(static_cast<std::uint32_t>(Exclusive | Create | ReadWrite | Truncate) == static_cast<std::uint32_t>(O_RDWR | O_CREAT | O_EXCL | O_TRUNC));
  CHECK(static_cast<std::uint32_t>(Exclusive | Create | Truncate | ReadWrite) == static_cast<std::uint32_t>(O_RDWR | O_CREAT | O_EXCL | O_TRUNC));
  CHECK(static_cast<std::uint32_t>(Exclusive | Truncate | ReadWrite | Create) == static_cast<std::uint32_t>(O_RDWR | O_CREAT | O_EXCL | O_TRUNC));
  CHECK(static_cast<std::uint32_t>(Exclusive | Truncate | Create | ReadWrite) == static_cast<std::uint32_t>(O_RDWR | O_CREAT | O_EXCL | O_TRUNC));
  CHECK(static_cast<std::uint32_t>(Truncate | ReadWrite | Create | Exclusive) == static_cast<std::uint32_t>(O_RDWR | O_CREAT | O_EXCL | O_TRUNC));
  CHECK(static_cast<std::uint32_t>(Truncate | ReadWrite | Exclusive | Create) == static_cast<std::uint32_t>(O_RDWR | O_CREAT | O_EXCL | O_TRUNC));
  CHECK(static_cast<std::uint32_t>(Truncate | Create | ReadWrite | Exclusive) == static_cast<std::uint32_t>(O_RDWR | O_CREAT | O_EXCL | O_TRUNC));
  CHECK(static_cast<std::uint32_t>(Truncate | Create | Exclusive | ReadWrite) == static_cast<std::uint32_t>(O_RDWR | O_CREAT | O_EXCL | O_TRUNC));
  CHECK(static_cast<std::uint32_t>(Truncate | Exclusive | ReadWrite | Create) == static_cast<std::uint32_t>(O_RDWR | O_CREAT | O_EXCL | O_TRUNC));
  CHECK(static_cast<std::uint32_t>(Truncate | Exclusive | Create | ReadWrite) == static_cast<std::uint32_t>(O_RDWR | O_CREAT | O_EXCL | O_TRUNC));
}

#include <catch2/catch.hpp>
#include <lpipp/mq.hpp>

using namespace lpipp::mq_constants;

TEST_CASE("open mode combinations") {
  REQUIRE(static_cast<int>(read_only) == (O_RDONLY));
  REQUIRE(static_cast<int>(read_only | create) == (O_RDONLY | O_CREAT));
  REQUIRE(static_cast<int>(create | read_only) == (O_RDONLY | O_CREAT));
  REQUIRE(static_cast<int>(read_only | create | exclusive) == (O_RDONLY | O_CREAT | O_EXCL));
  REQUIRE(static_cast<int>(read_only | exclusive | create) == (O_RDONLY | O_CREAT | O_EXCL));
  REQUIRE(static_cast<int>(create | read_only | exclusive) == (O_RDONLY | O_CREAT | O_EXCL));
  REQUIRE(static_cast<int>(create | exclusive | read_only) == (O_RDONLY | O_CREAT | O_EXCL));
  REQUIRE(static_cast<int>(exclusive | read_only | create) == (O_RDONLY | O_CREAT | O_EXCL));
  REQUIRE(static_cast<int>(exclusive | create | read_only) == (O_RDONLY | O_CREAT | O_EXCL));
  REQUIRE(static_cast<int>(read_only | create | exclusive | close_on_exec)
          == (O_RDONLY | O_CREAT | O_EXCL | O_CLOEXEC));
  REQUIRE(static_cast<int>(read_only | create | close_on_exec | exclusive)
          == (O_RDONLY | O_CREAT | O_EXCL | O_CLOEXEC));
  REQUIRE(static_cast<int>(read_only | exclusive | create | close_on_exec)
          == (O_RDONLY | O_CREAT | O_EXCL | O_CLOEXEC));
  REQUIRE(static_cast<int>(read_only | exclusive | close_on_exec | create)
          == (O_RDONLY | O_CREAT | O_EXCL | O_CLOEXEC));
  REQUIRE(static_cast<int>(read_only | close_on_exec | create | exclusive)
          == (O_RDONLY | O_CREAT | O_EXCL | O_CLOEXEC));
  REQUIRE(static_cast<int>(read_only | close_on_exec | exclusive | create)
          == (O_RDONLY | O_CREAT | O_EXCL | O_CLOEXEC));
  REQUIRE(static_cast<int>(create | read_only | exclusive | close_on_exec)
          == (O_RDONLY | O_CREAT | O_EXCL | O_CLOEXEC));
  REQUIRE(static_cast<int>(create | read_only | close_on_exec | exclusive)
          == (O_RDONLY | O_CREAT | O_EXCL | O_CLOEXEC));
  REQUIRE(static_cast<int>(create | exclusive | read_only | close_on_exec)
          == (O_RDONLY | O_CREAT | O_EXCL | O_CLOEXEC));
  REQUIRE(static_cast<int>(create | exclusive | close_on_exec | read_only)
          == (O_RDONLY | O_CREAT | O_EXCL | O_CLOEXEC));
  REQUIRE(static_cast<int>(create | close_on_exec | read_only | exclusive)
          == (O_RDONLY | O_CREAT | O_EXCL | O_CLOEXEC));
  REQUIRE(static_cast<int>(create | close_on_exec | exclusive | read_only)
          == (O_RDONLY | O_CREAT | O_EXCL | O_CLOEXEC));
  REQUIRE(static_cast<int>(exclusive | read_only | create | close_on_exec)
          == (O_RDONLY | O_CREAT | O_EXCL | O_CLOEXEC));
  REQUIRE(static_cast<int>(exclusive | read_only | close_on_exec | create)
          == (O_RDONLY | O_CREAT | O_EXCL | O_CLOEXEC));
  REQUIRE(static_cast<int>(exclusive | create | read_only | close_on_exec)
          == (O_RDONLY | O_CREAT | O_EXCL | O_CLOEXEC));
  REQUIRE(static_cast<int>(exclusive | create | close_on_exec | read_only)
          == (O_RDONLY | O_CREAT | O_EXCL | O_CLOEXEC));
  REQUIRE(static_cast<int>(exclusive | close_on_exec | read_only | create)
          == (O_RDONLY | O_CREAT | O_EXCL | O_CLOEXEC));
  REQUIRE(static_cast<int>(exclusive | close_on_exec | create | read_only)
          == (O_RDONLY | O_CREAT | O_EXCL | O_CLOEXEC));
  REQUIRE(static_cast<int>(close_on_exec | read_only | create | exclusive)
          == (O_RDONLY | O_CREAT | O_EXCL | O_CLOEXEC));
  REQUIRE(static_cast<int>(close_on_exec | read_only | exclusive | create)
          == (O_RDONLY | O_CREAT | O_EXCL | O_CLOEXEC));
  REQUIRE(static_cast<int>(close_on_exec | create | read_only | exclusive)
          == (O_RDONLY | O_CREAT | O_EXCL | O_CLOEXEC));
  REQUIRE(static_cast<int>(close_on_exec | create | exclusive | read_only)
          == (O_RDONLY | O_CREAT | O_EXCL | O_CLOEXEC));
  REQUIRE(static_cast<int>(close_on_exec | exclusive | read_only | create)
          == (O_RDONLY | O_CREAT | O_EXCL | O_CLOEXEC));
  REQUIRE(static_cast<int>(close_on_exec | exclusive | create | read_only)
          == (O_RDONLY | O_CREAT | O_EXCL | O_CLOEXEC));
  REQUIRE(static_cast<int>(read_only | create | exclusive | close_on_exec | nonblock)
          == (O_RDONLY | O_CREAT | O_EXCL | O_CLOEXEC | O_NONBLOCK));
  REQUIRE(static_cast<int>(read_only | create | exclusive | nonblock | close_on_exec)
          == (O_RDONLY | O_CREAT | O_EXCL | O_CLOEXEC | O_NONBLOCK));
  REQUIRE(static_cast<int>(read_only | create | close_on_exec | exclusive | nonblock)
          == (O_RDONLY | O_CREAT | O_EXCL | O_CLOEXEC | O_NONBLOCK));
  REQUIRE(static_cast<int>(read_only | create | close_on_exec | nonblock | exclusive)
          == (O_RDONLY | O_CREAT | O_EXCL | O_CLOEXEC | O_NONBLOCK));
  REQUIRE(static_cast<int>(read_only | create | nonblock | exclusive | close_on_exec)
          == (O_RDONLY | O_CREAT | O_EXCL | O_CLOEXEC | O_NONBLOCK));
  REQUIRE(static_cast<int>(read_only | create | nonblock | close_on_exec | exclusive)
          == (O_RDONLY | O_CREAT | O_EXCL | O_CLOEXEC | O_NONBLOCK));
  REQUIRE(static_cast<int>(read_only | exclusive | create | close_on_exec | nonblock)
          == (O_RDONLY | O_CREAT | O_EXCL | O_CLOEXEC | O_NONBLOCK));
  REQUIRE(static_cast<int>(read_only | exclusive | create | nonblock | close_on_exec)
          == (O_RDONLY | O_CREAT | O_EXCL | O_CLOEXEC | O_NONBLOCK));
  REQUIRE(static_cast<int>(read_only | exclusive | close_on_exec | create | nonblock)
          == (O_RDONLY | O_CREAT | O_EXCL | O_CLOEXEC | O_NONBLOCK));
  REQUIRE(static_cast<int>(read_only | exclusive | close_on_exec | nonblock | create)
          == (O_RDONLY | O_CREAT | O_EXCL | O_CLOEXEC | O_NONBLOCK));
  REQUIRE(static_cast<int>(read_only | exclusive | nonblock | create | close_on_exec)
          == (O_RDONLY | O_CREAT | O_EXCL | O_CLOEXEC | O_NONBLOCK));
  REQUIRE(static_cast<int>(read_only | exclusive | nonblock | close_on_exec | create)
          == (O_RDONLY | O_CREAT | O_EXCL | O_CLOEXEC | O_NONBLOCK));
  REQUIRE(static_cast<int>(read_only | close_on_exec | create | exclusive | nonblock)
          == (O_RDONLY | O_CREAT | O_EXCL | O_CLOEXEC | O_NONBLOCK));
  REQUIRE(static_cast<int>(read_only | close_on_exec | create | nonblock | exclusive)
          == (O_RDONLY | O_CREAT | O_EXCL | O_CLOEXEC | O_NONBLOCK));
  REQUIRE(static_cast<int>(read_only | close_on_exec | exclusive | create | nonblock)
          == (O_RDONLY | O_CREAT | O_EXCL | O_CLOEXEC | O_NONBLOCK));
  REQUIRE(static_cast<int>(read_only | close_on_exec | exclusive | nonblock | create)
          == (O_RDONLY | O_CREAT | O_EXCL | O_CLOEXEC | O_NONBLOCK));
  REQUIRE(static_cast<int>(read_only | close_on_exec | nonblock | create | exclusive)
          == (O_RDONLY | O_CREAT | O_EXCL | O_CLOEXEC | O_NONBLOCK));
  REQUIRE(static_cast<int>(read_only | close_on_exec | nonblock | exclusive | create)
          == (O_RDONLY | O_CREAT | O_EXCL | O_CLOEXEC | O_NONBLOCK));
  REQUIRE(static_cast<int>(read_only | nonblock | create | exclusive | close_on_exec)
          == (O_RDONLY | O_CREAT | O_EXCL | O_CLOEXEC | O_NONBLOCK));
  REQUIRE(static_cast<int>(read_only | nonblock | create | close_on_exec | exclusive)
          == (O_RDONLY | O_CREAT | O_EXCL | O_CLOEXEC | O_NONBLOCK));
  REQUIRE(static_cast<int>(read_only | nonblock | exclusive | create | close_on_exec)
          == (O_RDONLY | O_CREAT | O_EXCL | O_CLOEXEC | O_NONBLOCK));
  REQUIRE(static_cast<int>(read_only | nonblock | exclusive | close_on_exec | create)
          == (O_RDONLY | O_CREAT | O_EXCL | O_CLOEXEC | O_NONBLOCK));
  REQUIRE(static_cast<int>(read_only | nonblock | close_on_exec | create | exclusive)
          == (O_RDONLY | O_CREAT | O_EXCL | O_CLOEXEC | O_NONBLOCK));
  REQUIRE(static_cast<int>(read_only | nonblock | close_on_exec | exclusive | create)
          == (O_RDONLY | O_CREAT | O_EXCL | O_CLOEXEC | O_NONBLOCK));
  REQUIRE(static_cast<int>(create | read_only | exclusive | close_on_exec | nonblock)
          == (O_RDONLY | O_CREAT | O_EXCL | O_CLOEXEC | O_NONBLOCK));
  REQUIRE(static_cast<int>(create | read_only | exclusive | nonblock | close_on_exec)
          == (O_RDONLY | O_CREAT | O_EXCL | O_CLOEXEC | O_NONBLOCK));
  REQUIRE(static_cast<int>(create | read_only | close_on_exec | exclusive | nonblock)
          == (O_RDONLY | O_CREAT | O_EXCL | O_CLOEXEC | O_NONBLOCK));
  REQUIRE(static_cast<int>(create | read_only | close_on_exec | nonblock | exclusive)
          == (O_RDONLY | O_CREAT | O_EXCL | O_CLOEXEC | O_NONBLOCK));
  REQUIRE(static_cast<int>(create | read_only | nonblock | exclusive | close_on_exec)
          == (O_RDONLY | O_CREAT | O_EXCL | O_CLOEXEC | O_NONBLOCK));
  REQUIRE(static_cast<int>(create | read_only | nonblock | close_on_exec | exclusive)
          == (O_RDONLY | O_CREAT | O_EXCL | O_CLOEXEC | O_NONBLOCK));
  REQUIRE(static_cast<int>(create | exclusive | read_only | close_on_exec | nonblock)
          == (O_RDONLY | O_CREAT | O_EXCL | O_CLOEXEC | O_NONBLOCK));
  REQUIRE(static_cast<int>(create | exclusive | read_only | nonblock | close_on_exec)
          == (O_RDONLY | O_CREAT | O_EXCL | O_CLOEXEC | O_NONBLOCK));
  REQUIRE(static_cast<int>(create | exclusive | close_on_exec | read_only | nonblock)
          == (O_RDONLY | O_CREAT | O_EXCL | O_CLOEXEC | O_NONBLOCK));
  REQUIRE(static_cast<int>(create | exclusive | close_on_exec | nonblock | read_only)
          == (O_RDONLY | O_CREAT | O_EXCL | O_CLOEXEC | O_NONBLOCK));
  REQUIRE(static_cast<int>(create | exclusive | nonblock | read_only | close_on_exec)
          == (O_RDONLY | O_CREAT | O_EXCL | O_CLOEXEC | O_NONBLOCK));
  REQUIRE(static_cast<int>(create | exclusive | nonblock | close_on_exec | read_only)
          == (O_RDONLY | O_CREAT | O_EXCL | O_CLOEXEC | O_NONBLOCK));
  REQUIRE(static_cast<int>(create | close_on_exec | read_only | exclusive | nonblock)
          == (O_RDONLY | O_CREAT | O_EXCL | O_CLOEXEC | O_NONBLOCK));
  REQUIRE(static_cast<int>(create | close_on_exec | read_only | nonblock | exclusive)
          == (O_RDONLY | O_CREAT | O_EXCL | O_CLOEXEC | O_NONBLOCK));
  REQUIRE(static_cast<int>(create | close_on_exec | exclusive | read_only | nonblock)
          == (O_RDONLY | O_CREAT | O_EXCL | O_CLOEXEC | O_NONBLOCK));
  REQUIRE(static_cast<int>(create | close_on_exec | exclusive | nonblock | read_only)
          == (O_RDONLY | O_CREAT | O_EXCL | O_CLOEXEC | O_NONBLOCK));
  REQUIRE(static_cast<int>(create | close_on_exec | nonblock | read_only | exclusive)
          == (O_RDONLY | O_CREAT | O_EXCL | O_CLOEXEC | O_NONBLOCK));
  REQUIRE(static_cast<int>(create | close_on_exec | nonblock | exclusive | read_only)
          == (O_RDONLY | O_CREAT | O_EXCL | O_CLOEXEC | O_NONBLOCK));
  REQUIRE(static_cast<int>(create | nonblock | read_only | exclusive | close_on_exec)
          == (O_RDONLY | O_CREAT | O_EXCL | O_CLOEXEC | O_NONBLOCK));
  REQUIRE(static_cast<int>(create | nonblock | read_only | close_on_exec | exclusive)
          == (O_RDONLY | O_CREAT | O_EXCL | O_CLOEXEC | O_NONBLOCK));
  REQUIRE(static_cast<int>(create | nonblock | exclusive | read_only | close_on_exec)
          == (O_RDONLY | O_CREAT | O_EXCL | O_CLOEXEC | O_NONBLOCK));
  REQUIRE(static_cast<int>(create | nonblock | exclusive | close_on_exec | read_only)
          == (O_RDONLY | O_CREAT | O_EXCL | O_CLOEXEC | O_NONBLOCK));
  REQUIRE(static_cast<int>(create | nonblock | close_on_exec | read_only | exclusive)
          == (O_RDONLY | O_CREAT | O_EXCL | O_CLOEXEC | O_NONBLOCK));
  REQUIRE(static_cast<int>(create | nonblock | close_on_exec | exclusive | read_only)
          == (O_RDONLY | O_CREAT | O_EXCL | O_CLOEXEC | O_NONBLOCK));
  REQUIRE(static_cast<int>(exclusive | read_only | create | close_on_exec | nonblock)
          == (O_RDONLY | O_CREAT | O_EXCL | O_CLOEXEC | O_NONBLOCK));
  REQUIRE(static_cast<int>(exclusive | read_only | create | nonblock | close_on_exec)
          == (O_RDONLY | O_CREAT | O_EXCL | O_CLOEXEC | O_NONBLOCK));
  REQUIRE(static_cast<int>(exclusive | read_only | close_on_exec | create | nonblock)
          == (O_RDONLY | O_CREAT | O_EXCL | O_CLOEXEC | O_NONBLOCK));
  REQUIRE(static_cast<int>(exclusive | read_only | close_on_exec | nonblock | create)
          == (O_RDONLY | O_CREAT | O_EXCL | O_CLOEXEC | O_NONBLOCK));
  REQUIRE(static_cast<int>(exclusive | read_only | nonblock | create | close_on_exec)
          == (O_RDONLY | O_CREAT | O_EXCL | O_CLOEXEC | O_NONBLOCK));
  REQUIRE(static_cast<int>(exclusive | read_only | nonblock | close_on_exec | create)
          == (O_RDONLY | O_CREAT | O_EXCL | O_CLOEXEC | O_NONBLOCK));
  REQUIRE(static_cast<int>(exclusive | create | read_only | close_on_exec | nonblock)
          == (O_RDONLY | O_CREAT | O_EXCL | O_CLOEXEC | O_NONBLOCK));
  REQUIRE(static_cast<int>(exclusive | create | read_only | nonblock | close_on_exec)
          == (O_RDONLY | O_CREAT | O_EXCL | O_CLOEXEC | O_NONBLOCK));
  REQUIRE(static_cast<int>(exclusive | create | close_on_exec | read_only | nonblock)
          == (O_RDONLY | O_CREAT | O_EXCL | O_CLOEXEC | O_NONBLOCK));
  REQUIRE(static_cast<int>(exclusive | create | close_on_exec | nonblock | read_only)
          == (O_RDONLY | O_CREAT | O_EXCL | O_CLOEXEC | O_NONBLOCK));
  REQUIRE(static_cast<int>(exclusive | create | nonblock | read_only | close_on_exec)
          == (O_RDONLY | O_CREAT | O_EXCL | O_CLOEXEC | O_NONBLOCK));
  REQUIRE(static_cast<int>(exclusive | create | nonblock | close_on_exec | read_only)
          == (O_RDONLY | O_CREAT | O_EXCL | O_CLOEXEC | O_NONBLOCK));
  REQUIRE(static_cast<int>(exclusive | close_on_exec | read_only | create | nonblock)
          == (O_RDONLY | O_CREAT | O_EXCL | O_CLOEXEC | O_NONBLOCK));
  REQUIRE(static_cast<int>(exclusive | close_on_exec | read_only | nonblock | create)
          == (O_RDONLY | O_CREAT | O_EXCL | O_CLOEXEC | O_NONBLOCK));
  REQUIRE(static_cast<int>(exclusive | close_on_exec | create | read_only | nonblock)
          == (O_RDONLY | O_CREAT | O_EXCL | O_CLOEXEC | O_NONBLOCK));
  REQUIRE(static_cast<int>(exclusive | close_on_exec | create | nonblock | read_only)
          == (O_RDONLY | O_CREAT | O_EXCL | O_CLOEXEC | O_NONBLOCK));
  REQUIRE(static_cast<int>(exclusive | close_on_exec | nonblock | read_only | create)
          == (O_RDONLY | O_CREAT | O_EXCL | O_CLOEXEC | O_NONBLOCK));
  REQUIRE(static_cast<int>(exclusive | close_on_exec | nonblock | create | read_only)
          == (O_RDONLY | O_CREAT | O_EXCL | O_CLOEXEC | O_NONBLOCK));
  REQUIRE(static_cast<int>(exclusive | nonblock | read_only | create | close_on_exec)
          == (O_RDONLY | O_CREAT | O_EXCL | O_CLOEXEC | O_NONBLOCK));
  REQUIRE(static_cast<int>(exclusive | nonblock | read_only | close_on_exec | create)
          == (O_RDONLY | O_CREAT | O_EXCL | O_CLOEXEC | O_NONBLOCK));
  REQUIRE(static_cast<int>(exclusive | nonblock | create | read_only | close_on_exec)
          == (O_RDONLY | O_CREAT | O_EXCL | O_CLOEXEC | O_NONBLOCK));
  REQUIRE(static_cast<int>(exclusive | nonblock | create | close_on_exec | read_only)
          == (O_RDONLY | O_CREAT | O_EXCL | O_CLOEXEC | O_NONBLOCK));
  REQUIRE(static_cast<int>(exclusive | nonblock | close_on_exec | read_only | create)
          == (O_RDONLY | O_CREAT | O_EXCL | O_CLOEXEC | O_NONBLOCK));
  REQUIRE(static_cast<int>(exclusive | nonblock | close_on_exec | create | read_only)
          == (O_RDONLY | O_CREAT | O_EXCL | O_CLOEXEC | O_NONBLOCK));
  REQUIRE(static_cast<int>(close_on_exec | read_only | create | exclusive | nonblock)
          == (O_RDONLY | O_CREAT | O_EXCL | O_CLOEXEC | O_NONBLOCK));
  REQUIRE(static_cast<int>(close_on_exec | read_only | create | nonblock | exclusive)
          == (O_RDONLY | O_CREAT | O_EXCL | O_CLOEXEC | O_NONBLOCK));
  REQUIRE(static_cast<int>(close_on_exec | read_only | exclusive | create | nonblock)
          == (O_RDONLY | O_CREAT | O_EXCL | O_CLOEXEC | O_NONBLOCK));
  REQUIRE(static_cast<int>(close_on_exec | read_only | exclusive | nonblock | create)
          == (O_RDONLY | O_CREAT | O_EXCL | O_CLOEXEC | O_NONBLOCK));
  REQUIRE(static_cast<int>(close_on_exec | read_only | nonblock | create | exclusive)
          == (O_RDONLY | O_CREAT | O_EXCL | O_CLOEXEC | O_NONBLOCK));
  REQUIRE(static_cast<int>(close_on_exec | read_only | nonblock | exclusive | create)
          == (O_RDONLY | O_CREAT | O_EXCL | O_CLOEXEC | O_NONBLOCK));
  REQUIRE(static_cast<int>(close_on_exec | create | read_only | exclusive | nonblock)
          == (O_RDONLY | O_CREAT | O_EXCL | O_CLOEXEC | O_NONBLOCK));
  REQUIRE(static_cast<int>(close_on_exec | create | read_only | nonblock | exclusive)
          == (O_RDONLY | O_CREAT | O_EXCL | O_CLOEXEC | O_NONBLOCK));
  REQUIRE(static_cast<int>(close_on_exec | create | exclusive | read_only | nonblock)
          == (O_RDONLY | O_CREAT | O_EXCL | O_CLOEXEC | O_NONBLOCK));
  REQUIRE(static_cast<int>(close_on_exec | create | exclusive | nonblock | read_only)
          == (O_RDONLY | O_CREAT | O_EXCL | O_CLOEXEC | O_NONBLOCK));
  REQUIRE(static_cast<int>(close_on_exec | create | nonblock | read_only | exclusive)
          == (O_RDONLY | O_CREAT | O_EXCL | O_CLOEXEC | O_NONBLOCK));
  REQUIRE(static_cast<int>(close_on_exec | create | nonblock | exclusive | read_only)
          == (O_RDONLY | O_CREAT | O_EXCL | O_CLOEXEC | O_NONBLOCK));
  REQUIRE(static_cast<int>(close_on_exec | exclusive | read_only | create | nonblock)
          == (O_RDONLY | O_CREAT | O_EXCL | O_CLOEXEC | O_NONBLOCK));
  REQUIRE(static_cast<int>(close_on_exec | exclusive | read_only | nonblock | create)
          == (O_RDONLY | O_CREAT | O_EXCL | O_CLOEXEC | O_NONBLOCK));
  REQUIRE(static_cast<int>(close_on_exec | exclusive | create | read_only | nonblock)
          == (O_RDONLY | O_CREAT | O_EXCL | O_CLOEXEC | O_NONBLOCK));
  REQUIRE(static_cast<int>(close_on_exec | exclusive | create | nonblock | read_only)
          == (O_RDONLY | O_CREAT | O_EXCL | O_CLOEXEC | O_NONBLOCK));
  REQUIRE(static_cast<int>(close_on_exec | exclusive | nonblock | read_only | create)
          == (O_RDONLY | O_CREAT | O_EXCL | O_CLOEXEC | O_NONBLOCK));
  REQUIRE(static_cast<int>(close_on_exec | exclusive | nonblock | create | read_only)
          == (O_RDONLY | O_CREAT | O_EXCL | O_CLOEXEC | O_NONBLOCK));
  REQUIRE(static_cast<int>(close_on_exec | nonblock | read_only | create | exclusive)
          == (O_RDONLY | O_CREAT | O_EXCL | O_CLOEXEC | O_NONBLOCK));
  REQUIRE(static_cast<int>(close_on_exec | nonblock | read_only | exclusive | create)
          == (O_RDONLY | O_CREAT | O_EXCL | O_CLOEXEC | O_NONBLOCK));
  REQUIRE(static_cast<int>(close_on_exec | nonblock | create | read_only | exclusive)
          == (O_RDONLY | O_CREAT | O_EXCL | O_CLOEXEC | O_NONBLOCK));
  REQUIRE(static_cast<int>(close_on_exec | nonblock | create | exclusive | read_only)
          == (O_RDONLY | O_CREAT | O_EXCL | O_CLOEXEC | O_NONBLOCK));
  REQUIRE(static_cast<int>(close_on_exec | nonblock | exclusive | read_only | create)
          == (O_RDONLY | O_CREAT | O_EXCL | O_CLOEXEC | O_NONBLOCK));
  REQUIRE(static_cast<int>(close_on_exec | nonblock | exclusive | create | read_only)
          == (O_RDONLY | O_CREAT | O_EXCL | O_CLOEXEC | O_NONBLOCK));
  REQUIRE(static_cast<int>(nonblock | read_only | create | exclusive | close_on_exec)
          == (O_RDONLY | O_CREAT | O_EXCL | O_CLOEXEC | O_NONBLOCK));
  REQUIRE(static_cast<int>(nonblock | read_only | create | close_on_exec | exclusive)
          == (O_RDONLY | O_CREAT | O_EXCL | O_CLOEXEC | O_NONBLOCK));
  REQUIRE(static_cast<int>(nonblock | read_only | exclusive | create | close_on_exec)
          == (O_RDONLY | O_CREAT | O_EXCL | O_CLOEXEC | O_NONBLOCK));
  REQUIRE(static_cast<int>(nonblock | read_only | exclusive | close_on_exec | create)
          == (O_RDONLY | O_CREAT | O_EXCL | O_CLOEXEC | O_NONBLOCK));
  REQUIRE(static_cast<int>(nonblock | read_only | close_on_exec | create | exclusive)
          == (O_RDONLY | O_CREAT | O_EXCL | O_CLOEXEC | O_NONBLOCK));
  REQUIRE(static_cast<int>(nonblock | read_only | close_on_exec | exclusive | create)
          == (O_RDONLY | O_CREAT | O_EXCL | O_CLOEXEC | O_NONBLOCK));
  REQUIRE(static_cast<int>(nonblock | create | read_only | exclusive | close_on_exec)
          == (O_RDONLY | O_CREAT | O_EXCL | O_CLOEXEC | O_NONBLOCK));
  REQUIRE(static_cast<int>(nonblock | create | read_only | close_on_exec | exclusive)
          == (O_RDONLY | O_CREAT | O_EXCL | O_CLOEXEC | O_NONBLOCK));
  REQUIRE(static_cast<int>(nonblock | create | exclusive | read_only | close_on_exec)
          == (O_RDONLY | O_CREAT | O_EXCL | O_CLOEXEC | O_NONBLOCK));
  REQUIRE(static_cast<int>(nonblock | create | exclusive | close_on_exec | read_only)
          == (O_RDONLY | O_CREAT | O_EXCL | O_CLOEXEC | O_NONBLOCK));
  REQUIRE(static_cast<int>(nonblock | create | close_on_exec | read_only | exclusive)
          == (O_RDONLY | O_CREAT | O_EXCL | O_CLOEXEC | O_NONBLOCK));
  REQUIRE(static_cast<int>(nonblock | create | close_on_exec | exclusive | read_only)
          == (O_RDONLY | O_CREAT | O_EXCL | O_CLOEXEC | O_NONBLOCK));
  REQUIRE(static_cast<int>(nonblock | exclusive | read_only | create | close_on_exec)
          == (O_RDONLY | O_CREAT | O_EXCL | O_CLOEXEC | O_NONBLOCK));
  REQUIRE(static_cast<int>(nonblock | exclusive | read_only | close_on_exec | create)
          == (O_RDONLY | O_CREAT | O_EXCL | O_CLOEXEC | O_NONBLOCK));
  REQUIRE(static_cast<int>(nonblock | exclusive | create | read_only | close_on_exec)
          == (O_RDONLY | O_CREAT | O_EXCL | O_CLOEXEC | O_NONBLOCK));
  REQUIRE(static_cast<int>(nonblock | exclusive | create | close_on_exec | read_only)
          == (O_RDONLY | O_CREAT | O_EXCL | O_CLOEXEC | O_NONBLOCK));
  REQUIRE(static_cast<int>(nonblock | exclusive | close_on_exec | read_only | create)
          == (O_RDONLY | O_CREAT | O_EXCL | O_CLOEXEC | O_NONBLOCK));
  REQUIRE(static_cast<int>(nonblock | exclusive | close_on_exec | create | read_only)
          == (O_RDONLY | O_CREAT | O_EXCL | O_CLOEXEC | O_NONBLOCK));
  REQUIRE(static_cast<int>(nonblock | close_on_exec | read_only | create | exclusive)
          == (O_RDONLY | O_CREAT | O_EXCL | O_CLOEXEC | O_NONBLOCK));
  REQUIRE(static_cast<int>(nonblock | close_on_exec | read_only | exclusive | create)
          == (O_RDONLY | O_CREAT | O_EXCL | O_CLOEXEC | O_NONBLOCK));
  REQUIRE(static_cast<int>(nonblock | close_on_exec | create | read_only | exclusive)
          == (O_RDONLY | O_CREAT | O_EXCL | O_CLOEXEC | O_NONBLOCK));
  REQUIRE(static_cast<int>(nonblock | close_on_exec | create | exclusive | read_only)
          == (O_RDONLY | O_CREAT | O_EXCL | O_CLOEXEC | O_NONBLOCK));
  REQUIRE(static_cast<int>(nonblock | close_on_exec | exclusive | read_only | create)
          == (O_RDONLY | O_CREAT | O_EXCL | O_CLOEXEC | O_NONBLOCK));
  REQUIRE(static_cast<int>(nonblock | close_on_exec | exclusive | create | read_only)
          == (O_RDONLY | O_CREAT | O_EXCL | O_CLOEXEC | O_NONBLOCK));
}

#include <catch2/catch.hpp>
#include <lpipp/epoll.hpp>

using namespace lpipp;
using namespace epoll_constants;

TEST_CASE("epoll event bitor operations") {
  CHECK(static_cast<std::uint32_t>(in) == static_cast<std::uint32_t>(EPOLLIN));
  CHECK(static_cast<std::uint32_t>(in | rdhup) == static_cast<std::uint32_t>(EPOLLIN | EPOLLRDHUP));
  CHECK(static_cast<std::uint32_t>(rdhup | in) == static_cast<std::uint32_t>(EPOLLIN | EPOLLRDHUP));
  CHECK(static_cast<std::uint32_t>(in | rdhup | wakeup) == static_cast<std::uint32_t>(EPOLLIN | EPOLLRDHUP | EPOLLWAKEUP));
  CHECK(static_cast<std::uint32_t>(in | wakeup | rdhup) == static_cast<std::uint32_t>(EPOLLIN | EPOLLRDHUP | EPOLLWAKEUP));
  CHECK(static_cast<std::uint32_t>(rdhup | in | wakeup) == static_cast<std::uint32_t>(EPOLLIN | EPOLLRDHUP | EPOLLWAKEUP));
  CHECK(static_cast<std::uint32_t>(rdhup | wakeup | in) == static_cast<std::uint32_t>(EPOLLIN | EPOLLRDHUP | EPOLLWAKEUP));
  CHECK(static_cast<std::uint32_t>(wakeup | in | rdhup) == static_cast<std::uint32_t>(EPOLLIN | EPOLLRDHUP | EPOLLWAKEUP));
  CHECK(static_cast<std::uint32_t>(wakeup | rdhup | in) == static_cast<std::uint32_t>(EPOLLIN | EPOLLRDHUP | EPOLLWAKEUP));
  CHECK(static_cast<std::uint32_t>(in | rdhup | wakeup | oneshot) == static_cast<std::uint32_t>(EPOLLIN | EPOLLRDHUP | EPOLLWAKEUP | EPOLLONESHOT));
  CHECK(static_cast<std::uint32_t>(in | rdhup | oneshot | wakeup) == static_cast<std::uint32_t>(EPOLLIN | EPOLLRDHUP | EPOLLWAKEUP | EPOLLONESHOT));
  CHECK(static_cast<std::uint32_t>(in | wakeup | rdhup | oneshot) == static_cast<std::uint32_t>(EPOLLIN | EPOLLRDHUP | EPOLLWAKEUP | EPOLLONESHOT));
  CHECK(static_cast<std::uint32_t>(in | wakeup | oneshot | rdhup) == static_cast<std::uint32_t>(EPOLLIN | EPOLLRDHUP | EPOLLWAKEUP | EPOLLONESHOT));
  CHECK(static_cast<std::uint32_t>(in | oneshot | rdhup | wakeup) == static_cast<std::uint32_t>(EPOLLIN | EPOLLRDHUP | EPOLLWAKEUP | EPOLLONESHOT));
  CHECK(static_cast<std::uint32_t>(in | oneshot | wakeup | rdhup) == static_cast<std::uint32_t>(EPOLLIN | EPOLLRDHUP | EPOLLWAKEUP | EPOLLONESHOT));
  CHECK(static_cast<std::uint32_t>(rdhup | in | wakeup | oneshot) == static_cast<std::uint32_t>(EPOLLIN | EPOLLRDHUP | EPOLLWAKEUP | EPOLLONESHOT));
  CHECK(static_cast<std::uint32_t>(rdhup | in | oneshot | wakeup) == static_cast<std::uint32_t>(EPOLLIN | EPOLLRDHUP | EPOLLWAKEUP | EPOLLONESHOT));
  CHECK(static_cast<std::uint32_t>(rdhup | wakeup | in | oneshot) == static_cast<std::uint32_t>(EPOLLIN | EPOLLRDHUP | EPOLLWAKEUP | EPOLLONESHOT));
  CHECK(static_cast<std::uint32_t>(rdhup | wakeup | oneshot | in) == static_cast<std::uint32_t>(EPOLLIN | EPOLLRDHUP | EPOLLWAKEUP | EPOLLONESHOT));
  CHECK(static_cast<std::uint32_t>(rdhup | oneshot | in | wakeup) == static_cast<std::uint32_t>(EPOLLIN | EPOLLRDHUP | EPOLLWAKEUP | EPOLLONESHOT));
  CHECK(static_cast<std::uint32_t>(rdhup | oneshot | wakeup | in) == static_cast<std::uint32_t>(EPOLLIN | EPOLLRDHUP | EPOLLWAKEUP | EPOLLONESHOT));
  CHECK(static_cast<std::uint32_t>(wakeup | in | rdhup | oneshot) == static_cast<std::uint32_t>(EPOLLIN | EPOLLRDHUP | EPOLLWAKEUP | EPOLLONESHOT));
  CHECK(static_cast<std::uint32_t>(wakeup | in | oneshot | rdhup) == static_cast<std::uint32_t>(EPOLLIN | EPOLLRDHUP | EPOLLWAKEUP | EPOLLONESHOT));
  CHECK(static_cast<std::uint32_t>(wakeup | rdhup | in | oneshot) == static_cast<std::uint32_t>(EPOLLIN | EPOLLRDHUP | EPOLLWAKEUP | EPOLLONESHOT));
  CHECK(static_cast<std::uint32_t>(wakeup | rdhup | oneshot | in) == static_cast<std::uint32_t>(EPOLLIN | EPOLLRDHUP | EPOLLWAKEUP | EPOLLONESHOT));
  CHECK(static_cast<std::uint32_t>(wakeup | oneshot | in | rdhup) == static_cast<std::uint32_t>(EPOLLIN | EPOLLRDHUP | EPOLLWAKEUP | EPOLLONESHOT));
  CHECK(static_cast<std::uint32_t>(wakeup | oneshot | rdhup | in) == static_cast<std::uint32_t>(EPOLLIN | EPOLLRDHUP | EPOLLWAKEUP | EPOLLONESHOT));
  CHECK(static_cast<std::uint32_t>(oneshot | in | rdhup | wakeup) == static_cast<std::uint32_t>(EPOLLIN | EPOLLRDHUP | EPOLLWAKEUP | EPOLLONESHOT));
  CHECK(static_cast<std::uint32_t>(oneshot | in | wakeup | rdhup) == static_cast<std::uint32_t>(EPOLLIN | EPOLLRDHUP | EPOLLWAKEUP | EPOLLONESHOT));
  CHECK(static_cast<std::uint32_t>(oneshot | rdhup | in | wakeup) == static_cast<std::uint32_t>(EPOLLIN | EPOLLRDHUP | EPOLLWAKEUP | EPOLLONESHOT));
  CHECK(static_cast<std::uint32_t>(oneshot | rdhup | wakeup | in) == static_cast<std::uint32_t>(EPOLLIN | EPOLLRDHUP | EPOLLWAKEUP | EPOLLONESHOT));
  CHECK(static_cast<std::uint32_t>(oneshot | wakeup | in | rdhup) == static_cast<std::uint32_t>(EPOLLIN | EPOLLRDHUP | EPOLLWAKEUP | EPOLLONESHOT));
  CHECK(static_cast<std::uint32_t>(oneshot | wakeup | rdhup | in) == static_cast<std::uint32_t>(EPOLLIN | EPOLLRDHUP | EPOLLWAKEUP | EPOLLONESHOT));
  CHECK(static_cast<std::uint32_t>(in) == static_cast<std::uint32_t>(EPOLLIN));
  CHECK(static_cast<std::uint32_t>(in | wakeup) == static_cast<std::uint32_t>(EPOLLIN | EPOLLWAKEUP));
  CHECK(static_cast<std::uint32_t>(wakeup | in) == static_cast<std::uint32_t>(EPOLLIN | EPOLLWAKEUP));
  CHECK(static_cast<std::uint32_t>(in | wakeup | exclusive) == static_cast<std::uint32_t>(EPOLLIN | EPOLLWAKEUP | EPOLLEXCLUSIVE));
  CHECK(static_cast<std::uint32_t>(in | exclusive | wakeup) == static_cast<std::uint32_t>(EPOLLIN | EPOLLWAKEUP | EPOLLEXCLUSIVE));
  CHECK(static_cast<std::uint32_t>(wakeup | in | exclusive) == static_cast<std::uint32_t>(EPOLLIN | EPOLLWAKEUP | EPOLLEXCLUSIVE));
  CHECK(static_cast<std::uint32_t>(wakeup | exclusive | in) == static_cast<std::uint32_t>(EPOLLIN | EPOLLWAKEUP | EPOLLEXCLUSIVE));
  CHECK(static_cast<std::uint32_t>(exclusive | in | wakeup) == static_cast<std::uint32_t>(EPOLLIN | EPOLLWAKEUP | EPOLLEXCLUSIVE));
  CHECK(static_cast<std::uint32_t>(exclusive | wakeup | in) == static_cast<std::uint32_t>(EPOLLIN | EPOLLWAKEUP | EPOLLEXCLUSIVE));
  CHECK(static_cast<std::uint32_t>(in) == static_cast<std::uint32_t>(EPOLLIN));
  CHECK(static_cast<std::uint32_t>(in | rdhup) == static_cast<std::uint32_t>(EPOLLIN | EPOLLRDHUP));
  CHECK(static_cast<std::uint32_t>(rdhup | in) == static_cast<std::uint32_t>(EPOLLIN | EPOLLRDHUP));
  CHECK(static_cast<std::uint32_t>(in | rdhup | err) == static_cast<std::uint32_t>(EPOLLIN | EPOLLRDHUP | EPOLLERR));
  CHECK(static_cast<std::uint32_t>(in | err | rdhup) == static_cast<std::uint32_t>(EPOLLIN | EPOLLRDHUP | EPOLLERR));
  CHECK(static_cast<std::uint32_t>(rdhup | in | err) == static_cast<std::uint32_t>(EPOLLIN | EPOLLRDHUP | EPOLLERR));
  CHECK(static_cast<std::uint32_t>(rdhup | err | in) == static_cast<std::uint32_t>(EPOLLIN | EPOLLRDHUP | EPOLLERR));
  CHECK(static_cast<std::uint32_t>(err | in | rdhup) == static_cast<std::uint32_t>(EPOLLIN | EPOLLRDHUP | EPOLLERR));
  CHECK(static_cast<std::uint32_t>(err | rdhup | in) == static_cast<std::uint32_t>(EPOLLIN | EPOLLRDHUP | EPOLLERR));
}

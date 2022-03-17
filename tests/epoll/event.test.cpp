#include <catch2/catch.hpp>
#include <lpipp/epoll.hpp>

using namespace lpipp;
using namespace epoll_constants;

TEST_CASE("epoll event bitor operations") {
  CHECK(static_cast<std::uint32_t>(In) == static_cast<std::uint32_t>(EPOLLIN));
  CHECK(static_cast<std::uint32_t>(In | Rdhup) == static_cast<std::uint32_t>(EPOLLIN | EPOLLRDHUP));
  CHECK(static_cast<std::uint32_t>(Rdhup | In) == static_cast<std::uint32_t>(EPOLLIN | EPOLLRDHUP));
  CHECK(static_cast<std::uint32_t>(In | Rdhup | Wakeup) == static_cast<std::uint32_t>(EPOLLIN | EPOLLRDHUP | EPOLLWAKEUP));
  CHECK(static_cast<std::uint32_t>(In | Wakeup | Rdhup) == static_cast<std::uint32_t>(EPOLLIN | EPOLLRDHUP | EPOLLWAKEUP));
  CHECK(static_cast<std::uint32_t>(Rdhup | In | Wakeup) == static_cast<std::uint32_t>(EPOLLIN | EPOLLRDHUP | EPOLLWAKEUP));
  CHECK(static_cast<std::uint32_t>(Rdhup | Wakeup | In) == static_cast<std::uint32_t>(EPOLLIN | EPOLLRDHUP | EPOLLWAKEUP));
  CHECK(static_cast<std::uint32_t>(Wakeup | In | Rdhup) == static_cast<std::uint32_t>(EPOLLIN | EPOLLRDHUP | EPOLLWAKEUP));
  CHECK(static_cast<std::uint32_t>(Wakeup | Rdhup | In) == static_cast<std::uint32_t>(EPOLLIN | EPOLLRDHUP | EPOLLWAKEUP));
  CHECK(static_cast<std::uint32_t>(In | Rdhup | Wakeup | Oneshot) == static_cast<std::uint32_t>(EPOLLIN | EPOLLRDHUP | EPOLLWAKEUP | EPOLLONESHOT));
  CHECK(static_cast<std::uint32_t>(In | Rdhup | Oneshot | Wakeup) == static_cast<std::uint32_t>(EPOLLIN | EPOLLRDHUP | EPOLLWAKEUP | EPOLLONESHOT));
  CHECK(static_cast<std::uint32_t>(In | Wakeup | Rdhup | Oneshot) == static_cast<std::uint32_t>(EPOLLIN | EPOLLRDHUP | EPOLLWAKEUP | EPOLLONESHOT));
  CHECK(static_cast<std::uint32_t>(In | Wakeup | Oneshot | Rdhup) == static_cast<std::uint32_t>(EPOLLIN | EPOLLRDHUP | EPOLLWAKEUP | EPOLLONESHOT));
  CHECK(static_cast<std::uint32_t>(In | Oneshot | Rdhup | Wakeup) == static_cast<std::uint32_t>(EPOLLIN | EPOLLRDHUP | EPOLLWAKEUP | EPOLLONESHOT));
  CHECK(static_cast<std::uint32_t>(In | Oneshot | Wakeup | Rdhup) == static_cast<std::uint32_t>(EPOLLIN | EPOLLRDHUP | EPOLLWAKEUP | EPOLLONESHOT));
  CHECK(static_cast<std::uint32_t>(Rdhup | In | Wakeup | Oneshot) == static_cast<std::uint32_t>(EPOLLIN | EPOLLRDHUP | EPOLLWAKEUP | EPOLLONESHOT));
  CHECK(static_cast<std::uint32_t>(Rdhup | In | Oneshot | Wakeup) == static_cast<std::uint32_t>(EPOLLIN | EPOLLRDHUP | EPOLLWAKEUP | EPOLLONESHOT));
  CHECK(static_cast<std::uint32_t>(Rdhup | Wakeup | In | Oneshot) == static_cast<std::uint32_t>(EPOLLIN | EPOLLRDHUP | EPOLLWAKEUP | EPOLLONESHOT));
  CHECK(static_cast<std::uint32_t>(Rdhup | Wakeup | Oneshot | In) == static_cast<std::uint32_t>(EPOLLIN | EPOLLRDHUP | EPOLLWAKEUP | EPOLLONESHOT));
  CHECK(static_cast<std::uint32_t>(Rdhup | Oneshot | In | Wakeup) == static_cast<std::uint32_t>(EPOLLIN | EPOLLRDHUP | EPOLLWAKEUP | EPOLLONESHOT));
  CHECK(static_cast<std::uint32_t>(Rdhup | Oneshot | Wakeup | In) == static_cast<std::uint32_t>(EPOLLIN | EPOLLRDHUP | EPOLLWAKEUP | EPOLLONESHOT));
  CHECK(static_cast<std::uint32_t>(Wakeup | In | Rdhup | Oneshot) == static_cast<std::uint32_t>(EPOLLIN | EPOLLRDHUP | EPOLLWAKEUP | EPOLLONESHOT));
  CHECK(static_cast<std::uint32_t>(Wakeup | In | Oneshot | Rdhup) == static_cast<std::uint32_t>(EPOLLIN | EPOLLRDHUP | EPOLLWAKEUP | EPOLLONESHOT));
  CHECK(static_cast<std::uint32_t>(Wakeup | Rdhup | In | Oneshot) == static_cast<std::uint32_t>(EPOLLIN | EPOLLRDHUP | EPOLLWAKEUP | EPOLLONESHOT));
  CHECK(static_cast<std::uint32_t>(Wakeup | Rdhup | Oneshot | In) == static_cast<std::uint32_t>(EPOLLIN | EPOLLRDHUP | EPOLLWAKEUP | EPOLLONESHOT));
  CHECK(static_cast<std::uint32_t>(Wakeup | Oneshot | In | Rdhup) == static_cast<std::uint32_t>(EPOLLIN | EPOLLRDHUP | EPOLLWAKEUP | EPOLLONESHOT));
  CHECK(static_cast<std::uint32_t>(Wakeup | Oneshot | Rdhup | In) == static_cast<std::uint32_t>(EPOLLIN | EPOLLRDHUP | EPOLLWAKEUP | EPOLLONESHOT));
  CHECK(static_cast<std::uint32_t>(Oneshot | In | Rdhup | Wakeup) == static_cast<std::uint32_t>(EPOLLIN | EPOLLRDHUP | EPOLLWAKEUP | EPOLLONESHOT));
  CHECK(static_cast<std::uint32_t>(Oneshot | In | Wakeup | Rdhup) == static_cast<std::uint32_t>(EPOLLIN | EPOLLRDHUP | EPOLLWAKEUP | EPOLLONESHOT));
  CHECK(static_cast<std::uint32_t>(Oneshot | Rdhup | In | Wakeup) == static_cast<std::uint32_t>(EPOLLIN | EPOLLRDHUP | EPOLLWAKEUP | EPOLLONESHOT));
  CHECK(static_cast<std::uint32_t>(Oneshot | Rdhup | Wakeup | In) == static_cast<std::uint32_t>(EPOLLIN | EPOLLRDHUP | EPOLLWAKEUP | EPOLLONESHOT));
  CHECK(static_cast<std::uint32_t>(Oneshot | Wakeup | In | Rdhup) == static_cast<std::uint32_t>(EPOLLIN | EPOLLRDHUP | EPOLLWAKEUP | EPOLLONESHOT));
  CHECK(static_cast<std::uint32_t>(Oneshot | Wakeup | Rdhup | In) == static_cast<std::uint32_t>(EPOLLIN | EPOLLRDHUP | EPOLLWAKEUP | EPOLLONESHOT));
  CHECK(static_cast<std::uint32_t>(In) == static_cast<std::uint32_t>(EPOLLIN));
  CHECK(static_cast<std::uint32_t>(In | Wakeup) == static_cast<std::uint32_t>(EPOLLIN | EPOLLWAKEUP));
  CHECK(static_cast<std::uint32_t>(Wakeup | In) == static_cast<std::uint32_t>(EPOLLIN | EPOLLWAKEUP));
  CHECK(static_cast<std::uint32_t>(In | Wakeup | Exclusive) == static_cast<std::uint32_t>(EPOLLIN | EPOLLWAKEUP | EPOLLEXCLUSIVE));
  CHECK(static_cast<std::uint32_t>(In | Exclusive | Wakeup) == static_cast<std::uint32_t>(EPOLLIN | EPOLLWAKEUP | EPOLLEXCLUSIVE));
  CHECK(static_cast<std::uint32_t>(Wakeup | In | Exclusive) == static_cast<std::uint32_t>(EPOLLIN | EPOLLWAKEUP | EPOLLEXCLUSIVE));
  CHECK(static_cast<std::uint32_t>(Wakeup | Exclusive | In) == static_cast<std::uint32_t>(EPOLLIN | EPOLLWAKEUP | EPOLLEXCLUSIVE));
  CHECK(static_cast<std::uint32_t>(Exclusive | In | Wakeup) == static_cast<std::uint32_t>(EPOLLIN | EPOLLWAKEUP | EPOLLEXCLUSIVE));
  CHECK(static_cast<std::uint32_t>(Exclusive | Wakeup | In) == static_cast<std::uint32_t>(EPOLLIN | EPOLLWAKEUP | EPOLLEXCLUSIVE));
  CHECK(static_cast<std::uint32_t>(In) == static_cast<std::uint32_t>(EPOLLIN));
  CHECK(static_cast<std::uint32_t>(In | Rdhup) == static_cast<std::uint32_t>(EPOLLIN | EPOLLRDHUP));
  CHECK(static_cast<std::uint32_t>(Rdhup | In) == static_cast<std::uint32_t>(EPOLLIN | EPOLLRDHUP));
  CHECK(static_cast<std::uint32_t>(In | Rdhup | Err) == static_cast<std::uint32_t>(EPOLLIN | EPOLLRDHUP | EPOLLERR));
  CHECK(static_cast<std::uint32_t>(In | Err | Rdhup) == static_cast<std::uint32_t>(EPOLLIN | EPOLLRDHUP | EPOLLERR));
  CHECK(static_cast<std::uint32_t>(Rdhup | In | Err) == static_cast<std::uint32_t>(EPOLLIN | EPOLLRDHUP | EPOLLERR));
  CHECK(static_cast<std::uint32_t>(Rdhup | Err | In) == static_cast<std::uint32_t>(EPOLLIN | EPOLLRDHUP | EPOLLERR));
  CHECK(static_cast<std::uint32_t>(Err | In | Rdhup) == static_cast<std::uint32_t>(EPOLLIN | EPOLLRDHUP | EPOLLERR));
  CHECK(static_cast<std::uint32_t>(Err | Rdhup | In) == static_cast<std::uint32_t>(EPOLLIN | EPOLLRDHUP | EPOLLERR));
}

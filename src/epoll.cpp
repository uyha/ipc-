#include <cerrno>
#include <fcntl.h>
#include <lpipp/epoll.hpp>
#include <sys/epoll.h>
#include <unistd.h>

namespace lpipp {
auto epoll::create(bool close_on_exec) noexcept -> tl::expected<epoll, CreateError> {
  auto fd = ::epoll_create1(close_on_exec ? EPOLL_CLOEXEC : 0);
  if (fd == -1) {
    return tl::unexpected{map_create_error(errno)};
  }

  return epoll{fd};
}

auto epoll::remove(int fd) const noexcept -> tl::expected<void, RemoveError> {
  auto result = ::epoll_ctl(m_fd, EPOLL_CTL_DEL, fd, nullptr);
  if (result == -1) {
    return tl::unexpected{RemoveError::file_descriptor_not_registered};
  }
  return {};
}

auto epoll::wait(::epoll_event *event_list,
                 int max_events,
                 int timeout_millisecs,
                 ::sigset_t const *sigmask) const noexcept -> tl::expected<int, WaitError> {
  auto result = ::epoll_pwait(m_fd, event_list, max_events, timeout_millisecs, sigmask);
  if (result == -1) {
    return tl::unexpected{map_wait_error(errno)};
  }
  if (result == 0) {
    return tl::unexpected{WaitError::timeout};
  }
  return result;
}

epoll::epoll(epoll &&other) noexcept
    : m_fd{other.m_fd} {
  other.m_fd = -1;
}
auto epoll::operator=(epoll &&other) noexcept -> epoll & {
  m_fd       = other.m_fd;
  other.m_fd = -1;

  return *this;
}

epoll::~epoll() noexcept {
  if (m_fd == -1) {
    return;
  }
  ::close(m_fd);
}

epoll::epoll(int fd) noexcept
    : m_fd{fd} {}

} // namespace lpipp

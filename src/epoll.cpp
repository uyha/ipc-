#include <cerrno>
#include <fcntl.h>
#include <lpipp/epoll.hpp>
#include <unistd.h>

namespace lpipp {
auto epoll::create(bool close_on_exec) noexcept -> tl::expected<epoll, CreateError> {
  auto fd = ::epoll_create1(close_on_exec ? FD_CLOEXEC : 0);
  if (fd == -1) {
    return tl::unexpected{map_create_error(errno)};
  }

  return epoll{fd};
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

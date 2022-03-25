#include <lpipp/un.h>
#include <unistd.h>

namespace lpipp {
un::un(un &&other) noexcept
    : m_fd{other.m_fd} {
  other.m_fd = -1;
}
auto un::operator=(un &&other) noexcept -> un & {
  m_fd       = other.m_fd;
  other.m_fd = -1;

  return *this;
}

un::~un() noexcept {
  if (m_fd == -1) {
    return;
  }
  ::close(m_fd);
}

un::un(int fd) noexcept
    : m_fd{fd} {}
} // namespace lpipp

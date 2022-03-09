#include <lpipp/shm.hpp>
#include <sys/mman.h>
#include <unistd.h>

namespace lpipp {
auto shm::open(char const *name, ReadOnlyMode mode) noexcept -> tl::expected<shm, OpenError> {
  return shm::open(name, static_cast<int>(mode), 0);
}
auto shm::open(char const *name, ReadWriteMode mode) noexcept -> tl::expected<shm, OpenError> {
  return shm::open(name, static_cast<int>(mode), 0);
}
auto shm::open(char const *name, ReadCreateMode mode, std::filesystem::perms permissions) noexcept
    -> tl::expected<shm, OpenError> {
  return shm::open(name, static_cast<int>(mode), static_cast<::mode_t>(permissions));
}
auto shm::open(char const *name, ReadCreateMode mode, ::mode_t permissions) noexcept -> tl::expected<shm, OpenError> {
  return shm::open(name, static_cast<int>(mode), permissions);
}
auto shm::open(char const *name, ReadWriteCreateMode mode, std::filesystem::perms permissions) noexcept
    -> tl::expected<shm, OpenError> {
  return shm::open(name, static_cast<int>(mode), static_cast<::mode_t>(permissions));
}
auto shm::open(char const *name, ReadWriteCreateMode mode, ::mode_t permissions) noexcept
    -> tl::expected<shm, OpenError> {
  return shm::open(name, static_cast<int>(mode), permissions);
}
auto shm::open(char const *name, int flags, ::mode_t mode) noexcept -> tl::expected<shm, OpenError> {
  using tl::unexpected;

  auto fd = ::shm_open(name, flags, mode);
  if (fd == -1) {
    return unexpected{map_open_error(errno)};
  }

  return shm{fd};
}

auto shm::unlink(char const *name) noexcept -> tl::expected<void, UnlinkError> {
  if (auto result = ::shm_unlink(name); result == 0) {
    return {};
  } else {
    return tl::unexpected{map_unlink_error(errno)};
  }
}

shm::shm(shm &&other) noexcept
    : m_fd{other.m_fd} {
  other.m_fd = -1;
}
auto shm::operator=(shm &&other) noexcept -> shm & {
  m_fd       = other.m_fd;
  other.m_fd = -1;

  return *this;
}

shm::~shm() noexcept {
  if (m_fd == -1) {
    return;
  }
  ::close(m_fd);
}

shm::shm(int fd) noexcept
    : m_fd{fd} {}
} // namespace lpipp

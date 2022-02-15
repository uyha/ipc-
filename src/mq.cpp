#include <errno.h>
#include <ipcpp/mq.hpp>
#include <mqueue.h>

namespace ipcpp {
auto mq::open(const char *name, OpenMode mode) noexcept -> tl::expected<mq, OpenError> {
  using tl::unexpected, tl::expected;

  if (not valid_name(name)) {
    return unexpected{OpenError::name_invalid};
  }

  auto fd = ::mq_open(name, static_cast<int>(mode));
  if (fd == -1) {
    return unexpected{map_open_error(errno)};
  }

  return mq{fd};
}
auto mq::open(const char *name, OpenCreateMode mode, std::filesystem::perms permissions) noexcept
    -> tl::expected<mq, OpenError> {
  using tl::unexpected, tl::expected;

  if (not valid_name(name)) {
    return unexpected{OpenError::name_invalid};
  }

  auto fd = ::mq_open(name, static_cast<int>(mode), permissions, nullptr);
  if (fd == -1) {
    return unexpected{map_open_error(errno)};
  }

  return mq{fd};
}
auto mq::open(const char *name,
              OpenCreateMode mode,
              std::filesystem::perms permissions,
              CreateAttributes attributes) noexcept -> tl::expected<mq, OpenError> {
  using tl::unexpected, tl::expected;

  if (not valid_name(name)) {
    return unexpected{OpenError::name_invalid};
  }

  auto mq_attributes = attributes.mq_attr();

  auto fd = ::mq_open(name, static_cast<int>(mode), permissions, &mq_attributes);
  if (fd == -1) {
    return unexpected{map_open_error(errno)};
  }

  return mq{fd};
}

auto mq::unlink(const char *name) noexcept -> int {
  if (auto result = ::mq_unlink(name); result == 0) {
    return 0;
  } else {
    return errno;
  }
}

mq::mq(mq &&other) noexcept
    : m_fd{other.m_fd} {
  other.m_fd = -1;
}
auto mq::operator=(mq &&other) noexcept -> mq & {
  this->~mq();

  m_fd       = other.m_fd;
  other.m_fd = -1;

  return *this;
}

mq::~mq() noexcept {
  if (m_fd == -1) {
    return;
  }
  ::mq_close(m_fd);
}

mq::mq(int fd) noexcept
    : m_fd{fd} {}
} // namespace ipcpp

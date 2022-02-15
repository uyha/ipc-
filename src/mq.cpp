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
  return mq::open(name, mode, static_cast<int>(permissions), nullptr);
}
auto mq::open(const char *name,
              OpenCreateMode mode,
              std::filesystem::perms permissions,
              CreateAttributes attributes) noexcept -> tl::expected<mq, OpenError> {
  auto mq_attributes = attributes.mq_attr();

  return mq::open(name, mode, static_cast<int>(permissions), &mq_attributes);
}
auto mq::open(const char *name, OpenCreateMode mode, int permissions) noexcept
    -> tl::expected<mq, OpenError> {
  return mq::open(name, mode, permissions, nullptr);
}
auto mq::open(const char *name,
              OpenCreateMode mode,
              int permissions,
              CreateAttributes attributes) noexcept -> tl::expected<mq, OpenError> {
  auto mq_attributes = attributes.mq_attr();

  return mq::open(name, mode, permissions, &mq_attributes);
}

auto mq::open(const char *name,
              OpenCreateMode mode,
              int permissions,
              ::mq_attr *attributes) noexcept -> tl::expected<mq, OpenError> {
  using tl::unexpected, tl::expected;

  if (not valid_name(name)) {
    return unexpected{OpenError::name_invalid};
  }

  auto fd = ::mq_open(name, static_cast<int>(mode), permissions, attributes);
  if (fd == -1) {
    return unexpected{map_open_error(errno)};
  }

  return mq{fd};
}

auto mq::unlink(const char *name) noexcept -> tl::expected<void, UnlinkError> {
  if (auto result = ::mq_unlink(name); result == 0) {
    return {};
  } else {
    return tl::unexpected{map_unlink_error(errno)};
  }
}

auto mq::get_attributes() noexcept -> ::mq_attr {
  auto result = ::mq_attr{};
  ::mq_getattr(m_fd, &result);
  return result;
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

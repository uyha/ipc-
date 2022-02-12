#include <errno.h>
#include <ipcpp/mq.hpp>
#include <mqueue.h>

namespace ipcpp {
auto mq::open(const char *name, OpenMode mode) noexcept
    -> tl::expected<mq, int> {
  using tl::unexpected, tl::expected;

  auto fd = ::mq_open(name, static_cast<int>(mode));
  if (fd == -1) {
    return unexpected{errno};
  }

  return mq{fd};
}
auto mq::open(const char *name,
                        OpenCreateMode mode,
                        std::filesystem::perms permissions) noexcept
    -> tl::expected<mq, int> {
  using tl::unexpected, tl::expected;

  auto fd = ::mq_open(name, static_cast<int>(mode), permissions, nullptr);
  if (fd == -1) {
    return unexpected{errno};
  }

  return mq{fd};
}
auto mq::open(const char *name,
                        OpenCreateMode mode,
                        std::filesystem::perms permissions,
                        long max_messages,
                        long message_size) noexcept -> tl::expected<mq, int> {
  using tl::unexpected, tl::expected;

  auto attribute       = ::mq_attr{};
  attribute.mq_maxmsg  = max_messages;
  attribute.mq_msgsize = message_size;

  auto fd = ::mq_open(name, static_cast<int>(mode), permissions, &attribute);
  if (fd == -1) {
    return unexpected{errno};
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

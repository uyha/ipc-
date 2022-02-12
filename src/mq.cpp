#include <errno.h>
#include <ipcpp/mq.hpp>
#include <mqueue.h>

namespace ipcpp {
auto MessageQueue::open(const char *name, OpenMode mode) noexcept
    -> tl::expected<MessageQueue, int> {
  using tl::unexpected, tl::expected;

  auto fd = ::mq_open(name, static_cast<int>(mode));
  if (fd == -1) {
    return unexpected{errno};
  }

  return MessageQueue{fd};
}
auto MessageQueue::open(const char *name,
                        OpenCreateMode mode,
                        std::filesystem::perms permissions) noexcept
    -> tl::expected<MessageQueue, int> {
  using tl::unexpected, tl::expected;

  auto fd = ::mq_open(name, static_cast<int>(mode), permissions, nullptr);
  if (fd == -1) {
    return unexpected{errno};
  }

  return MessageQueue{fd};
}
auto MessageQueue::open(const char *name,
                        OpenCreateMode mode,
                        std::filesystem::perms permissions,
                        long max_messages,
                        long message_size) noexcept -> tl::expected<MessageQueue, int> {
  using tl::unexpected, tl::expected;

  auto attribute       = ::mq_attr{};
  attribute.mq_maxmsg  = max_messages;
  attribute.mq_msgsize = message_size;

  auto fd = ::mq_open(name, static_cast<int>(mode), permissions, &attribute);
  if (fd == -1) {
    return unexpected{errno};
  }

  return MessageQueue{fd};
}

MessageQueue::MessageQueue(MessageQueue &&other) noexcept
    : m_fd{other.m_fd} {
  other.m_fd = -1;
}
auto MessageQueue::operator=(MessageQueue &&other) noexcept -> MessageQueue & {
  m_fd       = other.m_fd;
  other.m_fd = -1;

  return *this;
}

MessageQueue::~MessageQueue() noexcept {
  if (m_fd == -1) {
    return;
  }
  ::mq_close(m_fd);
}

MessageQueue::MessageQueue(int fd) noexcept
    : m_fd{fd} {}
} // namespace ipcpp

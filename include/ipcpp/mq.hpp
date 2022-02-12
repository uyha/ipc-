#pragma once

#include <fcntl.h>
#include <filesystem>
#include <tl/expected.hpp>

namespace ipcpp {
class MessageQueue {
public:
  enum class OpenMode : int {
    read_only  = O_RDONLY,
    write_only = O_WRONLY,
    read_write = O_RDWR,
  };
  enum class CreateMode : int {
    create    = O_CREAT,
    exclusive = O_EXCL,
  };
  enum class OpenCreateMode : int {};
  enum class ExtraFlag : int {
    close_on_exec = O_CLOEXEC,
    nonblock      = O_NONBLOCK,
  };
  friend constexpr auto operator|(OpenMode lhs, ExtraFlag rhs) -> OpenMode {
    return static_cast<OpenMode>(static_cast<int>(lhs) | static_cast<int>(rhs));
  }
  friend constexpr auto operator|(ExtraFlag lhs, OpenMode rhs) -> OpenMode {
    return rhs | lhs;
  }
  friend constexpr auto operator|(OpenMode lhs, CreateMode rhs) -> OpenCreateMode {
    return static_cast<OpenCreateMode>(static_cast<int>(lhs) | static_cast<int>(rhs));
  }
  friend constexpr auto operator|(CreateMode lhs, OpenMode rhs) -> OpenCreateMode {
    return rhs | lhs;
  }
  friend constexpr auto operator|(OpenCreateMode lhs, ExtraFlag rhs) -> OpenCreateMode {
    return static_cast<OpenCreateMode>(static_cast<int>(lhs) | static_cast<int>(rhs));
  }
  friend constexpr auto operator|(ExtraFlag lhs, OpenCreateMode rhs) -> OpenCreateMode {
    return rhs | lhs;
  }

  static auto open(char const *name, OpenMode mode) noexcept -> tl::expected<MessageQueue, int>;
  static auto open(char const *name,
                   OpenCreateMode mode,
                   std::filesystem::perms permission) noexcept -> tl::expected<MessageQueue, int>;
  static auto open(char const *name,
                   OpenCreateMode mode,
                   std::filesystem::perms permission, 
                   long max_messages,
                   long message_size) noexcept -> tl::expected<MessageQueue, int>;

  MessageQueue(MessageQueue const &) = delete;
  MessageQueue(MessageQueue &&other) noexcept;

  auto operator=(MessageQueue const &) -> MessageQueue & = delete;
  auto operator=(MessageQueue &&other) noexcept -> MessageQueue &;

  ~MessageQueue() noexcept;

private:
  MessageQueue(int fd) noexcept;

  int m_fd;
};
namespace mq_constants {
static constexpr MessageQueue::OpenMode read_only      = MessageQueue::OpenMode::read_only;
static constexpr MessageQueue::OpenMode write_only     = MessageQueue::OpenMode::write_only;
static constexpr MessageQueue::OpenMode read_write     = MessageQueue::OpenMode::read_write;
static constexpr MessageQueue::CreateMode create       = MessageQueue::CreateMode::create;
static constexpr MessageQueue::CreateMode exclusive    = MessageQueue::CreateMode::exclusive;
static constexpr MessageQueue::ExtraFlag close_on_exec = MessageQueue::ExtraFlag::close_on_exec;
static constexpr MessageQueue::ExtraFlag nonblock      = MessageQueue::ExtraFlag::nonblock;
} // namespace mq_constants
} // namespace ipcpp

#pragma once

#include <fcntl.h>
#include <filesystem>
#include <tl/expected.hpp>

namespace ipcpp {
class mq {
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

  static auto open(char const *name, OpenMode mode) noexcept -> tl::expected<mq, int>;
  static auto open(char const *name,
                   OpenCreateMode mode,
                   std::filesystem::perms permission) noexcept -> tl::expected<mq, int>;
  static auto open(char const *name,
                   OpenCreateMode mode,
                   std::filesystem::perms permission,
                   long max_messages,
                   long message_size) noexcept -> tl::expected<mq, int>;
  static auto unlink(char const *name) noexcept -> int;

  mq(mq const &) = delete;
  mq(mq &&other) noexcept;

  auto operator=(mq const &) -> mq & = delete;
  auto operator=(mq &&other) noexcept -> mq &;

  ~mq() noexcept;

private:
  mq(int fd) noexcept;

  int m_fd;
};
namespace mq_constants {
static constexpr mq::OpenMode read_only      = mq::OpenMode::read_only;
static constexpr mq::OpenMode write_only     = mq::OpenMode::write_only;
static constexpr mq::OpenMode read_write     = mq::OpenMode::read_write;
static constexpr mq::CreateMode create       = mq::CreateMode::create;
static constexpr mq::CreateMode exclusive    = mq::CreateMode::exclusive;
static constexpr mq::ExtraFlag close_on_exec = mq::ExtraFlag::close_on_exec;
static constexpr mq::ExtraFlag nonblock      = mq::ExtraFlag::nonblock;
} // namespace mq_constants
} // namespace ipcpp

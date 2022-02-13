#pragma once

#include <asm-generic/errno.h>
#include <fcntl.h>
#include <filesystem>
#include <mqueue.h>
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

  enum class OpenError {
    PermissionDenied,
    NameInvalid,
    NameExisting,
    AttributeInvalid,
    TooManyProcessFiles,
    NameTooLong,
    TooManySystemFiles,
    QueueMissing,
    MemoryInsufficient,
    SpaceInsufficient,
    ErrorUnknown,
  };

  struct CreateAttributes {
    long max_messages;
    long message_size;

    constexpr auto mq_attr() const noexcept -> ::mq_attr {
      auto result       = ::mq_attr{};
      result.mq_maxmsg  = max_messages;
      result.mq_msgsize = message_size;

      return result;
    }
  };

  static auto open(char const *name, OpenMode mode) noexcept -> tl::expected<mq, OpenError>;
  static auto open(char const *name,
                   OpenCreateMode mode,
                   std::filesystem::perms permission) noexcept -> tl::expected<mq, OpenError>;
  static auto open(char const *name,
                   OpenCreateMode mode,
                   std::filesystem::perms permission,
                   CreateAttributes attributes) noexcept -> tl::expected<mq, OpenError>;
  static auto unlink(char const *name) noexcept -> int;

  mq(mq const &) = delete;
  mq(mq &&other) noexcept;

  auto operator=(mq const &) -> mq & = delete;
  auto operator                      =(mq &&other) noexcept -> mq &;

  ~mq() noexcept;

private:
  static constexpr auto valid_name(char const *name) -> bool {
    if (name[0] != '/') {
      return false;
    }
    if (name[1] == '\0') {
      return false;
    }
    while (*++name != '\0') {
      if (*name == '/') {
        return false;
      }
    }

    return true;
  }
  static constexpr auto map_open_error(int error) -> OpenError {
    switch (error) {
    case EACCES:
      return OpenError::PermissionDenied;
    case EEXIST:
      return OpenError::NameExisting;
    case EINVAL:
      return OpenError::AttributeInvalid;
    case EMFILE:
      return OpenError::TooManyProcessFiles;
    case ENAMETOOLONG:
      return OpenError::NameTooLong;
    case ENFILE:
      return OpenError::TooManySystemFiles;
    case ENOENT:
      return OpenError::QueueMissing;
    case ENOMEM:
      return OpenError::MemoryInsufficient;
    case ENOSPC:
      return OpenError::SpaceInsufficient;
    default:
      return OpenError::ErrorUnknown;
    }
  }

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

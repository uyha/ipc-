#pragma once

#include "macros.hpp"

#include <asm-generic/errno.h>
#include <fcntl.h>
#include <filesystem>
#include <mqueue.h>
#include <tl/expected.hpp>
#include <type_traits>

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

  IPCPP_BITOR_OP(OpenMode, ExtraFlag, OpenMode)
  IPCPP_BITOR_OP(OpenMode, CreateMode, OpenCreateMode)

  IPCPP_BITOR_OP(ExtraFlag, ExtraFlag)

  IPCPP_BITOR_OP(CreateMode, CreateMode)
  IPCPP_BITOR_OP(CreateMode, ExtraFlag, CreateMode)

  IPCPP_BITOR_OP(OpenCreateMode, CreateMode, OpenCreateMode)
  IPCPP_BITOR_OP(OpenCreateMode, ExtraFlag, OpenCreateMode)

  enum class OpenError {
    permission_denied,
    name_invalid,
    queue_existed,
    attribute_invalid,
    too_many_process_files,
    name_too_long,
    too_many_system_files,
    queue_missing,
    memory_insufficient,
    space_insufficient,
    error_unknown,
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
                   std::filesystem::perms permissions) noexcept -> tl::expected<mq, OpenError>;
  static auto open(char const *name,
                   OpenCreateMode mode,
                   std::filesystem::perms permissions,
                   CreateAttributes attributes) noexcept -> tl::expected<mq, OpenError>;
  static auto open(char const *name, OpenCreateMode mode, int permissions) noexcept
      -> tl::expected<mq, OpenError>;
  static auto
  open(char const *name, OpenCreateMode mode, int permissions, CreateAttributes attributes) noexcept
      -> tl::expected<mq, OpenError>;

  enum class UnlinkError { permission_denied, name_too_long, queue_missing, error_unknown };
  static auto unlink(char const *name) noexcept -> tl::expected<void, UnlinkError>;

  mq(mq const &) = delete;
  auto operator=(mq const &) -> mq & = delete;

  mq(mq &&other) noexcept;
  auto operator=(mq &&other) noexcept -> mq &;

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
      return OpenError::permission_denied;
    case EEXIST:
      return OpenError::queue_existed;
    case EINVAL:
      return OpenError::attribute_invalid;
    case EMFILE:
      return OpenError::too_many_process_files;
    case ENAMETOOLONG:
      return OpenError::name_too_long;
    case ENFILE:
      return OpenError::too_many_system_files;
    case ENOENT:
      return OpenError::queue_missing;
    case ENOMEM:
      return OpenError::memory_insufficient;
    case ENOSPC:
      return OpenError::space_insufficient;
    default:
      return OpenError::error_unknown;
    }
  }
  static constexpr auto map_unlink_error(int error) -> UnlinkError {
    switch (error) {
    case EACCES:
      return UnlinkError::permission_denied;
    case ENAMETOOLONG:
      return UnlinkError::name_too_long;
    case ENOENT:
      return UnlinkError::queue_missing;
    default:
      return UnlinkError::error_unknown;
    }
  }

  static auto
  open(char const *name, OpenCreateMode mode, int permissions, ::mq_attr *attributes) noexcept
      -> tl::expected<mq, OpenError>;
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

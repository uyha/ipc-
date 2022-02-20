#pragma once

#include "chrono.hpp"
#include "macros.hpp"

#include <filesystem>
#include <mqueue.h>
#include <pthread.h>
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
    long max_message_size;

    constexpr auto mq_attr() const noexcept -> ::mq_attr {
      auto result       = ::mq_attr{};
      result.mq_maxmsg  = max_messages;
      result.mq_msgsize = max_message_size;

      return result;
    }
  };

  [[nodiscard]] static auto open(char const *name, OpenMode mode) noexcept -> tl::expected<mq, OpenError>;
  [[nodiscard]] static auto open(char const *name, OpenCreateMode mode, std::filesystem::perms permissions) noexcept
      -> tl::expected<mq, OpenError>;
  [[nodiscard]] static auto
  open(char const *name, OpenCreateMode mode, std::filesystem::perms permissions, CreateAttributes attributes) noexcept
      -> tl::expected<mq, OpenError>;
  [[nodiscard]] static auto open(char const *name, OpenCreateMode mode, int permissions) noexcept
      -> tl::expected<mq, OpenError>;
  [[nodiscard]] static auto
  open(char const *name, OpenCreateMode mode, int permissions, CreateAttributes attributes) noexcept
      -> tl::expected<mq, OpenError>;

  enum class UnlinkError { permission_denied, name_too_long, queue_missing, error_unknown };
  [[nodiscard]] static auto unlink(char const *name) noexcept -> tl::expected<void, UnlinkError>;

  [[nodiscard]] auto get_attributes() const noexcept -> ::mq_attr;
  [[nodiscard]] auto set_block() noexcept -> ::mq_attr;
  [[nodiscard]] auto set_nonblock() noexcept -> ::mq_attr;

  enum class SendError : int { queue_full, interrupted, message_too_big, error_unknown };
  [[nodiscard]] auto send(char const *buffer, std::size_t len, unsigned int priority = 0) noexcept
      -> tl::expected<void, SendError>;

  enum class ReceiveError : int { queue_empty, interrupted, buffer_too_small, error_unknown };
  [[nodiscard]] auto receive(char *buffer, std::size_t len, unsigned int *priority = nullptr) noexcept
      -> tl::expected<std::size_t, ReceiveError>;

  enum class TimedSendError : int {
    queue_full,
    interrupted,
    timeout_invalid,
    message_too_big,
    timedout,
    error_unknown
  };
  template <typename DurationRep, typename DurationPeriod>
  [[nodiscard]] auto send(char const *buffer,
                          std::size_t len,
                          std::chrono::duration<DurationRep, DurationPeriod> wait_duration,
                          unsigned int priority = 0) noexcept -> tl::expected<void, TimedSendError> {
    auto timespec = to_timespec(std::chrono::system_clock::now() + wait_duration);
    return timed_send(buffer, len, &timespec, priority);
  }

  enum class TimedReceiveError : int {
    queue_empty,
    interrupted,
    timeout_invalid,
    buffer_too_small,
    timedout,
    error_unknown
  };
  template <typename DurationRep, typename DurationPeriod>
  [[nodiscard]] auto receive(char *buffer,
                             std::size_t len,
                             std::chrono::duration<DurationRep, DurationPeriod> wait_duration,
                             unsigned int *priority = nullptr) noexcept
      -> tl::expected<std::size_t, TimedReceiveError> {
    auto timespec = to_timespec(std::chrono::system_clock::now() + wait_duration);
    return timed_receive(buffer, len, &timespec, priority);
  }

  enum class NotifyError : int { registration_existed, memory_insufficient, error_unknown };
  [[nodiscard]] auto unnotify() noexcept -> tl::expected<void, NotifyError>;
  [[nodiscard]] auto notify() noexcept -> tl::expected<void, NotifyError>;
  [[nodiscard]] auto notify(void (*callback)(::sigval), ::pthread_attr_t *new_thread_attributes = nullptr) noexcept
      -> tl::expected<void, NotifyError>;
  [[nodiscard]] auto
  notify(void (*callback)(::sigval), int value, ::pthread_attr_t *new_thread_attributes = nullptr) noexcept
      -> tl::expected<void, NotifyError>;
  [[nodiscard]] auto
  notify(void (*callback)(::sigval), void *pointer, ::pthread_attr_t *new_thread_attributes = nullptr) noexcept
      -> tl::expected<void, NotifyError>;
  [[nodiscard]] auto notify(::sigevent *event) noexcept -> tl::expected<void, NotifyError>;

  mq(mq const &) = delete;
  auto operator=(mq const &) -> mq & = delete;

  mq(mq &&other) noexcept;
  auto operator=(mq &&other) noexcept -> mq &;

  ~mq() noexcept;

private:
  [[nodiscard]] static constexpr auto valid_name(char const *name) noexcept -> bool {
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
  [[nodiscard]] static constexpr auto map_open_error(int error) noexcept -> OpenError {
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
  [[nodiscard]] static constexpr auto map_unlink_error(int error) noexcept -> UnlinkError {
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
  [[nodiscard]] static constexpr auto map_send_error(int error) noexcept -> SendError {
    switch (error) {
    case EAGAIN:
      return SendError::queue_full;
    case EINTR:
      return SendError::interrupted;
    case EMSGSIZE:
      return SendError::message_too_big;
    default:
      return SendError::error_unknown;
    }
  }
  [[nodiscard]] static constexpr auto map_receive_error(int error) noexcept -> ReceiveError {
    switch (error) {
    case EAGAIN:
      return ReceiveError::queue_empty;
    case EINTR:
      return ReceiveError::interrupted;
    case EMSGSIZE:
      return ReceiveError::buffer_too_small;
    default:
      return ReceiveError::error_unknown;
    }
  }
  [[nodiscard]] static constexpr auto map_timed_send_error(int error) noexcept -> TimedSendError {
    switch (error) {
    case EAGAIN:
      return TimedSendError::queue_full;
    case EINTR:
      return TimedSendError::interrupted;
    case EINVAL:
      return TimedSendError::timeout_invalid;
    case EMSGSIZE:
      return TimedSendError::message_too_big;
    case ETIMEDOUT:
      return TimedSendError::timedout;
    default:
      return TimedSendError::error_unknown;
    }
  }
  [[nodiscard]] static constexpr auto map_timed_receive_error(int error) noexcept -> TimedReceiveError {
    switch (error) {
    case EAGAIN:
      return TimedReceiveError::queue_empty;
    case EINTR:
      return TimedReceiveError::interrupted;
    case EINVAL:
      return TimedReceiveError::timeout_invalid;
    case EMSGSIZE:
      return TimedReceiveError::buffer_too_small;
    case ETIMEDOUT:
      return TimedReceiveError::timedout;
    default:
      return TimedReceiveError::error_unknown;
    }
  }
  [[nodiscard]] static constexpr auto map_notify_error(int error) -> NotifyError {
    switch (error) {
    case EBUSY:
      return NotifyError::registration_existed;
    case ENOMEM:
      return NotifyError::memory_insufficient;
    default:
      return NotifyError::error_unknown;
    }
  }

  [[nodiscard]] static auto open(char const *name, OpenCreateMode mode, int permissions, ::mq_attr *attributes) noexcept
      -> tl::expected<mq, OpenError>;

  [[nodiscard]] auto set_attributes(long flags) noexcept -> ::mq_attr;

  [[nodiscard]] auto
  timed_send(char const *buffer, std::size_t len, ::timespec const *timeout, unsigned int priority) noexcept
      -> tl::expected<void, TimedSendError>;
  [[nodiscard]] auto
  timed_receive(char *buffer, std::size_t len, ::timespec const *timeout, unsigned int *priority = nullptr) noexcept
      -> tl::expected<std::size_t, TimedReceiveError>;

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

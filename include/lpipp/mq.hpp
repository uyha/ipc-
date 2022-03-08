#pragma once

#include "chrono.hpp"
#include "fcntl.hpp"
#include "macros.hpp"

#include <filesystem>
#include <mqueue.h>
#include <pthread.h>
#include <tl/expected.hpp>

namespace lpipp {
class mq : public fcntl<mq> {
public:
  enum class OpenMode : int {
    read_only  = O_RDONLY,
    write_only = O_WRONLY,
    read_write = O_RDWR,
  };
  enum class CreateMode : int {
    create = O_CREAT,
  };
  enum class ExclusiveMode : int {
    exclusive = O_EXCL,
  };
  enum class OpenCreateMode : int {};
  enum class ExtraFlag : int {
    close_on_exec = O_CLOEXEC,
    nonblock      = O_NONBLOCK,
  };

  LPIPP_BITOR_OP(OpenMode, ExtraFlag, OpenMode)
  LPIPP_BITOR_OP(OpenMode, ExclusiveMode, ExclusiveMode)
  LPIPP_BITOR_OP(OpenMode, CreateMode, OpenCreateMode)

  LPIPP_BITOR_OP(ExtraFlag, ExtraFlag)
  LPIPP_BITOR_OP(ExtraFlag, ExclusiveMode, ExtraFlag)

  LPIPP_BITOR_OP(CreateMode, CreateMode)
  LPIPP_BITOR_OP(CreateMode, ExclusiveMode, CreateMode)
  LPIPP_BITOR_OP(CreateMode, ExtraFlag, CreateMode)

  LPIPP_BITOR_OP(OpenCreateMode, CreateMode, OpenCreateMode)
  LPIPP_BITOR_OP(OpenCreateMode, ExclusiveMode, OpenCreateMode)
  LPIPP_BITOR_OP(OpenCreateMode, ExtraFlag, OpenCreateMode)

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
    space_insufficient
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
  [[nodiscard]] static auto open(char const *name,
                                 OpenCreateMode mode,
                                 std::filesystem::perms permissions,
                                 CreateAttributes attributes) noexcept -> tl::expected<mq, OpenError>;
  [[nodiscard]] static auto open(char const *name, OpenCreateMode mode, int permissions) noexcept
      -> tl::expected<mq, OpenError>;
  [[nodiscard]] static auto open(char const *name,
                                 OpenCreateMode mode,
                                 int permissions,
                                 CreateAttributes attributes) noexcept -> tl::expected<mq, OpenError>;

  enum class UnlinkError { permission_denied, name_too_long, queue_missing };
  [[nodiscard]] static auto unlink(char const *name) noexcept -> tl::expected<void, UnlinkError>;

  [[nodiscard]] auto get_attributes() const noexcept -> ::mq_attr;
  [[nodiscard]] auto set_block() noexcept -> ::mq_attr;
  [[nodiscard]] auto set_nonblock() noexcept -> ::mq_attr;

  enum class SendError : int { queue_full, queue_read_only, interrupted, message_too_big };
  [[nodiscard]] auto send(char const *buffer, std::size_t len, unsigned int priority = 0) noexcept
      -> tl::expected<void, SendError>;

  enum class ReceiveError : int { queue_empty, queue_write_only, interrupted, buffer_too_small };
  [[nodiscard]] auto receive(char *buffer, std::size_t len, unsigned int *priority = nullptr) noexcept
      -> tl::expected<std::size_t, ReceiveError>;

  enum class TimedSendError : int { queue_full, interrupted, timeout_invalid, message_too_big, timedout };
  template <typename DurationRep, typename DurationPeriod>
  [[nodiscard]] auto send(char const *buffer,
                          std::size_t len,
                          std::chrono::duration<DurationRep, DurationPeriod> wait_duration,
                          unsigned int priority = 0) noexcept -> tl::expected<void, TimedSendError> {
    auto timespec = to_timespec(std::chrono::system_clock::now() + wait_duration);
    return timed_send(buffer, len, &timespec, priority);
  }

  enum class TimedReceiveError : int { queue_empty, interrupted, timeout_invalid, buffer_too_small, timedout };
  template <typename DurationRep, typename DurationPeriod>
  [[nodiscard]] auto receive(char *buffer,
                             std::size_t len,
                             std::chrono::duration<DurationRep, DurationPeriod> wait_duration,
                             unsigned int *priority = nullptr) noexcept
      -> tl::expected<std::size_t, TimedReceiveError> {
    auto timespec = to_timespec(std::chrono::system_clock::now() + wait_duration);
    return timed_receive(buffer, len, &timespec, priority);
  }

  enum class NotifyError : int { registration_existed, memory_insufficient };
  [[nodiscard]] auto unnotify() noexcept -> tl::expected<void, NotifyError>;
  [[nodiscard]] auto notify() noexcept -> tl::expected<void, NotifyError>;
  [[nodiscard]] auto notify(void (*callback)(::sigval), ::pthread_attr_t *new_thread_attributes = nullptr) noexcept
      -> tl::expected<void, NotifyError>;
  [[nodiscard]] auto notify(void (*callback)(::sigval),
                            int value,
                            ::pthread_attr_t *new_thread_attributes = nullptr) noexcept
      -> tl::expected<void, NotifyError>;
  [[nodiscard]] auto notify(void (*callback)(::sigval),
                            void *pointer,
                            ::pthread_attr_t *new_thread_attributes = nullptr) noexcept
      -> tl::expected<void, NotifyError>;
  [[nodiscard]] auto notify(::sigevent *event) noexcept -> tl::expected<void, NotifyError>;

  enum class SignalNotifyError : int {
    registration_existed,
    signal_invalid,
    memory_insufficient,
  };
  [[nodiscard]] auto notify(int signal) noexcept -> tl::expected<void, SignalNotifyError>;

  mq(mq const &)                     = delete;
  auto operator=(mq const &) -> mq & = delete;

  mq(mq &&other) noexcept;
  auto operator=(mq &&other) noexcept -> mq &;

  ~mq() noexcept;

private:
  mq(int target_fd) noexcept;

  int m_fd;

  friend fcntl<mq>;

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
    case EACCES: return OpenError::permission_denied;
    case EEXIST: return OpenError::queue_existed;
    case EINVAL: return OpenError::attribute_invalid;
    case EMFILE: return OpenError::too_many_process_files;
    case ENAMETOOLONG: return OpenError::name_too_long;
    case ENFILE: return OpenError::too_many_system_files;
    case ENOENT: return OpenError::queue_missing;
    case ENOMEM: return OpenError::memory_insufficient;
    case ENOSPC: return OpenError::space_insufficient;
    default: return static_cast<OpenError>(error);
    }
  }
  [[nodiscard]] static constexpr auto map_unlink_error(int error) noexcept -> UnlinkError {
    switch (error) {
    case EACCES: return UnlinkError::permission_denied;
    case ENAMETOOLONG: return UnlinkError::name_too_long;
    case ENOENT: return UnlinkError::queue_missing;
    default: return static_cast<UnlinkError>(error);
    }
  }
  [[nodiscard]] static constexpr auto map_send_error(int error) noexcept -> SendError {
    switch (error) {
    case EAGAIN: return SendError::queue_full;
    case EBADF: return SendError::queue_read_only;
    case EINTR: return SendError::interrupted;
    case EMSGSIZE: return SendError::message_too_big;
    default: return static_cast<SendError>(error);
    }
  }
  [[nodiscard]] static constexpr auto map_receive_error(int error) noexcept -> ReceiveError {
    switch (error) {
    case EAGAIN: return ReceiveError::queue_empty;
    case EBADF: return ReceiveError::queue_write_only;
    case EINTR: return ReceiveError::interrupted;
    case EMSGSIZE: return ReceiveError::buffer_too_small;
    default: return static_cast<ReceiveError>(error);
    }
  }
  [[nodiscard]] static constexpr auto map_timed_send_error(int error) noexcept -> TimedSendError {
    switch (error) {
    case EAGAIN: return TimedSendError::queue_full;
    case EINTR: return TimedSendError::interrupted;
    case EINVAL: return TimedSendError::timeout_invalid;
    case EMSGSIZE: return TimedSendError::message_too_big;
    case ETIMEDOUT: return TimedSendError::timedout;
    default: return static_cast<TimedSendError>(error);
    }
  }
  [[nodiscard]] static constexpr auto map_timed_receive_error(int error) noexcept -> TimedReceiveError {
    switch (error) {
    case EAGAIN: return TimedReceiveError::queue_empty;
    case EINTR: return TimedReceiveError::interrupted;
    case EINVAL: return TimedReceiveError::timeout_invalid;
    case EMSGSIZE: return TimedReceiveError::buffer_too_small;
    case ETIMEDOUT: return TimedReceiveError::timedout;
    default: return static_cast<TimedReceiveError>(error);
    }
  }
  [[nodiscard]] static constexpr auto map_notify_error(int error) -> NotifyError {
    switch (error) {
    case EBUSY: return NotifyError::registration_existed;
    case ENOMEM: return NotifyError::memory_insufficient;
    default: return static_cast<NotifyError>(error);
    }
  }
  [[nodiscard]] static constexpr auto map_signal_notify_error(int error) -> SignalNotifyError {
    switch (error) {
    case EBUSY: return SignalNotifyError::registration_existed;
    case EINVAL: return SignalNotifyError::signal_invalid;
    case ENOMEM: return SignalNotifyError::memory_insufficient;
    default: return static_cast<SignalNotifyError>(error);
    }
  }

  [[nodiscard]] static auto open(char const *name, OpenCreateMode mode, int permissions, ::mq_attr *attributes) noexcept
      -> tl::expected<mq, OpenError>;

  [[nodiscard]] auto set_attributes(long flags) noexcept -> ::mq_attr;

  [[nodiscard]] auto timed_send(char const *buffer,
                                std::size_t len,
                                ::timespec const *timeout,
                                unsigned int priority) noexcept -> tl::expected<void, TimedSendError>;
  [[nodiscard]] auto timed_receive(char *buffer,
                                   std::size_t len,
                                   ::timespec const *timeout,
                                   unsigned int *priority = nullptr) noexcept
      -> tl::expected<std::size_t, TimedReceiveError>;
};
namespace mq_constants {
constexpr mq::OpenMode read_only      = mq::OpenMode::read_only;
constexpr mq::OpenMode write_only     = mq::OpenMode::write_only;
constexpr mq::OpenMode read_write     = mq::OpenMode::read_write;
constexpr mq::CreateMode create       = mq::CreateMode::create;
constexpr mq::ExclusiveMode exclusive = mq::ExclusiveMode::exclusive;
constexpr mq::ExtraFlag close_on_exec = mq::ExtraFlag::close_on_exec;
constexpr mq::ExtraFlag nonblock      = mq::ExtraFlag::nonblock;
} // namespace mq_constants
} // namespace lpipp

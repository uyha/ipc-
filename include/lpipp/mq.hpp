#pragma once

#include "chrono.hpp"
#include "fcntl.hpp"
#include "macros.hpp"

#include <filesystem>
#include <mqueue.h>
#include <pthread.h>
#include <system_error>
#include <tl/expected.hpp>
#include <type_traits>

namespace lpipp {
class mq : public fcntl<mq> {
public:
  enum class OpenError {
    permission_denied                          = EACCES,
    queue_existed                              = EEXIST,
    attribute_invalid                          = EINVAL,
    file_descriptors_per_process_limit_reached = EMFILE,
    name_too_long                              = ENAMETOOLONG,
    file_descriptors_system_wide_limit_reached = ENFILE,
    queue_missing                              = ENOENT,
    memory_insufficient                        = ENOMEM,
    space_insufficient                         = ENOSPC,
    name_invalid,
  };
  enum class UnlinkError { permission_denied = EACCES, name_too_long = ENAMETOOLONG, queue_missing = ENOENT };
  enum class SendError : int {
    queue_full      = EAGAIN,
    queue_read_only = EBADF,
    interrupted     = EINTR,
    message_too_big = EMSGSIZE,
  };
  enum class ReceiveError : int {
    queue_empty      = EAGAIN,
    queue_write_only = EBADF,
    interrupted      = EINTR,
    buffer_too_small = EMSGSIZE,
  };
  enum class TimedSendError : int {
    queue_full      = EAGAIN,
    queue_read_only = EBADF,
    interrupted     = EINTR,
    timeout_invalid = EINVAL,
    message_too_big = EMSGSIZE,
    timedout        = ETIMEDOUT
  };
  enum class TimedReceiveError : int {
    queue_empty      = EAGAIN,
    queue_write_only = EBADF,
    interrupted      = EINTR,
    timeout_invalid  = EINVAL,
    buffer_too_small = EMSGSIZE,
    timedout         = ETIMEDOUT
  };
  enum class NotifyError : int { registration_existed = EBUSY, memory_insufficient = ENOMEM };
  enum class SignalNotifyError : int {
    registration_existed = EBUSY,
    signal_invalid       = EINVAL,
    memory_insufficient  = ENOMEM,
  };

  friend LPIPP_DECLARE_MAKE_ERROR_CODE(OpenError);
  friend LPIPP_DECLARE_MAKE_ERROR_CODE(UnlinkError);
  friend LPIPP_DECLARE_MAKE_ERROR_CODE(SendError);
  friend LPIPP_DECLARE_MAKE_ERROR_CODE(ReceiveError);
  friend LPIPP_DECLARE_MAKE_ERROR_CODE(TimedSendError);
  friend LPIPP_DECLARE_MAKE_ERROR_CODE(TimedReceiveError);
  friend LPIPP_DECLARE_MAKE_ERROR_CODE(NotifyError);
  friend LPIPP_DECLARE_MAKE_ERROR_CODE(SignalNotifyError);

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
  LPIPP_BITOR_OP(OpenMode, ExclusiveMode, OpenMode)
  LPIPP_BITOR_OP(OpenMode, CreateMode, OpenCreateMode)

  LPIPP_BITOR_OP(ExtraFlag, ExtraFlag)
  LPIPP_BITOR_OP(ExtraFlag, ExclusiveMode, ExtraFlag)

  LPIPP_BITOR_OP(CreateMode, CreateMode)
  LPIPP_BITOR_OP(CreateMode, ExclusiveMode, CreateMode)
  LPIPP_BITOR_OP(CreateMode, ExtraFlag, CreateMode)

  LPIPP_BITOR_OP(OpenCreateMode, CreateMode, OpenCreateMode)
  LPIPP_BITOR_OP(OpenCreateMode, ExclusiveMode, OpenCreateMode)
  LPIPP_BITOR_OP(OpenCreateMode, ExtraFlag, OpenCreateMode)

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

  [[nodiscard]] static auto open(char const *name, OpenMode mode) noexcept -> tl::expected<mq, std::error_code>;
  [[nodiscard]] static auto open(char const *name, OpenCreateMode mode, std::filesystem::perms permissions) noexcept
      -> tl::expected<mq, std::error_code>;
  [[nodiscard]] static auto open(char const *name,
                                 OpenCreateMode mode,
                                 std::filesystem::perms permissions,
                                 CreateAttributes attributes) noexcept -> tl::expected<mq, std::error_code>;
  [[nodiscard]] static auto open(char const *name, OpenCreateMode mode, ::mode_t permissions) noexcept
      -> tl::expected<mq, std::error_code>;
  [[nodiscard]] static auto open(char const *name,
                                 OpenCreateMode mode,
                                 ::mode_t permissions,
                                 CreateAttributes attributes) noexcept -> tl::expected<mq, std::error_code>;

  [[nodiscard]] static auto unlink(char const *name) noexcept -> tl::expected<void, std::error_code>;

  [[nodiscard]] auto get_attributes() const noexcept -> ::mq_attr;
  [[nodiscard]] auto set_block() noexcept -> ::mq_attr;
  [[nodiscard]] auto set_nonblock() noexcept -> ::mq_attr;

  [[nodiscard]] auto send(char const *buffer, std::size_t len, unsigned int priority = 0) noexcept
      -> tl::expected<void, std::error_code>;

  [[nodiscard]] auto receive(char *buffer, std::size_t len, unsigned int *priority = nullptr) noexcept
      -> tl::expected<std::size_t, std::error_code>;

  template <typename DurationRep, typename DurationPeriod>
  [[nodiscard]] auto send(char const *buffer,
                          std::size_t len,
                          std::chrono::duration<DurationRep, DurationPeriod> wait_duration,
                          unsigned int priority = 0) noexcept -> tl::expected<void, std::error_code> {
    auto timespec = to_timespec(std::chrono::system_clock::now() + wait_duration);
    return timed_send(buffer, len, &timespec, priority);
  }

  template <typename DurationRep, typename DurationPeriod>
  [[nodiscard]] auto receive(char *buffer,
                             std::size_t len,
                             std::chrono::duration<DurationRep, DurationPeriod> wait_duration,
                             unsigned int *priority = nullptr) noexcept -> tl::expected<std::size_t, std::error_code> {
    auto timespec = to_timespec(std::chrono::system_clock::now() + wait_duration);
    return timed_receive(buffer, len, &timespec, priority);
  }

  [[nodiscard]] auto unnotify() noexcept -> tl::expected<void, std::error_code>;
  [[nodiscard]] auto notify() noexcept -> tl::expected<void, std::error_code>;
  [[nodiscard]] auto notify(void (*callback)(::sigval), ::pthread_attr_t *new_thread_attributes = nullptr) noexcept
      -> tl::expected<void, std::error_code>;
  [[nodiscard]] auto notify(void (*callback)(::sigval),
                            int value,
                            ::pthread_attr_t *new_thread_attributes = nullptr) noexcept
      -> tl::expected<void, std::error_code>;
  [[nodiscard]] auto notify(void (*callback)(::sigval),
                            void *pointer,
                            ::pthread_attr_t *new_thread_attributes = nullptr) noexcept
      -> tl::expected<void, std::error_code>;
  [[nodiscard]] auto notify(::sigevent *event) noexcept -> tl::expected<void, std::error_code>;

  [[nodiscard]] auto notify(int signal) noexcept -> tl::expected<void, std::error_code>;

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

  [[nodiscard]] static auto open(char const *name,
                                 OpenCreateMode mode,
                                 ::mode_t permissions,
                                 ::mq_attr *attributes) noexcept -> tl::expected<mq, std::error_code>;

  [[nodiscard]] auto set_attributes(long flags) noexcept -> ::mq_attr;

  [[nodiscard]] auto timed_send(char const *buffer,
                                std::size_t len,
                                ::timespec const *timeout,
                                unsigned int priority) noexcept -> tl::expected<void, std::error_code>;
  [[nodiscard]] auto timed_receive(char *buffer,
                                   std::size_t len,
                                   ::timespec const *timeout,
                                   unsigned int *priority = nullptr) noexcept
      -> tl::expected<std::size_t, std::error_code>;
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

LPIPP_IS_ERROR_CODE(lpipp::mq::OpenError)
LPIPP_IS_ERROR_CODE(lpipp::mq::UnlinkError)
LPIPP_IS_ERROR_CODE(lpipp::mq::SendError)
LPIPP_IS_ERROR_CODE(lpipp::mq::ReceiveError)
LPIPP_IS_ERROR_CODE(lpipp::mq::TimedSendError)
LPIPP_IS_ERROR_CODE(lpipp::mq::TimedReceiveError)
LPIPP_IS_ERROR_CODE(lpipp::mq::NotifyError)
LPIPP_IS_ERROR_CODE(lpipp::mq::SignalNotifyError)

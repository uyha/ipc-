#include <lpipp/mq.hpp>
#include <mqueue.h>
#include <signal.h>

namespace {
struct MqOpenErrorCategory : std::error_category {
  auto name() const noexcept -> char const * {
    return "mq::OpenError";
  }
  auto message(int error) const -> std::string {
    switch (static_cast<lpipp::mq::OpenError>(error)) {
    case lpipp::mq::OpenError::permission_denied:
      return "The queue exists, but the caller does not have permission to open it in the specified mode.";
    case lpipp::mq::OpenError::queue_existed:
      return "Exclusive create fails due a queue with the same name already exists";
    case lpipp::mq::OpenError::attribute_invalid:
      return "max_messages or max_message_size is 0, or at least one of them exceeds system limit (msg_max and "
             "msgsize_max)";
    case lpipp::mq::OpenError::file_descriptors_per_process_limit_reached:
      return "Too many file descriptors and message queues are being opened by this process";
    case lpipp::mq::OpenError::name_too_long: return "Name is too long";
    case lpipp::mq::OpenError::file_descriptors_system_wide_limit_reached:
      return "Too many file descriptors and message queues are being opened in this system";
    case lpipp::mq::OpenError::queue_missing: return "No message queue found with the specified name";
    case lpipp::mq::OpenError::memory_insufficient: return "Insufficient memory";
    case lpipp::mq::OpenError::space_insufficient: return "Insufficient space for the creation of a new message queue.";
    case lpipp::mq::OpenError::name_invalid:
      return "The specified name is invalid, it must start with a slash (/), and have at least 1 character non slash "
             "character follow";
    default: return "Unknown error";
    }
  }
} const mq_open_error_category{};

struct MqUnlinkErrorCategory : std::error_category {
  auto name() const noexcept -> char const * {
    return "mq::UnlinkError";
  }
  auto message(int error) const -> std::string {
    switch (static_cast<lpipp::mq::UnlinkError>(error)) {
    case lpipp::mq::UnlinkError::permission_denied:
      return "The caller does not have permission to unlink this message queue";
    case lpipp::mq::UnlinkError::name_too_long: return "Name too long";
    case lpipp::mq::UnlinkError::queue_missing: return "No queue with the specified name exists";
    default: return "Unknown error";
    }
  }
} const mq_unlink_error_category{};

struct MqSendErrorCategory : std::error_category {
  auto name() const noexcept -> char const * {
    return "mq::SendError";
  }
  auto message(int error) const -> std::string {
    switch (static_cast<lpipp::mq::SendError>(error)) {
    case lpipp::mq::SendError::queue_full: return "The queue was full and nonblock attribute is specified";
    case lpipp::mq::SendError::queue_read_only: return "The queue is not open for sending";
    case lpipp::mq::SendError::interrupted: return "Sending to queue was interrupted by a signal handler";
    case lpipp::mq::SendError::message_too_big:
      return "msg_len was greater than the mq_msgsize attribute of the message queue";
    default: return "Unknown error";
    }
  }
} const mq_send_error_category{};

struct MqReceiveErrorCategory : std::error_category {
  auto name() const noexcept -> char const * {
    return "mq::ReceiveError";
  }
  auto message(int error) const -> std::string {
    switch (static_cast<lpipp::mq::ReceiveError>(error)) {
    case lpipp::mq::ReceiveError::queue_empty: return "The queue was empty and nonblock attribute is specified";
    case lpipp::mq::ReceiveError::queue_write_only: return "The queue is not open for receiving";
    case lpipp::mq::ReceiveError::interrupted: return "Receiving from queue was interrupted by a signal handler";
    case lpipp::mq::ReceiveError::buffer_too_small:
      return "msg_len was less than the mq_msgsize attribute of the message queue";
    default: return "Unknown error";
    }
  }
} const mq_receive_error_category{};

struct MqTimedSendErrorCategory : std::error_category {
  auto name() const noexcept -> char const * {
    return "mq::TimedSendError";
  }
  auto message(int error) const -> std::string {
    switch (static_cast<lpipp::mq::TimedSendError>(error)) {
    case lpipp::mq::TimedSendError::queue_full: return "The queue was full and nonblock attribute is specified";
    case lpipp::mq::TimedSendError::queue_read_only: return "The queue is not open for sending";
    case lpipp::mq::TimedSendError::interrupted: return "Sending to queue was interrupted by a signal handler";
    case lpipp::mq::TimedSendError::timeout_invalid: return "Invalid timeout";
    case lpipp::mq::TimedSendError::message_too_big:
      return "msg_len was greater than the mq_msgsize attribute of the message queue";
    case lpipp::mq::TimedSendError::timedout: return "Timed out before a message is sent";
    default: return "Unknown error";
    }
  }
} const mq_timed_send_error_category{};

struct MqTimedReceiveErrorCategory : std::error_category {
  auto name() const noexcept -> char const * {
    return "mq::TimedReceiveError";
  }
  auto message(int error) const -> std::string {
    switch (static_cast<lpipp::mq::TimedReceiveError>(error)) {
    case lpipp::mq::TimedReceiveError::queue_empty: return "The queue was empty and nonblock attribute is specified";
    case lpipp::mq::TimedReceiveError::queue_write_only: return "The queue is not open for receiving";
    case lpipp::mq::TimedReceiveError::interrupted: return "Receiving from queue was interrupted by a signal handler";
    case lpipp::mq::TimedReceiveError::timeout_invalid: return "Invalid timeout";
    case lpipp::mq::TimedReceiveError::buffer_too_small:
      return "msg_len was less than the mq_msgsize attribute of the message queue";
    case lpipp::mq::TimedReceiveError::timedout: return "Timed out before a message is received";
    default: return "Unknown error";
    }
  }
} const mq_timed_receive_error_category{};

struct MqNotifyErrorCategory : std::error_category {
  auto name() const noexcept -> char const * {
    return "mq::NotifyError";
  }
  auto message(int error) const -> std::string {
    switch (static_cast<lpipp::mq::NotifyError>(error)) {
    case lpipp::mq::NotifyError::registration_existed:
      return "Another process has already registered to receive notification for this message queue";
    case lpipp::mq::NotifyError::memory_insufficient: return "Insufficient memory";
    default: return "Unknown error";
    }
  }
} const mq_notify_error_category{};

struct MqSignalNotifyErrorCategory : std::error_category {
  auto name() const noexcept -> char const * {
    return "mq::SignalNotifyError";
  }
  auto message(int error) const -> std::string {
    switch (static_cast<lpipp::mq::SignalNotifyError>(error)) {
    case lpipp::mq::SignalNotifyError::registration_existed:
      return "Another process has already registered to receive notification for this message queue";
    case lpipp::mq::SignalNotifyError::signal_invalid: return "Invalid signal number";
    case lpipp::mq::SignalNotifyError::memory_insufficient: return "Insufficient memory";
    default: return "Unknown error";
    }
  }
} const mq_signal_notify_error_category{};
} // namespace

namespace lpipp {
LPIPP_DEFINE_MAKE_ERROR_CODE(mq::OpenError, mq_open_error_category)
LPIPP_DEFINE_MAKE_ERROR_CODE(mq::UnlinkError, mq_unlink_error_category)
LPIPP_DEFINE_MAKE_ERROR_CODE(mq::SendError, mq_send_error_category)
LPIPP_DEFINE_MAKE_ERROR_CODE(mq::ReceiveError, mq_receive_error_category)
LPIPP_DEFINE_MAKE_ERROR_CODE(mq::TimedSendError, mq_timed_send_error_category)
LPIPP_DEFINE_MAKE_ERROR_CODE(mq::TimedReceiveError, mq_timed_receive_error_category)
LPIPP_DEFINE_MAKE_ERROR_CODE(mq::NotifyError, mq_notify_error_category)
LPIPP_DEFINE_MAKE_ERROR_CODE(mq::SignalNotifyError, mq_signal_notify_error_category)

auto mq::open(const char *name, OpenMode mode) noexcept -> tl::expected<mq, std::error_code> {
  using tl::unexpected;

  if (not valid_name(name)) {
    return unexpected{OpenError::name_invalid};
  }

  auto fd = ::mq_open(name, static_cast<int>(mode));
  if (fd == -1) {
    return unexpected{static_cast<OpenError>(errno)};
  }

  return mq{fd};
}
auto mq::open(const char *name, OpenCreateMode mode, std::filesystem::perms permissions) noexcept
    -> tl::expected<mq, std::error_code> {
  return mq::open(name, mode, static_cast<::mode_t>(permissions), nullptr);
}
auto mq::open(const char *name,
              OpenCreateMode mode,
              std::filesystem::perms permissions,
              CreateAttributes attributes) noexcept -> tl::expected<mq, std::error_code> {
  auto mq_attributes = attributes.mq_attr();

  return mq::open(name, mode, static_cast<::mode_t>(permissions), &mq_attributes);
}
auto mq::open(const char *name, OpenCreateMode mode, ::mode_t permissions) noexcept
    -> tl::expected<mq, std::error_code> {
  return mq::open(name, mode, permissions, nullptr);
}
auto mq::open(const char *name, OpenCreateMode mode, ::mode_t permissions, CreateAttributes attributes) noexcept
    -> tl::expected<mq, std::error_code> {
  auto mq_attributes = attributes.mq_attr();

  return mq::open(name, mode, permissions, &mq_attributes);
}

auto mq::open(const char *name, OpenCreateMode mode, ::mode_t permissions, ::mq_attr *attributes) noexcept
    -> tl::expected<mq, std::error_code> {
  using tl::unexpected, tl::expected;

  if (not valid_name(name)) {
    return unexpected{OpenError::name_invalid};
  }

  auto fd = ::mq_open(name, static_cast<int>(mode), permissions, attributes);
  if (fd == -1) {
    return unexpected{static_cast<OpenError>(errno)};
  }

  return mq{fd};
}

auto mq::unlink(const char *name) noexcept -> tl::expected<void, std::error_code> {
  if (auto result = ::mq_unlink(name); result == 0) {
    return {};
  } else {
    return tl::unexpected{static_cast<UnlinkError>(errno)};
  }
}

auto mq::get_attributes() const noexcept -> ::mq_attr {
  auto result = ::mq_attr{};
  ::mq_getattr(m_fd, &result);
  return result;
}

auto mq::set_block() noexcept -> ::mq_attr {
  return set_attributes(0);
}
auto mq::set_nonblock() noexcept -> ::mq_attr {
  return set_attributes(O_NONBLOCK);
}
auto mq::set_attributes(long flags) noexcept -> ::mq_attr {
  auto old_attributes     = ::mq_attr{};
  auto new_attributes     = ::mq_attr{};
  new_attributes.mq_flags = flags;

  ::mq_setattr(m_fd, &new_attributes, &old_attributes);

  return old_attributes;
}

auto mq::send(const char *buffer, std::size_t len, unsigned int priority) noexcept
    -> tl::expected<void, std::error_code> {
  if (auto const result = ::mq_send(m_fd, buffer, len, priority); result == 0) {
    return {};
  } else {
    return tl::unexpected{static_cast<SendError>(errno)};
  }
}
auto mq::receive(char *buffer, std::size_t len, unsigned int *priority) noexcept
    -> tl::expected<std::size_t, std::error_code> {
  if (auto const result = ::mq_receive(m_fd, buffer, len, priority); result != -1) {
    return result;
  } else {
    return tl::unexpected{static_cast<ReceiveError>(errno)};
  }
}

auto mq::timed_send(char const *buffer, std::size_t len, ::timespec const *timeout, unsigned int priority) noexcept
    -> tl::expected<void, std::error_code> {
  if (auto const result = ::mq_timedsend(m_fd, buffer, len, priority, timeout); result == 0) {
    return {};
  } else {
    return tl::unexpected{static_cast<TimedSendError>(errno)};
  }
}
auto mq::timed_receive(char *buffer, std::size_t len, ::timespec const *timeout, unsigned int *priority) noexcept
    -> tl::expected<std::size_t, std::error_code> {
  if (auto const result = ::mq_timedreceive(m_fd, buffer, len, priority, timeout); result != -1) {
    return {};
  } else {
    return tl::unexpected{static_cast<TimedReceiveError>(errno)};
  }
}

auto mq::unnotify() noexcept -> tl::expected<void, std::error_code> {
  if (auto result = ::mq_notify(m_fd, nullptr); result == 0) {
    return {};
  }
  return tl::unexpected{static_cast<NotifyError>(errno)};
}
auto mq::notify() noexcept -> tl::expected<void, std::error_code> {
  auto event         = ::sigevent{};
  event.sigev_notify = SIGEV_NONE;
  if (auto result = ::mq_notify(m_fd, &event); result == 0) {
    return {};
  }
  return tl::unexpected{static_cast<NotifyError>(errno)};
}
auto mq::notify(void (*callback)(::sigval), ::pthread_attr_t *new_thread_attributes) noexcept
    -> tl::expected<void, std::error_code> {
  auto event                    = ::sigevent{};
  event.sigev_notify            = SIGEV_THREAD;
  event.sigev_notify_function   = callback;
  event.sigev_notify_attributes = new_thread_attributes;

  return notify(&event);
}
auto mq::notify(void (*callback)(::sigval), int value, ::pthread_attr_t *new_thread_attributes) noexcept
    -> tl::expected<void, std::error_code> {
  auto event                    = ::sigevent{};
  event.sigev_notify            = SIGEV_THREAD;
  event.sigev_notify_function   = callback;
  event.sigev_notify_attributes = new_thread_attributes;
  event.sigev_value.sival_int   = value;

  return notify(&event);
}
auto mq::notify(void (*callback)(::sigval), void *pointer, ::pthread_attr_t *new_thread_attributes) noexcept
    -> tl::expected<void, std::error_code> {
  auto event                    = ::sigevent{};
  event.sigev_notify            = SIGEV_THREAD;
  event.sigev_notify_function   = callback;
  event.sigev_notify_attributes = new_thread_attributes;
  event.sigev_value.sival_ptr   = pointer;

  return notify(&event);
}
auto mq::notify(::sigevent *event) noexcept -> tl::expected<void, std::error_code> {
  if (auto result = ::mq_notify(m_fd, event); result == 0) {
    return {};
  }
  return tl::unexpected{static_cast<NotifyError>(errno)};
}

auto mq::notify(int signal) noexcept -> tl::expected<void, std::error_code> {
  auto event         = ::sigevent{};
  event.sigev_notify = SIGEV_SIGNAL;
  event.sigev_signo  = signal;
  if (auto result = ::mq_notify(m_fd, &event); result == 0) {
    return {};
  }
  return tl::unexpected{static_cast<SignalNotifyError>(errno)};
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

mq::mq(int target_fd) noexcept
    : m_fd{target_fd} {}
} // namespace lpipp

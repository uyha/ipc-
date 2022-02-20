#include <errno.h>
#include <lpipp/mq.hpp>
#include <mqueue.h>
#include <signal.h>

namespace lpipp {
auto mq::open(const char *name, OpenMode mode) noexcept -> tl::expected<mq, OpenError> {
  using tl::unexpected, tl::expected;

  if (not valid_name(name)) {
    return unexpected{OpenError::name_invalid};
  }

  auto fd = ::mq_open(name, static_cast<int>(mode));
  if (fd == -1) {
    return unexpected{map_open_error(errno)};
  }

  return mq{fd};
}
auto mq::open(const char *name, OpenCreateMode mode, std::filesystem::perms permissions) noexcept
    -> tl::expected<mq, OpenError> {
  return mq::open(name, mode, static_cast<int>(permissions), nullptr);
}
auto mq::open(const char *name,
              OpenCreateMode mode,
              std::filesystem::perms permissions,
              CreateAttributes attributes) noexcept -> tl::expected<mq, OpenError> {
  auto mq_attributes = attributes.mq_attr();

  return mq::open(name, mode, static_cast<int>(permissions), &mq_attributes);
}
auto mq::open(const char *name, OpenCreateMode mode, int permissions) noexcept -> tl::expected<mq, OpenError> {
  return mq::open(name, mode, permissions, nullptr);
}
auto mq::open(const char *name, OpenCreateMode mode, int permissions, CreateAttributes attributes) noexcept
    -> tl::expected<mq, OpenError> {
  auto mq_attributes = attributes.mq_attr();

  return mq::open(name, mode, permissions, &mq_attributes);
}

auto mq::open(const char *name, OpenCreateMode mode, int permissions, ::mq_attr *attributes) noexcept
    -> tl::expected<mq, OpenError> {
  using tl::unexpected, tl::expected;

  if (not valid_name(name)) {
    return unexpected{OpenError::name_invalid};
  }

  auto fd = ::mq_open(name, static_cast<int>(mode), permissions, attributes);
  if (fd == -1) {
    return unexpected{map_open_error(errno)};
  }

  return mq{fd};
}

auto mq::unlink(const char *name) noexcept -> tl::expected<void, UnlinkError> {
  if (auto result = ::mq_unlink(name); result == 0) {
    return {};
  } else {
    return tl::unexpected{map_unlink_error(errno)};
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

auto mq::send(const char *buffer, std::size_t len, unsigned int priority) noexcept -> tl::expected<void, SendError> {
  if (auto const result = ::mq_send(m_fd, buffer, len, priority); result == 0) {
    return {};
  } else {
    return tl::unexpected{map_send_error(errno)};
  }
}
auto mq::receive(char *buffer, std::size_t len, unsigned int *priority) noexcept
    -> tl::expected<std::size_t, ReceiveError> {
  if (auto const result = ::mq_receive(m_fd, buffer, len, priority); result != -1) {
    return result;
  } else {
    return tl::unexpected{map_receive_error(errno)};
  }
}

auto mq::timed_send(char const *buffer, std::size_t len, ::timespec const *timeout, unsigned int priority) noexcept
    -> tl::expected<void, TimedSendError> {
  if (auto const result = ::mq_timedsend(m_fd, buffer, len, priority, timeout); result == 0) {
    return {};
  } else {
    return tl::unexpected{map_timed_send_error(errno)};
  }
}
auto mq::timed_receive(char *buffer, std::size_t len, ::timespec const *timeout, unsigned int *priority) noexcept
    -> tl::expected<std::size_t, TimedReceiveError> {
  if (auto const result = ::mq_timedreceive(m_fd, buffer, len, priority, timeout); result != -1) {
    return {};
  } else {
    return tl::unexpected{map_timed_receive_error(errno)};
  }
}

auto mq::unnotify() noexcept -> tl::expected<void, NotifyError> {
  if (auto result = ::mq_notify(m_fd, nullptr); result == 0) {
    return {};
  }
  return tl::unexpected{map_notify_error(errno)};
}
auto mq::notify() noexcept -> tl::expected<void, NotifyError> {
  auto event         = ::sigevent{};
  event.sigev_notify = SIGEV_NONE;
  if (auto result = ::mq_notify(m_fd, &event); result == 0) {
    return {};
  }
  return tl::unexpected{map_notify_error(errno)};
}
auto mq::notify(void (*callback)(::sigval), ::pthread_attr_t *new_thread_attributes) noexcept
    -> tl::expected<void, NotifyError> {
  auto event                    = ::sigevent{};
  event.sigev_notify            = SIGEV_THREAD;
  event.sigev_notify_function   = callback;
  event.sigev_notify_attributes = new_thread_attributes;

  return notify(&event);
}
auto mq::notify(void (*callback)(::sigval), int value, ::pthread_attr_t *new_thread_attributes) noexcept
    -> tl::expected<void, NotifyError> {
  auto event                    = ::sigevent{};
  event.sigev_notify            = SIGEV_THREAD;
  event.sigev_notify_function   = callback;
  event.sigev_notify_attributes = new_thread_attributes;
  event.sigev_value.sival_int   = value;

  return notify(&event);
}
auto mq::notify(void (*callback)(::sigval), void *pointer, ::pthread_attr_t *new_thread_attributes) noexcept
    -> tl::expected<void, NotifyError> {
  auto event                    = ::sigevent{};
  event.sigev_notify            = SIGEV_THREAD;
  event.sigev_notify_function   = callback;
  event.sigev_notify_attributes = new_thread_attributes;
  event.sigev_value.sival_ptr   = pointer;

  return notify(&event);
}
auto mq::notify(::sigevent *event) noexcept -> tl::expected<void, NotifyError> {
  if (auto result = ::mq_notify(m_fd, event); result == 0) {
    return {};
  }
  return tl::unexpected{map_notify_error(errno)};
}

auto mq::notify(int signal) noexcept -> tl::expected<void, SignalNotifyError> {
  auto event         = ::sigevent{};
  event.sigev_notify = SIGEV_SIGNAL;
  event.sigev_signo  = signal;
  if (auto result = ::mq_notify(m_fd, &event); result == 0) {
    return {};
  }
  return tl::unexpected{map_signal_notify_error(errno)};
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
} // namespace lpipp

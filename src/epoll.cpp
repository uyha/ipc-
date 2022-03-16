#include "lpipp/macros.hpp"

#include <cerrno>
#include <fcntl.h>
#include <lpipp/epoll.hpp>
#include <sys/epoll.h>
#include <unistd.h>

namespace {
struct EpollCreateErrorCategory : std::error_category {
  auto name() const noexcept -> char const * {
    return "epoll::CreateError";
  }
  auto message(int error) const -> std::string {
    switch (static_cast<lpipp::epoll::CreateError>(error)) {
    case lpipp::epoll::CreateError::file_descriptors_per_process_limit_reached:
      return "Too many file descriptors are being opened by this process";
    case lpipp::epoll::CreateError::file_descriptors_system_limit_reached:
      return "Too many file descriptors are being opened in this system";
    case lpipp::epoll::CreateError::memory_insufficient: return "Insufficient memory";
    default: return "Unknown error";
    }
  }
} const epoll_create_error_category;

struct EpollAddErrorCategory : std::error_category {
  auto name() const noexcept -> char const * {
    return "epoll:AddError";
  }
  auto message(int error) const -> std::string {
    switch (static_cast<lpipp::epoll::AddError>(error)) {
    case lpipp::epoll::AddError::file_descriptor_invalid: return "Adding an invalid file descriptor";
    case lpipp::epoll::AddError::file_descriptor_existed: return "The specified file descriptor is already added";
    case lpipp::epoll::AddError::exclusive_on_epoll_error: return "Adding an epoll exclusively is not allowed";
    case lpipp::epoll::AddError::file_descriptor_not_supported:
      return "The specified file descriptor does not support epoll";
    case lpipp::epoll::AddError::loop_detected:
      return "A loop will be created if the specified file descriptor is added";
    case lpipp::epoll::AddError::memory_insufficient: return "Insufficient memory";
    case lpipp::epoll::AddError::max_watches_reached:
      return "The limit imposed by /proc/sys/fs/epoll/max_user_watches was encountered";
    case lpipp::epoll::AddError::file_descriptor_same_with_epoll:
      return "The specified file descriptor is the same with the epoll instance";
    default: return "Unknown error";
    }
  }
} const epoll_add_error_category{};

struct EpollRemoveErrorCategory : std::error_category {
  auto name() const noexcept -> char const * {
    return "epoll::RemoveError";
  }
  auto message(int error) const -> std::string {
    switch (static_cast<lpipp::epoll::RemoveError>(error)) {
    case lpipp::epoll::RemoveError::file_descriptor_not_registered:
      return "The specified file descriptor is not registered";
    default: return "Unknown error";
    }
  }
} const epoll_remove_error_category{};

struct EpollModifyErrorCategory : std::error_category {
  auto name() const noexcept -> char const * {
    return "epoll::ModifyError";
  }
  auto message(int error) const -> std::string {
    switch (static_cast<lpipp::epoll::ModifyError>(error)) {
    case lpipp::epoll::ModifyError::file_descriptor_invalid: return "Modifying an invalid file descriptor";
    case lpipp::epoll::ModifyError::exclusive_previously_specified:
      return "Modifying a file descriptor which was registered with exclusive specified is not allowed";
    case lpipp::epoll::ModifyError::file_descriptor_not_registered:
      return "The specified file descriptor is not registered";
    case lpipp::epoll::ModifyError::memory_insufficient: return "Insufficient memory";
    case lpipp::epoll::ModifyError::file_descriptor_same_with_epoll:
      return "The specified file descriptor is the same with the epoll instance";
    default: return "Unknown error";
    }
  }
} const epoll_modify_error_category{};

struct EpollWaitErrorCategory : std::error_category {
  auto name() const noexcept -> char const * {
    return "epoll::WaitError";
  }
  auto message(int error) const -> std::string {
    switch (static_cast<lpipp::epoll::WaitError>(error)) {
    case lpipp::epoll::WaitError::events_buffer_not_accessible: return "Events buffer is not accessible";
    case lpipp::epoll::WaitError::interrupted: return "epoll wait is interrupted by a signal handler";
    case lpipp::epoll::WaitError::max_events_invalid: return "max_events cannot be less than or equal to 0";
    case lpipp::epoll::WaitError::timeout:
    default: return "Unknown error";
    }
  }
} const epoll_wait_error_category{};

} // namespace

namespace lpipp {
LPIPP_DEFINE_MAKE_ERROR_CODE(epoll::CreateError, epoll_create_error_category)
LPIPP_DEFINE_MAKE_ERROR_CODE(epoll::AddError, epoll_add_error_category)
LPIPP_DEFINE_MAKE_ERROR_CODE(epoll::RemoveError, epoll_remove_error_category)
LPIPP_DEFINE_MAKE_ERROR_CODE(epoll::ModifyError, epoll_modify_error_category)
LPIPP_DEFINE_MAKE_ERROR_CODE(epoll::WaitError, epoll_wait_error_category)

auto epoll::create(bool close_on_exec) noexcept -> tl::expected<epoll, std::error_code> {
  auto fd = ::epoll_create1(close_on_exec ? EPOLL_CLOEXEC : 0);
  if (fd == -1) {
    return tl::unexpected{static_cast<CreateError>(errno)};
  }

  return epoll{fd};
}

auto epoll::remove(int fd) const noexcept -> tl::expected<void, std::error_code> {
  auto result = ::epoll_ctl(m_fd, EPOLL_CTL_DEL, fd, nullptr);
  if (result == -1) {
    return tl::unexpected{RemoveError::file_descriptor_not_registered};
  }
  return {};
}

auto epoll::wait(::epoll_event *event_list,
                 int max_events,
                 int timeout_millisecs,
                 ::sigset_t const *sigmask) const noexcept -> tl::expected<int, std::error_code> {
  auto result = ::epoll_pwait(m_fd, event_list, max_events, timeout_millisecs, sigmask);
  if (result == -1) {
    return tl::unexpected{static_cast<WaitError>(errno)};
  }
  if (result == 0) {
    return tl::unexpected{WaitError::timeout};
  }
  return result;
}

epoll::epoll(epoll &&other) noexcept
    : m_fd{other.m_fd} {
  other.m_fd = -1;
}
auto epoll::operator=(epoll &&other) noexcept -> epoll & {
  m_fd       = other.m_fd;
  other.m_fd = -1;

  return *this;
}

epoll::~epoll() noexcept {
  if (m_fd == -1) {
    return;
  }
  ::close(m_fd);
}

epoll::epoll(int fd) noexcept
    : m_fd{fd} {}

} // namespace lpipp

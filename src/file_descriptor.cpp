#include <lpipp/file_descriptor.hpp>

namespace {
struct FcntlDuplicateErrorCategory : std::error_category {
  auto name() const noexcept -> char const * {
    return "fcntl::DupError";
  }
  auto message(int error) const -> std::string {
    switch (static_cast<lpipp::detail::FileDescriptor::DupError>(error)) {
    case lpipp::detail::FileDescriptor::DupError::file_descriptors_per_process_limit_reached:
      return "Too many file descriptors are being opened by this process";
    default: return "Unknown error";
    }
  }
} const fcntl_duplicate_error_category{};

struct FcntlDuplicateAtLeastErrorCategory : std::error_category {
  auto name() const noexcept -> char const * {
    return "fcntl::DupAtLeastError";
  }
  auto message(int error) const -> std::string {
    switch (static_cast<lpipp::detail::FileDescriptor::DupAtLeastError>(error)) {
    case lpipp::detail::FileDescriptor::DupAtLeastError::invalid_minimum_file_descriptor_number:
      return "Minimum file descriptor number is negative or is greater than the maximum allowable value";
    case lpipp::detail::FileDescriptor::DupAtLeastError::file_descriptors_per_process_limit_reached:
      return "Too many file descriptors are being opened by this process";
    default: return "Unknown error";
    }
  }
} const fcntl_duplicate_at_least_error_category{};

struct StatErrorCategory : std::error_category {
  auto name() const noexcept -> char const * {
    return "fcntl::StatError";
  }
  auto message(int error) const -> std::string {
    switch (static_cast<lpipp::detail::FileDescriptor::StatError>(error)) {
    case lpipp::detail::FileDescriptor::StatError::memory_insufficient: return "Insufficient memory";
    default: return "Unknown error";
    }
  }
} const fcntl_stat_error_category{};

} // namespace

namespace lpipp::detail {
LPIPP_DEFINE_MAKE_ERROR_CODE(FileDescriptor::DupError, fcntl_duplicate_error_category)
LPIPP_DEFINE_MAKE_ERROR_CODE(FileDescriptor::DupAtLeastError, fcntl_duplicate_at_least_error_category)
LPIPP_DEFINE_MAKE_ERROR_CODE(FileDescriptor::StatError, fcntl_stat_error_category)

auto FileDescriptor::duplicate(int fd) noexcept -> tl::expected<int, std::error_code> {
  auto new_fd = ::dup(fd);
  if (new_fd == -1) {
    return tl::unexpected{static_cast<DupError>(errno)};
  }
  return new_fd;
}

auto FileDescriptor::duplicate_at_least(int fd, int minimum_fd) noexcept -> tl::expected<int, std::error_code> {
  auto new_fd = ::fcntl(fd, F_DUPFD, minimum_fd);
  if (new_fd == -1) {
    return tl::unexpected{static_cast<DupAtLeastError>(errno)};
  }
  return new_fd;
}

auto FileDescriptor::stat(int fd) noexcept -> tl::expected<struct ::stat, std::error_code> {
  struct ::stat stat_buffer {};
  auto new_fd = ::fstat(fd, &stat_buffer);
  if (new_fd == -1) {
    return tl::unexpected{static_cast<StatError>(errno)};
  }
  return stat_buffer;
}
} // namespace lpipp::detail

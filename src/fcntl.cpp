#include <lpipp/fcntl.hpp>

namespace {
struct FcntlDuplicateErrorCategory : std::error_category {
  auto name() const noexcept -> char const * {
    return "mq::DupError";
  }
  auto message(int error) const -> std::string {
    switch (static_cast<lpipp::detail::fcntl::DupError>(error)) {
    case lpipp::detail::fcntl::DupError::file_descriptors_per_process_limit_reached:
      return "Too many file descriptors are being opened by this process";
    default: return "Unknown error";
    }
  }
} const fcntl_duplicate_error_category{};

struct FcntlDuplicateAtLeastErrorCategory : std::error_category {
  auto name() const noexcept -> char const * {
    return "mq::DupAtLeastError";
  }
  auto message(int error) const -> std::string {
    switch (static_cast<lpipp::detail::fcntl::DupAtLeastError>(error)) {
    case lpipp::detail::fcntl::DupAtLeastError::invalid_minimum_file_descriptor_number:
      return "Minimum file descriptor number is negative or is greater than the maximum allowable value";
    case lpipp::detail::fcntl::DupAtLeastError::file_descriptors_per_process_limit_reached:
      return "Too many file descriptors are being opened by this process";
    default: return "Unknown error";
    }
  }
} const fcntl_duplicate_at_least_error_category{};
} // namespace

namespace lpipp::detail {
LPIPP_DEFINE_MAKE_ERROR_CODE(fcntl::DupError, fcntl_duplicate_error_category)
LPIPP_DEFINE_MAKE_ERROR_CODE(fcntl::DupAtLeastError, fcntl_duplicate_at_least_error_category)

auto fcntl::duplicate(int fd) noexcept -> tl::expected<int, std::error_code> {
  auto new_fd = ::dup(fd);
  if (new_fd == -1) {
    return tl::unexpected{static_cast<DupError>(errno)};
  }
  return new_fd;
}

auto fcntl::duplicate_at_least(int fd, int minimum_fd) noexcept -> tl::expected<int, DupAtLeastError> {
  auto new_fd = ::fcntl(fd, F_DUPFD, minimum_fd);
  if (new_fd == -1) {
    return tl::unexpected{static_cast<DupAtLeastError>(errno)};
  }
  return new_fd;
}
} // namespace lpipp::detail

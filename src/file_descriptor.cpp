#include <lpipp/file_descriptor.hpp>

namespace {
struct FcntlDuplicateErrorCategory : std::error_category {
  auto name() const noexcept -> char const * {
    return "DupError";
  }
  auto message(int error) const -> std::string {
    switch (static_cast<lpipp::DupError>(error)) {
    case lpipp::DupError::file_descriptors_per_process_limit_reached:
      return "Too many file descriptors are being opened by this process";
    default: return "Unknown error";
    }
  }
} const fcntl_duplicate_error_category{};

struct FcntlDuplicateAtLeastErrorCategory : std::error_category {
  auto name() const noexcept -> char const * {
    return "DupAtLeastError";
  }
  auto message(int error) const -> std::string {
    switch (static_cast<lpipp::DupAtLeastError>(error)) {
    case lpipp::DupAtLeastError::invalid_minimum_file_descriptor_number:
      return "Minimum file descriptor number is negative or is greater than the maximum allowable value";
    case lpipp::DupAtLeastError::file_descriptors_per_process_limit_reached:
      return "Too many file descriptors are being opened by this process";
    default: return "Unknown error";
    }
  }
} const fcntl_duplicate_at_least_error_category{};

struct StatErrorCategory : std::error_category {
  auto name() const noexcept -> char const * {
    return "StatError";
  }
  auto message(int error) const -> std::string {
    switch (static_cast<lpipp::StatError>(error)) {
    case lpipp::StatError::memory_insufficient: return "Insufficient memory";
    default: return "Unknown error";
    }
  }
} const fcntl_stat_error_category{};

struct ChownErrorCategory : std::error_category {
  auto name() const noexcept -> char const * {
    return "ChownError";
  }
  auto message(int error) const -> std::string {
    switch (static_cast<lpipp::ChownError>(error)) {
    case lpipp::ChownError::memory_insufficient: return "Insufficient memory";
    case lpipp::ChownError::permission_denied:
      return "Caller does not have permission to change file descriptor to the specified user and/or group";
    default: return "Unknown error";
    }
  }
} const chown_error_category{};

struct ChmodErrorCategory : std::error_category {
  auto name() const noexcept -> char const * {
    return "ChownError";
  }
  auto message(int error) const -> std::string {
    switch (static_cast<lpipp::ChmodError>(error)) {
    case lpipp::ChmodError::memory_insufficient: return "Insufficient memory";
    case lpipp::ChmodError::permission_denied:
      return "Caller does not have permission to change file descriptor to the specified mode";
    default: return "Unknown error";
    }
  }
} const chmod_error_category{};

} // namespace

namespace lpipp {
LPIPP_DEFINE_MAKE_ERROR_CODE(DupError, fcntl_duplicate_error_category)
LPIPP_DEFINE_MAKE_ERROR_CODE(DupAtLeastError, fcntl_duplicate_at_least_error_category)
LPIPP_DEFINE_MAKE_ERROR_CODE(StatError, fcntl_stat_error_category)
LPIPP_DEFINE_MAKE_ERROR_CODE(ChownError, chown_error_category)
LPIPP_DEFINE_MAKE_ERROR_CODE(ChmodError, chmod_error_category)
} // namespace lpipp

#pragma once

#include "macros.hpp"

#include <asm-generic/errno-base.h>
#include <cerrno>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <system_error>
#include <tl/expected.hpp>
#include <type_traits>
#include <unistd.h>

namespace lpipp {
enum class DupError : int { file_descriptors_per_process_limit_reached = EMFILE };
enum class DupAtLeastError : int {
  minimum_file_descriptor_number_invalid     = EINVAL,
  file_descriptors_per_process_limit_reached = EMFILE
};
enum class StatError : int { memory_insufficient = ENOMEM };
enum class ChownError : int { memory_insufficient = ENOMEM, permission_denied = EPERM };
enum class ChmodError : int { memory_insufficient = ENOMEM, permission_denied = EPERM };
} // namespace lpipp

LPIPP_IS_ERROR_CODE(lpipp::DupError)
LPIPP_IS_ERROR_CODE(lpipp::DupAtLeastError)
LPIPP_IS_ERROR_CODE(lpipp::StatError)
LPIPP_IS_ERROR_CODE(lpipp::ChownError)
LPIPP_IS_ERROR_CODE(lpipp::ChmodError)

namespace lpipp {

LPIPP_DECLARE_MAKE_ERROR_CODE(DupError);
LPIPP_DECLARE_MAKE_ERROR_CODE(DupAtLeastError);
LPIPP_DECLARE_MAKE_ERROR_CODE(StatError);
LPIPP_DECLARE_MAKE_ERROR_CODE(ChownError);
LPIPP_DECLARE_MAKE_ERROR_CODE(ChmodError);

template <typename T>
class FileDescriptor {
public:
  auto get_handle() const noexcept -> int {
    return static_cast<T const &>(*this).m_fd;
  }
  [[nodiscard]] auto duplicate() const noexcept -> tl::expected<T, std::error_code> {
    auto new_fd = ::dup(get_handle());
    if (new_fd == -1) {
      return tl::unexpected{static_cast<DupError>(errno)};
    }
    return T{new_fd};
  }
  [[nodiscard]] auto duplicate_at_least(int minimum_fd) const noexcept -> tl::expected<T, std::error_code> {
    auto new_fd = ::fcntl(get_handle(), F_DUPFD, minimum_fd);
    if (new_fd == -1) {
      return tl::unexpected{static_cast<DupAtLeastError>(errno)};
    }
    return T{new_fd};
  }

  [[nodiscard]] auto stat() const noexcept -> tl::expected<struct ::stat, std::error_code> {
    struct ::stat stat_buffer {};
    auto new_fd = ::fstat(get_handle(), &stat_buffer);
    if (new_fd == -1) {
      return tl::unexpected{static_cast<StatError>(errno)};
    }
    return stat_buffer;
  }

  [[nodiscard]] auto chown(::uid_t user, ::gid_t group) const noexcept -> tl::expected<void, std::error_code> {
    auto result = ::fchown(get_handle(), user, group);
    if (result == -1) {
      return tl::unexpected{static_cast<ChownError>(errno)};
    }
    return {};
  }

  [[nodiscard]] auto chmod(::mode_t mode) const noexcept -> tl::expected<void, std::error_code> {
    auto result = ::fchmod(get_handle(), mode);
    if (result == -1) {
      return tl::unexpected{static_cast<ChmodError>(errno)};
    }
    return {};
  }
};
} // namespace lpipp

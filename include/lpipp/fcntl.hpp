#pragma once

#include "macros.hpp"

#include <asm-generic/errno-base.h>
#include <cerrno>
#include <fcntl.h>
#include <sys/stat.h>
#include <system_error>
#include <tl/expected.hpp>
#include <type_traits>
#include <unistd.h>

namespace lpipp {
namespace detail {
struct fcntl {
  enum class DupError : int { file_descriptors_per_process_limit_reached = EMFILE };
  enum class DupAtLeastError : int {
    invalid_minimum_file_descriptor_number     = EINVAL,
    file_descriptors_per_process_limit_reached = EMFILE
  };
  enum class StatError : int { memory_insufficient = ENOMEM };

  friend LPIPP_DECLARE_MAKE_ERROR_CODE(DupError);
  friend LPIPP_DECLARE_MAKE_ERROR_CODE(DupAtLeastError);
  friend LPIPP_DECLARE_MAKE_ERROR_CODE(StatError);

  [[nodiscard]] static auto duplicate(int fd) noexcept -> tl::expected<int, std::error_code>;
  [[nodiscard]] static auto duplicate_at_least(int fd, int minimum_fd) noexcept -> tl::expected<int, DupAtLeastError>;
  [[nodiscard]] static auto stat(int fd) noexcept -> tl::expected<struct ::stat, std::error_code>;
};
} // namespace detail

template <typename T>
class fcntl {
public:
  auto get_handle() const noexcept -> int {
    return static_cast<T const &>(*this).m_fd;
  }
  [[nodiscard]] auto duplicate() const noexcept -> tl::expected<T, std::error_code> {
    auto result = detail::fcntl::duplicate(get_handle());
    if (not result) {
      return tl::unexpected{result.error()};
    }

    return T{*result};
  }
  [[nodiscard]] auto duplicate_at_least(int minimum_fd) const noexcept -> tl::expected<T, std::error_code> {
    auto result = detail::fcntl::duplicate_at_least(get_handle(), minimum_fd);
    if (not result) {
      return tl::unexpected{result.error()};
    }
    return T{*result};
  }

  [[nodiscard]] auto stat() const noexcept -> tl::expected<struct ::stat, std::error_code> {
    return detail::fcntl::stat(get_handle());
  }
};
} // namespace lpipp

LPIPP_IS_ERROR_CODE(lpipp::detail::fcntl::DupError)
LPIPP_IS_ERROR_CODE(lpipp::detail::fcntl::DupAtLeastError)
LPIPP_IS_ERROR_CODE(lpipp::detail::fcntl::StatError)

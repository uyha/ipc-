#pragma once

#include "macros.hpp"

#include <asm-generic/errno-base.h>
#include <cerrno>
#include <fcntl.h>
#include <system_error>
#include <tl/expected.hpp>
#include <type_traits>
#include <unistd.h>

namespace lpipp {
namespace detail {
struct fcntl {
  enum class DupError : int { file_descriptors_per_process_limit_reached = EMFILE };
  friend auto make_error_code(DupError error) noexcept -> std::error_code;

  enum class DupAtLeastError : int {
    invalid_minimum_file_descriptor_number     = EINVAL,
    file_descriptors_per_process_limit_reached = EMFILE
  };
  friend auto make_error_code(DupAtLeastError error) noexcept -> std::error_code;

  [[nodiscard]] static auto duplicate(int fd) noexcept -> tl::expected<int, std::error_code>;
  [[nodiscard]] static auto duplicate_at_least(int fd, int minimum_fd) noexcept -> tl::expected<int, DupAtLeastError>;
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
};
} // namespace lpipp

LPIPP_IS_ERROR_CODE(lpipp::detail::fcntl::DupError)
LPIPP_IS_ERROR_CODE(lpipp::detail::fcntl::DupAtLeastError)

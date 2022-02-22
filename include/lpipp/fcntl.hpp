#pragma once

#include "macros.hpp"

#include <asm-generic/errno-base.h>
#include <cerrno>
#include <fcntl.h>
#include <tl/expected.hpp>
#include <unistd.h>

namespace lpipp {
template <typename T>
class fcntl {
public:
  auto get_handle() const noexcept -> int {
    return static_cast<T const &>(*this).m_fd;
  }

  enum class DupError : int { fd_limit_reached };
  [[nodiscard]] auto duplicate() const noexcept -> tl::expected<T, DupError> {
    auto new_fd = ::dup(get_handle());
    if (new_fd == -1) {
      return tl::unexpected{map_dup_error(errno)};
    }
    return T{new_fd};
  }

  enum class DupAtLeastError : int { fd_invalid, fd_limit_reached };
  [[nodiscard]] auto duplicate_at_least(int minimum_fd) const noexcept -> tl::expected<T, DupAtLeastError> {
    auto new_fd = ::fcntl(get_handle(), F_DUPFD, minimum_fd);
    if (new_fd == -1) {
      return tl::unexpected{map_dup_at_least_error(errno)};
    }
    return T{new_fd};
  }

private:
  [[nodiscard]] static constexpr auto map_dup_error(int error) noexcept -> DupError {
    switch (error) {
    case EMFILE: return DupError::fd_limit_reached;
    default: return static_cast<DupError>(error);
    }
  }
  [[nodiscard]] static constexpr auto map_dup_at_least_error(int error) noexcept -> DupAtLeastError {
    switch (error) {
    case EINVAL: return DupAtLeastError::fd_invalid;
    case EMFILE: return DupAtLeastError::fd_limit_reached;
    default: return static_cast<DupAtLeastError>(error);
    }
  }
};
} // namespace lpipp

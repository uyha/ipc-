#include "lpipp/macros.hpp"

#include <lpipp/socket.hpp>

namespace {
struct GetSocketOptionErrorCategory : std::error_category {
  auto name() const noexcept -> char const * {
    return "socket::GetSocketOptionError";
  }
  auto message(int error) const -> std::string {
    switch (static_cast<lpipp::GetSocketOptionError>(error)) {
    case lpipp::GetSocketOptionError::option_unknown:
      return "The specified option does not exist in the specified level";
    case lpipp::GetSocketOptionError::option_not_supported:
      return "The current socket does not support the specified option";
    default: return "Unknown error";
    }
  }
} const get_socket_option_error_category{};

struct SetSocketOptionErrorCategory : std::error_category {
  auto name() const noexcept -> char const * {
    return "socket::SetSocketOptionError";
  }
  auto message(int error) const -> std::string {
    switch (static_cast<lpipp::SetSocketOptionError>(error)) {
    case lpipp::SetSocketOptionError::value_invalid: return "The specified option value is invalid";
    case lpipp::SetSocketOptionError::option_unknown:
      return "The specified option does not exist in the specified level";
    case lpipp::SetSocketOptionError::option_not_supported:
      return "The current socket does not support the specified option";
    default: return "Unknown error";
    }
  }
} const set_socket_option_error_category{};
} // namespace

namespace lpipp {
LPIPP_DEFINE_MAKE_ERROR_CODE(GetSocketOptionError, get_socket_option_error_category)
LPIPP_DEFINE_MAKE_ERROR_CODE(SetSocketOptionError, set_socket_option_error_category)
} // namespace lpipp

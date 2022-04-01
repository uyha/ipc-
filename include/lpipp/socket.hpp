#pragma once
#include "file_descriptor.hpp"

#include <concepts>
#include <linux/filter.h>
#include <sys/socket.h>
#include <type_traits>

namespace lpipp {
// clang-format off
template <typename T>
concept SocketOption = std::is_same_v<std::remove_cv_t<decltype(T::level)>, int> &&
                       std::is_same_v<std::remove_cv_t<decltype(T::name)>, int> &&
                       std::is_same_v<std::remove_cv_t<decltype(T::read)>, bool> &&
                       std::is_same_v<std::remove_cv_t<decltype(T::write)>, bool> &&
                       requires (T option){
                        {option.value};
                        {option.size};
                       };
// clang-format on
template <typename T>
concept SocketReadOption = SocketOption<T> && T::read;
template <typename T>
concept SocketWriteOption = SocketOption<T> && T::write;

namespace socket_options {
LPIPP_SOCKOPT(AcceptConnection, SOL_SOCKET, SO_ACCEPTCONN, true, false, bool);
LPIPP_SOCKOPT(AttachFilter, SOL_SOCKET, SO_ATTACH_FILTER, true, true, ::sock_fprog);
LPIPP_SOCKOPT(AttachBPF, SOL_SOCKET, SO_ATTACH_BPF, true, true, int);
LPIPP_SOCKOPT(AttachReusePortCBPF, SOL_SOCKET, SO_ATTACH_REUSEPORT_CBPF, true, true, ::sock_fprog);
LPIPP_SOCKOPT(AttachReusePortEBPF, SOL_SOCKET, SO_ATTACH_REUSEPORT_EBPF, true, true, int);

template <std::size_t buffer_size>
LPIPP_SOCKOPT(BindToDevice, SOL_SOCKET, SO_BINDTODEVICE, true, true, char[buffer_size]);
template <std::size_t buffer_size>
BindToDevice(char const (&)[buffer_size]) -> BindToDevice<buffer_size>;

LPIPP_SOCKOPT(KeepAlive, SOL_SOCKET, SO_KEEPALIVE, true, true, bool);
} // namespace socket_options
} // namespace lpipp

namespace lpipp {
enum class GetSocketOptionError : int { option_unknown = ENOPROTOOPT, option_not_supported = EOPNOTSUPP };
enum class SetSocketOptionError : int {
  value_invalid        = EINVAL,
  option_unknown       = ENOPROTOOPT,
  option_not_supported = EOPNOTSUPP
};

LPIPP_DECLARE_MAKE_ERROR_CODE(GetSocketOptionError);
LPIPP_DECLARE_MAKE_ERROR_CODE(SetSocketOptionError);
} // namespace lpipp

LPIPP_IS_ERROR_CODE(lpipp::GetSocketOptionError)
LPIPP_IS_ERROR_CODE(lpipp::SetSocketOptionError)

namespace lpipp {
template <typename T>
class Socket : public FileDescriptor<T> {
public:
  template <SocketReadOption Option>
  auto get_option() -> tl::expected<Option, std::error_code> {
    auto option = Option{};

    auto const result = ::getsockopt(this->get_handle(), Option::level, Option::name, &option.value, &option.size);
    if (result == -1) {
      return tl::unexpected{static_cast<GetSocketOptionError>(errno)};
    }

    return option;
  }

  template <SocketWriteOption Option>
  auto set_option(Option const &option) -> tl::expected<void, std::error_code> {
    constexpr auto size = socklen_t{sizeof(option)};

    auto const result = ::setsockopt(this->get_handle(), Option::level, Option::name, &option, size);
    if (result == -1) {
      return tl::unexpected{static_cast<GetSocketOptionError>(errno)};
    }

    return {};
  }
};
} // namespace lpipp

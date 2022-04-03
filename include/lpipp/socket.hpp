#pragma once
#include "file_descriptor.hpp"

#include <concepts>
#include <linux/filter.h>
#include <sys/socket.h>
#include <type_traits>

namespace lpipp {

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

LPIPP_SOCKOPT(BroadCast, SOL_SOCKET, SO_BROADCAST, true, true, bool);
LPIPP_SOCKOPT(BsdCompat, SOL_SOCKET, SO_BSDCOMPAT, true, true, bool);
LPIPP_SOCKOPT(Debug, SOL_SOCKET, SO_DEBUG, true, true, bool);
LPIPP_SOCKOPT(DetachFilter, SOL_SOCKET, SO_DETACH_FILTER, true, true, void *);
LPIPP_SOCKOPT(DetachBPF, SOL_SOCKET, SO_DETACH_BPF, true, true, void *);
LPIPP_SOCKOPT(Domain, SOL_SOCKET, SO_DOMAIN, true, false, int);
LPIPP_SOCKOPT(Error, SOL_SOCKET, SO_ERROR, true, false, int);
LPIPP_SOCKOPT(DontRoute, SOL_SOCKET, SO_DONTROUTE, true, true, bool);
LPIPP_SOCKOPT(IncomingCPU, SOL_SOCKET, SO_INCOMING_CPU, true, true, int);
LPIPP_SOCKOPT(IncomingNAPICPU, SOL_SOCKET, SO_INCOMING_NAPI_ID, true, true, int);
LPIPP_SOCKOPT(KeepAlive, SOL_SOCKET, SO_KEEPALIVE, true, true, bool);
LPIPP_SOCKOPT(Linger, SOL_SOCKET, SO_LINGER, true, true, ::linger);
LPIPP_SOCKOPT(LockFilter, SOL_SOCKET, SO_LOCK_FILTER, true, true, bool);
} // namespace socket_options
} // namespace lpipp

namespace lpipp {
template <typename T>
class Socket : public FileDescriptor<T> {
public:
  template <concepts::socket_read_option Option>
  auto get_option() -> tl::expected<Option, std::error_code> {
    auto option = Option{};

    auto const result = ::getsockopt(this->get_handle(), Option::level, Option::name, &option.value, &option.size);
    if (result == -1) {
      return tl::unexpected{std::make_error_code(static_cast<std::errc>(errno))};
    }

    return option;
  }

  template <concepts::socket_write_option Option>
  auto set_option(Option const &option) -> tl::expected<void, std::error_code> {
    auto const result = ::setsockopt(this->get_handle(), Option::level, Option::name, &option, option.size);
    if (result == -1) {
      return tl::unexpected{std::make_error_code(static_cast<std::errc>(errno))};
    }

    return {};
  }
};
} // namespace lpipp

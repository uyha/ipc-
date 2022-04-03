#pragma once
#include "concepts.hpp"
#include "file_descriptor.hpp"

#include <concepts>
#include <linux/filter.h>
#include <sys/socket.h>
#include <type_traits>

namespace lpipp {

namespace socket_options {
template <int level_val, int name_val, bool read_val, bool write_val, typename value_t>
struct SocketOption {
  using value_type = value_t;

  static constexpr int level  = level_val;
  static constexpr int name   = name_val;
  static constexpr bool read  = read_val;
  static constexpr bool write = write_val;

  value_type value;
  ::socklen_t size = sizeof(value);
};
template <int level_val, int name_val, bool read_val, bool write_val>
struct SocketOption<level_val, name_val, read_val, write_val, bool> {
  using value_type = bool;

  static constexpr int level  = level_val;
  static constexpr int name   = name_val;
  static constexpr bool read  = read_val;
  static constexpr bool write = write_val;

  int value;
  ::socklen_t size = sizeof(value);

  SocketOption() = default;
  SocketOption(bool val)
      : value{val} {}
};
template <int level_val, int name_val, bool read_val, bool write_val>
struct SocketOption<level_val, name_val, read_val, write_val, void> {
  using value_type = void;

  static constexpr int level  = level_val;
  static constexpr int name   = name_val;
  static constexpr bool read  = read_val;
  static constexpr bool write = write_val;

  int value        = 0;
  ::socklen_t size = sizeof(value);
};
struct AcceptConnection : SocketOption<SOL_SOCKET, SO_ACCEPTCONN, true, false, bool> {};
struct AttachFilter : SocketOption<SOL_SOCKET, SO_ATTACH_FILTER, false, true, ::sock_fprog> {};
struct AttachBPF : SocketOption<SOL_SOCKET, SO_ATTACH_BPF, false, true, int> {};
struct AttachReusePortCBPF : SocketOption<SOL_SOCKET, SO_ATTACH_REUSEPORT_CBPF, false, true, ::sock_fprog> {};
struct AttachReusePortEBPF : SocketOption<SOL_SOCKET, SO_ATTACH_REUSEPORT_EBPF, false, true, int> {};

template <std::size_t buffer_size>
struct BindToDevice : SocketOption<SOL_SOCKET, SO_BINDTODEVICE, true, true, char[buffer_size]> {};
template <std::size_t buffer_size>
BindToDevice(char const (&)[buffer_size]) -> BindToDevice<buffer_size>;

struct BroadCast : SocketOption<SOL_SOCKET, SO_BROADCAST, true, true, bool> {};
struct BsdCompat : SocketOption<SOL_SOCKET, SO_BSDCOMPAT, true, true, bool> {};
struct Debug : SocketOption<SOL_SOCKET, SO_DEBUG, true, true, bool> {};
struct DetachFilter : SocketOption<SOL_SOCKET, SO_DETACH_FILTER, false, true, void> {};
struct DetachBPF : SocketOption<SOL_SOCKET, SO_DETACH_BPF, true, true, void> {};
struct Domain : SocketOption<SOL_SOCKET, SO_DOMAIN, true, false, int> {};
struct Error : SocketOption<SOL_SOCKET, SO_ERROR, true, false, int> {};
struct DontRoute : SocketOption<SOL_SOCKET, SO_DONTROUTE, true, true, bool> {};
struct IncomingCPU : SocketOption<SOL_SOCKET, SO_INCOMING_CPU, true, true, int> {};
struct IncomingNAPIID : SocketOption<SOL_SOCKET, SO_INCOMING_NAPI_ID, true, false, int> {};
struct KeepAlive : SocketOption<SOL_SOCKET, SO_KEEPALIVE, true, true, bool> {};
struct Linger : SocketOption<SOL_SOCKET, SO_LINGER, true, true, ::linger> {};
struct LockFilter : SocketOption<SOL_SOCKET, SO_LOCK_FILTER, true, true, int> {};
struct Mark : SocketOption<SOL_SOCKET, SO_MARK, true, true, int> {};
struct OutOfBoundInline : SocketOption<SOL_SOCKET, SO_OOBINLINE, true, true, bool> {};
struct PassCredentials : SocketOption<SOL_SOCKET, SO_PASSCRED, true, true, bool> {};
struct PassSecurity : SocketOption<SOL_SOCKET, SO_PASSSEC, true, true, bool> {};
struct PeekOff : SocketOption<SOL_SOCKET, SO_PEEK_OFF, true, true, int> {};
struct PeerCredentials : SocketOption<SOL_SOCKET, SO_PEERCRED, true, false, ::ucred> {};

template <std::size_t buffer_size>
struct PeerSeccurity : SocketOption<SOL_SOCKET, SO_PEERSEC, true, false, char[buffer_size]> {};

struct Priority : SocketOption<SOL_SOCKET, SO_PRIORITY, true, true, int> {};
struct Protocol : SocketOption<SOL_SOCKET, SO_PROTOCOL, true, false, int> {};
struct ReceiveBuffer : SocketOption<SOL_SOCKET, SO_RCVBUF, true, true, std::uint32_t> {};
struct ReceiveBufferForce : SocketOption<SOL_SOCKET, SO_RCVBUFFORCE, false, true, std::uint32_t> {};
struct ReceiveLowAt : SocketOption<SOL_SOCKET, SO_RCVLOWAT, true, true, int> {};
struct SendLowAt : SocketOption<SOL_SOCKET, SO_SNDLOWAT, true, false, int> {};
struct ReceiveTimeout : SocketOption<SOL_SOCKET, SO_RCVTIMEO, true, true, ::timeval> {};
struct SendTimeout : SocketOption<SOL_SOCKET, SO_SNDTIMEO, true, true, ::timeval> {};
struct ReuseAddress : SocketOption<SOL_SOCKET, SO_REUSEADDR, true, true, bool> {};
struct ReusePort : SocketOption<SOL_SOCKET, SO_REUSEPORT, true, true, bool> {};
// I can't figure out what is the full form of this
struct RxqOvfl : SocketOption<SOL_SOCKET, SO_RXQ_OVFL, true, true, bool> {};
struct SelectErrQueue : SocketOption<SOL_SOCKET, SO_SELECT_ERR_QUEUE, true, true, bool> {};
struct SendBuffer : SocketOption<SOL_SOCKET, SO_SNDBUF, true, true, std::uint32_t> {};
struct SendBufferForce : SocketOption<SOL_SOCKET, SO_SNDBUFFORCE, false, true, std::uint32_t> {};
struct Timestamp : SocketOption<SOL_SOCKET, SO_TIMESTAMP, true, true, bool> {};
struct TimestampNanoSecond : SocketOption<SOL_SOCKET, SO_TIMESTAMPNS, true, true, bool> {};
struct Type : SocketOption<SOL_SOCKET, SO_TYPE, true, false, int> {};
struct BusyPoll : SocketOption<SOL_SOCKET, SO_BUSY_POLL, true, true, int> {};
} // namespace socket_options

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

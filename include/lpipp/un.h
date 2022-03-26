#pragma once

#include "macros.hpp"

#include <atomic>
#include <sys/socket.h>
#include <sys/un.h>
#include <system_error>
#include <tl/expected.hpp>

namespace lpipp {
class un {
public:
  enum class CreateError {
    permission_denied                          = EACCES,
    file_descriptors_per_process_limit_reached = EMFILE,
    file_descriptors_system_wide_limit_reached = ENFILE,
    memory_insufficient                        = ENOMEM,
  };

  friend LPIPP_DECLARE_MAKE_ERROR_CODE(CreateError);

  enum class SocketType : int {
    stream    = SOCK_STREAM,
    dgram     = SOCK_DGRAM,
    seqpacket = SOCK_SEQPACKET,
  };

  static auto create(SocketType socket_type) noexcept -> tl::expected<un, std::error_code>;

  un(un const &)                     = delete;
  auto operator=(un const &) -> un & = delete;

  un(un &&other) noexcept;
  auto operator=(un &&other) noexcept -> un &;

  ~un() noexcept;

private:
  un(int fd) noexcept;

  int m_fd;
};
namespace un_constants {
constexpr un::SocketType Stream    = un::SocketType::stream;
constexpr un::SocketType Dgram     = un::SocketType::dgram;
constexpr un::SocketType SeqPacket = un::SocketType::seqpacket;
} // namespace un_constants
} // namespace lpipp

LPIPP_IS_ERROR_CODE(lpipp::un::CreateError)

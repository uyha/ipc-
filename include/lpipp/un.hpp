#pragma once

#include "macros.hpp"
#include "socket.hpp"

#include <atomic>
#include <sys/socket.h>
#include <sys/un.h>
#include <system_error>
#include <tl/expected.hpp>

namespace lpipp {
class un : public Socket<un> {
public:
  enum class CreateError {
    permission_denied                          = EACCES,
    file_descriptors_per_process_limit_reached = EMFILE,
    file_descriptors_system_wide_limit_reached = ENFILE,
    memory_insufficient                        = ENOMEM,
  };
  enum class BindError {
    access_denied         = EACCES,
    address_in_use        = EADDRINUSE,
    invalid               = EINVAL,
    address_not_available = EADDRNOTAVAIL,
    loop_detected         = ELOOP,
    name_too_long         = ENAMETOOLONG,
    directory_not_exist   = ENOENT,
    memory_insufficient   = ENOMEM,
    not_directory         = ENOTDIR,
    filesystem_readonly   = EROFS,
  };
  enum class AbstractBindError {
    address_in_use      = EADDRINUSE,
    invalid             = EINVAL,
    name_too_long       = ENAMETOOLONG,
    memory_insufficient = ENOMEM,
  };

  friend LPIPP_DECLARE_MAKE_ERROR_CODE(CreateError);
  friend LPIPP_DECLARE_MAKE_ERROR_CODE(BindError);
  friend LPIPP_DECLARE_MAKE_ERROR_CODE(AbstractBindError);

  enum class SocketType : int {
    stream    = SOCK_STREAM,
    dgram     = SOCK_DGRAM,
    seqpacket = SOCK_SEQPACKET,
  };
  enum class SocketFlag : int {
    non_block     = SOCK_NONBLOCK,
    close_on_exec = SOCK_CLOEXEC,
  };

  LPIPP_BITOR_OP(SocketType, SocketFlag, SocketType)
  LPIPP_BITOR_OP(SocketFlag, SocketFlag)

  static auto create(SocketType socket_type) noexcept -> tl::expected<un, std::error_code>;

  auto bind(std::string_view address) noexcept -> tl::expected<void, std::error_code>;
  auto bind_abstract(std::string_view address) noexcept -> tl::expected<void, std::error_code>;

  un(un const &)                     = delete;
  auto operator=(un const &) -> un & = delete;

  un(un &&other) noexcept;
  auto operator=(un &&other) noexcept -> un &;

  ~un() noexcept;

private:
  static constexpr std::size_t max_length = sizeof(((::sockaddr_un *)nullptr)->sun_path) - 1;

  un(int fd) noexcept;

  int m_fd;

  friend FileDescriptor<un>;
};
namespace un_constants {
constexpr un::SocketType Stream    = un::SocketType::stream;
constexpr un::SocketType Dgram     = un::SocketType::dgram;
constexpr un::SocketType SeqPacket = un::SocketType::seqpacket;

constexpr un::SocketFlag NonBlock    = un::SocketFlag::non_block;
constexpr un::SocketFlag CloseOnExec = un::SocketFlag::close_on_exec;
} // namespace un_constants
} // namespace lpipp

LPIPP_IS_ERROR_CODE(lpipp::un::CreateError)
LPIPP_IS_ERROR_CODE(lpipp::un::BindError)
LPIPP_IS_ERROR_CODE(lpipp::un::AbstractBindError)

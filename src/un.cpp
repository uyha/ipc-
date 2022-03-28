#include <cstring>
#include <lpipp/un.hpp>
#include <unistd.h>

namespace {
struct CreateErrorCategory : std::error_category {
  auto name() const noexcept -> char const * {
    return "un::CreateError";
  }
  auto message(int error) const -> std::string {
    switch (static_cast<lpipp::un::CreateError>(error)) {
    case lpipp::un::CreateError::permission_denied:
      return "Permission to create a socket of the specified type and/or protocol is denied";
    case lpipp::un::CreateError::file_descriptors_per_process_limit_reached:
      return "Too many file descriptors are being opened by this process";
    case lpipp::un::CreateError::file_descriptors_system_wide_limit_reached:
      return "Too many file descriptors and message queues are being opened in this system";
    case lpipp::un::CreateError::memory_insufficient: return "Insufficient memory for creating a socket";
    default: return "Unknown error";
    }
  }
} const un_create_error_category{};

struct BindErrorCategory : std::error_category {
  auto name() const noexcept -> char const * {
    return "un::BindError";
  }
  auto message(int error) const -> std::string {
    switch (static_cast<lpipp::un::BindError>(error)) {
    case lpipp::un::BindError::access_denied:
      return "The address is protected and the user is not the superuser, or the caller does not have search "
             "permission on the path prefix";
    case lpipp::un::BindError::address_in_use: return "The given address is already in use";
    case lpipp::un::BindError::invalid: return "Socket is already bound to another address, or address is invalid";
    case lpipp::un::BindError::address_not_available:
      return "A nonexistent interface was requested or the requested address was not local";
    case lpipp::un::BindError::loop_detected: return "Too many symbolic links were encountered in resolving address";
    case lpipp::un::BindError::name_too_long: return "address is too long";
    case lpipp::un::BindError::directory_not_exist:
      return "A component in the directory prefix of the socket pathname does not exist";
    case lpipp::un::BindError::memory_insufficient: return "Insufficient kernel memory was available";
    case lpipp::un::BindError::not_directory: return "A component of the path prefix is not a directory";
    case lpipp::un::BindError::filesystem_readonly: return "The socket inode would reside on a read-only filesystem";
    default: return "Unknown error";
    }
  }
} const un_bind_error_category{};

struct AbstractBindErrorCategory : std::error_category {
  auto name() const noexcept -> char const * {
    return "un::AbstractBindError";
  }
  auto message(int error) const -> std::string {
    switch (static_cast<lpipp::un::AbstractBindError>(error)) {
    case lpipp::un::AbstractBindError::address_in_use: return "The given address is already in use";
    case lpipp::un::AbstractBindError::invalid:
      return "Socket is already bound to another address, or address is invalid";
    case lpipp::un::AbstractBindError::name_too_long: return "address is too long";
    case lpipp::un::AbstractBindError::memory_insufficient: return "Insufficient kernel memory was available";
    default: return "Unknown error";
    }
  }
} const un_abstract_bind_error_category{};

} // namespace

namespace lpipp {
LPIPP_DEFINE_MAKE_ERROR_CODE(un::CreateError, un_create_error_category)
LPIPP_DEFINE_MAKE_ERROR_CODE(un::BindError, un_bind_error_category)
LPIPP_DEFINE_MAKE_ERROR_CODE(un::AbstractBindError, un_abstract_bind_error_category)

auto un::create(SocketType socket_type) noexcept -> tl::expected<un, std::error_code> {
  auto fd = ::socket(AF_UNIX, static_cast<int>(socket_type), 0);
  if (fd == -1) {
    return tl::unexpected{static_cast<CreateError>(errno)};
  }

  return un{fd};
}

auto un::bind(std::string_view address) noexcept -> tl::expected<void, std::error_code> {
  if (address.length() > max_length) {
    return tl::unexpected{BindError::name_too_long};
  }

  ::sockaddr_un socket_address{};
  socket_address.sun_family                   = AF_UNIX;
  socket_address.sun_path[std::size(address)] = '\0';
  std::memcpy(socket_address.sun_path, std::data(address), std::size(address));

  auto const result = ::bind(m_fd, reinterpret_cast<::sockaddr *>(&socket_address), sizeof(::sockaddr_un));
  if (result == -1) {
    auto const error = static_cast<BindError>(errno);
    // When the socket is bound more than once, an error will occur, but the specified path will still be created
    if (error == BindError::invalid) {
      ::remove(socket_address.sun_path);
    }
    return tl::unexpected{error};
  }

  m_address_bound = true;
  std::memcpy(m_address, socket_address.sun_path, std::size(address) + 1);

  return {};
}

auto un::bind_abstract(std::string_view address) noexcept -> tl::expected<void, std::error_code> {
  if (address.length() > max_length) {
    return tl::unexpected{AbstractBindError::name_too_long};
  }

  ::sockaddr_un socket_address{};
  socket_address.sun_family  = AF_UNIX;
  socket_address.sun_path[0] = '\0';
  std::memcpy(socket_address.sun_path + 1, std::data(address), std::size(address));
  auto const result = ::bind(m_fd,
                             reinterpret_cast<::sockaddr *>(&socket_address),
                             sizeof(socket_address.sun_family) + address.length() + 1);

  if (result == -1) {
    return tl::unexpected{static_cast<AbstractBindError>(errno)};
  }

  return {};
}

un::un(un &&other) noexcept
    : m_fd{other.m_fd}
    , m_address_bound{other.m_address_bound} {
  std::memcpy(m_address, other.m_address, std::size(m_address));

  other.m_fd            = -1;
  other.m_address_bound = false;
}
auto un::operator=(un &&other) noexcept -> un & {
  this->~un();

  m_fd            = other.m_fd;
  m_address_bound = other.m_address_bound;
  std::memcpy(m_address, other.m_address, std::size(m_address));

  other.m_fd            = -1;
  other.m_address_bound = false;

  return *this;
}

un::~un() noexcept {
  if (m_fd == -1) {
    return;
  }
  if (m_address_bound) {
    ::remove(m_address);
  }
  ::close(m_fd);
}

un::un(int fd) noexcept
    : m_fd{fd} {}
} // namespace lpipp

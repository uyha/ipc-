#include <lpipp/un.h>
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

} // namespace

namespace lpipp {
LPIPP_DEFINE_MAKE_ERROR_CODE(un::CreateError, un_create_error_category)

auto un::create(SocketType socket_type) noexcept -> tl::expected<un, std::error_code> {
  auto fd = ::socket(AF_UNIX, static_cast<int>(socket_type), 0);
  if (fd == -1) {
    return tl::unexpected{static_cast<CreateError>(errno)};
  }

  return un{fd};
}

un::un(un &&other) noexcept
    : m_fd{other.m_fd} {
  other.m_fd = -1;
}
auto un::operator=(un &&other) noexcept -> un & {
  m_fd       = other.m_fd;
  other.m_fd = -1;

  return *this;
}

un::~un() noexcept {
  if (m_fd == -1) {
    return;
  }
  ::close(m_fd);
}

un::un(int fd) noexcept
    : m_fd{fd} {}
} // namespace lpipp

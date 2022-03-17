#include <lpipp/shm.hpp>
#include <unistd.h>

namespace {
struct ShmOpenErrorCategory : std::error_category {
  auto name() const noexcept -> char const * {
    return "shm::OpenError";
  }
  auto message(int error) const -> std::string {
    switch (static_cast<lpipp::shm::OpenError>(error)) {
    case lpipp::shm::OpenError::permission_denied:
      return "Could not open shared memory in the specified mode, or the caller does not have write permission on the "
             "object while truncate was specified";
    case lpipp::shm::OpenError::shared_memory_existed:
      return "Could not create a shared memory exclusively since a shared memory with the same name already exists";
    case lpipp::shm::OpenError::name_invalid:
      return "The specified name is invalid, it must start with a slash (/), and have at least 1 character non slash "
             "character follow";
    case lpipp::shm::OpenError::file_descriptors_per_process_limit_reached:
      return "Too many file descriptors are being opened by this process";
    case lpipp::shm::OpenError::name_too_long: return "Name is too long";
    case lpipp::shm::OpenError::file_descriptors_system_wide_limit_reached:
      return "Too many file descriptors are being opened in this system";
    case lpipp::shm::OpenError::shared_memory_missing: return "The specified shared memory does not exist";
    default: return "Unknown error";
    }
  }
} const shm_open_error_category{};

struct ShmUnlinkErrorCategory : std::error_category {
  auto name() const noexcept -> char const * {
    return "shm::UnlinkError";
  }
  auto message(int error) const -> std::string {
    switch (static_cast<lpipp::shm::UnlinkError>(error)) {
    case lpipp::shm::UnlinkError::permission_denied:
      return "Caller does not have permission to unlink the specified shared memory";
    case lpipp::shm::UnlinkError::shared_memory_missing: return "The specified shared memory does not exist";
    default: return "Unknown error";
    }
  }
} const shm_unlink_error_category{};

struct ShmTruncateErrorCategory : std::error_category {
  auto name() const noexcept -> char const * {
    return "shm::TruncateError";
  }
  auto message(int error) const -> std::string {
    switch (static_cast<lpipp::shm::TruncateError>(error)) {
    case lpipp::shm::TruncateError::unwritable:
      return "Could not truncate the shared memory, it is not open for writing";
    default: return "Unknown error";
    }
  }
} const shm_truncate_error_category{};

struct ShmMapErrorCategory : std::error_category {
  auto name() const noexcept -> char const * {
    return "shm::MapError";
  }
  auto message(int error) const -> std::string {
    switch (static_cast<lpipp::shm::MapError>(error)) {
    case lpipp::shm::MapError::access_invalid:
      return "Shared mapping with write protection was requested, but the shared memory is not open for read/write "
             "mode";
    case lpipp::shm::MapError::file_or_memory_locked: return "Too much memory has been locked";
    case lpipp::shm::MapError::argument_invalid: return "Some arguments (address, length, offset) are invalid";
    case lpipp::shm::MapError::system_wide_open_files_limit_reached:
      return "Too many file descriptors are being opened in this system";
    case lpipp::shm::MapError::memory_insufficient: return "Insufficient memory";
    case lpipp::shm::MapError::overflown:
      return "Overflown would happen with the supplied arguments (length and offset are too large)";
    case lpipp::shm::MapError::permission_denied:
      return "hugetlb mapping was requested, but the caller does not the CAP_IPC_LOCK capability, and is not a member "
             "of the sysctl_hugetlb_shm_group group";
    default: return "Unknown error";
    }
  }
} const shm_map_error_category{};

struct ShmUnmapErrorCategory : std::error_category {
  auto name() const noexcept -> char const * {
    return "shm::UnmapError";
  }
  auto message(int error) const -> std::string {
    switch (static_cast<lpipp::shm::UnmapError>(error)) {
    case lpipp::shm::UnmapError::argument_invalid: return "Either address or/and length are invalid";
    case lpipp::shm::UnmapError::memory_insufficient:
      return "A mapping split would happen after unmap which could lead to the maximum number of mappings being "
             "exceeded";
    default: return "Unknown error";
    }
  }
} const shm_unmap_error_category{};

} // namespace

namespace lpipp {
LPIPP_DEFINE_MAKE_ERROR_CODE(shm::OpenError, shm_open_error_category)
LPIPP_DEFINE_MAKE_ERROR_CODE(shm::UnlinkError, shm_unlink_error_category)
LPIPP_DEFINE_MAKE_ERROR_CODE(shm::TruncateError, shm_truncate_error_category)
LPIPP_DEFINE_MAKE_ERROR_CODE(shm::MapError, shm_map_error_category)
LPIPP_DEFINE_MAKE_ERROR_CODE(shm::UnmapError, shm_unmap_error_category)

auto shm::open(char const *name, ReadOnlyMode mode) noexcept -> tl::expected<shm, std::error_code> {
  return shm::open(name, static_cast<int>(mode), 0);
}
auto shm::open(char const *name, ReadWriteMode mode) noexcept -> tl::expected<shm, std::error_code> {
  return shm::open(name, static_cast<int>(mode), 0);
}
auto shm::open(char const *name, ReadCreateMode mode, std::filesystem::perms permissions) noexcept
    -> tl::expected<shm, std::error_code> {
  return shm::open(name, static_cast<int>(mode), static_cast<::mode_t>(permissions));
}
auto shm::open(char const *name, ReadCreateMode mode, ::mode_t permissions) noexcept
    -> tl::expected<shm, std::error_code> {
  return shm::open(name, static_cast<int>(mode), permissions);
}
auto shm::open(char const *name, ReadWriteCreateMode mode, std::filesystem::perms permissions) noexcept
    -> tl::expected<shm, std::error_code> {
  return shm::open(name, static_cast<int>(mode), static_cast<::mode_t>(permissions));
}
auto shm::open(char const *name, ReadWriteCreateMode mode, ::mode_t permissions) noexcept
    -> tl::expected<shm, std::error_code> {
  return shm::open(name, static_cast<int>(mode), permissions);
}
auto shm::open(char const *name, int flags, ::mode_t mode) noexcept -> tl::expected<shm, std::error_code> {
  using tl::unexpected;

  auto fd = ::shm_open(name, flags, mode);
  if (fd == -1) {
    return unexpected{static_cast<OpenError>(errno)};
  }

  return shm{fd};
}

auto shm::unlink(char const *name) noexcept -> tl::expected<void, std::error_code> {
  if (auto result = ::shm_unlink(name); result == 0) {
    return {};
  } else {
    return tl::unexpected{static_cast<UnlinkError>(errno)};
  }
}

auto shm::truncate(::off_t length) const noexcept -> tl::expected<void, std::error_code> {
  if (auto result = ::ftruncate(m_fd, length); result == 0) {
    return {};
  } else {
    return tl::unexpected{TruncateError::unwritable};
  }
}

auto shm::map(std::size_t length, MapProtection protection, MapFlag map_flag, ::off_t offset) const noexcept
    -> tl::expected<void *, std::error_code> {
  return map(nullptr, length, protection, map_flag, offset);
}
auto shm::map(void *address,
              std::size_t length,
              MapProtection protection,
              MapFlag map_flag,
              ::off_t offset) const noexcept -> tl::expected<void *, std::error_code> {
  auto const result = ::mmap(address, length, static_cast<int>(protection), static_cast<int>(map_flag), m_fd, offset);

  if (result == MAP_FAILED) {
    return tl::unexpected{static_cast<MapError>(errno)};
  }

  return result;
}

auto shm::unmap(void *address, std::size_t length) noexcept -> tl::expected<void, std::error_code> {
  auto const result = ::munmap(address, length);

  if (result == -1) {
    return tl::unexpected{static_cast<UnmapError>(errno)};
  }

  return {};
}

shm::shm(shm &&other) noexcept
    : m_fd{other.m_fd} {
  other.m_fd = -1;
}
auto shm::operator=(shm &&other) noexcept -> shm & {
  m_fd       = other.m_fd;
  other.m_fd = -1;

  return *this;
}

shm::~shm() noexcept {
  if (m_fd == -1) {
    return;
  }
  ::close(m_fd);
}

shm::shm(int fd) noexcept
    : m_fd{fd} {}
} // namespace lpipp

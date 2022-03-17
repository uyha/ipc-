#pragma once

#include "macros.hpp"

#include <fcntl.h>
#include <filesystem>
#include <sys/mman.h>
#include <tl/expected.hpp>

#if __has_include(<linux/mman.h>)
#include <linux/mman.h>
#endif

namespace lpipp {
class shm {
public:
  enum class OpenError {
    permission_denied                          = EACCES,
    shared_memory_existed                      = EEXIST,
    name_invalid                               = EINVAL,
    file_descriptors_per_process_limit_reached = EMFILE,
    name_too_long                              = ENAMETOOLONG,
    file_descriptors_system_wide_limit_reached = ENFILE,
    shared_memory_missing                      = ENOENT,
  };
  enum class UnlinkError { permission_denied = EACCES, shared_memory_missing = ENOENT };
  enum class TruncateError { unwritable = EINVAL };
  enum class MapError {
    access_invalid                       = EACCES,
    file_or_memory_locked                = EAGAIN,
    argument_invalid                     = EINVAL,
    system_wide_open_files_limit_reached = ENFILE,
    memory_insufficient                  = ENOMEM,
    overflown                            = EOVERFLOW,
    permission_denied                    = EPERM,
  };
  enum class UnmapError { argument_invalid = EINVAL, memory_insufficient = ENOMEM };

  friend LPIPP_DECLARE_MAKE_ERROR_CODE(OpenError);
  friend LPIPP_DECLARE_MAKE_ERROR_CODE(UnlinkError);
  friend LPIPP_DECLARE_MAKE_ERROR_CODE(TruncateError);
  friend LPIPP_DECLARE_MAKE_ERROR_CODE(MapError);
  friend LPIPP_DECLARE_MAKE_ERROR_CODE(UnmapError);

  enum class ReadOnlyMode : int { read_only = O_RDONLY };
  enum class ReadWriteMode : int { read_write = O_RDWR };
  enum class CreateMode : int { create = O_CREAT };
  enum class ExclusiveMode : int { exclusive = O_EXCL };
  enum class TruncateMode : int { truncate = O_TRUNC };

  enum class ReadCreateMode : int {};
  enum class ReadWriteCreateMode : int {};

  enum class CreateTruncateMode : int {};

  LPIPP_BITOR_OP(ReadOnlyMode, CreateMode, ReadCreateMode)
  LPIPP_BITOR_OP(ReadOnlyMode, ExclusiveMode, ReadOnlyMode)

  LPIPP_BITOR_OP(ReadWriteMode, CreateMode, ReadWriteCreateMode)
  LPIPP_BITOR_OP(ReadWriteMode, ExclusiveMode, ReadWriteMode)
  LPIPP_BITOR_OP(ReadWriteMode, TruncateMode, ReadWriteMode)
  LPIPP_BITOR_OP(ReadWriteMode, CreateTruncateMode, ReadWriteCreateMode)

  LPIPP_BITOR_OP(CreateMode, ExclusiveMode, CreateMode)
  LPIPP_BITOR_OP(CreateMode, TruncateMode, CreateTruncateMode)

  LPIPP_BITOR_OP(ExclusiveMode, TruncateMode, TruncateMode)
  LPIPP_BITOR_OP(ExclusiveMode, ReadCreateMode, ReadCreateMode)
  LPIPP_BITOR_OP(ExclusiveMode, ReadWriteCreateMode, ReadWriteCreateMode)
  LPIPP_BITOR_OP(ExclusiveMode, CreateTruncateMode, CreateTruncateMode)

  LPIPP_BITOR_OP(TruncateMode, ReadWriteCreateMode, ReadWriteCreateMode)

  [[nodiscard]] static auto open(char const *name, ReadOnlyMode mode) noexcept -> tl::expected<shm, std::error_code>;
  [[nodiscard]] static auto open(char const *name, ReadWriteMode mode) noexcept -> tl::expected<shm, std::error_code>;
  [[nodiscard]] static auto open(char const *name, ReadCreateMode mode, std::filesystem::perms permissions) noexcept
      -> tl::expected<shm, std::error_code>;
  [[nodiscard]] static auto open(char const *name, ReadCreateMode mode, ::mode_t permissions) noexcept
      -> tl::expected<shm, std::error_code>;
  [[nodiscard]] static auto open(char const *name,
                                 ReadWriteCreateMode mode,
                                 std::filesystem::perms permissions) noexcept -> tl::expected<shm, std::error_code>;
  [[nodiscard]] static auto open(char const *name, ReadWriteCreateMode mode, ::mode_t permissions) noexcept
      -> tl::expected<shm, std::error_code>;

  [[nodiscard]] static auto unlink(char const *name) noexcept -> tl::expected<void, std::error_code>;

  [[nodiscard]] auto truncate(::off_t length) const noexcept -> tl::expected<void, std::error_code>;

  enum class MapProtection { exec, read, write };
  enum class MapFlag { shared = MAP_SHARED, shared_validate = MAP_SHARED_VALIDATE, map_private = MAP_PRIVATE };
  enum class MapExtraFlag {
    map_32bit = MAP_32BIT,
    anonymous = MAP_ANONYMOUS,
    growsdown = MAP_GROWSDOWN,
    hugetlb   = MAP_HUGETLB,
    locked    = MAP_LOCKED,
    nonblock  = MAP_NONBLOCK,
    noreserve = MAP_NORESERVE,
    populate  = MAP_POPULATE,
    stack     = MAP_STACK,
    sync      = MAP_SYNC,

#if defined MAP_UNINITIALIZED
    uninitialized = MAP_UNINITIALIZED
#endif
  };

  LPIPP_BITOR_OP(MapProtection, MapProtection)

  LPIPP_BITOR_OP(MapFlag, MapExtraFlag, MapFlag)

  [[nodiscard]] auto map(std::size_t length,
                         MapProtection protection,
                         MapFlag map_flag,
                         ::off_t offset = 0) const noexcept -> tl::expected<void *, std::error_code>;
  [[nodiscard]] auto map(void *address,
                         std::size_t length,
                         MapProtection protection,
                         MapFlag map_flag,
                         ::off_t offset = 0) const noexcept -> tl::expected<void *, std::error_code>;

  [[nodiscard]] static auto unmap(void *address, std::size_t length) noexcept -> tl::expected<void, std::error_code>;

  shm(shm const &)                     = delete;
  auto operator=(shm const &) -> shm & = delete;

  shm(shm &&other) noexcept;
  auto operator=(shm &&other) noexcept -> shm &;

  ~shm() noexcept;

private:
  [[nodiscard]] static auto open(char const *name, int flags, ::mode_t mode) noexcept
      -> tl::expected<shm, std::error_code>;

  shm(int fd) noexcept;

  int m_fd;
};
namespace shm_constants {
constexpr shm::ReadOnlyMode read_only   = shm::ReadOnlyMode::read_only;
constexpr shm::ReadWriteMode read_write = shm::ReadWriteMode::read_write;
constexpr shm::CreateMode create        = shm::CreateMode::create;
constexpr shm::ExclusiveMode exclusive  = shm::ExclusiveMode::exclusive;
constexpr shm::TruncateMode truncate    = shm::TruncateMode::truncate;

constexpr shm::MapProtection exec  = shm::MapProtection::exec;
constexpr shm::MapProtection read  = shm::MapProtection::read;
constexpr shm::MapProtection write = shm::MapProtection::write;

constexpr shm::MapFlag shared          = shm::MapFlag::shared;
constexpr shm::MapFlag shared_validate = shm::MapFlag::shared_validate;
constexpr shm::MapFlag map_private     = shm::MapFlag::map_private;

constexpr shm::MapExtraFlag map_32bit = shm::MapExtraFlag::map_32bit;
constexpr shm::MapExtraFlag anonymous = shm::MapExtraFlag::anonymous;
constexpr shm::MapExtraFlag growsdown = shm::MapExtraFlag::growsdown;
constexpr shm::MapExtraFlag hugetlb   = shm::MapExtraFlag::hugetlb;
constexpr shm::MapExtraFlag locked    = shm::MapExtraFlag::locked;
constexpr shm::MapExtraFlag nonblock  = shm::MapExtraFlag::nonblock;
constexpr shm::MapExtraFlag noreserve = shm::MapExtraFlag::noreserve;
constexpr shm::MapExtraFlag populate  = shm::MapExtraFlag::populate;
constexpr shm::MapExtraFlag stack     = shm::MapExtraFlag::stack;
constexpr shm::MapExtraFlag sync      = shm::MapExtraFlag::sync;
#if defined MAP_UNINITIALIZED
constexpr shm::MapExtraFlag uninitialized = shm::MapExtraFlag::uninitialized;
#endif
} // namespace shm_constants
} // namespace lpipp

LPIPP_IS_ERROR_CODE(lpipp::shm::OpenError)
LPIPP_IS_ERROR_CODE(lpipp::shm::UnlinkError)
LPIPP_IS_ERROR_CODE(lpipp::shm::TruncateError)
LPIPP_IS_ERROR_CODE(lpipp::shm::MapError)
LPIPP_IS_ERROR_CODE(lpipp::shm::UnmapError)

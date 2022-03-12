#pragma once

#include "macros.hpp"

#include <fcntl.h>
#include <filesystem>
#include <tl/expected.hpp>

namespace lpipp {
class shm {
public:
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

  enum class OpenError {
    permission_denied,
    shared_memory_existed,
    name_invalid,
    file_descriptors_per_process_limit_reached,
    name_too_long,
    file_descriptors_system_wide_limit_reached,
    shared_memory_missing,
  };
  [[nodiscard]] static auto open(char const *name, ReadOnlyMode mode) noexcept -> tl::expected<shm, OpenError>;
  [[nodiscard]] static auto open(char const *name, ReadWriteMode mode) noexcept -> tl::expected<shm, OpenError>;
  [[nodiscard]] static auto open(char const *name, ReadCreateMode mode, std::filesystem::perms permissions) noexcept
      -> tl::expected<shm, OpenError>;
  [[nodiscard]] static auto open(char const *name, ReadCreateMode mode, ::mode_t permissions) noexcept
      -> tl::expected<shm, OpenError>;
  [[nodiscard]] static auto open(char const *name,
                                 ReadWriteCreateMode mode,
                                 std::filesystem::perms permissions) noexcept -> tl::expected<shm, OpenError>;
  [[nodiscard]] static auto open(char const *name, ReadWriteCreateMode mode, ::mode_t permissions) noexcept
      -> tl::expected<shm, OpenError>;

  enum class UnlinkError { permission_denied, shared_memory_missing };
  [[nodiscard]] static auto unlink(char const *name) noexcept -> tl::expected<void, UnlinkError>;

  enum class TruncateError { unwritable };
  [[nodiscard]] auto truncate(::off_t length) const noexcept -> tl::expected<void, TruncateError>;

  shm(shm const &)                     = delete;
  auto operator=(shm const &) -> shm & = delete;

  shm(shm &&other) noexcept;
  auto operator=(shm &&other) noexcept -> shm &;

  ~shm() noexcept;

private:
  [[nodiscard]] static auto map_open_error(int error) noexcept -> OpenError {
    switch (error) {
    case EACCES: return OpenError::permission_denied;
    case EEXIST: return OpenError::shared_memory_existed;
    case EINVAL: return OpenError::name_invalid;
    case EMFILE: return OpenError::file_descriptors_per_process_limit_reached;
    case ENAMETOOLONG: return OpenError::name_too_long;
    case ENFILE: return OpenError::file_descriptors_system_wide_limit_reached;
    case ENOENT: return OpenError::shared_memory_missing;
    default: return static_cast<OpenError>(error);
    }
  }
  [[nodiscard]] static auto map_unlink_error(int error) noexcept -> UnlinkError {
    switch (error) {
    case EACCES: return UnlinkError::permission_denied;
    case ENOENT: return UnlinkError::shared_memory_missing;
    default: return static_cast<UnlinkError>(error);
    }
  }

  [[nodiscard]] static auto open(char const *name, int flags, ::mode_t mode) noexcept -> tl::expected<shm, OpenError>;

  shm(int fd) noexcept;

  int m_fd;
};
namespace shm_constants {
constexpr shm::ReadOnlyMode read_only   = shm::ReadOnlyMode::read_only;
constexpr shm::ReadWriteMode read_write = shm::ReadWriteMode::read_write;
constexpr shm::CreateMode create        = shm::CreateMode::create;
constexpr shm::ExclusiveMode exclusive  = shm::ExclusiveMode::exclusive;
constexpr shm::TruncateMode truncate    = shm::TruncateMode::truncate;
} // namespace shm_constants
} // namespace lpipp

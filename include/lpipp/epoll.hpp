#pragma once
#include "concepts.hpp"
#include "fcntl.hpp"
#include "macros.hpp"

#include <asm-generic/errno-base.h>
#include <cstdint>
#include <sys/epoll.h>
#include <tl/expected.hpp>

namespace lpipp {
class epoll : fcntl<epoll> {
public:
  enum class CreateError { too_many_process_files, too_many_system_files, memory_insufficient };
  [[nodiscard]] static auto create(bool close_on_exec = false) noexcept -> tl::expected<epoll, CreateError>;

  enum class InOutEvent : std::uint32_t {
    in  = EPOLLIN,
    out = EPOLLOUT,
  };
  enum class SpecialEvent : std::uint32_t {
    rdhup = EPOLLRDHUP,
    pri   = EPOLLPRI,
  };
  enum class RequestEvent : std::uint32_t {
    // It's possible that this event is ignored if the process doesn't have CAP_BLOCK_SUSPEND capability
    wakeup = EPOLLWAKEUP,
    et     = EPOLLET,
  };
  enum class OneShotEvent : std::uint32_t {
    oneshot = EPOLLONESHOT,
  };
  enum class ExclusiveEvent : std::uint32_t {
    exclusive = EPOLLEXCLUSIVE,
  };
  enum class ResponseEvent : std::uint32_t {
    err = EPOLLERR,
    hup = EPOLLHUP,
  };

  LPIPP_BITOR_OP(InOutEvent, InOutEvent)
  LPIPP_BITOR_OP(InOutEvent, SpecialEvent, SpecialEvent)
  LPIPP_BITOR_OP(InOutEvent, RequestEvent, RequestEvent)
  LPIPP_BITOR_OP(InOutEvent, OneShotEvent, OneShotEvent)
  LPIPP_BITOR_OP(InOutEvent, ExclusiveEvent, ExclusiveEvent)
  LPIPP_BITOR_OP(InOutEvent, ResponseEvent, ResponseEvent)

  LPIPP_BITOR_OP(SpecialEvent, SpecialEvent)
  LPIPP_BITOR_OP(SpecialEvent, RequestEvent, OneShotEvent)
  LPIPP_BITOR_OP(SpecialEvent, OneShotEvent, OneShotEvent)
  LPIPP_BITOR_OP(SpecialEvent, ResponseEvent, ResponseEvent)

  LPIPP_BITOR_OP(RequestEvent, RequestEvent)
  LPIPP_BITOR_OP(RequestEvent, OneShotEvent, OneShotEvent)
  LPIPP_BITOR_OP(RequestEvent, ExclusiveEvent, ExclusiveEvent)

  LPIPP_BITOR_OP(OneShotEvent, OneShotEvent)

  LPIPP_BITOR_OP(ExclusiveEvent, ExclusiveEvent)

  LPIPP_BITOR_OP(ResponseEvent, RequestEvent)

  LPIPP_BITAND_OP(ResponseEvent)
  LPIPP_BITAND_OP(ResponseEvent, InOutEvent)
  LPIPP_BITAND_OP(ResponseEvent, SpecialEvent)

  enum class AddError {
    file_descriptor_invalid,
    file_descriptor_existed,
    file_descriptor_same_with_epoll,
  };
  template <concepts::is_any_of<CommonEvent, RequestEvent> Event,
            concepts::is_any_of<void *, int, std::uint32_t, std::uint64_t> Data>
  [[nodiscard]] auto add(int fd, Event events, Data data) const noexcept -> tl::expected<void, AddError>;

  epoll(epoll const &) = delete;
  auto operator=(epoll const &) -> epoll & = delete;

  epoll(epoll &&other) noexcept;
  auto operator=(epoll &&other) noexcept -> epoll &;

  ~epoll() noexcept;

private:
  epoll(int fd) noexcept;

  int m_fd;

  [[nodiscard]] static constexpr auto map_create_error(int error) noexcept -> CreateError {
    switch (error) {
    case EMFILE: return CreateError::too_many_process_files;
    case ENFILE: return CreateError::too_many_system_files;
    case ENOMEM: return CreateError::memory_insufficient;
    default: return static_cast<CreateError>(error);
    }
  }
  [[nodiscard]] static constexpr auto map_add_error(int error) noexcept -> AddError {
    switch (error) {
    case EBADF: return AddError::file_descriptor_invalid;
    case EEXIST: return AddError::file_descriptor_existed;
    default: return static_cast<AddError>(error);
    }
  }
};
namespace epoll_constants {
constexpr epoll::InOutEvent in            = epoll::InOutEvent::in;
constexpr epoll::InOutEvent out           = epoll::InOutEvent::out;
constexpr epoll::SpecialEvent rdhup       = epoll::SpecialEvent::rdhup;
constexpr epoll::SpecialEvent pri         = epoll::SpecialEvent::pri;
constexpr epoll::RequestEvent wakeup      = epoll::RequestEvent::wakeup;
constexpr epoll::RequestEvent et          = epoll::RequestEvent::et;
constexpr epoll::OneShotEvent oneshot     = epoll::OneShotEvent::oneshot;
constexpr epoll::ExclusiveEvent exclusive = epoll::ExclusiveEvent::exclusive;
constexpr epoll::ResponseEvent err        = epoll::ResponseEvent::err;
constexpr epoll::ResponseEvent hup        = epoll::ResponseEvent::hup;
} // namespace epoll_constants
} // namespace lpipp

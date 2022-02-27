#pragma once
#include "concepts.hpp"
#include "macros.hpp"

#include <asm-generic/errno-base.h>
#include <cstdint>
#include <sys/epoll.h>
#include <tl/expected.hpp>

namespace lpipp {
class epoll {
public:
  enum class CreateError { too_many_process_files, too_many_system_files, memory_insufficient };
  [[nodiscard]] static auto create(bool close_on_exec = false) noexcept -> tl::expected<epoll, CreateError>;

  enum class CommonEvent : std::uint32_t {
    in    = EPOLLIN,
    out   = EPOLLOUT,
    rdhup = EPOLLRDHUP,
    pri   = EPOLLPRI,
  };
  enum class RequestEvent : std::uint32_t {
    et      = EPOLLET,
    oneshot = EPOLLONESHOT,
    // It's possible that this event is ignored if the process doesn't have CAP_BLOCK_SUSPEND
    wakeup    = EPOLLWAKEUP,
    exclusive = EPOLLEXCLUSIVE,
  };
  enum class ResponseEvent : std::uint32_t {
    err = EPOLLERR,
    hup = EPOLLHUP,
  };

  LPIPP_BITOR_OP(CommonEvent, CommonEvent)

  enum class AddError {
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
};
namespace epoll_constants {
constexpr epoll::CommonEvent in         = epoll::CommonEvent::in;
constexpr epoll::CommonEvent out        = epoll::CommonEvent::out;
constexpr epoll::CommonEvent rdhup      = epoll::CommonEvent::rdhup;
constexpr epoll::CommonEvent pri        = epoll::CommonEvent::pri;
constexpr epoll::RequestEvent et        = epoll::RequestEvent::et;
constexpr epoll::RequestEvent oneshot   = epoll::RequestEvent::oneshot;
constexpr epoll::RequestEvent wakeup    = epoll::RequestEvent::wakeup;
constexpr epoll::RequestEvent exclusive = epoll::RequestEvent::exclusive;
constexpr epoll::ResponseEvent err      = epoll::ResponseEvent::err;
constexpr epoll::ResponseEvent hup      = epoll::ResponseEvent::hup;
} // namespace epoll_constants
} // namespace lpipp

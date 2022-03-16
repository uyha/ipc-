#pragma once
#include "concepts.hpp"
#include "fcntl.hpp"
#include "macros.hpp"

#include <asm-generic/errno-base.h>
#include <bits/types/sigset_t.h>
#include <bits/types/struct_timespec.h>
#include <chrono>
#include <cstdint>
#include <sys/epoll.h>
#include <tl/expected.hpp>

namespace lpipp {

template <typename T>
concept EpollEventData = concepts::is_any_of<T, void *, int, std::uint32_t, std::uint64_t>;

template <EpollEventData Data>
constexpr auto set_epoll_event_data(::epoll_event &event, Data data) noexcept {
  if constexpr (std::is_same_v<Data, void *>) {
    event.data.ptr = data;
  } else if constexpr (std::is_same_v<Data, int>) {
    event.data.fd = data;
  } else if constexpr (std::is_same_v<Data, std::uint32_t>) {
    event.data.u32 = data;
  } else if constexpr (std::is_same_v<Data, std::uint64_t>) {
    event.data.u64 = data;
  }
}

class epoll : public fcntl<epoll> {
public:
  enum class CreateError {
    file_descriptors_per_process_limit_reached = EMFILE,
    file_descriptors_system_limit_reached      = ENFILE,
    memory_insufficient                        = ENOMEM,
  };
  enum class AddError {
    file_descriptor_invalid       = EBADF,
    file_descriptor_existed       = EEXIST,
    exclusive_on_epoll_error      = EINVAL,
    file_descriptor_not_supported = EPERM,
    loop_detected                 = ELOOP,
    memory_insufficient           = ENOMEM,
    max_watches_reached           = ENOSPC,
    file_descriptor_same_with_epoll,
  };
  enum class RemoveError { file_descriptor_not_registered = ENOENT };
  enum class ModifyError {
    file_descriptor_invalid        = EBADF,
    exclusive_previously_specified = EINVAL,
    file_descriptor_not_registered = ENOENT,
    memory_insufficient            = ENOMEM,
    file_descriptor_same_with_epoll,
  };
  enum class WaitError {
    events_buffer_not_accessible = EFAULT,
    interrupted                  = EINTR,
    max_events_invalid           = EINVAL,
    timeout
  };

  friend LPIPP_DECLARE_MAKE_ERROR_CODE(CreateError);
  friend LPIPP_DECLARE_MAKE_ERROR_CODE(AddError);
  friend LPIPP_DECLARE_MAKE_ERROR_CODE(RemoveError);
  friend LPIPP_DECLARE_MAKE_ERROR_CODE(ModifyError);
  friend LPIPP_DECLARE_MAKE_ERROR_CODE(WaitError);

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

  [[nodiscard]] static auto create(bool close_on_exec = false) noexcept -> tl::expected<epoll, std::error_code>;

  template <concepts::is_any_of<InOutEvent, SpecialEvent, RequestEvent, OneShotEvent, ExclusiveEvent> Event,
            EpollEventData Data>
  [[nodiscard]] auto add(int fd, Event events, Data data) const noexcept -> tl::expected<void, std::error_code>;

  template <concepts::is_any_of<InOutEvent, SpecialEvent, RequestEvent, OneShotEvent> Event, EpollEventData Data>
  [[nodiscard]] auto modify(int fd, Event events, Data data) const noexcept -> tl::expected<void, std::error_code>;

  [[nodiscard]] auto remove(int fd) const noexcept -> tl::expected<void, std::error_code>;

  [[nodiscard]] auto wait(::epoll_event *event_list, int max_events, ::sigset_t const *sigmask = nullptr) const noexcept
      -> tl::expected<int, std::error_code> {
    return wait(event_list, max_events, -1, sigmask);
  }
  template <typename DurationRep, typename DurationPeriod>
  [[nodiscard]] auto wait(::epoll_event *event_list,
                          int max_events,
                          std::chrono::duration<DurationRep, DurationPeriod> wait_duration,
                          ::sigset_t const *sigmask = nullptr) const noexcept -> tl::expected<int, std::error_code> {
    int timeout = std::chrono::duration_cast<std::chrono::milliseconds>(wait_duration).count();
    return wait(event_list, max_events, timeout, sigmask);
  }

  epoll(epoll const &)                     = delete;
  auto operator=(epoll const &) -> epoll & = delete;

  epoll(epoll &&other) noexcept;
  auto operator=(epoll &&other) noexcept -> epoll &;

  ~epoll() noexcept;

private:
  epoll(int fd) noexcept;

  int m_fd;

  friend fcntl<epoll>;

  [[nodiscard]] auto wait(::epoll_event *event_list,
                          int max_events,
                          int timeout_millisecs,
                          ::sigset_t const *sigmask = nullptr) const noexcept -> tl::expected<int, std::error_code>;
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

LPIPP_IS_ERROR_CODE(lpipp::epoll::CreateError)
LPIPP_IS_ERROR_CODE(lpipp::epoll::AddError)
LPIPP_IS_ERROR_CODE(lpipp::epoll::RemoveError)
LPIPP_IS_ERROR_CODE(lpipp::epoll::ModifyError)
LPIPP_IS_ERROR_CODE(lpipp::epoll::WaitError)

namespace lpipp {
template <concepts::is_any_of<epoll::InOutEvent,
                              epoll::SpecialEvent,
                              epoll::RequestEvent,
                              epoll::OneShotEvent,
                              epoll::ExclusiveEvent> Event,
          EpollEventData Data>
[[nodiscard]] auto epoll::add(int fd, Event events, Data data) const noexcept -> tl::expected<void, std::error_code> {
  if (m_fd == fd) {
    return tl::unexpected{AddError::file_descriptor_same_with_epoll};
  }
  auto event_arg   = ::epoll_event{};
  event_arg.events = static_cast<std::uint32_t>(events);
  set_epoll_event_data(event_arg, data);

  auto result = ::epoll_ctl(m_fd, EPOLL_CTL_ADD, fd, &event_arg);
  if (result == -1) {
    return tl::unexpected{static_cast<AddError>(errno)};
  }
  return {};
}

template <concepts::is_any_of<epoll::InOutEvent, epoll::SpecialEvent, epoll::RequestEvent, epoll::OneShotEvent> Event,
          EpollEventData Data>
[[nodiscard]] auto epoll::modify(int fd, Event events, Data data) const noexcept
    -> tl::expected<void, std::error_code> {
  if (m_fd == fd) {
    return tl::unexpected{ModifyError::file_descriptor_same_with_epoll};
  }
  auto event_arg   = ::epoll_event{};
  event_arg.events = static_cast<std::uint32_t>(events);
  set_epoll_event_data(event_arg, data);

  auto result = ::epoll_ctl(m_fd, EPOLL_CTL_MOD, fd, &event_arg);
  if (result == -1) {
    return tl::unexpected{static_cast<ModifyError>(errno)};
  }
  return {};
}
} // namespace lpipp

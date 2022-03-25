#pragma once

#include <atomic>
#include <sys/un.h>

namespace lpipp {
class un {
public:
  un(un const &)                     = delete;
  auto operator=(un const &) -> un & = delete;

  un(un &&other) noexcept;
  auto operator=(un &&other) noexcept -> un &;

  ~un() noexcept;

private:
  un(int fd) noexcept;

  int m_fd;
};
} // namespace lpipp

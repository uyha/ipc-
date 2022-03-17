#include <lpipp/epoll.hpp>

int main() {
  using namespace lpipp;
  using namespace epoll_constants;

  auto epoll = epoll::create();
  (void)epoll->modify(0, Exclusive, 0);
}

#include <filesystem>
#include <fmt/printf.h>
#include <lpipp/mq.hpp>

int main() {
  using namespace lpipp::mq_constants;
  (void)lpipp::mq::unlink("/test");
  auto message_queue = lpipp::mq::open("/test", ReadOnly | Create | Exclusive, 0666);

  fmt::print("{}", message_queue.has_value());
}

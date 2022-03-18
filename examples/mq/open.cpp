#include <filesystem>
#include <fmt/printf.h>
#include <lpipp/mq.hpp>

int main() {
  using namespace lpipp::mq_constants;
  namespace fs = std::filesystem;
  auto mode    = ReadOnly;
  auto permissions = fs::perms::owner_read;
  auto message_queue = lpipp::mq::open("/test", mode);

  fmt::print("{}", message_queue.has_value());
}

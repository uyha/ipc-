#include <filesystem>
#include <fmt/printf.h>
#include <ipcpp/mq.hpp>

int main() {
  using namespace ipcpp::mq_constants;
  namespace fs = std::filesystem;
  auto mode    = read_only;
  auto permissions = fs::perms::owner_read;
  auto message_queue = ipcpp::mq::open("/test", mode);

  fmt::print("{}", message_queue.has_value());
}

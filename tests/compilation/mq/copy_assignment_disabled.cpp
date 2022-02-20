#include <lpipp/mq.hpp>
using namespace lpipp;
using namespace mq_constants;

int main() {
  auto name        = "/mq-copy-assignment-disabled";
  auto const queue = *mq::open(name, read_only);
  auto copy_queue  = queue;
  (void)mq::unlink(name);
}

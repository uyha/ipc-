#include <lpipp/mq.hpp>
using namespace lpipp;
using namespace mq_constants;

int main() {
  auto name        = "/mq-copy-assignment-disabled";
  auto const queue = *mq::open(name, ReadOnly);
  auto copy_queue  = queue;
  (void)mq::unlink(name);
}

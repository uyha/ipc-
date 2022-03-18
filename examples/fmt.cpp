#include <fmt/printf.h>
#include <lpipp/fmt.hpp>

int main() {
  struct ::stat local_stat {};
  fmt::print("{}\n", local_stat);
}

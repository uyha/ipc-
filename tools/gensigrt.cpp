#include <fmt/compile.h>
#include <fmt/os.h>
#include <fstream>
#include <signal.h>

int main(int argc, char **argv) {
  auto file = fmt::output_file(argv[1], O_WRONLY | O_CREAT | O_TRUNC);
  file.print("#pragma once\n"
             "namespace ipcpp {{\n"
             "constexpr auto signal_realtime_min = {};\n"
             "constexpr auto signal_realtime_max = {};\n"
             "}}",
             SIGRTMIN,
             SIGRTMAX);
}

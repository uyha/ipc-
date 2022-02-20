#include <malloc.h>
#include <signal.h>
#include <stdio.h>

int main(int argc, char **argv) {
  FILE *file          = fopen(argv[1], "w+");
  char const format[] = "#pragma once\n"
                        "namespace lpipp {\n"
                        "constexpr auto signal_realtime_min = %d;\n"
                        "constexpr auto signal_realtime_max = %d;\n"
                        "}";
  // Add 20 extra bytes for the content of SIGRTMIN and SIGRTMAX
  char *buffer = (char *)malloc(sizeof(format) + 20);

  int len = sprintf(buffer, format, SIGRTMIN, SIGRTMAX);
  fwrite(buffer, sizeof(char), len, file);

  free(buffer);
  fclose(file);
}

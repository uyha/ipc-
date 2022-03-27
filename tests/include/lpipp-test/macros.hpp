#pragma once

#define QUOTE(s) #s
#define UNIQUE_STR_IMPL(prefix, line) prefix QUOTE(line)

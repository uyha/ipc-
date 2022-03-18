#include <sys/stat.h>
#include <fmt/core.h>

template <>
struct fmt::formatter<::timespec> {
  constexpr auto parse(format_parse_context &context) -> decltype(context.begin()) {
    return context.begin();
  }
  template <typename Context>
  auto format(::timespec const &value, Context &context) -> decltype(context.out()) {
    return format_to(context.out(), "{{tv_sec = {}, tv_nsec = {}}}", value.tv_sec, value.tv_nsec);
  }
};
template <>
struct fmt::formatter<struct ::stat> {
  constexpr auto parse(format_parse_context &context) -> decltype(context.begin()) {
    return context.begin();
  }
  template <typename Context>
  auto format(struct ::stat const &value, Context &context) -> decltype(context.out()) {
    return format_to(context.out(),
                     "{{st_dev = {}, st_ino = {}, st_mode = 0{:o}, st_nlink = {}, st_uid = {}, "
                     "st_gid = {}, st_rdev = {}, st_size = {}, st_blksize = {}, st_blocks = {}, "
                     "st_atim = {}, st_mtim = {}, st_ctim = {}}}",
                     value.st_dev,
                     value.st_ino,
                     value.st_mode,
                     value.st_nlink,
                     value.st_uid,
                     value.st_gid,
                     value.st_rdev,
                     value.st_size,
                     value.st_blksize,
                     value.st_blocks,
                     value.st_atim,
                     value.st_mtim,
                     value.st_ctim);
  }
};

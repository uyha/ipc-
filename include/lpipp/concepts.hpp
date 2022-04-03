#pragma once
#include <type_traits>

namespace lpipp::concepts {
template <typename T, typename... Ts>
concept is_any_of = (std::is_same_v<T, Ts> || ...);

// clang-format off
template <typename T>
concept socket_option = std::is_same_v<std::remove_cv_t<decltype(T::level)>, int> &&
                       std::is_same_v<std::remove_cv_t<decltype(T::name)>, int> &&
                       std::is_same_v<std::remove_cv_t<decltype(T::read)>, bool> &&
                       std::is_same_v<std::remove_cv_t<decltype(T::write)>, bool> &&
                       requires (T option){
                        {option.value};
                        {option.size};
                       };
// clang-format on
template <typename T>
concept socket_read_option = socket_option<T> && T::read;
template <typename T>
concept socket_write_option = socket_option<T> && T::write;
} // namespace lpipp::concepts

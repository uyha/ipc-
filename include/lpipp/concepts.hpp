#pragma once
#include <type_traits>

namespace lpipp::concepts {
template <typename T, typename... Ts>
concept is_any_of = (std::is_same_v<T, Ts> || ...);
}

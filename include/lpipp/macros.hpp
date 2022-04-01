#pragma once

// bitor macros
#define LPIPP_BITOR_OVERRIDE(_1, _2, _3, NAME, ...) NAME

#define LPIPP_BITOR_OP_IMPL(left_type, right_type, return_type)                                                        \
  friend constexpr auto operator|(left_type lhs, right_type rhs) noexcept->return_type {                               \
    return static_cast<return_type>(static_cast<std::underlying_type_t<left_type>>(lhs)                                \
                                    | static_cast<std::underlying_type_t<right_type>>(rhs));                           \
  }

#define LPIPP_SAME_TYPE_BITOR_OP(input_type, return_type) LPIPP_BITOR_OP_IMPL(input_type, input_type, return_type)
#define LPIPP_DIFF_TYPE_BITOR_OP(left_type, right_type, return_type)                                                   \
  LPIPP_BITOR_OP_IMPL(left_type, right_type, return_type)                                                              \
                                                                                                                       \
  friend constexpr auto operator|(right_type lhs, left_type rhs) noexcept->return_type {                               \
    return rhs | lhs;                                                                                                  \
  }

#define LPIPP_BITOR_OP(...)                                                                                            \
  LPIPP_BITOR_OVERRIDE(__VA_ARGS__, LPIPP_DIFF_TYPE_BITOR_OP, LPIPP_SAME_TYPE_BITOR_OP, dummy)(__VA_ARGS__)

// bitand macro
#define LPIPP_BITAND_OVERRIDE(_1, _2, NAME, ...) NAME

#define LPIPP_BITAND_OP_IMPL(left_type, right_type)                                                                    \
  friend constexpr auto operator&(left_type lhs, right_type rhs) noexcept->bool {                                      \
    return static_cast<std::underlying_type_t<left_type>>(lhs) & static_cast<std::underlying_type_t<right_type>>(rhs); \
  }

#define LPIPP_SAME_TYPE_BITAND_OP(input_type) LPIPP_BITAND_OP_IMPL(input_type, input_type)
#define LPIPP_DIFF_TYPE_BITAND_OP(left_type, right_type)                                                               \
  LPIPP_BITAND_OP_IMPL(left_type, right_type)                                                                          \
                                                                                                                       \
  friend constexpr auto operator&(right_type lhs, left_type rhs) noexcept->bool {                                      \
    return rhs & lhs;                                                                                                  \
  }

#define LPIPP_BITAND_OP(...)                                                                                           \
  LPIPP_BITAND_OVERRIDE(__VA_ARGS__, LPIPP_DIFF_TYPE_BITAND_OP, LPIPP_SAME_TYPE_BITAND_OP, dummy)(__VA_ARGS__)

// std::error_code macros
#define LPIPP_IS_ERROR_CODE(error_enum)                                                                                \
  template <>                                                                                                          \
  struct std::is_error_code_enum<error_enum> : std::true_type {};

#define LPIPP_DECLARE_MAKE_ERROR_CODE(error_enum) auto make_error_code(error_enum error) noexcept->std::error_code

#define LPIPP_DEFINE_MAKE_ERROR_CODE(error_enum, error_category)                                                       \
  auto make_error_code(error_enum error) noexcept->std::error_code {                                                   \
    return {static_cast<int>(error), error_category};                                                                  \
  }

// socket options
#define LPIPP_SOCKOPT(opt, level_val, name_val, read_val, write_val, value_t)                                          \
  struct opt {                                                                                                         \
    using value_type = value_t;                                                                                        \
                                                                                                                       \
    static constexpr int level  = level_val;                                                                           \
    static constexpr int name   = name_val;                                                                            \
    static constexpr bool read  = read_val;                                                                            \
    static constexpr bool write = write_val;                                                                           \
                                                                                                                       \
    value_type value;                                                                                                  \
    ::socklen_t size = sizeof(value);                                                                                  \
  }

include(Conan)

find_package(fmt REQUIRED)

add_custom_target(lpipp-examples)

macro(lpipp_add_snippet name)
  add_executable(${name} ${ARGN})
  target_link_libraries(${name} PRIVATE lpipp::include fmt::fmt)
  target_compile_features(${name} PRIVATE cxx_std_20)
  add_dependencies(lpipp-examples ${name})
endmacro()

add_subdirectory(mq)

lpipp_add_snippet(lpipp-fmt fmt.cpp)


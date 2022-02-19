include(use_conan)

find_package(fmt REQUIRED)

add_executable(gensigrt gensigrt.cpp)
target_link_libraries(gensigrt PRIVATE fmt::fmt)
target_compile_features(gensigrt PRIVATE cxx_std_20)

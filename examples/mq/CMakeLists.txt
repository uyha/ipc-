find_package(fmt)

add_executable(mq-open EXCLUDE_FROM_ALL open.cpp)
target_link_libraries(mq-open PRIVATE lpipp::mq fmt::fmt)
target_compile_features(mq-open PRIVATE cxx_std_20)

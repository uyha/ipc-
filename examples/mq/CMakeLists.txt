find_package(fmt)

add_executable(mq-open open.cpp)
target_link_libraries(mq-open PRIVATE ipcpp::mq fmt::fmt)
target_compile_features(mq-open PRIVATE cxx_std_20)

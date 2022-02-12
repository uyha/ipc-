include(CTest)
include(use_conan)
include(build_options)

find_package(Catch2)
find_package(fmt)

macro(construct_test name)
    add_executable(${name} main.cpp ${ARGN})
    target_link_libraries(${name} PRIVATE Catch2::Catch2 fmt::fmt)
    target_compile_features(${name} PRIVATE cxx_std_20)
    add_test(
            NAME ${name}
            COMMAND ${name})
endmacro()

construct_test(message-queue message_queue.test.cpp)
target_link_libraries(message-queue PRIVATE ipcpp::mq)
target_compile_options(message-queue PRIVATE ${warnings})

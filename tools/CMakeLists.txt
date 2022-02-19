add_executable(gensigrt gensigrt.cpp)
add_custom_command(OUTPUT "${CMAKE_BINARY_DIR}/include/ipcpp/sig_rt.hpp"
  COMMAND "${CMAKE_COMMAND}" -E echo make_directory "${CMAKE_BINARY_DIR}/include/ipcpp"
  COMMAND gensigrt "${CMAKE_BINARY_DIR}/include/ipcpp/sig_rt.hpp"
  DEPENDS gensigrt gensigrt.cpp
)

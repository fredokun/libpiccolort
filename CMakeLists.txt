cmake_minimum_required(VERSION 2.6)

# C11
ENABLE_LANGUAGE(C)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11")

project(piccolort)

set(LIBRARY_OUTPUT_PATH lib/${CMAKE_BUILD_TYPE})

file(GLOB_RECURSE src_files src/*)
file(GLOB_RECURSE inc_files src/channel.h
                            src/commit.h
                            src/errors.h
                            src/pithread.h
                            src/primitives.h
                            src/queue.h
                            src/runtime.h
                            src/scheduler.h
                            src/tryaction.h
                            src/value.h)

add_library (piccolort STATIC ${src_files})

install(TARGETS piccolort DESTINATION lib)
install(FILES ${inc_files} DESTINATION include/piccolo)

# uninstall target
configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
  IMMEDIATE @ONLY
)
add_custom_target(uninstall
  COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake
)

cmake_minimum_required(VERSION 3.16)
project(bing2yi C)

set(CMAKE_C_STANDARD 99)
# set(CMAKE_EXE_LINKER_FLAGS -static)
add_executable(bing2yi main.c handlers.c vm_syscall.c)

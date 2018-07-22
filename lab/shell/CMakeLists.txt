cmake_minimum_required(VERSION 3.11)
include_directories(../lib)
ADD_LIBRARY(lab SHARED ../lib/lab.c)
add_executable(shell main.c shell.c signal.c command.c job.c)
target_link_libraries(shell lab)

add_executable(test_job test_job.c job.c)

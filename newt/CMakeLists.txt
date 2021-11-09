include(../cmake/newt.cmake)

file(GLOB_RECURSE srcs *.c *.cpp)

add_executable(newt-sample ${srcs})
target_compile_options(newt-sample PUBLIC -g -Wall)
target_link_libraries(newt-sample ${NEWT_LIBRARY})
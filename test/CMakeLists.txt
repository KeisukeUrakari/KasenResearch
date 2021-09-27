file(GLOB_RECURSE srcs ./*.cpp ../src/*.c ../src/*.cpp)
list(FILTER srcs EXCLUDE REGEX main.c)
list(FILTER srcs EXCLUDE REGEX main.cpp)

add_executable(${Project}-test ${srcs} ${srcs})
target_compile_options(${Project}-test PUBLIC -g -Wall)
target_link_libraries(${Project}-test ${libs} ${ext-libs} GTest::GTest GTest::Main)

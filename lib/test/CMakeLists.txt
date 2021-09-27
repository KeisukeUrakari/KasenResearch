file(GLOB_RECURSE srcs ./*.cpp ../src/*.c ../src/*.cpp)

add_executable(${libname}-test ${srcs})
target_compile_options(${libname}-test PUBLIC -g -Wall)
target_link_libraries(${libname}-test ${ext-libs} GTest::GTest GTest::Main)

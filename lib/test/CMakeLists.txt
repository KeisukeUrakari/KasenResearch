file(GLOB_RECURSE srcs ./*.cpp)

add_executable(${libname}-test ${srcs})
target_compile_options(${libname}-test PUBLIC -g -Wall)
target_link_libraries(${libname}-test  ${libname} PNG::PNG GTest::GTest GTest::Main Freetype::Freetype)

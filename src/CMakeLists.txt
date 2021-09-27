file(GLOB_RECURSE srcs ./*.c ./*.cpp)

add_executable(${Project} ${srcs})
target_compile_options(${Project} PUBLIC -g -Wall)
target_link_libraries(${Project} datediff ${libs} ${ext-libs})
file(GLOB_RECURSE srcs ./*.cpp)

include(../cmake/boost.cmake)

set(Target beast_async)

add_executable(${Target} ${srcs})
target_compile_options(${Target} PUBLIC -g -Wall)
target_link_libraries(${Target} ${Boost_LIBRARIES})

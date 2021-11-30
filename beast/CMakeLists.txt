file(GLOB_RECURSE srcs ./*.cpp)

include(../cmake/boost)

add_executable(beast ${srcs})
target_compile_options(beast PUBLIC -g -Wall)
target_link_libraries(beast ${Boost_LIBRARIES})
##target_link_libraries(<target> ${Boost_LIBRARIES})


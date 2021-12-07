file(GLOB_RECURSE srcs ./*.cpp)

include(../cmake/boost)

add_executable(multiio ${srcs})
target_compile_options(multiio PUBLIC -g -Wall)
target_link_libraries(multiio ${Boost_LIBRARIES})


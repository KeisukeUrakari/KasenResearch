set(libname kasen)

include_directories(${FREETYPE_INCLUDE_DIRS})

include(../cmake/boost.cmake)
include(../cmake/mosquitto.cmake)

file(GLOB_RECURSE srcs ./src/*.c ./src/*.cpp)

add_library(${libname} STATIC ${srcs})
target_compile_options(${libname} PUBLIC -g -Wall)
target_link_libraries(${libname} ${MOSQUITTO_LIBRARIES} ${Boost_LIBRARIES})
list(APPEND libs libname)

add_subdirectory(test)
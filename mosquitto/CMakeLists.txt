file(GLOB_RECURSE srcs ./*.c  ./*.cpp ../src/*.c ../src/*.cpp)
list(FILTER srcs EXCLUDE REGEX main.c)
list(FILTER srcs EXCLUDE REGEX main.cpp)

include(../cmake/mosquitto.cmake)

add_executable(publisher publisher.c)
target_compile_options(publisher PUBLIC -g -Wall)
target_link_libraries(publisher ${MOSQUITTO_LIBRARY})

add_executable(subscriber subscriber.c)
target_compile_options(subscriber PUBLIC -g -Wall)
target_link_libraries(subscriber ${MOSQUITTO_LIBRARY})
set(libname kasen)

include_directories(${FREETYPE_INCLUDE_DIRS})

file(GLOB_RECURSE srcs ./src/*.c ./src/*.cpp)

add_library(${libname} STATIC ${srcs})
target_link_libraries(${libname})
list(APPEND libs libname)

add_subdirectory(test)
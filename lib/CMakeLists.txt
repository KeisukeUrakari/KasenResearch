set(libname datediff)

file(GLOB_RECURSE srcs ./src/*.c ./src/*.cpp)

add_library(${libname} STATIC ${srcs})
target_link_libraries(${libname} ${ext-libs})
list(APPEND libs libname)

add_subdirectory(test)
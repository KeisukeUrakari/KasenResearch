file(GLOB_RECURSE srcs ./*.c ./*.cpp)

include_directories(${FREETYPE_INCLUDE_DIRS})
include_directories("../eggx-0.95")
link_directories("../eggx-0.95")

find_package(X11 REQUIRED)

add_executable(${Project} ${srcs})
target_compile_options(${Project} PUBLIC -g -Wall)
target_link_libraries(${Project} eggx m ${X11_LIBRARIES} Freetype::Freetype ${libs} ${ext-libs})
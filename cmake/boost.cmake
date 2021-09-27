# Boost

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)

find_package(Boost COMPONENTS system REQUIRED)

list(APPEND ext-libs pthread Boost::system)

#target_link_libraries(<target> pthread Boost::system)

# Boost

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)

find_package(Boost COMPONENTS system  date_time thread log log_setup REQUIRED)
ADD_DEFINITIONS(-DBOOST_LOG_DYN_LINK)

#target_link_libraries(<target> ${Boost_LIBRARIES})

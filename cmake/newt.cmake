find_path(NEWT_INCLUDE_DIR newt.h)
find_library(NEWT_LIBRARY NAMES newt)

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(NEWT DEFAULT_MSG NEWT_LIBRARY NEWT_INCLUDE_DIR)

#message(STATUS "libnewt include dir: ${NEWT_INCLUDE_DIR}")
#message(STATUS "libnewt: ${NEWT_LIBRARY}")

set(NEWT_LIBRARIES ${NEWT_LIBRARY})

mark_as_advanced(NEWT_INCLUDE_DIR NEWT_LIBRARY)
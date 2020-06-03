# Create a library called "libCbmBeamtime" which includes the source files given in
# the array .
# The extension is already found.  Any number of sources could be listed here.

add_subdirectory(roclight)
add_subdirectory(trb)

add_subdirectory(base)
add_subdirectory(data)
add_subdirectory(param)

add_subdirectory(trd)
add_subdirectory(rich)
add_subdirectory(tof)


add_subdirectory(cosy2014)
add_subdirectory(cern2014)

add_subdirectory(cern2016)

add_subdirectory(cosy2017)
add_subdirectory(cern2017)
add_subdirectory(star2017)

add_subdirectory(cosy2018)
add_subdirectory(star2018)


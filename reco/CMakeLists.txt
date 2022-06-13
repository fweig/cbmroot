# CMakeList file for libraries CbmRecoXXXXX
# P.-A. Loizeau, 7 February 2020

# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
# set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fsanitize=address")

add_subdirectory(base)
add_subdirectory(calibration)
add_subdirectory(detectors)
add_subdirectory(eventbuilder)
add_subdirectory(global)
add_subdirectory(KF)
add_subdirectory(L1)
add_subdirectory(littrack)
add_subdirectory(steer)
add_subdirectory(tracking)
add_subdirectory(qa)
add_subdirectory (tasks)
add_subdirectory (app)
add_subdirectory (mq)


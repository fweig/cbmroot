Set(FAIRMQ_LIBS FairMQStateMachine FairMQ)

# RPATH information should be present for executables in the build as well
# as in the installation directory

# use, i.e. don't skip the full RPATH for the build tree
set(CMAKE_SKIP_BUILD_RPATH FALSE)

# when building, don't use the install RPATH already
# (but later on when installing)
set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)

set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")

# add the automatically determined parts of the RPATH
# which point to directories outside the build tree to the install RPATH
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)



add_subdirectory(base)
add_subdirectory(source)
add_subdirectory(sink)
add_subdirectory(unpacker)

add_subdirectory(monitor)
add_subdirectory(histoServer)

add_subdirectory(mcbm)

#add_subdirectory(sts)
#add_subdirectory(test)

# The following subdirectories contain files which do not compile with
# FairRoot v18
# Don't compile it for the time being
# TODO: Fix the compilation problems
  add_subdirectory(parmq)
  add_subdirectory(hitbuilder)

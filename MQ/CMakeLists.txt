If(FairSoft_VERSION VERSION_LESS 18.6.0)
  Add_Definitions(-DHAVE_FAIRMQSTATEMACHINE)
EndIf()

add_subdirectory(base)
add_subdirectory(source)
add_subdirectory(sink)
#add_subdirectory(unpacker)

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
#  add_subdirectory(hitbuilder)

Message("Remove all ROOT files in macro directory")

file(GLOB_RECURSE _rootfiles 
     LIST_DIRECTORIES false
     ${CMAKE_BINARY_DIR}/macro/*.root
    )
foreach(file IN LISTS _rootfiles)
  file(REMOVE ${file})
endforeach()

Message("Remove all all*.par files in macro directory")

file(GLOB_RECURSE _parfiles 
     LIST_DIRECTORIES false
     ${CMAKE_BINARY_DIR}/macro/all*.par
    )

foreach(file IN LISTS _parfiles)
  file(REMOVE ${file})
endforeach()

Message("Remove all gphys.dat files in macro directory")

file(GLOB_RECURSE _datfiles 
     LIST_DIRECTORIES false
     ${CMAKE_BINARY_DIR}/macro/gphysi.dat
    )

foreach(file IN LISTS _datfiles)
  file(REMOVE ${file})
endforeach()

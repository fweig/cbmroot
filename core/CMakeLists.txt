# Needed inside the core directory
set(CBMDATA_DIR ${CMAKE_CURRENT_SOURCE_DIR}/data)
set(CBMBASE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/base)

add_subdirectory(data)
add_subdirectory(field)
add_subdirectory(base)
add_subdirectory(qa)

add_subdirectory(detectors/trd)
add_subdirectory(detectors/rich)
add_subdirectory(detectors/much)
add_subdirectory(detectors/tof)
add_subdirectory(detectors/sts)
add_subdirectory(detectors/psd)

Execute_Process(COMMAND ${ROOT_CONFIG_EXECUTABLE} --has-opengl
                OUTPUT_VARIABLE EveIsBuild
                OUTPUT_STRIP_TRAILING_WHITESPACE
               )

If(${EveIsBuild} STREQUAL yes)
  Message(STATUS "Build the eventdisplay.")
  add_subdirectory (eventdisplay)
Else()
  Message(STATUS "The eventdisplay is not build, there is no OpenGL support.")
EndIf()

set(CBMDATA_DIR ${CMAKE_CURRENT_SOURCE_DIR}/data PARENT_SCOPE)
set(CBMBASE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/base PARENT_SCOPE)
set(CBMFIELD_DIR ${CMAKE_CURRENT_SOURCE_DIR}/field PARENT_SCOPE)
set(CBMDETECTORBASE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/detectors PARENT_SCOPE)

If(GTEST_FOUND)
  add_subdirectory(data/test)
EndIf()

Install(FILES  ${CMAKE_CURRENT_SOURCE_DIR}/config/CbmConfigBase.h
        DESTINATION include
       )

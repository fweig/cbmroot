# Build the tsclient executable  which is taken from
# git@code.compeng.uni-frankfurt.de:flesnet.git
# This executable uses the fles_ipc library to read files
# in the tca format
# This file is only called if the compiler provides c++11 support

Set(INCLUDE_DIRECTORIES
  ${CMAKE_CURRENT_SOURCE_DIR}
)

Include_Directories( ${INCLUDE_DIRECTORIES})


Set(SRCS
evt.c
hadtu.c
hldread.c
showevt.c
subevt.c
)

Set_Source_Files_Properties(subevt.c COMPILE_FLAGS -Wno-implicit-function-declaration)

Set(EXE_NAME daq_anal)

GENERATE_EXECUTABLE()


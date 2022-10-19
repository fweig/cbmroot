# Copyright (C) 2022 Johann Wolfgang Goethe-Universitaet Frankfurt, Frankfurt am Main
# SPDX-License-Identifier: GPL-3.0-only
# Authors: Jan de Cuveland [committer]

set(SRCS
  Application.cxx
  ProgramOptions.cxx
  main.cxx
  )

set(HEADERS
  Application.h
  ProgramOptions.h
  ) 

add_executable(cbmreco_fairrun ${SRCS} ${HEADERS})
target_link_libraries(cbmreco_fairrun 
  PUBLIC 
    CbmRecoTasks 
  PRIVATE 
    Boost::program_options
    external::fles_monitoring
    external::fles_logging 
  )

install(TARGETS cbmreco_fairrun DESTINATION bin)

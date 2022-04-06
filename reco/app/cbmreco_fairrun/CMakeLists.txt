# Copyright (C) 2022 Johann Wolfgang Goethe-Universitaet Frankfurt, Frankfurt am Main
# SPDX-License-Identifier: GPL-3.0-only
# Authors: Jan de Cuveland [committer]

file(GLOB APP_SOURCES *.cxx)
file(GLOB APP_HEADERS *.h)

add_executable(cbmreco_fairrun ${APP_SOURCES} ${APP_HEADERS})

target_include_directories(cbmreco_fairrun SYSTEM PUBLIC ${Boost_INCLUDE_DIRS})

# This will no longer be necessary when the CbmRecoTasks library is set up to include this property
target_include_directories(cbmreco_fairrun
  PUBLIC ${CMAKE_SOURCE_DIR}/reco/tasks
)

# This will no longer be necessary when the Fairroot libraries are set up to include this property
target_link_directories(cbmreco_fairrun
  PUBLIC ${FAIRROOT_LIBRARY_DIR}
)

target_link_libraries(cbmreco_fairrun
  fles_logging
  CbmRecoTasks
  Core
  ${Boost_LIBRARIES}
)

install(TARGETS cbmreco_fairrun DESTINATION bin)

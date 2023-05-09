# CMakeList file for library libCbmRecoTasks
# V. Friese,     2 June 2021

set(INCLUDE_DIRECTORIES
  ${CMAKE_CURRENT_SOURCE_DIR}
  )

set(SRCS
  CbmReco.cxx
  CbmSourceTs.cxx
  CbmTaskBuildEvents.cxx
  CbmTaskDigiEventQa.cxx
  CbmTaskEventsCloneInToOut.cxx
  CbmTaskMakeRecoEvents.cxx
  CbmTaskTriggerDigi.cxx
  CbmTaskTofHitFinder.cxx
  CbmTaskUnpack.cxx
  CbmTaskUnpackXpu.cxx
)


if(OpenMP_CXX_FOUND)
  list(APPEND DEPENDENCIES OpenMP::OpenMP_CXX)
endif()


set(LIBRARY_NAME CbmRecoTasks)
set(LINKDEF ${LIBRARY_NAME}LinkDef.h)
set(PUBLIC_DEPENDENCIES
  CbmData
  CbmTofBase
  FairRoot::Base
  Algo
  ROOT::Core
  )

set(PRIVATE_DEPENDENCIES
  CbmBase
  CbmTrdBase
  FairLogger::FairLogger
  FairRoot::Online
  external::yaml-cpp
  ROOT::Hist
  ROOT::RHTTP
  )

set(INTERFACE_DEPENDENCIES
  external::fles_ipc
  )

# Check if the compiler supports std::execution in the respective STL
# library
CHECK_CXX_SOURCE_COMPILES("
#include <numeric>
#include <vector>
#include <execution>

int main(int argc, char *argv[])
{
    std::vector<double> v(10, 1);

    auto result = std::reduce(std::execution::par, v.begin(), v.end());
    return 0;
}" HAS_STD_EXECUTION)

if (HAS_STD_EXECUTION)
  message("Execution is available in STL")
  add_definitions(-DWITH_EXECUTION)
endif()

generate_cbm_library()

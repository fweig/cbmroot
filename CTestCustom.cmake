# -*- mode: cmake -*-

message(" -- Read CTestCustom.cmake --")

# Maximum size of uploaded test output of failed tests is 100kB
# Larger output is cutted
set(CTEST_CUSTOM_MAXIMUM_FAILED_TEST_OUTPUT_SIZE "102400")

# Maximum size of uploaded test output of passed tests is 1kB
# Larger output is cutted
set(CTEST_CUSTOM_MAXIMUM_PASSED_TEST_OUTPUT_SIZE "1024")

# -----------------------------------------------------------
# -- Number of warnings to display
# -----------------------------------------------------------

set(CTEST_CUSTOM_MAXIMUM_NUMBER_OF_WARNINGS "500" )

# -----------------------------------------------------------
# -- Number of errors to display
# -----------------------------------------------------------

set(CTEST_CUSTOM_MAXIMUM_NUMBER_OF_ERRORS   "50" )

# -----------------------------------------------------------
# -- Warning execptions
# -----------------------------------------------------------

Set(CTEST_CUSTOM_ERROR_EXCEPTION
    ${CTEST_CUSTOM_ERROR_EXCEPTION}
    "/include/boost"
    "/boost/include"
    "/include/root"
    "/root/include"
    "boost::"
)

set(CTEST_CUSTOM_WARNING_EXCEPTION
	${CTEST_CUSTOM_WARNING_EXCEPTION}

        "Dict.cxx"

        # -- warnings from our external packages
        "external/flib_dpb"
#        "external/flib_dpb_20"
        "external/ipc" 

        # -- remove intended fall through warnings
        "CbmMcbm2018UnpackerAlgoTof.cxx.*warning: this statement may fall through"
        "CbmMcbm2018UnpackerAlgoTof.cxx.*note: here"
        "CbmDeviceUnpackTofMcbm2018.cxx.*warning: this statement may fall through"
        "CbmDeviceUnpackTofMcbm2018.cxx.*note: here"
        "PairAnalysisHistos.cxx.*warning: this statement may fall through"
        "PairAnalysisHistos.cxx.*note: here"

        # -- warnings from structures for HADAQ memory casting
        "struct hadaq::HadTu"
        "struct hadaq::HadTuId"
        "TrbBridgeTrbNetHeaders.hpp"
        "TrbBridgeTrbNetHeaders.cpp"
        
        # -- warnings from structures for MBS/LMD memory casting
        "struct mbs::Header"

        # -- warnings from my test machine demac006
        "warning: text-based stub file"

        # -- don't show pragma message warnings
        "Compiling CBM Configuration"
  )

# -----------------------------------------------------------
# -- Warning addon's
# -----------------------------------------------------------
set(CTEST_CUSTOM_WARNING_MATCH	${CTEST_CUSTOM_WARNING_MATCH}
	)


Set (CTEST_CUSTOM_COVERAGE_EXCLUDE 
     ".*Dict.h"
     ".*Dict.cxx"
     ".*Fair.*"
     ".*GTest.*"
    )

Set(CTEST_CUSTOM_ERROR_EXCEPTION
    ${CTEST_CUSTOM_ERROR_EXCEPTION}
    # -- warnings from my test machine demac006
    "warning: text-based stub file"
   )

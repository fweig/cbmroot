# Create a library called "libCbmFlibReader" which includes the source files given in
# the array .
# The extension is already found.  Any number of sources could be listed here.

Set(INCLUDE_DIRECTORIES
  ${CBMDATA_DIR}
  ${CBMDATA_DIR}/base
  ${CBMDATA_DIR}/trd

  ${CBMBASE_DIR}

  ${CBMROOT_SOURCE_DIR}/beamtime/base
  ${CBMROOT_SOURCE_DIR}/beamtime/data/raw

  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/tools
  ${CMAKE_CURRENT_SOURCE_DIR}/lib

  ${CBMROOT_SOURCE_DIR}/beamtime/cern2014

)

Set(SYSTEM_INCLUDE_DIRECTORIES
  ${BASE_INCLUDE_DIRECTORIES}
  ${Boost_INCLUDE_DIR}
  ${ROOT_INCLUDE_DIR}
  )

Include_Directories(${INCLUDE_DIRECTORIES})
Include_Directories(SYSTEM ${SYSTEM_INCLUDE_DIRECTORIES})


Set(LINK_DIRECTORIES
  ${ROOT_LIBRARY_DIR}
  ${FAIRROOT_LIBRARY_DIR}
  ${Boost_LIBRARY_DIRS}
  ${Vc_LIB_DIR}
  ${KFParticle_LIB_DIR}
)

Link_Directories(
  ${LINK_DIRECTORIES}
)

Set(SRCS
  tools/CbmTrdTestBeamTools.cxx
  tools/CbmTrdTestBeamTools2017DESY.cxx
  tools/CbmTrdTestBeamTools2017GIF.cxx
  tools/CbmTrdTestBeamTools2018GIF.cxx
  tools/CbmTrdTestBeamToolsBuch2016.cxx
  tools/CbmTrdLabTools.cxx

  lib/CbmSpadicTriggerComp.cxx
  lib/CbmTrdQABase.cxx
  lib/CbmTrdQAHit.cxx
  lib/CbmTrdQACosmics.cxx
  lib/CbmTrdQAEfficiency.cxx
  lib/CbmTrdDESYAlignment.cxx
#  lib/CbmTrdQABaseline.cxx
  lib/CbmTrdSimpleDigitizer.cxx
#  lib/CbmTrdAdvDigitizer.cxx
  lib/CbmTrdDigiAnalysis.cxx
  lib/CbmTrdSimpleClusterizer.cxx
  lib/CbmTrdSimpleClusterAnalysis.cxx
  lib/CbmTrdClusterAnalysis.cxx
  lib/CbmTrdTestBeamAnalysis.cxx
#  lib/CbmTrdTimeCorrel.cxx
#  lib/CbmTrdTestBeamAnalysis2015SPS.cxx

)

Set(NO_DICT_SRCS
)

If(_UINT8_T_EXIST)
  Add_Definitions(-DHAVE_UINT8_T_HEADER_FILE)
  Set(DEFINITIONS HAVE_UINT8_T_HEADER_FILE)
EndIf()

Set(LINKDEF  CbmTrdBeamtimeLinkDef.h )
Set(LIBRARY_NAME CbmTrdBeamtime)
Set(DEPENDENCIES
  CbmBase CbmBeamtimeBase fles_ipc_legacy flib_dpb flib_dpb_20
  spadicMessage Base
)

GENERATE_LIBRARY()

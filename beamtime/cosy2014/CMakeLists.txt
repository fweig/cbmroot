# Create a library called "libCbmBeamtime" which includes the source files given in
# the array .
# The extension is already found.  Any number of sources could be listed here.

Set(INCLUDE_DIRECTORIES
  ${CBMBASE_DIR}

  ${CBMDATA_DIR}
  ${CBMDATA_DIR}/base
  ${CBMDATA_DIR}/mvd
  ${CBMDATA_DIR}/sts
  ${CBMDATA_DIR}/rich
  ${CBMDATA_DIR}/much
  ${CBMDATA_DIR}/trd
  ${CBMDATA_DIR}/tof
  ${CBMDATA_DIR}/psd

  ${CBMROOT_SOURCE_DIR}/beamtime/data
  ${CBMROOT_SOURCE_DIR}/beamtime/data/fhodo
  ${CBMROOT_SOURCE_DIR}/beamtime/base
  ${CBMROOT_SOURCE_DIR}/beamtime/roclight

  ${CBMDETECTORBASE_DIR}/sts

  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/fhodo
  ${CMAKE_CURRENT_SOURCE_DIR}/reco
  ${CMAKE_CURRENT_SOURCE_DIR}/unpacker
)


Set(SYSTEM_INCLUDE_DIRECTORIES
  ${BASE_INCLUDE_DIRECTORIES} 
  ${Boost_INCLUDE_DIR}
)

Include_Directories( 
  ${INCLUDE_DIRECTORIES}
)

Include_Directories( 
  SYSTEM ${SYSTEM_INCLUDE_DIRECTORIES}
)

Set(LINK_DIRECTORIES
  ${ROOT_LIBRARY_DIR}
  ${Boost_LIBRARY_DIRS}
  ${FAIRROOT_LIBRARY_DIR}
)
 
Link_Directories( 
  ${LINK_DIRECTORIES}
)

Set(SRCS
  CbmSourceLmd.cxx
  CbmSourceLmdNew.cxx
  CbmLmdDebug.cxx

  CbmDaqMap.cxx
  CbmDaqMapCosy2014.cxx
  CbmDaqMapCosy2014Much.cxx
  CbmDaqMapCosy2013.cxx
  
  unpacker/CbmROCUnpackAux.cxx
  unpacker/CbmROCUnpackDummy.cxx
  unpacker/CbmROCUnpackEpoch.cxx
  unpacker/CbmROCUnpackSystemMessage.cxx
  unpacker/CbmROCUnpackSts.cxx
  unpacker/CbmROCUnpackHodo.cxx
  unpacker/CbmROCUnpackMuch.cxx

  fhodo/CbmFiberHodoClusterFinder.cxx
  fhodo/CbmHodoRawHistos.cxx

  reco/CbmStsHodoCorrelations.cxx
  reco/CbmMuchRawHistos.cxx
  
  sts/reco/cosy/StsCosyBL.cxx
  sts/reco/cosy/StsCosyClusterFinder.cxx
  sts/reco/cosy/StsCosyHitFinder.cxx
  sts/reco/cosy/StsCosyAnalysis.cxx
  
  sts/settings/StsAlignment.cxx
  sts/settings/StsClusterSet.cxx
  sts/settings/StsHitSet.cxx
)

# Add artificial dependency which delays the compilation of the defined file
# until the libRoc.so is produced which is needed for the linking of the
# beamtime library. I couldn't find any better solution for the problem up
# to now.
Set_Source_Files_Properties(PROPERTIES CbmRocSource.cxx PROPERTIES
                            OBJECT_DEPENDS ${LIBRARY_OUTPUT_PATH}/libRoc.so
)

If(_UINT8_T_EXIST)
#  Add_Definitions(-DHAVE_UINT8_T_HEADER_FILE)
  Set(DEFINITIONS HAVE_UINT8_T_HEADER_FILE)
EndIf()

Set(LINKDEF  CbmBeamtimeCosy2014LinkDef.h)
Set(LIBRARY_NAME CbmBeamtimeCosy2014)
Set(DEPENDENCIES
    CbmBeamtimeBase
    CbmBeamtimeData
    CbmRocLight
    CbmStsBase 
#    CbmSts
    CbmData 
    CbmBase
    Base 
)

GENERATE_LIBRARY()

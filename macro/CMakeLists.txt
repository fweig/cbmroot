#--- Tests for continuous builds
add_subdirectory(run)
add_subdirectory(mcbm)
add_subdirectory(mvd)
add_subdirectory(much)
add_subdirectory(include)
if (${CMAKE_CXX_STANDARD} EQUAL 17)
  add_subdirectory (reco)
endif()

#--- Additional tests for nightly builds
If(NOT ${CBM_TEST_MODEL} MATCHES Experimental)
  add_subdirectory(field)
  add_subdirectory(C2F)
  add_subdirectory(dimuon)
  Message("Additional Nightly tests")
EndIf()

#--- Additional tests for weekly builds
If(${CBM_TEST_MODEL} MATCHES Weekly)
  add_subdirectory(KF)
  Message("Additional long running Weekly tests")
EndIf()

# Test if all libraries can be loaded
GENERATE_TEST_SCRIPT(${CBMROOT_SOURCE_DIR}/scripts/checklibs.sh ${CBMROOT_BINARY_DIR}/macro/)
add_test(load_libraries ${CBMROOT_BINARY_DIR}/macro/checklibs.sh)


# Install in any case
Install(PROGRAMS ${CBMROOT_SOURCE_DIR}/scripts/checklibs.sh
        DESTINATION share/cbmroot/macro/
       )
Install(FILES ${CBMROOT_SOURCE_DIR}/scripts/loadlib.C
        DESTINATION share/cbmroot/macro/
       )

Install(FILES KF/DecayConfig.C KF/registerGeantDecays.C KF/registerLightIons.C KF/registerPythiaDecays.C
              KF/kf_kfparticle.C KF/kf_thermal_signal_generator.C  KF/kf_transport.C
        DESTINATION share/cbmroot/macro/KF
       )
Install(CODE "FILE(MAKE_DIRECTORY ${CMAKE_INSTALL_PREFIX}/share/cbmroot/macro/KF/data)")

Install(FILES C2F/.rootrc C2F/c2f_transport.C C2F/sis100_electron.tree.list
              ${CBMROOT_SOURCE_DIR}/macro/analysis/common/analysis_tree_converter/run_analysis_tree_maker.C
              ${CBMROOT_SOURCE_DIR}/macro/analysis/common/at_kfpf_interface/run_at_kfpf.C
              ${CBMROOT_SOURCE_DIR}/macro/analysis/common/qa/run_analysistree_qa.C
              ${CBMROOT_SOURCE_DIR}/macro/analysis/common/cuts/cbm_cuts.h
        DESTINATION share/cbmroot/macro/c2f
       )
Install(CODE "FILE(MAKE_DIRECTORY ${CMAKE_INSTALL_PREFIX}/share/cbmroot/macro/c2f/data)")

Install(FILES dimuon/.rootrc dimuon/dimuon_transport.C dimuon/invM.C
              ${CBMROOT_SOURCE_DIR}/macro/much/run_ana.C
        DESTINATION share/cbmroot/macro/dimuon
       )
Install(CODE "FILE(MAKE_DIRECTORY ${CMAKE_INSTALL_PREFIX}/share/cbmroot/macro/dimuon/data)")

#--- Deprecated (not running)
#add_subdirectory(trd)
#add_subdirectory(littrack)
#add_subdirectory(beamtime)

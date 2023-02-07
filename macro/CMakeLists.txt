#--- Tests for continuous builds
add_subdirectory(run)
add_subdirectory(mcbm)
add_subdirectory(mvd)
add_subdirectory(much)
add_subdirectory(include)
add_subdirectory (reco)

#--- Additional tests for nightly builds
If(NOT ${CBM_TEST_MODEL} MATCHES Experimental)
  add_subdirectory(field)
  add_subdirectory(C2F)
  add_subdirectory(dimuon)
  add_subdirectory(PWG)
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
set_tests_properties(load_libraries PROPERTIES
                     TIMEOUT 900
                    )

# Install in any case
Install(PROGRAMS ${CBMROOT_SOURCE_DIR}/scripts/checklibs.sh
        DESTINATION share/cbmroot/macro/
       )
Install(FILES ${CBMROOT_SOURCE_DIR}/scripts/loadlib.C
        DESTINATION share/cbmroot/macro/
       )

Install(FILES ${CBMROOT_SOURCE_DIR}/macro/PWG/common/production/run_json.sh 
              ${CBMROOT_SOURCE_DIR}/macro/PWG/common/production/run_sim_reco_json.sh
              ${CBMROOT_SOURCE_DIR}/macro/PWG/common/production/run_transport_json_config.C
              ${CBMROOT_SOURCE_DIR}/macro/PWG/common/production/run_digi_json_config.C
              ${CBMROOT_SOURCE_DIR}/macro/PWG/common/production/run_reco_json_config.C
              ${CBMROOT_SOURCE_DIR}/macro/PWG/common/production/run_analysis_tree_maker_json_config.C
              ${CBMROOT_SOURCE_DIR}/macro/PWG/common/production/config.json
        DESTINATION share/cbmroot/macro/PWG/common/production
        )
Install(CODE "FILE(MAKE_DIRECTORY ${CMAKE_INSTALL_PREFIX}/share/cbmroot/macro/PWG/common/production)")

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

# Installation only
add_subdirectory(beamtime)

#--- Deprecated (not running)
#add_subdirectory(trd)
#add_subdirectory(littrack)

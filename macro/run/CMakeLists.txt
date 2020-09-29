# start of test CBM setups from geometry/setup
GENERATE_ROOT_TEST_SCRIPT(${CBMROOT_SOURCE_DIR}/macro/run/run_transport.C)
GENERATE_ROOT_TEST_SCRIPT(${CBMROOT_SOURCE_DIR}/macro/run/run_digi.C)
GENERATE_ROOT_TEST_SCRIPT(${CBMROOT_SOURCE_DIR}/macro/run/run_reco_event.C)
GENERATE_ROOT_TEST_SCRIPT(${CBMROOT_SOURCE_DIR}/macro/run/check_overlaps.C)
GENERATE_ROOT_TEST_SCRIPT(${CBMROOT_SOURCE_DIR}/macro/run/run_transport_qa.C)
GENERATE_ROOT_TEST_SCRIPT(${CBMROOT_SOURCE_DIR}/macro/run/run_qa.C)
GENERATE_ROOT_TEST_SCRIPT(${CBMROOT_SOURCE_DIR}/macro/run/run_reco_tb_track.C)
GENERATE_ROOT_TEST_SCRIPT(${CBMROOT_SOURCE_DIR}/macro/run/run_recoqa_tb_track.C)
GENERATE_ROOT_TEST_SCRIPT(${CBMROOT_SOURCE_DIR}/macro/run/run_reco_analysis.C)

Set(MACRO_DIR ${CMAKE_CURRENT_BINARY_DIR})
GENERATE_CBM_TEST_SCRIPT(${CBMROOT_SOURCE_DIR}/macro/geometry/check_media.C ${MACRO_DIR})

# Put the .rootrc file into the directory from which root is executed.
# Otherwise the rootalias file is not loaded
File(COPY ${CBMROOT_SOURCE_DIR}/macro/include/.rootrc DESTINATION ${CBMROOT_BINARY_DIR}/macro/run)

# Define Variables for Tests
if($ENV{ctest_model} MATCHES Weekly)
  Set(NumEvents 100)
  Set(uploadHistJPG 1)
  Set(pullDevAllowed .2)
Else()
  Set(NumEvents 3)
  Set(uploadHistJPG 0)
  Set(pullDevAllowed .5)
EndIf()

math(EXPR timeOutTime "${NumEvents} * 200")

# Define the different setups to be tested
If(($ENV{ctest_model} MATCHES Nightly) OR ($ENV{ctest_model} MATCHES Weekly))
  List(APPEND cbm_setup sis100_hadron sis100_electron sis100_muon_lmvm sis100_muon_jpsi sis300_electron)
Else()
  List(APPEND cbm_setup sis100_electron sis100_muon_jpsi)
EndIf()

ForEach(setup IN LISTS cbm_setup)

  # The test dependencies are set such that parallel execution is prevented, even is there is no logical
  # dependence. The reason is that all runs (for a given setup) use the same ROOT parameter file,
  # and concurrent access to this file results in erros.
  # The logical dependences are expressed in the required "ok" files.

  # --- Transport run, using run_transport.C
  Set(testname run_transport_${setup})
  Add_Test(${testname} ${CBMROOT_BINARY_DIR}/macro/run/run_transport.sh ${NumEvents} \"${setup}\" \"data/${setup}_test\")
  Set_Tests_Properties(${testname} PROPERTIES TIMEOUT ${timeOutTime})
  Set_Tests_Properties(${testname} PROPERTIES PASS_REGULAR_EXPRESSION "Test Passed;All ok")
  Set(fixture_digi fixture_digi_${testname})
  set_tests_properties(${testname} PROPERTIES FIXTURES_SETUP ${fixture_digi})
  
  # --- Digitisation run, event-by-event
  Set(testname run_digi_event_${setup})
  Add_Test(${testname} ${CBMROOT_BINARY_DIR}/macro/run/run_digi.sh ${NumEvents} \"data/${setup}_test\" 0. 0. 1)
  Set_Tests_Properties(${testname} PROPERTIES TIMEOUT ${timeOutTime}/2)
  Set_Tests_Properties(${testname} PROPERTIES PASS_REGULAR_EXPRESSION "Test Passed;All ok")
  set_tests_properties(${testname} PROPERTIES FIXTURES_REQUIRED ${fixture_digi})
  Set(fixture_reco_event fixture_reco_event_${testname})
  set_tests_properties(${testname} PROPERTIES FIXTURES_SETUP ${fixture_reco_event})

  # --- Reconstruction run, event-by-event
  Set(testname run_reco_event_${setup})
  Add_Test(${testname} ${CBMROOT_BINARY_DIR}/macro/run/run_reco_event.sh ${NumEvents} \"data/${setup}_test\" \"${setup}\")
  Set_Tests_Properties(${testname} PROPERTIES TIMEOUT ${timeOutTime})
  Set_Tests_Properties(${testname} PROPERTIES PASS_REGULAR_EXPRESSION "Test Passed;All ok")
  set_tests_properties(${testname} PROPERTIES FIXTURES_REQUIRED ${fixture_reco_event})
  Set(fixture_reco_event_mc fixture_reco_event_mc_${testname})
  set_tests_properties(${testname} PROPERTIES FIXTURES_SETUP ${fixture_reco_event_mc})
  
  # --- Reconstruction run, event-by-event, with MC input
  Set(testname run_reco_event_mc_${setup})
  Add_Test(${testname} ${CBMROOT_BINARY_DIR}/macro/run/run_reco_event.sh ${NumEvents} \"data/${setup}_test\" \"${setup}\" 1)
  Set_Tests_Properties(${testname} PROPERTIES TIMEOUT ${timeOutTime})
  Set_Tests_Properties(${testname} PROPERTIES PASS_REGULAR_EXPRESSION "Test Passed;All ok")
  set_tests_properties(${testname} PROPERTIES FIXTURES_REQUIRED ${fixture_reco_event_mc})
  Set(fixture_digi_tb fixture_digi_tb_${testname})
  set_tests_properties(${testname} PROPERTIES FIXTURES_SETUP ${fixture_digi_tb})

  # --- Quality Assurance
  Set(testname run_qa_event_mc_${setup})
  Add_Test(${testname} ${CBMROOT_BINARY_DIR}/macro/run/run_qa.sh ${NumEvents} \"data/${setup}_test\" \"${setup}\")
  Set_Tests_Properties(${testname} PROPERTIES TIMEOUT ${timeOutTime})
  Set_Tests_Properties(${testname} PROPERTIES PASS_REGULAR_EXPRESSION "Test Passed;All ok")
  Set_Tests_Properties(${testname} PROPERTIES FAIL_REGULAR_EXPRESSION "ERROR")
  Set_Tests_Properties(${testname} PROPERTIES FAIL_REGULAR_EXPRESSION "FATAL")
  Set_Tests_properties(${testname} PROPERTIES FIXTURES_REQUIRED ${fixture_reco_event_mc})

#  # --- Reconstruction Quality Assurance
#  Set(testname run_reco_event_qa_${setup})
#  Add_Test(${testname} ${CBMROOT_BINARY_DIR}/macro/run/run_reco_analysis.sh \"data/${setup}_test\" {{\"MvdCluster\",\"CbmMvdCluster\",\"MVD_Cluster_Finder\"},{\"MvdHit\",\"CbmMvdHit\",\"MVD_Hit_Finder\"},{\"StsCluster\",\"CbmStsCluster\",\"StsFindCluster\"},{\"StsHit\",\"CbmStsHit\",\"StsFindHit\"},{\"StsTrack\",\"CbmStsTrack\",\"StsFindTracksEvents\"}} {{\"sts\",{${pullDevAllowed},1,1,1}},{\"mvd\",{${pullDevAllowed},1,1,1}}} 2500 ${uploadHistJPG})
#  Set_Tests_Properties(${testname} PROPERTIES TIMEOUT ${timeOutTime})
#  Set_Tests_Properties(${testname} PROPERTIES PASS_REGULAR_EXPRESSION "Test Passed;All ok")
#  Set_Tests_Properties(${testname} PROPERTIES FAIL_REGULAR_EXPRESSION "Strong Deviation")
#  Set_Tests_properties(${testname} PROPERTIES FIXTURES_REQUIRED ${fixture_digi_tb})
#  set_tests_properties(${testname} PROPERTIES FIXTURES_SETUP ${fixture_digi_tb})
 
  # --- Digitisation run, time-based
  Set(testname run_digi_${setup})
  Add_Test(${testname} ${CBMROOT_BINARY_DIR}/macro/run/run_digi.sh ${NumEvents} \"data/${setup}_test\")
  Set_Tests_Properties(${testname} PROPERTIES TIMEOUT ${timeOutTime})
  Set_Tests_Properties(${testname} PROPERTIES PASS_REGULAR_EXPRESSION "Test Passed;All ok")
  set_tests_properties(${testname} PROPERTIES FIXTURES_REQUIRED ${fixture_digi_tb})
  Set(fixture_reco_tb fixture_reco_tb_${testname})
  set_tests_properties(${testname} PROPERTIES FIXTURES_SETUP ${fixture_reco_tb})
  
  # --- Reconstruction run, Time Based
  Set(testname run_reco_tb_${setup})
  Add_Test(${testname} ${CBMROOT_BINARY_DIR}/macro/run/run_reco_tb_track.sh \"data/${setup}_test\" -1 \"${setup}\")
  Set_Tests_Properties(${testname} PROPERTIES TIMEOUT ${timeOutTime})
  Set_Tests_Properties(${testname} PROPERTIES PASS_REGULAR_EXPRESSION "Test Passed;All ok")
  set_tests_properties(${testname} PROPERTIES FIXTURES_REQUIRED ${fixture_reco_tb})
  Set(fixture_reco_tb_qa fixture_reco_tb_qa_${testname})
  set_tests_properties(${testname} PROPERTIES FIXTURES_SETUP ${fixture_reco_tb_qa})

  # --- Reconstruction run, Time Based
  Set(testname run_reco_tb_qa_${setup})
  Add_Test(${testname} ${CBMROOT_BINARY_DIR}/macro/run/run_recoqa_tb_track.sh \"data/${setup}_test\" -1 \"${setup}\")
  Set_Tests_Properties(${testname} PROPERTIES TIMEOUT ${timeOutTime})
  Set_Tests_Properties(${testname} PROPERTIES PASS_REGULAR_EXPRESSION "Test Passed;All ok")
  set_tests_properties(${testname} PROPERTIES FIXTURES_REQUIRED ${fixture_reco_tb_qa})
  Set(fixture_reco_tb_ana fixture_reco_tb_ana_${testname})
  set_tests_properties(${testname} PROPERTIES FIXTURES_SETUP ${fixture_reco_tb_ana})

  # --- Reconstruction Quality Assurance
  Set(testname run_reco_tb_ana_${setup})
  Add_Test(${testname} ${CBMROOT_BINARY_DIR}/macro/run/run_reco_analysis.sh \"data/${setup}_test\" {{\"StsHit\",\"CbmStsHit\",\"RecoSts\"},{\"StsTrack\",\"CbmStsTrack\",\"STSFindTracks\"}} {{\"sts\",{${pullDevAllowed},1,1,1}}} 2500 ${uploadHistJPG})
  Set_Tests_Properties(${testname} PROPERTIES TIMEOUT ${timeOutTime})
  Set_Tests_Properties(${testname} PROPERTIES PASS_REGULAR_EXPRESSION "Test Passed;All ok")
  Set_Tests_Properties(${testname} PROPERTIES FAIL_REGULAR_EXPRESSION "Strong Deviation")
  Set_Tests_properties(${testname} PROPERTIES FIXTURES_REQUIRED ${fixture_reco_tb_ana})
  set(dep_qa_transport ${testname}) 

  # --- QA run
#  Set(testname run_transport_qa_${setup})
#  Add_Test(${testname} ${CBMROOT_BINARY_DIR}/macro/run/run_transport_qa.sh \"${setup}\" \"data/${setup}_test\")
#  Set_Tests_Properties(${testname} PROPERTIES TIMEOUT "20")
#  Set_Tests_Properties(${testname} PROPERTIES PASS_REGULAR_EXPRESSION "Test Passed;All ok")
#  Set_Tests_Properties(${testname} PROPERTIES FIXTURES_REQUIRED ${fixture_digi})
#  Set_Tests_Properties(${testname} PROPERTIES DEPENDS ${dep_qa_transport})
#  set(dep_overlaps ${testname}) 



  # --- Geometry checking; only for nightly test
  If( ($ENV{ctest_model} MATCHES Nightly) OR ($ENV{ctest_model} MATCHES Weekly) )
    Set(testname run_check_media_${setup})
    Add_Test(${testname} ${CBMROOT_BINARY_DIR}/macro/run/check_media.sh \"data/${setup}_test\")
    Set_Tests_Properties(${testname} PROPERTIES TIMEOUT ${timeOutTime})
    Set_Tests_Properties(${testname} PROPERTIES PASS_REGULAR_EXPRESSION "Test Passed;All ok")
    Set_Tests_properties(${testname} PROPERTIES FIXTURES_REQUIRED ${fixture_digi})

  	Set(testname run_overlap_${setup})
  	Add_Test(${testname} ${CBMROOT_BINARY_DIR}/macro/run/check_overlaps.sh \"data/${setup}_test\")
  	Set_Tests_Properties(${testname} PROPERTIES TIMEOUT "900")
  	Set_Tests_Properties(${testname} PROPERTIES PASS_REGULAR_EXPRESSION "Test Passed;All ok")
        Set_Tests_Properties(${testname} PROPERTIES FIXTURES_REQUIRED ${fixture_digi})
#        Set_Tests_Properties(${testname} PROPERTIES DEPENDS ${dep_overlaps})
        Set_Tests_Properties(${testname} PROPERTIES DEPENDS ${dep_qa_transport})
  EndIf()
 
EndForEach(setup IN LISTS cbm_setup)

# I will have to check these macros for being up-to-date or necessary
#GENERATE_ROOT_TEST_SCRIPT(${CBMROOT_SOURCE_DIR}/macro/run/run.C)
#add_test(run_run ${CBMROOT_BINARY_DIR}/macro/run/run.sh)
#SET_TESTS_PROPERTIES(run_run PROPERTIES TIMEOUT "300")
#SET_TESTS_PROPERTIES(run_run PROPERTIES PASS_REGULAR_EXPRESSION "Test Passed;All ok")

#GENERATE_ROOT_TEST_SCRIPT(${CBMROOT_SOURCE_DIR}/macro/run/checkFields.C)
#add_test(run_checkFields ${CBMROOT_BINARY_DIR}/macro/run/checkFields.sh)
#SET_TESTS_PROPERTIES(run_checkFields PROPERTIES TIMEOUT "60")

#GENERATE_ROOT_TEST_SCRIPT(${CBMROOT_SOURCE_DIR}/macro/run/checkFieldMap.C)
#add_test(run_checkFieldMap ${CBMROOT_BINARY_DIR}/macro/run/checkFieldMap.sh)
#SET_TESTS_PROPERTIES(run_checkFieldMap PROPERTIES TIMEOUT "60")

#GENERATE_ROOT_TEST_SCRIPT(${CBMROOT_SOURCE_DIR}/macro/run/checkFieldSym.C)
#add_test(run_checkFieldSym ${CBMROOT_BINARY_DIR}/macro/run/checkFieldSym.sh)
#SET_TESTS_PROPERTIES(run_checkFieldSym PROPERTIES TIMEOUT "60")


Install(FILES .rootrc run_transport.C run_digi.C run_reco_event.C check_overlaps.C 
        DESTINATION share/cbmroot/macro/run
       )
Install(DIRECTORY modules DESTINATION share/cbmroot/macro/run)
Install(CODE "FILE(MAKE_DIRECTORY \${CMAKE_INSTALL_PREFIX}/share/cbmroot/macro/run/data)")

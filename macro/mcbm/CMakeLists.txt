# start of test CBM setups from geometry/setup
GENERATE_ROOT_TEST_SCRIPT(${CBMROOT_SOURCE_DIR}/macro/mcbm/mcbm_transport.C)
GENERATE_ROOT_TEST_SCRIPT(${CBMROOT_SOURCE_DIR}/macro/mcbm/mcbm_digi.C)
GENERATE_ROOT_TEST_SCRIPT(${CBMROOT_SOURCE_DIR}/macro/mcbm/mcbm_reco_event.C)
GENERATE_ROOT_TEST_SCRIPT(${CBMROOT_SOURCE_DIR}/macro/mcbm/mcbm_match_check.C)
GENERATE_ROOT_TEST_SCRIPT(${CBMROOT_SOURCE_DIR}/macro/mcbm/mcbm_hadron_analysis.C)
GENERATE_ROOT_TEST_SCRIPT(${CBMROOT_SOURCE_DIR}/macro/mcbm/mcbm_check_overlaps.C)
GENERATE_ROOT_TEST_SCRIPT(${CBMROOT_SOURCE_DIR}/macro/mcbm/mcbm_qa.C)
#GENERATE_ROOT_TEST_SCRIPT(${CBMROOT_SOURCE_DIR}/macro/mcbm/mcbm_check_materials.C)

Set(MACRO_DIR ${CMAKE_CURRENT_BINARY_DIR})
GENERATE_CBM_TEST_SCRIPT(${CBMROOT_SOURCE_DIR}/macro/geometry/check_media.C ${MACRO_DIR})

# disable dev tests - GENERATE_ROOT_TEST_SCRIPT(${CBMROOT_SOURCE_DIR}/macro/mcbm/mcbm_mc_dev.C)
# disable dev tests - GENERATE_ROOT_TEST_SCRIPT(${CBMROOT_SOURCE_DIR}/macro/mcbm/mcbm_reco_dev.C)

# Put the .rootrc file into the directory from which root is executed.
# Otherwise the rootalias file is not loaded
File(COPY ${CBMROOT_SOURCE_DIR}/macro/include/.rootrc DESTINATION ${CBMROOT_BINARY_DIR}/macro/mcbm)

# Define the number of events to run
Set(NumEvents 10)
Set(BeamAngle 25)

# Define the different setups to be tested
If( ($ENV{ctest_model} MATCHES Nightly) OR ($ENV{ctest_model} MATCHES Weekly) )
  List(APPEND cbm_setup mcbm_beam_2020_03 mcbm_beam_2021_03)
Else()
  List(APPEND cbm_setup mcbm_beam_2020_03 mcbm_beam_2021_03)
EndIf()

ForEach(setup IN LISTS cbm_setup)

  Set(testname mcbm_transport_${setup})
  Add_Test(${testname} ${CBMROOT_BINARY_DIR}/macro/mcbm/mcbm_transport.sh ${NumEvents} \"${setup}\" \"data/${setup}_test\")
  Set_Tests_Properties(${testname} PROPERTIES TIMEOUT "600")
  Set_Tests_Properties(${testname} PROPERTIES PASS_REGULAR_EXPRESSION "Test Passed;All ok")
  Set(fixture_transport fixture_transport_${setup})
  set_tests_properties(${testname} PROPERTIES FIXTURES_SETUP ${fixture_transport})


  Set(testname mcbm_digi_event_${setup})
  Add_Test(${testname} ${CBMROOT_BINARY_DIR}/macro/mcbm/mcbm_digi.sh ${NumEvents} \"data/${setup}_test\" 0. 0. 1)
  Set_Tests_Properties(${testname} PROPERTIES TIMEOUT "300")
  Set_Tests_Properties(${testname} PROPERTIES PASS_REGULAR_EXPRESSION "Test Passed;All ok")
  set_tests_properties(${testname} PROPERTIES FIXTURES_REQUIRED ${fixture_transport})
  Set(fixture_digi_event fixture_digi_event_${setup})
  set_tests_properties(${testname} PROPERTIES FIXTURES_SETUP ${fixture_digi_event})

  Set(testname mcbm_reco_event_${setup})
  Add_Test(${testname} ${CBMROOT_BINARY_DIR}/macro/mcbm/mcbm_reco_event.sh ${NumEvents} \"data/${setup}_test\" \"${setup}\")
  Set_Tests_Properties(${testname} PROPERTIES TIMEOUT "300")
  Set_Tests_Properties(${testname} PROPERTIES PASS_REGULAR_EXPRESSION "Test Passed;All ok")
  set_tests_properties(${testname} PROPERTIES FIXTURES_REQUIRED ${fixture_digi_event})
  Set(fixture_reco_event fixture_reco_event_${setup})
  set_tests_properties(${testname} PROPERTIES FIXTURES_SETUP ${fixture_reco_event})

  Set(testname mcbm_match_check_${setup})
  Add_Test(${testname} ${CBMROOT_BINARY_DIR}/macro/mcbm/mcbm_match_check.sh ${NumEvents} \"${setup}\" \"data/${setup}_test\")
  Set_Tests_Properties(${testname} PROPERTIES TIMEOUT "300")
  Set_Tests_Properties(${testname} PROPERTIES PASS_REGULAR_EXPRESSION "Test Passed;All ok")
  set_tests_properties(${testname} PROPERTIES FIXTURES_REQUIRED ${fixture_reco_event})
  Set(fixture_match_check fixture_match_check_${setup})
  set_tests_properties(${testname} PROPERTIES FIXTURES_SETUP ${fixture_match_check})

  Set(testname mcbm_hadron_analysis_${setup})
  Add_Test(${testname} ${CBMROOT_BINARY_DIR}/macro/mcbm/mcbm_hadron_analysis.sh ${NumEvents} \"${setup}\" \"data/${setup}_test\")
  Set_Tests_Properties(${testname} PROPERTIES TIMEOUT "300")
  Set_Tests_Properties(${testname} PROPERTIES PASS_REGULAR_EXPRESSION "Test Passed;All ok")
  set_tests_properties(${testname} PROPERTIES FIXTURES_REQUIRED ${fixture_match_check})
  Set(fixture_hadron_analysis fixture_hadron_analysis_${setup})
  set_tests_properties(${testname} PROPERTIES FIXTURES_SETUP ${fixture_hadron_analysis})

  Set(testname mcbm_digi_${setup})
  Add_Test(${testname} ${CBMROOT_BINARY_DIR}/macro/mcbm/mcbm_digi.sh ${NumEvents} \"data/${setup}_test\")
  Set_Tests_Properties(${testname} PROPERTIES TIMEOUT "300")
  Set_Tests_Properties(${testname} PROPERTIES PASS_REGULAR_EXPRESSION "Test Passed;All ok")
  set_tests_properties(${testname} PROPERTIES FIXTURES_REQUIRED ${fixture_hadron_analysis})
  Set(fixture_digi_tb fixture_digi_tb_${setup})
  set_tests_properties(${testname} PROPERTIES FIXTURES_SETUP ${fixture_digi_tb})

  Set(testname mcbm_qa_event_mc_${setup})
  Add_Test(${testname} ${CBMROOT_BINARY_DIR}/macro/mcbm/mcbm_qa.sh ${NumEvents} \"data/${setup}_test\" \"${setup}\")
  Set_Tests_Properties(${testname} PROPERTIES TIMEOUT "300")
  Set_Tests_Properties(${testname} PROPERTIES PASS_REGULAR_EXPRESSION "Test Passed;All ok")
  Set_Tests_Properties(${testname} PROPERTIES FAIL_REGULAR_EXPRESSION "ERROR")
  Set_Tests_Properties(${testname} PROPERTIES FAIL_REGULAR_EXPRESSION "FATAL")
  Set_Tests_properties(${testname} PROPERTIES FIXTURES_REQUIRED ${fixture_digi_tb})
  Set(fixture_qa_event_mc fixture_qa_event_mc_${setup})
  set_tests_properties(${testname} PROPERTIES FIXTURES_SETUP ${fixture_qa_event_mc})

  #Set(testname mcbm_reco_${setup})
  #Add_Test(${testname} ${CBMROOT_BINARY_DIR}/macro/mcbm/mcbm_reco.sh ${NumEvents} ${BeamAngle} \"${setup}\")
  #Set_Tests_Properties(${testname} PROPERTIES TIMEOUT "600")
  #Set_Tests_Properties(${testname} PROPERTIES PASS_REGULAR_EXPRESSION "Test Passed;All ok")
  #Set_Tests_Properties(${testname} PROPERTIES DEPENDS ${dep_mc})
  #Set(_FileName_Dependency data/mcbm_mc_${setup}_ok)
  #Set_Tests_Properties(${testname} PROPERTIES REQUIRED_FILES ${_FileName_Dependency})

  Set(testname mcbm_overlap_${setup})
  Add_Test(${testname} ${CBMROOT_BINARY_DIR}/macro/mcbm/mcbm_check_overlaps.sh \"data/${setup}_test\")
  Set_Tests_Properties(${testname} PROPERTIES TIMEOUT "600")
  Set_Tests_Properties(${testname} PROPERTIES PASS_REGULAR_EXPRESSION "Test Passed;All ok")
  set_tests_properties(${testname} PROPERTIES FIXTURES_REQUIRED ${fixture_qa_event_mc})
  Set(fixture_overlap fixture_overlap_${setup})
  set_tests_properties(${testname} PROPERTIES FIXTURES_SETUP ${fixture_overlap})

  If( ($ENV{ctest_model} MATCHES Nightly) OR ($ENV{ctest_model} MATCHES Weekly) )
    Set(testname mcbm_check_media_${setup})
    Add_Test(${testname} ${CBMROOT_BINARY_DIR}/macro/mcbm/check_media.sh \"data/${setup}_test\")
    Set_Tests_Properties(${testname} PROPERTIES TIMEOUT "60")
    Set_Tests_Properties(${testname} PROPERTIES PASS_REGULAR_EXPRESSION "Test Passed;All ok")
    set_tests_properties(${testname} PROPERTIES FIXTURES_REQUIRED ${fixture_overlap})
  EndIf()

EndForEach(setup IN LISTS cbm_setup)


# disable dev tests - # Define the different configs to be tested
# disable dev tests - # test all subsystems
# disable dev tests - #List(APPEND cbm_config 801101110 810000000 801000000 800100000 800010000 800001000 800000100 800000010 800000001)
# disable dev tests - # test selected subsystems
# disable dev tests - List(APPEND cbm_config 801101110 801000000 800100000 800010000 800001000 800000100 800000010 800000001)
# disable dev tests -
# disable dev tests - ForEach(config IN LISTS cbm_config)
# disable dev tests -
# disable dev tests -   Set(testname mcbm_mc_dev_${config})
# disable dev tests -   Add_Test(${testname} ${CBMROOT_BINARY_DIR}/macro/mcbm/mcbm_mc_dev.sh ${NumEvents} ${BeamAngle} \"sis18_mcbm_25deg_long_dev\" \"${config}\")
# disable dev tests -   Set_Tests_Properties(${testname} PROPERTIES TIMEOUT "30")
# disable dev tests -   Set_Tests_Properties(${testname} PROPERTIES PASS_REGULAR_EXPRESSION "Test Passed;All ok")
# disable dev tests -
# disable dev tests -   Set(dep_mc ${testname})
# disable dev tests -
# disable dev tests -   Set(testname mcbm_reco_dev_${config})
# disable dev tests -   Add_Test(${testname} ${CBMROOT_BINARY_DIR}/macro/mcbm/mcbm_reco_dev.sh ${NumEvents} \"sis18_mcbm_25deg_long_dev\" \"${config}\")
# disable dev tests -   Set_Tests_Properties(${testname} PROPERTIES TIMEOUT "30")
# disable dev tests -   Set_Tests_Properties(${testname} PROPERTIES PASS_REGULAR_EXPRESSION "Test Passed;All ok")
# disable dev tests -   Set_Tests_Properties(${testname} PROPERTIES DEPENDS ${dep_mc})
# disable dev tests -   Set(_FileName_Dependency data/mcbm_mc_dev_${config}_ok)
# disable dev tests -   Set_Tests_Properties(${testname} PROPERTIES REQUIRED_FILES ${_FileName_Dependency})
# disable dev tests -
# disable dev tests -   Set(dep_reco ${testname})
# disable dev tests -
# disable dev tests - #DE  Set(testname mcbm_overlap_${config})
# disable dev tests - #DE  Add_Test(${testname} ${CBMROOT_BINARY_DIR}/macro/mcbm/mcbm_check_overlaps.sh \"sis18_mcbm_25deg_long_dev\")
# disable dev tests - #DE  Set_Tests_Properties(${testname} PROPERTIES TIMEOUT "600")
# disable dev tests - #DE  Set_Tests_Properties(${testname} PROPERTIES PASS_REGULAR_EXPRESSION "Test Passed;All ok")
# disable dev tests - #DE  Set_Tests_Properties(${testname} PROPERTIES DEPENDS ${dep_mc})
# disable dev tests - #DE  Set(_FileName_Dependency data/mcbm_mc_${config}_ok)
# disable dev tests - #DE  Set_Tests_Properties(${testname} PROPERTIES REQUIRED_FILES ${_FileName_Dependency})
# disable dev tests -
# disable dev tests - EndForEach(config IN LISTS cbm_config)
# end of test CBM configs from geometry/config

# disable dev tests - Install(FILES .rootrc mcbm_transport.C mcbm_reco.C mcbm_check_overlaps.C
Install(FILES .rootrc mcbm_transport.C mcbm_digi.C mcbm_reco_event.C mcbm_check_overlaps.C
              mcbm_match_check.C mcbm_hadron_analysis.C mcbm_qa.C
        DESTINATION share/cbmroot/macro/mcbm
       )
Install(DIRECTORY modules DESTINATION share/cbmroot/macro/mcbm)
Install(CODE "FILE(MAKE_DIRECTORY \${CMAKE_INSTALL_PREFIX}/share/cbmroot/macro/mcbm/data)")

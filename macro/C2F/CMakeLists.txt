# start of test CBM setups from geometry/setup
Set(MACRO_DIR ${CBMROOT_BINARY_DIR}/macro/C2F)
GENERATE_ROOT_TEST_SCRIPT(${CBMROOT_SOURCE_DIR}/macro/C2F/c2f_transport.C)
GENERATE_CBM_TEST_SCRIPT(${CBMROOT_SOURCE_DIR}/macro/run/run_digi.C ${MACRO_DIR})
GENERATE_CBM_TEST_SCRIPT(${CBMROOT_SOURCE_DIR}/macro/run/run_reco_event.C ${MACRO_DIR})
GENERATE_CBM_TEST_SCRIPT(${CBMROOT_SOURCE_DIR}/macro/analysis/common/analysis_tree_converter/run_analysis_tree_maker.C ${MACRO_DIR})
GENERATE_CBM_TEST_SCRIPT(${CBMROOT_SOURCE_DIR}/macro/analysis/common/at_kfpf_interface/run_at_kfpf.C ${MACRO_DIR})
GENERATE_CBM_TEST_SCRIPT(${CBMROOT_SOURCE_DIR}/macro/analysis/common/qa/run_analysistree_qa.C ${MACRO_DIR})

# Put the .rootrc file into the directory from which root is executed.
# Otherwise the rootalias file is not loaded
File(COPY ${CBMROOT_SOURCE_DIR}/macro/include/.rootrc DESTINATION ${MACRO_DIR})
File(COPY ${CBMROOT_SOURCE_DIR}/macro/C2F/sis100_electron.tree.list DESTINATION ${MACRO_DIR})

# Define the number of events to run
if(LARGE_TEST_STATISTIC)
  Set(NumEvents 100)
else()
  Set(NumEvents 20)
endif()

# Define the different setups to be tested
#If( ${CBM_TEST_MODEL} MATCHES Nightly OR ${CBM_TEST_MODEL} MATCHES Weekly )
  List(APPEND cbm_setup sis100_electron)
#EndIf()

  ForEach(setup IN LISTS cbm_setup)

    # The test dependencies are set such that parallel execution is prevented, even is there is no logical
    # dependence. The reason is that all runs (for a given setup) use the same ROOT parameter file,
    # and concurrent access to this file results in erros.
    # The logical dependences are expressed in the required "ok" files.

    # --- Transport run, using run_transport.C
    Set(testname c2f_transport_${setup})
    Add_Test(${testname} ${MACRO_DIR}/c2f_transport.sh ${NumEvents} \"${setup}\" \"data/${setup}_test\" \"\" \"TGeant4\" false)
    Set_Tests_Properties(${testname} PROPERTIES 
        TIMEOUT "900"
        PASS_REGULAR_EXPRESSION "Test Passed;All ok"
        FIXTURES_SETUP fixture_c2f_digi_${setup}
    )
  
    # --- Digitisation run, event-by-event
    Set(testname c2f_digi_event_${setup})
    Add_Test(${testname} ${MACRO_DIR}/run_digi.sh \"data/${setup}_test\" ${NumEvents} 
    	\"data/${setup}_test.event\" -1.)
    Set_Tests_Properties(${testname} PROPERTIES 
        TIMEOUT "300"
        PASS_REGULAR_EXPRESSION "Macro finished successfully"
        FIXTURES_REQUIRED fixture_c2f_digi_${setup}
        FIXTURES_SETUP fixture_c2f_reco_event_${setup}
    )

    # --- Reconstruction run, event-by-event
    Set(testname c2f_reco_event_${setup})
    Add_Test(${testname} ${MACRO_DIR}/run_reco_event.sh ${NumEvents} \"data/${setup}_test\" \"${setup}\")
    Set_Tests_Properties(${testname} PROPERTIES 
        TIMEOUT "1300"
        PASS_REGULAR_EXPRESSION "Test Passed;All ok"
        FIXTURES_REQUIRED fixture_c2f_reco_event_${setup}
        FIXTURES_SETUP fixture_c2f_ana_${setup}
    )
  
    # --- AnalysisTree
    Set(testname analysis_tree_maker_${setup})
    Add_Test(${testname} ${MACRO_DIR}/run_analysis_tree_maker.sh \"data/${setup}_test\" \"${setup}\")
    Set_Tests_Properties(${testname} PROPERTIES 
        TIMEOUT "300"
        PASS_REGULAR_EXPRESSION "Test Passed;All ok"
        FIXTURES_REQUIRED fixture_c2f_ana_${setup}
        FIXTURES_SETUP fixture_c2f_analysistree_${setup}
    )

    # --- AnalysisTree to KFPF interface
    Set(testname at_kfpf_${setup})
    Add_Test(${testname} ${MACRO_DIR}/run_at_kfpf.sh ${NumEvents} \"data/${setup}_test\")
    Set_Tests_Properties(${testname} PROPERTIES 
        TIMEOUT "300"
        PASS_REGULAR_EXPRESSION "Test Passed;All ok"
        FIXTURES_REQUIRED fixture_c2f_analysistree_${setup}
        FIXTURES_SETUP fixture_c2f_at_kfpf_interface_${setup}
    )

    # --- AnalysisTreeQA
    Set(testname analysistree_qa_${setup})
    Add_Test(${testname} ${MACRO_DIR}/run_analysistree_qa.sh \"data/${setup}_test.analysistree.root\" true)
    Set_Tests_Properties(${testname} PROPERTIES 
        TIMEOUT "300"
        PASS_REGULAR_EXPRESSION "Test Passed;All ok"
        FIXTURES_REQUIRED fixture_c2f_at_kfpf_interface_${setup}
#        FIXTURES_SETUP fixture_c2f_at_qa_${setup}
    )


  EndForEach(setup IN LISTS cbm_setup)
  # end of test CBM setups from geometry/setup

# Installation of macros is done in macro/CMakeLists.txt to install the
# macros even if the tests are not executed

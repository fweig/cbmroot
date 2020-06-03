#TODO: Test the different tof geometries

# Put the .rootrc file into the directory from which root is executed.
# Otherwise the rootalias file is not loaded
File(COPY ${CBMROOT_SOURCE_DIR}/macro/include/.rootrc DESTINATION ${CBMROOT_BINARY_DIR}/macro/tof/digitizer)

# Define the number of events to run
Set(NumEvents 2)

# Define the setup to run
Set(setup sis100_electron)

GENERATE_ROOT_TEST_SCRIPT(${CBMROOT_SOURCE_DIR}/macro/tof/digitizer/run_tof_disim.C)
add_test(tof_disim ${CBMROOT_BINARY_DIR}/macro/tof/digitizer/run_tof_disim.sh ${NumEvents} \"${setup}\")
SET_TESTS_PROPERTIES(tof_disim PROPERTIES TIMEOUT "600")
SET_TESTS_PROPERTIES(tof_disim PROPERTIES PASS_REGULAR_EXPRESSION "Test Passed;All ok")
set_tests_properties(tof_disim PROPERTIES FIXTURES_SETUP fixture_tof_digi1)

# This text macro does not run any more properly. It has either to be removed or updated.
#GENERATE_ROOT_TEST_SCRIPT(${CBMROOT_SOURCE_DIR}/macro/tof/digitizer/run_digi_test.C)
#add_test(tof_digi1 ${CBMROOT_BINARY_DIR}/macro/tof/digitizer/run_digi_test.sh ${NumEvents} \"${setup}\")
#SET_TESTS_PROPERTIES(tof_digi1 PROPERTIES TIMEOUT "300")
#SET_TESTS_PROPERTIES(tof_digi1 PROPERTIES PASS_REGULAR_EXPRESSION "Test Passed;All ok")
#set_tests_properties(tof_digi1 PROPERTIES FIXTURES_REQUIRED fixture_tof_digi1)


### To do later: run with different digitizer/clusterizer options
#GENERATE_ROOT_TEST_SCRIPT(${CBMROOT_SOURCE_DIR}/macro/tof/digitizer/tof_digi2.C)
#add_test(tof_digi2 ${CBMROOT_BINARY_DIR}/macro/tof/digitizer/tof_digi2.sh)
#SET_TESTS_PROPERTIES(tof_digi2 PROPERTIES TIMEOUT "300")
#SET_TESTS_PROPERTIES(tof_digi2 PROPERTIES DEPENDS tof_disim)
#SET_TESTS_PROPERTIES(tof_digi2 PROPERTIES PASS_REGULAR_EXPRESSION "Test Passed;All ok")

### Copy TOF macros and folders to allow execution of tasks on batch farm after installation
#Install(FILES .rootrc run_mc.C 
#        DESTINATION share/cbmroot/macro/tof/beamtime/feb15
#       )
#Install(FILES .rootrc run_mc.C
#        DESTINATION share/cbmroot/macro/tof/beamtime/nov15
#       )
#Install(DIRECTORY modules DESTINATION share/cbmroot/macro/run)
#Install(CODE "FILE(MAKE_DIRECTORY \${CMAKE_INSTALL_PREFIX}/share/cbmroot/macro/run/data)")

Install(FILES qa/tof_qa_sim.C qa/tof_qa_ana.C qa/tof_qa_hp.C qa/tof.digibdf.par
        DESTINATION share/cbmroot/macro/tof/qa
       )
Install(CODE "FILE(MAKE_DIRECTORY \${CMAKE_INSTALL_PREFIX}/share/cbmroot/macro/tof/qa/data)")

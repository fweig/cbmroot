# Put the .rootrc file into the directory from which root is executed.
# Otherwise the rootalias file is not loaded
File(COPY ${CBMROOT_SOURCE_DIR}/macro/include/.rootrc DESTINATION ${CBMROOT_BINARY_DIR}/macro/mvd/qa)

GENERATE_ROOT_TEST_SCRIPT(${CBMROOT_SOURCE_DIR}/macro/mvd/qa/mvd_qa1_transUrqmd.C)
add_test(mvd_qa1_transUrqmd ${CBMROOT_BINARY_DIR}/macro/mvd/qa/mvd_qa1_transUrqmd.sh)
SET_TESTS_PROPERTIES(mvd_qa1_transUrqmd PROPERTIES TIMEOUT "300")
SET_TESTS_PROPERTIES(mvd_qa1_transUrqmd PROPERTIES PASS_REGULAR_EXPRESSION "Test Passed;All ok")
set_tests_properties(mvd_qa1_transUrqmd PROPERTIES FIXTURES_SETUP fixture_mvd_qa3_digitize)

GENERATE_ROOT_TEST_SCRIPT(${CBMROOT_SOURCE_DIR}/macro/mvd/qa/mvd_qa2_transDelta.C)
add_test(mvd_qa2_transDelta ${CBMROOT_BINARY_DIR}/macro/mvd/qa/mvd_qa2_transDelta.sh)
SET_TESTS_PROPERTIES(mvd_qa2_transDelta PROPERTIES TIMEOUT "500")
SET_TESTS_PROPERTIES(mvd_qa2_transDelta PROPERTIES PASS_REGULAR_EXPRESSION "Test Passed;All ok")
#set_tests_properties(mvd_qa2_transDelta PROPERTIES FIXTURES_REQUIRED fixture_mvd_qa2_transDelta)
set_tests_properties(mvd_qa2_transDelta PROPERTIES FIXTURES_SETUP fixture_mvd_qa3_digitize)

GENERATE_ROOT_TEST_SCRIPT(${CBMROOT_SOURCE_DIR}/macro/mvd/qa/mvd_qa3_digitize.C)
add_test(mvd_qa3_digitize ${CBMROOT_BINARY_DIR}/macro/mvd/qa/mvd_qa3_digitize.sh)
SET_TESTS_PROPERTIES(mvd_qa3_digitize PROPERTIES TIMEOUT "300")
SET_TESTS_PROPERTIES(mvd_qa3_digitize PROPERTIES PASS_REGULAR_EXPRESSION "Test Passed;All ok")
set_tests_properties(mvd_qa3_digitize PROPERTIES FIXTURES_REQUIRED fixture_mvd_qa3_digitize)
set_tests_properties(mvd_qa3_digitize PROPERTIES FIXTURES_SETUP fixture_mvd_qa4_reco)

GENERATE_ROOT_TEST_SCRIPT(${CBMROOT_SOURCE_DIR}/macro/mvd/qa/mvd_qa4_reco.C)
add_test(mvd_qa4_reco ${CBMROOT_BINARY_DIR}/macro/mvd/qa/mvd_qa4_reco.sh)
SET_TESTS_PROPERTIES(mvd_qa4_reco PROPERTIES TIMEOUT "300")
SET_TESTS_PROPERTIES(mvd_qa4_reco PROPERTIES PASS_REGULAR_EXPRESSION "Test Passed;All ok")
set_tests_properties(mvd_qa4_reco PROPERTIES FIXTURES_REQUIRED fixture_mvd_qa4_reco)


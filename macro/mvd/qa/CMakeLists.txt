# Put the .rootrc file into the directory from which root is executed.
# Otherwise the rootalias file is not loaded
File(COPY ${CBMROOT_SOURCE_DIR}/macro/include/.rootrc DESTINATION ${CBMROOT_BINARY_DIR}/macro/mvd/qa)

# =====   Cleanup the data directory   =======================================
add_test(mvd_cleanup ${CMAKE_COMMAND}
        -P ${CMAKE_SOURCE_DIR}/cmake/scripts/cleanmacrodir.cmake)
set_tests_properties(mvd_cleanup PROPERTIES
        TIMEOUT 10
        FIXTURES_SETUP mvd_cleanup
)


GENERATE_ROOT_TEST_SCRIPT(${CBMROOT_SOURCE_DIR}/macro/mvd/qa/mvd_qa1_transUrqmd.C)
add_test(mvd_qa1_transUrqmd ${CBMROOT_BINARY_DIR}/macro/mvd/qa/mvd_qa1_transUrqmd.sh)
SET_TESTS_PROPERTIES(mvd_qa1_transUrqmd PROPERTIES
        TIMEOUT "300"
        PASS_REGULAR_EXPRESSION "Test Passed;All ok"
        FIXTURES_SETUP fixture_mvd_qa3_digitize
        FIXTURES_REQUIRED mvd_cleanup
)

GENERATE_ROOT_TEST_SCRIPT(${CBMROOT_SOURCE_DIR}/macro/mvd/qa/mvd_qa2_transDelta.C)
add_test(mvd_qa2_transDelta ${CBMROOT_BINARY_DIR}/macro/mvd/qa/mvd_qa2_transDelta.sh)
SET_TESTS_PROPERTIES(mvd_qa2_transDelta PROPERTIES
        TIMEOUT "500"
        PASS_REGULAR_EXPRESSION "Test Passed;All ok"
        FIXTURES_REQUIRED mvd_cleanup
        FIXTURES_SETUP fixture_mvd_qa3_digitize
)

GENERATE_ROOT_TEST_SCRIPT(${CBMROOT_SOURCE_DIR}/macro/mvd/qa/mvd_qa3_digitize.C)
add_test(mvd_qa3_digitize ${CBMROOT_BINARY_DIR}/macro/mvd/qa/mvd_qa3_digitize.sh)
SET_TESTS_PROPERTIES(mvd_qa3_digitize PROPERTIES
        TIMEOUT "300"
        PASS_REGULAR_EXPRESSION "Test Passed;All ok"
        FIXTURES_REQUIRED fixture_mvd_qa3_digitize
        FIXTURES_SETUP fixture_mvd_qa4_reco
)

GENERATE_ROOT_TEST_SCRIPT(${CBMROOT_SOURCE_DIR}/macro/mvd/qa/mvd_qa4_reco.C)
add_test(mvd_qa4_reco ${CBMROOT_BINARY_DIR}/macro/mvd/qa/mvd_qa4_reco.sh)
SET_TESTS_PROPERTIES(mvd_qa4_reco PROPERTIES
        TIMEOUT "300"
        PASS_REGULAR_EXPRESSION "Test Passed;All ok"
        FIXTURES_REQUIRED fixture_mvd_qa4_reco
)

Install(FILES mvd_qa1_transUrqmd.C mvd_qa2_transDelta.C mvd_qa3_digitize.C mvd_qa4_reco.C
        DESTINATION share/cbmroot/macro/mvd
       )
Install(CODE "FILE(MAKE_DIRECTORY ${CMAKE_INSTALL_PREFIX}/share/cbmroot/macro/mvd/data)")

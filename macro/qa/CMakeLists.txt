# =====   Generate the needed shell scripts   ================================
GENERATE_ROOT_TEST_SCRIPT(${CBMROOT_SOURCE_DIR}/macro/qa/qa_compare.C)

Set(MACRO_DIR ${CMAKE_CURRENT_BINARY_DIR})
# ============================================================================


# =====   Copy the .rootrc file into the directory from which root is executed
# --- Otherwise the rootalias file is not loade
file(COPY ${CBMROOT_SOURCE_DIR}/macro/include/.rootrc
	DESTINATION ${CBMROOT_BINARY_DIR}/macro/qa)
# ============================================================================

# Copy file-object map
file(COPY ${CBMROOT_SOURCE_DIR}/macro/qa/objects.yaml
	DESTINATION ${CBMROOT_BINARY_DIR}/macro/qa)

# =====   Define variables for tests   =======================================
if(${CBM_TEST_MODEL} MATCHES Weekly)
  Set(nEvents 100)
  Set(uploadHistJPG 1)
  Set(pullDevAllowed .2)
  Set(randomSeed 1)
else()
  Set(nEvents 3)
  Set(uploadHistJPG 0)
  Set(pullDevAllowed .5)
  Set(randomSeed 1)
endIf()

math(EXPR timeOutTime "10000")
math(EXPR nBeam "${nEvents} * 3")
# ============================================================================


# =====   Define the different setups to be tested with   ====================
if(NOT ${CBM_TEST_MODEL} MATCHES Experimental )
  List(APPEND cbm_setup
	sis100_hadron
        sis100_electron
	sis100_muon_lmvm
        sis100_muon_jpsi
	sis300_electron
	)
else()
  List(APPEND cbm_setup sis100_electron sis100_muon_jpsi)
endif()
# ============================================================================


# =====   Cleanup the data directory   =======================================
add_test(run_cleanup ${CMAKE_COMMAND}
	-P ${CMAKE_SOURCE_DIR}/cmake/scripts/cleanmacrodir.cmake)
set_tests_properties(run_cleanup PROPERTIES
	TIMEOUT ${timeOutTime}
	FIXTURES_SETUP run_cleanup
)
# ============================================================================


# =====   Define tests for each setup   ======================================
foreach(setup IN LISTS cbm_setup)

  # --- Short name for setup
  if(setup MATCHES sis100_hadron)
        set(sname s100h)
  elseif(setup MATCHES sis100_electron)
        set(sname s100e)
  elseif(setup MATCHES sis100_muon_lmvm)
	set(sname s100m2)
  elseif(setup MATCHES sis100_muon_jpsi)
  	set(sname s100m3)
  elseif(setup MATCHES sis300_electron)
  	set(sname s300e)
  else()
  	set(sname test)
  endif()

  # --- Test run_qa
  # --- Run QA tasks with Event-by-event reconstruction from time-based simulation
  # --- Real raw event builder
  set(testname qa_${sname}_compare)
  add_test(${testname} ${MACRODIR}/qa_compare.sh \"objects.yaml\" \"${sname}\" \"../run/data\" \"../run/data/\" \"data/QACheckerOutput.${sname}.root\" )
  set_tests_properties(${testname} PROPERTIES
  	TIMEOUT ${timeOutTime}
	FAIL_REGULAR_EXPRESSION "segmentation violation"
  PASS_REGULAR_EXPRESSION "Macro finished successfully"
	FIXTURES_REQUIRED "fixt_qa_${setup}"
	FIXTURES_SETUP fixt_qa_compare_${setup}
  )

endforeach(setup IN LISTS cbm_setup)
# ============================================================================

Install(FILES ../run/.rootrc qa_compare.C objects.yaml
        DESTINATION share/cbmroot/macro/qa
       )
#Install(CODE "FILE(MAKE_DIRECTORY \${CMAKE_INSTALL_PREFIX}/share/cbmroot/macro/run/data)")

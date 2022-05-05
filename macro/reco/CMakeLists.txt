# CMakeList.txt for macro/reco
# Defines regular tests
# V. Friese, November 2021

# =====   Generate the needed shell scripts   ================================
GENERATE_ROOT_TEST_SCRIPT(${CBMROOT_SOURCE_DIR}/macro/run/run_tra_file.C)
GENERATE_ROOT_TEST_SCRIPT(${CBMROOT_SOURCE_DIR}/macro/run/run_tra_beam.C)
GENERATE_ROOT_TEST_SCRIPT(${CBMROOT_SOURCE_DIR}/macro/run/run_digi.C)
GENERATE_ROOT_TEST_SCRIPT(${CBMROOT_SOURCE_DIR}/macro/reco/reco_fairroot.C)
# ============================================================================


# =====   Copy the .rootrc file into the directory from which root is executed
# --- Otherwise the rootalias file is not loaded
file(COPY ${CBMROOT_SOURCE_DIR}/macro/include/.rootrc
	DESTINATION ${CBMROOT_BINARY_DIR}/macro/reco)
# ============================================================================


# =====   Define variables for tests   =======================================
Set(randomSeed 1)
if(${CBM_TEST_MODEL} MATCHES Weekly)
  Set(nEvents 100)
else()
  Set(nEvents 3)
endIf()

math(EXPR timeOutTime "${nEvents} * 200")
math(EXPR nBeam "${nEvents} * 3")

set(datadir ${CBMROOT_BINARY_DIR}/macro/reco/data)
# ============================================================================


# =====   Define the different setups to be tested with   ====================
if(NOT ${CBM_TEST_MODEL} MATCHES Experimental )
  List(APPEND cbm_setup sis100_hadron sis100_electron sis100_muon_lmvm
  	sis100_muon_jpsi sis300_electron)
else()
  List(APPEND cbm_setup sis100_hadron)
endif()
# ============================================================================


# =====   Cleanup the data directory   =======================================
add_test(reco_cleanup ${CMAKE_COMMAND}
	-P ${CMAKE_SOURCE_DIR}/cmake/scripts/cleanmacrodir.cmake)
set_tests_properties(reco_cleanup PROPERTIES
	TIMEOUT ${timeOutTime}
	FIXTURES_SETUP reco_cleanup
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

  # --- Test reco_tra_coll
  # --- Transport run with collision events, using run_tra_file.C
  set(testname reco_tra_coll_${sname})
  set(input ${CBMROOT_SOURCE_DIR}/input/urqmd.auau.10gev.centr.root)
  set(output ${datadir}/${sname}_coll)
  add_test(${testname} ${CBMROOT_BINARY_DIR}/macro/run/run_tra_file.sh
  	\"${input}\" ${nEvents} \"${output}\" \"${setup}\" kGeant3 ${randomSeed} kTRUE)
  set_tests_properties(${testname} PROPERTIES
  	TIMEOUT ${timeOutTime}
  	PASS_REGULAR_EXPRESSION "Macro finished successfully"
  	FIXTURES_REQUIRED reco_cleanup
  	FIXTURES_SETUP fixt_tra_coll_${setup}
  	RESOURCE_LOCK collParDb_${setup}
  )

  # --- Test reco_tra_sign
  # --- Transport run with signal events, using run_tra_file.C
  set(testname reco_tra_sign_${sname})
  set(input ${CBMROOT_SOURCE_DIR}/input/pluto.auau.8gev.omega.mpmm.0001.root)
  set(output ${datadir}/${sname}_sign)
   add_test(${testname} ${CBMROOT_BINARY_DIR}/macro/run/run_tra_file.sh
  	\"${input}\" ${nEvents} \"${output}\" \"${setup}\" kGeant3 ${randomSeed} kTRUE)
  set_tests_properties(${testname} PROPERTIES
  	TIMEOUT ${timeOutTime}
  	PASS_REGULAR_EXPRESSION "Macro finished successfully"
  	FIXTURES_REQUIRED reco_cleanup
  	FIXTURES_SETUP fixt_tra_sign_${setup}
  	RESOURCE_LOCK signParDb_${setup}
  )

  # --- Test reco_tra_beam
  # --- Transport run with beam events, using run_tra_beam.C
  set(testname reco_tra_beam_${sname})
  set(output ${datadir}/${sname}_beam)
   add_test(${testname} ${CBMROOT_BINARY_DIR}/macro/run/run_tra_beam.sh
  	${nBeam} \"Au\" 10 -1 \"${output}\" \"${setup}\" kGeant3 ${randomSeed} kTRUE)
  set_tests_properties(${testname} PROPERTIES
  	TIMEOUT ${timeOutTime}
  	PASS_REGULAR_EXPRESSION "Macro finished successfully"
  	FIXTURES_REQUIRED reco_cleanup
  	FIXTURES_SETUP fixt_tra_beam_${setup}
  	RESOURCE_LOCK beamParDb_${setup}
  )

  # --- Test reco_digi
  # --- Detector response simulation, time-based, using run_digi.C
  set(testname reco_digi_${sname})
  set(eventrate 1.e7)
  set(beamrate 1.e9)
  set(tslength 1.e6)
  add_test(${testname} ${CBMROOT_BINARY_DIR}/macro/run/run_digi.sh
  	\"${datadir}/${sname}_coll\" -1 \"${datadir}/${sname}\"\ ${eventrate} ${tslength}
  	\"${datadir}/${sname}_sign\" \"${datadir}/${sname}_beam\" ${beamrate})
  set_tests_properties(${testname} PROPERTIES
  	TIMEOUT ${timeOutTime}
  	PASS_REGULAR_EXPRESSION "Macro finished successfully"
 	FIXTURES_REQUIRED "fixt_tra_coll_${setup};fixt_tra_sign_${setup};fixt_tra_beam_${setup}"
  	FIXTURES_SETUP fixt_digi_ts_${setup}
 	RESOURCE_LOCK collParDb_${setup}
  )

  # --- Test reco_reco_sim
  # --- Reconstruction from time-based simulation
  set(testname reco_reco_sim_${sname})
  add_test(${testname} ${MACRODIR}/reco_fairroot.sh
  	\"${datadir}/${sname}.raw.root\" \"${datadir}/${sname}.reco.root\")
  set_tests_properties(${testname} PROPERTIES
  	TIMEOUT ${timeOutTime}
  	PASS_REGULAR_EXPRESSION "Macro finished successfully"
	FIXTURES_REQUIRED fixt_digi_ts_${setup}
	FIXTURES_SETUP fixt_reco_sim_${setup}
 	RESOURCE_LOCK collParDb_${setup}
  )

endforeach(setup IN LISTS cbm_setup)
# ============================================================================

# ============================================================================

Install(FILES reco_fairroot.C reco_unpack.C
        DESTINATION share/cbmroot/macro/reco
       )
Install(CODE "FILE(MAKE_DIRECTORY \${CMAKE_INSTALL_PREFIX}/share/cbmroot/macro/reco/data)")

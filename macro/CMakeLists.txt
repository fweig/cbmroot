#--- Tests for continuous builds
add_subdirectory(run)
add_subdirectory(mcbm)
add_subdirectory(mvd)
add_subdirectory(much)
add_subdirectory(tof)
add_subdirectory(beamtime)
add_subdirectory(include)
 

#--- Additional tests for nightly builds
If($ENV{ctest_model} MATCHES Nightly)
  add_subdirectory(field)
  add_subdirectory(C2F)
  add_subdirectory(dimuon)
  Message("Additional Nightly tests")
EndIf($ENV{ctest_model}  MATCHES Nightly)

#--- Additional tests for weekly builds
If($ENV{ctest_model} MATCHES Weekly)
  add_subdirectory(KF)
  add_subdirectory(field)
  add_subdirectory(C2F)
  add_subdirectory(dimuon)
  Message("Additional long running Weekly tests")
EndIf()

# Test if all libraries can be loaded
GENERATE_TEST_SCRIPT(${CBMROOT_SOURCE_DIR}/scripts/checklibs.sh ${CBMROOT_BINARY_DIR}/macro/)
add_test(load_libraries ${CBMROOT_BINARY_DIR}/macro/checklibs.sh)

# Install in any case
Install(FILES KF/DecayConfig.C KF/registerGeantDecays.C KF/registerLightIons.C KF/registerPythiaDecays.C
              KF/kf_kfparticle.C KF/kf_thermal_signal_generator.C  KF/kf_transport.C
        DESTINATION share/cbmroot/macro/KF
       )
Install(CODE "FILE(MAKE_DIRECTORY ${CMAKE_INSTALL_PREFIX}/share/cbmroot/macro/KF/data)")

#--- Deprecated (not running)
#add_subdirectory(trd)
#add_subdirectory(littrack)


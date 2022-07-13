macro(generate_config_files)

  # generate config files used for build directory
  WRITE_CONFIG_FILE(config.sh)
  WRITE_CONFIG_FILE(config.csh)

  # generate config files used for installation directory 
  SET(ROOT_INCLUDE_PATH 
      ${ROOT_INCLUDE_PATH} 
      "${CMAKE_INSTALL_PREFIX}/include" 
      "${CMAKE_INSTALL_PREFIX}/include/mvd"
      "${CMAKE_INSTALL_PREFIX}/include/littrack"
      "${CMAKE_INSTALL_PREFIX}/include/KF"
      "${CMAKE_INSTALL_PREFIX}/include/AnalysisTree"
      "${CMAKE_INSTALL_PREFIX}/include/AnalysisTreeQA"
     )

  set(CMAKE_INSTALL_LIBDIR lib)
  SET(VMCWORKDIR ${CMAKE_INSTALL_PREFIX}/share/cbmroot)

  WRITE_CONFIG_FILE(config.sh_install)
  WRITE_CONFIG_FILE(config.csh_install)

  Install(FILES ${CMAKE_BINARY_DIR}/config.sh_install
          DESTINATION bin
          RENAME CbmRootConfig.sh
         )

  Install(FILES ${CMAKE_BINARY_DIR}/check_system.sh
          DESTINATION bin
         )

  Install(FILES ${CMAKE_BINARY_DIR}/config.csh_install
          DESTINATION bin
          RENAME CbmRootConfig.csh
         )

endmacro()


# ROOT macros, par files, documentation files, bash scripts
Install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        DESTINATION share/cbmroot/macro/beamtime
        FILES_MATCHING PATTERN "*.C"
                       PATTERN "*.par"
                       PATTERN "*.md"
                       PATTERN "*.sh"
       )
# RICH calibration file, resolve symlink to get the full file
get_filename_component(_resolvedRichFile ${CMAKE_CURRENT_SOURCE_DIR}/icd_offset_it_0.data REALPATH)
Install(FILES ${_resolvedRichFile}
        DESTINATION share/cbmroot/macro/beamtime/mcbm2022
       )

# Just the empty folder for output
Install(DIRECTORY data
        DESTINATION share/cbmroot/macro/beamtime/mcbm2022
        PATTERN "*" EXCLUDE)

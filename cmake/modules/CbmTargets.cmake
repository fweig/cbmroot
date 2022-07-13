macro(define_additional_targets)

  add_custom_target(cleanlib
                    COMMAND ${CMAKE_COMMAND} -E remove libCbm*
                    COMMAND ${CMAKE_COMMAND} -E remove G__Cbm*
                    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
                   )

  add_custom_target(cleantest
                    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_SOURCE_DIR}/cmake/scripts/cleantest.cmake
                   )

  find_package2(PRIVATE ClangFormat)
  if(ClangFormat_FOUND AND EXISTS ${CMAKE_SOURCE_DIR}/.git)
    if (FAIRROOT_FORMAT_BASE)
      set(BASE_COMMIT ${FAIRROOT_FORMAT_BASE})
    else()
      set(BASE_COMMIT upstream/master)
    endif()

    if (FAIRROOT_GIT_CLANG_FORMAT_BIN)
      set(GIT_CLANG_FORMAT_BIN ${FAIRROOT_GIT_CLANG_FORMAT_BIN})
    else()
      set(GIT_CLANG_FORMAT_BIN git-clang-format)
    endif()

    # Create a list C, C++ and header files which have been changed in the
    # current commit
    execute_process(COMMAND ${CMAKE_SOURCE_DIR}/cmake/scripts/find_files.sh
                    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                    OUTPUT_VARIABLE FileList
                    OUTPUT_STRIP_TRAILING_WHITESPACE
                   )
    string(REGEX REPLACE " " ";" FileList "${FileList}")

    # Loop over the files and create the code whch is executed when running
    # "make FormatCheck".
    # The produced code will run clang-format on one of the files. If
    # clang-format finds code which are not satisfying our code rules a
    # detailed error message is created. This error message can be checked on
    # our CDash web page.
    foreach(file ${FileList})
      set(file1 ${CMAKE_BINARY_DIR}/${file}.cf_out)

      list(APPEND myfilelist ${file1})
      add_custom_command(OUTPUT ${file1}
                         COMMAND ${CMAKE_SOURCE_DIR}/cmake/scripts/check-format-1.sh ${file} ${file1}
                         WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                        )
    endforeach()

    # Create the target FormatCheck which only depends on the files creted in
    # previous step. When running "make FormatCheck" clang-format is executed
    # for all C, C++ and header files which have changed in the commit.
    add_custom_target(FormatCheck
                      DEPENDS ${myfilelist}
                      )

  endif()

# TODO: check if still needed
#  if(RULE_CHECKER_FOUND)
#    ADD_CUSTOM_TARGET(RULES
#       COMMAND ${RULE_CHECKER_SCRIPT1} ${CMAKE_BINARY_DIR} viol > violations.html
#       DEPENDS $ENV{ALL_RULES})
#  endif(RULE_CHECKER_FOUND)


#  find_package(IWYU)
if(IWYU_FOUND)

  ADD_CUSTOM_TARGET(checkHEADERS
     DEPENDS $ENV{ALL_HEADER_RULES}
  )
endif(IWYU_FOUND)

endmacro()

macro(set_compiler_flags)

  #Check the compiler and set the compile and link flags
  If(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    Set(_default_build_type FullWarnings)
    Set(_cflags "-g -O2")
    Set(_cxxflags "-g -O2 -Wshadow -Wall -Wextra -Wunused-variable")

    String(TOUPPER ${_default_build_type} upper_build_type )
    Set(_build_type_cxx CMAKE_CXX_FLAGS_${upper_build_type})
    Set(_build_type_c CMAKE_C_FLAGS_${upper_build_type})
    Set(_exe_linker_flags CMAKE_EXE_LINKER_FLAGS_${upper_build_type})
    Set(_module_linker_flags CMAKE_MODULE_LINKER_FLAGS_${upper_build_type})
    Set(_shared_linker_flags CMAKE_SHARED_LINKER_FLAGS_${upper_build_type})
    Set(_static_linker_flags CMAKE_STATIC_LINKER_FLAGS_${upper_build_type})

    Message("No build type defined on command line. Set the build type ${_default_build_type}")
    Message("CXX_FLAGS: ${_cxxflags}")
    Message("C_FLAGS: ${_cflags}")

    Set(CMAKE_BUILD_TYPE ${_default_build_type} CACHE STRING "Choose the type of build." FORCE)
    If(CMAKE_SYSTEM_NAME MATCHES Linux OR CMAKE_SYSTEM_NAME MATCHES Darwin)
      If(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        Set(${_build_type_cxx} ${_cxxflags} CACHE STRING "Flags used by the CXX compiler during FullWarnings builds.")
        Set(${_build_type_c} ${_cflags} CACHE STRING "Flags used by the C compiler during FullWarnings builds.")
        Set(${_exe_linker_flags} "" CACHE STRING "Flags used by the linker during FullWarnings builds.")
        Set(${_module_linker_flags} "" CACHE STRING "Flags used by the linker during the creation of modules during FullWarnings builds.")
        Set(${_shared_linker_flags} "" CACHE STRING "Flags used by the linker during the creation of shared libraries during FullWarnings builds.")
        Set(${_static_linker_flags} "" CACHE STRING "Flags used by the linker during the creation of static libraries during FullWarnings builds.")
        mark_as_advanced(
          ${_build_type_cxx}
          ${_build_type_c}
          ${_exe_linker_flags}
          ${_module_linker_flags}
          ${_shared_linker_flags}
          ${_static_linker_flags}
        )
      EndIf()
    EndIf()
  EndIf()
  If(CMAKE_BUILD_TYPE MATCHES CONTINUOUS)
    Set(CMAKE_CXX_FLAGS_CONTINUOUS "-g -O2 -Wshadow -Wall -Wextra -Wunused-variable")
    Set(CMAKE_C_FLAGS_CONTINUOUS "-g -O2")
    Message("The build type is CONTINUOUS")
    Message("CXX_FLAGS: ${CMAKE_CXX_FLAGS_CONTINUOUS}")
    Message("C_FLAGS: ${CMAKE_C_FLAGS_CONTINUOUS}")
  EndIf()
  Check_Compiler()
  set(CMAKE_CONFIGURATION_TYPES ${CMAKE_CONFIGURATION_TYPES} "CONTINUOUS" "FullWarnings")


  #Set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wshadow -Weffc++ -Wno-unused-variable -Wno-unused-parameter -Wno-sign-compare -Wno-ignored-qualifiers -Wno-overloaded-vi
  #Set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Weverything -Wno-padded -Wno-global-constructors")
  #Set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Weverything -Wno-c++98-compat -Wno-c++98-compat-pedantic -Wno-documentation -Wno-padded -Wno-global-constructors -Wno-exit-time-destru
  #Set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wdeprecated -Wunused-exception-parameter -Wconversion -Wsign-conversion -Wold-style-cast -Wshorten-64-to-32 -Wswitch-enum -Wfloat-equa
  #Set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wold-style-cast")

endmacro()

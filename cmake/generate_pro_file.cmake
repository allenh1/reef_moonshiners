
macro(generate_pro_file sources headers qt_components qt_resources)
  set(SOURCES "SOURCES =")
  foreach(s ${sources})
    set(SOURCES "${SOURCES} \\\n ${s}")
  endforeach()

  set(HEADERS "HEADERS =")
  foreach(h ${headers})
    set(HEADERS "${HEADERS}\\\n  ${h}")
  endforeach()

  set(RESOURCES "RESOURCES =")
  foreach(r ${qt_resources})
    set(RESOURCES "${RESOURCES}\\\n  ${r}")
  endforeach()

  foreach(c ${qt_components})
    string(TOLOWER "${c}" c)
    set(COMPONENTS "${COMPONENTS} ${c}")
  endforeach()

  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/reef_moonshiners.pro.in reef_moonshiners.pro
    @ONLY
  )
endmacro()

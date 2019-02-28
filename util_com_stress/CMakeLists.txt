
add_executable(util_com_stress "")
target_sources(util_com_stress
        PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/src/util_com_stress.c
)

target_link_libraries(util_com_stress
	PUBLIC
		picogw
)

set_target_properties(util_com_stress PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# add the install targets
install (
        TARGETS util_com_stress
  	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT shlib
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

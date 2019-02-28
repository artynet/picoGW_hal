
add_executable(util_boot "")
target_sources(util_boot
        PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/src/util_boot.c
)

target_link_libraries(util_boot
	PUBLIC
		picogw
)

set_target_properties(util_boot PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# add the install targets
install (
        TARGETS util_boot
  	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT shlib
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

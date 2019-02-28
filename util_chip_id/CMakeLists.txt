
add_executable(util_chip_id "")
target_sources(util_chip_id
        PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/src/util_chip_id.c
)

target_link_libraries(util_chip_id
	PUBLIC
		picogw
)

set_target_properties(util_chip_id PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# add the install targets
install (
        TARGETS util_chip_id
  	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT shlib
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

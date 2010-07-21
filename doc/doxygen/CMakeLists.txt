PROJECT(doxygen)

INCLUDE(FindDoxygen)

CONFIGURE_FILE(${doxygen_SOURCE_DIR}/Doxyfile.in ${doxygen_BINARY_DIR}/Doxyfile)

ADD_CUSTOM_TARGET(doc-doxygen
	COMMAND ${DOXYGEN_EXECUTABLE} ${doxygen_BINARY_DIR}/Doxyfile
	)


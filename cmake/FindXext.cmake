find_path(XEXT_INCLUDE_DIR NAMES X11/extensions/Xext.h
	PATH_SUFFIXES X11/extensions
	PATHS /opt/X11/include
	DOC "The Xext include directory"
	)

find_library(XEXT_LIBRARY NAMES Xext
	PATHS /opt/X11/lib
	DOC "The Xext library"
	)

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(Xext DEFAULT_MSG XEXT_LIBRARY XEXT_INCLUDE_DIR)

if(XEXT_FOUND)
	set( XEXT_LIBRARIES ${XEXT_LIBRARY}  )
	set( XEXT_INCLUDE_DIRS ${XEXT_INCLUDE_DIR}  )
endif()

mark_as_advanced(XEXT_INCLUDE_DIR XEXT_LIBRARY)

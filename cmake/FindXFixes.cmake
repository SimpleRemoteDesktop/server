find_path(XFIXES_INCLUDE_DIR NAMES X11/extensions/Xfixes.h
	PATH_SUFFIXES X11/extensions
	PATHS /opt/X11/include
	DOC "The Xfixes include directory"
	)

find_library(XFIXES_LIBRARY NAMES Xfixes
	PATHS /opt/X11/lib
	DOC "The Xfixes library"
	)

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(XFixes DEFAULT_MSG XFIXES_LIBRARY XFIXES_INCLUDE_DIR)

if(XFIXES_FOUND)
	set( XFIXES_LIBRARIES ${XFIXES_LIBRARY}  )
	set( XFIXES_INCLUDE_DIRS ${XFIXES_INCLUDE_DIR}  )
endif()

mark_as_advanced(XFIXES_INCLUDE_DIR XFIXES_LIBRARY)

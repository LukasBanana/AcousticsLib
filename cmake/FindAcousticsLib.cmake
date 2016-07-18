
# Custom CMake module for finding "AcousticsLib" files
# Written by Lukas Hermanns on 18/07/2016

# Macros

macro(_GM_APPEND_LIBRARIES _list _release)
	set(_debug ${_release}_DEBUG)
	if(${_debug})
		set(${_list} ${${_list}} optimized ${${_release}} debug ${${_debug}})
	else()
		set(${_list} ${${_list}} ${${_release}})
	endif()
endmacro()

# Find library

find_path(AcLib_INCLUDE_DIR Geom/Geom.h)

find_library(AcLib_LIBRARY NAMES AcLib)
find_library(AcLib_LIBRARY_DEBUG NAMES AcLibD)

# Setup package handle

include(FindPackageHandleStandardArgs)

FIND_PACKAGE_HANDLE_STANDARD_ARGS(
	AcLib
	DEFAULT_MSG
	AcLib_INCLUDE_DIR
    AcLib_LIBRARY
    AcLib_LIBRARY_DEBUG
)

if(ACLIB_FOUND)
	set(AcLib_FOUND TRUE)
	_GM_APPEND_LIBRARIES(AcLib_LIBRARIES AcLib_LIBRARY)
endif(ACLIB_FOUND)

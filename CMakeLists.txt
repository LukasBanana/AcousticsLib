
# === CMake lists for the AcousticsLib - (15/07/2016) ===

cmake_minimum_required(VERSION 2.8)
project(AcousticsLib)


# === Build path ===

set(OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/build)
set(EXECUTABLE_OUTPUT_PATH ${OUTPUT_DIR} CACHE PATH "Build directory" FORCE)
set(LIBRARY_OUTPUT_PATH ${OUTPUT_DIR} CACHE PATH "Build directory" FORCE)
set(PROJECT_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/include")


# === Preprocessor definitions ===

macro(ADD_DEFINE IDENT)
	if(MSVC)
		add_definitions("/D${IDENT}")
	else()
		add_definitions("-D${IDENT}")
	endif()
endmacro()

macro(ADD_TEST_PROJECT TEST_NAME TEST_FILES)
	add_executable(${TEST_NAME} ${TEST_FILES})
	target_link_libraries(${TEST_NAME} AcLib)
	set_target_properties(${TEST_NAME} PROPERTIES LINKER_LANGUAGE CXX DEBUG_POSTFIX "D")
	target_compile_features(${TEST_NAME} PRIVATE cxx_range_for)
endmacro()

if(WIN32)
	ADD_DEFINE(_CRT_SECURE_NO_WARNINGS)
	ADD_DEFINE(_SCL_SECURE_NO_WARNINGS)
	ADD_DEFINE(NOMINMAX)
	if(MSVC)
		# Disable some warnings for MSVC compiler
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4250 /wd4251 /wd4290 /wd4103")
		ADD_DEFINE(_USE_MATH_DEFINES)
	endif()
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /DAC_DEBUG")
else()
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DAC_DEBUG")
endif()


# === Options ===

option(ACLIB_BUILD_NULL_AUDIO_SYSTEM "Build Null Audio System (for Debugging)" ON)


# === Global files ===

file(GLOB FilesInclude			${PROJECT_INCLUDE_DIR}/Ac/*.*)
file(GLOB FilesCore				${PROJECT_SOURCE_DIR}/sources/Core/*.*)
file(GLOB FilesFileHandler		${PROJECT_SOURCE_DIR}/sources/FileHandler/*.*)
file(GLOB FilesSystem			${PROJECT_SOURCE_DIR}/sources/System/*.*)
file(GLOB FilesSystemNull		${PROJECT_SOURCE_DIR}/sources/System/Null/*.*)
file(GLOB FilesSystemOpenAL		${PROJECT_SOURCE_DIR}/sources/System/OpenAL/*.*)
file(GLOB FilesSystemXAudio2	${PROJECT_SOURCE_DIR}/sources/System/XAudio2/*.*)
file(GLOB FilesPlatformBase		${PROJECT_SOURCE_DIR}/sources/Platform/*.*)

if(WIN32)
	file(GLOB FilesPlatform ${PROJECT_SOURCE_DIR}/sources/Platform/Win32/*.*)
elseif(APPLE)
	set(CMAKE_MACOSX_RPATH 1)
	file(GLOB FilesPlatform ${PROJECT_SOURCE_DIR}/sources/Platform/MacOS/*.*)
elseif(UNIX)
	file(GLOB FilesPlatform ${PROJECT_SOURCE_DIR}/sources/Platform/Linux/*.*)
endif()

set(FilesTest1 ${PROJECT_SOURCE_DIR}/test/Test1.cpp)
set(FilesTest2 ${PROJECT_SOURCE_DIR}/test/Test2.cpp)
set(FilesTest3 ${PROJECT_SOURCE_DIR}/test/Test3_3D.cpp)
set(FilesTest4 ${PROJECT_SOURCE_DIR}/test/Test4_Stream.cpp)
set(FilesTest5 ${PROJECT_SOURCE_DIR}/test/Test5_Mic.cpp)
set(FilesTest6 ${PROJECT_SOURCE_DIR}/test/Test6_Vis.cpp)


# === Source group folders ===

# --- Dyancmis ---

source_group("Include" FILES ${FilesInclude})
source_group("Sources\\Core" FILES ${FilesCore})
source_group("Sources\\FileHandler" FILES ${FilesFileHandler})
source_group("Sources\\System" FILES ${FilesSystem})
source_group("Sources\\System\\Null" FILES ${FilesSystemNull})
source_group("Sources\\System\\OpenAL" FILES ${FilesSystemOpenAL})
source_group("Sources\\System\\XAudio2" FILES ${FilesSystemXAudio2})
source_group("Sources\\Platform" FILES ${FilesPlatformBase} ${FilesPlatform})


# === Include directories ===

include_directories("${PROJECT_INCLUDE_DIR}")


# === Binaries ===

set(
	FilesAll
	${FilesInclude}
	${FilesCore}
	${FilesFileHandler}
	${FilesSystem}
	${FilesPlatformBase}
	${FilesPlatform}
)

# Main Project
add_library(AcLib SHARED ${FilesAll})
set_target_properties(AcLib PROPERTIES LINKER_LANGUAGE CXX DEBUG_POSTFIX "D")
target_compile_features(AcLib PRIVATE cxx_range_for)

if(WIN32)
	target_link_libraries(AcLib Winmm)
elseif(UNIX)
	target_link_libraries(AcLib dl pthread)
endif()

# Null audio system
if(ACLIB_BUILD_NULL_AUDIO_SYSTEM)
	message("Null Audio System -> AcLib_Null project included")
	add_library(AcLib_Null SHARED ${FilesSystemNull})
	set_target_properties(AcLib_Null PROPERTIES LINKER_LANGUAGE CXX DEBUG_POSTFIX "D")
	target_link_libraries(AcLib_Null AcLib)
	target_compile_features(AcLib_Null PRIVATE cxx_range_for)
endif()

# Library: GaussianLib
include(cmake/FindGaussianLib.cmake)

# Library: OggVorbis
include(cmake/FindOggVorbis.cmake)
if(OggVorbis_FOUND)
	message("Found OggVorbis")
	include_directories(${OGG_INCLUDE_DIR} ${VORBIS_INCLUDE_DIR})
	target_link_libraries(AcLib ${Ogg_LIBRARIES} ${Vorbis_LIBRARIES} ${VorbisFile_LIBRARIES})
	ADD_DEFINE(AC_PLUGIN_OGGVORBIS)
else()
	message("Missing OggVorbis -> OGG stream will be excluded from project")
endif()

# Library: OpenAL
find_package(OpenAL)
if(OPENAL_FOUND)
	message("Found OpenAL -> AcLib_OpenAL project included")
	include_directories(${OPENAL_INCLUDE_DIR})
	add_library(AcLib_OpenAL SHARED ${FilesSystemOpenAL})
	set_target_properties(AcLib_OpenAL PROPERTIES LINKER_LANGUAGE CXX DEBUG_POSTFIX "D")
	target_link_libraries(AcLib_OpenAL AcLib ${OPENAL_LIBRARY})
	target_compile_features(AcLib_OpenAL PRIVATE cxx_range_for)
endif()

# Library: XAudio2
if(WIN32)
	message("Found XAudio2 -> AcLib_XAudio2 project included")
	add_library(AcLib_XAudio2 SHARED ${FilesSystemXAudio2})
	set_target_properties(AcLib_XAudio2 PROPERTIES LINKER_LANGUAGE CXX DEBUG_POSTFIX "D")
	target_link_libraries(AcLib_XAudio2 AcLib Xaudio2)
	target_compile_features(AcLib_XAudio2 PRIVATE cxx_range_for)
endif()

# Test Projects
ADD_TEST_PROJECT(Test1 ${FilesTest1})
ADD_TEST_PROJECT(Test2 ${FilesTest2})
ADD_TEST_PROJECT(Test3_3D ${FilesTest3})
ADD_TEST_PROJECT(Test4_Stream ${FilesTest4})
ADD_TEST_PROJECT(Test5_Mic ${FilesTest5})

# Library: OpenGL & GLUT (for Test6)
find_package(OpenGL)
find_package(GLUT)
if(OpenGL_FOUND AND GLUT_FOUND)
	message("Found OpenGL and GLUT -> Test6_Vis project included")
	include_directories(${GLUT_INCLUDE_DIR})
	ADD_TEST_PROJECT(Test6_Vis ${FilesTest6})
	target_link_libraries(Test6_Vis ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})
else()
	message("Missing OpenGL and/or GLUT -> Test6_Vis will be excluded from project")
endif()



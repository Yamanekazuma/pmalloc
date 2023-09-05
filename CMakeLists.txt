cmake_minimum_required (VERSION 3.12)

set(VERSION_MAJOR 0)
set(VERSION_MINOR 2)
set(VERSION_PATCH 0)

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

FILE(GLOB src "src/*.c")
FILE(GLOB include "include/*.h")

project("pmalloc" VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH} LANGUAGES C)

add_library (
	${PROJECT_NAME}
	STATIC
	${src}
	${include}
)

add_library (${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_compile_features (${PROJECT_NAME} PUBLIC c_std_17)


target_compile_definitions (
	${PROJECT_NAME}
	PRIVATE
	WIN32
	_WINDOWS
	UNICODE
	_UNICODE
	$<$<CONFIG:Debug>:
		_DEBUG
		DEBUG
	>
)

if (MSVC)
	target_compile_options (
		${PROJECT_NAME}
		PRIVATE
		/arch:AVX
		/EHa-
		/EHs-
		/GA
		/GF
		/nologo
		/TC
		/utf-8
		/wd5105
		$<$<CONFIG:Debug>:
			/GR
			/JMC
			/RTC1
		>
		$<$<CONFIG:Release>:
			/GL
			/GR-
			/Gw
			/Gy
			/Oi
		>
	)
else()
	target_compile_options (
		${PROJECT_NAME}
		PRIVATE
		-mavx
		-mfpmath=sse
		$<$<CONFIG:Debug>:
			-Og
		>
		$<$<CONFIG:Release>:
			-fomit-frame-pointer
			-ftree-vectorize
		>
	)
endif()

target_include_directories (
	${PROJECT_NAME}
	PRIVATE
	include
)

target_include_directories (
	${PROJECT_NAME}
	PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/share>
	$<INSTALL_INTERFACE:include>
)

install (
	TARGETS ${PROJECT_NAME}
	EXPORT ${PROJECT_NAME}-config
	ARCHIVE DESTINATION lib
)

install (
	DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/share/
	DESTINATION include
)

install (
	EXPORT ${PROJECT_NAME}-config
	NAMESPACE ${PROJECT_NAME}::
	DESTINATION cmake
)

include(CMakePackageConfigHelpers)

write_basic_package_version_file (
	${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake
	COMPATIBILITY SameMajorVersion
)

install (
	FILES ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake
	DESTINATION cmake
)

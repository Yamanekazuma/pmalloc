cmake_minimum_required (VERSION 3.12)

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

FILE(GLOB src "src/*.c")
FILE(GLOB include "include/*.h")

project("pmalloc" VERSION 0.1.0 LANGUAGES C)

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
	PMALLOC_EXPORTS
	WIN32
	_WINDOWS
	UNICODE
	_UNICODE
	$<$<CONFIG:Debug>:
		_DEBUG
		DEBUG
	>
	$<$<CONFIG:Release>:
		_NDEBUG
		NDEBUG
	>
)

target_compile_options (
	${PROJECT_NAME}
	PRIVATE
	/W4
	/arch:AVX
	/EHsc
	/fp:fast
	/GS
	/GT
	/MP
	/nologo
	/Oi
	/Qpar
	/utf-8
	$<$<CONFIG:Debug>:
		/GR
		/JMC
		/MDd
		/Ob0
		/Od
		/RTC1
		/ZI
	>
	$<$<CONFIG:Release>:
		/GA
		/GL
		/GR-
		/Gy
		/guard:cf-
		/MD
		/O2
		/Oi
		/Os
	>
)

target_link_options (
	${PROJECT_NAME}
	PRIVATE
	/LTCG
	/nologo
	$<$<CONFIG:Debug>:
		/debug
		/opt:noref
	>
	$<$<CONFIG:Release>:
		/opt:ref
	>
)

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

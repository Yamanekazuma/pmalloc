cmake_minimum_required (VERSION 3.12)

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("pm-sample" VERSION 0.1.0 LANGUAGES C)

add_executable (
	${PROJECT_NAME}
	WIN32
	"sample.c"
	"sample.h"
)

find_package(pmalloc)
target_link_libraries(${PROJECT_NAME} PRIVATE pmalloc::pmalloc)

target_compile_features(${PROJECT_NAME} PRIVATE c_std_17)

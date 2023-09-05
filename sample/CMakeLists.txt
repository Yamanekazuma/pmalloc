cmake_minimum_required (VERSION 3.12)

if (POLICY CMP0141)
  cmake_policy (SET CMP0141 NEW)
  set (CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("pm-sample" VERSION 1.0.0 LANGUAGES C)

add_executable (
  ${PROJECT_NAME}
  WIN32
  "sample.c"
  "sample.h"
)

add_subdirectory (deps/pmalloc EXCLUDE_FROM_ALL)
target_link_libraries (${PROJECT_NAME} PRIVATE pmalloc::pmalloc)

target_compile_features (${PROJECT_NAME} PRIVATE c_std_17)

if (MSVC)
  target_link_options (${PROJECT_NAME} PRIVATE "LINKER:/SUBSYSTEM:CONSOLE$<$<CONFIG:Release>:,/RELEASE>")
else()
  target_link_options (${PROJECT_NAME} PRIVATE "LINKER:--subsystem,console")
endif()

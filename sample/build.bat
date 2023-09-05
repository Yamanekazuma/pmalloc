@echo off
goto :START

:USAGE
echo Usage: build.bat {x86^|x64} {debug^|release}
exit /b

:START
if "%~2%"=="" (
  call :USAGE
  exit /b
)

if not "%~1%"=="x86" (
  if not "%~1%"=="x64" (
    call :USAGE
    exit /b
  )
)

if not "%~2%"=="debug" (
  if not "%~2%"=="release" (
    call :USAGE
    exit /b
  )
)

mkdir build 2> nul
mkdir deps 2> nul
rmdir deps\pmalloc 2> nul
rmdir /s /q build\%~1% %-%~2%-win 2> nul

mklink /j "deps\pmalloc" "..\"            ^
  && cmake --preset=%~1% %-%~2%-win       ^
  && cmake --build build\%~1% %-%~2%-win

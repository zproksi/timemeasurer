@echo off
setlocal enabledelayedexpansion

REM ------------------------
REM Default Settings
REM ------------------------

set BITNESS=64
set BITNESS64=ON
set MODE=Release
set PROJECT=timemeasurer
set RETURN_VAL=0

goto :scriptstart

REM ------------------------
REM Some Functions Definitions
REM ------------------------

:divisor
echo ----------------------------------
exit /b
:manual
call :divisor
set SCRIPT_NAME=%~nx0
echo To build unit tests for timemeasurer:
echo Execute:
echo "%SCRIPT_NAME%"           Release mode with 64 bits bitness
echo "%SCRIPT_NAME% 32"        to set 32 bits bitness
echo "%SCRIPT_NAME% Debug"     to set Debug mode
call :divisor
exit /b

REM ------------------------
REM Parse Arguments
REM ------------------------

:scriptstart
FOR %%P IN (%*) DO (
    IF "%%P"=="32" (
        set BITNESS=32
        set BITNESS64=OFF
    )
    IF /i "%%P"=="Debug" (
        set MODE=Debug
    )
    IF /i "%%P"=="--help" (
        call :manual
        exit /b 0
    )
    IF "%%P"=="--man" (
        call :manual
        exit /b 0
    )
    IF "%%P"=="-h" (
        call :manual
        exit /b 0
    )
    IF "%%P"=="--h" (
        call :manual
        exit /b 0
    )
)

REM ------------------------
REM Print Selected Options
REM ------------------------

call :divisor
echo "Chosen mode is %MODE% for %BITNESS% bits"
call :divisor

:: Save the initial execution directory
set "INITIALDIR=%cd%"

:: Check if MODE is not Debug, if true set it to Release
if not "%MODE%"=="Debug" set "MODE=Release"

IF "%VS170COMNTOOLS%"=="" (
    echo Environment variable VS170COMNTOOLS not set or is empty
    echo Please set up this variable or run this script from the developer command line of MSVS 2022
    echo PS. This script need to be reworked if you are using later version of MSVS
    exit /b 1
)

SET "CMAKECMD=%VS170COMNTOOLS%..\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe"

IF NOT EXIST "%CMAKECMD%" (
    echo Cannot find cmake at the path: "%CMAKECMD%"
    echo Most probably you should install cmake for MSVS
    exit /b 1
) ELSE (
    echo ----------------------------------
    echo  Rebuilding of bpatch %MODE% mode
    echo ----------------------------------
)

:: Establish the directory containing the script
set "PROJECTDIR=%~dp0"

:: rebuilding initiation
cd /d "%PROJECTDIR%"
if exist "buildWin%MODE%%BITNESS%" rmdir /s /q "buildWin%MODE%%BITNESS%"
mkdir "buildWin%MODE%%BITNESS%" && cd "buildWin%MODE%%BITNESS%"
"%CMAKECMD%" -DCMAKE_BUILD_TYPE=%MODE% .. -DBITNESS64=%BITNESS64%
"%CMAKECMD%" --build . --config %MODE%

call :divisor
set "TESTNAME=test%PROJECT%"
set "FILENAME=%PROJECTDIR%buildWin%MODE%%BITNESS%\%MODE%\%TESTNAME%.exe"


IF EXIST "%FILENAME%" (
  echo "%TESTNAME%.exe built for %MODE% mode with %BITNESS% bits bitness is here:"
  echo %FILENAME%
) ELSE (
  echo "%TESTNAME%.exe built for %MODE% mode with %BITNESS% bits bitness could not be found at"
  echo %FILENAME%
)

call :divisor

:: Restore the initial execution directory
cd /d %INITIALDIR%

endlocal

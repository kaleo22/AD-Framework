@echo off
setlocal

set "ROOT=%~dp0.."
set "TEST_BUILD=%ROOT%\build\tests"

if not exist "%TEST_BUILD%" mkdir "%TEST_BUILD%"
if errorlevel 1 exit /b 1

rem Match the configuration of the MSYS2 UCRT64 Google Test package.
g++ -std=c++17 -Wall -Wextra -DGTEST_HAS_PTHREAD=0 ^
    -I"%ROOT%\include" ^
    "%ROOT%\tests\REQ-2-10.cpp" ^
    "%ROOT%\src\agent\dll_agent_adapter.cpp" ^
    "%ROOT%\src\framework\agent_loader.cpp" ^
    "%ROOT%\src\game\game_engine.cpp" ^
    "%ROOT%\src\framework\loaded_plugin.cpp" ^
    -o "%TEST_BUILD%\req_2_10_tests.exe" ^
    -lgtest_main -lgtest ^
    -lboost_filesystem-mt

if errorlevel 1 exit /b 1

"%TEST_BUILD%\req_2_10_tests.exe" --gtest_output="xml:%TEST_BUILD%\req_2_10_results.xml" %*
exit /b %errorlevel%

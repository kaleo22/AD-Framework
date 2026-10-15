@echo off
setlocal

set ROOT=%~dp0..
set INCLUDE=%ROOT%\include
set SRC=%ROOT%\src
set PLUGINS=%ROOT%\plugins\examples
set BUILD=%ROOT%\build
set PLUGIN_BUILD=%ROOT%\plugins\build
if not exist "%BUILD%" mkdir "%BUILD%"
if not exist "%PLUGIN_BUILD%" mkdir "%PLUGIN_BUILD%"

echo Building plugin_maf.dll...

g++ ^
    -I"%INCLUDE%\plugins" ^
    -shared ^
    "%PLUGINS%\plugin_maf.cpp" ^
    -o "%PLUGIN_BUILD%\plugin_maf.dll"

if errorlevel 1 goto :error


echo Building simple_agent.dll...

g++ ^
    -I"%INCLUDE%\plugins" ^
    -shared ^
    "%PLUGINS%\plugin_counter.cpp" ^
    -o "%PLUGIN_BUILD%\plugin_counter.dll"

if errorlevel 1 goto :error

echo Building AD Framework...

g++ ^
    -I"%INCLUDE%" ^
    "%SRC%\main.cpp" ^
    "%SRC%\framework\loaded_plugin.cpp" ^
    "%SRC%\framework\plugin_manager.cpp" ^
    "%SRC%\game\game_engine.cpp" ^
    "%SRC%\renderer\sfml_renderer.cpp" ^
    "%SRC%\agent\dll_agent_adapter.cpp" ^
    "%SRC%\framework\agent_loader.cpp" ^
    -o "%BUILD%\AD_Framework.exe" ^
    -lboost_filesystem-mt ^
    -lsfml-graphics ^
    -lsfml-window ^
    -lsfml-system

if errorlevel 1 goto :error

copy /Y "%PLUGIN_BUILD%\plugin_maf.dll" "%BUILD%\plugin_maf.dll" >nul
copy /Y "%PLUGIN_BUILD%\plugin_counter.dll" "%BUILD%\plugin_counter.dll" >nul

echo Build successful
echo Output:
echo %BUILD%\AD_Framework.exe
echo.

goto :end


:error

echo BUILD FAILED

exit /b 1


:end
endlocal
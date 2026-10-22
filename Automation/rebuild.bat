@echo off
REM Regenerates project and rebuilds it.
REM Usage: rebuild.bat [Debug|Release]
setlocal
set CONFIG=%~1
if "%CONFIG%"=="" set CONFIG=Debug
python "%~dp0automation.py" rebuild --config %CONFIG%
exit /b %errorlevel%

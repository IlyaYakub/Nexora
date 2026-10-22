@echo off
REM Removes build folder, regenerates project, rebuilds and runs tests.
REM Usage: clean_rebuild_test.bat [Debug|Release]
setlocal
set CONFIG=%~1
if "%CONFIG%"=="" set CONFIG=Debug
python "%~dp0automation.py" clean rebuild test --config %CONFIG%
exit /b %errorlevel%

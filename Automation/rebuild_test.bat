@echo off
REM Regenerates project, rebuilds and runs tests.
REM Usage: rebuild_test.bat [Debug|Release]
setlocal
set CONFIG=%~1
if "%CONFIG%"=="" set CONFIG=Debug
python "%~dp0automation.py" rebuild test --config %CONFIG%
exit /b %errorlevel%

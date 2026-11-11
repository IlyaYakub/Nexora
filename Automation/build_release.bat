@echo off
setlocal
cd /d "%~dp0.."

call Automation\generate_project_files.bat Release
if errorlevel 1 exit /b 1

cmake --build Build --config Release

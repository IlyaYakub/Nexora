@echo off
setlocal
cd /d "%~dp0.."

set CONFIG=%~1
if "%CONFIG%"=="" set CONFIG=Debug

conan install Config/conanfile.py --output-folder=Build --build=missing -s build_type=%CONFIG% -s compiler.cppstd=23
if errorlevel 1 exit /b 1

cmake -S . -B Build -DCMAKE_TOOLCHAIN_FILE=Build/conan_toolchain.cmake

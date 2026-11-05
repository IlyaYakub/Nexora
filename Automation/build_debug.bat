@echo off
setlocal
cd /d "%~dp0.."

conan install Config/conanfile.txt --output-folder=Build --build=missing -s build_type=Debug -s compiler.cppstd=23
if errorlevel 1 exit /b 1

cmake -S . -B Build -DCMAKE_TOOLCHAIN_FILE=Build/conan_toolchain.cmake
if errorlevel 1 exit /b 1

cmake --build Build --config Debug

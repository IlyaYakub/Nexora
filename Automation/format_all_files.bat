@echo off
setlocal enabledelayedexpansion
cd /d "%~dp0.."

for /r Engine %%f in (*.cpp *.h *.hpp) do (
    clang-format -i --style=file:Config/.clang-format "%%f"
)

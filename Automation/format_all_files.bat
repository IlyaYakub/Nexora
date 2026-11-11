@echo off
setlocal
cd /d "%~dp0.."

for /r Engine %%f in (*.cpp *.h) do (
    clang-format -i --style=file:Config/.clang-format "%%f"
)

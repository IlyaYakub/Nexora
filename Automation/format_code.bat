@echo off
REM Formats all .h/.hpp/.cpp files in Engine folder with clang-format (rules: Engine\.clang-format).
python "%~dp0automation.py" format
exit /b %errorlevel%

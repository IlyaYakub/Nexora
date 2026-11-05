@echo off
setlocal
cd /d "%~dp0.."

if exist Build rmdir /s /q Build

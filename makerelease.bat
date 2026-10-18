@echo off
powershell -NoProfile -ExecutionPolicy Bypass -File "%~dp0Build-ImmersionEvolved.ps1"
exit /b %ERRORLEVEL%

@echo off

REM Temp script

call "%Rad370%\bin\rsvars.bat"

call cmd /c msbuild /t:Build /p:Config=Debug;Platform=Win64x SweepThemMines.cbproj

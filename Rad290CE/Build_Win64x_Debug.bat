@echo off

REM Temp script

call "%Rad290%\bin\rsvars.bat"

call cmd /c msbuild /t:Build /p:Config=Debug;Platform=Win64x SweepThemMines.cbproj

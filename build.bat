@echo off
SET CONFIG="Debug|Win32"
SET SOLUTION_DIR="G:\Development\SDL Game engine\SDLApp\SDL Game Engine.sln"

IF EXIST "out.txt" (
	break > out.txt
)

for /F "tokens=*" %%A in (projectlist) do (
	IF EXIST "%%A" (
		ECHO build "%%A"
		devenv %SOLUTION_DIR% /build Debug /project "%%A" /projectconfig %CONFIG%
	)
) >> out.txt
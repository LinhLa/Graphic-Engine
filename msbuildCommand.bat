@echo off
SET CONFIG=%1
SET PLATFORM=%2
SET TOOLVERSION=%3
SET solution="G:\Development\SDL Game engine\SDLApp\SDL Game Engine.sln"

IF EXIST "out.txt" (
	break>out.txt
)

for /F "tokens=*" %%A in (projectlist) do (	
	IF EXIST "%%A" (
		ECHO build "%%A"
		msbuild "%%A" /p:configuration=%CONFIG% /p:platform=%PLATFORM%
	)
) > out.txt
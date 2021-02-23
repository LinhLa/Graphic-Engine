@echo off
SET CONFIG="Debug|Win32"
SET SOLUTION_DIR=".\SDL Game Engine.sln"

IF EXIST "buildlog.txt" (
	break > buildlog.txt
)

for /F "tokens=*" %%A in (projectlist) do (
	IF EXIST "%%A" (
		ECHO build "%%A"
		devenv %SOLUTION_DIR% /build Debug /project "%%A" /projectconfig %CONFIG%
	)
) >> buildlog.txt
@echo off
SET PATH=%PATH%;%cd%\..\..\lib;
SET cont_warning=0
SET exe_name=snowbross_v2_servidor.exe

IF NOT EXIST Debug (
	SET msg=Error: Primero asegurse de haber compilado el proyecto desde Visual Studio y que se encuentre dentro de "%cd%\Debug"
	GOTO showmsg
)
::IF NOT EXIST Debug/imagenes (
::	ECHO Warning: No existe el directorio "imagenes"
::	SET /A cont_warning+=1
::)
::IF NOT EXIST Debug/sonidos (
::	ECHO Warning: No existe el directorio "sonidos"
::	SET /A cont_warning+=1
::)
IF NOT EXIST Debug/%exe_name% (
	SET msg=Error: No existe el archivo %exe_name%
	GOTO showmsg
)
IF %cont_warning% GTR 0 (
	ECHO.
	ECHO Se encontraron Warnings pero se ejecutara el programa de todas formas
	ECHO.
	pause
)

GOTO runprogram

:showmsg
ECHO %msg%
ECHO.
pause
GOTO end

:runprogram
start /d "Debug" %exe_name% escenarioV2.json 8200 

:end
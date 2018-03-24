@echo off
SET PATH=%PATH%;%cd%\..\lib;
SET cont_warning=0

IF NOT EXIST Debug (
	SET msg=Error: Primero asegurse de haber compilado el proyecto desde Visual Studio y que se encuentre dentro de "%cd%\Debug"
	GOTO showmsg
)
IF NOT EXIST Debug/imagenes (
	ECHO Warning: No existe el directorio "imagenes"
	SET /A cont_warning+=1
)
IF NOT EXIST Debug/sonidos (
	ECHO Warning: No existe el directorio "sonidos"
	SET /A cont_warning+=1
)
IF NOT EXIST Debug/proyecto-base.exe (
	SET msg=Error: No existe el archivo proyecto-base.exe
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

:runprogram
start /d "Debug" proyecto-base.exe
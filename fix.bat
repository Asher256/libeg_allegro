@echo off

REM ************ Si entr‚e vide
if not "%1"=="" goto next
echo Programme de Configuration du Makefile pour Msdos et Windows vous
echo devez entrer le nom de votre compilateur.
echo.
echo "djgpp"            pour DJGPP
echo "msvc"             pour Visual C++.
echo "mingw32"          pour Mingw32
echo.
echo Exemple: "fix.bat djgpp"
echo.
echo ATTENTION: Vous devez ‚crire le nom du compilateur en minuscule.
goto fin
:next

REM ******** Teste l'entr‚e de l'utilisateur

if "%1"=="djgpp" goto djgpp
if "%1"=="msvc" goto msvc
if "%1"=="mingw32" goto mingw32

echo Le compilateur "%1" n'est pas valide...
REM Il n'a rien trouv‚? alors end
goto fin

REM ******** LES SUBS

REM [CONFIGURATION MICROSOFT VISUAL C++]
:mingw32
        echo Fixation pour MinGW 32 en cours...
        copy misc\mingw32.ad make.ad >nul
        echo Fin !
        echo.
        echo Ecrivez "make" pour compiler la librarie.
        echo Et "make install" pour installer la librairie.
goto fin

REM [CONFIGURATION MICROSOFT VISUAL C++]
:msvc
        echo Fixation pour Windows Visual C++ en cours...
        copy misc\msvc.ad make.ad >nul
        echo Fin !
        echo.
        echo Ecrivez "make" pour compiler la librarie.
        echo Et "make install" pour installer la librairie.
goto fin

REM [CONFIGURATION DJGPP]
:djgpp
        echo Fixation pour Dos DJGPP en cours...
        copy misc\djgpp.ad make.ad >nul
        echo Fin !
        echo.
        echo Ecrivez "make" pour compiler la librarie.
        echo Et "make install" pour installer la librairie.
goto fin

:fin
echo.

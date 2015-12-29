@echo off

REM **** Suppression de l'archive pour ne pas l'inclure (encore)
del ia.zip

REM **** Cr‚ation de save\
md save

REM **** Copie des fichiers dans save\
copy *.h save
copy *.dat save
copy *.cpp save
copy *.c* save
copy *.g* save
copy *.g* save
copy *.bat save
copy makefile.* save
cd save

REM **** Les mets en zip
zip -9 ia *.*

REM **** Copie le ZIP dans le r‚pertoire racine du projet
copy ia.zip ..
cd ..

deltree /y save
cls
echo Cr‚ation de ia.zip termin‚e...
echo.
choice Copier dans A: 
if errorlevel 2 goto fin

copy ia.zip a:\

:fin
echo.

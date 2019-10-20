@SET currentdir=%cd%
@ECHO OFF
@SET source=%currentdir%
@SET dest=%currentdir%\Bin

ClS
ECHO  **********************
ECHO  *  SWARMINTELIGENCE  *
ECHO  **********************
ECHO  Curent dir %currentdir%
ECHO  And push to %dest%

ECHO Clean bin folder
rmdir /S /Q %dest%

Mkdir %dest%

ECHO Copying
@SET execPath="%source%\release\Boids.exe"
ECHO %execPath%
xcopy %source%\release\Boids.exe %dest%

ECHO *WINDEPLOYQ**

Cd %dest%

REM CALL  "C:\Qt\5.12.5\msvc2017_64\bin\windeployqt.exe  -force  Boids.exe"
REM C:\Qt\5.12.5\msvc2017_64\bin\windeployqt.exe  -force Boids.exe

C:\Qt\5.12.5\msvc2017_64\bin\windeployqt.exe  -force -no-translations Boids.exe


REM CD %dest%
CD %currentdir%
ECHO ***********
ECHO  FINISHED 
ECHO ***********
Pause
REM exit
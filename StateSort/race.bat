rem  The following 3 lines contain paths that my be different on your PC
set VC=C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC
call "%VC%\Auxiliary\Build\vcvars64.bat"
"%VC%\Tools\MSVC\14.29.30133\bin\Hostx64\x64\CL" /O2 race.cpp
DEL race.obj
race
pause
exit

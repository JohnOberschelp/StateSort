rem  The following 3 lines contain paths that my be different on your PC
set VC=C:\Program Files\Microsoft Visual Studio\2022\Community\VC
call "%VC%\Auxiliary\Build\vcvars64.bat"
"%VC%\Tools\MSVC\14.42.34433\bin\Hostx64\x64\CL" /O2 race.cpp
DEL race.obj
race
pause
exit

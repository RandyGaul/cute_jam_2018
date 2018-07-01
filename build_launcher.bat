
if not exist .\bin mkdir .\bin
pushd .\bin
ctime.exe -begin cute_jam.ctm

if not defined DEV_ENV_DIR (
	echo Setting up vcvarsall.bat
	call "C:/Program Files (x86)/Microsoft Visual Studio/2017/Community/VC\Auxiliary/Build/vcvarsall.bat" x64
	REM call "../env.bat"
)
set DEV_ENV_DIR= ???

REM Turn off/on debug console
set CONSOLE= -subsystem:console
REM set CONSOLE= -subsystem:windows

set CFLAGS= -Zi -nologo -I../ -I../src /Fdcute_jam.pdb
set CFLAGS= -D__WINDOWS__ -EHa- -wd4577 %CFLAGS%
set LFLAGS= -incremental:no -opt:ref

set LFLAGS= user32.lib kernel32.lib opengl32.lib gdi32.lib dsound.lib SDL2.lib SDL2main.lib %LFLAGS%

REM cute exe
cl %CFLAGS% ../src/launcher/main.cpp -Fecute_jam.exe /link %LFLAGS% %CONSOLE%

:ENDING
ctime -end cute_jam.ctm
popd

echo Done!

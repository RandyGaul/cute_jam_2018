@echo off

if not exist .\bin mkdir .\bin
pushd .\bin

REM delete old pdbs
del cute_random_*.pdb > NUL 2> NUL

ctime.exe -begin cute.ctm

if not defined DEV_ENV_DIR (
	echo Setting up vcvarsall.bat
	call "C:/Program Files (x86)/Microsoft Visual Studio/2017/Community/VC\Auxiliary/Build/vcvarsall.bat" x64
	REM call "../env.bat"
)
set DEV_ENV_DIR= ???

set CFLAGS= -Zi -nologo -I../ -I../src /Fdcute.pdb
set CFLAGS= -D__WINDOWS__ -EHa- -wd4577 -Ox %CFLAGS%
set LFLAGS= -incremental:no -opt:ref

set LFLAGS= user32.lib kernel32.lib opengl32.lib gdi32.lib dsound.lib SDL2.lib SDL2main.lib %LFLAGS%

REM dll
echo "WAITING FOR PDB ..." > lock.tmp
cl %CFLAGS% ../src/cute_jam/cute.cpp ../glad/glad.c -LD /link -PDB:cute_random_%random%.pdb %LFLAGS%
del lock.tmp

if not "%1" == "" (
	if "%1" == "hotload" GOTO ENDING
)

:ENDING
ctime -end cute.ctm
popd

echo Done!

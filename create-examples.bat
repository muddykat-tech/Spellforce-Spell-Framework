@echo off
cd .\examples\

for /D %%d in (*) do (
	cd %%d
	echo Making in %%d...
	mingw32-make clean
	mingw32-make
	cd ..
)

echo Finished Making Examples Mods
pause
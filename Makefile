CC = g++
RC = windres
DLL_CFLAGS = -O0 -g -std=c++11 ${WARNS} -Iinclude -D ADD_EXPORTS -fpermissive
DLL_LDFLAGS = -shared -Wl,--kill-at -Wl,--subsystem,windows,--out-implib,lib/test_library.a
LOADER_OBJ = obj/test_loader.o
LIBRARY_OBJ =obj/test_library.o

.PHONY: all clean

all: bin/test_loader.exe bin/test_library.dll

clean:
	if exist bin\* del /q bin\*
	if exist lib\* del /q lib\*
	if exist obj\* del /q obj\*

bin lib obj:
	@if not exist "$@" mkdir "$@"

obj/test_loader.o: src/test_loader.c | obj
	${CC} ${DLL_CFLAGS} -c "$<" -o "$@"

obj/test_library.o: src/test_library.c | obj
	${CC} ${DLL_CFLAGS} -c "$<" -o "$@"

bin/test_library.dll: ${LIBRARY_OBJ} | bin lib
	${CC}  -o "$@" ${LIBRARY_OBJ} ${DLL_LDFLAGS}

bin/test_loader.exe: ${LOADER_OBJ} |bin lib
	${CC}  -o "$@" ${LOADER_OBJ}
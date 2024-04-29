CC = g++
RC = windres
DLL_CFLAGS = -O0 -g -std=c++11 ${WARNS} -Iinclude -D ADD_EXPORTS -fpermissive
DLL_LDFLAGS = -shared -Wl,--kill-at -Wl,--subsystem,windows,--out-implib,lib/test_library.a
LOADER_OBJ = obj/test_loader.o
LIBRARY_OBJ_A =obj/test_library_a.o
LIBRARY_OBJ_B =obj/test_library_b.o
LIBRARY_OBJ_C =obj/test_library_c.o

.PHONY: all clean

all: bin/test_loader.exe bin/test_library_a.dll bin/test_library_b.dll bin/test_library_c.dll

clean:
	if exist bin\* del /q bin\*
	if exist lib\* del /q lib\*
	if exist obj\* del /q obj\*

bin lib obj:
	@if not exist "$@" mkdir "$@"

obj/test_loader.o: src/test_loader.c | obj
	${CC} ${DLL_CFLAGS} -c "$<" -o "$@"

obj/test_library_a.o: src/test_library_a.c | obj
	${CC} ${DLL_CFLAGS} -c "$<" -o "$@"

obj/test_library_b.o: src/test_library_b.c | obj
	${CC} ${DLL_CFLAGS} -c "$<" -o "$@"

obj/test_library_c.o: src/test_library_c.c | obj
	${CC} ${DLL_CFLAGS} -c "$<" -o "$@"

bin/test_library_a.dll: ${LIBRARY_OBJ_A} | bin lib
	${CC}  -o "$@" ${LIBRARY_OBJ_A} ${DLL_LDFLAGS}

bin/test_library_b.dll: ${LIBRARY_OBJ_B} | bin lib
	${CC}  -o "$@" ${LIBRARY_OBJ_A} ${DLL_LDFLAGS}

bin/test_library_c.dll:${LIBRARY_OBJ_C} | bin lib
	${CC}  -o "$@" ${LIBRARY_OBJ_A} ${DLL_LDFLAGS}

bin/test_loader.exe: ${LOADER_OBJ} |bin lib
	${CC}  -o "$@" ${LOADER_OBJ}
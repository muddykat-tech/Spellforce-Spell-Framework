CC = g++
RC = windres
DLL_CFLAGS = -O0 -g -std=c++11 ${WARNS} -Iinclude -D ADD_EXPORTS -fpermissive
DLL_LDFLAGS = -shared -Wl,--kill-at -Wl,--subsystem,windows,--out-implib,lib/test_library.a
FW_LDFLAGS = -shared -static-libgcc -static-libstdc++ -s -Wl,-Bstatic,--whole-archive -lwinpthread -Wl,--no-whole-archive -Wl,--subsystem,windows,--out-implib,lib/sfsf.a
LOADER_OBJ = obj/test_loader.o
LIBRARY_OBJ_A = obj/test_library_a.o
LIBRARY_OBJ_B = obj/test_library_b.o
LIBRARY_OBJ_C = obj/test_library_c.o
SFSF_OBJ = obj/sf_asi.o obj/sfsf.o

.PHONY: all clean framework

all: bin/test_loader.exe bin/test_library_a.dll bin/test_library_b.dll bin/test_library_c.dll

framework: bin/sfsf.asi


clean:
	if exist bin\* del /q bin\*
	if exist lib\* del /q lib\*
	if exist obj\* del /q obj\*

bin lib obj:
	@if not exist "$@" mkdir "$@"

obj/sf_asi.o: src/asi/sf_asi.cpp src/asi/sf_asi.h | obj
	${CC} ${DLL_CFLAGS} -c "$<" -o "$@"

obj/sfsf.o: src/SpellforceSpellFramework.c src/asi/sf_asi.h | obj
	${CC} ${DLL_CFLAGS} -c "$<" -o "$@"

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

bin/sfsf.asi: $(SFSF_OBJ) | bin lib
	${CC} -o "$@" ${SFSF_OBJ} ${FW_LDFLAGS}
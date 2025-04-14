CC = g++
DLL_CFLAGS = -O0 -g -std=c++11 ${WARNS} -Iinclude -DADD_EXPORTS -fpermissive
DLL_LDFLAGS = -shared -static-libgcc -static-libstdc++ -s -Wl,-Bstatic,--whole-archive -lwinpthread -Wl,--no-whole-archive -Wl,--subsystem,windows,--out-implib,lib/multiple_auras.a
MOD_OBJ = obj/multiple_auras.o obj/sf_asi.o

.PHONY: all clean 

# Default target
all: bin/multiple_auras.sfm

# Target for cleaning up
clean:
	if exist bin\* del /q bin\*
	if exist lib\* del /q lib\*
	if exist obj\* del /q obj\*

# Target for creating directories
bin lib obj:
	@if not exist "$@" mkdir "$@"

#mod build
obj/multiple_auras.o: multiple_auras.cpp | obj
	${CC} ${DLL_CFLAGS} -c "$<" -o "$@"

obj/sf_asi.o: ../../src/asi/sf_asi.cpp ../../src/api/sfsf.h | obj
	${CC} ${DLL_CFLAGS} -c "$<" -o "$@"

bin/multiple_auras.sfm: ${MOD_OBJ} | bin lib
	${CC} -o "$@" ${MOD_OBJ} ${DLL_LDFLAGS}
CC = g++
DLL_CFLAGS = -O0 -g -std=c++11 ${WARNS} -Iinclude -DADD_EXPORTS -fpermissive
DLL_LDFLAGS = -shared -static-libgcc -static-libstdc++ -s -Wl,-Bstatic,--whole-archive -lwinpthread -Wl,--no-whole-archive -Wl,--subsystem,windows,--out-implib,lib/simple_damage.a
MOD_OBJ = obj/shield_wall.o

.PHONY: all clean 

# Default target
all: bin/shield_wall.sfm

# Target for cleaning up
clean:
	if exist bin\* del /q bin\*
	if exist lib\* del /q lib\*
	if exist obj\* del /q obj\*

# Target for creating directories
bin lib obj:
	@if not exist "$@" mkdir "$@"

#mod build
obj/shield_wall.o: shield_wall.cpp | obj
	${CC} ${DLL_CFLAGS} -c "$<" -o "$@"

bin/shield_wall.sfm: ${MOD_OBJ} | bin lib
	${CC} -o "$@" ${MOD_OBJ} ${DLL_LDFLAGS}
# Compiler and linker options
CC = g++
DLL_CFLAGS = -O0 -g -std=c++11 ${WARNS} -Iinclude -DADD_EXPORTS -fpermissive
INTERNALS_OBJ = obj/dllmain.o obj/sf_hooks.o obj/sf_modloader.o obj/sf_registry.o obj/sf_spellhandler.o
FW_LDFLAGS = -shared -static-libgcc -static-libstdc++ -s -Wl,-Bstatic,--whole-archive -lwinpthread -Wl,--no-whole-archive -Wl,--subsystem,windows,--out-implib,lib/sfsf.a


.PHONY: all clean 

all: bin/sfsf.asi

clean:
	if exist bin\* del /q bin\*
	if exist lib\* del /q lib\*
	if exist obj\* del /q obj\*

# Target for creating directories
bin lib obj:
	@if not exist "$@" mkdir "$@"


obj/dllmain.o: dllmain.cpp | obj
	${CC} ${DLL_CFLAGS} -c "$<" -o "$@"

obj/sf_hooks.o: sf_hooks.cpp | obj
	${CC} ${DLL_CFLAGS} -c "$<" -o "$@"

obj/sf_modloader.o: sf_modloader.c | obj
	${CC} ${DLL_CFLAGS} -c "$<" -o "$@"

obj/sf_registry.o: sf_registry.cpp | obj
	${CC} ${DLL_CFLAGS} -c "$<" -o "$@" 

obj/sf_spellhandler.o: sf_spellhandler.cpp | obj
	${CC} ${DLL_CFLAGS} -c "$<" -o "$@" 

obj/sf_asi.o: ../asi/sf_asi.cpp | obj
	${CC} ${DLL_CFLAGS} -c "$<" -o "$@" 

bin/sfsf.asi: $(INTERNALS_OBJ) | bin lib
	${CC} -o "$@" ${INTERNALS_OBJ} ${FW_LDFLAGS}
	cp "bin/sfsf.asi" "../../bin/sfsf.asi"
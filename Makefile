# Compiler and linker options
CC = g++
RC = windres
DLL_CFLAGS = -O0 -g -std=c++11 ${WARNS} -Iinclude -DADD_EXPORTS -fpermissive
DLL_LDFLAGS = -shared -static-libgcc -static-libstdc++ -s -Wl,-Bstatic,--whole-archive -lwinpthread -Wl,--no-whole-archive -Wl,--subsystem,windows,--out-implib,lib/testmod.a
FW_LDFLAGS = -shared -static-libgcc -static-libstdc++ -s -Wl,-Bstatic,--whole-archive -lwinpthread -Wl,--no-whole-archive -Wl,--subsystem,windows,--out-implib,lib/sfsf.a

# Object files for the new architecture
NTERNALS_OBJ = obj/dllmain.o obj/sf_hooks.o obj/sf_modloader.o obj/sf_registry.o obj/sf_spelltype_handlers.o obj/sf_asi.o obj/sf_utility.o obj/sf_spelltype_registry.o
TEST_MOD_OBJ = obj/TestMod.o
INTERNALS_SRC = src/internal

# Phony targets
.PHONY: all clean mods

# Default target
all: bin/sfsf.asi

# Framework Target
mods: bin/testmod.sfm

# Target for cleaning up
clean:
	if exist bin\* del /q bin\*
	if exist lib\* del /q lib\*
	if exist obj\* del /q obj\*

# Target for creating directories
bin lib obj:
	@if not exist "$@" mkdir "$@"


#internals build
obj/sf_asi.o: src/asi/sf_asi.cpp src/asi/sf_asi.h | obj
	${CC} ${DLL_CFLAGS} -c "$<" -o "$@"

obj/dllmain.o: ${INTERNALS_SRC}/dllmain.cpp src/asi/sf_asi.h | obj
	${CC} ${DLL_CFLAGS} -c "$<" -o "$@"

obj/sf_utility.o: ${INTERNALS_SRC}/sf_utility.c | obj
	${CC} ${DLL_CFLAGS} -c "$<" -o "$@"

obj/sf_hooks.o: ${INTERNALS_SRC}/sf_hooks.c | obj
	${CC} ${DLL_CFLAGS} -c "$<" -o "$@"

obj/sf_modloader.o: ${INTERNALS_SRC}/sf_modloader.c | obj
	${CC} ${DLL_CFLAGS} -c "$<" -o "$@"

obj/sf_registry.o: ${INTERNALS_SRC}/sf_registry.cpp | obj
	${CC} ${DLL_CFLAGS} -c "$<" -o "$@" 

obj/sf_spelltype_handlers.o: ${INTERNALS_SRC}/sf_spelltype_handlers.cpp | obj
	${CC} ${DLL_CFLAGS} -c "$<" -o "$@" 

obj/sf_spelltype_registry.o: ${INTERNALS_SRC}/sf_spelltype_registry.cpp | obj
	${CC} ${DLL_CFLAGS} -c "$<" -o "$@" 

#mod build
obj/TestMod.o: src/dev/TestMod.cpp | obj
	${CC} ${DLL_CFLAGS} -c "$<" -o "$@"

# Target for building DLLs for the new architecture
bin/testmod.sfm: ${TEST_MOD_OBJ} | bin lib
	${CC} -o "$@" ${TEST_MOD_OBJ} ${DLL_LDFLAGS}

# Target for building the Spellforce framework
bin/sfsf.asi: $(NTERNALS_OBJ) | bin lib
	${CC} -o "$@" ${NTERNALS_OBJ} ${FW_LDFLAGS}
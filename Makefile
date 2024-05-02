# Compiler and linker options
CC = g++
RC = windres
DLL_CFLAGS = -O0 -g -std=c++11 ${WARNS} -Iinclude -DADD_EXPORTS -fpermissive
DLL_LDFLAGS = -shared -Wl,--kill-at -Wl,--subsystem,windows,--out-implib,lib/test_library.a
FW_LDFLAGS = -shared -static-libgcc -static-libstdc++ -s -Wl,-Bstatic,--whole-archive -lwinpthread -Wl,--no-whole-archive -Wl,--subsystem,windows,--out-implib,lib/sfsf.a

# Object files for the old architecture
LOADER_OBJ = obj/test_loader.o
LIBRARY_OBJ_A = obj/test_library_a.o
LIBRARY_OBJ_B = obj/test_library_b.o
LIBRARY_OBJ_C = obj/test_library_c.o

# Object files for the new architecture
MODULE_LOADER_OBJ = obj/ModuleLoader.o
TEST_MOD_OBJ = obj/TestMod.o
SFSF_OBJ = obj/sf_asi.o obj/sfsf.o obj/ModuleLoader.o

# Phony targets
.PHONY: all clean framework

# Default target
all: bin/test_loader.exe bin/sfsf/test_library_a.dll bin/sfsf/test_library_b.dll bin/sfsf/test_library_c.dll bin/ModuleLoader.dll bin/testmod.sfm

# Framework Target
framework: bin/sfsf.asi bin/testmod.sfm

# Target for cleaning up
clean:
	if exist bin\* del /q bin\*
	if exist lib\* del /q lib\*
	if exist obj\* del /q obj\*

# Target for creating directories
bin lib obj:
	@if not exist "$@" mkdir "$@"

# Object file rules for the old architecture
obj/test_loader.o: src/test/test_loader.c | obj
	${CC} ${DLL_CFLAGS} -c "$<" -o "$@"

obj/test_library_a.o: src/test/test_library_a.c | obj
	${CC} ${DLL_CFLAGS} -c "$<" -o "$@"

obj/test_library_b.o: src/test/test_library_b.c | obj
	${CC} ${DLL_CFLAGS} -c "$<" -o "$@"

obj/test_library_c.o: src/test/test_library_c.c | obj
	${CC} ${DLL_CFLAGS} -c "$<" -o "$@"

# Object file rules for the new architecture
obj/ModuleLoader.o: src/ModuleLoader.c | obj
	${CC} ${DLL_CFLAGS} -c "$<" -o "$@"

obj/TestMod.o: src/dev/TestMod.c | obj
	${CC} ${DLL_CFLAGS} -c "$<" -o "$@"

obj/sf_asi.o: src/asi/sf_asi.cpp src/asi/sf_asi.h | obj
	${CC} ${DLL_CFLAGS} -c "$<" -o "$@"

obj/sfsf.o: src/SpellforceSpellFramework.c src/asi/sf_asi.h | obj
	${CC} ${DLL_CFLAGS} -c "$<" -o "$@"

# Target for building DLLs for the old architecture
bin/sfsf/test_library_a.dll: ${LIBRARY_OBJ_A} | bin lib
	${CC} -o "$@" ${LIBRARY_OBJ_A} ${DLL_LDFLAGS}

bin/sfsf/test_library_b.dll: ${LIBRARY_OBJ_B} | bin lib
	${CC} -o "$@" ${LIBRARY_OBJ_B} ${DLL_LDFLAGS}

bin/sfsf/test_library_c.dll:${LIBRARY_OBJ_C} | bin lib
	${CC} -o "$@" ${LIBRARY_OBJ_C} ${DLL_LDFLAGS}

# Target for building DLLs for the new architecture
bin/ModuleLoader.dll: ${MODULE_LOADER_OBJ} | bin lib
	${CC} -o "$@" ${MODULE_LOADER_OBJ} ${DLL_LDFLAGS}

bin/testmod.sfm: ${TEST_MOD_OBJ} | bin lib
	${CC} -o "$@" ${TEST_MOD_OBJ} ${DLL_LDFLAGS}

# Target for building the test loader executable
bin/test_loader.exe: ${LOADER_OBJ} | bin lib
	${CC} -o "$@" ${LOADER_OBJ}

# Target for building the Spellforce framework
bin/sfsf.asi: $(SFSF_OBJ) | bin lib
	${CC} -o "$@" ${SFSF_OBJ} ${FW_LDFLAGS}
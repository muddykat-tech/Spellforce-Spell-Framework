# Compiler and linker options
CC = g++
RC = windres
DLL_CFLAGS = -O0 -g -std=c++11 ${WARNS} -Iinclude -DADD_EXPORTS -fpermissive -m32
DLL_LDFLAGS = -m32 -shared -static-libgcc -static-libstdc++ -s -Wl,-Bstatic,--whole-archive -lwinpthread -Wl,--no-whole-archive -Wl,--subsystem,windows,--out-implib,lib/testmod.a
FW_LDFLAGS = -m32 -shared -static-libgcc -static-libstdc++ -s -Wl,-Bstatic,--whole-archive -lwinpthread -Wl,--no-whole-archive -Wl,--subsystem,windows,--out-implib,lib/sfsf.a

# Object files for the new architecture
HOOK_OBJ = obj/sf_hooks.o obj/sf_onhit_hook.o obj/sf_refresh_hook.o obj/sf_endspell_hook.o obj/sf_menu_hook.o obj/sf_spelleffect_hook.o obj/sf_subeffect_hook.o obj/sf_spelltype_hook.o obj/sf_damage_hook.o obj/sf_console_hook.o obj/sf_ai_hook.o obj/sf_utility_hooks.o 
REGISTRY_OBJ = obj/sf_registry.o obj/sf_mod_registry.o obj/sf_spelltype_registry.o obj/sf_spelleffect_registry.o obj/sf_spellend_registry.o obj/sf_subeffect_registry.o obj/sf_spellrefresh_registry.o obj/sf_vanilla_registry.o obj/sf_spelldamage_registry.o obj/sf_onhit_registry.o obj/sf_ai_avoidance_registry.o obj/sf_ai_single_target_registry.o obj/sf_ai_aoe_registry.o
HANDLER_OBJ = obj/sf_spelltype_handlers.o obj/sf_spelleffect_handlers.o obj/sf_spellend_handlers.o obj/sf_sub_effect_handlers.o obj/sf_spellrefresh_handlers.o obj/sf_spelldamage_handlers.o obj/sf_onhit_handlers.o obj/sf_ai_avoidance_handlers.o obj/sf_ai_spell_handlers.o obj/sf_ai_aoe_handlers.o

NTERNALS_OBJ = obj/sfsf.o obj/sf_modloader.o obj/sf_asi.o obj/sf_wrappers.o ${REGISTRY_OBJ} ${HANDLER_OBJ} ${HOOK_OBJ}
TEST_MOD_OBJ = obj/TestMod.o
INTERNALS_SRC = src/internal

CORE_SRC = ${INTERNALS_SRC}/core
HOOKS_SRC = ${CORE_SRC}/hooks
REGISTRY_SRC = ${INTERNALS_SRC}/registry
SPELL_DATA_REGISTRY_SRC = ${REGISTRY_SRC}/spell_data_registries
AI_DATA_REGISTRY_SRC = ${REGISTRY_SRC}/ai_data_registries
HANDLERS_SRC = ${INTERNALS_SRC}/handlers


# Colors
YELLOW_BASH=\033[1;33m
NC_BASH=\033[0m # No Color for Bash

YELLOW_PS=Yellow
NC_PS=Default

# Function to print colored text
define print_colored
	$(if $(findstring bash,$(SHELL)),\
		@echo -e "$(YELLOW_BASH)$(1)$(NC_BASH)",\
		$(if $(findstring cmd.exe,$(ComSpec)),\
			@powershell -NoProfile -Command "Write-Host -ForegroundColor $(YELLOW_PS) '$(1)'",\
			@powershell -NoProfile -Command "Write-Host -ForegroundColor $(YELLOW_PS) '$(1)'"))
endef

# Check if running in PowerShell
# Phony targets
.PHONY: all clean mods

# Default target
all: bin/sfsf.asi

# Framework Target
mods: bin/testmod.sfm

clean:
	$(call print_colored, "Cleaning Files")
	rm -rf bin/* lib/* obj/*

cln:
	$(call print_colored, "Cleaning Files")
	if exist bin\* del /q bin\*
	if exist lib\* del /q lib\*
	if exist obj\* del /q obj\*

# Target for creating directories
bin lib obj:
	@if [ ! -d "$@" ]; then mkdir -p "$@"; fi

# Internals build
obj/sf_asi.o: src/asi/sf_asi.cpp src/asi/sf_asi.h | obj
	${CC} ${DLL_CFLAGS} -c "$<" -o "$@"
	
obj/sfsf.o: ${INTERNALS_SRC}/sfsf.cpp src/asi/sf_asi.h | obj
	$(call print_colored, "================== Starting Build ==================")
	${CC} ${DLL_CFLAGS} -c "$<" -o "$@"

obj/sf_wrappers.o: ${CORE_SRC}/sf_wrappers.c | obj
	${CC} ${DLL_CFLAGS} -c "$<" -o "$@"

# Hooks
obj/sf_ai_hook.o: ${HOOKS_SRC}/sf_ai_hook.c | 
	${CC} ${DLL_CFLAGS} -c "$<" -o "$@"

obj/sf_onhit_hook.o: ${HOOKS_SRC}/sf_onhit_hook.c | obj
	${CC} ${DLL_CFLAGS} -c "$<" -o "$@"

obj/sf_refresh_hook.o: ${HOOKS_SRC}/sf_refresh_hook.c | obj
	${CC} ${DLL_CFLAGS} -c "$<" -o "$@"

obj/sf_endspell_hook.o: ${HOOKS_SRC}/sf_endspell_hook.c | obj
	${CC} ${DLL_CFLAGS} -c "$<" -o "$@"

obj/sf_menu_hook.o: ${HOOKS_SRC}/sf_menu_hook.c | obj
	${CC} -mgeneral-regs-only ${DLL_CFLAGS} -c "$<" -o "$@"

obj/sf_spelleffect_hook.o: ${HOOKS_SRC}/sf_spelleffect_hook.c | obj
	${CC} ${DLL_CFLAGS} -c "$<" -o "$@"

obj/sf_subeffect_hook.o: ${HOOKS_SRC}/sf_subeffect_hook.c | obj
	${CC} ${DLL_CFLAGS} -c "$<" -o "$@"

obj/sf_spelltype_hook.o: ${HOOKS_SRC}/sf_spelltype_hook.c | obj
	${CC} ${DLL_CFLAGS} -c "$<" -o "$@"

obj/sf_damage_hook.o: ${HOOKS_SRC}/sf_damage_hook.c | obj
	${CC} -mgeneral-regs-only ${DLL_CFLAGS} -c "$<" -o "$@"

obj/sf_console_hook.o: ${HOOKS_SRC}/sf_console_hook.c | obj
	${CC} ${DLL_CFLAGS} -c "$<" -o "$@"

obj/sf_utility_hooks.o: ${HOOKS_SRC}/sf_utility_hooks.c | obj
	${CC} ${DLL_CFLAGS} -c "$<" -o "$@"

obj/sf_hooks.o: ${CORE_SRC}/sf_hooks.c src/asi/sf_asi.h | obj
	$(call print_colored, "================== Building Hooks ==================")
	${CC} ${DLL_CFLAGS} -c "$<" -o "$@"

# Mod Loader
obj/sf_modloader.o: ${CORE_SRC}/sf_modloader.c | obj
	$(call print_colored, "================== Building Mod Loader ==================")
	${CC} ${DLL_CFLAGS} -c "$<" -o "$@"

# Registry and Handlers
obj/sf_registry.o: ${REGISTRY_SRC}/sf_registry.cpp | obj
	$(call print_colored, "================== Building Registries ==================")
	${CC} ${DLL_CFLAGS} -c "$<" -o "$@" 

obj/sf_mod_registry.o: ${REGISTRY_SRC}/sf_mod_registry.cpp | obj
	${CC} ${DLL_CFLAGS} -c "$<" -o "$@" 

obj/sf_spelltype_handlers.o: ${HANDLERS_SRC}/sf_spelltype_handlers.cpp | obj
	$(call print_colored, "================== Building Handlers ==================")
	${CC} ${DLL_CFLAGS} -c "$<" -o "$@" 

obj/sf_ai_avoidance_handlers.o: ${HANDLERS_SRC}/sf_ai_avoidance_handlers.cpp | obj
	${CC} ${DLL_CFLAGS} -c "$<" -o "$@" 

obj/sf_onhit_handlers.o: ${HANDLERS_SRC}/sf_onhit_handlers.cpp | obj
	${CC} ${DLL_CFLAGS} -c "$<" -o "$@" 

obj/sf_spelltype_registry.o: ${SPELL_DATA_REGISTRY_SRC}/sf_spelltype_registry.cpp | obj
	${CC} ${DLL_CFLAGS} -c "$<" -o "$@" 

obj/sf_spelleffect_handlers.o: ${HANDLERS_SRC}/sf_spelleffect_handlers.cpp | obj
	${CC} ${DLL_CFLAGS} -c "$<" -o "$@" 

obj/sf_spelleffect_registry.o: ${SPELL_DATA_REGISTRY_SRC}/sf_spelleffect_registry.cpp | obj
	${CC} ${DLL_CFLAGS} -c "$<" -o "$@" 

obj/sf_onhit_registry.o: ${SPELL_DATA_REGISTRY_SRC}/sf_onhit_registry.cpp | obj
	${CC} ${DLL_CFLAGS} -c "$<" -o "$@" 

obj/sf_spellend_handlers.o: ${HANDLERS_SRC}/sf_spellend_handlers.cpp | obj
	${CC} ${DLL_CFLAGS} -c "$<" -o "$@"

obj/sf_sub_effect_handlers.o: ${HANDLERS_SRC}/sf_sub_effect_handlers.cpp | obj
	${CC} ${DLL_CFLAGS} -c "$<" -o "$@"

obj/sf_spellend_registry.o: ${SPELL_DATA_REGISTRY_SRC}/sf_spellend_registry.cpp | obj
	${CC} ${DLL_CFLAGS} -c "$<" -o "$@" 

obj/sf_subeffect_registry.o: ${SPELL_DATA_REGISTRY_SRC}/sf_subeffect_registry.cpp | obj
	${CC} ${DLL_CFLAGS} -c "$<" -o "$@"

obj/sf_spellrefresh_handlers.o: ${HANDLERS_SRC}/sf_spellrefresh_handlers.cpp | obj
	${CC} ${DLL_CFLAGS} -c "$<" -o "$@"

obj/sf_spellrefresh_registry.o: ${SPELL_DATA_REGISTRY_SRC}/sf_spellrefresh_registry.cpp | obj
	${CC} ${DLL_CFLAGS} -c "$<" -o "$@"

obj/sf_vanilla_registry.o: ${REGISTRY_SRC}/sf_vanilla_registry.cpp | obj
	${CC} ${DLL_CFLAGS} -c "$<" -o "$@"

obj/sf_spelldamage_handlers.o: ${HANDLERS_SRC}/sf_spelldamage_handlers.cpp | obj
	${CC} ${DLL_CFLAGS} -c "$<" -o "$@" 

obj/sf_spelldamage_registry.o: ${SPELL_DATA_REGISTRY_SRC}/sf_spelldamage_registry.cpp | obj
	${CC} ${DLL_CFLAGS} -c "$<" -o "$@" 

obj/sf_ai_spell_handlers.o: ${HANDLERS_SRC}/sf_ai_spell_handlers.cpp | obj
	${CC} ${DLL_CFLAGS} -c "$<" -o "$@" 

obj/sf_ai_aoe_handlers.o: ${HANDLERS_SRC}/sf_ai_aoe_handlers.cpp | obj
	${CC} ${DLL_CFLAGS} -c "$<" -o "$@" 

obj/sf_ai_avoidance_registry.o: ${AI_DATA_REGISTRY_SRC}/sf_ai_avoidance_registry.cpp | obj
	${CC} ${DLL_CFLAGS} -c "$<" -o "$@" 

obj/sf_ai_single_target_registry.o: ${AI_DATA_REGISTRY_SRC}/sf_ai_single_target_registry.cpp | obj
	${CC} ${DLL_CFLAGS} -c "$<" -o "$@" 

obj/sf_ai_aoe_registry.o: ${AI_DATA_REGISTRY_SRC}/sf_ai_aoe_registry.cpp | obj
	${CC} ${DLL_CFLAGS} -c "$<" -o "$@" 

# Mod build
obj/TestMod.o: src/dev/TestMod.cpp | obj
	${CC} ${DLL_CFLAGS} -c "$<" -o "$@"

# Target for building DLLs for the new architecture
bin/testmod.sfm: ${TEST_MOD_OBJ} | bin lib
	${CC} -o "$@" ${TEST_MOD_OBJ} ${DLL_LDFLAGS}

# Target for building the Spellforce framework
bin/sfsf.asi: $(NTERNALS_OBJ) | bin lib
	${CC} -o "$@" ${NTERNALS_OBJ} ${FW_LDFLAGS}

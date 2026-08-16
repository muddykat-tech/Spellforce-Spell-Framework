# |-========================================================================-|
# |  Spellforce Spell Framework - build                                      |
# |                                                                          |
# |  Works from a POSIX shell (Linux, MSYS2/MinGW bash) and from cmd.exe.    |
# |  The shell is detected below; everything that differs between the two    |
# |  goes through the MKDIR / RMTREE / print_colored helpers.                |
# |-========================================================================-|

# |-========== Host shell detection ==========-|
# Do NOT test $(SHELL) here: mingw32-make reports /bin/sh even when no sh.exe
# exists, then runs recipes directly through CreateProcess. Probe the behaviour
# instead - cmd.exe echoes the quotes back, a POSIX shell strips them.
#
# Override when the probe guesses wrong:
#   mingw32-make WINDOWS_CMD=1     force the cmd.exe recipes
#   make WINDOWS_CMD=              force the POSIX recipes
ifeq ($(OS),Windows_NT)
  ifneq ($(shell echo "probe"),probe)
    WINDOWS_CMD := 1
  endif
endif

ifdef WINDOWS_CMD
  MKDIR  = @if not exist "$(1)" mkdir "$(1)"
  RMTREE = @if exist "$(1)" rmdir /s /q "$(1)"
  define print_colored
	@powershell -NoProfile -Command "Write-Host -ForegroundColor Yellow '$(subst ",,$(strip $(1)))'"
  endef
else
  MKDIR  = @mkdir -p "$(1)"
  RMTREE = @rm -rf "$(1)"
  define print_colored
	@printf '\033[1;33m%s\033[0m\n' '$(subst ",,$(strip $(1)))'
  endef
endif

# |-========== Toolchain ==========-|
CC = g++
RC = windres

DLL_CFLAGS = -Wall -O1 -g -std=c++11 ${WARNS} -Iinclude -DADD_EXPORTS -fpermissive -m32

# Naked trampolines and any recipe that must not have SSE registers clobbered.
ASM_CFLAGS = -mgeneral-regs-only

# Import libraries needed by the shader module. opengl32 supplies the GL 1.1
# entry points and wglGetProcAddress; gdi32 supplies SwapBuffers. Both are
# MinGW import archives, so listing them after -Bstatic is harmless.
GL_LIBS = -lopengl32 -lgdi32

COMMON_LDFLAGS = -m32 -shared -lgcc -static-libgcc -static-libstdc++ \
                 -Wl,-Bstatic,--whole-archive -lwinpthread -Wl,--no-whole-archive

DLL_LDFLAGS = ${COMMON_LDFLAGS} -Wl,--subsystem,windows,--out-implib,lib/testmod.a
FW_LDFLAGS  = ${COMMON_LDFLAGS} ${GL_LIBS} -Wl,--subsystem,windows,--out-implib,lib/sfsf.a

# |-========== Source trees ==========-|
INTERNALS_SRC           = src/internal
CORE_SRC                = ${INTERNALS_SRC}/core
HOOKS_SRC               = ${CORE_SRC}/hooks
REGISTRY_SRC            = ${INTERNALS_SRC}/registry
SPELL_DATA_REGISTRY_SRC = ${REGISTRY_SRC}/spell_data_registries
AI_DATA_REGISTRY_SRC    = ${REGISTRY_SRC}/ai_data_registries
BUILDING_REGISTRY_SRC   = ${REGISTRY_SRC}/building_registry
HANDLERS_SRC            = ${INTERNALS_SRC}/handlers

# |-========== Objects ==========-|
HOOK_OBJ = obj/sf_hooks.o obj/sf_onhit_hook.o obj/sf_campaign_hook.o \
           obj/sf_prepare_new_game.o obj/sf_refresh_hook.o obj/sf_endspell_hook.o \
           obj/sf_menu_hook.o obj/sf_spelleffect_hook.o obj/sf_subeffect_hook.o \
           obj/sf_spelltype_hook.o obj/sf_damage_hook.o obj/sf_console_hook.o \
           obj/sf_ai_hook.o obj/sf_utility_hooks.o obj/sf_vanilla_fix_hook.o \
           obj/sf_building_done_hook.o obj/sf_building_entry_hook.o \
           obj/sf_worker_logic_hook.o obj/sf_phys_effect_hook.o \
           obj/sf_enchant_hook.o obj/sf_effect_hook.o obj/sf_shader_hook.o

REGISTRY_OBJ = obj/sf_registry.o obj/sf_mod_registry.o obj/sf_error_registry.o \
               obj/sf_spelltype_registry.o obj/sf_spelleffect_registry.o \
               obj/sf_spellend_registry.o obj/sf_subeffect_registry.o \
               obj/sf_spellrefresh_registry.o obj/sf_vanilla_registry.o \
               obj/sf_spelldamage_registry.o obj/sf_onhit_registry.o \
               obj/sf_ai_avoidance_registry.o obj/sf_ai_single_target_registry.o \
               obj/sf_ai_aoe_registry.o obj/sf_phys_effect_registry.o \
               obj/sf_building_done_registry.o obj/sf_building_entry_registry.o \
               obj/sf_enchant_registry.o

HANDLER_OBJ = obj/sf_spelltype_handlers.o obj/sf_spelleffect_handlers.o \
              obj/sf_spellend_handlers.o obj/sf_sub_effect_handlers.o \
              obj/sf_spellrefresh_handlers.o obj/sf_spelldamage_handlers.o \
              obj/sf_onhit_handlers.o obj/sf_ai_avoidance_handlers.o \
              obj/sf_ai_spell_handlers.o obj/sf_ai_aoe_handlers.o \
              obj/sf_building_done_handlers.o \
              obj/sf_worker_building_entry_handlers.o obj/sf_phys_effect_handlers.o

CORE_OBJ = obj/sfsf.o obj/sf_modloader.o obj/sf_asi.o obj/sf_wrappers.o \
           obj/sf_ui_wrappers.o obj/sf_building_loader.o \
           obj/sf_screens_loader.o obj/sf_screens_module.o \
           obj/sf_campaign_loader.o obj/sf_campaign_module.o \
           obj/sf_shader_loader.o obj/sf_shader_module.o

# Spelling kept from the original so existing scripts referencing it still work.
NTERNALS_OBJ = ${CORE_OBJ} ${REGISTRY_OBJ} ${HANDLER_OBJ} ${HOOK_OBJ}
INTERNALS_OBJ = ${NTERNALS_OBJ}

TEST_MOD_OBJ = obj/TestMod.o

# |-========== Targets ==========-|
.PHONY: all mods clean cln

all: bin/sfsf.asi

mods: bin/testmod.sfm

clean:
	$(call print_colored, "Cleaning Files")
	$(call RMTREE,bin)
	$(call RMTREE,lib)
	$(call RMTREE,obj)

# Kept as an alias: `cln` was the cmd.exe-only clean before the shell
# detection above made a single target work everywhere.
cln: clean

bin lib obj:
	$(call MKDIR,$@)

# |-========== Core ==========-|
obj/sf_asi.o: src/asi/sf_asi.cpp src/asi/sf_asi.h | obj
	${CC} ${DLL_CFLAGS} -c "$<" -o "$@"

obj/sfsf.o: ${INTERNALS_SRC}/sfsf.cpp src/asi/sf_asi.h | obj
	$(call print_colored, "================== Starting Build ==================")
	${CC} ${DLL_CFLAGS} -c "$<" -o "$@"

obj/sf_wrappers.o: ${CORE_SRC}/sf_wrappers.c | obj
	${CC} ${DLL_CFLAGS} -c "$<" -o "$@"

obj/sf_ui_wrappers.o: ${CORE_SRC}/sf_ui_wrappers.c | obj
	${CC} ${DLL_CFLAGS} -c "$<" -o "$@"

obj/sf_building_loader.o: ${CORE_SRC}/sf_building_loader.c ${CORE_SRC}/sf_building_loader.h ${CORE_SRC}/jsmn.h | obj
	${CC} ${DLL_CFLAGS} -c "$<" -o "$@"

obj/sf_screens_loader.o: ${CORE_SRC}/sf_screens_loader.c ${CORE_SRC}/sf_screens_loader.h ${CORE_SRC}/jsmn.h | obj
	${CC} ${DLL_CFLAGS} -c "$<" -o "$@"

obj/sf_screens_module.o: ${CORE_SRC}/sf_screens_module.c ${CORE_SRC}/sf_screens_module.h ${CORE_SRC}/sf_screens_loader.h | obj
	${CC} ${DLL_CFLAGS} -c "$<" -o "$@"

obj/sf_campaign_loader.o: ${CORE_SRC}/sf_campaign_loader.c ${CORE_SRC}/sf_campaign_loader.h ${CORE_SRC}/jsmn.h | obj
	${CC} ${DLL_CFLAGS} -c "$<" -o "$@"

obj/sf_campaign_module.o: ${CORE_SRC}/sf_campaign_module.c ${CORE_SRC}/sf_campaign_module.h ${CORE_SRC}/sf_campaign_loader.h | obj
	${CC} ${DLL_CFLAGS} -c "$<" -o "$@"

obj/sf_shader_loader.o: ${CORE_SRC}/sf_shader_loader.c ${CORE_SRC}/sf_shader_loader.h ${CORE_SRC}/jsmn.h | obj
	${CC} ${DLL_CFLAGS} -c "$<" -o "$@"

obj/sf_shader_module.o: ${CORE_SRC}/sf_shader_module.c ${CORE_SRC}/sf_shader_module.h ${CORE_SRC}/sf_shader_loader.h | obj
	${CC} ${DLL_CFLAGS} -c "$<" -o "$@"

# |-========== Mod loader ==========-|
obj/sf_modloader.o: ${CORE_SRC}/sf_modloader.c | obj
	$(call print_colored, "================== Building Mod Loader ==================")
	${CC} ${DLL_CFLAGS} -c "$<" -o "$@"

# |-========== Hooks ==========-|
obj/sf_hooks.o: ${CORE_SRC}/sf_hooks.c src/asi/sf_asi.h | obj
	$(call print_colored, "================== Building Hooks ==================")
	${CC} ${DLL_CFLAGS} -c "$<" -o "$@"

# ${ASM_CFLAGS}: recipes containing naked trampolines.
obj/sf_enchant_hook.o: ${HOOKS_SRC}/sf_enchant_hook.c | obj
	${CC} ${ASM_CFLAGS} ${DLL_CFLAGS} -c "$<" -o "$@"

obj/sf_menu_hook.o: ${HOOKS_SRC}/sf_menu_hook.c | obj
	${CC} ${ASM_CFLAGS} ${DLL_CFLAGS} -c "$<" -o "$@"

obj/sf_damage_hook.o: ${HOOKS_SRC}/sf_damage_hook.c | obj
	${CC} ${ASM_CFLAGS} ${DLL_CFLAGS} -c "$<" -o "$@"

obj/sf_ai_hook.o: ${HOOKS_SRC}/sf_ai_hook.c | obj
	${CC} ${DLL_CFLAGS} -c "$<" -o "$@"

obj/sf_onhit_hook.o: ${HOOKS_SRC}/sf_onhit_hook.c | obj
	${CC} ${DLL_CFLAGS} -c "$<" -o "$@"

obj/sf_campaign_hook.o: ${HOOKS_SRC}/sf_campaign_hook.c | obj
	${CC} ${DLL_CFLAGS} -c "$<" -o "$@"

obj/sf_prepare_new_game.o: ${HOOKS_SRC}/sf_prepare_new_game.c | obj
	${CC} ${DLL_CFLAGS} -c "$<" -o "$@"

obj/sf_refresh_hook.o: ${HOOKS_SRC}/sf_refresh_hook.c | obj
	${CC} ${DLL_CFLAGS} -c "$<" -o "$@"

obj/sf_endspell_hook.o: ${HOOKS_SRC}/sf_endspell_hook.c | obj
	${CC} ${DLL_CFLAGS} -c "$<" -o "$@"

obj/sf_spelleffect_hook.o: ${HOOKS_SRC}/sf_spelleffect_hook.c | obj
	${CC} ${DLL_CFLAGS} -c "$<" -o "$@"

obj/sf_subeffect_hook.o: ${HOOKS_SRC}/sf_subeffect_hook.c | obj
	${CC} ${DLL_CFLAGS} -c "$<" -o "$@"

obj/sf_vanilla_fix_hook.o: ${HOOKS_SRC}/sf_vanilla_fix_hook.c | obj
	${CC} ${DLL_CFLAGS} -c "$<" -o "$@"

obj/sf_spelltype_hook.o: ${HOOKS_SRC}/sf_spelltype_hook.c | obj
	${CC} ${DLL_CFLAGS} -c "$<" -o "$@"

obj/sf_console_hook.o: ${HOOKS_SRC}/sf_console_hook.c | obj
	${CC} ${DLL_CFLAGS} -c "$<" -o "$@"

obj/sf_utility_hooks.o: ${HOOKS_SRC}/sf_utility_hooks.c | obj
	${CC} ${DLL_CFLAGS} -c "$<" -o "$@"

obj/sf_building_done_hook.o: ${HOOKS_SRC}/sf_building_done_hook.c | obj
	${CC} ${DLL_CFLAGS} -c "$<" -o "$@"

obj/sf_building_entry_hook.o: ${HOOKS_SRC}/sf_building_entry_hook.c | obj
	${CC} ${DLL_CFLAGS} -c "$<" -o "$@"

obj/sf_worker_logic_hook.o: ${HOOKS_SRC}/sf_worker_logic_hook.c | obj
	${CC} ${DLL_CFLAGS} -c "$<" -o "$@"

obj/sf_phys_effect_hook.o: ${HOOKS_SRC}/sf_phys_effect_hook.c | obj
	${CC} ${DLL_CFLAGS} -c "$<" -o "$@"

obj/sf_effect_hook.o: ${HOOKS_SRC}/sf_effect_hook.c | obj
	${CC} ${DLL_CFLAGS} -c "$<" -o "$@"

# No ${ASM_CFLAGS} here: this hook is plain C passing floats to glUniform*,
# not a naked trampoline, and that flag would block the SSE moves GCC needs.
obj/sf_shader_hook.o: ${HOOKS_SRC}/sf_shader_hook.c ${HOOKS_SRC}/sf_shader_hook.h ${CORE_SRC}/sf_shader_module.h | obj
	${CC} ${DLL_CFLAGS} -c "$<" -o "$@"

# |-========== Registries ==========-|
obj/sf_registry.o: ${REGISTRY_SRC}/sf_registry.cpp | obj
	$(call print_colored, "================== Building Registries ==================")
	${CC} ${DLL_CFLAGS} -c "$<" -o "$@"

obj/sf_mod_registry.o: ${REGISTRY_SRC}/sf_mod_registry.cpp | obj
	${CC} ${DLL_CFLAGS} -c "$<" -o "$@"

obj/sf_error_registry.o: ${REGISTRY_SRC}/sf_error_registry.cpp ${REGISTRY_SRC}/sf_error_registry.h | obj
	${CC} ${DLL_CFLAGS} -c "$<" -o "$@"

obj/sf_vanilla_registry.o: ${REGISTRY_SRC}/sf_vanilla_registry.cpp | obj
	${CC} ${DLL_CFLAGS} -c "$<" -o "$@"

obj/sf_spelltype_registry.o: ${SPELL_DATA_REGISTRY_SRC}/sf_spelltype_registry.cpp | obj
	${CC} ${DLL_CFLAGS} -c "$<" -o "$@"

obj/sf_spelleffect_registry.o: ${SPELL_DATA_REGISTRY_SRC}/sf_spelleffect_registry.cpp | obj
	${CC} ${DLL_CFLAGS} -c "$<" -o "$@"

obj/sf_onhit_registry.o: ${SPELL_DATA_REGISTRY_SRC}/sf_onhit_registry.cpp | obj
	${CC} ${DLL_CFLAGS} -c "$<" -o "$@"

obj/sf_spellend_registry.o: ${SPELL_DATA_REGISTRY_SRC}/sf_spellend_registry.cpp | obj
	${CC} ${DLL_CFLAGS} -c "$<" -o "$@"

obj/sf_subeffect_registry.o: ${SPELL_DATA_REGISTRY_SRC}/sf_subeffect_registry.cpp | obj
	${CC} ${DLL_CFLAGS} -c "$<" -o "$@"

obj/sf_spellrefresh_registry.o: ${SPELL_DATA_REGISTRY_SRC}/sf_spellrefresh_registry.cpp | obj
	${CC} ${DLL_CFLAGS} -c "$<" -o "$@"

obj/sf_spelldamage_registry.o: ${SPELL_DATA_REGISTRY_SRC}/sf_spelldamage_registry.cpp | obj
	${CC} ${DLL_CFLAGS} -c "$<" -o "$@"

obj/sf_phys_effect_registry.o: ${SPELL_DATA_REGISTRY_SRC}/sf_phys_effect_registry.cpp | obj
	${CC} ${DLL_CFLAGS} -c "$<" -o "$@"

obj/sf_enchant_registry.o: ${SPELL_DATA_REGISTRY_SRC}/sf_enchant_registry.cpp | obj
	${CC} ${DLL_CFLAGS} -c "$<" -o "$@"

obj/sf_ai_avoidance_registry.o: ${AI_DATA_REGISTRY_SRC}/sf_ai_avoidance_registry.cpp | obj
	${CC} ${DLL_CFLAGS} -c "$<" -o "$@"

obj/sf_ai_single_target_registry.o: ${AI_DATA_REGISTRY_SRC}/sf_ai_single_target_registry.cpp | obj
	${CC} ${DLL_CFLAGS} -c "$<" -o "$@"

obj/sf_ai_aoe_registry.o: ${AI_DATA_REGISTRY_SRC}/sf_ai_aoe_registry.cpp | obj
	${CC} ${DLL_CFLAGS} -c "$<" -o "$@"

obj/sf_building_done_registry.o: ${BUILDING_REGISTRY_SRC}/sf_building_done_registry.cpp | obj
	${CC} ${DLL_CFLAGS} -c "$<" -o "$@"

obj/sf_building_entry_registry.o: ${BUILDING_REGISTRY_SRC}/sf_building_entry_registry.cpp | obj
	${CC} ${DLL_CFLAGS} -c "$<" -o "$@"

# |-========== Handlers ==========-|
obj/sf_spelltype_handlers.o: ${HANDLERS_SRC}/sf_spelltype_handlers.cpp | obj
	$(call print_colored, "================== Building Handlers ==================")
	${CC} ${DLL_CFLAGS} -c "$<" -o "$@"

obj/sf_spelleffect_handlers.o: ${HANDLERS_SRC}/sf_spelleffect_handlers.cpp | obj
	${CC} ${DLL_CFLAGS} -c "$<" -o "$@"

obj/sf_spellend_handlers.o: ${HANDLERS_SRC}/sf_spellend_handlers.cpp | obj
	${CC} ${DLL_CFLAGS} -c "$<" -o "$@"

obj/sf_sub_effect_handlers.o: ${HANDLERS_SRC}/sf_sub_effect_handlers.cpp | obj
	${CC} ${DLL_CFLAGS} -c "$<" -o "$@"

obj/sf_spellrefresh_handlers.o: ${HANDLERS_SRC}/sf_spellrefresh_handlers.cpp | obj
	${CC} ${DLL_CFLAGS} -c "$<" -o "$@"

obj/sf_spelldamage_handlers.o: ${HANDLERS_SRC}/sf_spelldamage_handlers.cpp | obj
	${CC} ${DLL_CFLAGS} -c "$<" -o "$@"

obj/sf_onhit_handlers.o: ${HANDLERS_SRC}/sf_onhit_handlers.cpp | obj
	${CC} ${DLL_CFLAGS} -c "$<" -o "$@"

obj/sf_phys_effect_handlers.o: ${HANDLERS_SRC}/sf_phys_effect_handlers.cpp | obj
	${CC} ${DLL_CFLAGS} -c "$<" -o "$@"

obj/sf_ai_avoidance_handlers.o: ${HANDLERS_SRC}/sf_ai_avoidance_handlers.cpp | obj
	${CC} ${DLL_CFLAGS} -c "$<" -o "$@"

obj/sf_ai_spell_handlers.o: ${HANDLERS_SRC}/sf_ai_spell_handlers.cpp | obj
	${CC} ${DLL_CFLAGS} -c "$<" -o "$@"

obj/sf_ai_aoe_handlers.o: ${HANDLERS_SRC}/sf_ai_aoe_handlers.cpp | obj
	${CC} ${DLL_CFLAGS} -c "$<" -o "$@"

obj/sf_building_done_handlers.o: ${HANDLERS_SRC}/sf_building_done_handlers.cpp | obj
	${CC} ${DLL_CFLAGS} -c "$<" -o "$@"

obj/sf_worker_building_entry_handlers.o: ${HANDLERS_SRC}/sf_worker_building_entry_handlers.cpp | obj
	${CC} ${DLL_CFLAGS} -c "$<" -o "$@"

# |-========== Mods ==========-|
obj/TestMod.o: src/dev/TestMod.cpp | obj
	${CC} ${DLL_CFLAGS} -c "$<" -o "$@"

# |-========== Link ==========-|
# NOTE: unchanged from the original, which links the framework objects rather
# than ${TEST_MOD_OBJ} and uses ${FW_LDFLAGS} rather than ${DLL_LDFLAGS}.
# Left alone deliberately - fixing it changes what this target produces.
bin/testmod.sfm: ${TEST_MOD_OBJ} | bin lib
	${CC} -g -o "$@" ${NTERNALS_OBJ} ${FW_LDFLAGS}

bin/sfsf.asi: ${NTERNALS_OBJ} | bin lib
	${CC} -g -Wl,--export-all-symbols -o "$@" ${NTERNALS_OBJ} ${FW_LDFLAGS}

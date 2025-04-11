/**
 * @defgroup API API Documentation
 */

#pragma once
#include "sf_general_structures.h"
#include "sf_figure_functions.h"
#include "sf_iterator_functions.h"
#include "sf_spell_functions.h"
#include "sf_effect_functions.h"
#include "sf_toolbox_functions.h"
#include "sf_registration_functions.h"
#include "sf_ai_functions.h"

#include <stdint.h>

typedef void (*initializeModule_ptr)(void *);
typedef SFMod *(*registerMod_ptr)(void *);
typedef SFMod *(*createModInfo_ptr)(const char *mod_id, const char *mod_version,
                                    const char *mod_author,
                                    const char *mod_description);

typedef void (*logInfoFunc)(const char *);
typedef void (__thiscall *handler_ptr)(SF_CGdSpell *, uint16_t);
typedef int (__thiscall *refresh_handler_ptr)(SF_CGdSpell *, uint16_t);

/**
 * @brief Represents the Spellforce Spell Framework API.
 * @ingroup API
 * This structure serves as the central interface for interacting with
 * various subsystems of the Spellforce Spell Framework. Each member
 * corresponds to a specific API or function group, providing access
 * to the relevant functionality.
 */
struct SpellforceSpellFramework
{
    /**
     * @brief Pointer to the function responsible for creating mod information.
     */
    createModInfo_ptr createModInfo;

    /**
     * @brief Interface for spell-related functions.
     *
     * Provides utilities for managing and manipulating spells within the framework.
     */
    SpellFunctions *spellAPI;

    /**
     * @brief Interface for toolbox-related functions.
     *
     * Offers general-purpose tools and utilities to assist with framework operations.
     */
    ToolboxFunctions *toolboxAPI;

    /**
     * @brief Interface for figure-related functions.
     * Facilitates interactions with figures (e.g., npc, units).
     */
    FigureFunctions *figureAPI;

    /**
     * @brief Interface for iterator-related functions.
     *
     * Provides functionality for managing and navigating collections or sequences of objects.
     */
    IteratorFunctions *iteratorAPI;

    /**
     * @brief Interface for registration-related functions.
     *
     * Handles the registration of new spells within the framework.
     */
    RegistrationFunctions *registrationAPI;

    /**
     * @brief Interface for spell effect-related functions.
     * Provides access to functions that allow setting effect related data.
     */
    EffectFunctions *effectAPI;

    /**
     * @brief Interface for AI-related functions.
     *
     * Provides tools for interacting with AI systems.
     */
    AiFunctions *aiAPI;

    /**
     * @brief Interface for logging functionality.
     * Enables logging of information, warnings, and errors to assist in debugging and monitoring.
     */
    SFLog *logAPI;
};


typedef struct SpellforceSpellFramework FrameworkAPI;
typedef struct SpellforceSpellFramework *pFrameworkAPI;

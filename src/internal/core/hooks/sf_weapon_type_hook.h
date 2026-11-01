/**
 * @addtogroup WeaponTypeHook
 * @{
 */

#pragma once

/**
 * @brief Installs the whole-function SelectJobAnimation replacement.
 *
 * Called from initialize_vanilla_fix_hooks(). Redirects the single call site
 * in CDrwGdFigure::UpdateAnims; the original body at 0x7DB5F0 stays
 * byte-for-byte untouched.
 */
void install_select_job_animation_hook();

/** @} */

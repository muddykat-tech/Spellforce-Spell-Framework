<div align="center">

# Spellforce Spell Framework

**A native modding framework for SpellForce 1: Platinum Edition that supports new spells, buildings, campaigns, and UI, all in C/C++ without touching the game’s binary.**

[![Latest Release](https://img.shields.io/github/v/release/muddykat-tech/Spellforce-Spell-Framework?include_prereleases&label=release&color=blue)](https://github.com/muddykat-tech/Spellforce-Spell-Framework/releases)
[![Build](https://github.com/muddykat-tech/Spellforce-Spell-Framework/actions/workflows/c-release.yml/badge.svg)](https://github.com/muddykat-tech/Spellforce-Spell-Framework/actions/workflows/c-release.yml)
[![Downloads](https://img.shields.io/github/downloads-pre/muddykat-tech/Spellforce-Spell-Framework/latest/total?label=downloads)](https://github.com/muddykat-tech/Spellforce-Spell-Framework/releases/latest)
[![License: GPL v3](https://img.shields.io/badge/license-GPLv3-green)](LICENSE)
[![Maintained](https://img.shields.io/maintenance/yes/2026)](https://github.com/muddykat-tech/Spellforce-Spell-Framework/commits)

[![Documentation](https://img.shields.io/badge/docs-API%20reference-8a2be2)](https://muddykat-tech.github.io/Spellforce-Spell-Framework/)
[![Wiki](https://img.shields.io/badge/wiki-tutorials-lightgrey)](https://github.com/muddykat-tech/Spellforce-Spell-Framework/wiki)
[![Platform](https://img.shields.io/badge/platform-Windows%20x86-informational)](#requirements)
[![Language](https://img.shields.io/badge/language-C%20%2F%20C%2B%2B-00599C)](src/api)

[Installation](#installation) · [Quick Start](#quick-start) · [Custom Campaigns](#custom-campaigns) · [Documentation](#documentation-and-resources) · [Contributing](#contributing)

</div>

---

## Overview

The **Spellforce Spell Framework (SFSF)** is an ASI plugin that loads into *Spellforce 1* at runtime and exposes the game's internals through a stable, versioned C API. Mods are compiled as standalone `.sfm` modules and loaded at startup.

Where traditional *SpellForce* modding is limited to editing gamedata files or requires deep reverse engineering and assembly knowledge, SFSF lets you write entirely new spell types, implement custom effect logic, evaluate enemy AI for those spells, add new buildings, build full custom campaigns, and extend the game’s user interface. All of this is possible without interacting with ASM or the game’s internals.

> [!NOTE]
> **Current version: 5.0.0-RC.** The framework is under active development and the API may still change between major releases. See the [Changelog](CHANGELOG.md) for breaking changes before upgrading.

### Features

| Capability | Description |
| --- | --- |
| **Custom spells** | Register new spell lines and attach type, effect and end handlers written in C/C++. |
| **Spell AI** | Provide AI handlers so the game engine's casters evaluate and use your spells. |
| **Custom buildings** | Register new buildings alongside the vanilla set. |
| **Custom campaigns** | Ship full campaigns from a JSON manifest, with their own maps, starter kits and save folders. |
| **Custom loading screens** | Attach a loading screen mesh to a map. |
| **User interface** | Construct menus, labels, containers and screens through the UI API. |
| **Mod list & diagnostics** | An in-game mod list showing every loaded mod, its metadata, and any errors or ID conflicts it hit. |
| **Conflict registry** | Spell, effect, building and campaign IDs are claimed centrally, so clashes between mods are reported in the mod list or sfsf.log file |
| **Structured logging** | Levelled logging to the debugger for both the framework and your own mods. |

### API surface

The framework hands every mod a single `SpellforceSpellFramework *` on load, grouped into subsystem interfaces:

| Interface | Purpose |
| --- | --- |
| `registrationAPI` | Register spells and link handlers. |
| `spellAPI` | Query and manipulate active spells. |
| `effectAPI` | Set spell effect data. |
| `aiAPI` | Hook into AI spell evaluation. |
| `figureAPI` | Interact with figures (units and NPCs). |
| `buildingAPI` | Interact with and register buildings. |
| `iteratorAPI` | Traverse the engine's object collections. |
| `uiAPI` | Build and manipulate menus and screens. |
| `toolboxAPI` | General-purpose engine helpers. |
| `logAPI` | Logging and diagnostics. |

Full reference: **[SFSF Documentation Site](https://muddykat-tech.github.io/Spellforce-Spell-Framework/)**.

---

## Requirements

| | |
| --- | --- |
| **Game** | *Spellforce 1: Platinum Edition* (Steam): targets the **beta** branch |
| **Platform** | Windows, 32-bit (x86) |
| **Toolchain** *(mod authors)* | MinGW-w64 12.2.0, x86 |
| **Debugger** *(optional)* | [x64dbg](https://x64dbg.com/) (x32dbg build) |

---

## Installation

1. Download the latest `sfsf.asi` from the [Releases page](https://github.com/muddykat-tech/Spellforce-Spell-Framework/releases).
2. Copy `sfsf.asi` into the root directory of your *Spellforce 1* installation, next to the game executable.
3. Create an `sfsf` folder in that same directory and place any framework mods inside it.

The resulting layout:

```text
Spellforce Platinum Edition\
├── sfsf.asi
└── sfsf\
    ├── your_mod.sfm                # mod modules, discovered as sfsf\*.sfm
    ├── screens.json                # optional, custom loading screens
    ├── campaigns\
    │   └── your_campaign.json      # optional, one file per custom campaign
    └── <mod id>\
        └── buildings\
            └── your_building.json  # optional, per-mod building definitions
```

Launch the game as normal. Loaded mods appear in the in-game mod list, along with any errors they reported.

---

## Quick Start

Start from [`examples/template`](examples/template) for a complete, buildable skeleton, then browse the other [examples](examples) for working implementations of area-of-effect spells, auras, summons, custom towers and more.

---

## Custom Campaigns

SFSF can register custom campaigns from JSON files. Place one `.json` file per campaign in `sfsf\campaigns\` inside the game directory. They are picked up at startup by the Custom Campaign Module, and a **CUSTOM CAMPAIGNS** button appears on the main menu once at least one campaign registers.

### Example

```json
{
    "name": "My Campaign",
    "author": "Your Name",
    "description": "A short description shown in the detail panel.",
    "campaign_folder": "mycampaign",
    "start_map": "mycampaign_start",
    "tutorial_map": "mycampaign_tutorial",
    "starterkit": "SK_mycampaign.des",
    "campaign_name_id": 6543,
    "avatar_type": 8
}
```

### Fields

| Field | Required | Description |
| --- | --- | --- |
| `name` | Yes | Shown on the campaign button and in the detail panel. Max 63 characters. |
| `campaign_folder` | Yes | Plain folder name used for both `map\<campaign_folder>\` and the campaign's save folder. Max 63 characters. |
| `start_map` | Yes | Map name only, no path and no `.map` extension. Must exist at `map\<campaign_folder>\<start_map>.map`. |
| `description` | No | Detail panel text. Max 255 characters. A missing description logs a warning. |
| `author` | No | Shown under the name in the detail panel. Max 63 characters. A missing author logs a warning. |
| `tutorial_map` | No | Map name only. Must exist at `map\<campaign_folder>\<tutorial_map>.map`. When unset, the tutorial checkbox is ignored and the campaign always starts directly. |
| `starterkit` | No | `.des` filename relative to `figure_template\starterkit\`. Replaces the whole skill-derived `SK_<skill><subskill>.des` name, so the kit no longer varies with the player's chosen skills. Leave unset to keep vanilla behaviour. Max 127 characters. |
| `campaign_name_id` | No | Text ID used for the campaign name in the save header. Defaults to the Order campaign's string. |
| `avatar_type` | No | Identifies the campaign's saves. Must be in the range 8-23. Defaults to `8 + campaign index`. |
| `intro_video` | No | Reserved. **Not implemented yet** and currently ignored. |

### Rules

- A maximum of **16** campaigns can be registered. Files beyond that are skipped with an error.
- `campaign_folder` must be a plain folder name. Slashes, colons and `..` are rejected, as are the engine's own folders: `save`, `temp`, `char`, `campaign2` and `campaign3`.
- `campaign_folder` and `avatar_type` must be unique across all registered campaigns. A collision on either rejects the campaign. A duplicate `name` is allowed but logs a warning, since the menu buttons become indistinguishable.
- `start_map` and, when set, `tutorial_map` are checked against the files on disk at load time. A missing map rejects the campaign.
- Unknown keys are ignored, so a campaign file stays forward compatible.
- Rejections and warnings are attributed to the Custom Campaign Module and shown in its entry in the in-game mod list.

### Save separation

Each campaign gets its own save folder named after `campaign_folder`, created next to the vanilla campaign folders. Saves also carry the campaign's `avatar_type`.

> [!IMPORTANT]
> The default `avatar_type` is derived from the campaign's index, so changing which campaign files are installed can change that index and orphan existing saves. **set `avatar_type` explicitly** if the load order is causing issues.

---

## Custom Loading Screens

An optional `sfsf\screens.json` maps a map name to a loading screen mesh in `mesh\`:

```json
{
    "greyfell": "my_loading_screen.msb",
    "mycampaign_start": "my_other_screen.msb"
}
```

Keys are matched case-insensitively against the map name with its path and extension stripped. Where the name contains an underscore, only the part **after the first underscore** is matched, following the vanilla naming convention: `000_Greyfell.map` is keyed as `greyfell`, and `P101_Mirraw_Thur.map` as `mirraw_thur`. Values must end in `.msb` and the mesh must exist, otherwise the entry is rejected. A maximum of 128 entries are read.

---

## Building from Source

The framework and the bundled examples build with MinGW-w64 (x86):

```sh
mingw32-make          # build bin/sfsf.asi
mingw32-make mods     # build the reference test module (bin/testmod.sfm)
mingw32-make clean    # remove build artifacts
```

Each directory under [`examples`](examples) has its own Makefile and builds independently; the `mods` target in the root Makefile is the reference for how a module is compiled and linked.

---

## Documentation and Resources

| Resource | Description |
| --- | --- |
| [API Reference](https://muddykat-tech.github.io/Spellforce-Spell-Framework/) | Generated documentation for the framework and every API group. |
| [Wiki](https://github.com/muddykat-tech/Spellforce-Spell-Framework/wiki) | Tutorials and guides. Pages labelled **SRD** cover API details. |
| [Chapter 0: Build Environment](https://github.com/muddykat-tech/Spellforce-Spell-Framework/wiki/Chapter-0:-Build-environment) | Teekius' walk-through for setting up a working toolchain from scratch. |
| [API Headers](src/api) | The headers you compile against, just include `sfsf.h` and it pulls in the others as well. |
| [Examples](examples) | Buildable reference mods |
| [Changelog](CHANGELOG.md) | Release history, including breaking API changes. |
| [Coding Style](CodingStyle.md) | Conventions expected in contributions. |

### Debugging mods

Use **x64dbg** (the **x32dbg** build, since the game is 32-bit):

1. Attach the debugger to the *Spellforce* process.
2. Restart the debugger with the refresh symbol to capture all log output from the start.

This is the same workflow used to debug the framework itself.

### Ghidra project access

A partially annotated Ghidra project of the *Spellforce* codebase is available on request. Contact **jarrod.muddyman@muddykat.tech** for read-only access.

---

## Testing and Bug Reporting

SFSF is in active development and bugs are expected. Testing and detailed reports are the most valuable contribution you can make.

When reporting an issue, please include your game version and branch, the SFSF version, the mods you had loaded, and any relevant debugger output. Open reports on the [issue tracker](https://github.com/muddykat-tech/Spellforce-Spell-Framework/issues).

---

## Contributing

Contributions are welcome.

1. Read the [Contribution Guidelines](CONTRIBUTING.md) and the [Code of Conduct](CODE_OF_CONDUCT.md).
2. Match the conventions in [CodingStyle.md](CodingStyle.md) (a `.clang-format` and `uncrustify.cfg` are provided).
3. Fork, branch, and open a pull request describing the change and how you tested it.

---

## License

Released under the [GNU General Public License v3.0](LICENSE).

---

## Credits

**Core team**

- **UnSchtalch**
- **Shovel_Knight**
- **Muddykat-Tech**

Special thanks to **Teekius66** for the documentation and tutorial series, and to everyone in the *Spellforce* modding community whose testing and feedback keeps this project moving.

# Spellforce Spell Framework (SFSF)

![Maintenance](https://img.shields.io/maintenance/yes/2025)
![GitHub Release](https://img.shields.io/github/v/release/muddykat-tech/Spellforce-Spell-Framework?include_prereleases&label=Latest%20Release)
![GitHub Downloads (all assets, latest release)](https://img.shields.io/github/downloads-pre/muddykat-tech/Spellforce-Spell-Framework/latest/total)
![Workflow](https://github.com/muddykat-tech/Spellforce-Spell-Framework/actions/workflows/c-cpp.yml/badge.svg)

## Overview

The **Spellforce Spell Framework (SFSF)** is a powerful toolkit for developers aiming to create new spell types and custom spell logic for the game *Spellforce 1*. Tailored for the beta version of *Spellforce 1 Platinum Edition* on Steam.

---

## Installation

To use the Spellforce Spell Framework, follow these steps:

1. Download the latest `sfsf.asi` file from the [Releases Page](https://github.com/muddykat-tech/Spellforce-Spell-Framework/releases).
2. Move the `sfsf.asi` file into the base directory of your *Spellforce 1* installation.
3. Place any mods utilizing the framework in a subfolder named **`sfsf`** within the game directory.

---

## Testing and Bug Reporting

The Spellforce Spell Framework is in its **early development stages** and may contain bugs. Feedback, testing, and bug reports are invaluable to the framework's improvement. 

To report a bug or provide feedback, please use the [GitHub Issue Tracker](https://github.com/muddykat-tech/Spellforce-Spell-Framework/issues).

---

## Getting Started with Modding

Interested in creating mods with SFSF? Here are some helpful resources and tips:

- **API Files:** Access the required API files from the [API Source Directory](https://github.com/muddykat-tech/Spellforce-Spell-Framework/tree/master/src/api).
- **Example Mods:** Explore sample mod code [here](https://github.com/muddykat-tech/Spellforce-Spell-Framework/tree/master/examples).
- **Compilation Assistance:** Refer to the `mods` target in the repository's Makefile for guidance on building mods.
- **Debugging Mods:**  
   Use **x64dbg (x32dbg version)** to debug your mod:
   1. Attach the debugger to the Spellforce process.
   2. Restart the debugger using the refresh symbol to capture all logs.  
   *(This method is also used to debug the framework itself!)*

- **Curated Tutorials:** Follow Teekius' tutorial on setting up your environment and using the framework: [Chapter 0: Build Environment](https://github.com/muddykat-tech/Spellforce-Spell-Framework/wiki/Chapter-0:-Build-environment).
- **Technical Documentation:** Find in-depth documentation for the framework and API at the [Official SFSF Documentation Site](https://muddykat-tech.github.io/Spellforce-Spell-Framework/).
- **API Documentation (SRD):** Refer to the Wiki pages labeled "SRD" for API details: [SFSF Wiki](https://github.com/muddykat-tech/Spellforce-Spell-Framework/wiki).
- **Ghidra Project Access:** For Access to the ghidra project that contains the partially annotated codebase of Spellforce; You can request Read-Only Access from jarrod.muddyman@muddykat.tech 
---

## Contributing

We welcome contributions to improve the Spellforce Spell Framework! Contributions can include:

- Bug fixes
- Feature enhancements
- Documentation updates

Please refer to our [Contribution Guidelines](https://github.com/muddykat-tech/Spellforce-Spell-Framework/blob/master/CONTRIBUTING.md) for details on how to get involved.

---

## License

This project is licensed under the [GNU General Public License Version 3 (GPLv3)](https://github.com/muddykat-tech/Spellforce-Spell-Framework/blob/master/LICENSE).

---

## SFSF Team

The following contributors have played a significant role in developing the Spellforce Spell Framework:

- **UnSchtalch**  
- **Shovel_Knight**  
- **Muddykat-Tech**  

Special thanks to **Teekius66** for contributing to documentation and tutorials!

---

## Acknowledgments

Your continued support and feedback make this project possible. Together, we can enhance the *Spellforce 1* modding community!

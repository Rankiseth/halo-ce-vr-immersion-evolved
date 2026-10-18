# Halo CE VR - Immersion Evolved

**v0.1.0-alpha.1 / R1.37 - Win32 update**

An unofficial PCVR derivative of LivingFray's HaloCEVR, maintained by Rankiseth.

## Install

Read [INSTALL.txt](INSTALL.txt). Close Halo, back up your working installation,
then copy this package's contents into the folder containing `halo.exe`.
Replace included files and merge the `VR` folders. Keep your existing 32-bit
`openvr_api.dll` and `VR/config.txt`; this update does not include either file.

Requires Halo PC retail 1.10, a working HaloCEVR installation, SteamVR,
tracked controllers, and the Microsoft Visual C++ v14 x86 Redistributable.
Visual Studio is needed only to compile the source, not to install this ZIP.

## R1.37 highlights

- Magnum slide locks fully back when empty (Manual Reload enabled).
- Comfort > Play posture: Sitting (default) or Standing.
- Player height: 120-220 cm, default 180; applies only to Standing.
- Hip/supply zones and belt indicators move together; shoulder grab stays fixed.
- At 180 cm, standing belt depth is 60 cm below the headset.

Rankiseth confirmed the core changes working in-headset on 2026-10-18.
This package preserves that tested DLL and its runtime assets byte-for-byte.
See [RELEASE_NOTES.md](RELEASE_NOTES.md) for details and alpha limitations.

## Source, credits and permission

Source: https://github.com/Rankiseth/halo-ce-vr-immersion-evolved

Original mod: https://github.com/LivingFray/HaloCEVR

Read [CREDITS.md](CREDITS.md), [PERMISSIONS.md](PERMISSIONS.md), and
[THIRD_PARTY_NOTICES.md](THIRD_PARTY_NOTICES.md). Dependency licenses are in
`LICENSES/`. Game executables, maps, and extracted game audio are not included.

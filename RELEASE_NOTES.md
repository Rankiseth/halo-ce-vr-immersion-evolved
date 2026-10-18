# Halo CE VR - Immersion Evolved v0.1.0-alpha.1 (R1.37)

First Immersion Evolved alpha package for the original PC version of Halo CE.
**Update package:** install over an existing working HaloCEVR installation.

An unofficial PCVR derivative of LivingFray's HaloCEVR, built on the existing
Immersion Evolved / Stylish Restart work. Requires the original Halo PC retail
1.10 game and an existing HaloCEVR installation. MCC is not supported.

## New in R1.37

- **VR Settings > Comfort > Play posture:** choose Sitting (default) or Standing.
- **Player height (cm):** 120-220 cm; default 180 cm. Applies only to Standing.
  Sitting ignores the stored height and keeps the existing belt placement.
- Standing lowers the weapon hip holster and Magazine Zone together. A height
  of 180 cm places them 60 cm below the headset, matching the corrected Halo 2
  R47 calibration. Their horizontal offsets and handedness mirroring remain.
- The shoulder grab zone stays fixed. Only its belt preview and label lower.
- Live belt-position changes rebaseline grip edges and cancel transient reload
  gestures while preserving weapon ownership and loaded ammunition.
- Added an in-game sitting/standing guide page and Immersion Evolved identity.
- Added upstream permission evidence, credits, and dependency notices.
- Replaced inherited Arial font copies with Liberation Sans 2.1.5.

## Included from R1.36

The Magnum slide locks fully rearward after the last shot and stays back
through magazine replacement. Insert ammunition, grip the rearward slide,
complete the existing full pull gesture, and release/return to chamber it.
Short pulls and empty racks do not release the lock. Tactical reloads retain
their previous behavior. Manual Reload must be enabled.

## Maintainer headset report - 2026-10-18

Rankiseth confirmed that R1.37 works in-headset and that both the Magnum
empty-slide lock and Sitting/Standing holster settings behave correctly.
The maintainer-supplied update ZIP passed archive integrity, file checksum,
x86 DLL, R1.37 marker, and packaged-asset checks. The tested DLL is preserved
byte-for-byte in the release package. A compiler log was not supplied;
this verification is not a reproducible-build attestation.

The report confirms those core behaviors. Individual height extremes,
left/right-handed variants, persistence, and partial-rack cases were not
separately reported. Custom Edition, SPV3, and multiplayer remain unvalidated.

## Install

1. Close Halo and back up the working game folder and `VR/config.txt`.
2. Extract the **Win32-Update.zip** asset and copy its contents beside `halo.exe`,
   replacing included files and merging the `VR` folders.
3. Keep your existing 32-bit `openvr_api.dll` and `VR/config.txt`.
4. Start SteamVR and Halo. Open **Comfort** for Sitting/Standing and height in cm.

The Microsoft Visual C++ v14 **x86** Redistributable is required by the DLL.
See [Microsoft's runtime downloads](https://learn.microsoft.com/en-us/cpp/windows/latest-supported-vc-redist).
The package includes INSTALL.txt and a per-file SHA256.txt manifest.
GitHub's automatic source ZIP and tarball do not contain an installable DLL.

## Credits and permission

Based on [LivingFray's HaloCEVR](https://github.com/LivingFray/HaloCEVR).
LivingFray's public invitation to fork/reuse the code is documented in
[PERMISSIONS.md](https://github.com/Rankiseth/halo-ce-vr-immersion-evolved/blob/master/PERMISSIONS.md),
with the original message link and supplied screenshot. Dependency notices
are retained. This is an unofficial community derivative; no blanket standard
license is asserted for inherited code. No game files or extracted game audio
are included.

## Binary identity

The tested `d3d9.dll` is 805,376 bytes, Win32/x86. SHA-256:

```text
f3fc74821ffd754a5331689a8776239755b9a84b094c86d2b2e17e626f8a27c6
```

Please report the revision and reproduction steps to the Immersion Evolved
maintainer for alpha feedback.

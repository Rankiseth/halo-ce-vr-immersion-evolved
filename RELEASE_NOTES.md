# Halo CE VR - Immersion Evolved v0.1.0-alpha.1 (R1.37)

Release preparation. Core headset checks passed according to Rankiseth;
the tested binary and build output are still needed for release packaging.

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
This is a maintainer-reported runtime result. The tested binary and compiler
output have not yet been supplied for artifact verification.

The report confirms those core behaviors; individual height extremes,
left/right-handed variants, persistence, and partial-rack cases below were
not separately reported.

## Detailed release regression checklist

1. Build with Visual Studio 2022 x86 and keep the compiler output.
2. Confirm About shows v0.1.0-alpha.1 / R1.37 and the log marker is R1.37.
3. In Sitting, vary height: hip, Magazine Zone, and shoulder preview stay fixed.
4. In Standing at 180 cm, test both weapon holsters and magazine pickup/insertion.
5. Check 120/150/180/220 cm and both handedness preferences; visual and grab
   positions should agree. The shoulder grab remains at the shoulder.
6. Change posture while holding a gun, then resume. No unintended shot, draw,
   stow, or drop should occur. Save, restart, and verify the settings persist.
7. Verify the Magnum empty-slide sequence in both hands, including partial
   pulls, magazine insertion, and re-holstering.
8. Check all five settings tabs and the eleven Guide pages for readable text.

Report issues against Immersion Evolved with the revision and reproduction
steps; avoid sending fork-specific support requests to LivingFray.

<p align="center"><img width="460" height="215" src="Extras/Bindings/icon.png" alt="Halo VR"></p>

# Halo CE VR - Immersion Evolved

**v0.1.0-alpha.1 / R1.37 - development source**

An unofficial community derivative of [LivingFray's HaloCEVR](https://github.com/LivingFray/HaloCEVR)
for the original Windows PC version of Halo: Combat Evolved. Built around
physical weapon handling, manual reloads, body holsters, and VR comfort options.
Maintained by [**Rankiseth**](https://github.com/Rankiseth/halo-ce-vr-immersion-evolved). The original mod and its contributors are credited
in [CREDITS.md](CREDITS.md).

**Windows compilation and headset testing of R1.37 are pending.** This source
package does not include a compiled d3d9.dll. See [release notes](RELEASE_NOTES.md).

## Requirements

- A legitimate installation of Halo PC retail, patched to 1.10 (01.00.10.0621).
- An existing working HaloCEVR installation, including its 32-bit openvr_api.dll.
- SteamVR and tracked controllers. Quest-style controls are described below.
- Visual Studio 2022 with Desktop development with C++ and a Windows SDK to build.

MCC is not supported. The modified physical-interaction path targets retail
Halo PC; Custom Edition, SPV3, and multiplayer are not validated for this alpha.

## What's included

- Either-hand weapon handling, two-handed support, weapon hip/shoulder holsters,
  world pickups and drops, manual magazine/shell/rocket reloads, and physical grenades.
- R1.36 Magnum slide lock-open after the final shot, retained in R1.37.
- **Comfort > Play posture:** Sitting (default) or Standing.
- **Comfort > Player height (cm):** 120-220, default 180. Height affects Standing
  only; Sitting ignores it. The corrected calibration puts the belt 60 cm below
  the headset at 180 cm, compared with the original 43 cm seated offset.
- The hip holster and Magazine Zone move with height. The shoulder grab zone
  stays fixed; its belt preview and label move with the belt.
- Existing R1.35 vehicle sights, camera choice, and cinematic transition refinements.

## Build and install the update

1. Extract the complete source ZIP into a new folder. Keep your working build
   and VR/config.txt as a rollback.
2. Run from PowerShell in that folder:

   ```powershell
   powershell -NoProfile -ExecutionPolicy Bypass -File .\Build-ImmersionEvolved.ps1
   ```

3. After a successful build, close Halo and copy the contents of
   `artifacts/Immersion-Evolved-R1.37/update` into your existing HaloCEVR game folder.
   Keep the existing `openvr_api.dll` and your `VR/config.txt`.
4. Copy the whole update: it includes the bindings, font replacements, images,
   and license notices. The script also creates a Win32 update ZIP.
5. Confirm About shows `0.1.0-alpha.1 / R1.37`; `VR/inject.log` should contain
   `[Stylish Restart R1.37]`. Follow the headset checks in RELEASE_NOTES.md.

The build script stages files; it does not install or launch Halo. The source
includes MinHook directly, so no submodule checkout is needed. The historical
Build-Restart-R1.ps1 name forwards to the new script.

## Controls and comfort

Hold **X**, then press **RT** to open the VR menu; in the Ghost, tap **Y**.
Select **Comfort**, click the posture button to choose Sitting or Standing,
and enter your height in centimetres. Select **Save** to retain it.

Hold grip to keep a weapon. Release grip in its hip/shoulder grab zone to stow
it; release away from a holster to drop it. The Magazine Zone is on the opposite
hip. Saved handedness mirrors these positions. The shoulder weapon's small belt
preview is an indicator: reach to the shoulder to grab the weapon.

Right-handed reload starts with **B**; left-handed reload starts with a short
**Y** press. Hold physical left **Y** for five seconds to recenter. The eleven
pages under **Guide** cover weapons, grenades, vehicles, and posture settings.

The same configuration settings can be edited with Halo closed:

```text
StylishStandingMode = false
StylishPlayerHeightCm = 180
```

Choose `true` for Standing. Height has no effect when the mode is `false`.
Posture does not change camera height, world scale, chest grenades, or vehicle
camera settings.

## Permission and attribution

LivingFray publicly invited other modders to fork and reuse his code.
[PERMISSIONS.md](PERMISSIONS.md) preserves the exact statement, original Discord
message link, and supplied screenshot. This is not a blanket MIT/GPL/LGPL
license for the package. Dependency notices are retained in
[THIRD_PARTY_NOTICES.md](THIRD_PARTY_NOTICES.md) and `LICENSES/`.

This is a free PCVR community project. Game executables, maps, and extracted
Halo audio are not included. Runtime weapon meshes and sounds come from the
player's game installation. This fork is not endorsed by LivingFray or the
Halo rights holders.

## Development and validation

Run `python3 tests/run.py` with a C++17 g++ toolchain for portable production
adapter tests. These use engine/VR doubles and do not replace MSVC compilation
or headset testing. See `research/validation-R1.37.txt` for the actual results.

[RESTART-R1-README.txt](RESTART-R1-README.txt) and
[STYLISH-RESTART.md](STYLISH-RESTART.md) preserve the development history.
The inherited README is archived under `docs/upstream/README-R1.36.md`; its
relative links and instructions describe the earlier project layout.

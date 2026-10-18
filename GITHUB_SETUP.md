# Repository and release workflow

Repository: [Rankiseth/halo-ce-vr-immersion-evolved](https://github.com/Rankiseth/halo-ce-vr-immersion-evolved)

Owner: **Rankiseth**. Default branch: **master**.
The repository is a public fork of [LivingFray/HaloCEVR](https://github.com/LivingFray/HaloCEVR),
created by Rankiseth on 2026-10-18. The R1.37 source import preserves the
upstream commit history.

## Source lineage

The prepared gameplay baseline is upstream 1.4.0 commit
`ada2d3ba2965bbb9c6d6d653456b0ba0a897cf63`, plus the existing R1.36 work
and the R1.37 changes. The import is based on the fork's initial master commit
`18603218f96e9c31f887c069c1a112fe1edb943a`; the upstream commits between those
points changed documentation, not gameplay. MinHook is vendored, so the
prepared tree intentionally removes the inherited `.gitmodules` entry.

Keep README.md, PERMISSIONS.md, CREDITS.md, THIRD_PARTY_NOTICES.md, LICENSES,
and the permission screenshot with the source. Do not apply a blanket standard
license to inherited code. Preserve source notices and history.

## Next binary release

Planned title: **Halo CE VR - Immersion Evolved v0.1.0-alpha.1 (R1.37)**

Planned tag: **v0.1.0-alpha.1**

Rankiseth confirmed the core R1.37 headset checks on 2026-10-18. Archive and
verify the tested binary and compiler output, and use the detailed regression
checklist in RELEASE_NOTES.md before creating the tag and publishing a
pre-release. Publishing source commits does not create a binary release.

Build with Build-ImmersionEvolved.ps1. After a successful MSVC build, it stages
an update folder and a Win32 update ZIP. The source ZIP is not an installable
DLL release. Keep the release marked as a pre-release while it is alpha.

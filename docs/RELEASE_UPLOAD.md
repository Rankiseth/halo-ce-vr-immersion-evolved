# Publish v0.1.0-alpha.1

The maintainer-tested Win32 update has been prepared and statically checked.
The packaged DLL and runtime assets are unchanged from the uploaded build.

1. Open https://github.com/Rankiseth/halo-ce-vr-immersion-evolved and select
   Releases, then Draft a new release (or Create a new release for the first one).
2. Create tag `v0.1.0-alpha.1` targeting `master` at the package-preparation commit.
3. Set the title to **Halo CE VR - Immersion Evolved v0.1.0-alpha.1 (R1.37)**.
4. Paste the contents of the repository's RELEASE_NOTES.md into the description.
5. Attach `HaloCEVR-Immersion-Evolved-v0.1.0-alpha.1-R1.37-Win32-Update.zip`.
   Use the prepared package with the checksum below, which has current player docs.
6. Select **This is a pre-release** and leave **Set as latest release** off.
7. Wait for the asset upload to complete. Save draft to review it, or choose
   Publish release when ready to make the alpha download public.

Do not upload the source ZIP as the installable asset. GitHub supplies source
archives automatically for the release tag. The update requires a working
HaloCEVR installation and the original retail Halo PC 1.10 game.

Package SHA-256:

```text
f03ae6d77fc0bea52a80df363ba7547e05e95d053a85ac2a5948e49824faf4b8
```

DLL SHA-256 (unchanged from the tested build):

```text
f3fc74821ffd754a5331689a8776239755b9a84b094c86d2b2e17e626f8a27c6
```

Publication instructions follow [GitHub's release documentation](https://docs.github.com/en/repositories/releasing-projects-on-github/managing-releases-in-a-repository?tool=webui).

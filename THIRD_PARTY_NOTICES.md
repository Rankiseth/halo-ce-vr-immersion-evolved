# Third-party notices

There is no blanket MIT/GPL/LGPL declaration for this entire project.
LivingFray's permission statement is recorded in [PERMISSIONS.md](PERMISSIONS.md).

| Component | Location | License notice |
| --- | --- | --- |
| MinHook and bundled HDE | ThirdParty/MinHook | [MinHook.txt](LICENSES/MinHook.txt); retain individual source notices |
| OpenVR SDK headers/import library | ThirdParty/OpenVR | [BSD 3-Clause](LICENSES/OpenVR-BSD-3-Clause.txt) |
| JSON for Modern C++ 3.11.3 | ThirdParty/nlohmann | [MIT](LICENSES/nlohmann-json-MIT.txt); retain embedded notices |
| stb_image | ThirdParty/stb/stb_image.h | [Embedded MIT/public-domain alternatives](LICENSES/stb_image.h.txt) |
| stb_truetype | ThirdParty/stb/stb_truetype.h | [Embedded MIT/public-domain alternatives](LICENSES/stb_truetype.h.txt) |
| Liberation Sans 2.1.5 | Extras/Fonts | [SIL Open Font License 1.1](LICENSES/LiberationSans-OFL-1.1.txt) |

The four Liberation Sans font binaries are unmodified. Their file names are
normal.ttf, bold.ttf, italics.ttf, and bold_italics.ttf to match the existing
loader; their internal font names and copyright metadata are retained. These
replace the Microsoft-supplied Arial copies in the inherited working tree.

MinHook source is vendored from the existing R1.36 package, whose recorded
upstream dependency commit is 1e9ad1eb42db11bfcb65461f687c656612d1b555.
It is included directly so a source ZIP can be built without submodule setup.

License text sources:

- https://github.com/ValveSoftware/openvr/blob/master/LICENSE
- https://github.com/nlohmann/json/blob/v3.11.3/LICENSE.MIT
- https://github.com/liberationfonts/liberation-fonts/blob/2.1.5/LICENSE

Game executables, maps, extracted game audio, and the OpenVR runtime DLL are not
included in this source package. The build produces an update for an existing
HaloCEVR installation; keep the installed 32-bit openvr_api.dll. Runtime weapon
meshes and sounds are read from the player's installed game.

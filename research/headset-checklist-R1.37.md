# Additional R1.37 regression coverage

Core headset behavior was confirmed by Rankiseth on 2026-10-18. These detailed
cases are retained for further alpha testing; individual results were not
reported.


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

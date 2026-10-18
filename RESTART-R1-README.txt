HALO CE VR - IMMERSION EVOLVED v0.1.0-alpha.1 / R1.37
18 October 2026

Source preparation: Windows/MSVC build and headset testing pending.
Run Build-ImmersionEvolved.ps1; copy artifacts/Immersion-Evolved-R1.37/update
into an existing working HaloCEVR installation after a successful build.
Keep the original openvr_api.dll and VR/config.txt. No game is launched.

NEW: VR Settings > Comfort > Play posture: Sitting (default) / Standing.
Player height is in cm, bounded to 120-220, default 180.
Height affects Standing only. Sitting preserves the original belt placement.
At 180 cm the standing belt sits 60 cm below the headset (the corrected R47
calibration). The hip holster, Magazine Zone and shoulder belt preview/label
move down. The shoulder grab zone and chest grenades remain fixed.
Both handedness modes use the same calibration; choose Save to retain it.

The R1.36 Magnum lock-open change is included. See RELEASE_NOTES.md for its
reload sequence, build status and focused headset checks. Permission and
credits are in PERMISSIONS.md, CREDITS.md and THIRD_PARTY_NOTICES.md.

PREVIOUS R1.36 NOTES (HISTORICAL)
--------------------------------
HALO VR - RESTART R1.36 TEST SOURCE
17 October 2026

Small private maintenance update based on the supplied R1.35 source.
No precompiled DLL is included. Windows/MSVC compilation and in-headset
appearance/interaction still need your test.

BUILD / INSTALL
1. Extract HaloCEVR-Stylish-Restart-R1.36-Source.zip into a NEW folder.
   Keep your working R1.35 DLL/source as a rollback.
2. Open PowerShell in the extracted folder and run:
   powershell -NoProfile -ExecutionPolicy Bypass -File .\Build-Restart-R1.ps1
3. After a successful build, close Halo. Copy the CONTENTS of
   artifacts\Restart-R1.36\weapon-test into the folder containing halo.exe.
   Keep your existing VR\config.txt and installed game assets.
4. Confirm VR\inject.log starts with [Stylish Restart R1.36], or check About.
The script stages files only; it never installs or launches Halo automatically.

CHANGE IN R1.36
The Magnum slide now locks fully rearward after the last shot. It stays back
while the empty magazine is ejected and the new magazine is inserted. Grab
the rearward slide, perform the same full rearward hand pull used in R1.35,
and release (or return the hand forward) to chamber and close it.

Inserting a magazine alone, a short pull, or racking without loaded rounds
cannot unlock it. No new button binding or slide-release button is added.
The existing configured pull distance is retained. Loaded free-slide gestures
and tactical reloads keep their prior behavior. Other weapons are unchanged.
Manual Reload must be enabled; otherwise the original game controls the pose.

FOCUSED HEADSET CHECK
1. Fire the Magnum's last round and inspect the fully rearward slide.
2. Eject the empty magazine: slide stays back. Insert a fresh magazine: still back.
3. Grab the rearward slide, pull fully and release: slide closes and gun fires.
4. Try a short pull first: it must stay locked until a complete rack.
5. Reload with rounds remaining: slide stays forward and no rack is added.
6. Repeat in the opposite hand and after briefly holstering an empty Magnum.

VALIDATION
The complete production ReloadHandler portable fixture passes, including the
new last-shot, empty-rack, partial-pull, insertion, suspension, mirrored/angled
pose, descendant-node and tactical-reload checks. The unmodified R1.35 handler
fails the new last-shot position check as expected. See research/validation-R1.36.txt.
This is not a Windows build or a live-game/headset test.

PREVIOUS R1.35 NOTES (HISTORICAL)
--------------------------------
HALO VR - RESTART R1.35 TEST SOURCE
14 October 2026

Private update based on the delivered R1.34 source.
No precompiled DLL is included. Windows/MSVC compilation and headset testing
cannot be performed in this environment; build and in-game checks remain.

BUILD / INSTALL
1. Extract HaloCEVR-Stylish-Restart-R1.35-Source.zip into a NEW folder.
   Keep the working R1.34 DLL/source as a rollback.
2. Open PowerShell in the extracted folder and run:
   powershell -NoProfile -ExecutionPolicy Bypass -File .\Build-Restart-R1.ps1
3. After a successful build, close Halo. Copy the CONTENTS of
   artifacts\Restart-R1.35\weapon-test into the folder containing halo.exe.
   Keep VR\config.txt and installed game assets. ThirdParty is included.
4. Confirm VR\inject.log begins with [Stylish Restart R1.35].
The script stages files only; it never installs or launches Halo automatically.

CHANGES IN R1.35
- Scorpion/Banshee: replace the 15m muzzle-point sight with an eye-relative
  firing-direction sight. The old near-range zero depressed the reticle from
  a raised camera when aiming at distant targets. Crosshair Distance now
  controls stereo display depth without tilting the vehicle sight downward.
  Aimable weapons use native owner aim; fixed weapons use their muzzle facing.
  Remove R1.34's incorrect extra clamp of owner aim against the barrel marker.
  RT or both triggers shows primary direction; LT alone shows secondary.
- Cutscenes: automatically reset height, position and facing at the first
  tracked cinematic frame. OpenVR then adds each native shot's heading.
  On exit, reset to Halo's native player-control heading, including skipped
  scenes. Input is held at zero across the handoff so the previous VR heading
  cannot overwrite the mission's chosen direction before the first render.
  Defer resets through menus/loading/tracking loss. One reset per transition;
  free head look and right-stick turning remain available during the movie.
  Release a stick held through the transition before making another turn.
- Vehicle camera choice: Basic / First Person Vehicles is now near the top.
  ON retains cockpit views; OFF restores the native third-person camera.
  It takes effect immediately and clears cockpit pitch and dismount blending,
  including when changed from a menu. Choose Save to remember the preference.
  Third person uses the existing native vehicle control path; tank/Banshee
  retain RT primary and LT secondary. Scripted camera shots remain native.
- Preserve the user-confirmed Scorpion clipping fix, preferred height 1.5 /
  forward 1.0, Banshee camera settings, and R1.34 plasma-pistol thumb folding.

FOCUSED HEADSET CHECK
1. Scorpion: fire cannon and MG separately at a wall/target with steady aim;
   compare near and far targets, then pitch up/down, turn and drive on slopes.
   Keep height 1.5 / forward 1.0. Confirm the turret still clears the camera.
2. Banshee: test both weapons while level, then while climbing/diving.
3. Two Betrayals opening: look/turn away during the movie and let it finish.
   Gameplay should start facing the scripted combat direction. Repeat by
   skipping, then check another cinematic and a Pelican opening/seat handoff.
4. Enter a movie after crouching or changing physical position. Check its
   first view is centred without holding Y. Look around normally afterwards.
5. While seated, open VR Settings > Basic, turn First Person Vehicles OFF,
   close the menu, drive/aim/fire, then turn it ON. Repeat in tank, aircraft,
   Ghost and Warthog. Save, restart and confirm the preference persists.

RETICLE LIMITS / DIAGNOSTICS
This is a nominal firing-direction sight, not a traced impact marker. It
removes the artificial 15m downward convergence; real camera/muzzle separation
at close range, gun spread, native aim assistance, projectile drop and twin
emitter spacing still affect actual impacts. It does not change native shots.
The direction correction and native retail behaviour require in-game testing.
If a validated marker/weapon read fails, the original sight path remains.
The log reports [VehicleSight R1.35] and valid marker status on changes.
[Cinematic R1.35] records entry reset / exit restoration and native target yaw.
Please retain the log and stationary firing footage if an offset persists.

No new native hooks, executable addresses, bindings or game assets are added.
The complete portable regression output is in research/validation-R1.35.txt.

RETAINED R1.32 CHANGES (USER CONFIRMED)
- Preserve held weapons and holster assignments through temporary loading and
  checkpoint-save pauses. Moving the same player's object in memory no longer
  rebuilds its holsters or clears manual-reload state. A new player or a true
  saved-age rollback still starts a fresh session.
- AR/sniper support stays held when the gun becomes empty. Release the support
  grip, move to the charging handle, and grip again. Both hands are supported.
  Magnum free slide and required shotgun pumping remain as before.
- Physical RIGHT stick turns your view in native cutscenes using the existing
  smooth/snap turn setting. It does not change the scripted camera's aim.
  Hold physical Y for five seconds to reset position/height AND facing. In a
  cinematic, facing resets to the shot's forward direction; in a vehicle,
  the seat is recalibrated. Automatic loading recenter does not reset yaw.
- Preserve native Pelican opening scenery until the native camera switches
  into a nearby first-person view or ends the cinematic. The seat's established
  right-stick yaw/pitch look and exit smoothing remain. No mission timers,
  scripts, visibility override or new native memory addresses are added.

RETAINED R1.31b CHANGES
- Flashlight touch zone follows the saved Left Handed setting: left temple in
  right-handed mode, right temple in left-handed mode. Either hand can activate
  it, including while holding a weapon. Switching the firing hand alone does
  not move the zone. Existing radius, haptics and grenade exclusion remain.
- Guide and settings descriptions updated. R1.31a steering/reload fixes retained.

RETAINED R1.31a CHANGES
- Fix the R1.31 vehicle-stick regression shown in the supplied log. New analog
  handles registered but failed during reads with OpenVR InvalidHandle (3).
  Read failures/inactive actions now fall back to original Move/Look bindings,
  restricted to the correct physical left/right controller in either preset.
  An active centered input remains centered; aliases are never added together.
- Physical stick diagnostics report route changes only, instead of repeating
  errors every frame. The log also reports the selected action-manifest path.
- Remove optional free charging on the AR and sniper. Their handles engage when
  the chamber needs charging after running empty. Successful charging releases
  the latch; required empty reloads still work in either hand. Magnum free slide
  and required shotgun pumps remain available. Guide/settings text updated.

RETAINED R1.31 CHANGES
- On foot: legs only, OFF by default. Enable Basic > Show Legs if wanted.
  Torso, head and arms are removed from the optional body draw; walking legs
  remain animated. Tracked hands still work normally. The new OnFootLegs config
  key deliberately does not inherit the old torso option's true value.
- Ghost: LEFT stick moves forward/back and strafes left/right. RIGHT stick aims
  up/down/left/right and turns. LT OR RT fires; holding both does not double fire.
  A/X no longer fire the Ghost. Short Y still opens its menu; hold Y 5s recenters.
- Ghost Aim Sensitivity is independent of Turret Sensitivity. Default 1.0,
  compared with the old shared 0.55; adjustable range 0.1 to 2.5.
- Scorpion driver: RT main cannon; LT machine gun (native secondary weapon).
  A no longer fires the machine gun. R1.33 adds its first-person camera.
- VR Settings titles omit Stylish and use readable spacing. Legacy grip toggle,
  hand-swap distance, native shoulder-switch zones and old head-touch flashlight
  controls are hidden when their replacements are enabled. Inapplicable 3DOF
  tuning is hidden in 6DOF. Saved keys are retained for compatibility/fallback.
- Shotgun: no optional free pumping while ready. Still pump after a shot and
  after loading from empty. Only the Magnum retains optional free cycling.
- Pelican: discover/refresh seats during rendering as well as native input.
  This handles scripts that suspend gameplay input during an intro. Right stick
  looks around with independent view yaw/pitch; scripts still control flight.
  The shared path applies to Pelican seats across missions, including Silent
  Cartographer, 343 Guilty Spark, Truth and Reconciliation and Assault on the
  Control Room. These individual scenes still require in-game confirmation.
- Exiting vehicles/turrets: suspend headset-to-native yaw/pitch corrections for
  one elapsed second after detachment, then synchronize native/Chimera view
  references before resuming. Headset rendering and physical head tracking stay
  live. Position handoff uses frame-rate-independent exponential smoothing.
  Inspired by AJBats' draft explanation supplied by Ryan; no external code copied.
- Guide updated for all changed controls, including a new TANK / PELICAN page.

QUICK CONTROLS
On foot: hold grip to hold; release in a holster to stow, elsewhere to drop.
Either hand can hold a weapon. Free-hand grip at its support point reduces recoil.
Right-handed: right hip + right shoulder weapons; left hip Magazine Zone.
Left-handed mode mirrors weapon/ammo holsters. Plasma stays left chest, frag right.
B interacts. Reload with B (right-handed) or a short Y press (left-handed).
Hold physical left Y five seconds to recenter. X held + RT opens the menu,
except the Ghost where short Y opens it. Right A jumps. Crouch physically.
Flashlight: fresh grip with either hand at the left temple in right-handed mode,
or the right temple in left-handed mode. Works even while holding a gun.
World Scale 0.80 remains the recommended Quest 3 starting point; recenter on foot.

VEHICLES
Warthog driver: left stick steering, RT forward, LT reverse.
Warthog turret / Shade: right stick aims, RT fires; Turret Sensitivity tunes both.
Ghost: left stick movement/strafe; right stick aim/turn; either trigger fires.
Scorpion: RT main cannon, LT machine gun; existing movement/aim controls retained.
Banshee: RT plasma, LT plasma cannon; existing movement/aim controls retained.
Pelican: headset and right-stick look; scripted flight.
Fixed vehicle stick actions remain on the same physical hands with either preset.

MANUAL RELOADS
Magnum/AR/sniper: reload button ejects; free-hand grip at Magazine Zone takes a
magazine; bring it to the well. If it ran empty, pull the slide/charging handle
back, then return or release. Only the Magnum can cycle freely while ready;
AR/sniper handles require charging after empty, plus release and a fresh grip.
Shotgun: take shell from Magazine Zone and insert at top port. Pump back/forward
after each shot and after loading from empty. Shell Ratio: 1..4, reserve limited.
Launcher: reload button opens/closes casing. Grip/remove/discard spent rockets;
take fresh rockets from Magazine Zone, insert one or two, then close with button.
Needler: reload button only; no auto-reload. Plasma batteries are unchanged.

VALIDATION / LIMITS
Portable fixtures exercise the production camera/input/reload/render adapters
with synthetic native memory, VR and Windows API doubles. No real game/headset
or Windows DLL compilation is available here. Camera comfort, actual mesh
appearance and each mission intro need headset testing.
Campaign retail 1.0.10.0621 only. Custom Edition/multiplayer retain original paths.
Independent dual-weapon rendering/firing and manual steering remain pending.
R1.29's native-visibility rollback is retained; no new PVS override is introduced.
The earlier missing-cliff visibility issue is not claimed solved.
No game executable, maps, models, sounds or other Halo assets are in this ZIP.
Upstream HaloCEVR credit: LivingFray and contributors (README/LICENSE retained).

param()
$ErrorActionPreference = 'Stop'
$root = $PSScriptRoot
$vswhere = Join-Path ${env:ProgramFiles(x86)} 'Microsoft Visual Studio\Installer\vswhere.exe'
if (!(Test-Path $vswhere)) { throw 'Install Visual Studio 2022 with Desktop development with C++.' }
$msbuild = & $vswhere -latest -version '[17.0,18.0)' -products '*' -requires Microsoft.Component.MSBuild Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -find 'MSBuild\**\Bin\MSBuild.exe' | Select-Object -First 1
if (!$msbuild) { throw 'Visual Studio 2022 MSBuild was not found.' }
if (!(Test-Path (Join-Path $root 'ThirdParty\MinHook\build\VC17\libMinHook.vcxproj'))) { throw 'Extract the entire source archive, including ThirdParty.' }
$output = Join-Path $root 'artifacts\Immersion-Evolved-R1.37'
$native = Join-Path $output 'native'
$staging = Join-Path $output 'postbuild'
$package = Join-Path $output 'update'
$bindings = Join-Path $package 'VR\OpenVR'
if (Test-Path $package) { Remove-Item -LiteralPath $package -Recurse -Force }
New-Item -ItemType Directory -Force -Path $native,$staging,$bindings | Out-Null
$previousDirectory = $env:HALOCEVR_DIR
try {
    # Redirect upstream post-build copying into a staging directory, not Halo.
    $env:HALOCEVR_DIR = $staging
    Push-Location $root
    try {
        & $msbuild (Join-Path $root 'HaloCEVR.sln') /t:Rebuild /m /p:Configuration=Release /p:Platform=x86 "/p:OutDir=$native/" /verbosity:minimal
        if ($LASTEXITCODE -ne 0) { throw 'Build failed. Keep the full compiler output.' }
    } finally { Pop-Location }
} finally { $env:HALOCEVR_DIR = $previousDirectory }
$dll = Join-Path $native 'd3d9.dll'
if (!(Test-Path $dll)) { throw 'Expected d3d9.dll was not produced.' }
$bytes = [System.IO.File]::ReadAllBytes($dll)
if ($bytes.Length -lt 64) { throw 'Invalid DLL.' }
$pe = [BitConverter]::ToInt32($bytes,0x3c)
if ($pe -lt 0 -or $pe -gt ($bytes.Length - 6) -or [BitConverter]::ToUInt32($bytes,$pe) -ne 0x4550 -or [BitConverter]::ToUInt16($bytes,$pe+4) -ne 0x14c) { throw 'Expected a valid x86 PE DLL.' }
if (![System.Text.Encoding]::ASCII.GetString($bytes).Contains('[Stylish Restart R1.37]')) { throw 'Immersion Evolved R1.37 build marker missing.' }
Copy-Item $dll (Join-Path $package 'd3d9.dll') -Force
Copy-Item (Join-Path $root 'Extras\Bindings\*') $bindings -Recurse -Force
Get-ChildItem $bindings -Filter '*.py' | Remove-Item -Force
Copy-Item (Join-Path $root 'Extras\Fonts') (Join-Path $package 'VR\Fonts') -Recurse -Force
Copy-Item (Join-Path $root 'Extras\Images') (Join-Path $package 'VR\Images') -Recurse -Force
foreach ($notice in @('README.md','RELEASE_NOTES.md','PERMISSIONS.md','CREDITS.md','THIRD_PARTY_NOTICES.md','RESTART-R1-README.txt','STYLISH-RESTART.md')) {
    Copy-Item (Join-Path $root $notice) $package -Force
}
Copy-Item (Join-Path $root 'LICENSES') (Join-Path $package 'LICENSES') -Recurse -Force
New-Item -ItemType Directory -Force -Path (Join-Path $package 'docs') | Out-Null
Copy-Item (Join-Path $root 'docs\permissions') (Join-Path $package 'docs\permissions') -Recurse -Force
$hashes = Get-ChildItem $package -Recurse -File | Sort-Object FullName | ForEach-Object {
    $relative = $_.FullName.Substring($package.Length + 1).Replace('\','/')
    '{0}  {1}' -f (Get-FileHash $_.FullName -Algorithm SHA256).Hash.ToLowerInvariant(), $relative
}
Set-Content -Encoding ASCII (Join-Path $package 'SHA256.txt') $hashes
$zip = Join-Path $output 'HaloCEVR-Immersion-Evolved-v0.1.0-alpha.1-R1.37-Win32-Update.zip'
Compress-Archive -Path (Join-Path $package '*') -DestinationPath $zip -Force
Write-Host "Built Immersion Evolved v0.1.0-alpha.1 / R1.37: $zip"
Write-Host 'Install this update over a working HaloCEVR installation; keep its 32-bit openvr_api.dll and VR/config.txt.'
Write-Host 'Comfort: Sitting is the default. Standing uses player height in cm; 180 cm gives a 60 cm head-to-belt depth. Sitting ignores height.'
Write-Host 'B/Y opens/closes rocket casing; both rounds can be inserted before closing. Native rocket previews, segmented reload cues, raised support grip.'
Write-Host 'If startup still fails, keep VR\inject.log and VR\StylishCrash.log. Keep native\d3d9.pdb with this build.'
Write-Host 'AR: 35% tighter spread and smaller reticle; StylishARSpreadScale = 1 restores native. Outline magazine zone; native grenade previews, grey when empty. Driver steering arrow hidden.'
Write-Host 'World size: StylishWorldScale defaults to 1. Quest 3 tested preference: 0.80 for a smaller world, then recenter. Headset FOV is unchanged.'
Write-Host 'Read RESTART-R1-README.txt before testing. This script never installs or launches Halo.'
Write-Host 'Tutorial: look up/down/left/right in the headset, or move the right stick in those directions. No mouse input required by the new look-history bridge.'
Write-Host 'Ghost: left stick moves/strafes; right stick aims; LT or RT fires. Short Y opens its menu. Warthog: left-stick steer, RT/LT drive. Turrets: right-stick aim, RT fires.'
Write-Host 'B interacts. Hold physical left Y for 5 seconds to recenter position, height and facing; short Y reloads in left-handed mode. X+RT opens the menu outside the Ghost.'
Write-Host 'VR Settings > Guide: eleven pages of Quest 3 controls and weapon reload instructions. Basic > Show Legs is off by default. Torso is removed. Ghost Aim Sensitivity is separate from turrets.'
Write-Host 'Magnum locks open when empty until a fresh magazine and full pull/release; loaded free slide remains; AR/sniper charging and shotgun pumping only when required. Needler requires a reload press. Copy ALL five updated VR/OpenVR binding JSON files with this DLL.'
Write-Host 'Scorpion: RT main cannon, LT machine gun. Banshee: RT plasma, LT plasma cannon. Both have first-person view. Pelican: right stick looks around even during scripted rides.'
Write-Host 'Flashlight: left temple in right-handed mode, right temple in left-handed mode. Either grip can activate it.'

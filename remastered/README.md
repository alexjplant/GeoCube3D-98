# GeoCube 3D Remastered

This directory is the C++17 SDL3/OpenGL native single-player port and its
Emscripten/WebGL2 browser target. It is
deliberately separate from the Visual C++ 6 project in `../src`.
Multiplayer is not implemented and no DirectPlay source is part of this target.
The old sources remain historical reference material only.

Original author: Andy Weston (`@westonsoftware`). Remastered build by Alex
Plant (`@alexjplant`).

## M0 Baseline

The legacy project requires Visual C++ 6, MFC, DirectX 6, and the libraries
listed in `src/Launch.dsp`: DirectInput, DirectDraw, DirectPlay, Direct3DRM,
DirectSound, WinMM, DXGUID, and Video for Windows. The remastered target does not
include those headers or libraries.

The active legacy single-player level table has five levels. The level colors,
rock counts, and music names are recorded in `src/gameplay.cpp:22-69`.

Controls documented by `g_aHelpText` in `src/gameplay.cpp:75-87`:

| Action | Legacy control |
| --- | --- |
| Aim ship up/down/left/right | Arrow keys |
| Zoom in/out | Page Up / Page Down; numpad 9 / numpad 3 |
| Thrust forward/backward | A / Z |
| Thrust left/right | `<` / `>` (comma / period) |
| Fire | Ctrl |
| Shield | S |
| Pause | Pause key |
| Full stop | Numpad 5 |
| Quit | Escape |

The active code also maps F1 to Help and H to High Scores in
`CAGamePlay::UpdateInput` (`src/gameplay.cpp:868-881`).

The legacy game states are `eLoading`, `eRunning`, `ePaused`, `ePlayerHit`,
`eGameOver`, `eHighScore`, and `eHelpScreen`. They are documented here as the
behavior baseline; the remastered non-blocking state machine belongs to M2/M6.

The original `dist/Launch.exe` is a 32-bit Windows PE executable. This Linux
host has no Windows runtime, so the executable was not run. The remastered model
companions preserve the recognizable ship/cube presentation, while exact
legacy orientation and visual comparison remain documented renderer risks.

## Asset Inventory

`assets/manifest.csv` is the complete M0 manifest for assets referenced by the
active single-player path, the menu resource, and explicitly marked legacy
experimental code. The normalized copy uses lowercase paths and does not alter
the historical files in `src` or `dist`.

Important inventory findings:

- `src` and `dist` contain different subsets of assets.
- `src` has `jack.x`, `portal.x`, and `sphere3.x`; they are absent from `dist`.
- `dist` supplies the uppercase sound effects that legacy code loads with lowercase names.
- `w.x` and `s.x` are referenced only by disabled `SAVE_ME` logo code and are missing from both directories.
- Legacy BMP color-key transparency uses palette/index 0 for bullet and explosion decals.
- Native level music remains MIDI and is rendered at runtime by FluidSynth with the bundled SC-55 SoundFont.
- The browser target uses generated Ogg Vorbis tracks rendered from those MIDI files with the same SoundFont.
- `.3ds`, `.mmp`, `.stp`, and `.bin` files are source/reference or tooling artifacts, not active runtime assets in the current single-player path.

## Dependencies

The default CMake configuration fetches SDL3 release `3.2.0` with
`FetchContent`. The first configure therefore needs Git and network access.
OpenGL development files, FluidSynth 2.x, libsndfile, pkg-config, and a C++17
compiler are required. The runtime SoundFont is the Roland SC-55 bank
downloaded from the URL recorded in `assets/manifest.csv`.

The dependency scripts are:

```sh
# From remastered/ on Debian or Ubuntu
./scripts/install-deps-linux.sh

# From remastered/ on macOS with Homebrew
./scripts/install-deps-macos.sh
```

The Linux script installs `build-essential`, CMake, Git, pkg-config,
`libgl1-mesa-dev`, `libfluidsynth-dev`, `libsndfile1-dev`, and Xvfb. It
requires apt-get and sudo or root access. The macOS script installs CMake, Git,
pkg-config, the Homebrew `fluid-synth` formula, and `libsndfile`. It requires
Homebrew from `https://brew.sh`.

On macOS, run `eval "$(brew shellenv)"` in the shell used for CMake. If
pkg-config still cannot find FluidSynth, set
`PKG_CONFIG_PATH="$(brew --prefix)/lib/pkgconfig:$(brew --prefix)/share/pkgconfig"`.
The CMake build discovers FluidSynth through pkg-config. SDL3 itself is fetched
by CMake, so a separate SDL3 installation is not required.

For an offline build, install SDL3 from a package manager or from a source
build that exports its CMake package files, then configure with
`-DGEOCUBE_FETCH_SDL3=OFF`. The package must provide an `SDL3::SDL3` or
`SDL3::SDL3-shared` target. No dependency is vendored into this repository.

Apple's system OpenGL implementation supports a maximum core context of 3.2.
The shell requests OpenGL 3.3 core on Windows and Linux, and 3.2 core on macOS
as the platform-compatible initial context.

## Configure, Build, Run, Test

Run these commands from this directory:

```sh
cmake --preset debug
cmake --build --preset debug
ctest --preset debug
./build/debug/geocube3d --headless
./build/debug/geocube3d
```

The configure step copies `assets/` into the build tree. The executable finds
resources relative to its executable directory, or from `GEOCUBE_ASSET_ROOT`;
it does not use the process current working directory. `--headless` validates
resource-root discovery and representative assets without opening a window.
`--demo --frames 120` starts directly in the playable world for a bounded
renderer smoke run. Press F11 to toggle fullscreen and Escape to quit the
shell. On Linux, use Xvfb when no display is available:

```sh
xvfb-run -a ./build/debug/geocube3d --demo --frames 120
```

To exercise FluidSynth without a physical audio device, use SDL's dummy audio
driver:

```sh
SDL_AUDIODRIVER=dummy xvfb-run -a ./build/debug/geocube3d --demo --frames 120
```

Release commands are:

```sh
cmake --preset release
cmake --build --preset release
ctest --preset release
cmake --build build/release --target package
```

### Browser Build

The browser target uses SDL3's Emscripten backend, a WebGL2 context, an
Emscripten browser main loop, and Ogg Vorbis music loaded from the virtual
filesystem. FluidSynth and the SoundFont are not part of the browser runtime.
Install the Emscripten SDK and activate it before configuring:

```sh
emcmake cmake --preset web
cmake --build --preset web
```

The web build expects `assets/audio/music/*.ogg`. Render those files with the
native renderer target before the web configure:

```sh
cmake --preset release
cmake --build --preset release --target geocube3d_music_render
./scripts/render-music.sh
emcmake cmake --preset web
cmake --build --preset web
```

Serve `build/web/` through an HTTP server. The generated `geocube3d.html` is
the browser entry point; GitHub Pages renames it to `index.html` automatically
in the Pages workflow. Browser audio must be enabled by the page's first-click
prompt because browsers block autoplay before user interaction.

If the host has no display, the configure, build, CTest headless test, and
`--headless` command still verify the build and asset path. A windowed smoke
run must be performed later on a host with a working SDL video backend. On
macOS, run the windowed smoke directly because Xvfb is a Linux dependency.

The CPack archive contains the executable, SDL3 runtime when SDL3 was fetched,
the normalized assets, the SoundFont, and the asset validator. FluidSynth is a
native system dependency and is not copied into the archive; install it with
the platform script before running an installed package.

## M1 Shell

The shell currently provides SDL initialization, an OpenGL context, clear and
present, quit handling, resize-safe viewport updates, F11 fullscreen toggling,
focus change handling, a monotonic frame clock with a 250 ms delta clamp, and
temporary FPS output. It intentionally has no gameplay loop, Direct3D Retained
Mode compatibility layer, MFC, Win32, GDI, WinMM, DirectSound, networking, or
multiplayer path.

## M2 Core

`geocube_core` has no SDL, OpenGL, MFC, Win32, DirectX, or UI dependency. It
contains named actions, `Vec3` math, sphere/cylinder/AABB and swept-bullet
collision helpers, the five-level table, player movement and cube-border
reflection, bullets, rock splitting, scores, lives, and the original game
states. Simulation advances at 60 Hz with an eight-step catch-up limit. The
fixed-step clock and swept bullet collision are intentional portability
improvements; the original scoring, rock splits, five-level wrap, and three-life
game-over behavior are preserved.

The `geocube3d_core` CTest target runs independently of the SDL shell. The
renderer currently uses spherical gameplay bounds; exact mesh bounds can be
introduced without changing the core contract.

## M3 Renderer

The renderer uses OpenGL 3.3 core shaders, VAOs/VBOs, a perspective camera,
depth testing, six wireframe boundary grids, procedural rock geometry, a
remastered `geocube_mesh 1` fallback loader, and a case-sensitive BMP-to-RGBA
loader. The runtime loads converted OBJ/MTL companions for `ship`, `cube`,
`jack`, `portal`, `sphere3`, and `pasta`; the converted material groups preserve
the original diffuse colors. The player model uses the core ship direction/up
basis, so arrow input rotates the model and keeps the camera behind its forward
axis.

## M4 Input And Flow

SDL keyboard and mouse events map to the named core actions. Focus loss clears
held input. Controls are configurable from the Controls screen and persist in
SDL's settings directory. Defaults are arrows for aim, A/Z for forward/backward
thrust, comma/period for strafing, Space for fire, S for shield, Pause for pause,
Page Up/Down for zoom, C for stop, H for high scores, Enter
for confirm, and Escape for back/quit. The application runs the
core at 60 Hz, keeps rendering independent of simulation rate, handles
pause/high-score/game-over screens without blocking the SDL event loop,
and supports fullscreen and resize changes.

## M5 Audio

SDL3 audio streams load normalized WAV effects, mix overlapping voices, support
looping thrust/shield sounds, and tolerate missing devices or files. FluidSynth
loads `Roland.SC-55.sf2`, schedules the existing `.mid` files, and renders
stereo PCM directly into the SDL3 audio stream. A missing audio device remains
non-fatal, but a missing FluidSynth package or SoundFont is a build/runtime
dependency error for native MIDI playback. The Emscripten build excludes
FluidSynth and plays the generated `.ogg` tracks through browser audio instead.

## M6 UI And Persistence

The single-player menu, new-game flow, options, about, pause, HUD,
game-over, and high-score screens are nonblocking OpenGL UI overlays using a
small remastered bitmap font. Settings and scores are stored below SDL's
`SDL_GetPrefPath`. RmlUi is not required for the build; the lightweight overlay
keeps the same application-state boundary and avoids adding an unverified
third-party UI backend to the release candidate.

## M7 Packaging And Exclusion

The remastered target contains no `dxaplay.*`, `player.cpp`, online dialogs,
DirectPlay setup, or network synchronization. `geocube3d_asset_validate` checks
remastered meshes, converted OBJ/MTL models, BMPs, WAVs, MIDI files, and the
SoundFont. The browser bundle contains WebGL2 assets and generated Ogg Vorbis
tracks. Install/package output
contains the executable, fetched SDL3 runtime when applicable, and the
relative `assets/` tree.

Build and package commands:

```sh
cmake --preset release
cmake --build --preset release
ctest --preset release
cmake --build build/release --target package
```

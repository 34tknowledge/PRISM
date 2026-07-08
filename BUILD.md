# Building PRISM

PRISM is a JUCE 8 plugin built with CMake. JUCE itself is pulled automatically
via CMake `FetchContent` (pinned to tag **8.0.14**) — you do **not** install JUCE
by hand.

## Prerequisites

### Windows
- **Visual Studio 2022 or 2026** with the **"Desktop development with C++"**
  workload — this must include the MSVC toolset (STL/CRT headers + `lib\x64`) and
  a **Windows 10/11 SDK**. The bare compiler alone is not enough.
  - Quick check: the folder
    `...\VC\Tools\MSVC\<version>\include\vcruntime.h` must exist. If it doesn't,
    open the Visual Studio Installer → Modify → tick **Desktop development with
    C++** → Install.
- **CMake ≥ 3.22** and **Ninja** on PATH (or use the versions bundled with VS's
  "C++ CMake tools for Windows" component).
- **Git** (CMake uses it to fetch JUCE).

### macOS
- **Xcode** + command line tools.
- **CMake ≥ 3.22** (`brew install cmake ninja`).

## Build — Windows

Option A — the driver script (recommended, sets up the toolchain for you):

```powershell
powershell -ExecutionPolicy Bypass -File build.ps1
```

Option B — manual, from a **"x64 Native Tools Command Prompt for VS"** (so
`cl.exe` is on PATH):

```bat
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

## Build — macOS / Linux

```bash
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

On macOS this additionally produces the **AU** (Audio Unit) alongside the VST3.

## Output

After a successful build the artefacts are in:

```
build/Prism_artefacts/Release/VST3/PRISM.vst3
build/Prism_artefacts/Release/Standalone/PRISM.exe        (Windows)
build/Prism_artefacts/Release/AU/PRISM.component          (macOS only)
```

Because `COPY_PLUGIN_AFTER_BUILD` is on, the VST3 is also copied to your user
plugin folder automatically:
- Windows: `%CommonProgramFiles%\VST3` or `%USERPROFILE%\...\VST3`
- macOS: `~/Library/Audio/Plug-Ins/VST3` (and `.../Components` for AU)

Point your DAW's plugin scanner at that folder, rescan, and load **PRISM**
(manufacturer *starfinesse*).

## Quick sanity test without a DAW

The **Standalone** build (`PRISM.exe`) opens the full UI and lets you play with a
MIDI keyboard (or the on-screen keyboard) — the fastest way to hear it.

## Troubleshooting

- **`No CMAKE_CXX_COMPILER could be found`** — you're not in a VS developer
  environment. Use the *x64 Native Tools* prompt, or run `build.ps1`.
- **`Cannot open include file: 'vcruntime.h'` / missing STL headers** — the MSVC
  toolset's headers aren't installed. Open the VS Installer and add **Desktop
  development with C++** (with a Windows SDK). This is a components issue, not a
  code issue.
- **First configure is slow** — that's the one-time JUCE clone. Subsequent
  configures are fast.
- **MSVC internal compiler error (ICE)** — rare, and a compiler bug rather than a
  PRISM bug. Configure with `clang-cl` instead:
  `cmake -S . -B build -G Ninja -DCMAKE_C_COMPILER=clang-cl -DCMAKE_CXX_COMPILER=clang-cl`
  (install the "C++ Clang tools for Windows" VS component first).
- **Reset the build** — delete the `build/` folder and configure again.

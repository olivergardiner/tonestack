# Build, deployment and validation

## Current toolchain

The current CMake project requires CMake 3.19+ and Qt 6.5+ Core, Gui, Widgets, PrintSupport and Help. The version-3 preset file itself requires a CMake version supporting that preset schema (3.21+). The default preset selects Visual Studio 18 2026, x64, and Qt at `D:/Qt/6.11.1/msvc2022_64`. MSVC builds use `/FS` and `/Z7` for compiler Debug symbols, avoiding the PDB-server mismatch seen with `/Zi`. No C++ language standard is explicitly selected by CMake; legacy qmake selects C++11.

```powershell
cmake --preset default
cmake --build build --config Debug
cmake --build build --config Release
```

CMake links `ngspice/ngspice.lib` and copies `ngspice/ngspice.dll` alongside the executable. Do not confuse that library directory with the additional root-level DLL. Use binaries compatible with the selected architecture/toolchain. Although a MinGW preset and cross-platform Qt declarations exist, MSVC-specific CRT calls remain in the code. Cross-platform/MinGW support is not established.

The old `tonestack.pro`, solution and project files remain historical build routes. Prefer CMake for ongoing work. `.ui` files are processed by Qt's build integration; root `ui_*.h` files are generated artifacts.

## Running locally

The executable uses its bundled circuits/ and help/ directories regardless of the working directory. The CMake runtime_assets target refreshes those directories and ngspice.dll on every build. VS Code deployment tasks explicitly use PowerShell and add the matching Qt runtime.

See [Asset paths and deployment](asset-paths.md) for layout, custom-file resolution, installation commands and compatibility.

## Packaging

CMake installation includes Qt deployment, circuits, ngspice and compiled help. The install-tree checks are described in asset-paths.md. The legacy NSIS script expects a prepared staging directory with `bin/`, sibling `circuits/`, LICENSE and COPYING; it requests administrator privileges. Verify staging inputs and runtime working directory before using it. Keep packaging changes separate from incidental documentation work.

## Validation

Both configuration and numerical Qt Test suites are registered with CTest. Configuration tests use simulator stubs; the numerical suite links the real ngspice DLL and compares responses with independent calculations. See [the numerical baseline](numerical-baseline.md) for coverage and limits. For behavioural work, run relevant checks from this list and record actual results:

1. Start from the documented working directory; verify the default catalogue, diagram and curve.
2. Switch among tone-stack, basic and active modes; select entries with different numbers of components and pots.
3. Change R/C/L and source/load values; verify suffix display and a plausible response change.
4. Move pots across both endpoints; change taper types, test a ganged circuit, and check both settings algorithms and S taper.
5. Reset to catalogue values. Save As, inspect JSON, open it and compare supported values; explicitly check known name/filename/position limitations.
6. Snapshot, change colour/controls, clear and print; check range changes with active circuits.
7. Open help and follow contents/index links. For error-handling changes, exercise missing files, invalid JSON and simulator failure without risking user data.

Documentation-only validation should check relative Markdown links and reconcile statements against source. Do not describe manual GUI checks as completed unless they were actually performed.

## Configuration regression tests

The CMake build now includes the Qt Test target tst_configuration when BUILD_TESTING is enabled (the default). It exercises production component adapters with a captured ngspice command boundary, so it does not require a live simulator. It covers pot positions/gangs, legacy midpoint defaults, taper restoration, JSON and on-disk configuration round trips, long value text, and the 256-snapshot boundary and clear operation. The application still needs manual GUI and numerical ngspice checks.

With the matching Qt bin directory on PATH:

powershell commands:
    cmake --build build --config Debug
    ctest --test-dir build -C Debug --output-on-failure

CTest uses Qt's offscreen platform. Tests are CMake-only; the legacy qmake project continues to build just the application.

The controller suite adds deterministic scheduling/lifecycle tests; the numerical suite also checks the controller and real main-window update/recovery path offscreen. See [Simulation controller](simulation-controller.md).

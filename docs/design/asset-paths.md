# Asset paths and deployment

Runtime data no longer depends on the process working directory. AssetPaths (assetpaths.h) centralizes resolution, and spiceinput.h reads decks and includes through Qt before passing lines to ngSpice_Circ.

## Lookup and saved files

- The preferred layout is circuits/ and help/ beside the executable, with ngspice.dll in that same executable directory.
- The original installer layout (bin/ beside circuits/) remains supported as a fallback.
- Catalogue entries are resolved when loaded. Custom relative netlist/image references are relative to the containing .tsc file.
- Existing ../circuits/... and circuits/... references fall back to the bundled circuit directory. An existing document-local file takes precedence, allowing custom libraries to retain their own layout.
- Absolute references remain absolute while loaded. Saves keep bundled references in the legacy portable form; custom references are made relative to the saved file's directory. References across different Windows drives remain absolute.
- Circuit definitions still reference external assets; saving does not embed or copy a custom netlist or image. Move a custom file together with its referenced assets.
- Non-source ngspice commands remain trusted commands; their own path semantics are not rewritten.

The loader normalizes the end card and adds the empty terminator required by the older bundled DLL. The controller reads top-level decks using Qt rather than the shared library's source command parser, whose filename quoting is inconsistent. .include and .inc files are expanded relative to the file containing each directive, with cycle/depth checks. External .lib paths are made absolute. No code changes the process cwd during loading. Paths are ultimately limited by the bundled ngspice model parser for directives other than those handled here.

Help is found relative to the executable and opened read-only so installed assets need not be writable.

## Build and installation

The runtime_assets build target refreshes bundled circuit definitions, decks, PNG diagrams, model files, compiled help and ngspice on every build, including asset-only edits. Authoring files such as Visio, TeX and LaTeX build artifacts are excluded. CMake install rules include the same runtime assets under bin/; Qt's deployment script installs Qt runtime dependencies.

```powershell
cmake --preset default
cmake --build build --config Release
cmake --install build --config Release --prefix "D:/Some Folder/Tonestack"
```

The resulting bin/ folder can be launched with any working directory. The VS Code Build & Deploy Release task still adds the Qt runtime to the build output. Legacy qmake lists the new headers but does not run the CMake asset staging/install targets; package its assets manually in the documented layout.

The existing NSIS script uses its original staging convention and is not automatically invoked by CMake install.

## Checks

Numerical coverage includes a custom deck and include in a directory containing spaces, switching from a bundled circuit to that custom deck within one session, relative save-reference resolution, and starting a real main window from an unrelated working directory. The latter checks the bundled diagram and compiled help too.

Real-DLL controller scenarios run in separate test processes: the DLL retains process-wide state and does not reliably support repeatedly destroying and recreating native workers in one process. Deterministic mock lifecycle tests remain separate. This matches the application's one-controller-per-process design.

## Verification performed

On 10 November 2026, Debug and Release builds passed, and all seven CTest checks passed in both configurations. A Release CMake installation into build/Installation with spaces succeeded. An offscreen main-window probe copied into that installation loaded bundled circuits, a diagram and the compiled help with the Qt SDK removed from PATH. QtTest and the offscreen plugin were added only for the probe; they are not application runtime requirements.

This checks the local installed layout, not a signed installer, clean-machine redistributable installation or manual visual interaction. The legacy NSIS packaging workflow remains separate.

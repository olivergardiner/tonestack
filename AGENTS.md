# Tonestack agent guidance

Tonestack is a C++/Qt Widgets desktop application for interactive guitar tone-stack and filter analysis using the ngspice shared library. Read [docs/design/README.md](docs/design/README.md) before changing behaviour. The roadmap contains proposals, not approved implementation work.

## Working practices

- Inspect the working tree before editing; this repository can contain substantial existing staged and unstaged work. Preserve it and do not reset, stage, or commit unrelated changes.
- Keep changes focused. Do not turn maintenance requests into an architectural rewrite.
- Treat `.ui` files as the editable UI source. Do not hand-edit generated `ui_*.h`, build outputs, or generated Visual Studio projects.
- CMake is the current Qt 6 build entry point; `tonestack.pro` and the older Visual Studio files are legacy alternatives. Keep build descriptions consistent when adding source files, or explicitly document any divergence.
- Match the existing Qt naming and auto-connected slot conventions (`on_<object>_<signal>`). Renaming Designer objects requires checking the corresponding slots.
- Component adapters and Circuit hold widget pointers and queue simulator commands through SimulationController. There is no independent document model. Keep all application ngspice access on the controller worker. Preserve that behaviour unless the task explicitly changes it.
- Preserve the netlist naming, component units, enum values, and JSON conventions documented in `docs/design/circuit-format.md`.
- Do not assume circuit loading is passive: the JSON `command` is executed by ngspice. Use trusted circuit definitions.
- Document behavioural changes and file-format changes alongside the implementation. Do not silently change taper mathematics, sweep spacing, node conventions, or save semantics.

## Build and validation

From the repository root, with the configured Qt/MSVC tools installed:

```powershell
cmake --preset default
cmake --build build --config Debug
```

The preset currently uses Visual Studio 18 2026, x64 and `D:/Qt/6.11.1/msvc2022_64`; adapt paths/generator for another machine. See `docs/design/build-and-validation.md` for runtime layout and limitations. The MinGW preset is not evidence of a working MinGW port.

CMake registers configuration tests with simulator stubs and numerical tests linked to the real ngspice DLL. See docs/design/numerical-baseline.md for independent references and coverage. Run ctest --test-dir build -C Debug --output-on-failure with the matching Qt bin directory on PATH. For code changes, build and run the relevant manual checks in that document. Report exactly what ran, what passed, and any unavailable verification; do not report an empty CTest run as passing tests. Add targeted tests when introducing separable parsing or numerical logic. Documentation-only changes need link/content checks, not new tests.

## Source map

- `tonestackcalculator.*`: main window, plotting and actions.
- `simulationcontroller.*`: serial simulator worker, scheduling, result copies and errors.
- `circuit.*`: JSON loading/saving and UI configuration.
- `circuitelement.*`, `resistor.*`, `capacitor.*`, `inductor.*`: value parsing, display and `alter` commands.
- `potentiometer.*`, `potsweep.h`, `settingsdialog.*`: pot splitting and taper functions.
- `circuits/`: catalogues, SPICE decks, diagrams and diagram sources.
- `helpbrowser.*`, `helptextviewer.*`, `manual/help/`: embedded Qt Help.
- `installer/tonestack.nsi`: legacy Windows packaging.

Asset lookup and packaging conventions are documented in docs/design/asset-paths.md. Do not reintroduce cwd changes to load a netlist; use the shared circuit-input helper.

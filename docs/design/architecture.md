# Architecture and application flow

## Structure

`main.cpp` creates `QApplication` and `ToneStackCalculator`. The application is a single main window with Qt Designer forms, a `QGraphicsScene` frequency plot, component editors, circuit selection, settings and embedded help. ngspice is linked through its C shared-library API, not launched as a subprocess.

| Area | Responsibility |
| --- | --- |
| `ToneStackCalculator` | Startup, widget adapters, catalogue selection, simulation requests, plotting, snapshots, printing and file actions |
| `SimulationController` | Serial simulator worker, queued commands, request coalescing, copied results and errors |
| `Circuit` | Reads a circuit JSON object, queues its source command, configures widgets, writes the current component values |
| `circuitUiMap` | Non-owning pointers connecting `Circuit` to the main window widgets and component adapters |
| `CircuitElement` and R/C/L subclasses | Integer values, text conversion, JSON values, component visibility and simulator alterations |
| `Potentiometer` | Resistor-derived adapter splitting total resistance into two SPICE resistors; optional second gang |
| `PotSweep` / `SettingsDialog` | Taper interface and active taper algorithm, factors and preview graph |
| `HelpBrowser` / `HelpTextViewer` | Qt Help contents/index and `qthelp` resource rendering |

This is a UI-coupled design: model values live in component adapters, while `Circuit` also retains serialization metadata. The catalogue JSON array is separate from the edited adapter values. There is no general schematic editor, independent circuit document or undo stack. Simulation scheduling is isolated in SimulationController.

## Startup and selection

1. The settings/help member objects are constructed before the main-window constructor body.
2. The simulation controller starts its worker; ngSpice_Init runs there on the first job. The default catalogue is read from `../circuits/tonestacks.tsc` from the executable-relative asset directory.
3. Designer widgets are created and wrapped by three pot adapters, five adapters each for R/C/L, and source/load resistors (IDs 98/99). `pot4` is explicitly null.
4. Pots receive the settings dialog as their `PotSweep`. The plot grid and catalogue combo are built.
5. Combo changes invoke `setStack()` and `createPlot()` through Qt auto-connected slots. Loading queues the deck command before component overrides; the controller applies them before analysis.
6. Help is opened from `help/tonestack.qhc` relative to the executable directory.

The three mode actions replace the catalogue with tone stacks, basic electronics, or active circuits. Reset reloads the selected catalogue entry rather than restoring an undo checkpoint. Circuit restoration blocks the pot dial/type signals so partially restored values do not trigger extra simulations. This is still not a general transactional simulator interface.

## Edits, files and presentation

Each edit updates its adapter and queues alter commands; createPlot requests a debounced worker run. The displayed diagram is a static scaled pixmap, not generated from the netlist. Compare captures named response samples and request-time settings. Comparisons can be hidden, recoloured, deleted or restored as independent editable entries; they are session-only. Colour and Clear no longer rerun the simulation. All comparison curves use the current display range. Print renders the plot scene only.

Open appends objects from a `.tsc` file to the current catalogue and selects the first appended entry. Save writes the current `Circuit`; Save As first requests a name. Open and successful Save As retain a Save target. Saving uses QSaveFile for atomic replacement and updates the selected in-memory entry. Circuit names, pot positions and taper settings are restored; widget signals are blocked during circuit restoration to avoid intermediate simulations.

## Lifetime and maintenance boundaries

The main window allocates the `Circuit` and component adapters with raw `new`; its destructor only explicitly deletes the generated UI; the simulation member then joins its worker automatically. These adapters are not QObjects with automatic parent ownership. Scenes own items still attached to them, but `removeItem()` does not delete an item. Removed plot groups and duplicated C strings therefore need explicit lifetime review. Simulator callback state belongs to the worker; the DLL still supports only one application session. A future separation should establish ownership and simulation lifecycle before supporting multiple simultaneous windows or runs.

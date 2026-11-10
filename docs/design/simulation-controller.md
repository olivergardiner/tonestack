# Simulation controller

SimulationController owns scheduling for the application's single process-wide ngspice session. Circuit and component adapters keep their existing value/UI responsibilities, but their spice macro now calls dispatchSpiceCommand. When a controller exists, this queues commands; standalone numerical tests without a controller retain synchronous direct commands.

## Execution and ordering

The GUI thread owns pending commands, request generations and a 15 ms single-shot debounce timer. One worker QObject on one QThread owns initialization, all simulator commands, synchronous analysis, callbacks and vector access. Calling run synchronously on that worker avoids ngspice background callbacks and leaves the GUI event loop free. There are no GUI sleeps or nested processEvents loops.

Each plot request advances a generation counter. Rapid requests before a run are combined. While a run is active, edits accumulate for the next run; a new source command discards pending commands for the previous circuit, and repeated alter commands for the same element retain only the latest value. Source precedes the new circuit's component overrides. Only one job can run at a time.

SPICE file loading uses the circuit-input API and Qt include resolution; see asset-paths.md. The worker copies finite V(3) magnitudes into a QVector before returning. The GUI never accesses DLL-owned vectors. Completion is queued onto the GUI thread; an obsolete generation cannot replace the current plot or show an obsolete error. If a newer request exists, another debounced run follows.

The main window retains the original plot geometry, dB conversion and range conventions. It reports progress/errors in the status bar and disables Snapshot while waiting for a current response. Plot replacement happens only after a valid current result. Existing plot snapshots remain presentation objects.

## Failures and shutdown

Text diagnostics, nonzero command return codes, controlled exits, missing complex vectors and non-finite samples produce an error signal. Controlled exit is recorded instead of calling exit or issuing recursive quit commands. Single-file source commands are checked for readability before entering the DLL, so a missing netlist does not trigger its controlled-exit path. A failed load/alter batch requires loading a circuit again so later edits cannot accidentally simulate the previous deck. An ordinary analysis failure can be retried; a controlled simulator exit requires restarting the application.

The controller joins the worker at destruction, allowing an in-flight native analysis to finish before the worker is destroyed. Pending GUI work and queued results are discarded when the owner closes. There is no unsafe thread termination. There is currently no hard deadline or interruption of a hung native command: normal operation leaves the GUI responsive, but closing waits for that command. Hard isolation/time limits would require a separate simulator process or a separately validated cancellation protocol.

Only one application controller may exist at a time. The command-dispatch fallback is for standalone adapter tests, not concurrent simulator clients. DLL callback state has process lifetime; real-DLL controller checks run in isolated test processes. Arbitrary JSON commands remain trusted. Source/image asset paths are resolved explicitly.

## Verification

- The controller suite uses a semaphore-controlled fake engine to test coalescing, serialization, replacing a pending circuit, stale-result suppression, failure/retry and safe shutdown.
- The numerical suite runs the controller against the real DLL and compares its final RC response after a burst of edits with the analytical result.
- The numerical suite also drives real main-window widgets/slots offscreen through initial loading, rapid edits, active/basic mode switching and missing-netlist recovery.
- The existing independent RC, Fender, Pearl/ganged-pot and configuration-restoration baselines remain in place.

All three suites run through CTest. Automated offscreen checks do not replace a visual/manual check of plot appearance and mouse interaction.

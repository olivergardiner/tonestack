# Roadmap

This is a proposed backlog based on source inspection on 10 November 2026, not a commitment or authorization to implement it. Existing behaviour is documented in [docs/design](docs/design/README.md).

## First: establish a reliable baseline

- Completed: configuration round trips and a real-ngspice numerical baseline covering RC, Fender, Pearl OD5/ganged pots, and response restoration under both taper algorithms. Remaining: comprehensive unit parsing/formatting and taper endpoint tests, more circuits and background-run lifecycle checks.
- Completed in the first maintenance chunk: snapshot bounds, deletion of replaced/cleared plot groups, owned numeric formatting strings, and removal of the fixed-size QString conversion buffer. Remaining: review raw adapter ownership and other allocation lifetimes.
- Validate catalogue fields, component counts, ranges, enum values and values before applying changes. Initialize state and handle missing fields consistently; surface file and simulation errors in the UI.
- Completed in the first maintenance chunk: saved names, Open/Save As targets, atomic saves, dial positions and taper settings, with version 2 output and legacy-file defaults. Remaining: broader schema validation and portable assets.

## Next: simulation and deployment reliability

- Completed: serial worker controller, queued commands, edit coalescing, generation-checked copied results and status-bar errors; GUI polling and global callback flags removed. Remaining: evaluate native-command cancellation/process isolation for hard deadlines.
- Completed: executable-relative bundled assets, document-relative custom assets, legacy reference compatibility and staged/installed runtime data. Non-source commands remain trusted; custom assets are referenced rather than embedded.
- Read the actual frequency vector, validate complex results and centralize dB conversion/clipping. Give snapshots range/circuit metadata or explicit fixed-coordinate semantics.
- Produce and verify a complete Windows package with circuits, help, Qt and ngspice. Document third-party binary versions/licences and reproducible acquisition. Audit legacy qmake/VS/NSIS routes before retiring or updating them.

## Later: optional improvements

- Separate circuit data, serialization and taper mathematics from widgets so they can be tested independently.
- Consider data-driven component controls and additional pots beyond the current fixed slots.
- Completed: named session comparisons with visibility, colour, restore and range projection. Remaining: comparison-session persistence, phase display, numeric response export and automated knob sweeps.
- Persist settings and define Accept/Cancel behaviour clearly.
- Evaluate non-Windows builds only after replacing platform-specific calls and selecting compatible ngspice libraries.

Prioritize numerical correctness, compatibility with existing circuit files and interactive response over adding features. Confirm product priorities before beginning any of these work items.

- Requested: image export of the live/comparison curves. Agree circuit and parameter annotations before implementation; no export layout has been decided yet.

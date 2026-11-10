# Circuit format and authoring contracts

## Files and paths

A `.tsc` file is JSON with a top-level `circuits` array. The same format supplies catalogues and saved component configurations. Catalogues are `circuits/tonestacks.tsc`, `circuits/basic/basic.tsc`, and `circuits/active/active.tsc`. A saved configuration contains one circuit. It references external decks and images; it is not a self-contained circuit package.

Source commands and images resolve relative to their catalogue or saved file. Legacy bundled references remain portable. See [Asset paths and deployment](asset-paths.md) for precedence and save behaviour. Non-source commands remain trusted simulator commands; there is no strict schema validation.

| Circuit key | Current meaning |
| --- | --- |
| `circuitname` | Catalogue display name, restored on read and editable through Save As |
| `command` | ngspice command executed before parameter overrides |
| `image` | Diagram path; scaled to fit 469 by 326 pixels while preserving aspect ratio |
| `range` | Display range index: 0 = 0 to -40 dB, 1 = +20 to -20 dB, 2 = +40 to 0 dB; missing defaults to 0 |
| `rS`, `rL` | Source/load resistor objects, bound to R98 and R99 |
| `resistors`, `capacitors`, `inductors` | Ordered arrays of up to five editable components per kind |
| `pot1`, `pot2`, `pot3` | Optional potentiometer objects; absence hides that pot |

All component objects write `id` and integer `value`. Reading uses fixed adapter IDs and array positions; it does not remap from JSON `id`. Include all three arrays, using `[]` when empty. Missing arrays leave previous counts/visibility unchanged, and oversized arrays can make saving access past the five available adapters. Invalid ranges are not guarded before indexing plot arrays.

| Component | JSON integer unit | SPICE alteration |
| --- | --- | --- |
| Resistor | ohms | `alter r<ID>=<value>` |
| Capacitor | picofarads | `alter c<ID>=<value>P` |
| Inductor | microhenries | `alter l<ID>=<value>U` |

Text editing recognizes the first suffix character, case-insensitively: R uses M/K, C uses U/N, and L uses H/M for million/thousand multiples of its stored unit. Values are converted to integers. This parser is application-specific, not a complete SPICE number parser; do not assume `MEG`, scientific notation, fractions below the base unit, or malformed input behave like ngspice.

Pot objects additionally contain `function` (label), numeric `type` and optional `ganged` (defaults false). Type IDs are 0 linear, 1 log, 2 log A, 3 reverse log, 4 S taper. Reading restores position on a 0..99 scale, defaulting to 49 for legacy files and clamping out-of-range positions. Version 2 saves each pot position and a per-circuit taperSettings object (algorithm, log, logA, reverseLog). The root formatVersion is 2. Old files without position load at 49; absent taperSettings leaves current settings unchanged. Plot colours and snapshots remain unsaved.

## Deck contract

Start from `circuits/template.cir` and compare with a working catalogue entry. The UI supports three pots despite the template's four-pot wording.

- Ground is node 0, the source is across nodes 0/1, circuit input is node 2 and output must be node 3. Nodes 0..9 are reserved by convention.
- R98 connects source to input; R99 loads output to ground. Preserve these names for source/load editors.
- Editable passive elements must be R1..R5, C1..C5 and L1..L5, in the same order as their JSON arrays. Other fixed elements can use distinct names.
- Pot N uses `RPNLO` and `RPNHI` (for example RP1LO/RP1HI). A ganged pot additionally uses RP1LO2/RP1HI2. Wiring decides the electrical direction; inspect the actual splitting formula in the simulation document rather than relying on old template comments.
- Preserve `.AC DEC 40 10 100K`: plotting assumes this exact frequency spacing instead of reading the frequency vector.
- Keep an AC source of magnitude 1 for meaningful voltage-gain plots. The code plots the magnitude of V(3), not a calculated V(out)/V(in) ratio.

To add a circuit, add its deck, diagram and catalogue object together, then verify source paths, controls, end stops, source/load edits and the frequency curve. Active circuit decks may include fixed devices/models beyond the editable slots. Diagram source files (SVG, Visio or TeX) are authoring assets; the application displays the referenced image.

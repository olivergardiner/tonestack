# Named comparisons

Compare... captures a named, coloured comparison from the latest successful simulation. The Comparisons dock can be reopened from View. Check/uncheck entries to show or hide them, double-click a name to rename it, or select an entry and use Colour, Delete or Restore settings. Clear removes all comparisons and keeps the current live curve.

Each entry retains copied magnitude samples and the circuit configuration that produced them: source/image references, component values, pot positions/types/ganging, taper algorithm/factors and original range. Metadata is captured at request time so subsequent UI previews cannot relabel an earlier response. Pending or failed simulations cannot be captured. The limit is 256 comparisons; deletion frees capacity.

Every visible trace is projected from its samples onto the current plot's dB range, including passive/active mode changes. The list shows the captured range for context. Segments are clipped at the plot boundaries rather than flattened against them. The live curve uses the same rendering helper. Colour and Clear operate without rerunning ngspice.

Restore settings appends an independent editable circuit entry to the current catalogue and selects it, restores its original range and live colour, and requests a fresh simulation. Catalogue defaults and the retained comparison stay unchanged. Restoring uses the current external netlist/model files; their contents are not embedded.

Comparisons are session-only. They are not written into .tsc files, and this change does not alter the saved-file schema. A comparison can be restored and then saved as an ordinary circuit configuration.

Settings dialog Cancel now restores its prior taper settings. Accept reapplies the taper to all pots present in the selected circuit using their stored values/positions, avoiding loss of precision from reparsing rounded display text. These details keep captured configurations reproducible.

Tests cover request-time metadata, renaming, visibility, range projection, limits/deletion/clear, Settings acceptance/cancellation and restoration of the real-ngspice response after switching between passive and active modes. The dock layout is also inspected using an offscreen capture; native mouse interaction still benefits from a manual check.

Centre pots is separate from Reset: it moves only pots present in the current circuit to position 49 on the existing 0..99 scale (the same midpoint used by legacy defaults). Values, taper settings, retained comparisons and the save target stay unchanged. Ganged sections follow their pot normally. Dial signals are blocked during the operation and one new simulation is requested. Reset continues to reload the selected catalogue configuration.

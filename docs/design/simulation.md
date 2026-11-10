# Simulation, plotting and potentiometers

## Simulator interaction

The application now uses [SimulationController](simulation-controller.md). Adapter commands are queued, rapid changes are combined, and one worker thread initializes ngspice, applies commands, runs analysis and copies results. The GUI no longer uses bg_run, global running flags, sleeps or processEvents polling. Only the latest request can update the display. Progress and failures are reported in the status bar.

## Plot contract

The worker requests complex vector V(3) using ngGet_Vec_Info, validates its length/data and finite magnitudes, then copies magnitudes for the GUI. Missing or invalid results retain the old display and report an error.

The grid spans 400 by 400 scene units: four logarithmic frequency decades, 10 Hz through 100 kHz, and 40 dB vertically. Sample X coordinates use 100/40 = 2.5 units per point. The code does not inspect actual frequency values.

For complex sample z, the vertical coordinate is:

`y = -200 * log10(abs(z) + 1e-9) + 10 * rangeStart[currentRange]`

where `rangeStart` is 0, 20 or 40 dB. This corresponds to 10 scene units per dB for voltage magnitude. The epsilon avoids log(0). Clipping is implemented per point/segment rather than with a plot viewport: high gain is clamped at the top; segments whose new endpoint is below the bottom are omitted. No phase plot or numeric export is implemented.

Named comparisons now retain response samples and settings and redraw in the current range. See [Named comparisons](comparisons.md).

## Potentiometers

For dial position p, normalized sweep is `x = p / 99`. The active `PotSweep` supplies factor f. The adapter computes integer `rlo = total * f` and `rhi = total - rlo`, then alters both deck resistors. A second gang receives identical values when enabled. Zero-ohm end segments are possible; no minimum resistance is enforced.

The main window supplies `SettingsDialog` as the sweep provider. Its default algorithm uses `f = a * b^x - a`, with `b = (1/m - 1)^2` and `a = 1/(b - 1)`, where m is the midpoint factor. Defaults are 0.12 log, 0.25 log A and 0.88 reverse log. Near m=0.5 it returns x to avoid the singularity. Very small factors are adjusted in the implementation.

The alternative algorithm uses powers of 99, including a square-root sweep for log A and a mirrored reverse-log function. These curves do not share all endpoint properties of the default algorithm. S taper combines two scaled log halves. `Potentiometer` also has a fallback implementation for use without a sweep provider; S taper is linear there. Do not assume the fallback and settings algorithms are equivalent.

Settings changes mutate in-memory factors immediately. Accepting the dialog reapplies pot values and redraws. Taper settings are persisted in version 2 circuit saves; there is no application-wide settings persistence. Cancel restores the prior settings. Numerical tests should first capture current behaviour before changing these semantics.

The real-DLL numerical checks and their independent circuit references are documented in [Numerical baseline](numerical-baseline.md). The test reader copies the frequency descriptor before requesting V(3), because the bundled ngGet_Vec_Info API reuses its descriptor storage.

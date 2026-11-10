# Numerical baseline

The CTest numerical suite in tests/tst_numerical.cpp links the same bundled ngspice import library and DLL as the application. It does not stub the simulator. The configuration suite remains separate and uses stubs to isolate UI/configuration behaviour.

## Coverage and reference calculations

| Case | Reference | What it detects |
| --- | --- | --- |
| Basic catalogue RC low pass | H(f) = -1 / (1 + Rseries/Rload + j*2*pi*f*Rseries*C), with Rseries = R98 + R1 | Component overrides, capacitor units, loading, pole location and source polarity |
| Fender, linear controls at 10/49/90 | Independent complex nodal analysis of the current deck topology | Passive wiring, catalogue values and integer pot splitting |
| Pearl OD5, linear controls at 10/49/90 | Independent complex nodal analysis including all three finite-gain op-amps | Active wiring, ganged frequency control and op-amp frequency response |
| Saved Pearl configuration, both taper algorithms | Full complex response before and after JSON disk round trip | Restoration of positions, taper settings and component values |

Every run checks 161 samples and the frequency vector for .AC DEC 40 10 100K. Comparisons use complex voltages, so phase/sign errors cannot hide behind a matching magnitude. RC is checked again after the production capacitor adapter changes C1 from 1000 pF to 2200 pF. The restoration test changes both controls before reloading and confirms the intermediate response differs.

The reference solver is test-only code. It stamps admittances and uses Gaussian elimination with partial pivoting; it does not parse the SPICE deck or call production taper calculations for its linear-pot expectations. Topology and values are explicitly maintained in the test so a deck/catalogue edit requires reviewing its numerical consequences. Self-connected pot segments contribute no admittance. The source is negative at node 1 because the decks specify VIN 0 1 AC 1.

The OPAMP1 reference has a 10 Mohm differential input, gain 100000 / (1 + j*2*pi*f*1000*1.5915e-6) and a 10 ohm output resistance. This corresponds to the existing macro model, rather than assuming an ideal op-amp.

The absolute complex-voltage tolerance is 1e-8 + 2e-5*abs(reference) volts. This allows small solver differences while detecting meaningful response changes; it is not a claim of physical component accuracy. These are independently calculated expectations, not golden numbers recorded from the same simulation.

## Running and limits

From the repository root, with the matching Qt bin directory on PATH:

```powershell
cmake --preset default
cmake --build build --config Debug
ctest --test-dir build -C Debug --output-on-failure
# Use -C Release after building Release to check that configuration instead.
```

CMake points Qt tests at the matching SDK plugins for the offscreen platform and gives each suite a 60-second timeout. The runtime-assets target stages circuits, compiled help and ngspice beside the executable. Reference tests use the repository build directory for source fixture lookup; application asset lookup is independent of cwd. The legacy qmake build does not include these tests.

Reference checks use synchronous ngspice analyses. Additional tests now exercise the serial worker controller and real main-window updates/recovery offscreen. These do not validate visual plot clipping, arbitrary circuits or physical amplifier measurements; see simulation-controller.md. The numerical save/restore test uses production Circuit and SettingsDialog serialization and reads a temporary disk file; the separate configuration suite covers the main-window save path. Neither suite drives native file dialogs.

## Debug symbols

MSVC Debug compilation now uses /Z7 in place of /Zi, embedding compiler debug information in object files while retaining debugger symbols in the linked executable/PDB. This avoids the compiler's PDB-server dependency that produced C1902 on this installation. It does not change optimization levels or simulator mathematics.

Real-DLL controller and asset scenarios are separate CTest processes, each using one application-like simulator session. This isolates DLL global state and allows installed-layout probes to run a selected scenario.

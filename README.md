# tonestack

A reinterpretation of Duncan's Tonestack Calculator in C++ with QT and ngspice.

The original TSC was a very useful little tool for getting a feel for how different tone stacks would behave with knobs you could turn and see the results in real time. However, the range of tone stacks was limited and with it being built in Delphi, was hard for me to modify. While I never spent too long trying to get under the skin of the simulation approach, it seemed to me that it would be easier to create the circuits in Spice as this is such a well known and widely used engine. ngspice ships with a front end and this is perfectly adequate for creating suitable plots, but it lacks the immediacy and convenience of the TSC. However, ngspice is implemented as a DLL with a defined API (in C) and so can be relatively easly reskinned. I was originally going to develop in VS2019 using C++ but that limits your options in terms of UI technologies. I then turned to C# but it looked very hard to use that with a DLL with C bindings so I finally settled on QT, which should make it easy to go cross platform as well as being very capable.

This is a QT Creator project and needs packaging with QT's windeployqt.

The current version demonstrates the principle with a few stacks that were very easy to create as Spice files and the responses tally well with the outputs of the TSC. It now shows the pictures of the circuit but it only allows you to change the parameters of the pots, not the whole circuit but I plan to add this in due course.

The framework for adding circuits has now been simplified - it now uses a simple config structure to define each circuit.

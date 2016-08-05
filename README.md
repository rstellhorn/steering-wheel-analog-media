# steering-wheel-analog-media
Car Steering Wheel Media button interface to USB HID for Fixed Installation of a Tablet

Analog Steering Wheel Controls Interface

This uses an Analog input to measure the button presses
then sends the appropiate commands to an Android tablet
via keyboard HID shortcuts.

Requires https://github.com/NicoHood/HID

The values below are what I measured. If yours differs
use the analog to serial example sketch to retrieve
new numbers.

Vol Up 47-49
Vol Down 55-57
Seek Up 75-77
Seed Down 63-65
Source Up 167-169
Source Down 282-283
Talk 92-94
Prog 118-120 
normal 1018
with input_pullup

On a 05 chevy silverado the two wires are connected into the BCM
on Connector 3 (Brown) Pin A7 (Lite Green), 
and Connector 2 (Grey) Pin A2 (Dark Blue).
I cut the wires just short of the connectors and spliced them
to wires going to GND and A0 on the Leonardo.

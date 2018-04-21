# Boston Brighton Instruments

This project is based off of the VCVRack Template at https://github.com/vcvrack/template

You can find patches in `examples/` showing off how the modules can be used.

# Modules
## Comparatah
![Comparatah Rack Modules](https://github.com/tnorris/BBI/blob/master/doc/comparatah.png?raw=true)

Comparatah compares the CV on the `IN` port to the knob (0.0v -> 10.0v) below it. If `IN` is greater than the knob, 10.0V is sent out, if it's less than the knob, 0.0V is sent out. I made this module so I could get a clean timing signal out of my PO-24.

## Dividah
![Dividah Rack Module](https://github.com/tnorris/BBI/blob/master/doc/dividah.png?raw=true)

Dividah is a clock frequency divider. `IN` is the numerator, the 4 outputs (1, 2, 4, 8) are the denominators.

## GetRekt
![Get Rekt Rack Module](https://github.com/tnorris/BBI/blob/master/doc/getrekt.png?raw=true)

Get Rekt is a rectifier. It outputs the absolute value of the incoming signal.

## Hooptie
![Hooptie Rack Module](https://github.com/tnorris/BBI/blob/master/doc/hooptie.png?raw=true)

Hooptie takes Gate CV through the `IN` port, and sequentially routes it through the `output` ports. It doesn't always output, though! The higher you turn a step's knob, the more likely it will be to output the CV from `IN`. The `OUT` port at the bottom is a union of the 8 ports above it. When you send a rising edge to RST, the sequence will return back to the start.

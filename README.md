# Boston Brighton Instruments

This project is based off of the VCVRack Template at https://github.com/vcvrack/template

You can find patches in `examples/` showing off how the modules can be used.

# Modules
## Dividah
![Dividah Rack Module](https://github.com/tnorris/BBI/blob/master/doc/dividah.png?raw=true)

Dividah is a clock frequency divider. `IN` is the numerator, the 4 outputs (1, 2, 4, 8) are the denominators.

## Hooptie
![Hooptie Rack Module](https://github.com/tnorris/BBI/blob/master/doc/hooptie.png?raw=true)

Hooptie takes Gate CV through the `IN` port, and sequentially routes it through the `output` ports. It doesn't always output, though! The higher you turn a step's knob, the more likely it will be to output the CV from `IN`. The `OUT` port at the bottom is a union of the 8 ports above it. When you send a rising edge to RST, the sequence will return back to the start.

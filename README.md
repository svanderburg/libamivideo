libamivideo
===========
The hardware of the Commodore Amiga uses different means of storing and
displaying graphics as we use on modern hardware. The Amiga hardware uses a
palette consisting of at most 32 color registers (`ECS` chipset) storing 12-bit
color values or at most 256 color registers (`AGA` chipset) storing 24-bit color
values.

The Amiga stores pixels as bitplanes rather than bytes representing pixel color
index values or a combination of bytes representing RGB values.
In the bitplane format, each bit represents a pixel and this bit is part of the
index value storing the color value of that pixel. For example, if a 32 color
palette is used, the image is stored 5 times in memory, in which the first
bitplane represents to most significant index bit and the last bitplane the least
significant index bit.

Furthermore, the Amiga video chips have special screen modes to display more
colors than the amount of color registers available. The Extra Half Brite (EHB)
screen mode is capable of displaying 64 colors out of a predefined 32, in which
the last 32 colors values are half of the color values of the first 32. The
Hold-and-Modify (HAM) screen mode is used to modify a color component of the
previous adjacent pixel or to provide a new color from the given palette. This
screen mode makes it possible to display all possible color values with some
loss of quality.

Features
========
The purpose of this library is to act as a conversion library for the Amiga
video chips to modern hardware and vice versa. More specifically, this library
offers the following features:

* Converting a 12/24-bit color palette to a 32-bit true color palette
* Converting bitplanes to chunky or true color pixels
* Emulation of the Extra Half-brite (EHB) and Hold-and-Modify (HAM) screen modes

Installation
============
Compilation and installation of this library is straight forward, by using the
standard GNU autotools build instructions:

    $ ./configure
    $ make
    $ make install

More details about the installation process can be found in the `INSTALL` file
included in this package.

License
=======
This library is available under the MIT license

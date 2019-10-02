# TinySPH (UKY Edition)

TinySPH is a parallel 2D Smoothed Particle Hydrodynamics (SPH) code, designed to
run in real time on the Oak Ridge Leadership Computing Facility's "Tiny Titan"
Raspberry Pi cluster. This application was designed to demonstrate distributed
computing concepts and while it works best in a distributed environment it is
possible to run on a shared memory system such as a single multicore processor.
Although the code is designed for Tiny Titan it should, perhaps with some small
build tweaks, compile and run on various flavors of Linux and OS X. The code
makes use of MPI for distributed computing and requires at least two MPI
processes (1 render, 1 compute).

This repository is a fork of the original TinySPH code developed by Adam Simpson
(simpsonab@ornl.gov). Most of the code remains intact following his original
version, with slight modifications and updates as needed for the University of
Kentucky's Tiny Titan build.

## Screenshots

The screenshot below shows TinySPH running on a four core MacBook Pro, running
with 7 compute MPI processes and 1 MPI render process. The default rendering
mode shows a reconstructed liquid surface.

![alt text](https://raw.githubusercontent.com/AdamSimpson/SPH/master/images/SPH_Screenshot_liquid.png "SPH Screenshot liquid")

With the surface reconstruction removed, the underlying physics simulation can
be better viewed, showing SPH's particle nature. The color of the particle
indicates which processor core is responsible for it. TinySPH includes a simple
number-of-particle based load-balancing scheme.

![alt text](https://raw.githubusercontent.com/AdamSimpson/SPH/master/images/SPH_Screenshot.png "SPH Screenshot")

## Building and Running

On the University of Kentucky's Titan Titan cluster, just type `make` to build
and `make run` to run. This will run one process per node by default. If you
would like to run, for example, 4 processes per node, you can type `make PPN=4
run`.

This probably won't work out of the box on anyone else's system.

## Controls

The TinySPH demo can be controlled via keyboard, mouse, and Xbox 360 controller.
Each of the following controls is listed in the format `(KEY1, KEY2):
DESCRIPTION` where `KEY1` is a keyboard key and `KEY2` is the corresponding Xbox
360 controller button, if applicable.

- (`ESC`, `START`): Pause the simulation
- (`TAB`, `BACK`): Bring up the menu (hover over the terminal icon and press
(`A`, `A`) to exit the application
- (`ARROW_KEYS`, `DPAD`): Change the parameters located at the top-left of the
screen
- (`MOUSE_MOVEMENT`, `LEFT_JOYSTICK`): Move the sphere around
- (`A/B/X/Y`, `A/B/X/Y`): Fluid parameter presets
- (`[`, `LEFT_BUMPER`): Decrease number of processors used
- (`]`, `RIGHT_BUMPER`): Increase number of processors used
- (`L`, `LEFT_TRIGGER`): Toggle between particle- and liquid-surface rendering
methods

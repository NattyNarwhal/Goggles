# Goggles

This is a program that applies a filter based on colour temperate to your
screen, similar to programs like Redshift and f.lux. While I make a lot of use
of f.lux and similar built-in mechanisms on modern systems, this one works on
older Windows systems (think Windows 95, RISC NT, etc.). That, and I want to
get a feel for what older development environments were like; this one uses
VC++4.x.

I tried to keep this simple and portable. I only have x86 builds, but I have
done an untested MIPS build, and the x86 binary does work with modern Windows
on ARM (tested with an M1 MacBook Air).

This program uses some code from sscalc for sunrise/sunset calculation.

Documentation is in the RTF file used for docs; while this was pretty neat
to figure out from the "learn retro development" perspective, it impacts it
usability-wise for modern systems.

## How it works

The SetDeviceGammaRamp API does all the lifting. The actual filter is just
applying the appropriate multiplier.

## Graphics devices tested

Since we use SetDeviceGammaRamp, it depends on driver support. I have tested
the application on the following graphics devices and they work:

* 3dfx Voodoo 3
* Intel i855
* Parallels vGPU (on macOS)
  * Note that it changes the host gamma.

The following graphics devices don't work:

* VMware SVGA (errors on API call)

Multimonitor hasn't been tested.

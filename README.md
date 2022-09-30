# SMDI
SMDI - SCSI Musical Data Interchange - is a protocol for exchanging data between music devices over SCSI och MIDI hardware interfaces. The standard was defined by [Peavey](http://www.peavey.com) some time around 1990.

Its primary use is to enable musicians to transfer sample data (sampled sounds) back and forth between sound samplers and computers. A sound sampler is a device which can sample a sound and play it back, possibly using effects, filters, etc. It's a tool used in every professional recording studio. Common sound samplers supporting SMDI are manufactured by AKAI, Roland, EMU, Yamaha etc.

# ESMDI
ESMDI is a tool which implements the SMDI protocol and enables the user to transfer samples to and from a sampler. It is based on the [OpenSMDI](https://github.com/rdmark/OpenSMDI) library written by Christian Nowak. Development of OpenSMDI seems to have ceased at version 0.05 some time around 1999/2000 (the author has been contacted but has not replied). The latest release was under a GPL license, has been patched to work with ESMDI and is included as a submodule in this repo. Both OpenSMDI and ESMDI are written in C.

ESMDI is a console tool initially targeted for Linux but it is probably feasible to port it to other operating systems (the OpenSMDI library supports Windows as well).

The status of the project right now is that it is functional and usable but more features would be nice. There probably (likely) are a few bugs in there somewhere. 

This is a fork from the original [SourceForge project](https://sourceforge.net/projects/esmdi/).

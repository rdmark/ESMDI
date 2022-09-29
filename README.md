# SMDI
SMCDI - SCSI Musical Data Interchange - is a protocol for exchanging data between music devices over SCSI och MIDI hardware interfaces. The standard was defined by [Peavey](http://www.peavey.com) some time around 1990.

Its primary use is to enable musicians to transfer sample data (sampled sounds) back and forth between sound samplers and computers. A sound sampler is a device which can sample a sound and play it back, possibly using effects, filters, etc. It's a tool used in every professional recording studio. Common sound samplers supporting SMDI are manufactured by AKAI, Roland, EMU, Yamaha etc.

# ESMDI
ESMDI is a tool which implements the SMDI protocol and enables the user to transfer samples to and from a sampler. It is based on the [OpenSMDI](https://github.com/rdmark/OpenSMDI) library written by Christian Nowak. Development of OpenSMDI seems to have ceased at version 0.05 some time around 1999/2000 (the author has been contacted but has not replied). The latest release was under a GPL license and I have patched it up a little and plan to include it in my project. Both OpenSMDI and ESMDI are written in C and I prefer to stick with that choice.

ESMDI is a console tool initially targeted for Linux but if there is interest I'll gladly help out expanding its use to other operating systems (the OpenSMDI library supports Windows as well).

The status of the project right now is that it is functional and usable but more features would be nice. There probably (likely) are a few bugs in there somewhere. 

The reason why I want to get it up on SourceForge is that I'd like others to be able to use the tool, and possibly get feedback and help with implementing new features. I haven't seen any other open source/free software tool that implements the SMDI protocol so hopefully it is useful.

This is a fork from the original [SourceForge project](https://sourceforge.net/projects/esmdi/).

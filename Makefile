LIBDIR=/usr/lib/arm-linux-gnueabihf
OPENSMDI=opensmdi

default: esmdi-static

esmdi-static:
	gcc -I$(OPENSMDI) -lpthread esmdi.c $(PWD)/$(OPENSMDI)/bin/linux/smdi.o $(OPENSMDI)/bin/linux/aspi.linux.o $(LIBDIR)/libpthread.a $(LIBDIR)/libc.a $(LIBDIR)/libpopt.a -o $@

esmdi:
	gcc -I$(OPENSMDI) -lpthread -L./$(OPENSMDI)/bin/linux/ -lsmdi esmdi.c $(LIBDIR)/libpopt.a -o $@


clean:
	rm -f esmdi esmdi-static


esmdi2:	$(SMDI_SO_2) smditest.o
	gcc smditest.o -o esmdi $(BINDIR)/smdi.o $(BINDIR)/aspi.linux.o -lpthread -lpopt

esmdi-static2:	$(SMDI_SO_2) smditest.o
	gcc smditest.o -o esmdi-static $(BINDIR)/smdi.o $(BINDIR)/aspi.linux.o $(LIBDIR)/libpthread.a $(LIBDIR)/libpopt.a -static

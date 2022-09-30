
OPENSMDI=opensmdi

default: esmdi-static

esmdi-static:
	gcc -I$(OPENSMDI) -lpthread esmdi.c $(PWD)/$(OPENSMDI)/bin/linux/smdi.o $(OPENSMDI)/bin/linux/aspi.linux.o /usr/lib/libpthread.a /usr/lib/libc.a popt-1.6.4/.libs/libpopt.a -o $@

esmdi:
	gcc -I$(OPENSMDI) -lpthread -L./$(OPENSMDI)/bin/linux/ -lsmdi esmdi.c  popt-1.6.4/.libs/libpopt.a -o $@


clean:
	rm -f esmdi esmdi-static


esmdi2:	$(SMDI_SO_2) smditest.o
	gcc smditest.o -o esmdi $(BINDIR)/smdi.o $(BINDIR)/aspi.linux.o -lpthread -lpopt

esmdi-static2:	$(SMDI_SO_2) smditest.o
	gcc smditest.o -o esmdi-static $(BINDIR)/smdi.o $(BINDIR)/aspi.linux.o /usr/lib/libpthread.a /usr/lib/libpopt.a /usr/lib/libc.a -static

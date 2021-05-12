PREFIX ?= /home/syg/.local

D=$(if $(DESTDIR),$(DESTDIR)/)$(PREFIX)
B=$(D)/$(or $(BINDIR),bin)
T=$(D)/$(or $(DATADIR),share/wtflag/)

CFLAGS ?= -std=c89 -pedantic -Wall -O2 -march=native -D_POSIX_C_SOURCE=200112L -D_XOPEN_SOURCE=500 -DTEMPLATE_DIR=\"$(T)/templates\"

.PHONY: clean install
wtflag: wtflag.o
clean:
	rm -f $(wildcard wtflag *.o)
install: wtflag
	mkdir -p $(B) $(T)
	cp $< $(B)/
	cp -r templates/ $(T)

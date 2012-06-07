CC ?= gcc
LD := $(CC)

PYTHON ?= python

PREFIX ?= /usr/local

CFLAGS += -O3
CFLAGS += -Wall
CFLAGS += -Wextra
CFLAGS += -Wshadow
CFLAGS += -pedantic
CFLAGS += -std=c99

OBJS += src/dada.o
PROG := dada

ifeq ($(strip $(V)),)
	E := @echo
	Q := @
else
	E := @\#
	Q :=
endif

all: test
.PHONY: all

clean:
	$(E) "  CLEAN    "
	$(Q) rm -f $(OBJS) $(PROG)
	$(Q) find . -name *.pyc | xargs rm -f
.PHONY: clean

install: $(PROG)
	$(E) "  INSTALL  " $<
	$(Q) install $< $(PREFIX)/bin/
.PHONY: install

test: $(PROG)
	$(E) "  TEST     "
	$(Q) cd test; $(PYTHON) test_dada.py
.PHONY: test

$(PROG): $(OBJS)
	$(E) "  LINK     " $@
	$(Q) $(LD) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(E) "  COMPILE  " $@
	$(Q) $(CC) $(CFLAGS) -c -o $@ $<

CC ?= gcc
LD := $(CC)

PYTHON ?= python3

PREFIX ?= $(HOME)

CFLAGS += -D_XOPEN_SOURCE=500
CFLAGS += -O3
CFLAGS += -Wall
CFLAGS += -Wextra
CFLAGS += -Wshadow
CFLAGS += -pedantic
CFLAGS += -std=c99

OBJS += dada.o
PROG := dada

ifeq ($(strip $(V)),)
	E := @echo
	Q := @
else
	E := @\#
	Q :=
endif

all: $(PROG)
.PHONY: all

clean:
	$(E) "  CLEAN    "
	$(Q) $(RM) $(OBJS) $(PROG)
.PHONY: clean

install: $(PROG)
	$(E) "  INSTALL  " $<
	$(Q) install $< $(PREFIX)/bin/
.PHONY: install

test: $(PROG)
	$(E) "  TEST     "
	$(Q) $(PYTHON) test_dada.py
.PHONY: test

$(PROG): $(OBJS)
	$(E) "  LINK     " $@
	$(Q) $(LD) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(E) "  COMPILE  " $@
	$(Q) $(CC) $(CFLAGS) -c -o $@ $<

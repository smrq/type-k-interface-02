SRCDIR=src
OBJDIR=obj
BINDIR=bin
MCU=atmega32u4
CPU=16000000

CWD := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))

CFLAGS+=-Wall -Wextra -Werror=implicit-function-declaration -std=gnu11
OPTFLAGS+=-O2
EXTOPTFLAGS+=-flto
ARCHFLAGS+=-mmcu=$(MCU) -DF_CPU=$(CPU)

ifeq ($(OS),Windows_NT)
	REVERSE_SLASHES = $(subst /,\,$1)
	MKDIRP = if not exist $(call REVERSE_SLASHES,$1) (mkdir $(call REVERSE_SLASHES,$1))
	RMRF = if exist $(call REVERSE_SLASHES,$1) (rmdir /s /q $(call REVERSE_SLASHES,$1))
else
	MKDIRP = mkdir -p $1
	RMRF = rm -rf $1
endif

DOCKER_IMG := lpodkalicki/avr-toolchain
RUN := docker run --rm -v $(CWD)$(SRCDIR):/build/$(SRCDIR) -v $(CWD)$(OBJDIR):/build/$(OBJDIR) -v $(CWD)$(BINDIR):/build/$(BINDIR) $(DOCKER_IMG)

SRCS := $(shell $(RUN) find $(SRCDIR) -type f -name '*.c')
OBJS := $(SRCS:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
TARGET := unit_02

all: $(BINDIR)/$(TARGET).hex

.PHONY: all clean flash
.SECONDARY:

$(SRCDIR)/font/font.c: $(SRCDIR)/font/generate.js
	cd $(SRCDIR)/font && yarn && yarn start

$(BINDIR)/$(TARGET).elf: $(SRCS)
	$(call MKDIRP,$(@D))
	$(RUN) avr-gcc $(OPTFLAGS) $(EXTOPTFLAGS) $(ARCHFLAGS) $^ -o $@
	$(RUN) avr-size $@

$(BINDIR)/%.hex: $(BINDIR)/%.elf
	$(RUN) avr-objcopy -j .text -j .data -O ihex $< $@

$(BINDIR)/%.objdump: $(BINDIR)/%.elf
	$(RUN) avr-objdump -D -S -s $< > $@

clean:
	$(call RMRF,$(OBJDIR))
	$(call RMRF,$(BINDIR))

flash: $(BINDIR)/$(TARGET).hex
	$(CWD)vendor/dfu-programmer-win-0.7.2/dfu-programmer atmega32u4 erase --force
	$(CWD)vendor/dfu-programmer-win-0.7.2/dfu-programmer atmega32u4 flash $(BINDIR)/$(TARGET).hex
	$(CWD)vendor/dfu-programmer-win-0.7.2/dfu-programmer atmega32u4 reset

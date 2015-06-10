TARGET:= base64


DEBUG?=no

CC:=gcc
LD:=gcc
CFLAGS:=-Wall -Wunused -Werror
LDFLAGS:=
CP=cp
RM:=rm


ifeq ($(DEBUG),yes)
CFLAGS+=-O0 -g -DDEBUG
else
CFLAGS+=-O3
endif

# options for verbosity compilation
ifneq ($(V), 1)
Q := @
echo-cmd = @echo $(1)
else
Q :=
echo-cmd :=
endif

INSTALLDIR:=/usr/local/bin

SRCS:=main.c
SRCS+=base64.c


OBJS:=$(SRCS:%.c=%.o)


all: $(TARGET)

base64: $(OBJS)
	$(call echo-cmd,"LD   $@")
	$(Q)$(LD) -o $@ $^ $(LDFLAGS)


%.o: %.c
	$(call echo-cmd,"CC   $@")
	$(Q)$(CC) -o $@ -c $< $(CFLAGS)


.PHONY: clean install

install: $(TARGET)
	$(call echo-cmd,"INSTALL")
	$(Q)$(CP) $(TARGET) $(INSTALLDIR)

uninstall:
	$(call echo-cmd,"UNINSTALL")
	$(Q)$(RM) -f $(INSTALLDIR)/$(TARGET)

clean:
	$(call echo-cmd,"CLEAN")
	$(Q)$(RM) -fr $(TARGET) *.o *~

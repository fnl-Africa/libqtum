# tools to use
CC = i686-qtum-gcc
AR = i686-qtum-ar
AS = i686-qtum-as

HDRS = include/qtum.h

C_QTUM_SRC = src/qtum.c
C_QTUM_OBJS = $(subst .c,.o,$(C_QTUM_SRC))

S_QTUM_SRC = src/syscall.S
S_QTUM_OBJS = $(subst .S,.o,$(S_QTUM_SRC))

CFLAGS ?= -Wall -O3  -Wno-unused-function
CFLAGS += -Iinclude

OUTPUTS = libqtum.a

default: build

build: $(OUTPUTS)

libqtum.a: $(C_QTUM_OBJS) $(S_QTUM_OBJS)
	$(AR) crs libqtum.a $(C_QTUM_OBJS) $(S_QTUM_OBJS)

$(C_QTUM_OBJS): $(HDRS) $(C_QTUM_SRC)
	$(CC) $(CFLAGS) -c $*.c -o $@

$(S_QTUM_OBJS): $(HDRS) $(S_QTUM_SRC)
	$(AS) $*.S -o $@

clean:
	rm -f $(C_QTUM_OBJS) $(OUTPUTS) $(S_QTUM_OBJS)

deploy:
	echo "Warning! Do not use sudo if this fails!!"
	cp libqtum.a $(SYSROOT)/usr/lib/
	cp include/* $(SYSROOT)/usr/include/




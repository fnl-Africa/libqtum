# tools to use
CROSS_CC = i686-qtum-gcc
CROSS_AR = i686-qtum-ar
CROSS_AS = i686-qtum-as

HDRS = include/qtum.h include/shared-x86.h

C_QTUM_SRC = src/qtum.c
C_QTUM_OBJS = $(subst .c,.o,$(C_QTUM_SRC))

S_QTUM_SRC = src/syscall.S
S_QTUM_OBJS = $(subst .S,.o,$(S_QTUM_SRC))

C_MOCKS_SRC = mocks/qtummock.c
C_MOCKS_OBJS = $(subst .c,.o,$(C_MOCKS_SRC))

CROSS_CFLAGS ?= -Wall -Os -Wno-unused-function -ffunction-sections -fdata-sections
CROSS_CFLAGS += -Iinclude -DLIBQTUM_BUILD

CFLAGS += -DQTUM_MOCK -Iinclude

OUTPUTS = libqtum.a libmockqtum.a

default: build

build: $(OUTPUTS)

libqtum.a: $(C_QTUM_OBJS) $(S_QTUM_OBJS)
	$(CROSS_AR) crs libqtum.a $(C_QTUM_OBJS) $(S_QTUM_OBJS)

libmockqtum.a: $(C_MOCKS_OBJS)
	$(AR) crs libmockqtum.a $(C_MOCKS_OBJS)

$(C_MOCKS_OBJS): $(HDRS) $(C_MOCKS_SRC)
	$(CC) $(CFLAGS) -c $*.c -o $@

$(C_QTUM_OBJS): $(HDRS) $(C_QTUM_SRC)
	$(CROSS_CC) $(CROSS_CFLAGS) -c $*.c -o $@

$(S_QTUM_OBJS): $(HDRS) $(S_QTUM_SRC)
	$(CROSS_AS) $*.S -o $@

clean:
	rm -f $(C_QTUM_OBJS) $(OUTPUTS) $(S_QTUM_OBJS) $(C_MOCKS_OBJS)

deploy:
	echo "Warning! Do not use sudo if this fails!!"
	cp libqtum.a $(SYSROOT)/usr/lib/
	cp include/* $(SYSROOT)/usr/include/




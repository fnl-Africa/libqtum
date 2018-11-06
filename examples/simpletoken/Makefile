# files to be built
# fill this in as a template
HDRS = 
C_SRC = simpletoken.c
OUTPUT = simpletoken.contract
LIBS = 

# tools to use
CC = i686-qtum-gcc
CXX = i686-qtum-g++
AR = i686-qtum-ar
AS = i686-qtum-as
LD = i686-qtum-ld

C_OBJS = $(subst .c,.o,$(C_SRC))

#default flags will just remove dead code and give warnings
CFLAGS += -Wall -ffunction-sections -fdata-sections
LDFLAGS += -Wl,--gc-section


default: build

build: $(OUTPUT)

$(OUTPUT): $(C_OBJS)
	$(CC) $(LDFLAGS) -o $(OUTPUT) $(C_OBJS) $(LIBS)

$(C_OBJS): $(HDRS) $(C_SRC)
	$(CC) $(CFLAGS) -c $*.c -o $@


clean:
	rm -f $(C_OBJS) $(OUTPUT)


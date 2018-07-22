.PHTONY: all
ROOT = ..
include $(ROOT)/Make.defines
SRCS = lab.c
OBJS = $(SRCS:.c=.o)
CFLAGS=
all: $(LIB)

$(LIB): $(OBJS)
	$(AR) rv $(LIB) $^

$(OBJS): $(SRCS)


clean:
	rm -rf *.o $(LIB)
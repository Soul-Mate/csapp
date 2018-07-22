ROOT = ..

# import define
include $(ROOT)/Make.defines

# source file
SRCS := main.c signal.c job.c shell.c command.c

# program name
PROG = shell

OBJS = $(SRCS:.c=.o)

# test_job: test_job.o job.o
# 	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

# test_job.o: test_job.c
# 	$(CC) -c $^ -o $@ -I../lib

# job.o: job.c
# 	$(CC) -c $^ -o $@ -I../lib

$(PROG): $(OBJS) $(LIBLAB)
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

$(OBJS): $(SRCS)

#create liblab.a
include $(ROOT)/Make.liblab.inc

clean:
	rm -rf *.o $(PROG)
	rm -rf ../lib/*.a ../lib/*.o
.SUFFIXES : .c .o

OBJS = socket_client.o
SRCS = $(OBJS:.o=.c)
TARGET = simple_client 

CC = gcc
#CFLAGS = -g -c
CFLAGS = -c
LDFLAGS =
LDLIBS = 

$(TARGET): $(OBJS)
	$(CC) -o $@ $(OBJS) $(LDLIBS)
	rm -f $(OBJS)

.c.o:
	$(CC) $(INC) $(CFLAGS) $<

clean:
	rm -f $(TARGET) $(OBJS)



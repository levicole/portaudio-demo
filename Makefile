CC = gcc
CFLAGS = -Wall -g

INCLUDES = -I/usr/local/include
LFLAGS = -L/usr/local/lib

LIBS = -lsndfile -lportaudio

SRCS = demo.c

OBJS = $(SRCS:.c=.o)

MAIN=demo

all: $(MAIN)

$(MAIN): $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $(MAIN) $(OBJS) $(LFLAGS) $(LIBS)

clean:
	$(RM) *.o *~ $(MAIN)

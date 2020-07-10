CC = gcc
CFLAGS = -Wall -g

INCLUDES = -I/usr/local/include
LFLAGS = -L/usr/local/lib

LIBS = -lsndfile -lportaudio

SRCS = hello-sine.c

OBJS = $(SRCS:.c=.o)

MAIN=hello-sine

all: $(MAIN)

$(MAIN): $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $(MAIN) $(OBJS) $(LFLAGS) $(LIBS)

clean:
	$(RM) *.o *~ $(MAIN)

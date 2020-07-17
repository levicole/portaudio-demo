CC = gcc
CFLAGS = -Wall -g -Wextra -Isrc

INCLUDES = -I/usr/local/include
LFLAGS = -L/usr/local/lib

LIBS = -lsndfile -lportaudio -lreadline

SRCS=$(wildcard src/**/*.c src/*.c)

OBJS = $(SRCS:.c=.o)

MAIN=build/demo

all: $(MAIN)

$(MAIN): $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $(MAIN) $(OBJS) $(LFLAGS) $(LIBS)

clean:
	$(RM) $(OBJS) $(MAIN)

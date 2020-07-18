CC = gcc
CFLAGS = -Wall -g -Wextra -Isrc

INCLUDES = -I/usr/local/include
LFLAGS = -L/usr/local/lib

LIBS = -lsndfile -lportaudio -lreadline

ifeq ($(OS),Windows_NT)
	MAIN=build/demo.exe
	LIBS := $(LIBS) -lmingw32 -lSDL2main -lSDL2 -lopengl32 -lm -lGLU32
else
	MAIN=build/demo
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S),Darwin)
		LIBS := $(LIBS) -lSDL2 -framework OpenGL -lm
	else
		LIBS := $(LIBS) -lSDL2 -lGL -lGLU
	endif
endif

SRCS=$(wildcard src/**/*.c src/*.c)

OBJS = $(SRCS:.c=.o)


all: $(MAIN)

$(MAIN): $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $(MAIN) $(OBJS) $(LFLAGS) $(LIBS)

clean:
	$(RM) $(OBJS) $(MAIN)

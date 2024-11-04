CC ?= gcc
CFLAGS = -Wall -Werror -O2
SRC = sigma_rizzler.c
NAME = sigma

all: build

build: 
	$(CC) $(CFLAGS) $(SRC) -o $(NAME)

clean:
	rm $(NAME)
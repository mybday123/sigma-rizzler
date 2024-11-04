CC ?= gcc
CFLAGS = -Wall -Werror -O2
SRC = sigma_rizzler.c
NAME = sigma

all: build

build: sigma_rizzler
	$(CC) $(CFLAGS) $(SRC) -o $(NAME)

clean:
	rm $(NAME)
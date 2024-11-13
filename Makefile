CC ?= gcc
STRIP ?= strip
CFLAGS = -Wall -Werror -O2
SRC = sigma_rizzler.c
NAME = sigma

all: build

build: 
	$(CC) $(CFLAGS) $(SRC) -o $(NAME)
	$(STRIP) $(NAME)

clean:
	rm $(NAME) $(NAME).exe
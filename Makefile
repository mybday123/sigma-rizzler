CC ?= gcc
STRIP ?= strip
CFLAGS = -O2
WERROR = 1
WFLAGS = -Wall -Werror
SRC = sigma_rizzler.c
NAME = sigma

ifeq ($(WERROR), 1)
	CFLAGS += $(WFLAGS)
endif

all: build

build: 
	$(CC) $(CFLAGS) $(SRC) -o $(NAME)
	$(STRIP) $(NAME)

clean:
	rm -f $(NAME) $(NAME).exe

CC ?= gcc
CC_WINDOWS ?= x86_64-w64-mingw32-gcc
STRIP ?= strip
CFLAGS = -O2
WERROR = 1
WFLAGS = -Wall -Werror
SRC = sigma_rizzler.c
NAME = sigma

ifeq ($(WERROR), 1)
	CFLAGS += $(WFLAGS)
endif

all: linux windows

linux: 
	$(CC) $(CFLAGS) $(SRC) -o $(NAME)
	$(STRIP) $(NAME)

windows: 
	$(CC_WINDOWS) $(CFLAGS) $(SRC) -o $(NAME).exe
	$(STRIP) $(NAME).exe

clean:
	rm -f $(NAME) $(NAME).exe

help:
	@echo "Available targets:"
	@echo "  all        - Compile both Linux and Windows targets"
	@echo "  linux      - Compile Linux target"
	@echo "  windows    - Compile Windows Target"
	@echo "  clean      - Remove all build files"
	@echo "  help       - Show this message"

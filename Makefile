FINDINCFOLDER := $(addprefix -I,$(wildcard ./*/inc))
FINDSRCFOLDER := $(wildcard */src/*.c)
OBJ := $(patsubst %.c,%.o,$(FINDSRCFOLDER))
CC := gcc
CFLAGS := -g -Wall -std=gnu11 $(FINDINCFOLDER) -liw
BUILDFOLDER := build
BUILDOBJ := $(wildcard $(BUILDFOLDER)/*.o)

all: clean prepare build main

prepare:
	mkdir $(BUILDFOLDER)

build: $(OBJ)
	
%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)
	mv $@ $(BUILDFOLDER)/$(notdir $@)

main: $(BUILDOBJ)
	$(CC) -o $@ main.c $? $(CFLAGS)

clean:
	rm -rfv $(BUILDFOLDER)
	rm -f main
	
	
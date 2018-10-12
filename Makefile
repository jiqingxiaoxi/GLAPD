#!/usr/bin/env make
all:
	gcc single.c -o Single -lm
	gcc LAMP.c -o LAMP -lm

clean:
	rm -f Single
	rm -f LAMP


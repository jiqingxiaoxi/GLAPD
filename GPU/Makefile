#!/usr/bin/env make
all:
	nvcc -arch sm_20 single.cu -o Single
	nvcc -arch sm_20 LAMP.cu -o LAMP

clean:
	rm -f Single
	rm -f LAMP

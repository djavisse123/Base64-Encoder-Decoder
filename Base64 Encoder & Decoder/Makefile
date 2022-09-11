CC = gcc
CFlags = -Wall -std=c99

all: encode decode

encode.o: encode.c state24.h filebuffer.h
	gcc -g -Wall -std=c99   -c -o encode.o encode.c
decode.o: decode.c state24.h filebuffer.h
	gcc -g -Wall -std=c99   -c -o decode.o decode.c
state24.o: state24.c state24.h
	gcc -g -Wall -std=c99   -c -o state24.o state24.c
filebuffer.o: filebuffer.c filebuffer.h
	gcc -g -Wall -std=c99   -c -o filebuffer.o filebuffer.c

encode: encode.o state24.o filebuffer.o
	gcc encode.o state24.o filebuffer.o -o encode

decode: decode.o state24.o filebuffer.o
	gcc decode.o state24.o filebuffer.o -o decode

clean:
	rm -f encode.o decode.o filebuffer.o state24.o
	rm -f encode
	rm -f decode
	rm -f output.txt
	rm -f stderr.txt
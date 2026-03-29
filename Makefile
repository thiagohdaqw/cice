
all: bin stun_server main

bin:
	mkdir -p bin

main: src/main.c
	gcc -o bin/main src/main.c

stun_server: src/stun_server.c
	gcc -o bin/stun_server src/stun_server.c

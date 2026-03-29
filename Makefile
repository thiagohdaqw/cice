
all: bin stun_server

bin:
	mkdir -p bin

stun_server: src/stun_server.c
	gcc -o bin/stun_server src/stun_server.c

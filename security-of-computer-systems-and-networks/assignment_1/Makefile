CC ?= gcc

C_SERVER_FLAGS = -fno-stack-protector -z execstack -g
C_SHELL_FLAGS = -nostartfiles -no-pie

SRC = vulnerable_server.c
OUT = vulnerable_server

server:
	$(CC) $(C_SERVER_FLAGS) vulnerable_server.c -o vulnerable_server

shell:
	$(CC) $(C_SHELL_FLAGS) shell.s -o shell

clean:
	rm -f vulnerable_server shell

.PHONY: all clean
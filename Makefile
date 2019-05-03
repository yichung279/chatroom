server:ser.c
	gcc ser.c -o ser && ./ser

client:cli.c
	gcc cli.c -o cli && ./cli $(host)

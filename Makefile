


server: ser.c lib/q_conn.c lib/q_conn.h
	  gcc ser.c -o ser && ./ser

client: cli.c
	gcc cli.c -o cli && ./cli

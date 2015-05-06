CC = gcc

FILES = client.c
FILE = server.c
OUTFILES = Client
OUTFILE = Server
FLAGG = -lSDL2_net -lSDL2 -lSDL2_image -lSDL2_mixer

Build: $(FILES)
	$(CC) $(FILES) $(FLAGG) -o $(OUTFILES)
Build: $(FILE)
	$(CC) $(FILE) $(FLAGG) -o $(OUTFILE)

clean:
	rm -f *.o core

rebuild: clean Build

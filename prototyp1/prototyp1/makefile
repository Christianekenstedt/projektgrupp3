CC = gcc

FILES = main.c clientFunctions.c
OUTFILE = main
FLAGG = -lSDL2_net -lSDL2 -lSDL2_image -lSDL2_mixer

Build: $(FILES) 
	$(CC) $(FILES) $(FLAGG) -o $(OUTFILE)
clean:
	rm -f *.o core

rebuild: clean Build

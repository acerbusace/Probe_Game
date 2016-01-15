OBJ = main.o serverClient.o game.o

probe: $(OBJ)
	gcc -o probe $(OBJ)

main.o: main.c defs.h
	gcc -c main.c

serverClient.o: serverClient.c defs.h
	gcc -c serverClient.c

game.o: game.c defs.h
	gcc -c game.c

clean: 
	rm probe $(OBJ)

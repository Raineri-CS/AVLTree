prog:
	gcc -o prog main.c -std=c99 -I$(PWD)/gfx -L$(PWD)/gfx -lgfx -lSDL -lSDL_gfx -lSDL_ttf -Wall -Wextra -pedantic

all:
	make -C gfx;\
	make;\

all-clean:
	make -C gfx clean
	make -C gfx 
	make clean && make

clean-all:
	make -C gfx clean
	make clean;\

debug:
	make -C gfx clean
	make -C gfx 
	gcc -g -o prog main.c -std=c99 -I$(PWD)/gfx -L$(PWD)/gfx -lgfx -lSDL -lSDL_gfx -lSDL_ttf -Wall -Wextra -pedantic

clean:
	-rm prog 

#all:
#	gcc -lglut -LGL -L/usr/lib main.c -o example
#

all:
	g++ -lGL -lGLU -lGLEW -lSDL -L/usr/lib main.cpp shader.cpp -o proj

clean:
	rm -rf *.o proj

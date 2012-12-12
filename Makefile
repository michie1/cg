all:
	g++ -lGL -lGLU -lGLEW -lSDL -lSDL_mixer -L/usr/lib main.cpp shader.cpp -o fireworks

clean:
	rm -rf *.o fireworks

#include <cstdlib>
#include <iostream>

#include "GL/glew.h"
#include "GL/gl.h"
#include "GL/glu.h"
#include "SDL/SDL.h"

#include "world.h"

int main(int argc, char** argv)
{
	World w;
	w.hoi();	

	int width = 500;
	int height = 500;
	bool running = true;


	SDL_Init(SDL_INIT_EVERYTHING);

	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glEnable(GL_DEPTH_TEST);
	gluPerspective(45, (float)width/height, .1, 100);
	glMatrixMode(GL_MODELVIEW);	


	SDL_SetVideoMode(width, height, 24, SDL_OPENGL | SDL_GL_DOUBLEBUFFER);

	while(running) {


		SDL_Event event;
		while(SDL_PollEvent(&event)) {
			switch(event.type) {
				case SDL_QUIT: 
					running = false;  
					break;
				case SDL_KEYDOWN: 
					if(event.key.keysym.sym == SDLK_ESCAPE) {
						running = false;
					}
			}
		}	


		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
		glLoadIdentity();

		// Doe hier leuke dingen

	
		SDL_GL_SwapBuffers();

	}

	SDL_Quit();
	return 0;
}



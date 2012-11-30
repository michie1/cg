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
	int color = 0;
	int rotate = 0;


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
					} else if(event.key.keysym.sym == SDLK_LEFT) {
						color = 1;
						rotate = 1;
					} else if(event.key.keysym.sym == SDLK_RIGHT) {
						color = 0;
						rotate = 0;
					}
			}
		}	


		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		

		// Doe hier leuke dingen
		//
		//

		glRotatef(30, 1, 1, 1);
		
		// x axis
		glBegin(GL_LINES);
			glColor3f(1, 0, 0); // red
			glVertex3f(0, 0, 0);
			glVertex3f(1, 0, 0);
		glEnd();
	
		// y axis
		glBegin(GL_LINES);
			glColor3f(0, 1, 0); // green
			glVertex3f(0, 0, 0);
			glVertex3f(0, 1, 0);
		glEnd();

		// z axis
		glBegin(GL_LINES);
			glColor3f(0, 0, 1); // blue
			glVertex3f(0, 0, 0);
			glVertex3f(0, 0, 1);
		glEnd();

		glPushMatrix();
		if(rotate == 1) {
			glTranslatef(0.15, 0.15, 0);
			glRotatef(45, 0, 0, 1);
			glTranslatef(-0.15, -0.15, 0);
		} 

	 	// front
		glBegin(GL_QUADS);
			if(color == 0 ) {
				glColor3f(1, 0.5, 0.5);
			} else if(color == 1) {
				glColor3f(0, 0, 1);
				//color = 0;
			}
			glVertex3f(0.1, 0.1, 1); 
			glVertex3f(0.2, 0.1, 1);
		 	glVertex3f(0.2, 0.2, 1); 
			glVertex3f(0.1, 0.2, 1); 
		glEnd();
		glPopMatrix();


		// back
		glBegin(GL_QUADS);
			if(color == 0 ) {
				glColor3f(1, 0.5, 0.5);
			} else if(color == 1) {
				glColor3f(0, 0, 1);
				//color = 0;
			}
			glVertex3f(0.1, 0.1, 0); 
			glVertex3f(0.2, 0.1, 0);
		 	glVertex3f(0.2, 0.2, 0); 
			glVertex3f(0.1, 0.2, 0); 
		glEnd();
		glPopMatrix();

		// left
		glBegin(GL_QUADS);
			glColor3f(1, 1, 0.5);
			glVertex3f(0.1, 0.1, 0); 
			glVertex3f(0.1, 0.2, 0);
		 	glVertex3f(0.1, 0.2, 1); 
			glVertex3f(0.1, 0.1, 1); 
		glEnd();
		glPopMatrix();


		
	
		SDL_GL_SwapBuffers();

	}

	SDL_Quit();
	return 0;
}



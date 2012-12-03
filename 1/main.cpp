#include <cstdlib>
#include <iostream>

#include "GL/glew.h"
#include "GL/gl.h"
#include "GL/glu.h"
#include "SDL/SDL.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

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


	SDL_Init(SDL_INIT_EVERYTHING); // SDL_INIT_VIDEO:w


	//glViewport(0, 0, width, height);
	//glMatrixMode(GL_PROJECTION);
	//glEnable(GL_DEPTH_TEST);
	//gluPerspective(45, (float)width/height, 1, 1000);
	//glMatrixMode(GL_MODELVIEW);	


	SDL_SetVideoMode(width, height, 24, SDL_OPENGL | SDL_GL_DOUBLEBUFFER);


	/*

	GLuint program_object;
	GLuint vertex_shader;
	GLuint fragment_shader;

	glm::mat4 projectionMatrix;
	glm::mat4 viewMatrix;
	glm::mat4 modelMatrix;


	//program_object = glCreateProgram();
	//vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	//fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

	projectionMatrix = glm::perspective(60.0f, (float)width/height, 0.1f, 1000.0f);
	viewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.f));
	modelMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));

	GLuint p;


	GLuint v, g, f;
	char *vs,*gs, *fs;
			 
	// Create shader handlers
	v = glCreateShader(GL_VERTEX_SHADER);
	g = glCreateShader(GL_GEOMETRY_SHADER);
	f = glCreateShader(GL_FRAGMENT_SHADER);

	// Read source code from files
	vs = textFileRead("example.vert");
	gs = textFileRead("example.geom");
	fs = textFileRead("example.frag");

	const char * vv = vs;
	const char * gg = gs;
	const char * ff = fs;

	// Set shader source
	glShaderSource(v, 1, &vv,NULL);
	glShaderSource(g, 1, &gg,NULL);
	glShaderSource(f, 1, &ff,NULL);

	free(vs);free(gs);free(fs);

	// Compile all shaders
	glCompileShader(v);
	glCompileShader(g);
	glCompileShader(f);

	// Create the program
	p = glCreateProgram();

	// Attach shaders to program
	glAttachShader(p,v);
	glAttachShader(p,g);
	glAttachShader(p,f);

	// Link and set program to use
	glLinkProgram(p);
	glUseProgram(p);

	*/


	float fTriangle[9]; // Data to render triangle (3 vertices, each has 3 floats)

	float fTriangleColor[9]; 


	GLuint uiVBO[4];
	GLuint uiVAO[2];




	//glEnable(GL_DEPTH_TEST);
	//glClearColor(0.0f, 0.5f, 1.0f, 1.0f);

	//glewInit();


	// Setup triangle vertices
	fTriangle[0] = -0.4f; fTriangle[1] = 0.1f; fTriangle[2] = 0.0f;
	fTriangle[3] = 0.4f; fTriangle[4] = 0.1f; fTriangle[5] = 0.0f;
	fTriangle[6] = 0.0f; fTriangle[7] = 0.7f; fTriangle[8] = 0.0f;

	//glGenVertexArrays(2, uiVAO);

	//glGenVertexArrays(2, uiVAO); // Generate two VAOs, one for triangle and one for quad
	//glGenBuffers(4, uiVBO); // And four VBOs
 
	/*
	glBindVertexArray(uiVAO[0]);

	glBindBuffer(GL_ARRAY_BUFFER, uiVBO[0]);
	glBufferData(GL_ARRAY_BUFFER, 9*sizeof(float), fTriangle, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, uiVBO[1]);
	glBufferData(GL_ARRAY_BUFFER, 9*sizeof(float), fTriangleColor, GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0); 
	*/
	

	while(running) {

		
	//glClear(GL_COLOR_BUFFER_BIT);
	//glEnableVertexAttribArray(0);

	//glBindBuffer(GL_ARRAY_BUFFER, uiVBO[0]);

	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//glDrawArrays(GL_TRIANGLES, 0, 3);

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

		/*

		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		

		// Doe hier leuke dingen
		//
		//

		//glRotatef(30, 1, 1, 1);

		// x axis
		glBegin(GL_LINES);
			glColor3f(1, 0, 0); // red
			glVertex3f(0, 0, 0);
			glVertex3f(10, 0, 0);
		glEnd();
	
		// y axis
		glBegin(GL_LINES);
			glColor3f(0, 1, 0); // green
			glVertex3f(0, 0, 0);
			glVertex3f(0, 10, 0);
		glEnd();

		// z axis
		glBegin(GL_LINES);
			glColor3f(0, 0, 1); // blue
			glVertex3f(0, 0, 0);
			glVertex3f(0, 0, -10);
		glEnd();

		glPushMatrix();
		

		// weg
		glBegin(GL_QUADS);
			glColor3f(0, 1, 1); // paars
			glVertex3f(0, 0, 0);
			glVertex3f(1, 0, 0);
			glVertex3f(1, 0, -1);
			glVertex3f(0, 0, -1);
		glEnd();

		
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
			glVertex3f(0.1, 0.1, -1); 
			glVertex3f(0.2, 0.1, -1);
		 	glVertex3f(0.2, 0.2, -1); 
			glVertex3f(0.1, 0.2, -1); 
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
		 	glVertex3f(0.1, 0.2, -1); 
			glVertex3f(0.1, 0.1, -1); 
		glEnd();
		glPopMatrix();

		*/

		SDL_GL_SwapBuffers();

	}

	SDL_Quit();
	return 0;
}



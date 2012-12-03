#include <cstdlib>
#include <iostream>

#include "GL/glew.h"
#include "GL/gl.h"
#include "GL/glu.h"
#include "SDL/SDL.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "shader.h"


class Control {
	private:
		void handleEvents();
		int iWidth;
		int iHeight;
		bool running;
		float fRotationAngle;
		GLuint uiVAO[2];
		GLuint uiVBO[4];
		GLuint projMatrixLoc, viewMatrixLoc;
		glm::mat4 projMatrix, viewMatrix;
		GLuint programID;

	public:
		Control();
		void initialize();
		void deinitialize();
		void prepare();
		void run();
		void render();
		void setWidth(int w);
		void setHeight(int h);
};

// Public

Control::Control() {

}

void Control::initialize() {
	// SDL
	SDL_Init(SDL_INIT_EVERYTHING); 
	SDL_SetVideoMode(iWidth, iHeight, 32, SDL_OPENGL | SDL_GL_DOUBLEBUFFER);
	SDL_EnableKeyRepeat(10, 10);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 32);
	SDL_WM_SetCaption("Computer Graphics Final Project 2012", NULL);		

	// GLEW
	glewInit();

	// GL
	glViewport(0, 0, iWidth, iHeight);
	glClearColor(0.0f, 0.5f, 1.0f, 1.0f); 
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	glDepthMask(GL_TRUE);
	glClearDepth(1.0f);
}

void Control::deinitialize() {
	
	glDeleteBuffers(4, uiVBO);
	glDeleteVertexArrays(2, uiVAO);

	SDL_Quit();
}

void Control::prepare() {
	//GLuint uiVAO[2];
	//GLuint uiVBO[4];
	fRotationAngle = 0.0f;

	float fTriangle[] = {
		-0.4f, 0.1f, -1.0f,
		0.4f, 0.1f, -1.0f,
		0.0f, 0.7f, -1.0f };
	
	float fTriangleColor[] = {
		1.0f, 0, 0,
		0, 1.0f, 0,
		0, 0, 1.0f };

	float fLine[] = {
		-1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
 		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, 1.0f };

	float fLineColor[] = {
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 1.0f,
 		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f	};

	glGenVertexArrays(1, uiVAO);
	glGenBuffers(4, uiVBO); 

	// Triangle
	glBindVertexArray(uiVAO[0]);

	glBindBuffer(GL_ARRAY_BUFFER, uiVBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(fTriangle), fTriangle, GL_STATIC_DRAW); 
	glEnableVertexAttribArray(0);
 	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	
	glBindBuffer(GL_ARRAY_BUFFER, uiVBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(fTriangleColor), fTriangleColor, GL_STATIC_DRAW); 
	glEnableVertexAttribArray(1);
 	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);


	// Line
	

	glBindVertexArray(uiVAO[1]);
	
	glBindBuffer(GL_ARRAY_BUFFER, uiVBO[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(fLine), fLine, GL_STATIC_DRAW); 
	glEnableVertexAttribArray(0);
 	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, uiVBO[3]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(fLineColor), fLineColor, GL_STATIC_DRAW); 
	glEnableVertexAttribArray(1);
 	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);



//	GLuint programID = LoadShaders("shader.vert", "shader.frag");
	programID = LoadShaders("shader.vert", "shader.frag");

	
	//GLuint projMatrixLoc, viewMatrixLoc;
	//glm::mat4 projMatrix, viewMatrix;

	projMatrixLoc = glGetUniformLocation(programID, "projectionMatrix");
	viewMatrixLoc = glGetUniformLocation(programID, "viewMatrix");

}

void Control::run() {
	running = true;

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
						fRotationAngle -= 5.0f;
					} else if(event.key.keysym.sym == SDLK_RIGHT) {
						fRotationAngle += 5.0f;
					}
					break;
			}
		}
		
		render();
	}

}

void Control::render() {
		

		viewMatrix = glm::lookAt(glm::vec3(-1.0f, 1.0f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		projMatrix = glm::perspective(45.0f, (float) iWidth / (float) iHeight, 0.1f, 100.0f);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

		glUseProgram(programID);

		glUniformMatrix4fv(projMatrixLoc, 1, GL_FALSE, glm::value_ptr(projMatrix));
		glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE, glm::value_ptr(viewMatrix));


		glm::mat4 mCurrent;

		//viewMatrix = glm::mat4(1.0);

		// Triangles
		glBindVertexArray(uiVAO[0]);


		mCurrent = glm::rotate(viewMatrix, fRotationAngle, glm::vec3(0.0f, 1.0f, 0.0f));
		//mCurrent = glm::translate(viewMatrix, glm::vec3(0.0f, -1.0f, 0.0f));
		//mCurrent = glm::rotate(mCurrent, fRotationAngle, glm::vec3(0.0f, 1.0f, 0.0f));
		//mCurrent = glm::translate(mCurrent, glm::vec3(0.0f, 0.0f, 1.0f));
    glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE, glm::value_ptr(mCurrent)); 
		glDrawArrays(GL_TRIANGLES, 0, 3);

		
		mCurrent = glm::translate(viewMatrix, glm::vec3(0.0f, -1.0f, 0.0f));
    glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE, glm::value_ptr(mCurrent)); 
		glDrawArrays(GL_TRIANGLES, 0, 3);
		

		// Lines
		
		glBindVertexArray(uiVAO[1]);
		
		mCurrent = viewMatrix;

    glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE, glm::value_ptr(mCurrent)); 
		glDrawArrays(GL_LINES, 0, 2);

    glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE, glm::value_ptr(mCurrent)); 
		glDrawArrays(GL_LINES, 2, 2);

    glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE, glm::value_ptr(mCurrent)); 
		glDrawArrays(GL_LINES, 4, 2);
		
		SDL_GL_SwapBuffers();

}

void Control::setWidth(int w) {
	iWidth = w;
}

void Control::setHeight(int h) {
	iHeight = h;
}

// Private

void Control::handleEvents() {

}

int main(int argc, char** argv)
{
	Control c;
	c.setWidth(500);
	c.setHeight(500);
	c.initialize();

	c.prepare();
	c.run();

	c.deinitialize();

	return 0;
}



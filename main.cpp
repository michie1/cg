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

#include <time.h>

struct particle {
	glm::vec3 location;
	
};

class Firework {
	private:
		particle particles[1];

	public:
		Firework();
};


class Control {
	private:
		void handleEvents();
		int iWidth;
		int iHeight;
		bool running;
		GLuint iProjLoc, iViewModelLoc;
		glm::mat4 mProj, mViewModel;
		GLuint programID;
		GLuint uiVAO[2];
		GLuint uiVBO[4];
		GLuint dataBufferID;
		GLint vertexLoc;
		GLint colorLoc;

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
	SDL_EnableKeyRepeat(1, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 32);
	SDL_WM_SetCaption("Computer Graphics Final Project 2012", NULL);		

	// GLEW
	glewInit();

	// GL
	glViewport(0, 0, iWidth, iHeight);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); 
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
	running = true;

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

	float fTest[] = {
		0.0f, 0.0f, 0.0f,
		2.0f, 2.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		2.0f, 2.0f, 0.0f };

	glGenVertexArrays(3, uiVAO);
	glGenBuffers(7, uiVBO); 

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

	// Test
	
	//glBindBuffer(GL_ARRAY_BUFFER, uiVBO[4]);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(fTest), fTest, GL_STATIC_DRAW); 
//	glEnableVertexAttribArray(0);
// 	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
//


  glGenBuffers(1, &dataBufferID);
  glBindBuffer(GL_ARRAY_BUFFER, dataBufferID);
  glBufferData(GL_ARRAY_BUFFER, sizeof(fTest), fTest, GL_STATIC_DRAW);
	glFinish();

	programID = LoadShaders("shader.vert", "shader.frag");

	glUseProgram(programID);
	vertexLoc = glGetAttribLocation(programID, "inPosition");
	colorLoc = glGetAttribLocation(programID, "inColor");
	//glUseProgram(0);

	iProjLoc = glGetUniformLocation(programID, "projectionMatrix");
	iViewModelLoc = glGetUniformLocation(programID, "viewMatrix");

}

void Control::run() {
	render();
}

void Control::render() {

	float up = 0.0f;
	float rotation = 0.0f;
	float y[10];
	float x[10];

	float hi[8];

	int max = 5;

	float circlex[360], circley[360];
	for(int i = 0; i < 360; i++) {
		circlex[i] = 0; 
		circley[i] = 0;
	}
	
	int newX, newY;
	float speed = 1.0f;


	float fCamRoty = 0.0f;
	float fCamZ = 5.0f;


	bool go = false;

	for(int i = 0; i < 10; i++) {
		y[i] = 0.0f;
		x[i] = -(float)i / 1.5;
		if(i <= 8) {
			hi[i] = 0.0f;
		}
	}

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
						rotation -= 5.0f;
					} else if(event.key.keysym.sym == SDLK_RIGHT) {
						rotation += 5.0f;
					} else if(event.key.keysym.sym == SDLK_UP) {
						up += 0.15f;
					} else if(event.key.keysym.sym == SDLK_DOWN) {
						up -= 0.15f;
					}	else if(event.key.keysym.sym == 'a') {
						fCamRoty += 1.0f;
					} else if(event.key.keysym.sym == 'd') {
						fCamRoty -= 1.0f;
					} else if(event.key.keysym.sym == 'w') {
						fCamZ -= 0.1f;
					} else if(event.key.keysym.sym == 's') {
						fCamZ += 0.1f;
					} else if(event.key.keysym.sym == 'g') {
						go = true;
					}
					//std::cout << event.key.keysym.sym << std::endl;
					break;
			}
		}


		mViewModel = glm::lookAt(glm::vec3(-1.0f, 1.0f, fCamZ), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		mViewModel = glm::rotate(mViewModel, fCamRoty, glm::vec3(0.0f, 1.0f, 0.0f));
		mProj = glm::perspective(45.0f, (float) iWidth / (float) iHeight, 0.1f, 100.0f);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 


		glUniformMatrix4fv(iProjLoc, 1, GL_FALSE, glm::value_ptr(mProj));
		glUniformMatrix4fv(iViewModelLoc, 1, GL_FALSE, glm::value_ptr(mViewModel));


		glm::mat4 mCurrent, temp;


		// Triangles
		glBindVertexArray(uiVAO[0]);

		// beam

		mCurrent = glm::rotate(mViewModel, rotation, glm::vec3(0.0f, 1.0f, 0.0f));
		mCurrent = glm::scale(mCurrent, glm::vec3(0.2f, 0.2f, 0.2f));
		mCurrent = glm::translate(mCurrent, glm::vec3(0.0f, up, 0.0f));

		for(int i = 0; i < 500; i++) {
			mCurrent = glm::translate(mCurrent, glm::vec3(0.2f, 0.0f, 0.0f));
			if(i % 2 == 0) {
				//temp = glm::translate(mCurrent, glm::vec3(-1.0f, 0.0f, 0.0f));
				temp = mCurrent;
			} else {
				temp = mCurrent;
			}
			temp = glm::rotate(temp, rotation * i, glm::vec3(1.0f, 0.0f, 0.0f));
	    glUniformMatrix4fv(iViewModelLoc, 1, GL_FALSE, glm::value_ptr(temp)); 
			glDrawArrays(GL_TRIANGLES, 0, 3);
		}

		
		// big triangle

		mCurrent = glm::translate(mViewModel, glm::vec3(0.0f, -1.0f, 0.0f));
		mCurrent = glm::translate(mViewModel, glm::vec3( (float)(rand() % 5) / 100.0f, (float)(rand() % 5) / 200.0f -1.0f, 0.0f));
    glUniformMatrix4fv(iViewModelLoc, 1, GL_FALSE, glm::value_ptr(mCurrent)); 
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// upmoving triangles

		mCurrent = glm::scale(mViewModel, glm::vec3(0.2f, 0.2f, 0.2f));
		for(int i = 0; i < 10; i++) {
			temp = glm::translate(mCurrent, glm::vec3( x[i] + (float)(rand() %5) / 50.0f, y[i], 0.0f));
			if(y[i] > 5.0f) {
				if(y[i] > 10.0f) {
					y[i] = 0.0f;
				} else {
					if(i >= 5) {
						//x[i] -= 0.01f;
					} else {
						//x[i] += 0.01f;
					}
					temp = glm::rotate(temp, (float) i * y[i] * 100, glm::vec3(0.0f, 0.0f, 1.0f));
					y[i] += 0.01f;
				}
			} else {
				y[i] += 0.05f;
			}
    	glUniformMatrix4fv(iViewModelLoc, 1, GL_FALSE, glm::value_ptr(temp)); 
			glDrawArrays(GL_TRIANGLES, 0, 3);
		}


		//
		
		mCurrent = glm::scale(mViewModel, glm::vec3(0.1f, 0.1f, 0.1f));
		mCurrent = glm::translate(mCurrent, glm::vec3( -10.0f, -10.0f, 0.0f));
		max = 10;	
		
		if(go) {
			for(int i = 1; i <= max; i++ ) {
			
				for(int j = 0; j < max; j++) {
					temp = glm::translate(mCurrent, glm::vec3( (float)i , (float)j, 0.0f));
					//temp = mCurrent;
					if(i > max/2){
						if(j > max/2) {
							hi[0] += 0.001f;
							hi[1] += 0.001f;
							temp = glm::translate(temp, glm::vec3(hi[0], hi[1], 0.0f));
						} else {
							hi[2] += 0.001f;
							hi[3] -= 0.001f;
							temp = glm::translate(temp, glm::vec3(hi[2], hi[3], 0.0f));
						}
					} else {
						if(j > max/2) {
							hi[4] -= 0.001f;
							hi[5] += 0.001f;
							temp = glm::translate(temp, glm::vec3(hi[4], hi[5], 0.0f));
						} else {
							hi[6] -= 0.001f;
							hi[7] -= 0.001f;
							temp = glm::translate(temp, glm::vec3(hi[6], hi[7], 0.0f));
						}
					}

					glUniformMatrix4fv(iViewModelLoc, 1, GL_FALSE, glm::value_ptr(temp)); 
					glDrawArrays(GL_TRIANGLES, 0, 3);
				}
			}
		}

		//

		mCurrent = glm::scale(mViewModel, glm::vec3(0.1f, 0.1f, 0.1f));
		mCurrent = glm::translate(mCurrent, glm::vec3(5.0f, -5.0f, 0.0f));

		if(go) {	
			for(int i = 0; i < 360; i++){
				temp = mCurrent;
	
				circlex[i] += cos(i) * 0.02f * speed;
				circley[i] += sin(i) * 0.02f * speed;
				temp = glm::translate(temp, glm::vec3( circlex[i], circley[i], 0.0f));
	
		  	glUniformMatrix4fv(iViewModelLoc, 1, GL_FALSE, glm::value_ptr(temp)); 
				glDrawArrays(GL_TRIANGLES, 0, 3);
	
			}
		}

		

		// Lines (axis)
		
		glBindVertexArray(uiVAO[1]);
		
		mCurrent = mViewModel;

    glUniformMatrix4fv(iViewModelLoc, 1, GL_FALSE, glm::value_ptr(mCurrent)); 
		glDrawArrays(GL_LINES, 0, 2);

    glUniformMatrix4fv(iViewModelLoc, 1, GL_FALSE, glm::value_ptr(mCurrent)); 
		glDrawArrays(GL_LINES, 2, 2);

	  glUniformMatrix4fv(iViewModelLoc, 1, GL_FALSE, glm::value_ptr(mCurrent)); 
		glDrawArrays(GL_LINES, 4, 2);


		// Test

		glUseProgram(programID);
		glBindVertexArray(uiVAO[2]);

	  //glUniformMatrix4fv(iViewModelLoc, 1, GL_FALSE, glm::value_ptr(mCurrent)); 
		glBindBuffer(GL_ARRAY_BUFFER, dataBufferID);
		//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(GLfloat), 0);
		//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(GLfloat), 0);
    glEnableVertexAttribArray(0);
    //glEnableVertexAttribArray(1);
		//glDrawArrays(GL_LINES, 0, 2);
		
		SDL_GL_SwapBuffers();
	}
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
	srand(time(NULL));
	Control c;
	c.setWidth(500);
	c.setHeight(500);
	c.initialize();

	c.prepare();
	c.run();

	c.deinitialize();

	return 0;
}



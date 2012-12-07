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
#include <list>

struct particle {
	glm::vec3 location;
  glm::vec4 color;
  float alpha;
  float size; 
};

class Firework {
	private:
		std::list<particle> particles;
		GLuint iViewModelLoc, iColorLoc, iVertexLoc, iPointSize;

	public:
		Firework();
		void draw();
		void setLocations(GLuint vm, GLuint c, GLuint v, GLuint p);
};

Firework::Firework() {
	particle p;
	p.location = glm::vec3(1.0f, 1.0f, 0.0f);
	p.color = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
  particles.push_back(p);

	p.location.x += 0.1f;
  particles.push_back(p);

	p.location.x += 0.1f;
  particles.push_back(p);

}

void Firework::draw() {
	std::list<particle>::iterator i;
	for(i = particles.begin(); i != particles.end(); i++) {
			glUniform4f(iColorLoc, i->color.x, i->color.y, i->color.z, i->color.w);
			glUniform3f(iVertexLoc, i->location.x, i->location.y, i->location.z);
			//glUniformVec3f(iVertexLoc, i->location);
			glDrawArrays(GL_POINTS, 0, 2);
	}
}

void Firework::setLocations(GLuint vm, GLuint c, GLuint v, GLuint p) {
		iViewModelLoc = vm;
		iColorLoc = c;
	 	iVertexLoc = v;
	 	iPointSize = p;
}



class Control {
	private:
		void handleEvents();
		int iWidth;
		int iHeight;
		bool running;
		GLuint iProjLoc, iViewModelLoc, iColorLoc, iVertexLoc, iPointSize;
		glm::mat4 mProj, mViewModel;
		GLuint programID;
		GLuint uiVAO[3];
		GLuint uiVBO[7];
		GLuint dataBufferID;
		GLint vertexLoc;
		GLint colorLoc;
		float color[3];
		glm::vec3 eye, center;
    Firework fw;


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

float fRand() {
	return (float) rand() / RAND_MAX;
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
  glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
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
	color[0] = fRand();
	color[1] = fRand();
	color[2] = fRand();

	float fTriangle[] = {
		-0.4f, 0.1f, -1.0f,
		0.4f, 0.1f, -1.0f,
		0.0f, 0.7f, -1.0f };
	
	float fLine[] = {
		-10.0f, 0.0f, 0.0f,
		10.0f, 0.0f, 0.0f,
		0.0f, -10.0f, 0.0f,
 		0.0f, 10.0f, 0.0f,
		0.0f, 0.0f, -10.0f,
		0.0f, 0.0f, 10.0f };


	glGenVertexArrays(4, uiVAO);
	glGenBuffers(7, uiVBO); 

	// Triangle
	glBindVertexArray(uiVAO[0]);

	glBindBuffer(GL_ARRAY_BUFFER, uiVBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(fTriangle), fTriangle, GL_STATIC_DRAW); 
	glEnableVertexAttribArray(0);
 	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	

	// Line
	
	glBindVertexArray(uiVAO[1]);
	
	glBindBuffer(GL_ARRAY_BUFFER, uiVBO[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(fLine), fLine, GL_STATIC_DRAW); 
	glEnableVertexAttribArray(0);
 	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);


  // Particle

  float fParticle[] = { 1.0f, 1.0f, 1.0f, 2.0f, 2.0f, 2.0f };

	glBindVertexArray(uiVAO[2]);
	
	//glBindBuffer(GL_ARRAY_BUFFER, uiVBO[5]);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(fParticle), fParticle, GL_STATIC_DRAW); 
	glEnableVertexAttribArray(0);
 	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
  


	programID = LoadShaders("shader.vert", "shader.frag");

	glUseProgram(programID);
	//vertexLoc = glGetAttribLocation(programID, "inPosition");
	//colorLoc = glGetAttribLocation(programID, "inColor");
	//glUseProgram(0);

	iProjLoc = glGetUniformLocation(programID, "projectionMatrix");
	iViewModelLoc = glGetUniformLocation(programID, "viewMatrix");
	iColorLoc = glGetUniformLocation(programID, "color");
	iVertexLoc = glGetUniformLocation(programID, "inPosition");
	iPointSize = glGetUniformLocation(programID, "pointSize");
	fw.setLocations(iViewModelLoc, iColorLoc, iVertexLoc, iPointSize);

	eye = glm::vec3(-1.0f, 1.0f, 5.0f);
	center = glm::vec3(0.0f, 0.0f, 0.0f);
}

void Control::run() {
	render();
}

void Control::render() {

	float up = 0.0f;
	float rotation = 0.0f;
	float y[10];
	float x[10];
	
	int newX, newY;
	float speed = 1.0f;


	float fCamRoty = 0.0f;
	float fCamZ = 5.0f;

		

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
						//rotation -= 5.0f;
						center.x -= 0.1f;
					} else if(event.key.keysym.sym == SDLK_RIGHT) {
						//rotation += 5.0f;
						center.x += 0.1f;
					} else if(event.key.keysym.sym == SDLK_UP) {
						//up += 0.15f;
						center.y += 0.1f;
					} else if(event.key.keysym.sym == SDLK_DOWN) {
						//up -= 0.15f;
						center.y -= 0.1f;
					}	else if(event.key.keysym.sym == 'a') {
						fCamRoty += 1.0f;
						//eye.x -= 0.1f;
					} else if(event.key.keysym.sym == 'd') {
						fCamRoty -= 1.0f;
						//eye.x += 0.1f;
					} else if(event.key.keysym.sym == 'w') {
						eye.z -= .1;
					} else if(event.key.keysym.sym == 's') {
						eye.z += .1f;
					} else if(event.key.keysym.sym == 'g') {
					} else if(event.key.keysym.sym == 'r') {
						eye = glm::vec3(-1.0f, 1.0f, 5.0f);
						center = glm::vec3(0.0f, 0.0f, 0.0f);
					}
					//std::cout << event.key.keysym.sym << std::endl;
					break;
			}
		}


		mViewModel = glm::lookAt(eye, center, glm::vec3(0.0f, 1.0f, 0.0f));
		mViewModel = glm::rotate(mViewModel, fCamRoty, glm::vec3(0.0f, 1.0f, 0.0f));
		mProj = glm::perspective(45.0f, (float) iWidth / (float) iHeight, 0.1f, 100.0f);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

		glUniformMatrix4fv(iProjLoc, 1, GL_FALSE, glm::value_ptr(mProj));
		glUniformMatrix4fv(iViewModelLoc, 1, GL_FALSE, glm::value_ptr(mViewModel));

		glm::mat4 mCurrent, temp;


		// Lines (axis)
   

		glUniform1f(iPointSize, 1.0f);

		glUniform4f(iColorLoc, 1.0f, 0.0f, 0.0f, 1.0f);
		for(float x = -10.0f; x < 10.0f; x++) {
			glUniform3f(iVertexLoc, x / 10.0f, 0.0f, 0.0f);
			glDrawArrays(GL_POINTS, 0, 2);
		}
		glUniform4f(iColorLoc, 0.0f, 1.0f, 0.0f, 1.0f);
		for(float y = -10.0f; y < 10.0f; y++) {
			glUniform3f(iVertexLoc, 0.0f, y / 10.0f, 0.0f);
			glDrawArrays(GL_POINTS, 0, 2);
		}
		glUniform4f(iColorLoc, 0.0f, 0.0f, 1.0f, 1.0f);
		for(float z = -10.f; z < 10.0f; z++) {
			glUniform3f(iVertexLoc, 0.0f, 0.0f, z / 10.0f);
			glDrawArrays(GL_POINTS, 0, 2);
		}


	/*

	glUniform3f(iColorLoc, 1.0f, 0.0f, 0.0f);
	for(float i = -10.0f; i < 10.0f; i++) {	
		mCurrent = glm::translate(mViewModel, glm::vec3(i / 10.0f, .0f, 0.0f));
		glUniformMatrix4fv(iViewModelLoc, 1, GL_FALSE, glm::value_ptr(mCurrent)); 
		glDrawArrays(GL_POINTS, 0, 2);
	} 

	*/




    // Firework


		fw.draw();

		//glUniform3f(iVertexLoc, 1.0f, 1.0f, 0.0f);
		//glDrawArrays(GL_POINTS, 0, 2);

		// Test



		//glUseProgram(programID);
		//glBindVertexArray(uiVAO[2]);

	  //glUniformMatrix4fv(iViewModelLoc, 1, GL_FALSE, glm::value_ptr(mCurrent)); 
		//glBindBuffer(GL_ARRAY_BUFFER, dataBufferID);
		//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(GLfloat), 0);
		//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(GLfloat), 0);
    //glEnableVertexAttribArray(0);
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



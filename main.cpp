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

float fRand() {
	return (float) rand() / RAND_MAX;
}

glm::vec3 randVelocity(glm::vec3 &from) {
	glm::vec3 v = glm::vec3((float)(rand() % 400 ) / 100.0f - 2.0f, (float)(rand() % 400 ) / 100.0f - 2.0f, (float)(rand() % 400 ) / 100.0f - 2.0f );

	//std::cout << glm::distance(v, from) << std::endl;
	if(glm::distance(v, from) <= 1.0f) {
		return v;
	} else {
		return randVelocity(from);
	}
}

struct particle {
	glm::vec3 location, velocity, gravity;
  glm::vec4 color;
  float alpha;
  float size; 
	float age;
	bool isNewest;
	bool exploded;
	float explodetime;
	float lifetime;
	particle() {
		exploded = false;
		explodetime = 1.0f;
		age = 0.0f;
		lifetime = 5.0f;
		isNewest = true;
		velocity = glm::vec3(0.0f, 4.0f, 0.0f);
		gravity = glm::vec3(0.0f, -1.0f, 0.0f);
	}
	particle copy() {
		particle n;
		n.location = location;
		n.color = color;
		n.size = size;
		return n;
	}
};

class Firework {
	private:
		std::list<particle> particles;
		GLuint iViewModelLoc, iColorLoc, iVertexLoc, iPointSize;
		int age;
		glm::vec3 gravity;


	public:
		Firework();
		void insert(std::list<particle>::iterator i, particle p);
		void calc(float dt);
		void draw();
		void setLocations(GLuint vm, GLuint c, GLuint v, GLuint p);
		glm::vec4 randColor();
};

Firework::Firework() {
	float age = 0.0f;
	glm::vec3 origin = glm::vec3(0.0f, 0.0f, 0.0f);

	particle p;
	
	p.location = glm::vec3(0.0f, 0.0f, 0.0f);
	//p.velocity = randVelocity(origin);
	p.color = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
	particles.push_back(p);


	//p = p.copy();
	//p.location.y += 0.01f;
	//pushP(p);
}

void Firework::insert(std::list<particle>::iterator i, particle p) {
	particles.back().isNewest = false;
//	particles.push_back(p);
	particles.insert(i, p);
	//std::cout << p.location.y << std::endl;
	//std::cout << particles.size() << std::endl;
}

void Firework::calc(float dt) {
	age += dt;
	std::list<particle>::iterator i;
	particle p;
	for(i = particles.begin(); i != particles.end(); i++) {
		i->age += dt;
		i->velocity *= 0.99f;
		i->gravity *= 1.01f;
		
		// Fading away
		if(i->age > i->lifetime ) {
			i = particles.erase(i);	
		} else if(i->exploded == false && i->age > i->explodetime) {
			i->exploded = true;
			for(int a = 0; a < 500; a++ ) {
		 		particle n;
				n.location = i->location;
				/*
				n.velocity.x = (float)(rand() % 400 ) / 100.0f - 2.0f;
				n.velocity.y = (float)(rand() % 400 ) / 100.0f - 2.0f;
				n.velocity.z = (float)(rand() % 400 ) / 100.0f - 2.0f;
				*/
				n.velocity = 0.2f * i->velocity + randVelocity(i->location);
				n.lifetime = (float)(rand() % 4);
				//std::cout << n.velocity.x << std::endl;
				//std::cout << n.lifetime << std::endl;
				n.exploded = true;
				n.color = randColor();
				insert(i, n);
			}
			i = particles.erase(i);	

		}	else {
			i->location += (i->velocity + i->gravity) * dt;
			i->color.w = 1 - i->age / i->lifetime;
		}

		


		//std::cout << i->location.y << std::endl;
		/*
		if(i->isNewest && i->age > 100 && age < 1000 ) {
			p = i->copy();	
			p.location.y += 0.01f;
			insert(i, p);
		}
		*/
		//std::cout << i->color.w << std::endl;
	}
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
 
glm::vec4 Firework::randColor() {
	return glm::vec4( fRand(), fRand(), fRand(), 1.0f);
}


class Control {
	private:
		void handleEvents();
		int iWidth;
		int iHeight;
		bool running;
		GLuint iProjLoc, iViewModelLoc, iColorLoc, iVertexLoc, iPointSizeLoc;
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
		//Uint32 lastTime, newTime;


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
  glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
	glEnable( GL_BLEND );
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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

	/*
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
	*/

	glGenVertexArrays(4, uiVAO);
	glGenBuffers(7, uiVBO); 

	// Triangle
	
	/*
	glBindVertexArray(uiVAO[0]);

	glBindBuffer(GL_ARRAY_BUFFER, uiVBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(fTriangle), fTriangle, GL_STATIC_DRAW); 
	glEnableVertexAttribArray(0);
 	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	*/	

	// Line
	
	glBindVertexArray(uiVAO[1]);
	
	glBindBuffer(GL_ARRAY_BUFFER, uiVBO[2]);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(fLine), fLine, GL_STATIC_DRAW); 
	//glEnableVertexAttribArray(0);
 	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	

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
	iPointSizeLoc = glGetUniformLocation(programID, "pointSize");
	fw.setLocations(iViewModelLoc, iColorLoc, iVertexLoc, iPointSizeLoc);

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

		
	int lasttime = SDL_GetTicks();
	int newtime;

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


		glClearAccum(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

		glUniformMatrix4fv(iProjLoc, 1, GL_FALSE, glm::value_ptr(mProj));
		glUniformMatrix4fv(iViewModelLoc, 1, GL_FALSE, glm::value_ptr(mViewModel));

		glm::mat4 mCurrent, temp;


		// Trials
		glAccum(GL_RETURN, 0.95f);
		glClear(GL_ACCUM_BUFFER_BIT);

		// Lines (axis)
   

		glUniform1f(iPointSizeLoc, 1.0f);

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


		glUniform1f(iPointSizeLoc, 2.0f);


		newtime = SDL_GetTicks();
		fw.calc((newtime - lasttime)/1000.0f);
		lasttime = newtime;

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
		 glAccum(GL_ACCUM, 0.9f);
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



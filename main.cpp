#include <cstdlib>
#include <iostream>

#include "GL/glew.h"
#include "GL/gl.h"
#include "GL/glu.h"
#include "SDL/SDL.h"
#include "SDL/SDL_mixer.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "shader.h"

#include <time.h>
#include <list>

float fRand() {
	return (float) rand() / RAND_MAX;
}

// A particle with all their own properties.
struct particle {
	glm::vec3 location, velocity, gravity;
  glm::vec4 color;
  float alpha;
  float size; 
	float age;
	bool exploded;
	float explodetime;
	float lifetime;
	float timebeforelaunch;
	float pointSize;
	bool follow; // follow location as eye position
	particle() {
		exploded = false;
		follow = false;
		explodetime = (float)((rand() % 10)) / 10.f + 1.0f; // Explode between 1 and 2 seconds.
		age = 0.0f;
		lifetime = 4.0f;
		timebeforelaunch = 1.0f;
		gravity = glm::vec3(0.0f, -1.0f, 0.0f);
		pointSize = 6.0f;
	}
};

// Firework class for controling the particles
class Firework {
	private:
		std::list<particle> particles;
		GLuint iViewModelLoc, iColorLoc, iVertexLoc, iPointSizeLoc;
		int age;
		glm::vec4 randColor(float alpha);
		glm::vec3 randomVelocity(float min, float max);
	public:
		Firework();
		void add();
		void playExplode(int salvos);
		void calc(float dt);
		void draw();
		void setLocations(GLuint vm, GLuint c, GLuint v, GLuint p);
		bool z; // Z enabled?
		glm::vec3 rocket;
};

Firework::Firework() {
	z = true;
	add();
	particles.front().follow = true; // Follow the first particle, when follow is enabled.
}

// Add new rocket
void Firework::add() {
	particle p;
	p.location = glm::vec3( (float)(rand() % 4) - 2.0f, 0.0f, 0.0f);
	p.velocity = randomVelocity(-2.0f, 2.0f);
	p.velocity.y += 6.0f; // We want the particle to go high	
	p.color = randColor(0.0f);
	particles.push_back(p);
}

void Firework::playExplode(int salvos) {
	Mix_Chunk *music;
	Mix_OpenAudio(44100, AUDIO_S16SYS, 2, 2048);
	switch(salvos) {
		case 1:
			music = Mix_LoadWAV("explode.wav");
			break;
		case 2:
			music = Mix_LoadWAV("explode2.wav");
			break;
		case 3:
			music = Mix_LoadWAV("explode3.wav");
			break;
		case 4:
			music = Mix_LoadWAV("explode4.wav");
			break;
	}	
	Mix_PlayChannel(-1,music,0);
}

// Calculate the new positions of the particles
void Firework::calc(float dt) {
	age += dt;
	std::list<particle>::iterator i;
	particle p;
	for(i = particles.begin(); i != particles.end(); i++) {
		i->timebeforelaunch -= dt;
		if(i->timebeforelaunch < 0.0f) {
				i->age += dt;
				i->velocity *= 0.99f;
				i->gravity *= 1.01f;

				if(i->age > i->lifetime ) { // Remove old particles
					i = particles.erase(i);	
				} else if(i->exploded == false && i->age > i->explodetime) { // Time for explosion when not yet exploded.
					i->exploded = true;
					
					int salvos = rand() % 3 + 1; // Amount of shots at explosion, with delay between shots, between 1 and 4.
					playExplode(salvos);


					int temp;
					glm::vec4 c[2]; // Two colors
				 	c[0] = i->color;	
				 	c[1] = randColor(0.0f);	
					int amount;
					if(rand() % 50 == 0) { // Once in a while a big explosion 
						amount = 10000;	
					} else {
						amount = rand() % 100 + 500; // Between 500 and 600 particles per rocket.
					}
 					

					for(int a = 0; a < amount; a++ ) {
						particle n;
						n.location = i->location;

						n.velocity = randomVelocity(-2.0f, 2.0f);
						if(z == false) { // If z is disabled, the particles will not explode only in the x and y direction.
							n.velocity.z = 0.0f;
						}

						// Normalize the vector, so every distance from the particle from the rocket are the same.
						// Gives a sphere shape instead of a cube.
						n.velocity = glm::normalize(n.velocity) * 3.0f; 		
						n.velocity += 0.5f * i->velocity; // Take the velocity of the rocket into account.
						
						if(salvos == 1) {
							n.timebeforelaunch = 0.0f;
							n.color = c[0];
						} else {
							temp = rand() % salvos;
							n.timebeforelaunch = temp * 0.35f;
							n.color = c[temp % 2]; // Alternating colors
						}

						n.lifetime = (float)(rand() % 4);
						n.exploded = true;

						n.color.w = 0.0f; // Make invisible till launch
						n.pointSize = (float)(rand() % 30) / 10.0f + 1.0f; // Point size between 1 and 4
						particles.insert(i, n);
					}

					bool f = i->follow;
					i = particles.erase(i);	// Remove exploded rocket.
					add();
					if(f) {
						particles.back().follow = true;
					}

				}	else {
					i->location += (i->velocity + i->gravity) * dt;
					i->color.w = 1 - i->age / i->lifetime; // Fade alpha
					if(i->follow) {
						rocket = i->location; // Set the eye to the rocket location.
					}
				}
		}
	}
}

// Draw all the particles
void Firework::draw() {
	std::list<particle>::iterator i;
	for(i = particles.begin(); i != particles.end(); i++) {
			glUniform4f(iColorLoc, i->color.x, i->color.y, i->color.z, i->color.w);
			glUniform3f(iVertexLoc, i->location.x, i->location.y, i->location.z);
			glUniform1f(iPointSizeLoc, i->pointSize);
			glDrawArrays(GL_POINTS, 0, 2);
	}
}

void Firework::setLocations(GLuint vm, GLuint c, GLuint v, GLuint p) {
		iViewModelLoc = vm;
		iColorLoc = c;
	 	iVertexLoc = v;
	 	iPointSizeLoc = p;
}
 
glm::vec4 Firework::randColor(float alpha) {
	return glm::vec4( fRand(), fRand(), fRand(), alpha);
}

glm::vec3 Firework::randomVelocity(float min, float max) {
	return glm::vec3(
		(float)( rand() % ( (int)(max - min) * 100 ) ) / 100.f + min,
		(float)( rand() % ( (int)(max - min) * 100 ) ) / 100.f + min,
		(float)( rand() % ( (int)(max - min) * 100 ) ) / 100.f + min );
}

// A class for setting up SDL (and the event handler), glew and the firework class.
class Control {
	private:
		int iWidth;
		int iHeight;
		bool running;
		GLuint iProjLoc, iViewModelLoc, iColorLoc, iVertexLoc, iPointSizeLoc;
		glm::mat4 mProj, mViewModel;
		GLuint programID;
		glm::vec3 eye, center;
    Firework fw;

	public:
		void initialize();
		void deinitialize();
		void prepare();
		void run();
		void setWidth(int w);
		void setHeight(int h);
};

void Control::initialize() {
	// SDL
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
	SDL_SetVideoMode(iWidth, iHeight, 32, SDL_OPENGL | SDL_GL_DOUBLEBUFFER);
	SDL_EnableKeyRepeat(1, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 32);
	SDL_WM_SetCaption("Computer Graphics Final Project 2012", NULL);		

	// SDL sound
	int audio_rate = 22050;
	Uint16 audio_format = AUDIO_S16SYS;
	int audio_channels = 2;
	int audio_buffers = 4096;

	if(Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers) != 0) {
		fprintf(stderr, "Unable to initialize audio: %s\n", Mix_GetError());
		exit(1);
	}
	
	// GLEW
	glewInit();

	// GL
	glViewport(0, 0, iWidth, iHeight);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); 
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glClearDepth(1.0f);
  glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
	glEnable( GL_BLEND );
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClear(GL_ACCUM_BUFFER_BIT);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_POINT_SMOOTH);
}

void Control::deinitialize() {
	SDL_Quit();
}

void Control::prepare() {
	running = true;

	// Shaders
	programID = LoadShaders("shader.vert", "shader.frag");
	glUseProgram(programID);

	iProjLoc = glGetUniformLocation(programID, "projectionMatrix");
	iViewModelLoc = glGetUniformLocation(programID, "viewMatrix");
	iColorLoc = glGetUniformLocation(programID, "color");
	iVertexLoc = glGetUniformLocation(programID, "inPosition");
	iPointSizeLoc = glGetUniformLocation(programID, "pointSize");

	fw.setLocations(iViewModelLoc, iColorLoc, iVertexLoc, iPointSizeLoc);

	eye = glm::vec3(-1.0f, 1.0f, 12.0f);
	center = glm::vec3(0.0f, 2.0f, 0.0f);
}

void Control::run() {
	float camRoty = 0.0f;
	int lasttime = SDL_GetTicks();
	int newtime;
	bool follow = false;

	while(running) {

		// Event handler
		SDL_Event event;
		while(SDL_PollEvent(&event)) {
			switch(event.type) {
				case SDL_QUIT: 
					running = false;  
					break;
				case SDL_KEYDOWN: 
					switch(event.key.keysym.sym) {
						case SDLK_ESCAPE:
							running = false;
							break;
						case SDLK_LEFT:
							center.x -= 0.1f;
							break;
						case SDLK_RIGHT:
							center.x += 0.1f;
							break;
						case SDLK_UP:
							center.y += 0.1f;
							break;
						case SDLK_DOWN:
							center.y -= 0.1f;
							break;
						case 'a':
							camRoty += 1.0f;
							break;
						case 'd':
							camRoty -= 1.0f;
							break;
						case 'w':
							eye.z -= 0.1f;
							break;
						case 's':
							eye.z += 0.1f;
							break;
					}
					break;
				case SDL_KEYUP:
					switch(event.key.keysym.sym) {
						case 'z':
							fw.z = !fw.z;
							break;
						case 'm':
							fw.add();
							break;
						case 'r':
							eye = glm::vec3(-1.0f, 1.0f, 12.0f);
							center = glm::vec3(0.0f, 2.0f, 0.0f);
							camRoty = 0.0f;
							break;
						case 'f':
							if(follow) {
								eye = glm::vec3(-1.0f, 1.0f, 12.0f);
								center = glm::vec3(0.0f, 2.0f, 0.0f);
								follow = false;
							} else {
								follow = true;	
							}
					}	
					break;
				}
			}

		//  Setup the camera
		mViewModel = glm::lookAt(eye, center, glm::vec3(0.0f, 1.0f, 0.0f));
		if(follow == false) {
			mViewModel = glm::rotate(mViewModel, camRoty, glm::vec3(0.0f, 1.0f, 0.0f));
		}
		mProj = glm::perspective(45.0f, (float) iWidth / (float) iHeight, 0.1f, 100.0f);

		glClearAccum(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

		glUniformMatrix4fv(iProjLoc, 1, GL_FALSE, glm::value_ptr(mProj));
		glUniformMatrix4fv(iViewModelLoc, 1, GL_FALSE, glm::value_ptr(mViewModel));

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

    // Firework
		newtime = SDL_GetTicks();
		fw.calc((newtime - lasttime) / 1000.0f);
		if(follow) {
			eye = fw.rocket * 2.5f;
			center = fw.rocket;
		}
		lasttime = newtime;
		fw.draw();

		SDL_GL_SwapBuffers();
		glAccum(GL_ACCUM, 0.90f);
	}
}

void Control::setWidth(int w) {
	iWidth = w;
}

void Control::setHeight(int h) {
	iHeight = h;
}

int main(int argc, char** argv)
{
	srand(time(NULL));
	Control c;
	c.setWidth(1280);
	c.setHeight(720);
	c.initialize();

	c.prepare();
	c.run();

	c.deinitialize();

	return 0;
}

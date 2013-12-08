#include "camera2.h"
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/GLU.h>
#include <GL/GL.h>
#include <mmsystem.h>
#include <math.h>
#include "SDL.h"
#include <iostream>
#include "enemy.h"
/*
Player Attribs
*/
bool isground, iscollision, issprint;
float sprintspeed, normalspeed;
float energy; // if sprinting, energy goes down .. vice versa
int points; // score
int player_health;
vec3 gravity; // gravity
void player_init() {
	player_health = 100;
	gravity = vec3(0,-.3,0);
	//	setPosition(getPosition());
	points = 0;
	normalspeed = 0.5;
	isground = iscollision = issprint = false;
}



int mX, mY;
mat4 ViewMatrix;
mat4 ProjectionMatrix;
int key_pressed;
// Initial position : on +Z
vec3 position = vec3 (0,-1.0f,3.0f);
vec3 direction(0.0f,0.0f,-1.0f),right(1,0,0),up(0,1,0);
// Initial horizontal angle : toward -Z
float horizontalAngle = -3.14f;
// Initial vertical angle : none
float verticalAngle = 0.0f;
// Initial Field of View
float initialFoV = 45.0f;
float deltaTime;
float speed = 0.3; // 3 units / second
float mouseSpeed = 0.005f;
bool mouse_mov = false;
bool cam_moved;
vec3 trial;
mat4 getViewMatrix(){
	return ViewMatrix;
}
mat4 getProjectionMatrix(){
	return ProjectionMatrix;
}

vec3 getPosition() {
	return position;
}
vec3 getP_Direction() {
	return direction;
}
void updateDelta(float d) {
	deltaTime = d;
}
float getDelta() {
	return deltaTime;
}
void mouse_position(int x, int y) {
	if(mX != x || mY !=  y) {
		mX = x; mY = y;
		horizontalAngle +=  -(mouseSpeed * deltaTime * float(1024/2 - x ));
		verticalAngle   += -(mouseSpeed * deltaTime * float( 768/2 - y ));
		glutWarpPointer(1024/2, 768/2);
		cam_moved = true;
	} else {
		cam_moved = false;
	}
}
void shoot(int b, int state, int x, int y) {
	std::cout << "player ";
	print(getPosition());
	std::cout << "enemy ";
	print(getE_Position());
	std::cout << "enemy dir ";
	print(getDirection());
	switch(b) {
	case GLUT_LEFT_BUTTON:
		std::cout << "Direction = ";
		print(direction);
		break;
	}
}

void blah(int b, int x, int y) {
	switch(b) {

	case GLUT_KEY_END:
		exit(99);
		break;
	}
}
void keyboard_action(unsigned char b, int x, int y) {
	print(position);
	std::cout << "\n\n deltaTime:" << deltaTime << std::endl;

	switch(b) {
	case 'w':
		//position += direction * deltaTime * speed;
		position += vec3(direction.v[0],0,direction.v[2]) * deltaTime * speed;
		//		position.v[2] += direction.v[0] * deltaTime * speed;
		break;
	case 's':
		position -= vec3(direction.v[0],0,direction.v[2]) * deltaTime * speed;
		break;
	case 'a':
		position.v[0] += right.v[0] * deltaTime * speed;
		break;
	case 'd':
		position.v[0] -= right.v[0] * deltaTime * speed;
		break;
	case 'p':
		system("PAUSED");
	}
	cam_moved = true;
}

void computeMatricesFromInputs(){
	if(cam_moved) {

	}
	// glfwGetTime is called only once, the first time this function is called
	//	static double lastTime = timeGetTime();

	// Compute time difference between current and last frame
	//	double currentTime = timeGetTime();
	//	delta_time = float(currentTime - lastTime);

	// clamp rotation angle between 0 - 2*PI
	//		if (horizontalAngle > 3.14f*2) horizontalAngle = 0;
	//		if (horizontalAngle < 0) horizontalAngle = 3.14f*2;

	// clamp camera up/down values so we can't go upside down
	//		if (verticalAngle >= 3.14f/2.0f) verticalAngle = 3.14f/2.0f;
	//		if (verticalAngle <= -3.14f/2.0f) verticalAngle = -3.14f/2.0f;
	// Direction : Spherical coordinates to Cartesian coordinates conversion

	float FoV = initialFoV - 5 * SDL_MOUSEWHEEL;

	direction = vec3(
		cos(verticalAngle) * sin(horizontalAngle), 
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle)
		);

	// Right vector
	right = vec3(
		sin(horizontalAngle - 3.14f/2.0f), 
		0,
		cos(horizontalAngle - 3.14f/2.0f)
		);
	// Up vector
	up = cross( right, direction );



	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	ProjectionMatrix = perspective(FoV, 4.0f / 3.0f, 0.1f, 100.0f);
	// Camera matrix

	ViewMatrix= look_at(position,           // Camera is here
		position+direction, // and looks here : at the same position, plus "direction"
		up                  // Head is up (set to 0,-1,0 to look upside-down)
		);
	cam_moved = false;
	// For the next frame, the "last time" will be "now"
	//	lastTime = currentTime;
}

int getPoints() {
	return points;
}
int getHealth() {
	return player_health;
}
void setHealth(bool h, int amount) {
	player_health = h ? player_health - amount :  player_health + amount;
}

void addPoints(int score) {
	points += score;
}
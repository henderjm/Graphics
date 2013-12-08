#include <SDL.h>
#include "maths_funcs.h"
#include <math.h>
#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/GLU.h>
#include <GL/GL.h>

class camera
{
	vec3 loc;
	vec3 cam_up;
	vec3 cam_right;
	float camYaw;
	float camPitch;
	float speed;
	float mousespeed;
	bool mi;
	void lockCamera();
	void moveCamera(const float& dir);
	void moveCameraUp(const float& dir);
	public:
	camera();
	camera(const vec3& loc);
	camera(const vec3& loc,const float& yaw,const float& pitch);
	camera(const vec3& loc,const float& yaw,const float& pitch,const float& sp,const float& ms);
	void control(int b, int x, int y);
	void update();
	vec3 getVector();
	vec3 getLocation();
	float getYaw();
	float getPitch();
	void setLocation(const vec3& newcoord);
	void lookAt(const float& y, const float& p);
	void mouseIn();
	void mouseOut();
	bool IsMouseIn();
	void setSpeed(float sp,float ms);
};


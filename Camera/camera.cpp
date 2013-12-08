#include "camera.h"

camera::camera()
{
	loc = vec3(0.0,0.0,0.0);
	camYaw=0.0;
	camPitch=0.0;
	speed=0.2;
	mousespeed=0.2;
	mi=false;
	int * arr;
}
camera::camera(const vec3& l)
{ 
	loc=l;
	camYaw=0.0;
	camPitch=0.0;
	speed=0.3;
	mousespeed=0.3;
	mi=false;
	cam_right = vec3(1,0,0);
	cam_up = vec3(0,1,0);
}
camera::camera(const vec3& l,const float& yaw,const float& pitch)
{
	loc=l;
	 camYaw=yaw;
	 camPitch=pitch;
	 speed=0.3;
	 mousespeed=0.3;
	 mi=false;
}
camera::camera(const vec3& l,const float& yaw,const float& pitch,const float& sp,const float& ms)
{
	loc=l;
	 camYaw=yaw;
	 camPitch=pitch;
	 speed=sp;
	 mousespeed=ms;
	 mi=false;
}


void camera::lockCamera()
{
	if(camPitch>90)
		camPitch=90;
	if(camPitch<-90)
		camPitch=-90;
	if(camYaw<0.0)
		camYaw+=360.0;
	if(camYaw>360.0)
		camYaw-=360;
}

void camera::moveCamera(const float &dir)
{
	std::cout << "move cam2\n";
	float rad=(camYaw+dir)*M_PI/180.0;
	loc.v[0] -=sin(rad)*speed;
	loc.v[2] -=cos(rad)*speed;
	print(loc);
}

void camera::moveCameraUp(const float& dir)
{
	float rad=(camPitch+dir)*M_PI/180.0;
	loc.v[1]+=sin(rad)*speed;	
}

void camera::control(int b, int x, int y)
{
	std::cout << "move cam\n";
	if(mi)
	{
		int MidX=320;
		int MidY=240;
		glutSetCursor(GLUT_CURSOR_NONE);
//		SDL_ShowCursor(SDL_DISABLE);
		int tmpx,tmpy;
		
//		SDL_GetMouseState(&tmpx,&tmpy);
		
//		camYaw+=mousespeed*(MidX-tmpx);
//		camPitch+=mousespeed*(MidY-tmpy);
		lockCamera();
		glutWarpPointer(MidX,MidY);
		const Uint8* state=SDL_GetKeyboardState(NULL);
		if(b == GLUT_KEY_LEFT) {
			std::cout << "keyboard\n";
		}
		if(state[SDLK_w])
		{
				moveCamera(0.0);
		}else if(state[SDLK_s])
		{
				moveCamera(180.0);
		}		
		if(state[SDLK_a])
			moveCamera(90.0);
		else if(state[SDLK_d])
			moveCamera(270);	
	}
	
//	glRotatef(-camPitch,1.0,0.0,0.0);
//	glRotatef(-camYaw,0.0,1.0,0.0);
}

void camera::update()
{
//	glTranslatef(-loc.v[0],-loc.v[1],-loc.v[2]);
}

vec3 camera::getLocation()
{
	return loc;
}

float camera::getYaw()
{
	return camYaw;
}
float camera::getPitch()
{
	return camPitch;
}

void camera::setLocation(const vec3& newcoord)
{
	loc=newcoord;
}

void camera::lookAt(const float& y, const float& p)
{
	camYaw=y;
	camPitch=p;
}

void camera::mouseIn()
{
	mi=true;
}

void camera::mouseOut()
{
	mi=false;
}

vec3 camera::getVector()
{
//	std::cout << camYaw << " " << camPitch << std::endl;
	return (vec3(-cos(camPitch*3.14/180.0)*sin(camYaw*3.14/180.0),sin(camPitch*3.14/180.0),-cos(camPitch*3.14/180.0)*cos(camYaw*3.14/180.0)));
}

bool camera::IsMouseIn()
{
	return mi;
}


void camera::setSpeed(float sp,float ms)
{
	speed=sp;
	mousespeed=ms;
}

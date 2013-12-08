#define _USE_MATH_DEFINES
#include "camera2.h"
#include "collisionsphere.h"
#include <cmath>
/*#include <string>

int health;
vec3 force; // gravity
vec3 direction; // pos + direction
bool isground, iscollision, issprint;
float sprintspeed, normalspeed;
float energy; // if sprinting, energy goes down .. vice versa
int points; // score
*/
int getE_Points();
int getE_Health();
int valuePoints(int value);
GLuint getE_id();
bool e_dead();
void setE_id(GLuint id);
void setE_Health(int health);
void e_init(mat4 *, vec3 s, collisionsphere c, vec3 rotation, vec3 & player_loc);
void move(mat4 &, vec3 &p);

vec3 getE_Position();
vec3 getDirection();
void setE_Position(vec3 &);
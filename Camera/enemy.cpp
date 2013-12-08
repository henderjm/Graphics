#include "enemy.h"

#include <iostream>
int e_health;
int e_points;
GLuint e_id;
float e_speed;
bool e_isAlive;
vec3 e_position;
vec3 e_direction;
float e_horizontal, e_vertical;
mat4 b;
vec3  temp;
vec3 e_rotation;
collisionsphere e_cs;

void e_init(mat4 * baddy, vec3 spawn, collisionsphere cs, vec3 rot, vec3 & p_loc) {
	e_health = 100;
	e_points = 0;
	e_speed = 0.0005;
	e_isAlive = true;
	e_cs = cs;
	e_position = spawn;
	e_direction = p_loc - e_cs.center;
	e_horizontal = 3.14f;
	e_vertical = 0.0f;
	b = *baddy;
	temp = e_position;
	e_rotation = vec3(0,0,0);
}
void setE_id(GLuint id) {
	e_id = id;
}

int getE_Points() {
	return e_points;
}

int getE_Health() {
	return e_health;
}

vec3 getE_Position() {
	return e_cs.center;
}

void setE_Health(int damage) {
	e_health -= damage;
}

vec3 getDirection() {
	return e_direction;
}

void move(mat4 & e, vec3 & pos) {
	//e_position -= vec3(getPosition().v[0],0,getPosition().v[2]) * getDelta() * e_speed;

	if(e_isAlive) {
		e = scale(e, vec3(1.5,1.5,1.5));
		e_direction = vec3(getPosition() - e_cs.center);
		e_direction = normalise(e_direction);
		e_rotation.v[1] = std::acos(e_direction.v[2]);
		if(e_direction.v[0] > 0)
			e_rotation.v[1] =-e_rotation.v[1];
		e = rotate_x_deg(e, e_rotation.v[0]);
		e = rotate_y_deg(e, (-e_rotation.v[1] * (180/M_PI)));
		e = rotate_z_deg(e, e_rotation.v[2]);
	}
	if(e_isAlive) {
		vec3 newpos(e_cs.center);
		newpos += e_direction * e_speed;
//		newpos.v[1] -= 0.3;
		setE_Position(newpos);
		e = translate(e,getE_Position());
		
	}
//	e_position =  getPosition() - e_position;
//	temp -= getPosition() * getDelta() * e_speed;//vec3(getPosition().v[0],0,getPosition().v[2]);// * getDelta() * e_speed;

}

bool e_dead() {
	return (e_health <= 0) ? true : false;
}
void setE_Position(vec3& newpos) {
	e_cs.center = newpos;
}
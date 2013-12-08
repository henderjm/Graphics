#ifndef CONTROLS_HPP
#define CONTROLS_HPP
#include "maths_funcs.h"

void updateDelta(float delta);
void mouse_position(int x, int y);
void keyboard_action(unsigned char b, int x, int y);
void blah(int b, int x, int y);
void computeMatricesFromInputs();
float getDelta();
mat4 getViewMatrix();
mat4 getProjectionMatrix();
vec3 getPosition(); // eye
vec3 getP_Direction(); // direction for baddy to come after you :)
// ----Player atts


//vec3 direction; // pos + direction


int getPoints();
int getHealth();
void addPoints(int);
void setHealth(bool ,int);
void player_init();
void shoot(int b, int x, int y, int z);
#endif
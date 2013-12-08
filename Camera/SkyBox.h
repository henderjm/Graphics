#include <string>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>


void loadSkybox(char * a_sDirectory, char * a_sFront, char * a_sBack, char * a_sLeft, char * a_sRight, char * a_sTop, char * a_sBottom); 
void renderSkybox(); 

void releaseSkybox(); 

unsigned int uiVAO; 
//CVertexBufferObject vboRenderData; 
//CTexture tTextures[6]; 
//string sDirectory; 
//string sFront, sBack, sLeft, sRight, sTop, sBottom; 

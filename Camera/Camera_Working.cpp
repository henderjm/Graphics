#define _CRT_SECURE_NO_DEPRECATE

//Some Windows Headers (For Time, IO, etc.)
#include <windows.h>
#include <mmsystem.h>
#include "camera.h"
#include "enemy.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include "camera2.h"
//#include "maths_funcs.h"
#include "stb_image.h"
// Assimp includes
#include <assimp/cimport.h> // C importer
#include <assimp/scene.h> // collects data
#include <assimp/postprocess.h> // various extra operations
#include <stdio.h>
#include "obj_parser.h"
//#include <math.h>
#include <vector>
#include <assert.h>
#include "SOIL.h"
// Macro for indexing vertex buffer
#define BUFFER_OFFSET(i) ((char *)NULL + (i))
#define FRONT "../textures/skybox/negz.jpg"
#define BACK "../textures/skybox/posz.jpg"
#define TOP "../textures/skybox/posy.jpg"
#define BOTTOM "../textures/skybox/negy.jpg"
#define LEFT "../textures/skybox/negx.jpg"
#define RIGHT "../textures/skybox/posx.jpg"

//using namespace std;
GLuint shaderProgramID;

unsigned int teapot_vao = 0;
int width = 1024.0;
int height = 768.0;
GLuint loc1;
GLuint loc2;
GLuint loc3,loc4,loc5;
GLuint vaos[3] = {0,1,2};
// global floats to make objects move
std::vector<float> g_vp, g_vp2, g_vn, g_vn2, g_vt, g_vt2;
int g_point_count1,g_point_count2,g_point_count3;
float sky_points[] = {
	-10.0f,  10.0f, -10.0f,
	-10.0f, -10.0f, -10.0f,
	10.0f, -10.0f, -10.0f,
	10.0f, -10.0f, -10.0f,
	10.0f,  10.0f, -10.0f,
	-10.0f,  10.0f, -10.0f,

	-10.0f, -10.0f,  10.0f,
	-10.0f, -10.0f, -10.0f,
	-10.0f,  10.0f, -10.0f,
	-10.0f,  10.0f, -10.0f,
	-10.0f,  10.0f,  10.0f,
	-10.0f, -10.0f,  10.0f,

	10.0f, -10.0f, -10.0f,
	10.0f, -10.0f,  10.0f,
	10.0f,  10.0f,  10.0f,
	10.0f,  10.0f,  10.0f,
	10.0f,  10.0f, -10.0f,
	10.0f, -10.0f, -10.0f,

	-10.0f, -10.0f,  10.0f,
	-10.0f,  10.0f,  10.0f,
	10.0f,  10.0f,  10.0f,
	10.0f,  10.0f,  10.0f,
	10.0f, -10.0f,  10.0f,
	-10.0f, -10.0f,  10.0f,

	-10.0f,  10.0f, -10.0f,
	10.0f,  10.0f, -10.0f,
	10.0f,  10.0f,  10.0f,
	10.0f,  10.0f,  10.0f,
	-10.0f,  10.0f,  10.0f,
	-10.0f,  10.0f, -10.0f,

	-10.0f, -10.0f, -10.0f,
	-10.0f, -10.0f,  10.0f,
	10.0f, -10.0f, -10.0f,
	10.0f, -10.0f, -10.0f,
	-10.0f, -10.0f,  10.0f,
	10.0f, -10.0f,  10.0f
};
GLuint texs[3];
//GLfloat move = 0.0;
float rotate;
//vec3 cam_pos = vec3 (0.0f, 0.0f,2.0f);
float cam_yaw = 0.0f;
// angle of rotation for the camera direction
float angle=0.0;
mat4 zombie;
mat4 model;
enum {
	MOUSE_ZOOM_IN = 3,
	MOUSE_ZOOM_OUT = 4
};
/*----------------------------------------------------------------------------
MESH TO LOAD
----------------------------------------------------------------------------*/
// this mesh is a dae file format but you should be able to use any other format too, obj is typically what is used
// put the mesh in your project directory, or provide a filepath for it here
#define MESH_NAME "../Mesh/spiral.obj"
#define WORLD "../Mesh/world.dae"
#define OBJECT "../Mesh/test_cube.dae"
/*----------------------------------------------------------------------------
----------------------------------------------------------------------------*/
#pragma region MESH LOADING
/*----------------------------------------------------------------------------
MESH LOADING FUNCTION
----------------------------------------------------------------------------*/

bool load_mesh (const char* file_name, int & g_point_count) {
	const aiScene* scene = aiImportFile (file_name, aiProcess_MakeLeftHanded); // TRIANGLES!
	if (!scene) {
		fprintf (stderr, "ERROR: reading mesh %s\n", file_name);
		return false;
	}
	printf ("  %i animations\n", scene->mNumAnimations);
	printf ("  %i cameras\n", scene->mNumCameras);
	printf ("  %i lights\n", scene->mNumLights);
	printf ("  %i materials\n", scene->mNumMaterials);
	printf ("  %i meshes\n", scene->mNumMeshes);
	printf ("  %i textures\n", scene->mNumTextures);

	for (unsigned int m_i = 0; m_i < scene->mNumMeshes; m_i++) {
		const aiMesh* mesh = scene->mMeshes[m_i];
		printf ("    %i vertices in mesh\n", mesh->mNumVertices);
		g_point_count = mesh->mNumVertices;
		for (unsigned int v_i = 0; v_i < mesh->mNumVertices; v_i++) {
			if (mesh->HasPositions ()) {
				const aiVector3D* vp = &(mesh->mVertices[v_i]);
				//printf ("      vp %i (%f,%f,%f)\n", v_i, vp->x, vp->y, vp->z);
				g_vp.push_back (vp->x);
				g_vp.push_back (vp->y);
				g_vp.push_back (vp->z);
			}
			if (mesh->HasNormals ()) {
				const aiVector3D* vn = &(mesh->mNormals[v_i]);
				//printf ("      vn %i (%f,%f,%f)\n", v_i, vn->x, vn->y, vn->z);
				g_vn.push_back (vn->x);
				g_vn.push_back (vn->y);
				g_vn.push_back (vn->z);
			}
			if (mesh->HasTextureCoords (0)) {
				const aiVector3D* vt = &(mesh->mTextureCoords[0][v_i]);
				//printf ("      vt %i (%f,%f)\n", v_i, vt->x, vt->y);
				g_vt.push_back (vt->x);
				g_vt.push_back (vt->y);
			}
			if (mesh->HasTangentsAndBitangents ()) {
				// NB: could store/print tangents here
			}
		}
	}

	aiReleaseImport (scene);
	return true;
}

void drawSkybox(std::vector<float> & vp, int & g_point, GLuint & vao) {

	int length = sizeof(sky_points) / sizeof(float);
	std::cout << length <<"\n\n";
	for(int i = 0; i < length; i++) {
		vp.push_back (sky_points[i]);
	}
	loc1 = glGetAttribLocation(shaderProgramID, "vertex_position");
	if(length != vp.size())
		std::cout << length << "- length\tvp - " << vp.size() << std::endl;

	g_point = length;
	unsigned int vbo = 0;
	glGenBuffers (1, &vbo);
	glBindBuffer (GL_ARRAY_BUFFER, vbo);
	glBufferData (GL_ARRAY_BUFFER, 3 * 36 * sizeof (float), &vp[0], GL_STATIC_DRAW);

	std::cout << "vao dae: "<< vao <<std::endl;
	glGenVertexArrays (1, &vao);
	
	glBindVertexArray (vao);

	glEnableVertexAttribArray (loc1);
	glBindBuffer (GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer (loc1, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	texs[0] = SOIL_load_OGL_cubemap (
		RIGHT,
		LEFT,
		BOTTOM,
		TOP,

		BACK,
		FRONT,

		SOIL_LOAD_RGB,
		SOIL_CREATE_NEW_ID,
		0
		);
	glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	if (0 == texs[0]) {
		printf("SOIL loading error: '%s'\n", SOIL_last_result());
	}

	//	glBindVertexArray(vao);


	//	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texs[0]);
}

#pragma endregion /*MESH LOADING*/

// Shader Functions- click on + to expand
#pragma region SHADER_FUNCTIONS

std::string readShaderSource(const std::string& fileName)
{
	std::ifstream file(fileName);
	if(file.fail())
		return false;

	std::stringstream stream;
	stream << file.rdbuf();
	file.close();

	return stream.str();
}


static void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType)
{
	// create a shader object
	GLuint ShaderObj = glCreateShader(ShaderType);

	if (ShaderObj == 0) {
		fprintf(stderr, "Error creating shader type %d\n", ShaderType);
		exit(0);
	}
	std::string outShader = readShaderSource(pShaderText);
	const char* pShaderSource = outShader.c_str();

	// Bind the source code to the shader, this happens before compilation
	glShaderSource(ShaderObj, 1, (const GLchar**)&pShaderSource, NULL);
	// compile the shader and check for errors
	glCompileShader(ShaderObj);
	GLint success;
	// check for shader related errors using glGetShaderiv
	glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar InfoLog[1024];
		glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
		fprintf(stderr, "Error compiling shader type %d: '%s'\n", ShaderType, InfoLog);
		system("PAUSE");
		exit(1);
	}
	// Attach the compiled shader object to the program object
	glAttachShader(ShaderProgram, ShaderObj);
}

GLuint CompileShaders()
{
	//Start the process of setting up our shaders by creating a program ID
	//Note: we will link all the shaders together into this ID
	shaderProgramID = glCreateProgram();
	if (shaderProgramID == 0) {
		fprintf(stderr, "Error creating shader program\n");
		system("pause");
		exit(1);
	}

	// Create two shader objects, one for the vertex, and one for the fragment shader
	AddShader(shaderProgramID, "../Shaders/simpleVertexShader.txt", GL_VERTEX_SHADER);
	//	AddShader(shaderProgramID, "../Shaders/skybox_technique.vert", GL_VERTEX_SHADER);
	AddShader(shaderProgramID, "../Shaders/simpleFragmentShader.txt", GL_FRAGMENT_SHADER);
	//	AddShader(shaderProgramID, "../Shaders/skybox_technique.frag", GL_FRAGMENT_SHADER);

	GLint Success = 0;
	GLchar ErrorLog[1024] = { 0 };


	// After compiling all shader objects and attaching them to the program, we can finally link it
	glLinkProgram(shaderProgramID);
	// check for program related errors using glGetProgramiv
	glGetProgramiv(shaderProgramID, GL_LINK_STATUS, &Success);
	if (Success == 0) {
		glGetProgramInfoLog(shaderProgramID, sizeof(ErrorLog), NULL, ErrorLog);
		fprintf(stderr, "Error linking shader program: '%s'\n", ErrorLog);
		system("pause");
		exit(1);
	}

	// program has been successfully linked but needs to be validated to check whether the program can execute given the current pipeline state
	glValidateProgram(shaderProgramID);
	// check for program related errors using glGetProgramiv
	glGetProgramiv(shaderProgramID, GL_VALIDATE_STATUS, &Success);
	if (!Success) {
		glGetProgramInfoLog(shaderProgramID, sizeof(ErrorLog), NULL, ErrorLog);
		fprintf(stderr, "Invalid shader program: '%s'\n", ErrorLog);
		system("pause");
		exit(1);
	}
	// Finally, use the linked shader program
	// Note: this program will stay in effect for all draw calls until you replace it with another or explicitly disable its use
	glUseProgram(shaderProgramID);

	return shaderProgramID;
}
#pragma endregion SHADER_FUNCTIONS


/*--------------------------------------------------------------
Image Loading Function
--------------------------------------------------------------*/
bool load_image_to_texture (
	const char* file_name, unsigned int& tex, bool gen_mips
	) {
		printf ("loading image %s\n", file_name);
		int x, y, n;
		int force_channels = 4;
		unsigned char* image_data = stbi_load (file_name, &x, &y, &n, force_channels);
		if (!image_data) {
			fprintf (
				stderr,
				"ERROR: could not load image %s. Check file type and path\n",
				file_name
				);
			fprintf (stderr, "ERROR: could not load image %s", file_name);
			return false;
		}
		printf ("image loaded: %ix%i %i bytes per pixel\n", x, y, n);
		// NPOT check
		if (x & (x - 1) != 0 || y & (y - 1) != 0) {
			fprintf (
				stderr, "WARNING: texture %s is not power-of-2 dimensions\n", file_name
				);
			fprintf (stderr, "WARNING: texture %s is not power-of-two dimensions", file_name);
		}

		// FLIP UP-SIDE DIDDLY-DOWN
		// make upside-down copy for GL
		unsigned char *imagePtr = &image_data[0];
		int halfTheHeightInPixels = y / 2;
		int heightInPixels = y;

		// Assuming RGBA for 4 components per pixel.
		int numColorComponents = 4;
		// Assuming each color component is an unsigned char.
		int widthInChars = x * numColorComponents;
		unsigned char *top = NULL;
		unsigned char *bottom = NULL;
		unsigned char temp = 0;
		for( int h = 0; h < halfTheHeightInPixels; h++) {
			top = imagePtr + h * widthInChars;
			bottom = imagePtr + (heightInPixels - h - 1) * widthInChars;
			for (int w = 0; w < widthInChars; w++) {
				// Swap the chars around.
				temp = *top;
				*top = *bottom;
				*bottom = temp;
				++top;
				++bottom;
			}
		}

		// Copy into an OpenGL texture
		glGenTextures (1, &tex);
		glActiveTexture (GL_TEXTURE0);
		glBindTexture (GL_TEXTURE_2D, tex);
		//glPixelStorei (GL_UNPACK_ALIGNMENT, 1); // TODO?
		glTexImage2D (
			GL_TEXTURE_2D,
			0,
			GL_RGBA,
			x,
			y,
			0,
			GL_RGBA,
			GL_UNSIGNED_BYTE,
			image_data
			);
		// NOTE: need this or it will not load the texture at all
		if (gen_mips) {
			// shd be in core since 3.0 according to:
			// http://www.opengl.org/wiki/Common_Mistakes#Automatic_mipmap_generation
			// next line is to circumvent possible extant ATI bug
			// but NVIDIA throws a warning glEnable (GL_TEXTURE_2D);
			glGenerateMipmap (GL_TEXTURE_2D);
			printf ("mipmaps generated %s\n", file_name);
			glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 4);
		} else {
			glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		}
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		printf ("image loading done");
		stbi_image_free (image_data);

		return true;
}


// VBO Functions - click on + to expand
#pragma region VBO_FUNCTIONS

bool assign_texture(GLuint & vao, GLuint tex_id, char * TEXTURE) {
	//glUseProgram (shaderProgramID);
	
	tex_id = SOIL_load_OGL_texture (
		TEXTURE,
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_INVERT_Y
		);
	if (0 == tex_id) {
		printf("SOIL fucking loading error: '%s'\n", SOIL_last_result());
		return false;
	}

	glGenerateMipmap (GL_TEXTURE_2D);
//	printf ("mipmaps generated %s\n", TEXTURE);

	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 4);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glGenTextures (1, &tex_id);
	glBindTexture (GL_TEXTURE_2D, tex_id);
	//	
	return true;
	//	
}
void loading_meshes(std::vector<float> & vp, std::vector<float> & vn, GLuint & vao, int & g_count, unsigned int & vp_vbo, GLuint  & vn_vbo, unsigned int & vt_vbo, bool load_text, int tex_id, char * TEXTURE) {

	loc1 = glGetAttribLocation(shaderProgramID, "vertex_position");
	loc2 = glGetAttribLocation(shaderProgramID, "vertex_normal");
	loc3 = glGetAttribLocation(shaderProgramID, "basic_texture");

	glGenBuffers (1, &vp_vbo);
	glBindBuffer (GL_ARRAY_BUFFER, vp_vbo);
	glBufferData (GL_ARRAY_BUFFER, 3 * g_count * sizeof (float), &vp[0], GL_STATIC_DRAW);
	//	glBufferData (GL_ARRAY_BUFFER, 3 * g_point_count * sizeof (float), points, GL_STATIC_DRAW);

	if(!vn.empty()){
		glGenBuffers (1, &vn_vbo);
		glBindBuffer (GL_ARRAY_BUFFER, vn_vbo);
		//	glBufferData (GL_ARRAY_BUFFER, 3 * g_point_count * sizeof (float), normals, GL_STATIC_DRAW);
		glBufferData (GL_ARRAY_BUFFER, 3 * g_count * sizeof (float), &vn[0], GL_STATIC_DRAW);
	}

	//texture
	/*	glGenBuffers(1,&vt_vbo);
	glBindBuffer(GL_ARRAY_BUFFER,vt_vbo);
	int dimensions = 2;
	int length = 6;
	glBufferData(GL_ARRAY_BUFFER, 2*6*sizeof(float),&texc[0],GL_STATIC_DRAW);
	*/

	glGenVertexArrays (1, &vao);
	std::cout << "vao dae: "<< vao <<std::endl;
	glBindVertexArray (vao);

	glEnableVertexAttribArray (loc1);
	glBindBuffer (GL_ARRAY_BUFFER, vp_vbo);
	glVertexAttribPointer (loc1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray (loc2);
	glBindBuffer (GL_ARRAY_BUFFER, vn_vbo);
	glVertexAttribPointer (loc2, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	if(load_text) {
		if(!assign_texture(vao,tex_id, TEXTURE))
			printf("Error loading texture");

	}
	//	This is for texture coordinates which you don't currently need, so I have commented it out
//	glEnableVertexAttribArray (loc3);
//	glBindBuffer (GL_ARRAY_BUFFER, vt_vbo);
//	glVertexAttribPointer (loc3, 2, GL_FLOAT, GL_FALSE, 0, NULL);

	//	glBindVertexArray(0);

}

void loading_meshes(float * vp, float * vn, GLuint & vao, int & g_count, unsigned int vp_vbo, GLuint vn_vbo, unsigned int vt_vbo) {
	std::cout << "obj load\n";
	loc1 = glGetAttribLocation(shaderProgramID, "vertex_position");
	loc2 = glGetAttribLocation(shaderProgramID, "vertex_normal");
	loc3 = glGetAttribLocation(shaderProgramID, "basic_texture");

	glGenBuffers (1, &vp_vbo);
	glBindBuffer (GL_ARRAY_BUFFER, vp_vbo);
	glBufferData (GL_ARRAY_BUFFER, 3 * g_count * sizeof (float), &vp[0], GL_STATIC_DRAW);
	//	glBufferData (GL_ARRAY_BUFFER, 3 * g_point_count * sizeof (float), points, GL_STATIC_DRAW);

	glGenBuffers (1, &vn_vbo);
	glBindBuffer (GL_ARRAY_BUFFER, vn_vbo);
	//	glBufferData (GL_ARRAY_BUFFER, 3 * g_point_count * sizeof (float), normals, GL_STATIC_DRAW);
	glBufferData (GL_ARRAY_BUFFER, 3 * g_count * sizeof (float), &vn[0], GL_STATIC_DRAW);


	//texture
	/*	glGenBuffers(1,&vt_vbo);
	glBindBuffer(GL_ARRAY_BUFFER,vt_vbo);
	int dimensions = 2;
	int length = 6;
	glBufferData(GL_ARRAY_BUFFER, 2*6*sizeof(float),texcoords,GL_STATIC_DRAW);
	*/

	glGenVertexArrays (1, &vao);
	std::cout << "vao obj: "<< vao <<std::endl;
	glBindVertexArray (vao);

	glEnableVertexAttribArray (loc1);
	glBindBuffer (GL_ARRAY_BUFFER, vp_vbo);
	glVertexAttribPointer (loc1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray (loc2);
	glBindBuffer (GL_ARRAY_BUFFER, vn_vbo);
	glVertexAttribPointer (loc2, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	//	This is for texture coordinates which you don't currently need, so I have commented it out
	//	glEnableVertexAttribArray (loc3);
	//	glBindBuffer (GL_ARRAY_BUFFER, vt_vbo);
	//	glVertexAttribPointer (loc3, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindVertexArray(0);
}
void initVAO() {
	GLuint spaceship_vao;
	//glGenVertexArrays (2, &spaceship_vao);
	//std::cout << "vao: "<< vao <<std::endl;
	/*
	glBindVertexArray (vao);

	glEnableVertexAttribArray (loc1);
	glBindBuffer (GL_ARRAY_BUFFER, vp_vbo);
	glVertexAttribPointer (loc1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray (loc2);
	glBindBuffer (GL_ARRAY_BUFFER, vn_vbo);
	glVertexAttribPointer (loc2, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	//	This is for texture coordinates which you don't currently need, so I have commented it out
	//	glEnableVertexAttribArray (loc3);
	//	glBindBuffer (GL_ARRAY_BUFFER, vt_vbo);
	//	glVertexAttribPointer (loc3, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	*/
}
void generateObjectBufferTeapot () {
	const char * file_name = "../Shaders/grass.png";
	unsigned int tex = 0;
	unsigned int vp_vbo = 0, vt_vbo = 0;
	unsigned int tex_id;
	GLuint vn_vbo = 0;

	unsigned int vp_vbo2 = 0, vt_vbo2 = 0;
	GLuint vn_vbo2 = 0;
	unsigned int vp_vbo3 = 0, vt_vbo3 = 0;
	GLuint vn_vbo3 = 0;
	float * points = NULL, * normals = NULL, * textures = NULL;
	//	load_obj_file(MESH_NAME, points, textures, normals, g_point_count1);


	//	loading_meshes(g_vp, g_vn, vaos[0], g_point_count1, vp_vbo, vn_vbo, vt_vbo);
	g_vp.clear(); g_vn.clear(); g_vt.clear();
	// plane
	load_mesh(WORLD, g_point_count2);
	//	load_image_to_texture(file_name, tex, true);
	float texcoords[] = { 0.0f,1.0f,0.0f,0.0f,1.0f,0.0f,1.0,0.0,1.0,1.0,0.0,1.0 };
	loading_meshes(g_vp, g_vn, vaos[1], g_point_count2, vp_vbo2, vn_vbo2, vt_vbo2, true, tex = 1, "../Shaders/grass.bmp");
	g_vp.clear(); g_vn.clear();
	// spiral
	load_mesh(OBJECT, g_point_count3);
	loading_meshes(g_vp, g_vn, vaos[2], g_point_count3, vp_vbo3, vn_vbo3, vt_vbo3, true, tex = 1, "../Shaders/grass.bmp");
	e_init(&zombie, vec3(0.0,0.0, 10.0), collisionsphere(vec3(0.0,0.0, 10.0), 2), vec3(0,0,0), getPosition());
	g_vp.clear(); g_vn.clear();
//	drawSkybox(g_vp, g_point_count1, vaos[0]);
}


#pragma endregion VBO_FUNCTIONS

void renderSky() {
	//glUseProgram (shaderProgramID);
	glDepthMask(GL_TRUE);
	glBindVertexArray(vaos[0]);
	glActiveTexture(GL_TEXTURE0);
	//int mat = glGetUniformLocation(shaderProgramID, "model");
	//mat4 models = identity_mat4();

	//glUniformMatrix4fv(mat, 1, GL_FALSE, models.m);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 36);
	glDepthMask(GL_TRUE);
}

void display(){

	computeMatricesFromInputs();
	glEnable (GL_DEPTH_TEST); // enable depth-testing
	glDepthFunc (GL_LESS); // depth-testing interprets a smaller value as "closer"
	glClearColor (0.5f, 0.5f, 0.5f, 1.0f);
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram (shaderProgramID);
	//glBindTexture(GL_TEXTURE_2D, texs[1]);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(vaos[1]);
	
//	

	int matrix_location2 = glGetUniformLocation (shaderProgramID, "model");
	int view_mat_location2 = glGetUniformLocation (shaderProgramID, "view");
	int proj_mat_location2 = glGetUniformLocation (shaderProgramID, "proj");
	int tex_loc2 = glGetUniformLocation (shaderProgramID,"basic_texture");

	mat4 viewMatrix2 = getViewMatrix();
	mat4 persp_proj2 = getProjectionMatrix();
	mat4 model3 = identity_mat4 ();
	model3 = rotate_x_deg(model3,90);
	model3 = scale(model3, vec3(20,20,20));

	glUniformMatrix4fv (proj_mat_location2, 1, GL_FALSE, persp_proj2.m);
	glUniformMatrix4fv (view_mat_location2, 1, GL_FALSE, viewMatrix2.m);
	glUniformMatrix4fv (matrix_location2, 1, GL_FALSE, model3.m);
	//	glBindTexture(GL_TEXTURE_2D, texs[1]);
	glDrawArrays(GL_TRIANGLES,0,g_point_count2);

	glBindVertexArray(vaos[2]);
	int matrix_location3 = glGetUniformLocation (shaderProgramID, "model");
	int view_mat_location3 = glGetUniformLocation (shaderProgramID, "view");
	int proj_mat_location3 = glGetUniformLocation (shaderProgramID, "proj");

	zombie = identity_mat4();
	
	move(zombie, getPosition());
	
//	zombie = translate(zombie, getE_Position());
	//zombie = translate(zombie, vec3(0.0,-20.0, 0));
	
	
	glUniformMatrix4fv (proj_mat_location3, 1, GL_FALSE, persp_proj2.m);
	glUniformMatrix4fv (view_mat_location3, 1, GL_FALSE, viewMatrix2.m);
	glUniformMatrix4fv(matrix_location3,1,GL_FALSE, zombie.m);
	glDrawArrays(GL_TRIANGLES, 0, g_point_count3);
	// ---------------------------SKYBOX----------------------------------------------------------------
	
	glDepthMask (GL_FALSE);


	glActiveTexture(GL_TEXTURE0);glBindTexture(GL_TEXTURE_CUBE_MAP, texs[0]);
	glBindVertexArray(vaos[0]);
	int matrix_location = glGetUniformLocation (shaderProgramID, "model");
	int view_mat_location = glGetUniformLocation (shaderProgramID, "view");
	int proj_mat_location = glGetUniformLocation (shaderProgramID, "proj");
	int tex_loc = glGetUniformLocation (shaderProgramID,"basic_texture");

	mat4 viewMatrix = getViewMatrix();
	mat4 persp_proj = getProjectionMatrix();
	model = identity_mat4();
	model = rotate_x_deg(model, 90);
	model = translate(model, getPosition());
//	mat4 global = model3 * model;
	glUniformMatrix4fv (proj_mat_location, 1, GL_FALSE, persp_proj.m);
	glUniformMatrix4fv (view_mat_location, 1, GL_FALSE, viewMatrix.m);
	glUniformMatrix4fv (matrix_location, 1, GL_FALSE, model.m);
	glDrawArrays(GL_TRIANGLES,0,36);
	// tell GL to only draw onto a pixel if the shape is closer to the viewer
	glDepthMask (GL_TRUE);
	//glUseProgram (shaderProgramID);
	/*	glBindVertexArray(vaos[1]);
	//Declare your uniform variables that will be used in your shader
	int matrix_location2 = glGetUniformLocation (shaderProgramID, "model");

	//	int tex_loc = glGetUniformLocation (shaderProgramID,"basic_texture");
	glUniform1i(tex_loc,0);
	mat4 model2 = identity_mat4();
	model2 = scale(model2, vec3(100,100,100));
	model2 = rotate_x_deg(model2, 90);
	mat4 global2 = global2 * model2;


	glUniformMatrix4fv(matrix_location2, 1, GL_FALSE, model2.m);
	glDrawArrays(GL_TRIANGLES,0, g_point_count2); */
	/*

	// BOTTOM-LEFT
	mat4 viewMatrix2 = getViewMatrix();
	mat4 persp_proj2 = getProjectionMatrix();

	mat4 model2 = identity_mat4 ();

	model2 = rotate_z_deg (model, 180) * rotate_y_deg(model, 180);
	model2 = scale (model2, vec3(.1,.1,.1));
	mat4 global1 = model2;
	//	view = translate (view, vec3 (-cam_pos[0], -cam_pos[1], -cam_pos[2]));
	//	glViewport (200, 150, width / 2, height / 2);
	glUniformMatrix4fv (proj_mat_location2, 1, GL_FALSE, persp_proj2.m);
	glUniformMatrix4fv (view_mat_location2, 1, GL_FALSE, viewMatrix2.m);
	glUniformMatrix4fv (matrix_location2, 1, GL_FALSE, model2.m);
	glDrawArrays (GL_TRIANGLES, 36, g_point_count3);
	*/

	//	int matrix_location2 = glGetUniformLocation(shaderProgramID, "model");


	//	glDrawElements(GL_TRIANGLES, g_point_count2, GL_UNSIGNED_SHORT, &vaos[0]);
	/*
	glBindVertexArray(vaos[1]);
	int matrix_location2 = glGetUniformLocation (shaderProgramID, "model");
	int view_mat_location2 = glGetUniformLocation (shaderProgramID, "view");
	int proj_mat_location2 = glGetUniformLocation (shaderProgramID, "proj");
	mat4 model2 = identity_mat4 ();

	model2 = rotate_x_deg (model, 180) * rotate_y_deg(model, 180);
	//	model2 = scale (model, vec3(5,5,5));
	//	view = translate (view, vec3 (-cam_pos[0], -cam_pos[1], -cam_pos[2]));
	//	glViewport (200, 150, width / 2, height / 2);
	glUniformMatrix4fv (matrix_location2, 1, GL_FALSE, model2.m);
	//	mat4 spiral = identity_mat4();
	//	mat4 global = model * spiral;
	//	glUniformMatrix4fv(matrix_location,1,GL_FALSE, global.m);
	glDrawArrays(GL_TRIANGLES,g_point_count2 + 1,g_point_count3);
	//	glDrawElements(GL_TRIANGLES, g_point_count3, GL_UNSIGNED_SHORT, &vaos[1]);
	*/
	glutSwapBuffers();

}


void updateScene() {	

	// Wait until at least 16ms passed since start of last frame (Effectively caps framerate at ~60fps)
	static DWORD  last_time = 0;
	DWORD  curr_time = timeGetTime();
	//	deltaTimeTime = float (curr_time - last_time);
	float delta = (curr_time - last_time) * 0.1f;
	if (delta > 0.3f){
		delta = 0.3f;
		//	deltaTimeTime = 0.03f;
	}
	last_time = curr_time;
	updateDelta(delta);
	// Draw the next frame
	glutPostRedisplay();
}


void init()
{
	// Create 3 vertices that make up a triangle that fits on the viewport 
	GLfloat vertices[] = {-1.0f, -1.0f, 0.0f, 1.0,
		1.0f, -1.0f, 0.0f, 1.0, 
		0.0f, 1.0f, 0.0f, 1.0};
	// Create a color array that identfies the colors of each vertex (format R, G, B, A)
	GLfloat colors[] = {0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f};
	// Set up the shaders
	player_init();
	GLuint shaderProgramID = CompileShaders();
	// load teapot mesh into a vertex buffer array
	generateObjectBufferTeapot ();
	computeMatricesFromInputs();
	glutSetCursor(GLUT_CURSOR_NONE);
	//	glutWarpPointer(1024/2, 768/2);
}


int main(int argc, char** argv){
	//	std::cout << "position x = " << getx(position);

	//	
	//	cam.setLocation(position);
	//	print(direction);
	// Set up the window

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);
	glutInitWindowSize(width, height);
	glutCreateWindow("Viewport Teapots");
	// Tell glut where the display function is
	glutDisplayFunc(display);
	glutIdleFunc(updateScene);
	glutKeyboardFunc(keyboard_action);
	glutSpecialFunc(blah);
	glutPassiveMotionFunc(mouse_position);
	glutMouseFunc(shoot);
	// A call to glewInit() must be done after glut is initialized!
	GLenum res = glewInit();
	// Check for any errors
	if (res != GLEW_OK) {
		fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
		return 1;
	}

	// Set up your objects and shaders
	init();
	// Begin infinite event loop
	glutMainLoop();
	return 0;
}
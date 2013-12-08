#include "maths_funcs.h"
#include <string>
#include <list>
#include <GL/glew.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include<sdl.h>
#include <windows.h>
#include <windowsx.h>
#include <vector> // STL dynamic memory.
class game_component
{
private:            
	void RotateVectors();
protected:
public:        
	game_component(void);
	virtual bool Initialise();
    virtual void Update(float timeDelta);
    virtual void Draw();
	
	virtual void Cleanup();
//   GameComponent * parent;
    vec3 position;
    vec3 look;
    vec3 up;
    vec3 right;
    vec3 scale;
    vec3 velocity;
    mat4 world;
    versor orientation;
    vec3 ambient;
    vec3 specular;
    vec3 diffuse; // Will use either this of the array of colours depending on the drawMode setting
    bool initialised;        
    std::string tag;
	enum draw_modes {materials, textured, single_material};
    enum world_modes {from_self, from_self_with_parent, from_child, to_parent, from_parent};


    draw_modes drawMode;
    world_modes worldMode;
   
	// std::list<std::shared_ptr<GameComponent>> children;
    static const vec3 basisUp;
    static const vec3 basisLook;
    static const vec3 basisRight;
    bool moved;
    float speed;
	bool alive;
	void Strafe(float units); 
    void Fly(float units);    
	void Walk(float units);                   
	void Pitch(float angle); // rotate on right vector
	void Yaw(float angle);   // rotate on up vector
	void Roll(float angle);  // rotate on look vector
	void RecalculateVectors();
	void UpdateFromParent();
};
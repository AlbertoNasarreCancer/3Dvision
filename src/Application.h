/*  by Javi Agenjo 2013 UPF  javi.agenjo@gmail.com
	This class encapsulates the game, is in charge of creating the game, getting the user input, process the update and render.
*/

#ifndef GAME_H
#define GAME_H

#include "includes.h"
#include "Camera.h"
#include "Mesh.h"
#include "Texture.h"
#include "Shader.h"
#include <memory>

class Application
{
public:
	static Application* instance;

	//window
	SDL_Window* window;
	int window_width;
	int window_height;

	float time;

	//keyboard state
	const Uint8* keystate;

	
	enum class PoligonMode
	{
		POINT = GL_POINTS,
		LINE = GL_LINES,
		FILL = GL_TRIANGLES
	};

	//Defines the poligon mode
	PoligonMode poligonMode;

	std::vector<std::unique_ptr<Mesh>> aseFiles;
	int aseIndex;
	
	std::vector<std::unique_ptr<Texture>> aseTextures;
	Shader shader1;
	Shader shader3;
	Shader shader2;
	Shader shader4;
	Shader shader5;
	Shader shader6;
	Shader shader7;

	int shaderId;
	

	//mouse state
	int mouse_state; //tells which buttons are pressed
	Vector2 mouse_position; //last mouse position
	Vector2 mouse_delta; //mouse movement in the last frame

	Camera* camera; //our global camera

	Application( SDL_Window* window );
	~Application( void );

	void init( void );
	void render( void );
	void update( float dt );

	
	void renderShader1(const int aseIndex, const Matrix44& transform);
	void renderShader2( const int aseIndex, const Matrix44& transform );
	void renderShader3(const int aseIndex, const Matrix44& transform);
	void renderShader4(const int aseIndex, const Matrix44& transform);
	void renderShader5(const int aseIndex, const Matrix44& transform);
	void renderShader6(const int aseIndex, const Matrix44& transform);
	void renderShader7(const int aseIndex, const Matrix44& transform);
	

	void onKeyPressed( SDL_KeyboardEvent event );
	void onMouseButton( SDL_MouseButtonEvent event );
	void onResize( SDL_Event e );

	void setWindowSize( int width, int height );

	float speed_car;
};


#endif 

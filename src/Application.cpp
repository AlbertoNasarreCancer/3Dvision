#include "Application.h"
#include "Utils.h"
#include "Mesh.h"
#include "Light.h"
#include "Material.h"
#include <cmath>

//some globals
Mesh* mesh = NULL;
int flag = 1;

Application* Application::instance = NULL;

Application::Application( SDL_Window* window )
{
	this->window = window;
	instance = this;

	// initialize attributes
	// Warning: DO NOT CREATE STUFF HERE, USE THE INIT 
	// things create here cannot access opengl
	SDL_GetWindowSize( window, &window_width, &window_height );
	keystate = NULL;
}

Application::~Application( void )
{
	delete camera;
	delete mesh;

	aseFiles.clear();
}

//Here we have already GL working, so we can create meshes and textures
void Application::init( void )
{

	//OpenGL flags
	glEnable( GL_CULL_FACE ); //render both sides of every triangle
	glEnable( GL_DEPTH_TEST ); //check the occlusions using the Z buffer
	//To have double sided polygons
	//glDisable( GL_CULL_FACE );

	//create our camera
	camera = new Camera();
	camera->lookAt( Vector3( 0, 25, 25 ), Vector3( 0, 0, 0 ), Vector3( 0, 1, 0 ) ); //position the camera and point to 0,0,0
	camera->setPerspective( 70, window_width / (float)window_height, 0.1f, 10000 ); //set the projection, we want to be perspective

	//create a plane mesh
	mesh = new Mesh();
	mesh->createPlane( 10 );

	

	
	aseFiles.push_back( std::unique_ptr<Mesh>( new Mesh( "Box2.ASE", 0.1f ) ) );
	aseFiles.push_back( std::unique_ptr<Mesh>( new Mesh( "teapot.ASE", 0.1f ) ) );
	aseFiles.push_back( std::unique_ptr<Mesh>( new Mesh( "Girl.ASE", 10.f ) ) );
	aseFiles.push_back( std::unique_ptr<Mesh>( new Mesh( "dark_fighter_6.ASE", 0.15f ) ) );
	aseFiles.push_back( std::unique_ptr<Mesh>( new Mesh( "delorean.ASE", 0.15f ) ) );

	//hacer lo mismo pero con las texturas
	aseTextures.push_back(std::unique_ptr<Texture>(new Texture("bricks.bmp")));
	aseTextures.push_back(std::unique_ptr<Texture>(new Texture("Jellyfish.bmp")));
	aseTextures.push_back(std::unique_ptr<Texture>(new Texture("torsoDiffuseMap.bmp")));
	aseTextures.push_back(std::unique_ptr<Texture>(new Texture("dark_fighter_6.bmp")));
	aseTextures.push_back(std::unique_ptr<Texture>(new Texture("delorean.bmp")));

	

	aseIndex = 0;
	
	shaderId = 1;

	shader1.load( "Vertex_2.glsl", "Fragment_2.glsl" );//**********************************
	shader2.load("Vertex_1.glsl", "Fragment_1.glsl");//**********************************
	shader3.load("Vertex_3.glsl", "Fragment_3.glsl");
	shader4.load("Vertex_4.glsl", "Fragment_4.glsl");
	shader5.load("Vertex_5.glsl", "Fragment_5.glsl");
	shader6.load("Vertex_6.glsl", "Fragment_6.glsl");
	shader7.load("Vertex_8.glsl", "Fragment_8.glsl");


	poligonMode = PoligonMode::FILL;

	//hide the cursor
	SDL_ShowCursor( true ); //hide or show the mouse

	speed_car = 1;
}

//what to do when the image has to be draw
void Application::render( void )
{
	glClearColor( 1.0, 0.0, 0.0, 1.0 );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	//set the clear color (the background color)
	glClearColor( 0.0, 0.0, 0.0, 1.0 );

	// Clear the window and the depth buffer
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	//Put the camera matrices on the stack of OpenGL (only for fixed rendering)
	camera->set();

	//Draw out world
	//drawGrid( 500 ); //background grid

	
	glEnable( GL_BLEND ); // Enable alpha channel
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

	Matrix44 transform;

	
	Matrix44 transform1;
	Matrix44 transform_coche;
	Matrix44 transform_coche2;
	Matrix44 transform2 = transform;
	Matrix44 transform3 = transform;
	Matrix44 transform4;

	
	Vector3 a = Vector3(1, 1, 1);
	float aa = 5;
	
	
	


	transform2.setRotation(M_PI*sin(time - 0.1) / 3, Vector3(0, 1, 0));
	transform3.setTranslation(sin(time), 18, 0);
	transform1.setRotation(M_PI, Vector3(0, 1, 0));
	transform = transform2 * transform3 * transform1;

	transform2.setRotation(M_PI*sin(-1 * time) / 3, Vector3(0, 1, 0));
	transform3.setTranslation(sin(time), 18, 10);
	transform4 = transform2 * transform3;
	
	
	
	
	
	
	
	transform2.setRotation(speed_car*time, Vector3(0, 1, 0));
	transform3.setTranslation(20 * cos(speed_car*time), 0, 20 * sin(speed_car*time));
	transform_coche = transform2 * transform3;

	transform2.setRotation((speed_car*time)+3.14, Vector3(0, 1, 0));
	transform3.setTranslation(-10 * cos(speed_car*time), 0, -10 * sin(speed_car*time));
	transform_coche2 = transform2 * transform3;
	
	
	mesh->render(GL_TRIANGLES);
	if (flag == 1) {
		renderShader1(1, transform);
	}
	else if (flag == 2)
		renderShader2(aseIndex, transform);
	else if (flag == 3)
		renderShader3(aseIndex, transform);
	else if (flag == 4)
		renderShader4(aseIndex, transform);
	else if (flag == 5)
		renderShader5(aseIndex, transform);
	else if (flag == 6)
		renderShader6(aseIndex, transform);
	else if (flag == 7) {
		

		renderShader6(3, transform);
		renderShader6(3, transform4);
		renderShader5(4, transform_coche);
		renderShader6(4, transform_coche2);
	} else if (flag == 8) {

		mesh->createPlane(100000);
		renderShader7(3, transform);
		renderShader7(3, transform4);
		renderShader7(4, transform_coche);
		renderShader7(4, transform_coche2);
	}

		
		//aseFiles[aseIndex]->render(static_cast<std::underlying_type<PoligonMode>::type>(poligonMode));

	
	
	//swap between front buffer and back buffer
	SDL_GL_SwapWindow( this->window );
}

void Application::renderShader1(const int aseIndex, const Matrix44& transform)
{
	
	shader1.enable();

	int position = shader1.getAttribLocation("Position");

	shader1.setMatrix44("mvp", camera->viewprojection_matrix);

	Matrix44 tanslation2Centroid;
	tanslation2Centroid.setTranslation(aseFiles[aseIndex]->centroid.x, aseFiles[aseIndex]->centroid.y, aseFiles[aseIndex]->centroid.z);
	Matrix44 invTanslation2Centroid = tanslation2Centroid;
	invTanslation2Centroid.inverse();

	shader1.setMatrix44("transform", invTanslation2Centroid * transform * tanslation2Centroid);

	glEnableVertexAttribArrayARB(position);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, aseFiles[aseIndex]->vertices_vbo_id);
	glVertexAttribPointerARB(position, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3), 0);

	aseFiles[aseIndex]->render(static_cast<std::underlying_type<PoligonMode>::type>(poligonMode));
	shader1.disable();
	
}

void Application::renderShader2( const int aseIndex, const Matrix44& transform )
{
	
	shader2.enable();

	int position = shader2.getAttribLocation( "Position" );
	int normal = shader2.getAttribLocation("Normal");
	
	shader2.setMatrix44( "mvp", camera->viewprojection_matrix );

	Matrix44 tanslation2Centroid;
	tanslation2Centroid.setTranslation(aseFiles[aseIndex]->centroid.x, aseFiles[aseIndex]->centroid.y, aseFiles[aseIndex]->centroid.z);
	Matrix44 invTanslation2Centroid = tanslation2Centroid;
	invTanslation2Centroid.inverse();

	shader2.setMatrix44( "transform", invTanslation2Centroid * transform * tanslation2Centroid );

	glEnableVertexAttribArrayARB( position );
	glBindBufferARB( GL_ARRAY_BUFFER_ARB, aseFiles[ aseIndex ]->vertices_vbo_id );
	glVertexAttribPointerARB( position, 3, GL_FLOAT, GL_FALSE, sizeof( Vector3 ), 0 );

	glEnableVertexAttribArrayARB(normal);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, aseFiles[aseIndex]->normals_vbo_id);
	glVertexAttribPointerARB(normal, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3), 0);

	aseFiles[ aseIndex ]->render( static_cast<std::underlying_type<PoligonMode>::type>( poligonMode ) );

	shader2.disable();

	
}

void Application::renderShader3(const int aseIndex, const Matrix44& transform)
{

	
	shader3.enable();

	int position = shader3.getAttribLocation("Position");
	int texture = shader3.getAttribLocation("Texture");

	shader3.setMatrix44("mvp", camera->viewprojection_matrix);

	shader3.setTexture("texture1", *aseTextures[aseIndex]);

	Matrix44 tanslation2Centroid;
	tanslation2Centroid.setTranslation(aseFiles[aseIndex]->centroid.x, aseFiles[aseIndex]->centroid.y, aseFiles[aseIndex]->centroid.z);
	Matrix44 invTanslation2Centroid = tanslation2Centroid;
	invTanslation2Centroid.inverse();

	shader3.setMatrix44("transform", invTanslation2Centroid * transform * tanslation2Centroid);

	glEnableVertexAttribArrayARB(position);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, aseFiles[aseIndex]->vertices_vbo_id);
	glVertexAttribPointerARB(position, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3), 0);

	glEnableVertexAttribArrayARB(texture);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, aseFiles[aseIndex]->uvs_vbo_id);
	glVertexAttribPointerARB(texture, 2, GL_FLOAT, GL_FALSE, sizeof(Vector2), 0);

	aseFiles[aseIndex]->render(static_cast<std::underlying_type<PoligonMode>::type>(poligonMode));

	shader3.disable();
	
}

void Application::renderShader4(const int aseIndex, const Matrix44& transform)
{
	
	shader4.enable();

	int position = shader4.getAttribLocation("Position");
	int normal = shader4.getAttribLocation("Normal");
	int texture = shader4.getAttribLocation("Texture");//////

	// LIGHT
	float La = 0.4;
	float Ld = 2;
	float Ls = 1;
	float R = 1;
	float G = 1;
	float B = 1;

	// MATERIAL
	float Ka = 0.2f;
	float Kd = 0.4f;
	float Ks = 0.7f;
	float Shininess = 50.f;

	// GENERATE
	Light light("Light", Vector3(0, 1, 0), Vector3(La*R, La*G, La*B), Vector3(Ld*R, Ld*G, Ld*B), Vector3(Ls*R, Ls*G, Ls*B));////
	Material material("Material", Vector3(Ka, Ka, Ka), Vector3(Kd, Kd, Kd), Vector3(Ks, Ks, Ks), Shininess);////

	light.bind(shader4);///
	material.bind(shader4);///

	shader4.setMatrix44("mvp", camera->viewprojection_matrix);
	shader4.setTexture("texture1", *aseTextures[aseIndex]);
	shader4.setVector3("V", camera->eye);/////

	Matrix44 tanslation2Centroid;
	tanslation2Centroid.setTranslation(aseFiles[aseIndex]->centroid.x, aseFiles[aseIndex]->centroid.y, aseFiles[aseIndex]->centroid.z);
	Matrix44 invTanslation2Centroid = tanslation2Centroid;
	invTanslation2Centroid.inverse();

	Matrix44 model_matrix = invTanslation2Centroid * transform * tanslation2Centroid;

	shader4.setMatrix44("transform", model_matrix);

	Matrix44 inv_model_matrix = model_matrix; ////
	inv_model_matrix.inverse();///
	inv_model_matrix.transpose();///

	shader4.setMatrix44("inv_model_matrix", inv_model_matrix);///

	glEnableVertexAttribArrayARB(position);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, aseFiles[aseIndex]->vertices_vbo_id);
	glVertexAttribPointerARB(position, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3), 0);

	glEnableVertexAttribArrayARB(texture);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, aseFiles[aseIndex]->uvs_vbo_id);
	glVertexAttribPointerARB(texture, 2, GL_FLOAT, GL_FALSE, sizeof(Vector2), 0);

	aseFiles[aseIndex]->render(static_cast<std::underlying_type<PoligonMode>::type>(poligonMode));

	shader4.disable();
}

void Application::renderShader5(const int aseIndex, const Matrix44& transform)
{
	shader5.enable();

	int position = shader5.getAttribLocation("Position");
	int normal = shader5.getAttribLocation("Normal");
	int texture = shader5.getAttribLocation("Texture");

	// LIGHT
	float La = 0.4;
	float Ld = 2;
	float Ls = 1;
	float R = 1;
	float G = 1;
	float B = 1;

	// MATERIAL
	float Ka = 0.2f;
	float Kd = 0.4f;
	float Ks = 0.7f;
	float Shininess = 50.f;

	// GENERATE
	Light light("Light", Vector3(0, 0, 10), Vector3(La*R, La*G, La*B), Vector3(Ld*R, Ld*G, Ld*B), Vector3(Ls*R, Ls*G, Ls*B));
	Material material("Material", Vector3(Ka, Ka, Ka), Vector3(Kd, Kd, Kd), Vector3(Ks, Ks, Ks), Shininess);

	light.bind(shader5);
	material.bind(shader5);

	shader5.setMatrix44("mvp", camera->viewprojection_matrix);
	shader5.setTexture("texture1", *aseTextures[aseIndex]);
	shader5.setVector3("V", camera->eye);

	Matrix44 tanslation2Centroid;
	tanslation2Centroid.setTranslation(aseFiles[aseIndex]->centroid.x, aseFiles[aseIndex]->centroid.y, aseFiles[aseIndex]->centroid.z);
	Matrix44 invTanslation2Centroid = tanslation2Centroid;
	invTanslation2Centroid.inverse();

	Matrix44 model_matrix = invTanslation2Centroid * transform * tanslation2Centroid;

	shader5.setMatrix44("transform", model_matrix);

	Matrix44 inv_model_matrix = model_matrix;
	inv_model_matrix.inverse();
	inv_model_matrix.transpose();

	shader5.setMatrix44("inv_model_matrix", inv_model_matrix);

	glEnableVertexAttribArrayARB(position);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, aseFiles[aseIndex]->vertices_vbo_id);
	glVertexAttribPointerARB(position, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3), 0);

	glEnableVertexAttribArrayARB(texture);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, aseFiles[aseIndex]->uvs_vbo_id);
	glVertexAttribPointerARB(texture, 2, GL_FLOAT, GL_FALSE, sizeof(Vector2), 0);

	aseFiles[aseIndex]->render(static_cast<std::underlying_type<PoligonMode>::type>(poligonMode));

	shader5.disable();
}



void Application::renderShader6(const int aseIndex, const Matrix44& transform)
{
	shader6.enable();

	int position = shader6.getAttribLocation("Position");
	int normal = shader6.getAttribLocation("Normal");
	int texture = shader6.getAttribLocation("Texture");

	// LIGHT
	float La = 0.5;
	float Ld = 2;
	float Ls = 2;
	float R = 1;
	float G = 1;
	float B = 1;

	// MATERIAL
	float Ka = 0.2f;
	float Kd = 0.4f;
	float Ks = 0.7f;
	float Shininess = 80.f;

	shader6.setFloat("in_ang", 120 * DEG2RAD);
	shader6.setFloat("out_ang", 130 * DEG2RAD);

	// GENERATE
	Light light("Light", Vector3(0.2, 0.2, 0), Vector3(La*R, La*G, La*B), Vector3(Ld*R, Ld*G, Ld*B), Vector3(Ls*R, Ls*G, Ls*B));
	Material material("Material", Vector3(Ka, Ka, Ka), Vector3(Kd, Kd, Kd), Vector3(Ks, Ks, Ks), Shininess);

	light.bind(shader6);
	material.bind(shader6);

	shader6.setMatrix44("mvp", camera->viewprojection_matrix);
	shader6.setTexture("texture1", *aseTextures[aseIndex]);
	shader6.setVector3("V", camera->eye);

	Matrix44 tanslation2Centroid;
	tanslation2Centroid.setTranslation(aseFiles[aseIndex]->centroid.x, aseFiles[aseIndex]->centroid.y, aseFiles[aseIndex]->centroid.z);
	Matrix44 invTanslation2Centroid = tanslation2Centroid;
	invTanslation2Centroid.inverse();

	Matrix44 model_matrix = invTanslation2Centroid * transform * tanslation2Centroid;

	shader6.setMatrix44("transform", model_matrix);

	Matrix44 inv_model_matrix = model_matrix;
	inv_model_matrix.inverse();
	inv_model_matrix.transpose();

	shader6.setMatrix44("inv_model_matrix", inv_model_matrix);

	glEnableVertexAttribArrayARB(position);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, aseFiles[aseIndex]->vertices_vbo_id);
	glVertexAttribPointerARB(position, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3), 0);

	glEnableVertexAttribArrayARB(texture);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, aseFiles[aseIndex]->uvs_vbo_id);
	glVertexAttribPointerARB(texture, 2, GL_FLOAT, GL_FALSE, sizeof(Vector2), 0);

	aseFiles[aseIndex]->render(static_cast<std::underlying_type<PoligonMode>::type>(poligonMode));

	shader6.disable();
}

void Application::renderShader7(const int aseIndex, const Matrix44& transform)
{
	shader7.enable();

	int position = shader7.getAttribLocation("Position");
	int normal = shader7.getAttribLocation("Normal");
	int texture = shader7.getAttribLocation("Texture");

	// LIGHT
	float La = 0.5;
	float Ld = 2;
	float Ls = 2;
	float R = 1;
	float G = 1;
	float B = 1;

	// MATERIAL
	float Ka = 0.2f;
	float Kd = 0.4f;
	float Ks = 0.7f;
	float Shininess = 50.f;

	shader7.setFloat("in_ang", 120 * DEG2RAD);
	shader7.setFloat("out_ang", 130 * DEG2RAD);

	// GENERATE
	Light light("Light", Vector3(0.2, 0.2, 0.2), Vector3(La*R, La*G*0, La*B), Vector3(Ld*R, Ld*G*0, Ld*B), Vector3(Ls*R, Ls*G*0, Ls*B));
	Light light2("Light2", Vector3(0, 100, 0), Vector3(La*R*0, La*G, La*B), Vector3(Ld*R*0, Ld*G, Ld*B), Vector3(Ls*R*0, Ls*G, Ls*B));
	Light light3("Light3", Vector3(0.1, 0, 0.1), Vector3(La*R, La*G, La*B * 0), Vector3(Ld*R, Ld*G, Ld*B * 0), Vector3(Ls*R, Ls*G, Ls*B * 0));
	Material material("Material", Vector3(Ka, Ka, Ka), Vector3(Kd, Kd, Kd), Vector3(Ks, Ks, Ks), Shininess);

	light.bind(shader7);
	light2.bind(shader7);
	light3.bind(shader7);
	material.bind(shader7);

	shader7.setMatrix44("mvp", camera->viewprojection_matrix);
	shader7.setTexture("texture1", *aseTextures[aseIndex]);
	shader7.setVector3("V", camera->eye);

	Matrix44 tanslation2Centroid;
	tanslation2Centroid.setTranslation(aseFiles[aseIndex]->centroid.x, aseFiles[aseIndex]->centroid.y, aseFiles[aseIndex]->centroid.z);
	Matrix44 invTanslation2Centroid = tanslation2Centroid;
	invTanslation2Centroid.inverse();

	Matrix44 model_matrix = invTanslation2Centroid * transform * tanslation2Centroid;

	shader7.setMatrix44("transform", model_matrix);

	Matrix44 inv_model_matrix = model_matrix;
	inv_model_matrix.inverse();
	inv_model_matrix.transpose();

	shader7.setMatrix44("inv_model_matrix", inv_model_matrix);

	glEnableVertexAttribArrayARB(position);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, aseFiles[aseIndex]->vertices_vbo_id);
	glVertexAttribPointerARB(position, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3), 0);

	glEnableVertexAttribArrayARB(texture);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, aseFiles[aseIndex]->uvs_vbo_id);
	glVertexAttribPointerARB(texture, 2, GL_FLOAT, GL_FALSE, sizeof(Vector2), 0);

	aseFiles[aseIndex]->render(static_cast<std::underlying_type<PoligonMode>::type>(poligonMode));

	shader7.disable();
}

void Application::update( float seconds_elapsed )
{
	
	float speed = seconds_elapsed * 10; //the speed is defined by the seconds_elapsed so it goes constant

	int x, y;

	float z = time;
	
	mouse_state = SDL_GetMouseState(&x, &y);
	mouse_delta = mouse_position - Vector2(x, y);
	mouse_position = Vector2(x, y);

										 //mouse input to rotate the cam
	if( mouse_state & SDL_BUTTON( SDL_BUTTON_LEFT ) ) //is left button pressed?
	{
		camera->rotate( mouse_delta.x * 0.005f, Vector3( 0, -1, 0 ) );
		camera->rotate( mouse_delta.y * 0.005f, camera->getLocalVector( Vector3( -1, 0, 0 ) ) );
	}

	if( mouse_state & SDL_BUTTON( SDL_BUTTON_RIGHT ) ) //is RIGHT button pressed?
	{
		camera->move( Vector3( 0, -mouse_delta.y * 0.1f, 0 ) );
		camera->move( Vector3( mouse_delta.x * 0.1f, 0, 0 ) );
	}

	//async input to move the camera around
	if( keystate[ SDL_SCANCODE_LSHIFT ] ) speed *= 10; //move faster with left shift
	if( keystate[ SDL_SCANCODE_W ] || keystate[ SDL_SCANCODE_UP ] ) camera->move( Vector3( 0, 0, 1 ) * speed );
	if( keystate[ SDL_SCANCODE_S ] || keystate[ SDL_SCANCODE_DOWN ] ) camera->move( Vector3( 0, 0, -1 ) * speed );
	if( keystate[ SDL_SCANCODE_A ] || keystate[ SDL_SCANCODE_LEFT ] ) camera->move( Vector3( 1, 0, 0 ) * speed );
	if( keystate[ SDL_SCANCODE_D ] || keystate[ SDL_SCANCODE_RIGHT ] ) camera->move( Vector3( -1, 0, 0 ) * speed );
	//camera->move(Vector3(cos(time*3.14), sin(time*3.14), 0) * speed);
	// EXERCISE PRACT 1
}

//Keyboard event handler (sync input)
void Application::onKeyPressed( SDL_KeyboardEvent event )
{
	switch( event.keysym.sym )
	{
		case SDLK_ESCAPE: exit( 0 ); //ESC key, kill the app

		// EXERCISE PRACT 2
		case SDLK_PLUS:
		case SDLK_KP_PLUS:
			++aseIndex %= (int)aseFiles.size();
			break;
		case SDLK_MINUS:
		case SDLK_KP_MINUS:
			aseIndex = ( --aseIndex < 0 ) ? (int)aseFiles.size() - 1 : aseIndex;
			break;
			// EXERCISE PRACT 2
	}

	// EXERCISE PRACT 2
	switch( event.keysym.scancode )
	{
		case SDL_SCANCODE_P:
			poligonMode = PoligonMode::POINT;
			break;

		case SDL_SCANCODE_L:
			poligonMode = PoligonMode::LINE;
			break;

		case SDL_SCANCODE_F:
			poligonMode = PoligonMode::FILL;
			break;
		
	}
	// EXERCISE PRACT 2
	switch (event.keysym.sym)
	{
	case SDLK_1: flag = 1; break;

	case SDLK_2: flag = 2; break;

	case SDLK_3: flag = 3; break;

	case SDLK_4: flag = 4; break;

	case SDLK_5: flag = 5; break;

	case SDLK_6: flag = 6; break;

	case SDLK_7: flag = 7; break;

	case SDLK_8: flag = 8; break;

	case SDLK_z: speed_car = speed_car - 1; break;

	case SDLK_x: speed_car = speed_car + 1; break;
	}
	
}


void Application::onMouseButton( SDL_MouseButtonEvent event )
{

}

void Application::setWindowSize( int width, int height )
{
	
	glViewport( 0, 0, width, height );
	window_width = width;
	window_height = height;
	camera->aspect = width / (float)height;
	
}


/*  by Javi Agenjo 2013 UPF  javi.agenjo@gmail.com
	The Mesh contains the info about how to render a mesh and also how to parse it from a file.
*/

#ifndef MESH_H
#define MESH_H

#include <vector>
#include "Maths.h"
#include "utils.h"
#include "extra/TextParser.h"
using namespace std;

class Shader;

class Mesh
{
public:
	std::vector< Vector3 > vertices; //here we store the vertices
	std::vector< Vector3 > normals;	 //here we store the normals
	std::vector< Vector2 > uvs;	 //here we store the texture coordinates
	std::vector< Vector4 > colors;	 //here we store colors by vertex
	Vector3 centroid;
	unsigned int vertices_vbo_id;
	unsigned int normals_vbo_id;
	unsigned int uvs_vbo_id;
	unsigned int colors_vbo_id;

	Mesh();
	Mesh( const Mesh& m );
	Mesh(const std::string & fileName, float scale);
	~Mesh();

	void clear();
	void render( int primitive );

	void createPlane( float size );
	void createQuad( float center_x, float center_y, float w, float h, bool flip_uvs = false );
	void loadASE(const std::string& fileName, float scale);
	void Mesh::allocateARBBuffers(void);
};

#endif
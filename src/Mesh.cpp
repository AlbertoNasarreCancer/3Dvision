#include "Mesh.h"
#include <cassert>
#include "includes.h"
#include <vector>
#include "Maths.h"
#include "utils.h"
#include "extra/TextParser.h"

Mesh::Mesh()
{
	vertices_vbo_id = 0;
	normals_vbo_id = 0;
	uvs_vbo_id = 0;
	colors_vbo_id = 0;
}

Mesh::Mesh( const Mesh& m )
{
	vertices = m.vertices;
	normals = m.normals;
	uvs = m.uvs;
	colors = m.colors;
}

Mesh::Mesh(const std::string & fileName, float scale) :Mesh() //creamos este nuevo constructor pero no lo utilizamos.
{
	vertices_vbo_id = 0;
	normals_vbo_id = 0;
	uvs_vbo_id = 0;
	colors_vbo_id = 0;
	loadASE(fileName, scale);
}

Mesh::~Mesh()
{
	if( vertices_vbo_id ) glDeleteBuffersARB( 1, &vertices_vbo_id );
	if( normals_vbo_id ) glDeleteBuffersARB( 1, &normals_vbo_id );
	if( uvs_vbo_id ) glDeleteBuffersARB( 1, &uvs_vbo_id );
	if( colors_vbo_id ) glDeleteBuffersARB( 1, &colors_vbo_id );
}

void Mesh::clear()
{
	vertices.clear();
	normals.clear();
	uvs.clear();
	colors.clear();
}


void Mesh::render( int primitive )
{
	assert( vertices.size() && "No vertices in this mesh" );

	glEnableClientState( GL_VERTEX_ARRAY );

	if( vertices_vbo_id )
	{
		glBindBufferARB( GL_ARRAY_BUFFER_ARB, vertices_vbo_id );
		glVertexPointer( 3, GL_FLOAT, 0, NULL );
	}
	else
		glVertexPointer( 3, GL_FLOAT, 0, &vertices[ 0 ] );

	if( normals.size() )
	{
		glEnableClientState( GL_NORMAL_ARRAY );
		if( normals_vbo_id )
		{
			glBindBufferARB( GL_ARRAY_BUFFER_ARB, normals_vbo_id );
			glNormalPointer( GL_FLOAT, 0, NULL );
		}
		else
			glNormalPointer( GL_FLOAT, 0, &normals[ 0 ] );
	}

	if( uvs.size() )
	{
		glEnableClientState( GL_TEXTURE_COORD_ARRAY );
		if( uvs_vbo_id )
		{
			glBindBufferARB( GL_ARRAY_BUFFER_ARB, uvs_vbo_id );
			glTexCoordPointer( 2, GL_FLOAT, 0, NULL );
		}
		else
			glTexCoordPointer( 2, GL_FLOAT, 0, &uvs[ 0 ] );
	}

	if( colors.size() )
	{
		glEnableClientState( GL_COLOR_ARRAY );
		if( colors_vbo_id )
		{
			glBindBufferARB( GL_ARRAY_BUFFER_ARB, colors_vbo_id );
			glColorPointer( 4, GL_FLOAT, 0, NULL );
		}
		else
			glColorPointer( 4, GL_FLOAT, 0, &colors[ 0 ] );
	}

	glDrawArrays( primitive, 0, (GLsizei)vertices.size() );
	glDisableClientState( GL_VERTEX_ARRAY );

	if( normals.size() )
		glDisableClientState( GL_NORMAL_ARRAY );
	if( uvs.size() )
		glDisableClientState( GL_TEXTURE_COORD_ARRAY );
	if( colors.size() )
		glDisableClientState( GL_COLOR_ARRAY );

}

void Mesh::createQuad( float center_x, float center_y, float w, float h, bool flip_uvs )
{
	vertices.clear();
	normals.clear();
	uvs.clear();
	colors.clear();

	//create six vertices (3 for upperleft triangle and 3 for lowerright)

	vertices.push_back( Vector3( center_x + w*0.5f, center_y + h*0.5f, 0.0f ) );
	vertices.push_back( Vector3( center_x - w*0.5f, center_y - h*0.5f, 0.0f ) );
	vertices.push_back( Vector3( center_x + w*0.5f, center_y - h*0.5f, 0.0f ) );
	vertices.push_back( Vector3( center_x - w*0.5f, center_y + h*0.5f, 0.0f ) );
	vertices.push_back( Vector3( center_x - w*0.5f, center_y - h*0.5f, 0.0f ) );
	vertices.push_back( Vector3( center_x + w*0.5f, center_y + h*0.5f, 0.0f ) );

	//texture coordinates
	uvs.push_back( Vector2( 1.0f, flip_uvs ? 0.0f : 1.0f ) );
	uvs.push_back( Vector2( 0.0f, flip_uvs ? 1.0f : 0.0f ) );
	uvs.push_back( Vector2( 1.0f, flip_uvs ? 1.0f : 0.0f ) );
	uvs.push_back( Vector2( 0.0f, flip_uvs ? 0.0f : 1.0f ) );
	uvs.push_back( Vector2( 0.0f, flip_uvs ? 1.0f : 0.0f ) );
	uvs.push_back( Vector2( 1.0f, flip_uvs ? 0.0f : 1.0f ) );

	//all of them have the same normal
	normals.push_back( Vector3( 0.0f, 0.0f, 1.0f ) );
	normals.push_back( Vector3( 0.0f, 0.0f, 1.0f ) );
	normals.push_back( Vector3( 0.0f, 0.0f, 1.0f ) );
	normals.push_back( Vector3( 0.0f, 0.0f, 1.0f ) );
	normals.push_back( Vector3( 0.0f, 0.0f, 1.0f ) );
	normals.push_back( Vector3( 0.0f, 0.0f, 1.0f ) );
}



void Mesh::createPlane(float size)
{
	vertices.clear();
	normals.clear();
	uvs.clear();
	colors.clear();

	//create six vertices (3 for upperleft triangle and 3 for lowerright)
	//arriba
	vertices.push_back(Vector3(size, size, size));
	vertices.push_back(Vector3(size, size, -size));
	vertices.push_back(Vector3(-size, size, -size));
	vertices.push_back(Vector3(-size, size, size));
	vertices.push_back(Vector3(size, size, size));
	vertices.push_back(Vector3(-size, size, -size));
	//abajo
	vertices.push_back(Vector3(size, -size, size));
	vertices.push_back(Vector3(-size, -size, size));
	vertices.push_back(Vector3(-size, -size, -size));
	vertices.push_back(Vector3(-size, -size, -size));
	vertices.push_back(Vector3(size, -size, -size));
	vertices.push_back(Vector3(size, -size, size));
	//esquerra
	vertices.push_back(Vector3(-size, -size, size));
	vertices.push_back(Vector3(-size, size, size));
	vertices.push_back(Vector3(-size, -size, -size));
	vertices.push_back(Vector3(-size, -size, -size));
	vertices.push_back(Vector3(-size, size, size));
	vertices.push_back(Vector3(-size, size, -size));
	//dreta
	vertices.push_back(Vector3(size, -size, size));
	vertices.push_back(Vector3(size, -size, -size));
	vertices.push_back(Vector3(size, size, size));
	vertices.push_back(Vector3(size, size, size));
	vertices.push_back(Vector3(size, -size, -size));
	vertices.push_back(Vector3(size, size, -size));
	//cara
	vertices.push_back(Vector3(size, size, size));
	vertices.push_back(Vector3(-size, -size, size));
	vertices.push_back(Vector3(size, -size, size));
	vertices.push_back(Vector3(-size, -size, size));
	vertices.push_back(Vector3(size, size, size));
	vertices.push_back(Vector3(-size, size, size));
	//culo
	vertices.push_back(Vector3(-size, -size, -size));
	vertices.push_back(Vector3(-size, size, -size));
	vertices.push_back(Vector3(size, -size, -size));
	vertices.push_back(Vector3(size, -size, -size));
	vertices.push_back(Vector3(-size, size, -size));
	vertices.push_back(Vector3(size, size, -size));

	//all of them have the same normal
	normals.push_back(Vector3(0, 1, 0));
	normals.push_back(Vector3(0, 1, 0));
	normals.push_back(Vector3(0, 1, 0));
	normals.push_back(Vector3(0, 1, 0));
	normals.push_back(Vector3(0, 1, 0));
	normals.push_back(Vector3(0, 1, 0));

	normals.push_back(Vector3(0, 1, 0));
	normals.push_back(Vector3(0, 1, 0));
	normals.push_back(Vector3(0, 1, 0));
	normals.push_back(Vector3(0, 1, 0));
	normals.push_back(Vector3(0, 1, 0));
	normals.push_back(Vector3(0, 1, 0));

	normals.push_back(Vector3(0, 1, 0));
	normals.push_back(Vector3(0, 1, 0));
	normals.push_back(Vector3(0, 1, 0));
	normals.push_back(Vector3(0, 1, 0));
	normals.push_back(Vector3(0, 1, 0));
	normals.push_back(Vector3(0, 1, 0));

	normals.push_back(Vector3(0, 1, 0));
	normals.push_back(Vector3(0, 1, 0));
	normals.push_back(Vector3(0, 1, 0));
	normals.push_back(Vector3(0, 1, 0));
	normals.push_back(Vector3(0, 1, 0));
	normals.push_back(Vector3(0, 1, 0));
	normals.push_back(Vector3(0, 1, 0));
	normals.push_back(Vector3(0, 1, 0));
	normals.push_back(Vector3(0, 1, 0));
	normals.push_back(Vector3(0, 1, 0));
	normals.push_back(Vector3(0, 1, 0));
	normals.push_back(Vector3(0, 1, 0));

	normals.push_back(Vector3(0, 1, 0));
	normals.push_back(Vector3(0, 1, 0));
	normals.push_back(Vector3(0, 1, 0));
	normals.push_back(Vector3(0, 1, 0));
	normals.push_back(Vector3(0, 1, 0));
	normals.push_back(Vector3(0, 1, 0));

	//texture coordinates
	uvs.push_back(Vector2(1, 1));
	uvs.push_back(Vector2(1, 0));
	uvs.push_back(Vector2(0, 0));
	uvs.push_back(Vector2(0, 1));
	uvs.push_back(Vector2(1, 1));
	uvs.push_back(Vector2(0, 0));

	uvs.push_back(Vector2(1, 1));
	uvs.push_back(Vector2(1, 0));
	uvs.push_back(Vector2(0, 0));
	uvs.push_back(Vector2(0, 1));
	uvs.push_back(Vector2(1, 1));
	uvs.push_back(Vector2(0, 0));

	uvs.push_back(Vector2(1, 1));
	uvs.push_back(Vector2(1, 0));
	uvs.push_back(Vector2(0, 0));
	uvs.push_back(Vector2(0, 1));
	uvs.push_back(Vector2(1, 1));
	uvs.push_back(Vector2(0, 0));

	uvs.push_back(Vector2(1, 1));
	uvs.push_back(Vector2(1, 0));
	uvs.push_back(Vector2(0, 0));
	uvs.push_back(Vector2(0, 1));
	uvs.push_back(Vector2(1, 1));
	uvs.push_back(Vector2(0, 0));

	uvs.push_back(Vector2(1, 1));
	uvs.push_back(Vector2(1, 0));
	uvs.push_back(Vector2(0, 0));
	uvs.push_back(Vector2(0, 1));
	uvs.push_back(Vector2(1, 1));
	uvs.push_back(Vector2(0, 0));

	uvs.push_back(Vector2(1, 1));
	uvs.push_back(Vector2(1, 0));
	uvs.push_back(Vector2(0, 0));
	uvs.push_back(Vector2(0, 1));
	uvs.push_back(Vector2(1, 1));
	uvs.push_back(Vector2(0, 0));

	//Color
	colors.push_back(Vector4(1, 0.5, 0, 0));
	colors.push_back(Vector4(1, 0, 0, 0));
	colors.push_back(Vector4(1, 1, 0, 0));
	colors.push_back(Vector4(1, 1, 1, 1));
	colors.push_back(Vector4(1, 0.5, 0, 1));
	colors.push_back(Vector4(1, 1, 0, 1));

	colors.push_back(Vector4(1, 0.5, 0.5, 1));
	colors.push_back(Vector4(1, 0.5, 0.5, 1));
	colors.push_back(Vector4(1, 0, 0.5, 0.5));
	colors.push_back(Vector4(1, 1, 1.5, 1));
	colors.push_back(Vector4(1, 1, 1, 1));
	colors.push_back(Vector4(1, 1, 1, 1));

	colors.push_back(Vector4(0.5, 0.5, 1.5, 1));
	colors.push_back(Vector4(1, 1, 1, 1));
	colors.push_back(Vector4(0, 1, 1, 0));
	colors.push_back(Vector4(0, 1, 1, 1));
	colors.push_back(Vector4(1, 1, 1, 1));
	colors.push_back(Vector4(1, 1, 0, 1));

	colors.push_back(Vector4(0, 1, 1, 0));
	colors.push_back(Vector4(1, 1, 1, 1));
	colors.push_back(Vector4(1, 0.5, 0, 1));
	colors.push_back(Vector4(1, 0.5, 0, 1));
	colors.push_back(Vector4(1, 1, 1, 1));
	colors.push_back(Vector4(1, 0, 0, 1));

	colors.push_back(Vector4(1, 0.5, 0, 0));
	colors.push_back(Vector4(0, 0, 1, 1));
	colors.push_back(Vector4(0, 1, 1, 1));
	colors.push_back(Vector4(0, 0, 1, 1));
	colors.push_back(Vector4(1, 0.5, 1, 1));
	colors.push_back(Vector4(1, 1, 1, 0));

	colors.push_back(Vector4(1, 0.5, 0.5, 0));
	colors.push_back(Vector4(1, 0.5, 0, 0));
	colors.push_back(Vector4(1, 0.5, 0, 0.5));
	colors.push_back(Vector4(1.5, 1.5, 1, 1));
	colors.push_back(Vector4(1, 1, 1.5, 1.5));
	colors.push_back(Vector4(1, 1, 1, 1));
}

void Mesh::loadASE(const std::string & fileName, float scale)
{
	// EXERCISE PRACT 2
	
	TextParser t(AddCurrentPath(fileName).c_str());


	// NUM VERTEX
	t.seek("*MESH_NUMVERTEX");//usamos la funcion seek para encontrar en numero de vertex y caras en el archivo y con getinit los extraemos.
	size_t numVertex = (size_t)t.getint();
	// NUM FACES
	t.seek("*MESH_NUMFACES");
	size_t numFaces = (size_t)t.getint();

	// VERTEX
	vector<Vector3> v_dictionary; // creamos un vector de vertices sin repetir
	v_dictionary.resize(numVertex);	

	// EXERCISE PRACT 3
	float centroid_x = 0.f;
	float centroid_y = 0.f;
	float centroid_z = 0.f;
	// EXERCISE PRACT 3

	for (int i = 0; i < numVertex; i++) //para cada vertice guardamos su posicion en v_dictionary
	{
		t.seek("*MESH_VERTEX");//buscamos el siguiente MESH VERTEX
		size_t position = (size_t)t.getint(); 
		float x = scale * (float)t.getfloat();
		float y = scale * (float)t.getfloat();
		float z = scale * (float)t.getfloat();
		v_dictionary[i].x = x;
		v_dictionary[i].z = z;
		v_dictionary[i].y = y;	

		// EXERCISE PRACT 3
		centroid_x += x;
		centroid_y += y;
		centroid_z += z;
		// EXERCISE PRACT 3
	}

	// EXERCISE PRACT 3
	centroid_x /= float(numVertex);
	centroid_y /= float(numVertex);
	centroid_z /= float(numVertex);
	// EXERCISE PRACT 3

	centroid = Vector3(centroid_x, centroid_y, centroid_z);

	// FACES
	vertices.resize(numFaces * 3);//en el vector vertices guardamos todos los vertices en orden
	normals.resize(numFaces*3);
	for (int i = 0; i < (numFaces * 3); i++)//guardamos todos los vertices de cada triangulo
	{
		t.seek("*MESH_FACE");
		t.getword();
		t.getword();

		vertices[i] = v_dictionary[(int)t.getfloat()];
		t.getword();
		i++;
		vertices[i] = v_dictionary[(int)t.getfloat()];
		t.getword();
		i++;
		vertices[i] = v_dictionary[(int)t.getfloat()];
	}

	for (int i = 0; i < (numFaces * 3); i++)
	{
		/*normals.push_back(Vector3(1,0,0));*/
		colors.push_back(Vector4(1, 1, 0, 1));
		uvs.push_back(Vector2(1, 0));
	}

	// Loading texture coordinates
	std::vector<Vector2> vTextureVertices;

	t.seek("*MESH_NUMTVERTEX");
	size_t numTVertex = (size_t)t.getint();
	vTextureVertices.resize(numTVertex);

	// Loading Texture Vertex
	
	for (int i = 0; i < numTVertex ; i++)
	{
		t.seek("*MESH_TVERT");

		int num = t.getint(); // #

		vTextureVertices[i].x = (float)t.getfloat();
		vTextureVertices[i].y = (float)t.getfloat();

		
	}


	t.seek("*MESH_NUMTVFACES");
	size_t numTFaces = (size_t)t.getint();
	uvs.resize(numTFaces * 3);

	
	for (int i = 0; i < (numTFaces * 3); i++)
	{
		t.seek("*MESH_TFACE");
			
			

		int num = t.getint(); // #

		unsigned int A = (unsigned int)t.getint();
		unsigned int B = (unsigned int)t.getint();
		unsigned int C = (unsigned int)t.getint();

		uvs[i] = vTextureVertices[A];
		i++;
		uvs[i] = vTextureVertices[B];
		i++;
		uvs[i] = vTextureVertices[C];
	}


	for (int i = 0; i < (numFaces*3); i++)
	{
		t.seek("*MESH_VERTEXNORMAL");
		t.getword();
		

		normals[i].x = t.getfloat();
		
		
		normals[i].y = t.getfloat();
		
		
		normals[i].z = t.getfloat();

	}



	allocateARBBuffers();

	//FIN EXERCISE PRACT 2
}

void Mesh::allocateARBBuffers(void)
{
	glGenBuffersARB(1, &vertices_vbo_id);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, vertices_vbo_id);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, sizeof(Vector3) * vertices.size(), &vertices[0], GL_STATIC_DRAW_ARB);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);

	glGenBuffersARB(1, &normals_vbo_id);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, normals_vbo_id);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, sizeof(Vector3) * normals.size(), &normals[0], GL_STATIC_DRAW_ARB);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);

	glGenBuffersARB(1, &uvs_vbo_id);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, uvs_vbo_id);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, sizeof(Vector2) * uvs.size(), &uvs[0], GL_STATIC_DRAW_ARB);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);

	if (colors.size() > 0)
	{
		glGenBuffersARB(1, &colors_vbo_id);
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, colors_vbo_id);
		glBufferDataARB(GL_ARRAY_BUFFER_ARB, sizeof(Vector4) * colors.size(), &colors[0], GL_STATIC_DRAW_ARB);
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
	}
}

#include "Camera.h"

#include "includes.h"
#include <iostream>

Camera::Camera()
{
	view_matrix.setIdentity();
	setOrthographic( -100, 100, -100, 100, -100, 100 );
}

void Camera::set()
{
	updateViewMatrix();
	updateProjectionMatrix();

	glMatrixMode( GL_MODELVIEW );
	glLoadMatrixf( view_matrix.m );
	glMatrixMode( GL_PROJECTION );
	glLoadMatrixf( projection_matrix.m );
	glMatrixMode( GL_MODELVIEW );
}

void Camera::updateViewMatrix()
{
	if( type == PERSPECTIVE )
		view_matrix.lookAt( eye, center, up );
	else
		view_matrix.setIdentity();

	viewprojection_matrix = view_matrix * projection_matrix;

	extractFrustum();
}

// ******************************************

//Create a projection matrix
void Camera::updateProjectionMatrix()
{
	if( type == ORTHOGRAPHIC )
		projection_matrix.ortho( left, right, bottom, top, near_plane, far_plane );
	else
		projection_matrix.perspective( fov, aspect, near_plane, far_plane );

	viewprojection_matrix = view_matrix * projection_matrix;

	extractFrustum();
}

Vector3 Camera::getLocalVector( const Vector3& v )
{
	

	Matrix44 iView = view_matrix;
	iView.inverse();
	return iView.rotateVector(v);//multiplicamos por la matriz inversa para cambiar de cordenadas de mundo a cordenadas locales (de camara)
}

void Camera::move( Vector3 delta )
{
	
	Vector3 local_vector = getLocalVector(delta); //obtenemos cordenadas de camara 

	center = center + local_vector; //movemos el center en la dirección en la que se mueve el ratón.
	eye = eye + local_vector; //movemos el eye en la dirección en la que se mueve el ratón.
	
	updateViewMatrix(); // actualizamos la matriz con las nuevas posiciones
}

void Camera::rotate( float angle, const Vector3& axis )
{
	

	Matrix44 R;
	R.setRotation(angle, getLocalVector(axis));//obtenemos cordenadas de camara y rotamos la matriz con el angulo marcado por el ratón

	Vector3 front = (center - eye);    //calculamos el nuevo vector front
	Vector3 new_front = R * front; //calculamos el nuevo vector front aplicandole la matriz de rotación.

	center = eye + new_front; //desplazamos el center.

	updateViewMatrix();
	
}

void Camera::setOrthographic( float left, float right, float bottom, float top, float near_plane, float far_plane )
{
	type = ORTHOGRAPHIC;

	this->left = left;
	this->right = right;
	this->bottom = bottom;
	this->top = top;
	this->near_plane = near_plane;
	this->far_plane = far_plane;

	updateProjectionMatrix();
}

void Camera::setPerspective( float fov, float aspect, float near_plane, float far_plane )
{
	type = PERSPECTIVE;

	this->fov = fov;
	this->aspect = aspect;
	this->near_plane = near_plane;
	this->far_plane = far_plane;

	//update projection
	updateProjectionMatrix();
}

void Camera::lookAt( const Vector3& eye, const Vector3& center, const Vector3& up )
{
	this->eye = eye;
	this->center = center;
	this->up = up;

	updateViewMatrix();
}


void Camera::extractFrustum()
{
	float   proj[ 16 ];
	float   modl[ 16 ];
	float   clip[ 16 ];
	float   t;

	memcpy( proj, projection_matrix.m, sizeof( Matrix44 ) );
	memcpy( modl, view_matrix.m, sizeof( Matrix44 ) );

	/* Combine the two matrices (multiply projection by modelview) */
	clip[ 0 ] = modl[ 0 ] * proj[ 0 ] + modl[ 1 ] * proj[ 4 ] + modl[ 2 ] * proj[ 8 ] + modl[ 3 ] * proj[ 12 ];
	clip[ 1 ] = modl[ 0 ] * proj[ 1 ] + modl[ 1 ] * proj[ 5 ] + modl[ 2 ] * proj[ 9 ] + modl[ 3 ] * proj[ 13 ];
	clip[ 2 ] = modl[ 0 ] * proj[ 2 ] + modl[ 1 ] * proj[ 6 ] + modl[ 2 ] * proj[ 10 ] + modl[ 3 ] * proj[ 14 ];
	clip[ 3 ] = modl[ 0 ] * proj[ 3 ] + modl[ 1 ] * proj[ 7 ] + modl[ 2 ] * proj[ 11 ] + modl[ 3 ] * proj[ 15 ];

	clip[ 4 ] = modl[ 4 ] * proj[ 0 ] + modl[ 5 ] * proj[ 4 ] + modl[ 6 ] * proj[ 8 ] + modl[ 7 ] * proj[ 12 ];
	clip[ 5 ] = modl[ 4 ] * proj[ 1 ] + modl[ 5 ] * proj[ 5 ] + modl[ 6 ] * proj[ 9 ] + modl[ 7 ] * proj[ 13 ];
	clip[ 6 ] = modl[ 4 ] * proj[ 2 ] + modl[ 5 ] * proj[ 6 ] + modl[ 6 ] * proj[ 10 ] + modl[ 7 ] * proj[ 14 ];
	clip[ 7 ] = modl[ 4 ] * proj[ 3 ] + modl[ 5 ] * proj[ 7 ] + modl[ 6 ] * proj[ 11 ] + modl[ 7 ] * proj[ 15 ];

	clip[ 8 ] = modl[ 8 ] * proj[ 0 ] + modl[ 9 ] * proj[ 4 ] + modl[ 10 ] * proj[ 8 ] + modl[ 11 ] * proj[ 12 ];
	clip[ 9 ] = modl[ 8 ] * proj[ 1 ] + modl[ 9 ] * proj[ 5 ] + modl[ 10 ] * proj[ 9 ] + modl[ 11 ] * proj[ 13 ];
	clip[ 10 ] = modl[ 8 ] * proj[ 2 ] + modl[ 9 ] * proj[ 6 ] + modl[ 10 ] * proj[ 10 ] + modl[ 11 ] * proj[ 14 ];
	clip[ 11 ] = modl[ 8 ] * proj[ 3 ] + modl[ 9 ] * proj[ 7 ] + modl[ 10 ] * proj[ 11 ] + modl[ 11 ] * proj[ 15 ];

	clip[ 12 ] = modl[ 12 ] * proj[ 0 ] + modl[ 13 ] * proj[ 4 ] + modl[ 14 ] * proj[ 8 ] + modl[ 15 ] * proj[ 12 ];
	clip[ 13 ] = modl[ 12 ] * proj[ 1 ] + modl[ 13 ] * proj[ 5 ] + modl[ 14 ] * proj[ 9 ] + modl[ 15 ] * proj[ 13 ];
	clip[ 14 ] = modl[ 12 ] * proj[ 2 ] + modl[ 13 ] * proj[ 6 ] + modl[ 14 ] * proj[ 10 ] + modl[ 15 ] * proj[ 14 ];
	clip[ 15 ] = modl[ 12 ] * proj[ 3 ] + modl[ 13 ] * proj[ 7 ] + modl[ 14 ] * proj[ 11 ] + modl[ 15 ] * proj[ 15 ];

	/* Extract the numbers for the RIGHT plane */
	frustum[ 0 ][ 0 ] = clip[ 3 ] - clip[ 0 ];
	frustum[ 0 ][ 1 ] = clip[ 7 ] - clip[ 4 ];
	frustum[ 0 ][ 2 ] = clip[ 11 ] - clip[ 8 ];
	frustum[ 0 ][ 3 ] = clip[ 15 ] - clip[ 12 ];

	/* Normalize the result */
	t = sqrt( frustum[ 0 ][ 0 ] * frustum[ 0 ][ 0 ] + frustum[ 0 ][ 1 ] * frustum[ 0 ][ 1 ] + frustum[ 0 ][ 2 ] * frustum[ 0 ][ 2 ] );
	frustum[ 0 ][ 0 ] /= t;
	frustum[ 0 ][ 1 ] /= t;
	frustum[ 0 ][ 2 ] /= t;
	frustum[ 0 ][ 3 ] /= t;

	/* Extract the numbers for the LEFT plane */
	frustum[ 1 ][ 0 ] = clip[ 3 ] + clip[ 0 ];
	frustum[ 1 ][ 1 ] = clip[ 7 ] + clip[ 4 ];
	frustum[ 1 ][ 2 ] = clip[ 11 ] + clip[ 8 ];
	frustum[ 1 ][ 3 ] = clip[ 15 ] + clip[ 12 ];

	/* Normalize the result */
	t = sqrt( frustum[ 1 ][ 0 ] * frustum[ 1 ][ 0 ] + frustum[ 1 ][ 1 ] * frustum[ 1 ][ 1 ] + frustum[ 1 ][ 2 ] * frustum[ 1 ][ 2 ] );
	frustum[ 1 ][ 0 ] /= t;
	frustum[ 1 ][ 1 ] /= t;
	frustum[ 1 ][ 2 ] /= t;
	frustum[ 1 ][ 3 ] /= t;

	/* Extract the BOTTOM plane */
	frustum[ 2 ][ 0 ] = clip[ 3 ] + clip[ 1 ];
	frustum[ 2 ][ 1 ] = clip[ 7 ] + clip[ 5 ];
	frustum[ 2 ][ 2 ] = clip[ 11 ] + clip[ 9 ];
	frustum[ 2 ][ 3 ] = clip[ 15 ] + clip[ 13 ];

	/* Normalize the result */
	t = sqrt( frustum[ 2 ][ 0 ] * frustum[ 2 ][ 0 ] + frustum[ 2 ][ 1 ] * frustum[ 2 ][ 1 ] + frustum[ 2 ][ 2 ] * frustum[ 2 ][ 2 ] );
	frustum[ 2 ][ 0 ] /= t;
	frustum[ 2 ][ 1 ] /= t;
	frustum[ 2 ][ 2 ] /= t;
	frustum[ 2 ][ 3 ] /= t;

	/* Extract the TOP plane */
	frustum[ 3 ][ 0 ] = clip[ 3 ] - clip[ 1 ];
	frustum[ 3 ][ 1 ] = clip[ 7 ] - clip[ 5 ];
	frustum[ 3 ][ 2 ] = clip[ 11 ] - clip[ 9 ];
	frustum[ 3 ][ 3 ] = clip[ 15 ] - clip[ 13 ];

	/* Normalize the result */
	t = sqrt( frustum[ 3 ][ 0 ] * frustum[ 3 ][ 0 ] + frustum[ 3 ][ 1 ] * frustum[ 3 ][ 1 ] + frustum[ 3 ][ 2 ] * frustum[ 3 ][ 2 ] );
	frustum[ 3 ][ 0 ] /= t;
	frustum[ 3 ][ 1 ] /= t;
	frustum[ 3 ][ 2 ] /= t;
	frustum[ 3 ][ 3 ] /= t;

	/* Extract the FAR plane */
	frustum[ 4 ][ 0 ] = clip[ 3 ] - clip[ 2 ];
	frustum[ 4 ][ 1 ] = clip[ 7 ] - clip[ 6 ];
	frustum[ 4 ][ 2 ] = clip[ 11 ] - clip[ 10 ];
	frustum[ 4 ][ 3 ] = clip[ 15 ] - clip[ 14 ];

	/* Normalize the result */
	t = sqrt( frustum[ 4 ][ 0 ] * frustum[ 4 ][ 0 ] + frustum[ 4 ][ 1 ] * frustum[ 4 ][ 1 ] + frustum[ 4 ][ 2 ] * frustum[ 4 ][ 2 ] );
	frustum[ 4 ][ 0 ] /= t;
	frustum[ 4 ][ 1 ] /= t;
	frustum[ 4 ][ 2 ] /= t;
	frustum[ 4 ][ 3 ] /= t;

	/* Extract the NEAR plane */
	frustum[ 5 ][ 0 ] = clip[ 3 ] + clip[ 2 ];
	frustum[ 5 ][ 1 ] = clip[ 7 ] + clip[ 6 ];
	frustum[ 5 ][ 2 ] = clip[ 11 ] + clip[ 10 ];
	frustum[ 5 ][ 3 ] = clip[ 15 ] + clip[ 14 ];

	/* Normalize the result */
	t = sqrt( frustum[ 5 ][ 0 ] * frustum[ 5 ][ 0 ] + frustum[ 5 ][ 1 ] * frustum[ 5 ][ 1 ] + frustum[ 5 ][ 2 ] * frustum[ 5 ][ 2 ] );
	frustum[ 5 ][ 0 ] /= t;
	frustum[ 5 ][ 1 ] /= t;
	frustum[ 5 ][ 2 ] /= t;
	frustum[ 5 ][ 3 ] /= t;
}

bool Camera::testPointInFrustum( Vector3 v )
{
	int p;

	for( p = 0; p < 6; p++ )
		if( frustum[ p ][ 0 ] * v.x + frustum[ p ][ 1 ] * v.y + frustum[ p ][ 2 ] * v.z + frustum[ p ][ 3 ] <= 0 )
			return false;
	return true;
}



bool Camera::testSphereInFrustum( Vector3 v, float radius )
{
	int p;

	for( p = 0; p < 6; p++ )
		if( frustum[ p ][ 0 ] * v.x + frustum[ p ][ 1 ] * v.y + frustum[ p ][ 2 ] * v.z + frustum[ p ][ 3 ] <= -radius )
			return false;
	return true;
}


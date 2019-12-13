#include "Texture.h"
#include "extra/ImageLoader.h"
#include "Utils.h"
#include <memory>

Texture::Texture( void )
    : m_GLTexture( -1 )
{

}

Texture::Texture( const std::string& p_oFileName )
	: m_GLTexture( -1 )
{
	load( p_oFileName );
}

Texture::~Texture( void )
{
    releaseTexture();
}

void Texture::releaseTexture( void )
{
    if( m_GLTexture > -1 )
    {
        glDeleteTextures( 1, &m_GLTexture );
        glFinish();
    }
}

void Texture::load( const std::string& p_oFileName )
{
	std::unique_ptr<Image> pImage(loadBMP(AddCurrentPath(p_oFileName).c_str()));
	// EXERCISE PRACT 3
	releaseTexture();
	//We bind the texture to the pointer that we have in the class :
		glGenTextures(1, &m_GLTexture);
	glBindTexture(GL_TEXTURE_2D, m_GLTexture);
	//We set the parameter to interpolate if the area is bigger than the texture :
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//We set the parameter to interpolate if the area is smaller than the texture :
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//We set the wrap parameter for texture coordinate s :
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	//We set the wrap parameter for texture coordinate t :
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	//We specify a two dimensional texture image :
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, pImage->width, pImage->height, 0, GL_RGB, GL_UNSIGNED_BYTE, pImage->pixels);
	//And finally we bind the texture
		glBindTexture(GL_TEXTURE_2D, 0);

	// EXERCISE PRACT 3
}


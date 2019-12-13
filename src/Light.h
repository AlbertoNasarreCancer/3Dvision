#include <iostream>
#include "Maths.h"
#include "Shader.h"

class Light {

	std::string m_sLightName; // Light Id used in the shader
	Vector3 m_v3Position; // Light position in eye coords.
	Vector3 m_v3La; // Ambient light intensity
	Vector3 m_v3Ld; // Diffuse light intensity
	Vector3 m_v3Ls; // Specular light intensity


public:
	Light(std::string m_sLightName, Vector3 m_v3Position, Vector3 m_v3La, Vector3 m_v3Ld, Vector3 m_v3Ls);
	void bind(Shader& shader) const;
};




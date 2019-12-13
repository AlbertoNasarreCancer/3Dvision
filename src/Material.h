#include "includes.h"
#include "Camera.h"
#include "Mesh.h"
#include "Texture.h"
#include "Shader.h"
#include <memory>

class Material
{
	std::string m_sMaterialName;
	Vector3 m_v3Ka; // Ambient reflectivity
	Vector3 m_v3Kd; // Diffuse reflectivity
	Vector3 m_v3Ks; // Specular reflectivity
	float m_fShininess; // Specular shininess factor

public:

	Material(std::string m_sMaterialName, Vector3 m_v3Ka, Vector3 m_v3Kd, Vector3 m_v3Ks, float m_fShininess);
	void bind(Shader& shader) const;
};
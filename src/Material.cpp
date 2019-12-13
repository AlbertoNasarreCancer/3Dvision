#include "material.h"
#include "Maths.h"
using namespace std;

Material::Material(string m_sMaterialName, Vector3 m_v3Ka, Vector3 m_v3Kd, Vector3 m_v3Ks, float m_fShininess)
{
	this->m_sMaterialName = m_sMaterialName;
	this->m_v3Ka = m_v3Ka;
	this->m_v3Kd = m_v3Kd;
	this->m_v3Ks = m_v3Ks;
	this->m_fShininess = m_fShininess;
};

void Material::bind(Shader& shader) const
{
	shader.setVector3(m_sMaterialName + ".Ka", m_v3Ka);
	shader.setVector3(m_sMaterialName + ".Kd", m_v3Kd);
	shader.setVector3(m_sMaterialName + ".Ks", m_v3Ks);
	shader.setFloat(m_sMaterialName + ".Shininess", m_fShininess);
}
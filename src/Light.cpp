#include "light.h"
#include "Maths.h"
using namespace std;


Light::Light(string m_sLightName, Vector3 m_v3Position, Vector3 m_v3La, Vector3 m_v3Ld, Vector3 m_v3Ls)
{
	this->m_sLightName = m_sLightName;
	this->m_v3Position = m_v3Position;
	this->m_v3La = m_v3La;
	this->m_v3Ld = m_v3Ld;
	this->m_v3Ls = m_v3Ls;
}

void Light::bind(Shader& shader) const
{
	shader.setVector3(m_sLightName + ".Position", m_v3Position);
	shader.setVector3(m_sLightName + ".La", m_v3La);
	shader.setVector3(m_sLightName + ".Ld", m_v3Ld);
	shader.setVector3(m_sLightName + ".Ls", m_v3Ls);
}
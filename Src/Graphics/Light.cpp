#include "Light.h"

Light::Light()
{
}

Light::Light(const Light& other)
{
}

Light::~Light()
{
}

void Light::SetDiffuseColor(float red, float green, float blue, float alpha)
{
	m_diffuseColor = DirectX::XMFLOAT4(red, green, blue, alpha);
}

void Light::SetDirection(float x, float y, float z)
{
	m_direction = DirectX::XMFLOAT3(x, y, z);
}

DirectX::XMFLOAT4 Light::GetDiffuseColor()
{
	return m_diffuseColor;
}

DirectX::XMFLOAT3 Light::GetDirection()
{
	return m_direction;
}
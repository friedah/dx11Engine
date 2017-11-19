#include "Light.h"



Light::Light(const DirectX::XMFLOAT4 &color, const DirectX::XMFLOAT3 &direction):diffuseColor(color), lightDirection(direction)
{
}


Light::~Light()
{
}
DirectX::XMFLOAT4 Light::diffuse_color() const
{
	return diffuseColor;
}

DirectX::XMFLOAT3 Light::light_direction() const
{
	return lightDirection;
}
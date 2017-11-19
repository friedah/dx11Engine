#pragma once
#include <DirectXMath.h>

class Light
{
	DirectX::XMFLOAT4 diffuseColor;
	DirectX::XMFLOAT3 lightDirection;
public:
	Light(const DirectX::XMFLOAT4 &color, const DirectX::XMFLOAT3 &direction);
	~Light();

	DirectX::XMFLOAT4 diffuse_color() const;

	DirectX::XMFLOAT3 light_direction() const;
};


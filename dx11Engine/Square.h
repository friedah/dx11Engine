#pragma once
#include "Model.h"
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXCollision.h>
class Square:public Model
{
	struct Vertex
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT3 normal;
	};
	static const int vertexnum = 8, indexnum = 36;
	static const DirectX::XMFLOAT3 vertexs[vertexnum];
	static const uint16_t indexs[indexnum];
	static const int InputElementCount = 2;
	static const D3D11_INPUT_ELEMENT_DESC InputElements[InputElementCount];
	Vertex m_vertexs[vertexnum];
public:
	Square();
	virtual ~Square();
	virtual void Release() override;
	void GeneralVertexData();
	virtual bool CreateModel(ID3D11Device* d3dDevice, ID3D11DeviceContext* deviceContext, void const* shaderByteCode, size_t byteCodeLength) override;
	virtual bool Draw(ID3D11DeviceContext* deviceContext) override;
};


#pragma once
#include <d3d11_1.h>
#include <wrl\client.h>
#include <stdint.h>
#include <memory>
#include <vector>
#include <directxmath.h>


class Model
{
protected:
	uint32_t                                                indexCount = 0;
	uint32_t                                                startIndex = 0;
	uint32_t                                                vertexCount = 0;
	uint32_t                                                vertexOffset = 0;
	uint32_t                                                vertexStride = 0;
	D3D_PRIMITIVE_TOPOLOGY                                  primitiveType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	DXGI_FORMAT                                             indexFormat = DXGI_FORMAT_R16_UINT;
	Microsoft::WRL::ComPtr<ID3D11Buffer>                    indexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>                    vertexBuffer;
	bool m_init;

	DirectX::XMMATRIX m_worldMatrix;
	DirectX::XMFLOAT3 m_position;
	DirectX::XMFLOAT3 m_scale;
	DirectX::XMFLOAT3 m_rotation;

public:
	Model();
	virtual ~Model();
	virtual void Release() {};
	virtual bool CreateModel(ID3D11Device* d3dDevice) = 0;
	bool CreateDataBuffer(ID3D11Device* d3dDevice, const size_t &vbsize, const void *vbData, const size_t &ibsize, const void *ibData);
	virtual bool Draw(ID3D11DeviceContext* deviceContext) ;
	void GetWorldMatrix(DirectX::XMMATRIX&);
	void SetPosition(const float &x, const float &y, const float &z);
	void SetScale(const float &x, const float &y, const float &z);
	void SetRotate(const float &x, const float &y, const float &z);
};


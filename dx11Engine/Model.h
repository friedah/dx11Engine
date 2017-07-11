#pragma once
#include <d3d11_1.h>
#include <wrl\client.h>
#include <stdint.h>
#include <memory>
#include <vector>
class Model
{
protected:
	uint32_t                                                indexCount;
	uint32_t                                                startIndex;
	uint32_t                                                vertexOffset;
	uint32_t                                                vertexStride;
	D3D_PRIMITIVE_TOPOLOGY                                  primitiveType;
	DXGI_FORMAT                                             indexFormat;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>               inputLayout;
	Microsoft::WRL::ComPtr<ID3D11Buffer>                    indexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>                    vertexBuffer;
	std::shared_ptr<std::vector<D3D11_INPUT_ELEMENT_DESC>>  vbDecl;
	bool m_init;
public:
	Model();
	virtual ~Model();
	virtual void Release() = 0;
	virtual bool CreateModel(ID3D11Device* d3dDevice, ID3D11DeviceContext* deviceContext, void const* shaderByteCode, size_t byteCodeLength) = 0;
	virtual bool Draw(ID3D11DeviceContext* deviceContext) = 0;
};


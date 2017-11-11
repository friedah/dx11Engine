#include "Model.h"
#include "BasicHelper.h"



Model::Model():m_init(false)
{
}


Model::~Model()
{
}
bool Model::Draw(ID3D11DeviceContext* deviceContext)
{
	FAILEDRETURN(m_init);
	deviceContext->IASetPrimitiveTopology(primitiveType);
	deviceContext->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &vertexStride, &vertexOffset);
	if (indexBuffer.Get())
	{
		deviceContext->IASetIndexBuffer(indexBuffer.Get(), indexFormat, 0);
		deviceContext->DrawIndexed(indexCount, startIndex, vertexOffset);
	}
	else
		deviceContext->Draw(vertexCount, vertexOffset);
	return true;
}

bool Model::CreateDataBuffer(ID3D11Device* d3dDevice, const size_t &vbsize, const void *vbData, const size_t &ibsize, const void *ibData)
{

	Microsoft::WRL::ComPtr<ID3D11Buffer> vb;
	if(vbData)
	{
		D3D11_BUFFER_DESC desc = { 0 };
		desc.ByteWidth = static_cast<UINT>(vbsize);
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.MiscFlags = 0;
		desc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA vertexData = { 0 };
		vertexData.pSysMem = vbData;
		vertexData.SysMemPitch = 0;
		vertexData.SysMemSlicePitch = 0;
		FAILEDRETURN(d3dDevice->CreateBuffer(&desc, &vertexData, vb.GetAddressOf()));
	}

	Microsoft::WRL::ComPtr<ID3D11Buffer> ib;
	if (ibData)
	{
		D3D11_BUFFER_DESC desc = { 0 };
		desc.ByteWidth = static_cast<UINT>(ibsize);
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.MiscFlags = 0;
		desc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA indexData = { 0 };
		indexData.pSysMem = ibData;
		indexData.SysMemPitch = 0;
		indexData.SysMemSlicePitch = 0;
		FAILEDRETURN(d3dDevice->CreateBuffer(&desc, &indexData, ib.GetAddressOf()));
	}

	indexBuffer = ib;
	vertexBuffer = vb;
	return true;
}
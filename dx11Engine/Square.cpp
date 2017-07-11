#include "Square.h"
#include "BasicHelper.h"
const DirectX::XMFLOAT3 Square::vertexs[Square::vertexnum] =
{
	DirectX::XMFLOAT3(-1.0f, -1.0f, -1.0f),
	DirectX::XMFLOAT3(-1.0f, +1.0f, -1.0f),
	DirectX::XMFLOAT3(+1.0f, +1.0f, -1.0f),
	DirectX::XMFLOAT3(+1.0f, -1.0f, -1.0f),
	DirectX::XMFLOAT3(-1.0f, -1.0f, +1.0f),
	DirectX::XMFLOAT3(-1.0f, +1.0f, +1.0f),
	DirectX::XMFLOAT3(+1.0f, +1.0f, +1.0f),
	DirectX::XMFLOAT3(+1.0f, -1.0f, +1.0f),
};
const uint16_t Square::indexs[Square::indexnum] =
{
	// front face
	0, 1, 2,
	0, 2, 3,

	// back face
	4, 6, 5,
	4, 7, 6,

	// left face
	4, 5, 1,
	4, 1, 0,

	// right face
	3, 2, 6,
	3, 6, 7,

	// top face
	1, 5, 6,
	1, 6, 2,

	// bottom face
	4, 0, 3,
	4, 3, 7
};
const D3D11_INPUT_ELEMENT_DESC Square::InputElements[] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};
Square::Square()
{
	indexCount = indexnum;
	startIndex = 0;
	vertexStride = sizeof(Vertex);
	primitiveType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	vertexOffset = 0;
	indexFormat = DXGI_FORMAT_R16_UINT;
	vbDecl = std::make_shared<std::vector<D3D11_INPUT_ELEMENT_DESC>>(InputElements, InputElements + InputElementCount);
}


Square::~Square()
{
}
void Square::Release()
{
}
void Square::GeneralVertexData()
{
	DirectX::XMVECTOR vertexnormal[vertexnum];
	for (size_t i = 0; i < vertexnum; i++)
	{
		m_vertexs[i].pos = vertexs[i];
		m_vertexs[i].normal = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
		//vertexnormal[i] = DirectX::XMVectorZero();
	}
	//for (size_t i = 0; i < indexnum; i+= 3)
	//{
	//	Vertex v0 = m_vertexs[indexs[i]], v1 = m_vertexs[indexs[i + 1]], v2 = m_vertexs[indexs[i + 2]];
	//	DirectX::XMVECTOR e0 = DirectX::XMVectorSet(v1.pos.x - v0.pos.x, v1.pos.y - v0.pos.y, v1.pos.z - v0.pos.z, 0.0f);
	//	DirectX::XMVECTOR e1 = DirectX::XMVectorSet(v2.pos.x - v0.pos.x, v2.pos.y - v0.pos.y, v2.pos.z - v0.pos.z, 0.0f);
	//	DirectX::XMVECTOR normal = DirectX::XMVector3Cross(e0, e1);
	//	vertexnormal[i    ] = DirectX::XMVectorAdd(vertexnormal[i    ], normal);
	//	vertexnormal[i + 1] = DirectX::XMVectorAdd(vertexnormal[i + 1], normal);
	//	vertexnormal[i + 2] = DirectX::XMVectorAdd(vertexnormal[i + 2], normal);
	//}
	//
	//for (size_t i = 0; i < vertexnum; i++)
	//	DirectX::XMStoreFloat3(&m_vertexs[i].normal, DirectX::XMVector3Normalize(vertexnormal[i]));
}
	
bool Square::CreateModel(ID3D11Device* d3dDevice, ID3D11DeviceContext* deviceContext, 
	void const* shaderByteCode, size_t byteCodeLength)
{
	size_t indexSize = sizeof(uint16_t) * indexCount;
	size_t vertexSize = sizeof(Vertex) * vertexnum;
	GeneralVertexData();

	Microsoft::WRL::ComPtr<ID3D11Buffer> vb;
	{
		D3D11_BUFFER_DESC desc = { 0 };
		desc.ByteWidth = static_cast<UINT>(vertexSize);
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.MiscFlags = 0;
		desc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA vertexData = { 0 };
		vertexData.pSysMem = m_vertexs;
		vertexData.SysMemPitch = 0;
		vertexData.SysMemSlicePitch = 0;
		FAILEDRETURN(d3dDevice->CreateBuffer(&desc, &vertexData, vb.GetAddressOf()));
	}

	Microsoft::WRL::ComPtr<ID3D11Buffer> ib;
	{
		D3D11_BUFFER_DESC desc = { 0 };
		desc.ByteWidth = static_cast<UINT>(indexSize);
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.MiscFlags = 0;
		desc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA indexData = { 0 };
		indexData.pSysMem = indexs;
		indexData.SysMemPitch = 0;
		indexData.SysMemSlicePitch = 0;
		FAILEDRETURN(d3dDevice->CreateBuffer(&desc, &indexData, ib.GetAddressOf()));
	}

	Microsoft::WRL::ComPtr<ID3D11InputLayout> il;
	{

		FAILEDRETURN(
			d3dDevice->CreateInputLayout(InputElements,
				InputElementCount,
				shaderByteCode, byteCodeLength,
				il.GetAddressOf()));
	}

	indexBuffer = ib;
	vertexBuffer = vb;
	inputLayout = il;
	m_init = true;
	return true;
}
bool Square::Draw(ID3D11DeviceContext* deviceContext)
{
	FAILEDRETURN(m_init);
	deviceContext->IASetInputLayout(inputLayout.Get());
	deviceContext->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &vertexStride, &vertexOffset);
	deviceContext->IASetIndexBuffer(indexBuffer.Get(), indexFormat, 0);
	deviceContext->IASetPrimitiveTopology(primitiveType);
	deviceContext->DrawIndexed(indexCount, startIndex, vertexOffset);
	return true;
}

#include "BasicShader.h"
#include"BasicHelper.h"


BasicShader::BasicShader()
{
}


BasicShader::~BasicShader()
{
	
}
void BasicShader::Release()
{
}
bool BasicShader::Initialize(HWND hwnd, ID3D11Device* device)
{
	m_vertexShaderData = std::make_shared<ShaderData>();

	Microsoft::WRL::ComPtr<ID3D11VertexShader> vs;
	{
		ID3D10Blob* shader = 0;
		FALSERETURN(CompileShaderFile(hwnd, L"../Shader/basic.vs", "vs_5_0", &shader));
		FAILEDRETURN(device->CreateVertexShader(shader->GetBufferPointer(), shader->GetBufferSize(), NULL, vs.GetAddressOf()));
		m_vertexShaderData.get()->size = shader->GetBufferSize();
		m_vertexShaderData.get()->shaderCode = new BYTE[shader->GetBufferSize()];
		memcpy(m_vertexShaderData.get()->shaderCode, shader->GetBufferPointer(), shader->GetBufferSize());
		shader->Release();
	}

	Microsoft::WRL::ComPtr<ID3D11PixelShader> ps;
	{
		ID3D10Blob* shader = 0;
		FALSERETURN(CompileShaderFile(hwnd, L"../Shader/basic.ps", "ps_5_0", &shader));
		FAILEDRETURN(device->CreatePixelShader(shader->GetBufferPointer(), shader->GetBufferSize(), NULL, ps.GetAddressOf()));
		shader->Release();
	}

	Microsoft::WRL::ComPtr<ID3D11Buffer> buf;
	{
		D3D11_BUFFER_DESC desc;
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.ByteWidth = sizeof(ShaderMatrices);
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.MiscFlags = 0;
		desc.StructureByteStride = 0;
		FAILEDRETURN(device->CreateBuffer(&desc, NULL, buf.GetAddressOf()));
	}

	vsPtr = vs;
	psPtr = ps;
	constantBuffer = buf;
	return true;
}

bool BasicShader::Apply(ID3D11DeviceContext* deviceContext)
{
	if (dirty)
	{
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		FAILEDRETURN(deviceContext->Map(constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));
		ShaderMatrices * dataPtr = (ShaderMatrices*)mappedResource.pData;
		dataPtr->world = XMMatrixTranspose(matrix.world);
		dataPtr->view = XMMatrixTranspose(matrix.view);
		dataPtr->projection = XMMatrixTranspose(matrix.projection);
		deviceContext->Unmap(constantBuffer.Get(), 0);
		DirtyClear();
	}
	{
		deviceContext->VSSetConstantBuffers(constantBufferNum, 1, constantBuffer.GetAddressOf());
	}
	deviceContext->VSSetShader(vsPtr.Get(), NULL, 0);
	deviceContext->PSSetShader(psPtr.Get(), NULL, 0);
	return true;
}

void BasicShader::GetVertexShaderBytecode(void const** pShaderByteCode, size_t* pByteCodeLength)
{
	(*pShaderByteCode) = m_vertexShaderData.get()->shaderCode;
	(*pByteCodeLength) = m_vertexShaderData.get()->size;
}
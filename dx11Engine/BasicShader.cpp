#include "BasicShader.h"
#include"BasicHelper.h"

const D3D11_INPUT_ELEMENT_DESC BasicShader::InputElements[] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",   0, DXGI_FORMAT_R32G32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};
const WCHAR BasicShader::vs_path[MAXPATHLEN] = L"../Shader/basic.vs";
const WCHAR BasicShader::ps_path[MAXPATHLEN] = L"../Shader/basic.ps";
const WCHAR BasicShader::vs_path_tex[MAXPATHLEN] = L"../Shader/basictex.vs";
const WCHAR BasicShader::ps_path_tex[MAXPATHLEN] = L"../Shader/basictex.ps";
BasicShader::BasicShader(bool texture_en):m_texure_en(texture_en)
{
	auto count = m_texure_en ? TexElementCount : BaiscElementCount;
	vbDecl = std::make_shared<std::vector<D3D11_INPUT_ELEMENT_DESC>>(InputElements, InputElements + count);
}


BasicShader::~BasicShader()
{
	
}
bool BasicShader::SetTexture(TextureClass *texture)
{
	m_texture = texture;
}


bool BasicShader::Initialize(HWND hwnd, ID3D11Device* device)
{
	m_vertexShaderData = std::make_shared<ShaderData>();
	const WCHAR *t[SHADER_NUM] = { m_texure_en ? vs_path_tex : vs_path, nullptr, nullptr, nullptr, m_texure_en? ps_path_tex : ps_path, nullptr };
	FALSERETURN(CreateShader(hwnd, device, t));

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
		deviceContext->VSSetConstantBuffers(0, constantBufferNum, constantBuffer.GetAddressOf());
		if(m_texture)
			deviceContext->PSSetShaderResources(0, 1, m_texture->GetTexture());
	}
	Shader::Apply(deviceContext);
	return true;
}

#include "BasicShader.h"
#include"BasicHelper.h"

const D3D11_INPUT_ELEMENT_DESC BasicShader::InputElements[] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",   0, DXGI_FORMAT_R32G32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};
const std::map<int, BasicShader::PATH>::value_type  BasicShader::shaderPathInit[] = {
	std::map<int, PATH>::value_type(Color, PATH(L"../Shader/basic.vs", L"../Shader/basic.ps")),
	std::map<int, PATH>::value_type(Color | Tex, PATH(L"../Shader/basictex.vs", L"../Shader/basictex.ps")),
	std::map<int, PATH>::value_type(Color | Tex | DiffuseLight, PATH(L"../Shader/basictexdiffuse.vs", L"../Shader/basictexdiffuse.ps"))
};

const std::map<int, BasicShader::PATH>  BasicShader::shaderPath(shaderPathInit, shaderPathInit + ShaderTypeNumber);
BasicShader::BasicShader(int flag):m_flag((ShaderFlag)flag)
{
	auto count = m_flag & Tex ? TexElementCount : BaiscElementCount;
	vbDecl = std::make_shared<std::vector<D3D11_INPUT_ELEMENT_DESC>>(InputElements, InputElements + count);
}


BasicShader::~BasicShader()
{
	
}
void BasicShader::SetTexture(ID3D11ShaderResourceView *texture)
{
	m_texture = texture;
	
}


bool BasicShader::Initialize(HWND hwnd, ID3D11Device* device)
{
	PATH cur = shaderPath.find(m_flag)->second;
	m_vertexShaderData = std::make_shared<ShaderData>();
	const WCHAR *t[SHADER_NUM] = {std::get<0>(cur).c_str(), nullptr, nullptr, nullptr,  std::get<1>(cur).c_str() };
	FALSERETURN(CreateShader(hwnd, device, t));

	Microsoft::WRL::ComPtr<ID3D11Buffer> mvp_buf;
	{
		D3D11_BUFFER_DESC desc;
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.ByteWidth = sizeof(ShaderMatrices);
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.MiscFlags = 0;
		desc.StructureByteStride = 0;
		FAILEDRETURN(device->CreateBuffer(&desc, NULL, mvp_buf.GetAddressOf()));
	}

	Microsoft::WRL::ComPtr<ID3D11Buffer> diff_buf;
	if(m_flag & DiffuseLight)
	{
		D3D11_BUFFER_DESC desc;
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.ByteWidth = sizeof(DiffuseMatrics);
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.MiscFlags = 0;
		desc.StructureByteStride = 0;
		FAILEDRETURN(device->CreateBuffer(&desc, NULL, diff_buf.GetAddressOf()));
	}

	Microsoft::WRL::ComPtr<ID3D11SamplerState> sampleState;
	if(m_flag & Tex)
	{
		D3D11_SAMPLER_DESC samplerDesc;
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.MipLODBias = 0.0f;
		samplerDesc.MaxAnisotropy = 1;
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		samplerDesc.BorderColor[0] = 0;
		samplerDesc.BorderColor[1] = 0;
		samplerDesc.BorderColor[2] = 0;
		samplerDesc.BorderColor[3] = 0;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

		// Create the texture sampler state.
		FAILEDRETURN(device->CreateSamplerState(&samplerDesc, sampleState.GetAddressOf()));
	}



	m_sampleState = sampleState;
	cb_mvp = mvp_buf;
	cb_diffuse = diff_buf;
	return true;
}

void BasicShader::SetDiffusePara(const DirectX::XMFLOAT4& color, const DirectX::XMFLOAT3& direction)
{
	if (nullptr == m_diffuse.get())
		m_diffuse = std::make_unique<DiffuseMatrics>();
	m_diffuse->diffuseColor = color;
	m_diffuse->lightDirection = direction;
}

bool BasicShader::Apply(ID3D11DeviceContext* deviceContext)
{
	if (dirty)
	{
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		FAILEDRETURN(deviceContext->Map(cb_mvp.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));
		ShaderMatrices * dataPtr = (ShaderMatrices*)mappedResource.pData;
		dataPtr->world = XMMatrixTranspose(matrix.world);
		dataPtr->view = XMMatrixTranspose(matrix.view);
		dataPtr->projection = XMMatrixTranspose(matrix.projection);
		deviceContext->Unmap(cb_mvp.Get(), 0);
		DirtyClear();
	}
	if(m_flag & DiffuseLight)
	{
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		FAILEDRETURN(deviceContext->Map(cb_diffuse.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));
		DiffuseMatrics * dataPtr = (DiffuseMatrics*)mappedResource.pData;
		// Copy the lighting variables into the constant buffer.
		dataPtr->diffuseColor = m_diffuse->diffuseColor;
		dataPtr->lightDirection = m_diffuse->lightDirection;
		dataPtr->padding = 0.0f;
		deviceContext->Unmap(cb_diffuse.Get(), 0);
	}
	if(m_flag & Tex)
	{
		deviceContext->PSSetShaderResources(0, 1, &m_texture);
		deviceContext->PSSetSamplers(0, 1, m_sampleState.GetAddressOf());
	}
	
	deviceContext->VSSetConstantBuffers(0, constantBufferNum, cb_mvp.GetAddressOf());
	deviceContext->PSSetConstantBuffers(0, constantBufferNum, cb_diffuse.GetAddressOf());

	Shader::Apply(deviceContext);
	return true;
}

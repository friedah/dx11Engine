#pragma once
#include "Shader.h"
#include "BasicHelper.h"
#include <string>
#include <map>
struct DiffuseMatrics
{
	DirectX::XMFLOAT4 diffuseColor;
	DirectX::XMFLOAT3 lightDirection;
	float padding;  // Added extra padding so structure is a multiple of 16 for CreateBuffer function requirements.
};
class BasicShader:public Shader, public IShaderMatrix
{
public:
	enum ShaderFlag
	{
		Color = 0,
		Tex = 1,
		DiffuseLight = 2
	};
private:
	using PATH = std::tuple<std::wstring, std::wstring>;

	static const int constantBufferNum = 1;
	static const int InputElementCount = 3;
	static const int BaiscElementCount = 2;
	static const int TexElementCount = 3;
	static const int ShaderTypeNumber = 3;

	Microsoft::WRL::ComPtr<ID3D11Buffer> cb_mvp, cb_diffuse;
	static const D3D11_INPUT_ELEMENT_DESC InputElements[InputElementCount];
	static const std::map<int, PATH>::value_type  shaderPathInit[ShaderTypeNumber];
	static const std::map<int, PATH> shaderPath;
	int m_flag;
	ID3D11ShaderResourceView *m_texture = nullptr;
	Microsoft::WRL::ComPtr<ID3D11SamplerState> m_sampleState;
	std::unique_ptr<DiffuseMatrics> m_diffuse;
public:
	
	BasicShader(int flag = Color);
	~BasicShader();
	bool Initialize(HWND hwnd, ID3D11Device* device) override;
	void SetTexture(ID3D11ShaderResourceView *texture);
	void SetDiffusePara(const DirectX::XMFLOAT4 &color, const DirectX::XMFLOAT3 &direction);
	bool Apply(ID3D11DeviceContext* deviceContext) override;
};


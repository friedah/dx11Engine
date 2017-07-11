#pragma once
#include "Shader.h"
class BasicShader:public Shader, public IShaderMatrix
{
	Microsoft::WRL::ComPtr<ID3D11VertexShader> vsPtr;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>  psPtr;
	Microsoft::WRL::ComPtr<ID3D11Buffer> constantBuffer;
	static const int constantBufferNum = 1;
public:
	BasicShader();
	~BasicShader();
	virtual bool Initialize(HWND hwnd, ID3D11Device* device) override;
	virtual bool Apply(ID3D11DeviceContext* deviceContext) override;
	virtual void Release() override;
	virtual void GetVertexShaderBytecode(void const** pShaderByteCode, size_t* pByteCodeLength);
};


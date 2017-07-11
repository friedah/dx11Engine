#pragma once
#include <d3d11_1.h>
#include <wrl\client.h>
#include <stdint.h>
#include <memory>
#include <DirectXMath.h>
struct ShaderMatrices
{
	ShaderMatrices();

	DirectX::XMMATRIX world;
	DirectX::XMMATRIX view;
	DirectX::XMMATRIX projection;
};
struct ShaderData
{
	BYTE * shaderCode;
	UINT size;
	ShaderData():shaderCode(NULL), size(0)
	{
		
	}
	~ShaderData()
	{
		delete shaderCode;
	}
};
class Shader
{
protected:
	std::shared_ptr<ShaderData> m_vertexShaderData;
public:
	Shader();
	virtual ~Shader();
	virtual bool Apply(ID3D11DeviceContext* deviceContext) = 0;
	virtual void GetVertexShaderBytecode(void const** pShaderByteCode, size_t* pByteCodeLength) = 0;
	virtual bool Initialize(HWND hwnd, ID3D11Device* device) = 0;
	virtual void Release() = 0;

	virtual bool CompileShaderFile(HWND hwnd, const WCHAR* fileName, const char * shaderLan,  ID3D10Blob** shader);
	void OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, const WCHAR* shaderFilename);
	
};

class IShaderMatrix
{
protected:
	ShaderMatrices matrix;
	bool dirty;
public:
	IShaderMatrix();
	virtual ~IShaderMatrix();
	virtual void DirtyClear();
	virtual void SetWorld(const DirectX::FXMMATRIX &value);
	virtual void SetView(const DirectX::FXMMATRIX &value);
	virtual void SetProjection(const DirectX::FXMMATRIX &value);
};

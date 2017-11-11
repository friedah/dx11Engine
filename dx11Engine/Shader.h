#pragma once
#include <d3d11_1.h>
#include <wrl\client.h>
#include <stdint.h>
#include <memory>
#include <DirectXMath.h>
#include <vector>

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
enum ShaderType
{
	VS,
	HS,
	DS,
	GS,
	PS,
	CS,
	SHADER_NUM,
};
class Shader
{
protected:
	Microsoft::WRL::ComPtr<ID3D11VertexShader> vsPtr;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>  psPtr;
	Microsoft::WRL::ComPtr<ID3D11HullShader>  hsPtr;
	Microsoft::WRL::ComPtr<ID3D11DomainShader>  dsPtr;
	Microsoft::WRL::ComPtr<ID3D11GeometryShader>  gsPtr;
	Microsoft::WRL::ComPtr<ID3D11ComputeShader>  csPtr;

	std::shared_ptr<ShaderData> m_vertexShaderData;
	std::shared_ptr<std::vector<D3D11_INPUT_ELEMENT_DESC>>  vbDecl;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>               inputLayout;
public:
	Shader();
	virtual ~Shader();
	virtual bool Apply(ID3D11DeviceContext* deviceContext);
	virtual void GetVertexShaderBytecode(void const** pShaderByteCode, size_t* pByteCodeLength);
	virtual bool Initialize(HWND hwnd, ID3D11Device* device) = 0;
	virtual void Release() {};

	virtual bool CompileShaderFile(HWND hwnd, const WCHAR* fileName, const char * shaderLan,  ID3D10Blob** shader);
	void OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, const WCHAR* shaderFilename);
	bool CreateShader(HWND hwnd, ID3D11Device* device, const WCHAR *file_path[SHADER_NUM]);
	
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

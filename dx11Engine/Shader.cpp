#include "Shader.h"
#include <d3dcompiler.h>
#include <fstream>
#include "BasicHelper.h"

Shader::Shader()
{
}


Shader::~Shader()
{
}

void Shader::OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, const WCHAR* shaderFilename)
{
	char* compileErrors;
	unsigned long long bufferSize, i;
	std::ofstream fout;


	// Get a pointer to the error message text buffer.
	compileErrors = (char*)(errorMessage->GetBufferPointer());

	// Get the length of the message.
	bufferSize = errorMessage->GetBufferSize();

	// Open a file to write the error message to.
	fout.open("shader-error.txt");

	// Write out the error message.
	for (i = 0; i<bufferSize; i++)
	{
		fout << compileErrors[i];
	}

	// Close the file.
	fout.close();

	// Release the error message.
	errorMessage->Release();
	errorMessage = 0;

	MessageBox(hwnd, L"Error compiling shader.  Check shader-error.txt for message.", shaderFilename, MB_OK);

	return;
}
bool Shader::CompileShaderFile(HWND hwnd, const WCHAR* fileName, const char * shaderLan, ID3D10Blob** shader)
{
	ID3D10Blob* errorMessage;
	HRESULT result = D3DCompileFromFile(fileName, NULL, NULL, "main", shaderLan, D3D10_SHADER_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0,
		shader, &errorMessage);
	if (FAILED(result))
	{
		// If the shader failed to compile it should have writen something to the error message.
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd,  fileName);
		}
		return false;
	}
	return true;
}

bool Shader::CreateShader(HWND hwnd, ID3D11Device* device, const WCHAR *file_path[])
{
	static const std::string shader_model[SHADER_NUM] = {
		"vs_5_0",
		"hs_5_0",
		"ds_5_0",
		"gs_5_0",
		"ps_5_0",
		"cs_5_0",
	};
	for (auto i = 0; i < SHADER_NUM; ++i)
	{
		ID3D10Blob* shader = nullptr;
		if (nullptr == file_path[i])
			continue;
		FALSERETURN(CompileShaderFile(hwnd, file_path[i], shader_model[i].c_str(), &shader));
		switch ((ShaderType)i)
		{
		case VS:
			FAILEDRETURN(device->CreateVertexShader(shader->GetBufferPointer(), shader->GetBufferSize(), NULL, vsPtr.GetAddressOf()));
			m_vertexShaderData->size = shader->GetBufferSize();
			m_vertexShaderData->shaderCode = new BYTE[shader->GetBufferSize()];
			memcpy(m_vertexShaderData->shaderCode, shader->GetBufferPointer(), shader->GetBufferSize());
			break;
		case HS:
			FAILEDRETURN(device->CreateHullShader(shader->GetBufferPointer(), shader->GetBufferSize(), NULL, hsPtr.GetAddressOf()));
			break;
		case DS:
			FAILEDRETURN(device->CreateDomainShader(shader->GetBufferPointer(), shader->GetBufferSize(), NULL, dsPtr.GetAddressOf()));
			break;
		case GS:
			FAILEDRETURN(device->CreateGeometryShader(shader->GetBufferPointer(), shader->GetBufferSize(), NULL, gsPtr.GetAddressOf()));
			break;
		case PS:
			FAILEDRETURN(device->CreatePixelShader(shader->GetBufferPointer(), shader->GetBufferSize(), NULL, psPtr.GetAddressOf()));
			break;
		case CS:
			FAILEDRETURN(device->CreateComputeShader(shader->GetBufferPointer(), shader->GetBufferSize(), NULL, csPtr.GetAddressOf()));
			break;
		default:
			FALSERETURN(false);
		}
		shader->Release();
	}
	if(vbDecl.get())
	{
		Microsoft::WRL::ComPtr<ID3D11InputLayout> il;
		{

			FAILEDRETURN(
				device->CreateInputLayout(vbDecl->data(),
					vbDecl->size(),
					m_vertexShaderData->shaderCode, m_vertexShaderData->size,
					il.GetAddressOf()));
		}
		inputLayout = il;
	}



	return true;
}

bool Shader::Apply(ID3D11DeviceContext* deviceContext)
{
	if(inputLayout.Get())
		deviceContext->IASetInputLayout(inputLayout.Get());
	if (vsPtr.Get())
		deviceContext->VSSetShader(vsPtr.Get(), nullptr, 0);
	if (psPtr.Get())
		deviceContext->PSSetShader(psPtr.Get(), nullptr, 0);
	if (hsPtr.Get())
		deviceContext->HSSetShader(hsPtr.Get(), nullptr, 0);
	if (dsPtr.Get())
		deviceContext->DSSetShader(dsPtr.Get(), nullptr, 0);
	if (gsPtr.Get())
		deviceContext->GSSetShader(gsPtr.Get(), nullptr, 0);
	if (csPtr.Get())
		deviceContext->CSSetShader(csPtr.Get(), nullptr, 0);
	return true;
}

void Shader::GetVertexShaderBytecode(void const** pShaderByteCode, size_t* pByteCodeLength)
{
	if (nullptr == vsPtr.Get())
		return;
	(*pShaderByteCode) = m_vertexShaderData.get()->shaderCode;
	(*pByteCodeLength) = m_vertexShaderData.get()->size;
}
ShaderMatrices::ShaderMatrices()
{
	world = DirectX::XMMatrixIdentity();
	view = DirectX::XMMatrixIdentity();
	projection = DirectX::XMMatrixIdentity();
}

void IShaderMatrix::SetWorld(const DirectX::FXMMATRIX &value)
{
	matrix.world = value;
	dirty = true;
}
void IShaderMatrix::SetView(const DirectX::FXMMATRIX &value)
{
	matrix.view = value;
	dirty = true;
}
void IShaderMatrix::SetProjection(const DirectX::FXMMATRIX &value)
{
	matrix.projection= value;
	dirty = true;
}

IShaderMatrix::IShaderMatrix():dirty(false)
{
}
IShaderMatrix::~IShaderMatrix()
{

}

void IShaderMatrix::DirtyClear()
{
	dirty = false;
}
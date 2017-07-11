#include "Shader.h"
#include <d3dcompiler.h>
#include <fstream>

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
#include "TextureClass.h"
#include <DDSTextureLoader.h>



TextureClass::TextureClass()
{
}


TextureClass::~TextureClass()
{
}

bool TextureClass::Initialize(ID3D11Device* device, WCHAR* texturePath)
{
	HRESULT result = DirectX::CreateDDSTextureFromFile(device, texturePath, m_texture.GetAddressOf(), m_textureView.GetAddressOf());
	if (FAILED(result))
	{
		return false;
	}
	return true;
}

ID3D11ShaderResourceView* TextureClass::GetTexture()
{
	return m_textureView.Get();
}


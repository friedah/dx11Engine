#include "TextureClass.h"
#include <DDSTextureLoader.h>



TextureClass::TextureClass()
{
	m_texture = NULL;
}


TextureClass::~TextureClass()
{
}

bool TextureClass::Initialize(ID3D11Device* device, WCHAR* texturePath)
{
	HRESULT result = DirectX::CreateDDSTextureFromFile(device, texturePath, &m_texture, &m_textureView);
	if (FAILED(result))
	{
		return false;
	}
	return true;
}
void TextureClass::Shutdown()
{
	if (m_texture)
		delete m_texture;

	if (m_textureView)
		delete m_textureView;
}
ID3D11ShaderResourceView* TextureClass::GetTexture()
{
	return m_textureView;
}


#pragma once
//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
class TextureClass
{
public:
	bool Initialize(ID3D11Device*, WCHAR*);
	void Shutdown();
	ID3D11ShaderResourceView* GetTexture();

	TextureClass();
	~TextureClass();
private:
	ID3D11Resource* m_texture;
	ID3D11ShaderResourceView* m_textureView;
};


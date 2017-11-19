#pragma once
//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <wrl/client.h>

class TextureClass
{
public:
	bool Initialize(ID3D11Device*, WCHAR*);
	ID3D11ShaderResourceView* GetTexture();

	TextureClass();
	~TextureClass();
private:
	Microsoft::WRL::ComPtr<ID3D11Resource> m_texture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_textureView;
};


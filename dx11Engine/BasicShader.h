#pragma once
#include "Shader.h"
#include "BasicHelper.h"
#include "TextureClass.h"

class BasicShader:public Shader, public IShaderMatrix
{
	Microsoft::WRL::ComPtr<ID3D11Buffer> constantBuffer;
	static const int constantBufferNum = 1;
	static const int InputElementCount = 3;
	static const int BaiscElementCount = 2;
	static const int TexElementCount = 3;
	static const D3D11_INPUT_ELEMENT_DESC InputElements[InputElementCount];
	static const WCHAR vs_path[MAXPATHLEN], ps_path[MAXPATHLEN];
	static const WCHAR vs_path_tex[MAXPATHLEN], ps_path_tex[MAXPATHLEN];
	bool m_texure_en = false;
	TextureClass *m_texture = nullptr;
public:
	BasicShader(bool texture_en = false);
	~BasicShader();
	bool Initialize(HWND hwnd, ID3D11Device* device) override;
	bool SetTexture(TextureClass *texture);
	bool Apply(ID3D11DeviceContext* deviceContext) override;
};


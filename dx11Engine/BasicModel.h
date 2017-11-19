#pragma once
#include "Model.h"
#include "TextureClass.h"
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXCollision.h>
#include <memory>
class BasicModel:public Model
{
	struct Vertex
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT3 normal;
		DirectX::XMFLOAT2 texcoord;
	};
	std::unique_ptr<Vertex[]> m_model;
	std::unique_ptr<unsigned short[]> m_index;
	std::shared_ptr<TextureClass> m_Texture;
	bool LoadModel(char* filename);
	bool LoadTexture(ID3D11Device* device, WCHAR* filename);
	virtual bool CreateModel(ID3D11Device* d3dDevice) override;
public:
	TextureClass* getTexture()const { return m_Texture.get(); }
	BasicModel();
	virtual ~BasicModel();
	bool Initialize(ID3D11Device* device, char* modelFilename, WCHAR* textureFilename);


};


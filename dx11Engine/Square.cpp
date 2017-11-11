#include "Square.h"
#include "BasicHelper.h"
#include "fstream"
BasicModel::BasicModel()
{
	vertexStride = sizeof(Vertex);
}


BasicModel::~BasicModel()
{
}

bool BasicModel::Initialize(ID3D11Device* device, char* modelFilename, WCHAR* textureFilename)
{
	bool result;


	// Load in the model data,
	FALSERETURN(LoadModel(modelFilename));

	// Load the texture for this model.
	result = LoadTexture(device, textureFilename);
	if (!result)
	{
		return false;
	}
	CreateModel(device);
	return true;
}
	
bool BasicModel::CreateModel(ID3D11Device* d3dDevice)
{
	size_t indexSize = sizeof(uint16_t) * indexCount;
	size_t vertexSize = sizeof(Vertex) * vertexCount;

	FALSERETURN(CreateDataBuffer(d3dDevice, vertexSize, m_model.get(), indexSize, m_index.get()));

	m_init = true;
	return true;
}


bool BasicModel::LoadModel(char* filename)
{
	std::ifstream fin;
	char input;
	int i;


	// Open the model file.
	fin.open(filename);

	// If it could not open the file then exit.
	if (fin.fail())
	{
		return false;
	}

	// Read up to the value of vertex count.
	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}

	// Read in the vertex count.
	fin >> vertexCount;

	// Set the number of indices to be the same as the vertex count.
	indexCount = vertexCount;

	// Create the model using the vertex count that was read in.
	m_model = std::make_unique<Vertex[]>(vertexCount);


	// Read up to the beginning of the data.
	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}
	fin.get(input);
	fin.get(input);

	// Read in the vertex data.
	for (i = 0; i<vertexCount; i++)
	{
		float a, b, c;
		fin >> a >> b >> c;
		m_model[i].pos = DirectX::XMFLOAT3(a, b, c);

		fin >> a >> b;
		m_model[i].texcoord = DirectX::XMFLOAT2(a, b);

		fin >> a >> b >> c;
		m_model[i].normal = DirectX::XMFLOAT3(a, b, c);

		m_index[i] = i;
	}

	// Close the model file.
	fin.close();



	return true;
}


bool BasicModel::LoadTexture(ID3D11Device* device, WCHAR* filename)
{
	bool result;
	if (nullptr == filename)
		return true;

	// Create the texture object.
	m_Texture = std::make_shared<TextureClass>();
	if (!m_Texture)
	{
		return false;
	}

	// Initialize the texture object.
	result = m_Texture->Initialize(device, filename);
	if (!result)
	{
		return false;
	}

	return true;
}

bool BasicModel::Draw(ID3D11DeviceContext* deviceContext) 
{
	if(m_Texture.get())

}

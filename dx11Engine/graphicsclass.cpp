////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "graphicsclass.h"
#include"BasicHelper.h"

GraphicsClass::GraphicsClass()
{
	m_Direct3D = 0;
}


GraphicsClass::GraphicsClass(const GraphicsClass& other)
{
}


GraphicsClass::~GraphicsClass()
{
}


bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;


	// Create the Direct3D object.
	m_Direct3D = new D3DClass;
	if(!m_Direct3D)
	{
		return false;
	}

	// Initialize the Direct3D object.
	result = m_Direct3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D.", L"Error", MB_OK);
		return false;
	}
	
	m_basicShader = std::make_shared<BasicShader>(BasicShader::Color | BasicShader::Tex | BasicShader::DiffuseLight);
	FALSERETURN(m_basicShader->Initialize(hwnd, m_Direct3D->GetDevice()));

	m_square = std::make_shared<BasicModel>();
	FALSERETURN(m_square->Initialize(m_Direct3D->GetDevice(), "../data/cube.txt", L"../data/seafloor.dds"));

	m_basicShader->SetTexture(m_square->getTexture()->GetTexture());

	m_camera = std::make_shared<CameraClass>();
	m_camera->SetPosition(0.0f, 0.0f, -10.0f);

	m_light = std::make_shared<Light>(DirectX::XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f));
	return true;
}


void GraphicsClass::Shutdown()
{
	// Release the D3D object.
	if(m_Direct3D)
	{
		m_Direct3D->Shutdown();
		delete m_Direct3D;
		m_Direct3D = 0;
	}

	return;
}


bool GraphicsClass::Frame()
{
	bool result;

	static float rotation = 0.0f;


	// Update the rotation variable each frame.
	rotation += (float)DirectX::XM_PI * 0.01f;
	if (rotation > 2 *DirectX::XM_PI)
	{
		rotation -= 2 * DirectX::XM_PI;
	}
	// Render the graphics scene.
	if(!Render(rotation))
	{
		return false;
	}

	return true;
}


bool GraphicsClass::Render(const float &rotation)
{
	// Clear the buffers to begin the scene.
	m_Direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	m_camera->Render();
	m_square->SetRotate(0, rotation, 0);
	m_square->GetWorldMatrix(worldMatrix);
	m_camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);

	m_basicShader->SetWorld(worldMatrix);
	m_basicShader->SetView(viewMatrix);
	m_basicShader->SetProjection(projectionMatrix);
	m_basicShader->SetDiffusePara(m_light->diffuse_color(), m_light->light_direction());

	m_basicShader->Apply(m_Direct3D->GetDeviceContext());
	m_square->Draw(m_Direct3D->GetDeviceContext());
		
	// Present the rendered scene to the screen.
	m_Direct3D->EndScene();

	return true;
}
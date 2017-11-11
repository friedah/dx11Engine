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
	
	m_basicShader = std::make_shared<BasicShader>();
	FALSERETURN(m_basicShader.get()->Initialize(hwnd, m_Direct3D->GetDevice()));

	m_square = std::make_shared<BasicModel>();
	FALSERETURN(m_square.get()->Initialize(m_Direct3D->GetDevice(), "../Engine/data/cube.txt", L"../Engine/data/seafloor.dds"));

	m_camera = std::make_shared<CameraClass>();
	m_camera.get()->SetPosition(0.0f, 0.0f, -10.0f);
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


	// Render the graphics scene.
	result = Render();
	if(!result)
	{
		return false;
	}

	return true;
}


bool GraphicsClass::Render()
{
	// Clear the buffers to begin the scene.
	m_Direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	m_camera.get()->Render();
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_camera.get()->GetViewMatrix(viewMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);

	m_basicShader.get()->SetWorld(worldMatrix);
	m_basicShader.get()->SetView(viewMatrix);
	m_basicShader.get()->SetProjection(projectionMatrix);

	m_basicShader.get()->Apply(m_Direct3D->GetDeviceContext());
	m_square.get()->Draw(m_Direct3D->GetDeviceContext());
		
	// Present the rendered scene to the screen.
	m_Direct3D->EndScene();

	return true;
}
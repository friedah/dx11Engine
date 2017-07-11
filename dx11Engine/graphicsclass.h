////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "d3dclass.h"
#include "BasicShader.h"
#include "Square.h"
#include "cameraclass.h"
/////////////
// GLOBALS //
/////////////
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;


////////////////////////////////////////////////////////////////////////////////
// Class name: GraphicsClass
////////////////////////////////////////////////////////////////////////////////
class GraphicsClass
{
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

	virtual bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame();
protected:
	virtual bool Render();
	D3DClass* m_Direct3D;
private:
	std::shared_ptr<Square> m_square;
	std::shared_ptr<BasicShader> m_basicShader;
	std::shared_ptr<CameraClass> m_camera;
};

#endif
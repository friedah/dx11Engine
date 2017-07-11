////////////////////////////////////////////////////////////////////////////////
// Filename: TriangleClass.h
////////////////////////////////////////////////////////////////////////////////
#pragma once

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "..\graphicsclass.h"
#include "..\d3dclass.h"
#include "..\cameraclass.h"
#include "modelclass.h"
#include "colorshaderclass.h"


////////////////////////////////////////////////////////////////////////////////
// Class name: TriangleClass
////////////////////////////////////////////////////////////////////////////////
class TriangleClass:public GraphicsClass
{
public:
	TriangleClass();
	TriangleClass(const TriangleClass&);
	~TriangleClass();
	virtual bool Initialize(int, int, HWND);
	virtual void Shutdown();
protected:
	virtual bool Render();

private:
	CameraClass* m_Camera;
	ModelClass* m_Model;
	ColorShaderClass* m_ColorShader;
};


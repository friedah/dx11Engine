#pragma once
#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
class StatuClass
{
public:
	StatuClass();
	~StatuClass();
private:
	ID3D11RasterizerState* m_rasterState;
	ID3D11DepthStencilState* m_depthStencilState;
};


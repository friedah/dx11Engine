#pragma once
#include "Shader.h"

class Texuture
{
public:
	struct TextureDESC
	{
		unsigned int dim;
		unsigned int width;
		unsigned int height;
		unsigned int depth;
		unsigned int mips;
		unsigned int arrays;
		DXGI_FORMAT format;
		unsigned int bind_flag;
		unsigned int msaa;
		bool isCube;
		unsigned int use_mip;
		unsigned int use_arrays;
		unsigned int use_first_array;
		unsigned int use_ws;
		unsigned int use_w;
	};
	Texuture(ID3D11Device* device, WCHAR* texturePath);
	Texuture(ID3D11Device* device, const TextureDESC &desc);
	virtual ~Texuture();
	const ID3D11DepthStencilView* get_depth_view() const {
		return m_depthView.Get();
	};
	const ID3D11RenderTargetView* get_renderTarget_view() const {
		return m_renderTargetView.Get();
	};
	const ID3D11ShaderResourceView* get_shader_view() const {
		return m_shaderView.Get();
	};
private:
	void create_shader_view(ID3D11Device* device, const TextureDESC &desc);
	void create_depth_view(ID3D11Device* device, const TextureDESC &desc);
	void create_renderTarget_view(ID3D11Device* device, const TextureDESC &desc);
	Microsoft::WRL::ComPtr<ID3D11Resource> m_texture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_shaderView;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_depthView;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_renderTargetView;
};

class Buffer
{
public:
	Buffer(ID3D11Device* device, unsigned int size, unsigned int bind_flag, WCHAR* txtPath, unsigned int stride = 0, unsigned int offset = 0, D3D11_USAGE usage = D3D11_USAGE_DEFAULT, unsigned int cpu_access = 0);
	Buffer(ID3D11Device* device, unsigned int size, unsigned int bind_flag, unsigned char * data = nullptr, unsigned int stride = 0, unsigned int offset = 0, D3D11_USAGE usage = D3D11_USAGE_DEFAULT, unsigned int cpu_access = 0);
	virtual ~Buffer();
	const ID3D11Buffer* get() const { return m_buffer.Get(); };
private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_buffer;
};

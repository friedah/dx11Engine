#include "ResouceClass.h"
#include "DirectXTex/DDSTextureLoader.h"


Texuture::Texuture(ID3D11Device* device, WCHAR* texturePath)
{
	HRESULT result = DirectX::CreateDDSTextureFromFile(device, texturePath, m_texture.GetAddressOf(), m_shaderView.GetAddressOf());
	assert(!FAILED(result));
}
Texuture::Texuture(ID3D11Device* device, const TextureDESC &desc)
{
	switch(desc.dim)
	{
	case 1:
		{
			D3D11_TEXTURE1D_DESC textdesc;
			ZeroMemory(&textdesc, sizeof(textdesc));

			// Set up the description of the depth buffer.
			textdesc.Width = desc.width;
			textdesc.MipLevels = desc.mips;
			textdesc.ArraySize = desc.arrays;
			textdesc.Format = desc.format;
			textdesc.Usage = D3D11_USAGE_DEFAULT;
			textdesc.BindFlags = desc.bind_flag;
			textdesc.CPUAccessFlags = 0;
			textdesc.MiscFlags = 0;
			device->CreateTexture1D(&textdesc, nullptr, reinterpret_cast<ID3D11Texture1D**> (m_texture.GetAddressOf()));
		}
		break;
	case 2:
		{
		D3D11_TEXTURE2D_DESC textdesc;
		ZeroMemory(&textdesc, sizeof(textdesc));

		// Set up the description of the depth buffer.
		textdesc.Width = desc.width;
		textdesc.Height = desc.height;
		textdesc.MipLevels = desc.mips;
		textdesc.ArraySize = desc.arrays;
		textdesc.Format = desc.format;
		textdesc.SampleDesc.Count = desc.msaa;
		textdesc.SampleDesc.Quality = 0;
		textdesc.Usage = D3D11_USAGE_DEFAULT;
		textdesc.BindFlags = desc.bind_flag;
		textdesc.CPUAccessFlags = 0;
		textdesc.MiscFlags = desc.isCube ? D3D11_RESOURCE_MISC_TEXTURECUBE : 0;
		device->CreateTexture2D(&textdesc, nullptr, reinterpret_cast<ID3D11Texture2D**> (m_texture.GetAddressOf()));
		}
		break;
	case 3:
		{
		D3D11_TEXTURE3D_DESC textdesc;
		ZeroMemory(&textdesc, sizeof(textdesc));

		// Set up the description of the depth buffer.
		textdesc.Width = desc.width;
		textdesc.Height = desc.height;
		textdesc.Depth = desc.depth;
		textdesc.MipLevels = desc.mips;
		textdesc.Format = desc.format;
		textdesc.Usage = D3D11_USAGE_DEFAULT;
		textdesc.BindFlags = desc.bind_flag;
		textdesc.CPUAccessFlags = 0;
		textdesc.MiscFlags = 0;
		device->CreateTexture3D(&textdesc, nullptr, reinterpret_cast<ID3D11Texture3D**> (m_texture.GetAddressOf()));
		}
		break;
	default:
		assert(0);
	}
	
	create_shader_view(device, desc);
}
void Texuture::create_shader_view(ID3D11Device* device, const TextureDESC &desc)
{
	HRESULT hr;
	if (desc.bind_flag & D3D11_BIND_SHADER_RESOURCE)
	{

		D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
		memset(&SRVDesc, 0, sizeof(SRVDesc));
		SRVDesc.Format = desc.format;
		switch (desc.dim)
		{
		case 1:
		{
			if (desc.arrays > 1)
			{
				SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE1DARRAY;
				SRVDesc.Texture1DArray.MipLevels = desc.mips;
				SRVDesc.Texture1DArray.ArraySize = static_cast<UINT>(desc.arrays);
			}
			else
			{
				SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE1D;
				SRVDesc.Texture1D.MipLevels = desc.mips;
			}
		}
		break;
		case 2:
		{
			if (desc.isCube)
			{
				if (desc.arrays > 6)
				{
					SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBEARRAY;
					SRVDesc.TextureCubeArray.MipLevels = desc.mips;

					// Earlier we set arraySize to (NumCubes * 6)
					SRVDesc.TextureCubeArray.NumCubes = static_cast<UINT>(desc.arrays / 6);
				}
				else
				{
					SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
					SRVDesc.TextureCube.MipLevels = desc.mips;
				}
			}
			else if (desc.arrays > 1)
			{
				SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
				SRVDesc.Texture2DArray.MipLevels = desc.mips;
				SRVDesc.Texture2DArray.ArraySize = static_cast<UINT>(desc.arrays);
			}
			else
			{
				SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
				SRVDesc.Texture2D.MipLevels = desc.mips;
			}
		}
		break;
		case 3:
		{
			SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE3D;
			SRVDesc.Texture3D.MipLevels = desc.mips;

		}
		break;
		default:
			assert(0);
		}
		hr = device->CreateShaderResourceView(m_texture.Get(), &SRVDesc, m_shaderView.GetAddressOf());
		assert(!FAILED((hr)));
	}
}
void Texuture::create_renderTarget_view(ID3D11Device* device, const TextureDESC &desc)
{
	HRESULT hr;
	if (desc.bind_flag & D3D11_BIND_RENDER_TARGET)
	{

		D3D11_RENDER_TARGET_VIEW_DESC SRVDesc;
		memset(&SRVDesc, 0, sizeof(SRVDesc));
		SRVDesc.Format = desc.format;
		switch (desc.dim)
		{
		case 1:
		{
			if (desc.arrays > 1)
			{
				SRVDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE1DARRAY;
				SRVDesc.Texture1DArray.MipSlice = desc.use_mip;
				SRVDesc.Texture1DArray.FirstArraySlice = desc.use_first_array;
				SRVDesc.Texture1DArray.ArraySize = static_cast<UINT>(desc.use_arrays);
			}
			else
			{
				SRVDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE1D;
				SRVDesc.Texture1D.MipSlice = desc.mips;
			}
		}
		break;
		case 2:
		{
			
			if (desc.arrays > 1)
			{
				if (desc.msaa)
				{
					SRVDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DMSARRAY;

				}
				else
				{
					SRVDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
					SRVDesc.Texture2DArray.MipSlice = desc.use_mip;
				}
				SRVDesc.Texture2DArray.FirstArraySlice = desc.use_first_array;
				SRVDesc.Texture2DArray.ArraySize = static_cast<UINT>(desc.use_arrays);
			}
			else
			{
				if (desc.msaa)
				{
					SRVDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DMS;

				}
				else {
					SRVDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
					SRVDesc.Texture2D.MipSlice = desc.use_mip;
				}
			}
		}
		break;
		case 3:
		{
			SRVDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE3D;
			SRVDesc.Texture3D.MipSlice = desc.use_mip;
			SRVDesc.Texture3D.WSize = desc.use_ws;
			SRVDesc.Texture3D.FirstWSlice = desc.use_w;

		}
		break;
		default:
			assert(0);
		}
		hr = device->CreateRenderTargetView(m_texture.Get(), &SRVDesc, m_renderTargetView.GetAddressOf());
		assert(!FAILED((hr)));
	}
}
void Texuture::create_depth_view(ID3D11Device* device, const TextureDESC &desc)
{
	HRESULT hr;
	if (desc.bind_flag & D3D11_BIND_RENDER_TARGET)
	{

		D3D11_DEPTH_STENCIL_VIEW_DESC SRVDesc;
		memset(&SRVDesc, 0, sizeof(SRVDesc));
		SRVDesc.Format = desc.format;
		switch (desc.dim)
		{
		case 1:
		{
			if (desc.arrays > 1)
			{
				SRVDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE1DARRAY;
				SRVDesc.Texture1DArray.MipSlice = desc.use_mip;
				SRVDesc.Texture1DArray.FirstArraySlice = desc.use_first_array;
				SRVDesc.Texture1DArray.ArraySize = static_cast<UINT>(desc.use_arrays);
			}
			else
			{
				SRVDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE1D;
				SRVDesc.Texture1D.MipSlice = desc.mips;
			}
		}
		break;
		case 2:
		{

			if (desc.arrays > 1)
			{
				if (desc.msaa)
				{
					SRVDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMSARRAY;

				}
				else
				{
					SRVDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
					SRVDesc.Texture2DArray.MipSlice = desc.use_mip;
				}
				SRVDesc.Texture2DArray.FirstArraySlice = desc.use_first_array;
				SRVDesc.Texture2DArray.ArraySize = static_cast<UINT>(desc.use_arrays);
			}
			else
			{
				if (desc.msaa)
				{
					SRVDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;

				}
				else {
					SRVDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
					SRVDesc.Texture2D.MipSlice = desc.use_mip;
				}
			}
		}
		break;
		default:
			assert(0);
		}
		hr = device->CreateDepthStencilView(m_texture.Get(), &SRVDesc, m_depthView.GetAddressOf());
		assert(!FAILED((hr)));
	}
}
Texuture::~Texuture()
{
}
Buffer::Buffer(ID3D11Device* device, unsigned size, unsigned bind_flag, WCHAR* txtPath, unsigned stride, unsigned offset, D3D11_USAGE usage, unsigned cpu_access)
{
	
}

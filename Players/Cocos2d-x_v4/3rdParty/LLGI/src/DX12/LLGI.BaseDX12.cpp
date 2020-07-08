
#include "LLGI.BaseDX12.h"

namespace LLGI
{

ID3D12Resource* CreateResourceBuffer(ID3D12Device* device,
									 D3D12_HEAP_TYPE heapType,
									 DXGI_FORMAT format,
									 D3D12_RESOURCE_DIMENSION resourceDimention,
									 D3D12_RESOURCE_STATES resourceState,
									 D3D12_RESOURCE_FLAGS flags,
									 Vec2I size,
									 int32_t samplingCount)
{
	D3D12_HEAP_PROPERTIES heapProps = {};
	D3D12_RESOURCE_DESC resDesc = {};

	ID3D12Resource* resource = nullptr;

	heapProps.Type = heapType;
	heapProps.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heapProps.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;

	heapProps.CreationNodeMask = 1; // TODO: set properly for multi-adaptor.
	heapProps.VisibleNodeMask = 1;	// TODO: set properly for multi-adaptor.

	if (samplingCount > 1)
	{
		D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS msaaQualityDesc{};
		msaaQualityDesc.SampleCount = samplingCount;
		msaaQualityDesc.Format = format;
		msaaQualityDesc.Flags = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;

		HRESULT hr = device->CheckFeatureSupport(D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS, &msaaQualityDesc, sizeof(msaaQualityDesc));
		if (FAILED(hr))
		{
			return nullptr;
		}
	}

	resDesc.Dimension = resourceDimention;
	resDesc.Width = size.X;
	resDesc.Height = size.Y;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.Format = DirectX12::GetGeneratedFormat(format);
	resDesc.Layout = (resourceDimention == D3D12_RESOURCE_DIMENSION_BUFFER ? D3D12_TEXTURE_LAYOUT_ROW_MAJOR : D3D12_TEXTURE_LAYOUT_UNKNOWN);
	resDesc.SampleDesc.Count = samplingCount;
	resDesc.Flags = flags;

	D3D12_CLEAR_VALUE clearValue = {};
	clearValue.Format = format;
	clearValue.Color[0] = 0.0f;
	clearValue.Color[1] = 0.0f;
	clearValue.Color[2] = 0.0f;
	clearValue.Color[3] = 0.0f;

	if ((flags & D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL) != 0)
	{
		clearValue.Format = format;
		clearValue.DepthStencil.Depth = 1.0f;
		clearValue.DepthStencil.Stencil = 0;
	}

	auto setClearValue =
		resourceDimention != D3D12_RESOURCE_DIMENSION_BUFFER &&
		(((flags & D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET) != 0) || ((flags & D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL) != 0));

	// clearValue causes CLEARRENDERTARGETVIEW_MISMATCHINGCLEARVALUE
	auto hr = device->CreateCommittedResource(
		&heapProps, D3D12_HEAP_FLAG_NONE, &resDesc, resourceState, (setClearValue ? &clearValue : nullptr), IID_PPV_ARGS(&resource));

	if (FAILED(hr))
	{
		SafeRelease(resource);
		return nullptr;
	}
	return resource;
}

DXGI_FORMAT ConvertFormat(TextureFormatType format)
{
	if (format == TextureFormatType::R8G8B8A8_UNORM)
		return DXGI_FORMAT_R8G8B8A8_UNORM;

	if (format == TextureFormatType::R16G16B16A16_FLOAT)
		return DXGI_FORMAT_R16G16B16A16_FLOAT;

	if (format == TextureFormatType::R32G32B32A32_FLOAT)
		return DXGI_FORMAT_R32G32B32A32_FLOAT;

	if (format == TextureFormatType::R8G8B8A8_UNORM_SRGB)
		return DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;

	if (format == TextureFormatType::R16G16_FLOAT)
		return DXGI_FORMAT_R16G16_FLOAT;

	if (format == TextureFormatType::R8_UNORM)
		return DXGI_FORMAT_R8_UNORM;

	if (format == TextureFormatType::D32)
		return DXGI_FORMAT_D32_FLOAT;

	if (format == TextureFormatType::D24S8)
		return DXGI_FORMAT_D24_UNORM_S8_UINT;

	throw "Not implemented";
	return DXGI_FORMAT_UNKNOWN;
}

TextureFormatType ConvertFormat(DXGI_FORMAT format)
{
	if (format == DXGI_FORMAT_R8G8B8A8_UNORM)
		return TextureFormatType::R8G8B8A8_UNORM;

	if (format == DXGI_FORMAT_R16G16B16A16_FLOAT)
		return TextureFormatType::R16G16B16A16_FLOAT;

	if (format == DXGI_FORMAT_R32G32B32A32_FLOAT)
		return TextureFormatType::R32G32B32A32_FLOAT;

	if (format == DXGI_FORMAT_R8G8B8A8_UNORM_SRGB)
		return TextureFormatType::R8G8B8A8_UNORM_SRGB;

	if (format == DXGI_FORMAT_R16G16_FLOAT)
		return TextureFormatType::R16G16_FLOAT;

	if (format == DXGI_FORMAT_R8_UNORM)
		return TextureFormatType::R8_UNORM;

	if (format == DXGI_FORMAT_D32_FLOAT)
		return TextureFormatType::D32;

	if (format == DXGI_FORMAT_D24_UNORM_S8_UINT)
		return TextureFormatType::D24S8;

	throw "Not implemented";
	return TextureFormatType::Unknown;
}

namespace DirectX12
{

DXGI_FORMAT GetGeneratedFormat(DXGI_FORMAT format)
{
	if (format == DXGI_FORMAT_D24_UNORM_S8_UINT)
		format = DXGI_FORMAT_R24G8_TYPELESS;

	if (format == DXGI_FORMAT_D32_FLOAT)
		format = DXGI_FORMAT_R32_TYPELESS;

	return format;
}

DXGI_FORMAT GetShaderResourceViewFormat(DXGI_FORMAT format)
{
	if (format == DXGI_FORMAT_D24_UNORM_S8_UINT)
		format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;

	if (format == DXGI_FORMAT_D32_FLOAT)
		format = DXGI_FORMAT_R32_FLOAT;

	return format;
}

} // namespace DirectX12

} // namespace LLGI

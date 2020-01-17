
#include "LLGI.BaseDX12.h"

namespace LLGI
{

ID3D12Resource* CreateResourceBuffer(ID3D12Device* device,
									 D3D12_HEAP_TYPE heapType,
									 DXGI_FORMAT format,
									 D3D12_RESOURCE_DIMENSION resourceDimention,
									 D3D12_RESOURCE_STATES resourceState,
									 D3D12_RESOURCE_FLAGS flags,
									 Vec2I size)
{
	D3D12_HEAP_PROPERTIES heapProps = {};
	D3D12_RESOURCE_DESC resDesc = {};

	ID3D12Resource* resource = nullptr;

	heapProps.Type = heapType;
	heapProps.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heapProps.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;

	heapProps.CreationNodeMask = 1; // TODO: set properly for multi-adaptor.
	heapProps.VisibleNodeMask = 1;  // TODO: set properly for multi-adaptor.

	resDesc.Dimension = resourceDimention;
	resDesc.Width = size.X;
	resDesc.Height = size.Y;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.Format = format;
	resDesc.Layout = (resourceDimention == D3D12_RESOURCE_DIMENSION_BUFFER ? D3D12_TEXTURE_LAYOUT_ROW_MAJOR : D3D12_TEXTURE_LAYOUT_UNKNOWN);
	resDesc.SampleDesc.Count = 1;
	resDesc.Flags = flags;

	D3D12_CLEAR_VALUE clearValue = {};
	clearValue.Format = format;
	clearValue.Color[0] = 0.0f;
	clearValue.Color[1] = 0.0f;
	clearValue.Color[2] = 0.0f;
	clearValue.Color[3] = 0.0f;

	if ((flags & D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL) != 0)
	{
		clearValue.Format = DXGI_FORMAT_D32_FLOAT;
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

	throw "Not implemented";
	return TextureFormatType::Uknown;
}

} // namespace LLGI

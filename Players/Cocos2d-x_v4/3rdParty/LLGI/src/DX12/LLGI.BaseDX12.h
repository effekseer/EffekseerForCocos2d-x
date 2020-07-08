
#pragma once

#include "../LLGI.Base.h"

#include <DirectXMath.h>
#include <d3d12.h>
#include <dxgi1_4.h>

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

namespace LLGI
{

class GraphicsDX12;
class RenderPassDX12;
class SingleFrameMemoryPoolDX12;

ID3D12Resource* CreateResourceBuffer(ID3D12Device* device,
									 D3D12_HEAP_TYPE heapType,
									 DXGI_FORMAT format,
									 D3D12_RESOURCE_DIMENSION resourceDimention,
									 D3D12_RESOURCE_STATES resourceState,
									 D3D12_RESOURCE_FLAGS flags,
									 Vec2I size,
									 int32_t samplingCount);

DXGI_FORMAT ConvertFormat(TextureFormatType format);

TextureFormatType ConvertFormat(DXGI_FORMAT format);

namespace DirectX12
{

DXGI_FORMAT GetGeneratedFormat(DXGI_FORMAT format);

DXGI_FORMAT GetShaderResourceViewFormat(DXGI_FORMAT format);

} // namespace DirectX12

} // namespace LLGI

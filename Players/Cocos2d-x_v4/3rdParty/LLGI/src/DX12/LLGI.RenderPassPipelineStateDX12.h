#pragma once

#include "../LLGI.Graphics.h"
#include "LLGI.BaseDX12.h"
#include <functional>
#include <unordered_map>

namespace LLGI
{

class GraphicsDX12;
class RenderPassDX12;

class RenderPassPipelineStateDX12 : public RenderPassPipelineState
{
private:
public:
	RenderPassPipelineStateDX12() {}

	virtual ~RenderPassPipelineStateDX12() {}

	std::array<DXGI_FORMAT, RenderTargetMax> RenderTargetFormats;
	int32_t RenderTargetCount = 0;
	bool HasDepth = false;
};

struct RenderPassPipelineStateDX12Key
{
	bool isPresentMode;
	bool hasDepth;
	std::array<DXGI_FORMAT, RenderTargetMax> RenderTargetFormats;
	int32_t RenderTargetCount = 0;

	bool operator==(const RenderPassPipelineStateDX12Key& value) const
	{
		if (RenderTargetCount != value.RenderTargetCount)
			return false;

		for (int32_t i = 0; i < RenderTargetCount; i++)
		{
			if (RenderTargetFormats[i] != value.RenderTargetFormats[i])
				return false;
		}

		return (isPresentMode == value.isPresentMode && hasDepth == value.hasDepth);
	}

	struct Hash
	{
		typedef std::size_t result_type;

		std::size_t operator()(const RenderPassPipelineStateDX12Key& key) const
		{
			auto ret = std::hash<bool>()(key.isPresentMode);
			ret += std::hash<bool>()(key.hasDepth);

			for (int32_t i = 0; i < key.RenderTargetCount; i++)
			{
				ret += std::hash<uint64_t>()(key.RenderTargetFormats[i]);
			}

			return ret;
		}
	};
};

} // namespace LLGI

#pragma once

#include "../LLGI.Graphics.h"
#include "../Utils/LLGI.FixedSizeVector.h"
#include "LLGI.BaseVulkan.h"
#include <functional>
#include <unordered_map>

namespace LLGI
{

class RenderPassVulkan;
class RenderPassPipelineStateVulkan;
class RenderPassPipelineStateCacheVulkan;
class TextureVulkan;

class RenderPassVulkan : public RenderPass
{
private:
	RenderPassPipelineStateCacheVulkan* renderPassPipelineStateCache_ = nullptr;
	vk::Device device_;
	ReferenceObject* owner_ = nullptr;

	std::shared_ptr<TextureVulkan> depthBufferPtr;

public:
	struct RenderTargetProperty
	{
		vk::Format format;
		vk::Image colorBuffer;
		std::shared_ptr<TextureVulkan> colorBufferPtr;
	};

	RenderPassPipelineStateVulkan* renderPassPipelineState = nullptr;

	vk::Framebuffer frameBuffer_;

	FixedSizeVector<RenderTargetProperty, RenderTargetMax> renderTargetProperties;

	vk::Image depthBuffer;

	RenderPassVulkan(RenderPassPipelineStateCacheVulkan* renderPassPipelineStateCache, vk::Device device, ReferenceObject* owner);
	virtual ~RenderPassVulkan();

	bool Initialize(const TextureVulkan** textures, int32_t textureCount, TextureVulkan* depthTexture);

	Vec2I GetImageSize() const;

	virtual void SetIsColorCleared(bool isColorCleared) override;

	virtual void SetIsDepthCleared(bool isDepthCleared) override;

private:
	void ResetRenderPassPipelineState();
};

class RenderPassPipelineStateVulkan : public RenderPassPipelineState
{
private:
	vk::Device device_;
	ReferenceObject* owner_ = nullptr;

public:
	RenderPassPipelineStateVulkan(vk::Device device, ReferenceObject* owner);

	virtual ~RenderPassPipelineStateVulkan();

	vk::RenderPass renderPass_;

	vk::RenderPass GetRenderPass() const;
};

struct RenderPassPipelineStateVulkanKey
{
	bool isPresentMode;
	FixedSizeVector<vk::Format, RenderTargetMax> formats;
	bool hasDepth;
	bool isColorCleared;
	bool isDepthCleared;

	bool operator==(const RenderPassPipelineStateVulkanKey& value) const
	{
		return (isPresentMode == value.isPresentMode && hasDepth == value.hasDepth && formats == value.formats &&
				isColorCleared == value.isColorCleared && isDepthCleared == value.isDepthCleared);
	}

	struct Hash
	{
		typedef std::size_t result_type;

		std::size_t operator()(const RenderPassPipelineStateVulkanKey& key) const
		{
			return key.formats.get_hash() + std::hash<bool>()(key.isPresentMode) + std::hash<bool>()(key.hasDepth) +
				   std::hash<bool>()(key.isColorCleared) + std::hash<bool>()(key.isDepthCleared);
		}
	};
};

} // namespace LLGI

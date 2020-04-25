#pragma once

#include "../LLGI.Graphics.h"
#include "LLGI.BaseVulkan.h"
#include "LLGI.RenderPassPipelineStateCacheVulkan.h"
#include "LLGI.RenderPassVulkan.h"
#include <functional>
#include <unordered_map>

namespace LLGI
{

class RenderPassVulkan;
class RenderPassPipelineStateVulkan;
class TextureVulkan;

class GraphicsVulkan : public Graphics
{
private:
	int32_t swapBufferCount_ = 0;

	vk::Device vkDevice;
	vk::Queue vkQueue;
	vk::CommandPool vkCmdPool;
	vk::PhysicalDevice vkPysicalDevice;

	vk::Sampler defaultSampler_ = nullptr;

	std::function<void(vk::CommandBuffer, vk::Fence)> addCommand_;
	RenderPassPipelineStateCacheVulkan* renderPassPipelineStateCache_ = nullptr;
	ReferenceObject* owner_ = nullptr;

public:
	GraphicsVulkan(const vk::Device& device,
				   const vk::Queue& quque,
				   const vk::CommandPool& commandPool,
				   const vk::PhysicalDevice& pysicalDevice,
				   int32_t swapBufferCount,
				   std::function<void(vk::CommandBuffer,vk::Fence)> addCommand,
				   RenderPassPipelineStateCacheVulkan* renderPassPipelineStateCache = nullptr,
				   ReferenceObject* owner = nullptr);

	virtual ~GraphicsVulkan();

	void SetWindowSize(const Vec2I& windowSize) override;

	void Execute(CommandList* commandList) override;

	void WaitFinish() override;

	VertexBuffer* CreateVertexBuffer(int32_t size) override;
	IndexBuffer* CreateIndexBuffer(int32_t stride, int32_t count) override;
	Shader* CreateShader(DataStructure* data, int32_t count) override;
	PipelineState* CreatePiplineState() override;
	SingleFrameMemoryPool* CreateSingleFrameMemoryPool(int32_t constantBufferPoolSize, int32_t drawingCount) override;
	CommandList* CreateCommandList(SingleFrameMemoryPool* memoryPool) override;
	ConstantBuffer* CreateConstantBuffer(int32_t size) override;
	RenderPass* CreateRenderPass(const Texture** textures, int32_t textureCount, Texture* depthTexture) override;

	Texture* CreateTexture(const TextureInitializationParameter& parameter) override;
	Texture* CreateRenderTexture(const RenderTextureInitializationParameter& parameter) override;
	Texture* CreateDepthTexture(const DepthTextureInitializationParameter& parameter) override;

	Texture* CreateTexture(uint64_t id) override;

	std::vector<uint8_t> CaptureRenderTarget(Texture* renderTarget) override;

	RenderPassPipelineState* CreateRenderPassPipelineState(RenderPass* renderPass) override;

	RenderPassPipelineState* CreateRenderPassPipelineState(const RenderPassPipelineStateKey& key) override;

	vk::PhysicalDevice GetPysicalDevice() const { return vkPysicalDevice; }
	vk::Device GetDevice() const { return vkDevice; }
	vk::CommandPool GetCommandPool() const { return vkCmdPool; }
	vk::Queue GetQueue() const { return vkQueue; }

	int32_t GetSwapBufferCount() const;
	uint32_t GetMemoryTypeIndex(uint32_t bits, const vk::MemoryPropertyFlags& properties);

	VkCommandBuffer BeginSingleTimeCommands();
	bool EndSingleTimeCommands(VkCommandBuffer commandBuffer);

	//! temp
	vk::Sampler& GetDefaultSampler() { return defaultSampler_; };
};

} // namespace LLGI
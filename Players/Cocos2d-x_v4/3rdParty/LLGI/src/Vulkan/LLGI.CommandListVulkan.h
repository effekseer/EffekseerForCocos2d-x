#pragma once

#include "../LLGI.CommandList.h"
#include "LLGI.BaseVulkan.h"

namespace LLGI
{
enum class CommandListPreCondition
{
	Standalone,
	External,
};

class DescriptorPoolVulkan
{
private:
	std::shared_ptr<GraphicsVulkan> graphics_;
	vk::DescriptorPool descriptorPool_ = nullptr;
	int32_t size_ = 0;
	int32_t stage_ = 0;
	int32_t offset = 0;
	std::vector<std::vector<vk::DescriptorSet>> cache;

public:
	DescriptorPoolVulkan(std::shared_ptr<GraphicsVulkan> graphics, int32_t size, int stage);
	virtual ~DescriptorPoolVulkan();
	const std::vector<vk::DescriptorSet>& Get(PipelineStateVulkan* pip);
	void Reset();
};

class CommandListVulkan : public CommandList
{
private:
	std::shared_ptr<GraphicsVulkan> graphics_;
	std::vector<vk::CommandBuffer> commandBuffers;
	std::vector<std::shared_ptr<DescriptorPoolVulkan>> descriptorPools;
	int32_t currentSwapBufferIndex_;
	std::vector<vk::Fence> fences_;
	vk::Sampler samplers_[2][2];

public:
	CommandListVulkan();
	~CommandListVulkan() override;

	bool
	Initialize(GraphicsVulkan* graphics, int32_t drawingCount, CommandListPreCondition precondition = CommandListPreCondition::Standalone);

	void Begin() override;
	void End() override;

	void BeginExternal(VkCommandBuffer nativeCommandBuffer);
	void EndExternal();

	void SetScissor(int32_t x, int32_t y, int32_t width, int32_t height) override;
	void Draw(int32_t primitiveCount, int32_t instanceCount) override;
	void CopyTexture(Texture* src, Texture* dst) override;

	void BeginRenderPass(RenderPass* renderPass) override;
	void EndRenderPass() override;
	vk::CommandBuffer GetCommandBuffer() const;
	vk::Fence GetFence() const;

	void WaitUntilCompleted() override;
};

} // namespace LLGI

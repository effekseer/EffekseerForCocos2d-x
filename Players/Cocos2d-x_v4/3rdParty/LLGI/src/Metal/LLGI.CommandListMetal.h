#pragma once

#include "../LLGI.CommandList.h"
#import <MetalKit/MetalKit.h>

#include <memory>

namespace LLGI
{

class GraphicsMetal;
class IndexBuffer;

struct CommandListMetalPlatformRenderPassContext
{
	id<MTLRenderCommandEncoder> RenderEncoder = nullptr;
};

class CommandListMetal : public CommandList
{
	GraphicsMetal* graphics_ = nullptr;

	MTLSamplerDescriptor* samplers_[2][2][3];
	id<MTLSamplerState> samplerStates_[2][2][3];

	id<MTLCommandBuffer> commandBuffer_ = nullptr;
	id<MTLRenderCommandEncoder> renderEncoder_ = nullptr;
	id<MTLFence> fence_ = nullptr;
	bool isCompleted_ = true;

public:
	CommandListMetal(Graphics* graphics);
	~CommandListMetal() override;

	void Begin() override;
	void End() override;
	void SetScissor(int32_t x, int32_t y, int32_t width, int32_t height) override;
	void Draw(int32_t primitiveCount, int32_t instanceCount) override;
	void CopyTexture(Texture* src, Texture* dst) override;
	void GenerateMipMap(Texture* src) override;
	void BeginRenderPass(RenderPass* renderPass) override;
	void EndRenderPass() override;

	void WaitUntilCompleted() override;

	bool BeginWithPlatform(void* platformContextPtr) override;
	void EndWithPlatform() override;

	bool BeginRenderPassWithPlatformPtr(void* platformPtr) override;
	bool EndRenderPassWithPlatformPtr() override;

	bool GetIsCompleted() { return isCompleted_; }

	void ResetCompleted() { isCompleted_ = false; }

	id<MTLCommandBuffer>& GetCommandBuffer() { return commandBuffer_; }
	id<MTLRenderCommandEncoder>& GetRenderCommandEncorder() { return renderEncoder_; }
};

} // namespace LLGI

#pragma once

#include "../LLGI.CommandList.h"
#import <MetalKit/MetalKit.h>

namespace LLGI
{

struct CommandList_Impl;
class IndexBuffer;

class CommandListMetal : public CommandList
{
	CommandList_Impl* impl = nullptr;
	Graphics* graphics_ = nullptr;

	MTLSamplerDescriptor* samplers[2][2];
	id<MTLSamplerState> samplerStates[2][2];

public:
	CommandListMetal();
	virtual ~CommandListMetal();

	bool Initialize(Graphics* graphics);

	void Begin() override;
	void End() override;
	void SetScissor(int32_t x, int32_t y, int32_t width, int32_t height) override;
	void Draw(int32_t primitiveCount, int32_t instanceCount) override;
	void CopyTexture(Texture* src, Texture* dst) override;
	void BeginRenderPass(RenderPass* renderPass) override;
	void EndRenderPass() override;

	void WaitUntilCompleted() override;

	CommandList_Impl* GetImpl();
};

} // namespace LLGI

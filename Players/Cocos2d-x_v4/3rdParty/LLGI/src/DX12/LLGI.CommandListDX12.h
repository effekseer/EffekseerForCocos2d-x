
#pragma once

#include "../LLGI.CommandList.h"
#include "LLGI.BaseDX12.h"
#include "LLGI.DescriptorHeapDX12.h"
#include "LLGI.GraphicsDX12.h"
#include "LLGI.PipelineStateDX12.h"
#include "LLGI.RenderPassDX12.h"

namespace LLGI
{
class DescriptorHeapDX12;

struct PlatformContextDX12
{
	ID3D12GraphicsCommandList* commandList = nullptr;
};

class CommandListDX12 : public CommandList
{
private:
	std::shared_ptr<DescriptorHeapDX12> cbreDescriptorHeap_;
	std::shared_ptr<DescriptorHeapDX12> rtDescriptorHeap_;
	std::shared_ptr<DescriptorHeapDX12> dtDescriptorHeap_;
	std::shared_ptr<DescriptorHeapDX12> smpDescriptorHeap_;
	std::shared_ptr<ID3D12GraphicsCommandList> commandList_;
	std::shared_ptr<ID3D12CommandAllocator> commandAllocator_;
	ID3D12Fence* fence_ = nullptr;
	HANDLE fenceEvent_ = nullptr;
	UINT64 fenceValue_ = 1;

	std::shared_ptr<GraphicsDX12> graphics_;
	std::shared_ptr<RenderPassDX12> renderPass_;

	ID3D12GraphicsCommandList* currentCommandList_ = nullptr;

public:
	CommandListDX12();
	virtual ~CommandListDX12();
	bool Initialize(GraphicsDX12* graphics, int32_t drawingCount);

	void Begin() override;
	void End() override;
	bool BeginWithPlatform(void* platformContextPtr) override;
	void EndWithPlatform() override;

	void BeginRenderPass(RenderPass* renderPass) override;
	void EndRenderPass() override;
	void Draw(int32_t pritimiveCount) override;

	void Clear(const Color8& color);

	void ClearDepth();

	ID3D12GraphicsCommandList* GetCommandList() const;

	ID3D12Fence* GetFence() const;
	UINT64 GetAndIncFenceValue();

	void WaitUntilCompleted() override;
};

} // namespace LLGI

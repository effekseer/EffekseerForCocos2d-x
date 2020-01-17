#include "LLGI.DescriptorHeapDX12.h"

namespace LLGI
{

DescriptorHeapDX12::DescriptorHeapDX12(std::shared_ptr<GraphicsDX12> graphics, D3D12_DESCRIPTOR_HEAP_TYPE type, int size, int stage)
	: graphics_(graphics), type_(type), size_(size), stage_(stage)
{
	SafeAddRef(graphics);

	auto heap = CreateHeap(size_ * stage_);
	assert(heap != nullptr);
	descriptorHeaps_ = heap;
	cpuHandles_ = descriptorHeaps_->GetCPUDescriptorHandleForHeapStart();
	gpuHandles_ = descriptorHeaps_->GetGPUDescriptorHandleForHeapStart();
}

DescriptorHeapDX12::~DescriptorHeapDX12()
{
	SafeRelease(graphics_);
	SafeRelease(descriptorHeaps_);
}

void DescriptorHeapDX12::IncrementCpuHandle(int count)
{
	auto size = static_cast<int>(graphics_->GetDevice()->GetDescriptorHandleIncrementSize(type_));
	cpuHandles_.ptr += static_cast<size_t>(size * count);
}

void DescriptorHeapDX12::IncrementGpuHandle(int count)
{
	auto size = static_cast<int>(graphics_->GetDevice()->GetDescriptorHandleIncrementSize(type_));
	gpuHandles_.ptr += static_cast<size_t>(size * count);
}

ID3D12DescriptorHeap* DescriptorHeapDX12::GetHeap()
{ return descriptorHeaps_; }

D3D12_CPU_DESCRIPTOR_HANDLE DescriptorHeapDX12::GetCpuHandle()
{ return cpuHandles_; }

D3D12_GPU_DESCRIPTOR_HANDLE DescriptorHeapDX12::GetGpuHandle()
{
	return gpuHandles_;
}

void DescriptorHeapDX12::Reset()
{
	cpuHandles_ = descriptorHeaps_->GetCPUDescriptorHandleForHeapStart();
	gpuHandles_ = descriptorHeaps_->GetGPUDescriptorHandleForHeapStart();
}

ID3D12DescriptorHeap* DescriptorHeapDX12::CreateHeap(int numDescriptors)
{
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
	ID3D12DescriptorHeap* heap = nullptr;

	heapDesc.NumDescriptors = numDescriptors;
	heapDesc.Type = type_;
	heapDesc.Flags = (type_ <= D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER)
						 ? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE
						 : D3D12_DESCRIPTOR_HEAP_FLAG_NONE; // Only descriptor heaps for CBV, SRV, UAV, sampler can be shader visible.

	heapDesc.NodeMask = 1; // TODO: set properly for multi-adaptor.

	auto hr = graphics_->GetDevice()->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&heap));
	if (FAILED(hr))
	{
		SafeRelease(heap);
		return nullptr;
	}
	return heap;
}

} // namespace LLGI

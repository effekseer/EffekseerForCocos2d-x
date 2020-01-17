#pragma once

#include "LLGI.BaseDX12.h"
#include "LLGI.GraphicsDX12.h"

namespace LLGI
{
class DescriptorHeapDX12
{
private:
	std::shared_ptr<GraphicsDX12> graphics_;
	int size_ = 0;
	int stage_ = 0;
	int offset_ = 0;
	D3D12_DESCRIPTOR_HEAP_TYPE type_ = D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER;

	ID3D12DescriptorHeap* descriptorHeaps_ = nullptr;
	D3D12_CPU_DESCRIPTOR_HANDLE cpuHandles_;
	D3D12_GPU_DESCRIPTOR_HANDLE gpuHandles_;

	ID3D12DescriptorHeap* CreateHeap(int numDescriptors);

public:
	DescriptorHeapDX12(std::shared_ptr<GraphicsDX12> graphics, D3D12_DESCRIPTOR_HEAP_TYPE type, int size, int stage);
	virtual ~DescriptorHeapDX12();

	void IncrementCpuHandle(int count);
	void IncrementGpuHandle(int count);
	ID3D12DescriptorHeap* GetHeap();
	D3D12_CPU_DESCRIPTOR_HANDLE GetCpuHandle();
	D3D12_GPU_DESCRIPTOR_HANDLE GetGpuHandle();

	void Reset();
};

} // namespace LLGI


#pragma once

#include "../LLGI.VertexBuffer.h"
#include "LLGI.BaseVulkan.h"
#include "LLGI.GraphicsVulkan.h"

namespace LLGI
{
class SingleFrameMemoryPoolVulkan;

class VertexBufferVulkan : public VertexBuffer
{
private:
	std::shared_ptr<GraphicsVulkan> graphics_;
	std::unique_ptr<Buffer> cpuBuf;
	std::unique_ptr<Buffer> gpuBuf;
	void* data = nullptr;
	int32_t memSize = 0;

public:
	bool Initialize(GraphicsVulkan* graphics, int32_t size);
    bool InitializeAsShortTime(SingleFrameMemoryPoolVulkan* memoryPool, int32_t size);

	VertexBufferVulkan();
	virtual ~VertexBufferVulkan();

	void* Lock() override;
	void* Lock(int32_t offset, int32_t size) override;
	void Unlock() override;
	int32_t GetSize() override;

	vk::Buffer GetBuffer() { return gpuBuf->buffer(); }
};

} // namespace LLGI

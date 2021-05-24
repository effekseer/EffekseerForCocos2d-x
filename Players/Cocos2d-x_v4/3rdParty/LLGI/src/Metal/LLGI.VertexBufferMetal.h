#pragma once

#include "../LLGI.VertexBuffer.h"

namespace LLGI
{

class BufferMetal;

class VertexBufferMetal : public VertexBuffer
{
private:
	BufferMetal* buffer_ = nullptr;

public:
	VertexBufferMetal(Graphics* graphics, int32_t size);
	~VertexBufferMetal() override;
	void* Lock() override;
	void* Lock(int32_t offset, int32_t size) override;
	void Unlock() override;
	int32_t GetSize() override;
	BufferMetal& GetBuffer() { return *buffer_; }
};

} // namespace LLGI

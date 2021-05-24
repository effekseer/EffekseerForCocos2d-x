#pragma once

#include "../LLGI.IndexBuffer.h"
#include "LLGI.BufferMetal.h"

namespace LLGI
{

struct Buffer_Impl;

class IndexBufferMetal : public IndexBuffer
{
private:
	BufferMetal* buffer_ = nullptr;
	int32_t stride_ = 0;
	int32_t count_ = 0;

public:
	IndexBufferMetal(Graphics* graphics, int32_t stride, int32_t count);
	~IndexBufferMetal() override;
	void* Lock() override;
	void* Lock(int32_t offset, int32_t size) override;
	void Unlock() override;
	int32_t GetStride() override;
	int32_t GetCount() override;
	BufferMetal& GetBuffer() { return *buffer_; }
};

} // namespace LLGI

#pragma once

#include "../LLGI.Graphics.h"
#import <MetalKit/MetalKit.h>
#include <functional>
#include <memory>
#include <unordered_map>

namespace LLGI
{

class BufferMetal : public ReferenceObject
{
private:
	id<MTLBuffer> buffer_ = nullptr;

public:
	BufferMetal(Graphics* graphics, int32_t size);
	~BufferMetal() override;

	void* GetData() { return buffer_.contents; }

	id<MTLBuffer>& GetBuffer() { return buffer_; }

	int32_t GetSize() const { return buffer_.length; }
};

} // namespace LLGI

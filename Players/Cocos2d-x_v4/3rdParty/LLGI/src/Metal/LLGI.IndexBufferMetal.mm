#include "LLGI.IndexBufferMetal.h"
#include "LLGI.GraphicsMetal.h"
#include "LLGI.Metal_Impl.h"

#import <MetalKit/MetalKit.h>

namespace LLGI
{

IndexBufferMetal::IndexBufferMetal(Graphics* graphics, int32_t stride, int32_t count)
{
	buffer_ = new BufferMetal(graphics, stride * count);
	stride_ = stride;
	count_ = count;
}

IndexBufferMetal::~IndexBufferMetal() { SafeRelease(buffer_); }

void* IndexBufferMetal::Lock() { return buffer_->GetData(); }

void* IndexBufferMetal::Lock(int32_t offset, int32_t size)
{
	NSCAssert(0 <= offset && offset + size <= buffer_->GetSize(), @"Run off the buffer");
	auto buffer = static_cast<uint8_t*>(buffer_->GetData());
	buffer += offset;
	return buffer;
}

void IndexBufferMetal::Unlock() {}

int32_t IndexBufferMetal::GetStride() { return stride_; }

int32_t IndexBufferMetal::GetCount() { return count_; }

}

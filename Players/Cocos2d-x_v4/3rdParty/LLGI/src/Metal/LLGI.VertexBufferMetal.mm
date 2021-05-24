#include "LLGI.VertexBufferMetal.h"
#include "LLGI.BufferMetal.h"
#include "LLGI.GraphicsMetal.h"
#include "LLGI.Metal_Impl.h"

#import <MetalKit/MetalKit.h>

namespace LLGI
{

VertexBufferMetal::VertexBufferMetal(Graphics* graphics, int32_t size) { buffer_ = new BufferMetal(graphics, size); }

VertexBufferMetal::~VertexBufferMetal() { SafeRelease(buffer_); }

void* VertexBufferMetal::Lock() { return buffer_->GetData(); }

void* VertexBufferMetal::Lock(int32_t offset, int32_t size)
{
	NSCAssert(0 <= offset && offset + size <= buffer_->GetSize(), @"Run off the buffer");
	auto buffer = static_cast<uint8_t*>(buffer_->GetData());
	buffer += offset;
	return buffer;
}

void VertexBufferMetal::Unlock() {}

int32_t VertexBufferMetal::GetSize() { return buffer_->GetSize(); }

}

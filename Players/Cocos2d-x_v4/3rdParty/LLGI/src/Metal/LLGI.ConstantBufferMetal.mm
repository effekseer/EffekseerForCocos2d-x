#include "LLGI.ConstantBufferMetal.h"
#include "LLGI.GraphicsMetal.h"
#include "LLGI.Metal_Impl.h"

#import <MetalKit/MetalKit.h>

namespace LLGI
{

ConstantBufferMetal::ConstantBufferMetal() {}

ConstantBufferMetal::~ConstantBufferMetal() { SafeRelease(buffer_); }

bool ConstantBufferMetal::Initialize(Graphics* graphics, int32_t size)
{
	auto buffer = new BufferMetal(graphics, size);
	{
		SafeAssign(buffer_, buffer);
		SafeRelease(buffer);
		size_ = size;
		offset_ = 0;
		return true;
	}

	SafeRelease(buffer);
	return false;
}

bool ConstantBufferMetal::InitializeAsShortTime(BufferMetal* buffer, int32_t offset, int32_t size)
{
	SafeAssign(buffer_, buffer);
	size_ = size;
	offset_ = offset;
	return true;
}

void* ConstantBufferMetal::Lock()
{
	auto buffer = static_cast<uint8_t*>(buffer_->GetData());
	buffer += offset_;
	return buffer;
}

void* ConstantBufferMetal::Lock(int32_t offset, int32_t size)
{
	NSCAssert(0 <= offset && offset + offset_ + size <= buffer_->GetSize(), @"Run off the buffer");

	auto buffer = static_cast<uint8_t*>(buffer_->GetData());
	buffer += offset + offset_;
	return buffer;
}

void ConstantBufferMetal::Unlock() {}

int32_t ConstantBufferMetal::GetSize() { return buffer_->GetSize(); }

}

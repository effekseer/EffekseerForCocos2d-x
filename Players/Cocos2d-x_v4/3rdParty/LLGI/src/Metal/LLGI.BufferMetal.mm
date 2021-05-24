#include "LLGI.BufferMetal.h"
#include "LLGI.CommandListMetal.h"
#include "LLGI.ConstantBufferMetal.h"
#include "LLGI.GraphicsMetal.h"
#include "LLGI.IndexBufferMetal.h"
#include "LLGI.Metal_Impl.h"
#include "LLGI.PipelineStateMetal.h"
#include "LLGI.RenderPassMetal.h"
#include "LLGI.ShaderMetal.h"
#include "LLGI.SingleFrameMemoryPoolMetal.h"
#include "LLGI.TextureMetal.h"
#include "LLGI.VertexBufferMetal.h"

#import <MetalKit/MetalKit.h>

namespace LLGI
{

BufferMetal::BufferMetal(Graphics* graphics, int32_t size)
{
	auto g = static_cast<GraphicsMetal*>(graphics);
	buffer_ = [g->GetDevice() newBufferWithLength:size options:MTLResourceOptionCPUCacheModeDefault];
}

BufferMetal::~BufferMetal()
{
	if (buffer_ != nullptr)
	{
		[buffer_ release];
		buffer_ = nullptr;
	}
}

}

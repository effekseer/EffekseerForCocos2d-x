#include "LLGI.PipelineStateMetal.h"
#include "LLGI.GraphicsMetal.h"
#include "LLGI.Metal_Impl.h"
#include "LLGI.ShaderMetal.h"
#include "LLGI.RenderPassMetal.h"

#import <MetalKit/MetalKit.h>

namespace LLGI
{

PipelineState_Impl::PipelineState_Impl() {}

PipelineState_Impl::~PipelineState_Impl()
{
	if (pipelineStateDescriptor != nullptr)
	{
		[pipelineStateDescriptor release];
		pipelineStateDescriptor = nullptr;
	}

	if (depthStencilState != nullptr)
	{
		[depthStencilState release];
		depthStencilState = nullptr;
	}

	if (pipelineState != nullptr)
	{
		[pipelineState release];
		pipelineState = nullptr;
	}
}

void PipelineState_Impl::Compile(PipelineState* self, Graphics_Impl* graphics)
{
	auto self_ = static_cast<PipelineStateMetal*>(self);
	auto renderPassPipelineStateMetal_ = static_cast<RenderPassPipelineStateMetal*>(self_->GetRenderPassPipelineState());

	pipelineStateDescriptor = [[MTLRenderPipelineDescriptor alloc] init];

	// vertex layout
	MTLVertexDescriptor* vertexDescriptor = [MTLVertexDescriptor vertexDescriptor];

	int vertexOffset = 0;
	for (int i = 0; i < self_->VertexLayoutCount; i++)
	{
		vertexDescriptor.attributes[i].offset = vertexOffset;

		if (self_->VertexLayouts[i] == VertexLayoutFormat::R32G32B32_FLOAT)
		{
			vertexDescriptor.attributes[i].format = MTLVertexFormatFloat3;
			vertexDescriptor.attributes[i].bufferIndex = VertexBufferIndex;
			vertexOffset += sizeof(float) * 3;
		}

        if (self_->VertexLayouts[i] == VertexLayoutFormat::R32G32B32A32_FLOAT)
        {
            vertexDescriptor.attributes[i].format = MTLVertexFormatFloat4;
            vertexDescriptor.attributes[i].bufferIndex = VertexBufferIndex;
            vertexOffset += sizeof(float) * 4;
        }

		if (self_->VertexLayouts[i] == VertexLayoutFormat::R32G32_FLOAT)
		{
			vertexDescriptor.attributes[i].format = MTLVertexFormatFloat2;
			vertexDescriptor.attributes[i].bufferIndex = VertexBufferIndex;
			vertexOffset += sizeof(float) * 2;
		}

		if (self_->VertexLayouts[i] == VertexLayoutFormat::R8G8B8A8_UINT)
		{
			vertexDescriptor.attributes[i].format = MTLVertexFormatUChar4;
			vertexDescriptor.attributes[i].bufferIndex = VertexBufferIndex;
			vertexOffset += sizeof(float);
		}

		if (self_->VertexLayouts[i] == VertexLayoutFormat::R8G8B8A8_UNORM)
		{
			vertexDescriptor.attributes[i].format = MTLVertexFormatUChar4Normalized;
			vertexDescriptor.attributes[i].bufferIndex = VertexBufferIndex;
			vertexOffset += sizeof(float);
		}
	}

	vertexDescriptor.layouts[VertexBufferIndex].stepRate = 1;
	vertexDescriptor.layouts[VertexBufferIndex].stepFunction = MTLVertexStepFunctionPerVertex;
	vertexDescriptor.layouts[VertexBufferIndex].stride = vertexOffset;

	pipelineStateDescriptor.vertexDescriptor = vertexDescriptor;

	// setup shaders
	auto vs = static_cast<ShaderMetal*>(self_->GetShaders()[static_cast<int>(ShaderStageType::Vertex)]);
	auto ps = static_cast<ShaderMetal*>(self_->GetShaders()[static_cast<int>(ShaderStageType::Pixel)]);

	id<MTLFunction> vf = [vs->GetImpl()->library newFunctionWithName:@"main0"];
	id<MTLFunction> pf = [ps->GetImpl()->library newFunctionWithName:@"main0"];
	pipelineStateDescriptor.vertexFunction = vf;
	pipelineStateDescriptor.fragmentFunction = pf;

	// setup a depth
	MTLDepthStencilDescriptor* depthStencilDescriptor = [[MTLDepthStencilDescriptor alloc] init];
	depthStencilDescriptor.depthWriteEnabled = self_->IsDepthWriteEnabled;

	if (self_->IsDepthTestEnabled)
	{
		std::array<MTLCompareFunction, 10> depthCompareOps;
		depthCompareOps[static_cast<int>(DepthFuncType::Never)] = MTLCompareFunctionNever;
		depthCompareOps[static_cast<int>(DepthFuncType::Less)] = MTLCompareFunctionLess;
		depthCompareOps[static_cast<int>(DepthFuncType::Equal)] = MTLCompareFunctionEqual;
		depthCompareOps[static_cast<int>(DepthFuncType::LessEqual)] = MTLCompareFunctionLessEqual;
		depthCompareOps[static_cast<int>(DepthFuncType::Greater)] = MTLCompareFunctionGreater;
		depthCompareOps[static_cast<int>(DepthFuncType::NotEqual)] = MTLCompareFunctionNotEqual;
		depthCompareOps[static_cast<int>(DepthFuncType::GreaterEqual)] = MTLCompareFunctionGreaterEqual;
		depthCompareOps[static_cast<int>(DepthFuncType::Always)] = MTLCompareFunctionAlways;
		depthStencilDescriptor.depthCompareFunction = depthCompareOps[static_cast<int>(self_->DepthFunc)];
	}
	else
	{
		depthStencilDescriptor.depthCompareFunction = MTLCompareFunctionAlways;
	}
	
	
	if (renderPassPipelineStateMetal_->GetImpl()->depthStencilFormat != MTLPixelFormatInvalid)
	{
		if (self_->IsStencilTestEnabled)
		{
			depthStencilDescriptor.frontFaceStencil.depthFailureOperation = MTLStencilOperationKeep;
			depthStencilDescriptor.frontFaceStencil.stencilFailureOperation = MTLStencilOperationKeep;
			depthStencilDescriptor.frontFaceStencil.depthStencilPassOperation = MTLStencilOperationKeep;
			depthStencilDescriptor.frontFaceStencil.stencilCompareFunction = MTLCompareFunctionEqual;
			depthStencilDescriptor.frontFaceStencil.readMask = 0xFF;
			depthStencilDescriptor.frontFaceStencil.writeMask = 0xFF;
		}
		else
		{
			// always write to stencil reference value
			depthStencilDescriptor.frontFaceStencil.depthFailureOperation = MTLStencilOperationKeep;
			depthStencilDescriptor.frontFaceStencil.stencilFailureOperation = MTLStencilOperationKeep;
			depthStencilDescriptor.frontFaceStencil.depthStencilPassOperation = MTLStencilOperationReplace;
			depthStencilDescriptor.frontFaceStencil.stencilCompareFunction = MTLCompareFunctionAlways;
			depthStencilDescriptor.frontFaceStencil.readMask = 0xFF;
			depthStencilDescriptor.frontFaceStencil.writeMask = 0xFF;
		}
	}

	depthStencilState = [graphics->device newDepthStencilStateWithDescriptor:depthStencilDescriptor];
	[depthStencilDescriptor release];

	// topology
	if (self_->Topology == TopologyType::Triangle)
	{
		pipelineStateDescriptor.inputPrimitiveTopology = MTLPrimitiveTopologyClassTriangle;
	}
	else if (self_->Topology == TopologyType::Line)
	{
		pipelineStateDescriptor.inputPrimitiveTopology = MTLPrimitiveTopologyClassLine;
	}
	else
	{
		assert(0);
	}

	// clulling (on commandlist)

	// setup a blend

	MTLRenderPipelineColorAttachmentDescriptor* colorAttachment = pipelineStateDescriptor.colorAttachments[0];

	if (self_->IsBlendEnabled)
	{
		colorAttachment.blendingEnabled = true;

		std::array<MTLBlendOperation, 10> blendOps;
		blendOps[static_cast<int>(BlendEquationType::Add)] = MTLBlendOperationAdd;
		blendOps[static_cast<int>(BlendEquationType::Sub)] = MTLBlendOperationSubtract;
		blendOps[static_cast<int>(BlendEquationType::ReverseSub)] = MTLBlendOperationReverseSubtract;
		blendOps[static_cast<int>(BlendEquationType::Min)] = MTLBlendOperationMin;
		blendOps[static_cast<int>(BlendEquationType::Max)] = MTLBlendOperationMax;

		std::array<MTLBlendFactor, 20> blendFuncs;
		blendFuncs[static_cast<int>(BlendFuncType::Zero)] = MTLBlendFactorZero;
		blendFuncs[static_cast<int>(BlendFuncType::One)] = MTLBlendFactorOne;
		blendFuncs[static_cast<int>(BlendFuncType::SrcColor)] = MTLBlendFactorSourceColor;
		blendFuncs[static_cast<int>(BlendFuncType::OneMinusSrcColor)] = MTLBlendFactorOneMinusSourceColor;
		blendFuncs[static_cast<int>(BlendFuncType::SrcAlpha)] = MTLBlendFactorSourceAlpha;
		blendFuncs[static_cast<int>(BlendFuncType::OneMinusSrcAlpha)] = MTLBlendFactorSourceAlpha;
		blendFuncs[static_cast<int>(BlendFuncType::DstColor)] = MTLBlendFactorDestinationColor;
		blendFuncs[static_cast<int>(BlendFuncType::OneMinusDstColor)] = MTLBlendFactorOneMinusDestinationColor;
		blendFuncs[static_cast<int>(BlendFuncType::DstAlpha)] = MTLBlendFactorDestinationAlpha;
		blendFuncs[static_cast<int>(BlendFuncType::OneMinusDstAlpha)] = MTLBlendFactorDestinationAlpha;

		colorAttachment.sourceRGBBlendFactor = blendFuncs[static_cast<int>(self_->BlendSrcFunc)];
		colorAttachment.destinationRGBBlendFactor = blendFuncs[static_cast<int>(self_->BlendDstFunc)];
		colorAttachment.sourceAlphaBlendFactor = blendFuncs[static_cast<int>(self_->BlendSrcFuncAlpha)];
		colorAttachment.destinationAlphaBlendFactor = blendFuncs[static_cast<int>(self_->BlendDstFuncAlpha)];
		colorAttachment.rgbBlendOperation = blendOps[static_cast<int>(self_->BlendEquationRGB)];
		colorAttachment.alphaBlendOperation = blendOps[static_cast<int>(self_->BlendEquationAlpha)];
	}
	else
	{
		colorAttachment.blendingEnabled = false;
	}

	[pipelineStateDescriptor.colorAttachments objectAtIndexedSubscript:0].pixelFormat =
		renderPassPipelineStateMetal_->GetImpl()->pixelFormat;
	if (renderPassPipelineStateMetal_->GetImpl()->depthStencilFormat != MTLPixelFormatInvalid)
	{
		pipelineStateDescriptor.depthAttachmentPixelFormat = renderPassPipelineStateMetal_->GetImpl()->depthStencilFormat ;
		pipelineStateDescriptor.stencilAttachmentPixelFormat = renderPassPipelineStateMetal_->GetImpl()->depthStencilFormat ;
	}
	
	if (self_->IsMSAA)
	{
		pipelineStateDescriptor.sampleCount = graphics->maxMultiSamplingCount;
	}

	NSError* pipelineError = nil;
	pipelineState = [graphics->device newRenderPipelineStateWithDescriptor:pipelineStateDescriptor error:&pipelineError];
}

PipelineStateMetal::PipelineStateMetal()
{
	impl = new PipelineState_Impl();
	shaders.fill(nullptr);
}

PipelineStateMetal::~PipelineStateMetal()
{
	SafeDelete(impl);

	for (auto& shader : shaders)
	{
		SafeRelease(shader);
	}

	SafeRelease(graphics_);
}

bool PipelineStateMetal::Initialize(GraphicsMetal* graphics)
{
	SafeAddRef(graphics);
	SafeRelease(graphics_);
	graphics_ = graphics;

	return true;
}

void PipelineStateMetal::SetShader(ShaderStageType stage, Shader* shader)
{
	SafeAddRef(shader);
	SafeRelease(shaders[static_cast<int>(stage)]);
	shaders[static_cast<int>(stage)] = shader;
}

void PipelineStateMetal::Compile() { impl->Compile(this, graphics_->GetImpl()); }

}

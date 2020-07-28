
#pragma once

#include "LLGI.Base.h"

namespace LLGI
{

class PipelineState : public ReferenceObject
{
protected:
	std::shared_ptr<RenderPassPipelineState> renderPassPipelineState_ = nullptr;

public:
	PipelineState();
	~PipelineState() override = default;

	CullingMode Culling = CullingMode::Clockwise;
	TopologyType Topology = TopologyType::Triangle;

	bool IsBlendEnabled = true;

	BlendFuncType BlendSrcFunc = BlendFuncType::SrcAlpha;
	BlendFuncType BlendDstFunc = BlendFuncType::OneMinusSrcAlpha;
	BlendFuncType BlendSrcFuncAlpha = BlendFuncType::SrcAlpha;
	BlendFuncType BlendDstFuncAlpha = BlendFuncType::OneMinusSrcAlpha;

	BlendEquationType BlendEquationRGB = BlendEquationType::Add;
	BlendEquationType BlendEquationAlpha = BlendEquationType::Add;

	bool IsDepthTestEnabled = false;
	bool IsDepthWriteEnabled = false;
	bool IsStencilTestEnabled = false;
	DepthFuncType DepthFunc = DepthFuncType::Less;

	std::array<std::string, VertexLayoutMax> VertexLayoutNames;
	std::array<VertexLayoutFormat, VertexLayoutMax> VertexLayouts;

	//! only for DirectX12
	std::array<int32_t, VertexLayoutMax> VertexLayoutSemantics;
	int32_t VertexLayoutCount = 0;

	virtual void SetShader(ShaderStageType stage, Shader* shader);

	virtual void SetRenderPassPipelineState(RenderPassPipelineState* renderPassPipelineState);

	virtual bool Compile();
};

} // namespace LLGI

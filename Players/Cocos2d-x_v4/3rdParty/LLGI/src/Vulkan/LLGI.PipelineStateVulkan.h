
#pragma once

#include "../LLGI.PipelineState.h"
#include "LLGI.BaseVulkan.h"
#include "LLGI.GraphicsVulkan.h"

namespace LLGI
{

class PipelineStateVulkan : public PipelineState
{
private:
	GraphicsVulkan* graphics_ = nullptr;
	std::array<Shader*, static_cast<int>(ShaderStageType::Max)> shaders;

	vk::Pipeline pipeline_ = nullptr;
	vk::PipelineLayout pipelineLayout_ = nullptr;
	std::array<vk::DescriptorSetLayout, 2> descriptorSetLayouts;

public:
	PipelineStateVulkan();
	virtual ~PipelineStateVulkan();

	bool Initialize(GraphicsVulkan* graphics);

	void SetShader(ShaderStageType stage, Shader* shader) override;
	void Compile() override;

	vk::Pipeline GetPipeline() const { return pipeline_; }

	vk::PipelineLayout GetPipelineLayout() const { return pipelineLayout_; }

	const std::array<vk::DescriptorSetLayout, 2>& GetDescriptorSetLayout() const { return descriptorSetLayouts; }
};

} // namespace LLGI

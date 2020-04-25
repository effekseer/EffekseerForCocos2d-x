#include "LLGI.PipelineStateVulkan.h"
#include "LLGI.ShaderVulkan.h"

// for x11
#undef Always

namespace LLGI
{

PipelineStateVulkan::PipelineStateVulkan()
{
	shaders.fill(0);
	for (auto i = 0; i < descriptorSetLayouts.size(); i++)
	{
		descriptorSetLayouts[i] = nullptr;
	}
}

PipelineStateVulkan ::~PipelineStateVulkan()
{
	for (auto& shader : shaders)
	{
		SafeRelease(shader);
	}

	for (auto i = 0; i < descriptorSetLayouts.size(); i++)
	{
		graphics_->GetDevice().destroyDescriptorSetLayout(descriptorSetLayouts[i]);
		descriptorSetLayouts[i] = nullptr;
	}

	if (pipelineLayout_)
	{
		graphics_->GetDevice().destroyPipelineLayout(pipelineLayout_);
		pipelineLayout_ = nullptr;
	}

	if (pipeline_)
	{
		graphics_->GetDevice().destroyPipeline(pipeline_);
		pipeline_ = nullptr;
	}

	SafeRelease(graphics_);
}

bool PipelineStateVulkan::Initialize(GraphicsVulkan* graphics)
{
	SafeRelease(graphics_);
	SafeAddRef(graphics);
	graphics_ = graphics;
	return true;
}

void PipelineStateVulkan::SetShader(ShaderStageType stage, Shader* shader)
{

	SafeAddRef(shader);
	SafeRelease(shaders[static_cast<int>(stage)]);
	shaders[static_cast<int>(stage)] = shader;
}

void PipelineStateVulkan::Compile()
{
	vk::GraphicsPipelineCreateInfo graphicsPipelineInfo;

	std::vector<vk::PipelineShaderStageCreateInfo> shaderStageInfos;

	// setup shaders
	std::string mainName = "main";

	for (size_t i = 0; i < this->shaders.size(); i++)
	{
		auto shader = static_cast<ShaderVulkan*>(shaders[i]);

		vk::PipelineShaderStageCreateInfo info;

		if (i == static_cast<int>(ShaderStageType::Vertex))
			info.stage = vk::ShaderStageFlagBits::eVertex;

		if (i == static_cast<int>(ShaderStageType::Pixel))
			info.stage = vk::ShaderStageFlagBits::eFragment;

		info.module = shader->GetShaderModule();
		info.pName = mainName.c_str();
		shaderStageInfos.push_back(info);
	}

	graphicsPipelineInfo.pStages = shaderStageInfos.data();
	graphicsPipelineInfo.stageCount = shaderStageInfos.size();

	// setup layouts
	std::vector<vk::VertexInputBindingDescription> bindDescs;
	std::vector<vk::VertexInputAttributeDescription> attribDescs;

	int vertexOffset = 0;
	for (int i = 0; i < VertexLayoutCount; i++)
	{
		vk::VertexInputAttributeDescription attribDesc;

		attribDesc.binding = 0;
		attribDesc.location = i;
		attribDesc.offset = vertexOffset;

		if (VertexLayouts[i] == VertexLayoutFormat::R32G32B32_FLOAT)
		{
			attribDesc.format = vk::Format::eR32G32B32Sfloat;
			vertexOffset += sizeof(float) * 3;
		}

		if (VertexLayouts[i] == VertexLayoutFormat::R32G32_FLOAT)
		{
			attribDesc.format = vk::Format::eR32G32Sfloat;
			vertexOffset += sizeof(float) * 2;
		}

		if (VertexLayouts[i] == VertexLayoutFormat::R8G8B8A8_UINT)
		{
			attribDesc.format = vk::Format::eR8G8B8A8Uint;
			vertexOffset += sizeof(float);
		}

		if (VertexLayouts[i] == VertexLayoutFormat::R8G8B8A8_UNORM)
		{
			attribDesc.format = vk::Format::eR8G8B8A8Unorm;
			vertexOffset += sizeof(float);
		}

		attribDescs.push_back(attribDesc);
	}

	vk::VertexInputBindingDescription bindDesc;
	bindDesc.binding = 0;
	bindDesc.stride = vertexOffset;
	bindDesc.inputRate = vk::VertexInputRate::eVertex;
	bindDescs.push_back(bindDesc);

	vk::PipelineVertexInputStateCreateInfo inputStateInfo;
	inputStateInfo.pVertexBindingDescriptions = bindDescs.data();
	inputStateInfo.pVertexAttributeDescriptions = attribDescs.data();
	inputStateInfo.vertexBindingDescriptionCount = bindDescs.size();
	inputStateInfo.vertexAttributeDescriptionCount = attribDescs.size();
	graphicsPipelineInfo.pVertexInputState = &inputStateInfo;

	// setup a topology
	vk::PipelineInputAssemblyStateCreateInfo inputAssemblyStateInfo;
	if (Topology == TopologyType::Triangle)
	{
		inputAssemblyStateInfo.topology = vk::PrimitiveTopology::eTriangleList;
	}
	else if (Topology == TopologyType::Line)
	{
		inputAssemblyStateInfo.topology = vk::PrimitiveTopology::eLineList;
	}
	else
	{
		assert(0);
	}
	inputAssemblyStateInfo.primitiveRestartEnable = false;

	graphicsPipelineInfo.pInputAssemblyState = &inputAssemblyStateInfo;

	// setup a viewport
	vk::PipelineViewportStateCreateInfo viewportStateInfo;

	vk::Viewport viewport;
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = (float)1280; // TODO : temp
	viewport.height = (float)720; // TODO : temp
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;

	viewportStateInfo.pViewports = &viewport;
	viewportStateInfo.viewportCount = 1;

	vk::Rect2D scissor = {};
	scissor.offset = vk::Offset2D(0, 0);
	scissor.extent.width = 1280; // TODO : temp
	scissor.extent.height = 720; // TODO : temp

	viewportStateInfo.pScissors = &scissor;
	viewportStateInfo.scissorCount = 1;

	graphicsPipelineInfo.pViewportState = &viewportStateInfo;

	// setup a rasterlizer
	vk::PipelineRasterizationStateCreateInfo rasterizationState;
	rasterizationState.depthClampEnable = false;
	rasterizationState.rasterizerDiscardEnable = false;
	rasterizationState.polygonMode = vk::PolygonMode::eFill;

	if (Culling == CullingMode::Clockwise)
	{
		rasterizationState.cullMode = vk::CullModeFlagBits::eBack;
	}
	else if (Culling == CullingMode::CounterClockwise)
	{
		rasterizationState.cullMode = vk::CullModeFlagBits::eFront;
	}
	else if (Culling == CullingMode::DoubleSide)
	{
		rasterizationState.cullMode = vk::CullModeFlagBits::eNone;
	}

	rasterizationState.frontFace = vk::FrontFace::eClockwise;

	rasterizationState.depthBiasEnable = false;
	rasterizationState.depthBiasConstantFactor = 0.0f;
	rasterizationState.depthBiasClamp = 0.0f;
	rasterizationState.depthBiasSlopeFactor = 0.0f;
    rasterizationState.lineWidth = 1.0f;    // disable lineWidth. (Must not be zero)

	graphicsPipelineInfo.pRasterizationState = &rasterizationState;

	// setup a multisampling
	vk::PipelineMultisampleStateCreateInfo multisampleStateInfo;
	multisampleStateInfo.sampleShadingEnable = false;
	multisampleStateInfo.rasterizationSamples = vk::SampleCountFlagBits::e1;
	multisampleStateInfo.minSampleShading = 1.0f;
	multisampleStateInfo.pSampleMask = nullptr;
	multisampleStateInfo.alphaToCoverageEnable = false;
	multisampleStateInfo.alphaToOneEnable = false;

	graphicsPipelineInfo.pMultisampleState = &multisampleStateInfo;

	// setup a depthstencil
	vk::PipelineDepthStencilStateCreateInfo depthStencilInfo;

	depthStencilInfo.depthTestEnable = IsDepthTestEnabled;
	depthStencilInfo.depthWriteEnable = IsDepthWriteEnabled;
	depthStencilInfo.stencilTestEnable = IsStencilTestEnabled;

	std::array<vk::CompareOp, 10> depthCompareOps;
	depthCompareOps[static_cast<int>(DepthFuncType::Never)] = vk::CompareOp::eNever;
	depthCompareOps[static_cast<int>(DepthFuncType::Less)] = vk::CompareOp::eLess;
	depthCompareOps[static_cast<int>(DepthFuncType::Equal)] = vk::CompareOp::eEqual;
	depthCompareOps[static_cast<int>(DepthFuncType::LessEqual)] = vk::CompareOp::eLessOrEqual;
	depthCompareOps[static_cast<int>(DepthFuncType::Greater)] = vk::CompareOp::eGreater;
	depthCompareOps[static_cast<int>(DepthFuncType::NotEqual)] = vk::CompareOp::eNotEqual;
	depthCompareOps[static_cast<int>(DepthFuncType::GreaterEqual)] = vk::CompareOp::eGreaterOrEqual;
	depthCompareOps[static_cast<int>(DepthFuncType::Always)] = vk::CompareOp::eAlways;

	depthStencilInfo.depthCompareOp = depthCompareOps[static_cast<int>(DepthFunc)];

	graphicsPipelineInfo.pDepthStencilState = &depthStencilInfo;

	// blending
	vk::PipelineColorBlendAttachmentState blendInfo;
	blendInfo.colorWriteMask =
		vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA;

	if (IsBlendEnabled)
	{
		blendInfo.blendEnable = true;

		std::array<vk::BlendOp, 10> blendOps;
		blendOps[static_cast<int>(BlendEquationType::Add)] = vk::BlendOp::eAdd;
		blendOps[static_cast<int>(BlendEquationType::Sub)] = vk::BlendOp::eSubtract;
		blendOps[static_cast<int>(BlendEquationType::ReverseSub)] = vk::BlendOp::eReverseSubtract;
		blendOps[static_cast<int>(BlendEquationType::Min)] = vk::BlendOp::eMin;
		blendOps[static_cast<int>(BlendEquationType::Max)] = vk::BlendOp::eMax;

		std::array<vk::BlendFactor, 20> blendFuncs;
		blendFuncs[static_cast<int>(BlendFuncType::Zero)] = vk::BlendFactor::eZero;
		blendFuncs[static_cast<int>(BlendFuncType::One)] = vk::BlendFactor::eOne;
		blendFuncs[static_cast<int>(BlendFuncType::SrcColor)] = vk::BlendFactor::eSrcColor;
		blendFuncs[static_cast<int>(BlendFuncType::OneMinusSrcColor)] = vk::BlendFactor::eOneMinusSrcColor;
		blendFuncs[static_cast<int>(BlendFuncType::SrcAlpha)] = vk::BlendFactor::eSrcAlpha;
		blendFuncs[static_cast<int>(BlendFuncType::OneMinusSrcAlpha)] = vk::BlendFactor::eSrcAlpha;
		blendFuncs[static_cast<int>(BlendFuncType::DstColor)] = vk::BlendFactor::eDstColor;
		blendFuncs[static_cast<int>(BlendFuncType::OneMinusDstColor)] = vk::BlendFactor::eOneMinusDstColor;
		blendFuncs[static_cast<int>(BlendFuncType::DstAlpha)] = vk::BlendFactor::eDstAlpha;
		blendFuncs[static_cast<int>(BlendFuncType::OneMinusDstAlpha)] = vk::BlendFactor::eDstAlpha;

		blendInfo.srcColorBlendFactor = blendFuncs[static_cast<int>(BlendSrcFunc)];
		blendInfo.dstColorBlendFactor = blendFuncs[static_cast<int>(BlendDstFunc)];
		blendInfo.srcAlphaBlendFactor = blendFuncs[static_cast<int>(BlendSrcFuncAlpha)];
		blendInfo.dstAlphaBlendFactor = blendFuncs[static_cast<int>(BlendDstFuncAlpha)];
		blendInfo.colorBlendOp = blendOps[static_cast<int>(BlendEquationRGB)];
		blendInfo.alphaBlendOp = blendOps[static_cast<int>(BlendEquationAlpha)];
	}
	else
	{
		blendInfo.blendEnable = false;
	}

	vk::PipelineColorBlendStateCreateInfo colorBlendInfo;
	colorBlendInfo.logicOpEnable = VK_FALSE;
	colorBlendInfo.logicOp = vk::LogicOp::eCopy;
	colorBlendInfo.attachmentCount = 1;
	colorBlendInfo.pAttachments = &blendInfo;
	colorBlendInfo.blendConstants[0] = 0.0f;
	colorBlendInfo.blendConstants[1] = 0.0f;
	colorBlendInfo.blendConstants[2] = 0.0f;
	colorBlendInfo.blendConstants[3] = 0.0f;

	graphicsPipelineInfo.pColorBlendState = &colorBlendInfo;

	// dynamic state
	vk::DynamicState dynamicStates[] = {vk::DynamicState::eViewport, vk::DynamicState::eScissor};

	vk::PipelineDynamicStateCreateInfo dynamicStateInfo;
	dynamicStateInfo.pDynamicStates = dynamicStates;
	dynamicStateInfo.dynamicStateCount = 2;

	graphicsPipelineInfo.pDynamicState = &dynamicStateInfo;

	// setup a render pass
	assert(renderPassPipelineState_ != nullptr);
	graphicsPipelineInfo.renderPass = static_cast<RenderPassPipelineStateVulkan*>(renderPassPipelineState_.get())->GetRenderPass();

	// uniform layout info
	std::array<vk::DescriptorSetLayoutBinding, 3> uboLayoutBindings;
	uboLayoutBindings[0].binding = 0;
	uboLayoutBindings[0].descriptorType = vk::DescriptorType::eUniformBufferDynamic;
	uboLayoutBindings[0].descriptorCount = 1;
	uboLayoutBindings[0].stageFlags = vk::ShaderStageFlagBits::eVertex | vk::ShaderStageFlagBits::eFragment;
	uboLayoutBindings[0].pImmutableSamplers = nullptr;

	uboLayoutBindings[1].binding = 1;
	uboLayoutBindings[1].descriptorType = vk::DescriptorType::eCombinedImageSampler;
	uboLayoutBindings[1].descriptorCount = 1;
	uboLayoutBindings[1].stageFlags = vk::ShaderStageFlagBits::eVertex | vk::ShaderStageFlagBits::eFragment;
	uboLayoutBindings[1].pImmutableSamplers = nullptr;

	uboLayoutBindings[2].binding = 2;
	uboLayoutBindings[2].descriptorType = vk::DescriptorType::eCombinedImageSampler;
	uboLayoutBindings[2].descriptorCount = 1;
	uboLayoutBindings[2].stageFlags = vk::ShaderStageFlagBits::eVertex | vk::ShaderStageFlagBits::eFragment;
	uboLayoutBindings[2].pImmutableSamplers = nullptr;

	vk::DescriptorSetLayoutCreateInfo descriptorSetLayoutInfo;
	descriptorSetLayoutInfo.bindingCount = uboLayoutBindings.size();
	descriptorSetLayoutInfo.pBindings = uboLayoutBindings.data();

	descriptorSetLayouts[0] = graphics_->GetDevice().createDescriptorSetLayout(descriptorSetLayoutInfo);
	descriptorSetLayouts[1] = graphics_->GetDevice().createDescriptorSetLayout(descriptorSetLayoutInfo);

	vk::PipelineLayoutCreateInfo layoutInfo = {};
	layoutInfo.setLayoutCount = 2;
	layoutInfo.pSetLayouts = descriptorSetLayouts.data();
	layoutInfo.pushConstantRangeCount = 0;
	layoutInfo.pPushConstantRanges = nullptr;

	pipelineLayout_ = graphics_->GetDevice().createPipelineLayout(layoutInfo);
	graphicsPipelineInfo.layout = pipelineLayout_;

	// setup a pipeline
	pipeline_ = graphics_->GetDevice().createGraphicsPipeline(nullptr, graphicsPipelineInfo);
}

} // namespace LLGI
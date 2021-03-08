#include "LLGI.CommandListVulkan.h"
#include "LLGI.ConstantBufferVulkan.h"
#include "LLGI.GraphicsVulkan.h"
#include "LLGI.IndexBufferVulkan.h"
#include "LLGI.PipelineStateVulkan.h"
#include "LLGI.TextureVulkan.h"
#include "LLGI.VertexBufferVulkan.h"

namespace LLGI
{

DescriptorPoolVulkan::DescriptorPoolVulkan(std::shared_ptr<GraphicsVulkan> graphics, int32_t size, int stage) : graphics_(graphics)
{
	std::array<vk::DescriptorPoolSize, 3> poolSizes;
	poolSizes[0].type = vk::DescriptorType::eUniformBufferDynamic;
	poolSizes[0].descriptorCount = size * stage;
	poolSizes[1].type = vk::DescriptorType::eCombinedImageSampler;
	poolSizes[1].descriptorCount = size * stage;

	vk::DescriptorPoolCreateInfo poolInfo;
	poolInfo.poolSizeCount = 2;
	poolInfo.pPoolSizes = poolSizes.data();
	poolInfo.maxSets = size * stage;

	descriptorPool_ = graphics_->GetDevice().createDescriptorPool(poolInfo);
}

DescriptorPoolVulkan ::~DescriptorPoolVulkan()
{
	if (descriptorPool_)
	{
		graphics_->GetDevice().destroyDescriptorPool(descriptorPool_);
		descriptorPool_ = nullptr;
	}
}

const std::vector<vk::DescriptorSet>& DescriptorPoolVulkan::Get(PipelineStateVulkan* pip)
{
	if (cache.size() > static_cast<size_t>(offset))
	{
		offset++;
		return cache[offset - 1];
	}

	// TODO : improve it
	vk::DescriptorSetAllocateInfo allocateInfo;
	allocateInfo.descriptorPool = descriptorPool_;
	allocateInfo.descriptorSetCount = 2;
	allocateInfo.pSetLayouts = (pip->GetDescriptorSetLayout().data());

	std::vector<vk::DescriptorSet> descriptorSets = graphics_->GetDevice().allocateDescriptorSets(allocateInfo);
	cache.push_back(descriptorSets);
	offset++;
	return cache[offset - 1];
}

void DescriptorPoolVulkan::Reset() { offset = 0; }

CommandListVulkan::CommandListVulkan() {}

CommandListVulkan::~CommandListVulkan()
{
	if (commandBuffers_.size() > 0)
	{
		graphics_->GetDevice().freeCommandBuffers(graphics_->GetCommandPool(), commandBuffers_);
	}
	commandBuffers_.clear();

	descriptorPools.clear();

	for (size_t i = 0; i < fences_.size(); i++)
	{
		graphics_->GetDevice().destroyFence(fences_[i]);
	}
	fences_.clear();

	for (int w = 0; w < 2; w++)
	{
		for (int f = 0; f < 2; f++)
		{
			if (samplers_[w][f])
			{
				graphics_->GetDevice().destroySampler(samplers_[w][f]);
			}
		}
	}
}

bool CommandListVulkan::Initialize(GraphicsVulkan* graphics, int32_t drawingCount)
{
	SafeAddRef(graphics);
	graphics_ = CreateSharedPtr(graphics);

	vk::CommandBufferAllocateInfo allocInfo;
	allocInfo.commandPool = graphics->GetCommandPool();
	allocInfo.level = vk::CommandBufferLevel::ePrimary;
	allocInfo.commandBufferCount = graphics->GetSwapBufferCount();
	commandBuffers_ = graphics->GetDevice().allocateCommandBuffers(allocInfo);

	for (size_t i = 0; i < static_cast<size_t>(graphics_->GetSwapBufferCount()); i++)
	{
		auto dp = std::make_shared<DescriptorPoolVulkan>(graphics_, drawingCount, 2);
		descriptorPools.push_back(dp);

		fences_.emplace_back(graphics->GetDevice().createFence(vk::FenceCreateFlags()));
	}

	// Sampler
	for (int w = 0; w < 2; w++)
	{
		for (int f = 0; f < 2; f++)
		{
			vk::Filter filters[2];
			filters[0] = vk::Filter::eNearest;
			filters[1] = vk::Filter::eLinear;

			vk::SamplerAddressMode am[2];
			am[0] = vk::SamplerAddressMode::eClampToEdge;
			am[1] = vk::SamplerAddressMode::eRepeat;

			vk::SamplerCreateInfo samplerInfo;
			samplerInfo.magFilter = filters[f];
			samplerInfo.minFilter = filters[f];
			samplerInfo.anisotropyEnable = false;
			samplerInfo.maxAnisotropy = 1;
			samplerInfo.addressModeU = am[w];
			samplerInfo.addressModeV = am[w];
			samplerInfo.addressModeW = am[w];
			samplerInfo.borderColor = vk::BorderColor::eIntOpaqueBlack;
			samplerInfo.unnormalizedCoordinates = false;
			samplerInfo.compareEnable = false;
			samplerInfo.compareOp = vk::CompareOp::eAlways;
			samplerInfo.mipmapMode = vk::SamplerMipmapMode::eLinear;
			samplerInfo.mipLodBias = 0.0f;
			samplerInfo.minLod = 0.0f;
			samplerInfo.maxLod = 8.0f;

			samplers_[w][f] = graphics_->GetDevice().createSampler(samplerInfo);
		}
	}

	currentSwapBufferIndex_ = -1;
	return true;
}

void CommandListVulkan::Begin()
{
	currentSwapBufferIndex_++;
	currentSwapBufferIndex_ %= commandBuffers_.size();

	graphics_->GetDevice().resetFences(1, &(fences_[currentSwapBufferIndex_]));

	currentCommandBuffer_ = commandBuffers_[currentSwapBufferIndex_];

	currentCommandBuffer_.reset(vk::CommandBufferResetFlagBits::eReleaseResources);
	vk::CommandBufferBeginInfo cmdBufInfo;
	currentCommandBuffer_.begin(cmdBufInfo);

	auto& dp = descriptorPools[currentSwapBufferIndex_];
	dp->Reset();

	CommandList::Begin();
}

void CommandListVulkan::End()
{
	currentCommandBuffer_.end();
	CommandList::End();
}

bool CommandListVulkan::BeginWithPlatform(void* platformContextPtr)
{
	auto ptr = reinterpret_cast<PlatformContextVulkan*>(platformContextPtr);

	currentSwapBufferIndex_++;
	currentSwapBufferIndex_ %= commandBuffers_.size();

	currentCommandBuffer_ = ptr->commandBuffer;

	auto& dp = descriptorPools[currentSwapBufferIndex_];
	dp->Reset();

	return CommandList::BeginWithPlatform(platformContextPtr);
}

void CommandListVulkan::EndWithPlatform()
{
	currentCommandBuffer_ = vk::CommandBuffer();
	CommandList::EndWithPlatform();
}

void CommandListVulkan::SetScissor(int32_t x, int32_t y, int32_t width, int32_t height)
{
	vk::Rect2D scissor = vk::Rect2D(vk::Offset2D(x, y), vk::Extent2D(width, height));
	currentCommandBuffer_.setScissor(0, scissor);
}

void CommandListVulkan::Draw(int32_t primitiveCount, int32_t instanceCount)
{
	BindingVertexBuffer vb_;
	BindingIndexBuffer ib_;
	PipelineState* pip_ = nullptr;

	bool isVBDirtied = false;
	bool isIBDirtied = false;
	bool isPipDirtied = false;

	GetCurrentVertexBuffer(vb_, isVBDirtied);
	GetCurrentIndexBuffer(ib_, isIBDirtied);
	GetCurrentPipelineState(pip_, isPipDirtied);

	assert(vb_.vertexBuffer != nullptr);
	assert(ib_.indexBuffer != nullptr);
	assert(pip_ != nullptr);

	auto vb = static_cast<VertexBufferVulkan*>(vb_.vertexBuffer);
	auto ib = static_cast<IndexBufferVulkan*>(ib_.indexBuffer);
	auto pip = static_cast<PipelineStateVulkan*>(pip_);

	// assign a vertex buffer
	if (isVBDirtied)
	{
		vk::DeviceSize vertexOffsets = vb_.offset;
		vk::Buffer vkBuf = vb->GetBuffer();
		currentCommandBuffer_.bindVertexBuffers(0, 1, &(vkBuf), &vertexOffsets);
	}

	// assign an index vuffer
	if (isIBDirtied)
	{
		vk::DeviceSize indexOffset = ib_.offset;
		vk::IndexType indexType = vk::IndexType::eUint16;

		if (ib->GetStride() == 2)
			indexType = vk::IndexType::eUint16;
		if (ib->GetStride() == 4)
			indexType = vk::IndexType::eUint32;

		currentCommandBuffer_.bindIndexBuffer(ib->GetBuffer(), indexOffset, indexType);
	}

	auto& dp = descriptorPools[currentSwapBufferIndex_];

	std::vector<vk::DescriptorSet> descriptorSets = dp->Get(pip);
	/*
	vk::DescriptorSetAllocateInfo allocateInfo;
	allocateInfo.descriptorPool = descriptorPool;
	allocateInfo.descriptorSetCount = 2;
	allocateInfo.pSetLayouts = (pip->GetDescriptorSetLayout().data());

	std::vector<vk::DescriptorSet> descriptorSets = graphics_->GetDevice().allocateDescriptorSets(allocateInfo);
	*/

	std::array<vk::WriteDescriptorSet, NumTexture * 2 + 2> writeDescriptorSets;
	int writeDescriptorIndex = 0;

	std::array<vk::DescriptorBufferInfo, NumTexture * 2 + 2> descriptorBufferInfos;
	int descriptorBufferIndex = 0;

	std::array<vk::DescriptorImageInfo, NumTexture * 2 + 2> descriptorImageInfos;
	int descriptorImageIndex = 0;

	std::array<bool, static_cast<int>(ShaderStageType::Max)> stages;
	stages.fill(false);

	ConstantBuffer* vcb = nullptr;
	GetCurrentConstantBuffer(ShaderStageType::Vertex, vcb);
	if (vcb != nullptr)
	{
		stages[0] = true;

		descriptorBufferInfos[descriptorBufferIndex].buffer = (static_cast<ConstantBufferVulkan*>(vcb)->GetBuffer());
		descriptorBufferInfos[descriptorBufferIndex].offset = static_cast<ConstantBufferVulkan*>(vcb)->GetOffset();
		descriptorBufferInfos[descriptorBufferIndex].range = vcb->GetSize();

		vk::WriteDescriptorSet desc;
		desc.descriptorType = vk::DescriptorType::eUniformBufferDynamic;
		desc.dstSet = descriptorSets[0];
		desc.dstBinding = 0;
		desc.dstArrayElement = 0;
		desc.pBufferInfo = &(descriptorBufferInfos[descriptorBufferIndex]);
		desc.descriptorCount = 1;

		writeDescriptorSets[writeDescriptorIndex] = desc;

		descriptorBufferIndex++;
		writeDescriptorIndex++;
	}

	ConstantBuffer* pcb = nullptr;
	GetCurrentConstantBuffer(ShaderStageType::Pixel, pcb);
	if (pcb != nullptr)
	{
		stages[1] = true;

		descriptorBufferInfos[descriptorBufferIndex].buffer = (static_cast<ConstantBufferVulkan*>(pcb)->GetBuffer());
		descriptorBufferInfos[descriptorBufferIndex].offset = static_cast<ConstantBufferVulkan*>(pcb)->GetOffset();
		descriptorBufferInfos[descriptorBufferIndex].range = pcb->GetSize();
		vk::WriteDescriptorSet desc;
		desc.descriptorType = vk::DescriptorType::eUniformBufferDynamic;
		desc.dstSet = descriptorSets[1];
		desc.dstBinding = 0;
		desc.dstArrayElement = 0;
		desc.pBufferInfo = &(descriptorBufferInfos[descriptorBufferIndex]);
		desc.descriptorCount = 1;

		writeDescriptorSets[writeDescriptorIndex] = desc;

		descriptorBufferIndex++;
		writeDescriptorIndex++;
	}

	// Assign textures
	for (int stage_ind = 0; stage_ind < static_cast<int32_t>(ShaderStageType::Max); stage_ind++)
	{
		for (int unit_ind = 0; unit_ind < static_cast<int32_t>(currentTextures[stage_ind].size()); unit_ind++)
		{
			if (currentTextures[stage_ind][unit_ind].texture == nullptr)
				continue;

			stages[stage_ind] = true;

			auto texture = (TextureVulkan*)currentTextures[stage_ind][unit_ind].texture;
			auto wm = (int32_t)currentTextures[stage_ind][unit_ind].wrapMode;
			auto mm = (int32_t)currentTextures[stage_ind][unit_ind].minMagFilter;

			vk::DescriptorImageInfo imageInfo;
			if (texture->GetType() == TextureType::Depth)
			{
				//	texture->ResourceBarrior(cmdBuffer, vk::ImageLayout::eDepthStencilReadOnlyOptimal);
				imageInfo.imageLayout = vk::ImageLayout::eDepthStencilReadOnlyOptimal;
			}
			else
			{
				//	texture->ResourceBarrior(cmdBuffer, vk::ImageLayout::eShaderReadOnlyOptimal);
				imageInfo.imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
			}

			imageInfo.imageView = texture->GetView();
			imageInfo.sampler = samplers_[wm][mm];
			descriptorImageInfos[descriptorImageIndex] = imageInfo;

			vk::WriteDescriptorSet desc;
			desc.dstSet = descriptorSets[stage_ind];
			desc.dstBinding = unit_ind + 1;
			desc.dstArrayElement = 0;
			desc.pImageInfo = &descriptorImageInfos[descriptorImageIndex];
			desc.descriptorCount = 1;
			desc.descriptorType = vk::DescriptorType::eCombinedImageSampler;

			writeDescriptorSets[writeDescriptorIndex] = desc;

			descriptorImageIndex++;
			writeDescriptorIndex++;
		}
	}

	if (writeDescriptorIndex > 0)
	{
		graphics_->GetDevice().updateDescriptorSets(writeDescriptorIndex, writeDescriptorSets.data(), 0, nullptr);
	}

	std::array<uint32_t, static_cast<int>(ShaderStageType::Max)> offsets;
	offsets.fill(0);

	if (stages[0] || stages[1])
	{
		currentCommandBuffer_.bindDescriptorSets(
			vk::PipelineBindPoint::eGraphics, pip->GetPipelineLayout(), 0, 2, descriptorSets.data(), 2, offsets.data());
	}

	// assign a pipeline
	if (isPipDirtied)
	{
		currentCommandBuffer_.bindPipeline(vk::PipelineBindPoint::eGraphics, pip->GetPipeline());
	}

	// draw
	int indexPerPrim = 0;
	if (pip->Topology == TopologyType::Triangle)
	{
		indexPerPrim = 3;
	}
	else if (pip->Topology == TopologyType::Line)
	{
		indexPerPrim = 2;
	}
	else if (pip->Topology == TopologyType::Point)
	{
		indexPerPrim = 1;
	}
	else
	{
		assert(0);
	}

	currentCommandBuffer_.drawIndexed(indexPerPrim * primitiveCount, instanceCount, 0, 0, 0);

	CommandList::Draw(primitiveCount, instanceCount);
}

void CommandListVulkan::CopyTexture(Texture* src, Texture* dst)
{
	if (isInRenderPass_)
	{
		Log(LogType::Error, "Please call CopyTexture outside of RenderPass");
		return;
	}

	auto srcTex = static_cast<TextureVulkan*>(src);
	auto dstTex = static_cast<TextureVulkan*>(dst);

	if (srcTex->GetMipmapCount() != dstTex->GetMipmapCount())
	{
		Log(LogType::Error, "CopyTexture : MipLevel is different.");
		return;
	}

	if (srcTex->GetMipmapCount() != 1)
	{
		Log(LogType::Error, "CopyTexture : MipLevel is not supported.");
		return;
	}

	std::array<vk::ImageCopy, 1> imageCopy;
	imageCopy[0].dstOffset = vk::Offset3D(0, 0, 0);
	imageCopy[0].srcOffset = vk::Offset3D(0, 0, 0);
	imageCopy[0].extent.width = src->GetSizeAs2D().X;
	imageCopy[0].extent.height = src->GetSizeAs2D().Y;
	imageCopy[0].extent.depth = 1;
	imageCopy[0].srcSubresource.aspectMask = srcTex->GetSubresourceRange().aspectMask;
	imageCopy[0].srcSubresource.layerCount = 1;
	imageCopy[0].srcSubresource.baseArrayLayer = 0;
	imageCopy[0].dstSubresource.aspectMask = dstTex->GetSubresourceRange().aspectMask;
	imageCopy[0].dstSubresource.layerCount = 1;
	imageCopy[0].dstSubresource.baseArrayLayer = 0;

	srcTex->ResourceBarrior(currentCommandBuffer_, vk::ImageLayout::eTransferSrcOptimal);
	dstTex->ResourceBarrior(currentCommandBuffer_, vk::ImageLayout::eTransferDstOptimal);
	currentCommandBuffer_.copyImage(
		srcTex->GetImage(), srcTex->GetImageLayouts()[0], dstTex->GetImage(), dstTex->GetImageLayouts()[0], imageCopy);
	dstTex->ResourceBarrior(currentCommandBuffer_, vk::ImageLayout::eShaderReadOnlyOptimal);
	srcTex->ResourceBarrior(currentCommandBuffer_, vk::ImageLayout::eShaderReadOnlyOptimal);

	RegisterReferencedObject(src);
	RegisterReferencedObject(dst);
}

void CommandListVulkan::GenerateMipMap(Texture* src)
{
	auto srcTex = static_cast<TextureVulkan*>(src);

	int32_t mipWidth = src->GetSizeAs2D().X;
	int32_t mipHeight = src->GetSizeAs2D().Y;

	for (int32_t i = 1; i < src->GetMipmapCount(); i++)
	{
		srcTex->ResourceBarrior(i - 1, currentCommandBuffer_, vk::ImageLayout::eTransferSrcOptimal);
		srcTex->ResourceBarrior(i, currentCommandBuffer_, vk::ImageLayout::eTransferDstOptimal);

		vk::ImageBlit blit{};
		blit.srcOffsets[0] = vk::Offset3D(0, 0, 0);
		blit.srcOffsets[1] = vk::Offset3D(mipWidth, mipHeight, 1);
		blit.srcSubresource.aspectMask = vk::ImageAspectFlagBits::eColor;
		blit.srcSubresource.mipLevel = i - 1;
		blit.srcSubresource.baseArrayLayer = 0;
		blit.srcSubresource.layerCount = 1;
		blit.dstOffsets[0] = vk::Offset3D(0, 0, 0);
		blit.dstOffsets[1] = vk::Offset3D(mipWidth > 1 ? mipWidth / 2 : 1, mipHeight > 1 ? mipHeight / 2 : 1, 1);
		blit.dstSubresource.aspectMask = vk::ImageAspectFlagBits::eColor;
		blit.dstSubresource.mipLevel = i;
		blit.dstSubresource.baseArrayLayer = 0;
		blit.dstSubresource.layerCount = 1;

		currentCommandBuffer_.blitImage(srcTex->GetImage(),
										vk::ImageLayout::eTransferSrcOptimal,
										srcTex->GetImage(),
										vk::ImageLayout::eTransferDstOptimal,
										1,
										&blit,
										vk::Filter::eLinear);

		mipWidth = mipWidth > 1 ? mipWidth / 2 : 1;
		mipHeight = mipHeight > 1 ? mipHeight / 2 : 1;
	}

	srcTex->ResourceBarrior(currentCommandBuffer_, vk::ImageLayout::eShaderReadOnlyOptimal);
}

void CommandListVulkan::BeginRenderPass(RenderPass* renderPass)
{
	auto renderPass_ = static_cast<RenderPassVulkan*>(renderPass);

	vk::ClearColorValue clearColor(std::array<float, 4>{renderPass_->GetClearColor().R / 255.0f,
														renderPass_->GetClearColor().G / 255.0f,
														renderPass_->GetClearColor().B / 255.0f,
														renderPass_->GetClearColor().A / 255.0f});
	vk::ClearDepthStencilValue clearDepth(1.0f, 0);

	vk::ImageSubresourceRange colorSubRange;
	colorSubRange.aspectMask = vk::ImageAspectFlagBits::eColor;
	colorSubRange.levelCount = 1;
	colorSubRange.layerCount = 1;

	vk::ImageSubresourceRange depthSubRange;
	depthSubRange.aspectMask = vk::ImageAspectFlagBits::eDepth | vk::ImageAspectFlagBits::eStencil;
	depthSubRange.levelCount = 1;
	depthSubRange.layerCount = 1;

	vk::ClearValue clear_values[RenderTargetMax + 1];
	int clearValueCount = 0;

	if (renderPass->GetIsColorCleared())
	{
		for (int32_t i = 0; i < renderPass_->GetRenderTextureCount(); i++)
		{
			clear_values[i].color = clearColor;
			clearValueCount++;
		}
	}

	if (renderPass_->GetHasDepthTexture() && renderPass->GetIsDepthCleared())
	{
		clear_values[renderPass_->GetRenderTextureCount()].depthStencil = clearDepth;
		clearValueCount++;
	}

	clear_values[renderPass_->GetRenderTextureCount()].depthStencil = clearDepth;

	if (renderPass_->GetHasDepthTexture())
	{
		auto t = static_cast<TextureVulkan*>(renderPass_->GetDepthTexture());
		t->ResourceBarrior(currentCommandBuffer_, vk::ImageLayout::eDepthStencilAttachmentOptimal);
	}

	// begin renderpass
	vk::RenderPassBeginInfo renderPassBeginInfo;
	renderPassBeginInfo.framebuffer = renderPass_->frameBuffer_;
	renderPassBeginInfo.renderPass = renderPass_->renderPassPipelineState->GetRenderPass();
	renderPassBeginInfo.renderArea.extent = vk::Extent2D(renderPass_->GetImageSize().X, renderPass_->GetImageSize().Y);
	renderPassBeginInfo.clearValueCount = clearValueCount;
	renderPassBeginInfo.pClearValues = clear_values;
	currentCommandBuffer_.beginRenderPass(renderPassBeginInfo, vk::SubpassContents::eInline);

	vk::Viewport viewport = vk::Viewport(
		0.0f, 0.0f, static_cast<float>(renderPass_->GetImageSize().X), static_cast<float>(renderPass_->GetImageSize().Y), 0.0f, 1.0f);
	currentCommandBuffer_.setViewport(0, viewport);

	vk::Rect2D scissor = vk::Rect2D(vk::Offset2D(), vk::Extent2D(renderPass_->GetImageSize().X, renderPass_->GetImageSize().Y));
	currentCommandBuffer_.setScissor(0, scissor);

	auto layoutOffset = 0;
	for (int32_t i = 0; i < renderPass_->GetRenderTextureCount(); i++)
	{
		auto t = static_cast<TextureVulkan*>(renderPass_->GetRenderTexture(i));
		t->ChangeImageLayout(renderPass_->renderPassPipelineState->finalLayouts_.at(i));
	}

	layoutOffset += renderPass_->GetRenderTextureCount();

	if (renderPass_->GetHasDepthTexture())
	{
		auto t = static_cast<TextureVulkan*>(renderPass_->GetDepthTexture());
		t->ChangeImageLayout(renderPass_->renderPassPipelineState->finalLayouts_.at(layoutOffset));
	}

	if (renderPass_->GetHasDepthTexture())
	{
		layoutOffset += 1;
	}

	if (auto t = static_cast<TextureVulkan*>(renderPass_->GetResolvedRenderTexture()))
	{
		t->ChangeImageLayout(renderPass_->renderPassPipelineState->finalLayouts_.at(layoutOffset));
	}

	CommandList::BeginRenderPass(renderPass);
}

void CommandListVulkan::EndRenderPass()
{
	// end renderpass
	currentCommandBuffer_.endRenderPass();

	CommandList::EndRenderPass();
}

vk::CommandBuffer CommandListVulkan::GetCommandBuffer() const { return currentCommandBuffer_; }

vk::Fence CommandListVulkan::GetFence() const { return fences_[currentSwapBufferIndex_]; }

void CommandListVulkan::WaitUntilCompleted()
{
	if (currentSwapBufferIndex_ >= 0)
	{
		vk::Result fenceRes =
			graphics_->GetDevice().waitForFences(fences_[currentSwapBufferIndex_], VK_TRUE, std::numeric_limits<int>::max());
		if (fenceRes != vk::Result::eSuccess)
		{
			throw "Invalid waitForFences";
		}
	}
}

} // namespace LLGI

#include "LLGI.RenderPassPipelineStateCacheVulkan.h"

namespace LLGI
{

RenderPassPipelineStateCacheVulkan::RenderPassPipelineStateCacheVulkan(vk::Device device, ReferenceObject* owner)
	: device_(device), owner_(owner)
{
	SafeAddRef(owner_);
}

RenderPassPipelineStateCacheVulkan::~RenderPassPipelineStateCacheVulkan()
{
	renderPassPipelineStates_.clear();
	SafeRelease(owner_);
}

RenderPassPipelineStateVulkan* RenderPassPipelineStateCacheVulkan::Create(bool isPresentMode,
																		  bool hasDepth,
																		  const FixedSizeVector<vk::Format, RenderTargetMax>& formats,
																		  bool isColorCleared,
																		  bool isDepthCleared)
{
	RenderPassPipelineStateVulkanKey key;
	key.isPresentMode = isPresentMode;
	key.hasDepth = hasDepth;
	key.formats = formats;
	key.isColorCleared = isColorCleared;
	key.isDepthCleared = isDepthCleared;

	// already?
	{
		auto it = renderPassPipelineStates_.find(key);

		if (it != renderPassPipelineStates_.end())
		{
			auto ret = it->second;

			if (ret != nullptr)
			{
				auto retptr = ret.get();
				SafeAddRef(retptr);
				return retptr;
			}
		}
	}

	// settings
	FixedSizeVector<vk::AttachmentDescription, RenderTargetMax + 1> attachmentDescs;
	FixedSizeVector<vk::AttachmentReference, RenderTargetMax + 1> attachmentRefs;
	attachmentDescs.resize(formats.size() + (hasDepth ? 1 : 0));
	attachmentRefs.resize(formats.size() + (hasDepth ? 1 : 0));

	// color buffer
	int colorCount = formats.size();

	for (int i = 0; i < colorCount; i++)
	{
		attachmentDescs.at(i).format = formats.at(i);
		attachmentDescs.at(i).samples = vk::SampleCountFlagBits::e1;

		if (isColorCleared)
			attachmentDescs.at(i).loadOp = vk::AttachmentLoadOp::eClear;
		else
			attachmentDescs.at(i).loadOp = vk::AttachmentLoadOp::eDontCare;

		attachmentDescs.at(i).storeOp = vk::AttachmentStoreOp::eStore;
		attachmentDescs.at(i).stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
		attachmentDescs.at(i).stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
	}

	if (isPresentMode)
	{
		// When clearing, the initialLayout does not matter.
		attachmentDescs.at(0).initialLayout = (isColorCleared) ? vk::ImageLayout::eUndefined : vk::ImageLayout::ePresentSrcKHR;
		attachmentDescs.at(0).finalLayout = vk::ImageLayout::ePresentSrcKHR;
	}
	else
	{
		for (int i = 0; i < colorCount; i++)
		{
			// When clearing, the initialLayout does not matter.
			attachmentDescs.at(i).initialLayout = (isColorCleared) ? vk::ImageLayout::eUndefined : vk::ImageLayout::eShaderReadOnlyOptimal;
			attachmentDescs.at(i).finalLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
		}
	}

	// depth buffer
	if (hasDepth)
	{
		attachmentDescs.at(colorCount).format = vk::Format::eD32SfloatS8Uint;
		attachmentDescs.at(colorCount).samples = vk::SampleCountFlagBits::e1;

		if (isDepthCleared)
			attachmentDescs.at(colorCount).loadOp = vk::AttachmentLoadOp::eClear;
		else
			attachmentDescs.at(colorCount).loadOp = vk::AttachmentLoadOp::eDontCare;

		attachmentDescs.at(colorCount).storeOp = vk::AttachmentStoreOp::eStore;
		attachmentDescs.at(colorCount).stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
		attachmentDescs.at(colorCount).stencilStoreOp = vk::AttachmentStoreOp::eStore;

		// When clearing, the initialLayout does not matter.
		attachmentDescs.at(colorCount).initialLayout = (isDepthCleared) ? vk::ImageLayout::eUndefined : vk::ImageLayout::eDepthStencilAttachmentOptimal;
		attachmentDescs.at(colorCount).finalLayout = vk::ImageLayout::eDepthStencilAttachmentOptimal;
	}

	for (int i = 0; i < colorCount; i++)
	{
		vk::AttachmentReference& colorReference = attachmentRefs.at(i);
		colorReference.attachment = i;
		colorReference.layout = vk::ImageLayout::eColorAttachmentOptimal;
	}

	if (hasDepth)
	{
		vk::AttachmentReference& depthReference = attachmentRefs.at(colorCount);
		depthReference.attachment = colorCount;
		depthReference.layout = vk::ImageLayout::eDepthStencilAttachmentOptimal;
	}

	std::array<vk::SubpassDescription, 1> subpasses;
	{
		vk::SubpassDescription& subpass = subpasses[0];
		subpass.pipelineBindPoint = vk::PipelineBindPoint::eGraphics;
		subpass.colorAttachmentCount = colorCount;
		subpass.pColorAttachments = &attachmentRefs.at(0);

		if (hasDepth)
		{
			subpass.pDepthStencilAttachment = &attachmentRefs.at(colorCount);
		}
		else
		{
			subpass.pDepthStencilAttachment = nullptr;
		}
	}

	std::array<vk::SubpassDependency, 1> subpassDepends;
	{
		vk::SubpassDependency& dependency = subpassDepends[0];

		/*
		//monsho
		dependency.srcSubpass = 0;
		dependency.dstSubpass = VK_SUBPASS_EXTERNAL;
		dependency.srcAccessMask = vk::AccessFlagBits::eColorAttachmentWrite;
		dependency.dstAccessMask = vk::AccessFlagBits::eColorAttachmentRead;
		dependency.srcStageMask = vk::PipelineStageFlagBits::eBottomOfPipe;
		dependency.dstStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;
		*/

		dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
		dependency.dstSubpass = 0;
		dependency.srcAccessMask = static_cast<vk::AccessFlagBits>(0);
		dependency.dstAccessMask = vk::AccessFlagBits::eColorAttachmentWrite;
		dependency.srcStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;
		dependency.dstStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;
	}

	{
		vk::RenderPassCreateInfo renderPassInfo;
		renderPassInfo.attachmentCount = (uint32_t)attachmentDescs.size();
		renderPassInfo.pAttachments = attachmentDescs.data();
		renderPassInfo.subpassCount = (uint32_t)subpasses.size();
		renderPassInfo.pSubpasses = subpasses.data();

		// based on official
		// renderPassInfo.dependencyCount = (uint32_t)subpassDepends.size();
		// renderPassInfo.pDependencies = subpassDepends.data();
		renderPassInfo.dependencyCount = 0;
		renderPassInfo.pDependencies = nullptr;

		auto renderPass = device_.createRenderPass(renderPassInfo);
		if (!renderPass)
		{
			return nullptr;
		}

		std::shared_ptr<RenderPassPipelineStateVulkan> ret = CreateSharedPtr(new RenderPassPipelineStateVulkan(device_, owner_));
		ret->renderPass_ = renderPass;

		renderPassPipelineStates_[key] = ret;

		auto retptr = ret.get();
		SafeAddRef(retptr);
		return retptr;
	}
}

} // namespace LLGI

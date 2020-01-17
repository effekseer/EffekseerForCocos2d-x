
#include "LLGI.TextureVulkan.h"

namespace LLGI
{

TextureVulkan::TextureVulkan() {}

TextureVulkan::~TextureVulkan()
{
	if (image_)
	{
		if (!isExternalResource_)
		{
			device_.destroyImageView(view_);
			device_.destroyImage(image_);
			device_.freeMemory(devMem_);

			image_ = nullptr;
			view_ = nullptr;
		}
	}

	if (isStrongRef_)
	{
		SafeRelease(graphics_);
	}

	SafeRelease(owner_);
}

bool TextureVulkan::Initialize(GraphicsVulkan* graphics, bool isStrongRef, const Vec2I& size, bool isRenderPass)
{
	graphics_ = graphics;
	if (isStrongRef_)
	{
		SafeAddRef(graphics_);
	}

	type_ = TextureType::Color;

	if (isRenderPass)
	{
		type_ = TextureType::Render;
	}

	cpuBuf = std::unique_ptr<Buffer>(new Buffer(graphics_));

	vk::Format format = vk::Format::eR8G8B8A8Unorm;

	// image
	vk::ImageCreateInfo imageCreateInfo;

	imageCreateInfo.imageType = vk::ImageType::e2D;
	imageCreateInfo.extent.width = size.X;
	imageCreateInfo.extent.height = size.Y;
	imageCreateInfo.extent.depth = 1;
	imageCreateInfo.mipLevels = 1;
	imageCreateInfo.arrayLayers = 1;
	imageCreateInfo.format = format;
	imageCreateInfo.tiling = vk::ImageTiling::eOptimal;
	imageCreateInfo.initialLayout = vk::ImageLayout::eUndefined;

	if (isRenderPass)
	{
		isRenderPass_ = isRenderPass;
		imageCreateInfo.usage = vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eSampled;
	}
	else
	{
		imageCreateInfo.usage = vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled;
	}

	imageCreateInfo.sharingMode = vk::SharingMode::eExclusive;
	imageCreateInfo.samples = vk::SampleCountFlagBits::e1;
	imageCreateInfo.flags = (vk::ImageCreateFlagBits)0;

	image_ = graphics_->GetDevice().createImage(imageCreateInfo);

	// get device
	auto device = graphics_->GetDevice();

	// calculate size
	memorySize = size.X * size.Y * 4;

	// create a buffer on cpu
	{
		vk::BufferCreateInfo bufferInfo;
		bufferInfo.size = memorySize;
		bufferInfo.usage = vk::BufferUsageFlagBits::eTransferSrc;
		vk::Buffer buffer = graphics_->GetDevice().createBuffer(bufferInfo);

		vk::MemoryRequirements memReqs = graphics_->GetDevice().getBufferMemoryRequirements(buffer);
		vk::MemoryAllocateInfo memAlloc;
		memAlloc.allocationSize = memReqs.size;
		memAlloc.memoryTypeIndex = graphics_->GetMemoryTypeIndex(memReqs.memoryTypeBits, vk::MemoryPropertyFlagBits::eHostVisible);
		vk::DeviceMemory devMem = graphics_->GetDevice().allocateMemory(memAlloc);
		graphics_->GetDevice().bindBufferMemory(buffer, devMem, 0);

		cpuBuf->Attach(buffer, devMem);
	}

	// create a buffer on gpu
	{
		vk::MemoryRequirements memReqs = device.getImageMemoryRequirements(image_);
		vk::MemoryAllocateInfo memAlloc;
		memAlloc.allocationSize = memReqs.size;
		memAlloc.memoryTypeIndex = graphics_->GetMemoryTypeIndex(memReqs.memoryTypeBits, vk::MemoryPropertyFlagBits::eDeviceLocal);
		devMem_ = device.allocateMemory(memAlloc);
		graphics_->GetDevice().bindImageMemory(image_, devMem_, 0);
	}

	// create a texture view
	{
		vk::ImageViewCreateInfo imageViewInfo;
		imageViewInfo.image = image_;
		imageViewInfo.viewType = vk::ImageViewType::e2D;
		imageViewInfo.format = format;
		imageViewInfo.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
		imageViewInfo.subresourceRange.baseMipLevel = 0;
		imageViewInfo.subresourceRange.levelCount = 1;
		imageViewInfo.subresourceRange.baseArrayLayer = 0;
		imageViewInfo.subresourceRange.layerCount = 1;
		view_ = device.createImageView(imageViewInfo);
	}

	textureSize = size;
	vkTextureFormat_ = imageCreateInfo.format;
	device_ = graphics_->GetDevice();

	return true;
}

bool TextureVulkan::InitializeAsRenderTexture(GraphicsVulkan* graphics,
											  bool isStrongRef,
											  const RenderTextureInitializationParameter& parameter)
{
	return Initialize(graphics, isStrongRef, parameter.Size, true);
}

bool TextureVulkan::InitializeAsScreen(const vk::Image& image, const vk::ImageView& imageVew, vk::Format format, const Vec2I& size)
{
	type_ = TextureType::Screen;

	this->image_ = image;
	this->view_ = imageVew;
	vkTextureFormat_ = format;
	textureSize = size;
	memorySize = size.X * size.Y * 4; // TODO: format
	isExternalResource_ = true;
	return true;
}

bool TextureVulkan::InitializeAsDepthStencil(vk::Device device,
											 vk::PhysicalDevice physicalDevice,
											 const Vec2I& size,
											 ReferenceObject* owner)
{
	type_ = TextureType::Depth;
	textureSize = size;

	owner_ = owner;
	SafeAddRef(owner_);
	device_ = device;

	// check a format whether specified format is supported
	vk::Format depthFormat = vk::Format::eD32SfloatS8Uint;
	vk::FormatProperties formatProps = physicalDevice.getFormatProperties(depthFormat);
	assert(formatProps.optimalTilingFeatures & vk::FormatFeatureFlagBits::eDepthStencilAttachment);

	vk::ImageAspectFlags aspect = vk::ImageAspectFlagBits::eDepth | vk::ImageAspectFlagBits::eStencil;

	// create an image
	vk::ImageCreateInfo imageCreateInfo;
	imageCreateInfo.imageType = vk::ImageType::e2D;
	imageCreateInfo.extent = vk::Extent3D(size.X, size.Y, 1);
	imageCreateInfo.format = depthFormat;
	imageCreateInfo.mipLevels = 1;
	imageCreateInfo.arrayLayers = 1;
	imageCreateInfo.usage = vk::ImageUsageFlagBits::eDepthStencilAttachment;
	image_ = device.createImage(imageCreateInfo);

	// allocate memory
	vk::MemoryRequirements memReqs = device.getImageMemoryRequirements(image_);
	vk::MemoryAllocateInfo memAlloc;
	memAlloc.allocationSize = memReqs.size;
	memAlloc.memoryTypeIndex = GetMemoryTypeIndex(physicalDevice, memReqs.memoryTypeBits, vk::MemoryPropertyFlagBits::eDeviceLocal);
	devMem_ = device.allocateMemory(memAlloc);
	device.bindImageMemory(image_, devMem_, 0);

	// create view
	vk::ImageViewCreateInfo viewCreateInfo;
	viewCreateInfo.viewType = vk::ImageViewType::e2D;
	viewCreateInfo.format = depthFormat;
	viewCreateInfo.components = {vk::ComponentSwizzle::eR, vk::ComponentSwizzle::eG, vk::ComponentSwizzle::eB, vk::ComponentSwizzle::eA};
	viewCreateInfo.subresourceRange.aspectMask = aspect;
	viewCreateInfo.subresourceRange.levelCount = 1;
	viewCreateInfo.subresourceRange.layerCount = 1;
	viewCreateInfo.image = image_;
	view_ = device.createImageView(viewCreateInfo);

	vkTextureFormat_ = depthFormat;

	return true;
	// change layout(nt needed?)
	/*
	{
		vk::CommandBufferBeginInfo cmdBufferBeginInfo;
		vk::BufferCopy copyRegion;

		// start to store commands
		vkCmdBuffers[0].begin(cmdBufferBeginInfo);

		vk::ImageSubresourceRange subresourceRange;
		subresourceRange.aspectMask = aspect;
		subresourceRange.levelCount = 1;
		subresourceRange.layerCount = 1;
		SetImageBarrior(vkCmdBuffers[0],
						depthStencilBuffer.image,
						vk::ImageLayout::eUndefined,
						vk::ImageLayout::eDepthStencilAttachmentOptimal,
						subresourceRange);

		vkCmdBuffers[0].end();

		// submit and wait
		std::array<vk::SubmitInfo, 1> copySubmitInfos;
		copySubmitInfos[0].commandBufferCount = 1;
		copySubmitInfos[0].pCommandBuffers = &vkCmdBuffers[0];

		vkQueue.submit(copySubmitInfos.size(), copySubmitInfos.data(), vk::Fence());
		vkQueue.waitIdle();
	}
	*/
}

bool TextureVulkan::InitializeFromExternal(TextureType type, VkImage image, VkImageView imageView, VkFormat format, const Vec2I& size)
{
	type_ = type;
	image_ = vk::Image(image);
	view_ = vk::ImageView(imageView);
	vkTextureFormat_ = vk::Format(format);
	textureSize = size;
	isExternalResource_ = true;
	return true;
}

void* TextureVulkan::Lock()
{
	if (graphics_ == nullptr)
		return nullptr;

	data = graphics_->GetDevice().mapMemory(cpuBuf->devMem(), 0, memorySize, vk::MemoryMapFlags());
	return data;
}

void TextureVulkan::Unlock()
{
	if (graphics_ == nullptr)
	{
		return;
	}

	graphics_->GetDevice().unmapMemory(cpuBuf->devMem());

	// copy buffer
	vk::CommandBufferAllocateInfo cmdBufInfo;
	cmdBufInfo.commandPool = graphics_->GetCommandPool();
	cmdBufInfo.level = vk::CommandBufferLevel::ePrimary;
	cmdBufInfo.commandBufferCount = 1;
	vk::CommandBuffer copyCommandBuffer = graphics_->GetDevice().allocateCommandBuffers(cmdBufInfo)[0];

	vk::CommandBufferBeginInfo cmdBufferBeginInfo;

	copyCommandBuffer.begin(cmdBufferBeginInfo);

	vk::ImageLayout imageLayout = vk::ImageLayout::eTransferDstOptimal;
	vk::BufferImageCopy imageBufferCopy;

	imageBufferCopy.bufferOffset = 0;
	imageBufferCopy.bufferRowLength = 0;
	imageBufferCopy.bufferImageHeight = 0;

	imageBufferCopy.imageSubresource.aspectMask = vk::ImageAspectFlagBits::eColor;
	imageBufferCopy.imageSubresource.mipLevel = 0;
	imageBufferCopy.imageSubresource.baseArrayLayer = 0;
	imageBufferCopy.imageSubresource.layerCount = 1;

	imageBufferCopy.imageOffset = vk::Offset3D(0, 0, 0);
	imageBufferCopy.imageExtent = vk::Extent3D(static_cast<uint32_t>(GetSizeAs2D().X), static_cast<uint32_t>(GetSizeAs2D().Y), 1);

	vk::ImageSubresourceRange colorSubRange;
	colorSubRange.aspectMask = vk::ImageAspectFlagBits::eColor;
	colorSubRange.levelCount = 1;
	colorSubRange.layerCount = 1;

	SetImageLayout(copyCommandBuffer, image_, vk::ImageLayout::eUndefined, vk::ImageLayout::eTransferDstOptimal, colorSubRange);

	copyCommandBuffer.copyBufferToImage(cpuBuf->buffer(), image_, imageLayout, imageBufferCopy);

	SetImageLayout(copyCommandBuffer, image_, vk::ImageLayout::eTransferDstOptimal, vk::ImageLayout::eShaderReadOnlyOptimal, colorSubRange);

	copyCommandBuffer.end();

	// submit and wait to execute command
	std::array<vk::SubmitInfo, 1> copySubmitInfos;
	copySubmitInfos[0].commandBufferCount = 1;
	copySubmitInfos[0].pCommandBuffers = &copyCommandBuffer;

	graphics_->GetQueue().submit(copySubmitInfos.size(), copySubmitInfos.data(), vk::Fence());
	graphics_->GetQueue().waitIdle();

	graphics_->GetDevice().freeCommandBuffers(graphics_->GetCommandPool(), copyCommandBuffer);
}

Vec2I TextureVulkan::GetSizeAs2D() const { return textureSize; }

} // namespace LLGI

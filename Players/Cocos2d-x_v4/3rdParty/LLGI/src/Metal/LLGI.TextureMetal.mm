#include "LLGI.TextureMetal.h"
#include "LLGI.Metal_Impl.h"

namespace LLGI
{

Texture_Impl::Texture_Impl() : msaaTexture_(nullptr) {}

Texture_Impl::~Texture_Impl()
{
	if (!fromExternal_ && texture != nullptr)
	{
		[texture release];
		texture = nullptr;
	}
	if (msaaTexture_ != nullptr)
	{
		[msaaTexture_ release];
		msaaTexture_ = nullptr;
	}
}

bool Texture_Impl::Initialize(id<MTLDevice> device, const Vec2I& size, TextureType type)
{
    MTLTextureDescriptor* textureDescriptor = nullptr;
    
    if(type == TextureType::Depth)
    {
#if TARGET_OS_IOS
        auto format = MTLPixelFormatDepth32Float_Stencil8;
#elif TARGET_OS_MAC
		bool supported = device.isDepth24Stencil8PixelFormatSupported;
		
		auto format = (supported) ? MTLPixelFormatDepth24Unorm_Stencil8 : MTLPixelFormatDepth32Float_Stencil8;
#endif
        textureDescriptor = [MTLTextureDescriptor texture2DDescriptorWithPixelFormat:format
																			   width:size.X
																			  height:size.Y
																		   mipmapped:NO];
		textureDescriptor.usage = MTLTextureUsageRenderTarget;
		textureDescriptor.textureType = MTLTextureType2D;
		textureDescriptor.storageMode = MTLStorageModePrivate;
    }
    else
    {
        textureDescriptor = [MTLTextureDescriptor texture2DDescriptorWithPixelFormat:MTLPixelFormatRGBA8Unorm
																								 width:size.X
																								height:size.Y
																							 mipmapped:NO];
    }
    
    if(type == TextureType::Render)
    {
        textureDescriptor.usage = MTLTextureUsageRenderTarget | MTLTextureUsageShaderRead;
        textureDescriptor.textureType = MTLTextureType2D;
        textureDescriptor.depth = 1;
        textureDescriptor.storageMode =MTLStorageModePrivate;
    }
    
	
	texture = [device newTextureWithDescriptor:textureDescriptor];

	size_ = size;

    fromExternal_ = false;
    
	return true;
}

bool Texture_Impl::Initialize(Graphics_Impl* graphics, const RenderTextureInitializationParameter& parameter)
{
	id<MTLDevice> device = graphics->device;
    MTLTextureDescriptor* textureDescriptor = nullptr;

	textureDescriptor = [MTLTextureDescriptor texture2DDescriptorWithPixelFormat:MTLPixelFormatRGBA8Unorm
																							 width:parameter.Size.X
																							height:parameter.Size.Y
																						 mipmapped:NO];
	textureDescriptor.usage = MTLTextureUsageRenderTarget | MTLTextureUsageShaderRead;
	textureDescriptor.textureType = MTLTextureType2D;
	textureDescriptor.depth = 1;
	textureDescriptor.storageMode =MTLStorageModePrivate;
	
	if (parameter.IsMultiSampling)
	{
		multiSampled_ = true;
		
		MTLTextureDescriptor* desc = [MTLTextureDescriptor texture2DDescriptorWithPixelFormat:MTLPixelFormatRGBA8Unorm
																						width:parameter.Size.X
																					   height:parameter.Size.Y
																					mipmapped:NO];
		desc.textureType = MTLTextureType2DMultisample;
		desc.storageMode = MTLStorageModePrivate;
		desc.sampleCount = graphics->maxMultiSamplingCount;
		desc.usage = MTLTextureUsageRenderTarget;

		msaaTexture_ = [device newTextureWithDescriptor:desc];
	}
	
	texture = [device newTextureWithDescriptor:textureDescriptor];

	size_ = parameter.Size;

    fromExternal_ = false;
    
	return true;
}

void Texture_Impl::Reset(id<MTLTexture> nativeTexture)
{
    texture = nativeTexture;
	size_.X = texture.width;
	size_.Y = texture.height;
    // TODO format
    fromExternal_ = true;
}

void Texture_Impl::Write(const uint8_t* data)
{
	MTLRegion region = {{0, 0, 0}, {static_cast<uint32_t>(size_.X), static_cast<uint32_t>(size_.Y), 1}};

	[texture replaceRegion:region mipmapLevel:0 withBytes:data bytesPerRow:size_.X * 4];
}

TextureMetal::TextureMetal() { impl = new Texture_Impl(); }

TextureMetal::~TextureMetal()
{
	SafeDelete(impl);
	SafeRelease(owner_);
}

bool TextureMetal::Initialize(id<MTLDevice> device, ReferenceObject* owner, Vec2I size, TextureType type)
{
    type_ = type;
    
    SafeAssign(owner_, owner);

	data.resize(size.X * size.Y * 4);
	return impl->Initialize(device, size, type_);
}

bool TextureMetal::Initialize(GraphicsMetal* owner, const RenderTextureInitializationParameter& parameter)
{
    type_ = TextureType::Render;
    
    SafeAssign(owner_, static_cast<ReferenceObject*>(owner));

	if (parameter.Format != TextureFormatType::R8G8B8A8_UNORM) {
		throw "Not implemented.";
	}
	data.resize(parameter.Size.X * parameter.Size.Y * 4);	// TODO: pixel size
	return impl->Initialize(owner->GetImpl(), parameter);
}

void TextureMetal::Reset(id<MTLTexture> nativeTexture)
{
    type_ = TextureType::Screen;
    impl->Reset(nativeTexture);
}

void* TextureMetal::Lock() { return data.data(); }

void TextureMetal::Unlock() { impl->Write(data.data()); }

Vec2I TextureMetal::GetSizeAs2D() const { return impl->size_; }

Texture_Impl* TextureMetal::GetImpl() const { return impl; }

}

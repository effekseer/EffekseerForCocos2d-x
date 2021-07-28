#include "../EffekseerForCocos2d-x.h"
#ifdef CC_USE_METAL

#include "../../EffekseerRendererLLGI/EffekseerRendererLLGI.RendererImplemented.h"
#include "../../EffekseerRendererMetal/EffekseerRendererMetal.h"
#include "../../3rdParty/LLGI/src/Metal/LLGI.GraphicsMetal.h"
#include "../../EffekseerRendererCommon/ModelLoader.h"
#include "renderer/backend/metal/TextureMTL.h"
#include "renderer/backend/metal/CommandBufferMTL.h"
#include "renderer/backend/metal/Utils.h"
#include <Metal/LLGI.TextureMetal.h>

namespace efk {

void SetMTLObjectsFromCocos2d(Effekseer::RefPtr<EffekseerRenderer::CommandList> commandList)
{
    auto d = cocos2d::Director::getInstance();
    auto buffer = d->getCommandBuffer();
    auto bufferM = static_cast<cocos2d::backend::CommandBufferMTL*>(buffer);
    
    // use render pass descriptor from Cocos and add depth test
    auto descriptor = d->getRenderer()->getRenderPassDescriptor();
    descriptor.depthTestEnabled = true;
    // using Cocos render pass
    bufferM->beginRenderPass(descriptor);
    auto v = d->getRenderer()->getViewport();
    // important for ensuring znear and zfar are in sync with Cocos
    bufferM->setViewport(v.x, v.y, v.w, v.h);
    
    // set Command Buffer and Render Encoder from Cocos
    EffekseerRendererMetal::BeginCommandList(commandList, bufferM->getRenderCommandEncoder());
}


#pragma region DistortingCallbackMetal
class DistortingCallbackMetal
    : public EffekseerRenderer::DistortingCallback
{

    Effekseer::RefPtr<EffekseerRenderer::CommandList> commandList_ = nullptr;
    id<MTLTexture>                                  texture = nullptr;
    Effekseer::Backend::TextureRef textureInternal_ = nullptr;

public:
    DistortingCallbackMetal(Effekseer::RefPtr<EffekseerRenderer::CommandList> commandList);

    virtual ~DistortingCallbackMetal();

    virtual bool OnDistorting(EffekseerRenderer::Renderer* renderer) override;
};

DistortingCallbackMetal::DistortingCallbackMetal(Effekseer::RefPtr<EffekseerRenderer::CommandList> commandList)
: commandList_(commandList)
{
}

DistortingCallbackMetal::~DistortingCallbackMetal()
{
    if(textureInternal_ != nullptr)
    {
        [texture release];
        textureInternal_.Reset();
    }
}

bool DistortingCallbackMetal::OnDistorting(EffekseerRenderer::Renderer* renderer)
{
    // to get viewport
    auto drawable = cocos2d::backend::DeviceMTL::getCurrentDrawable();

    if(textureInternal_ == nullptr)
    {
        auto deviceMTL = static_cast<cocos2d::backend::DeviceMTL*>(cocos2d::backend::Device::getInstance());
        
        MTLTextureDescriptor* textureDescriptor =
        [MTLTextureDescriptor texture2DDescriptorWithPixelFormat:cocos2d::backend::Utils::getDefaultColorAttachmentPixelFormat()
                                                           width:drawable.texture.width
                                                          height:drawable.texture.height
                                                       mipmapped:NO];
        
        texture = [deviceMTL->getMTLDevice() newTextureWithDescriptor:textureDescriptor];
        textureInternal_ = EffekseerRendererMetal::CreateTexture(renderer->GetGraphicsDevice(), texture);
    }

    auto commandBuffer = static_cast<cocos2d::backend::CommandBufferMTL*>(cocos2d::Director::getInstance()->getCommandBuffer());
    commandBuffer->endEncoding();
    
    EffekseerRendererMetal::EndCommandList(commandList_);
    
    MTLRegion region =
    {
        {0, 0, 0},          // MTLOrigin
        {texture.width, texture.height, 1}  // MTLSize
    };
    
    id<MTLBlitCommandEncoder> blitEncoder = [commandBuffer->getMTLCommandBuffer() blitCommandEncoder];
    
    [blitEncoder copyFromTexture:drawable.texture sourceSlice:0 sourceLevel:0 sourceOrigin:region.origin sourceSize:region.size toTexture:texture destinationSlice:0 destinationLevel:0 destinationOrigin:{0, 0, 0}];
    [blitEncoder endEncoding];
    cocos2d::backend::Device::getInstance()->setFrameBufferOnly(true); // reset
    
    SetMTLObjectsFromCocos2d(commandList_);
    
    auto r = static_cast<EffekseerRendererLLGI::Renderer*>(renderer);
    r->SetBackground(textureInternal_);

    return true;
}
#pragma endregion

static ::Effekseer::Backend::GraphicsDeviceRef g_graphicsDevice = nullptr;

class EffekseerGraphicsDevice : public ::EffekseerRendererLLGI::Backend::GraphicsDevice
{
private:

public:
    EffekseerGraphicsDevice(LLGI::Graphics* graphics)
        : ::EffekseerRendererLLGI::Backend::GraphicsDevice(graphics)
    {
    }

    virtual ~EffekseerGraphicsDevice()
    {
        g_graphicsDevice = nullptr;
    }

    static ::Effekseer::Backend::GraphicsDeviceRef create()
    {
        if (g_graphicsDevice == nullptr)
        {
            auto graphics = new LLGI::GraphicsMetal();
            graphics->Initialize(nullptr);

            g_graphicsDevice = std::move(Effekseer::MakeRefPtr<EffekseerGraphicsDevice>(graphics));
            ES_SAFE_RELEASE(graphics);
        }

        //creating_ = false;
        
        return g_graphicsDevice;
    }
    
    int Release() override
    {
        auto ret = ::EffekseerRendererLLGI::Backend::GraphicsDevice::Release();
        if (ret == 1)
        {
            g_graphicsDevice = nullptr;
        }

        return ret;
    }
};

Effekseer::ModelLoaderRef CreateModelLoader(Effekseer::FileInterface* effectFile)
{
	return Effekseer::MakeRefPtr<::EffekseerRenderer::ModelLoader>(EffekseerGraphicsDevice::create(), effectFile);
}

::Effekseer::MaterialLoaderRef CreateMaterialLoader(Effekseer::FileInterface* effectFile)
{
    auto device = EffekseerGraphicsDevice::create();
    auto ret = EffekseerRendererMetal::CreateMaterialLoader(device, effectFile);
    return ret;
}

void UpdateTextureData(::Effekseer::TextureRef textureData, cocos2d::Texture2D* texture)
{
    auto textureMTL = static_cast<cocos2d::backend::TextureMTL*>(texture->getBackendTexture());
	auto device = EffekseerGraphicsDevice::create().DownCast<::EffekseerRendererLLGI::Backend::GraphicsDevice>();

    auto backend = device->CreateTexture(reinterpret_cast<uint64_t>(textureMTL->getMTLTexture()), []() -> void {});
	textureData->SetBackend(backend);
}

void CleanupTextureData(::Effekseer::TextureRef textureData)
{
}

::EffekseerRenderer::DistortingCallback* CreateDistortingCallback(::EffekseerRenderer::RendererRef renderer, Effekseer::RefPtr<::EffekseerRenderer::CommandList> commandList)
{
    return new DistortingCallbackMetal(commandList);
}


void EffectEmitter::beforeRender(EffekseerRenderer::RendererRef renderer, Effekseer::RefPtr<EffekseerRenderer::CommandList> commandList)
{
    SetMTLObjectsFromCocos2d(commandList);
}

void EffectEmitter::afterRender(EffekseerRenderer::RendererRef renderer, Effekseer::RefPtr<EffekseerRenderer::CommandList> commandList)
{
    EffekseerRendererMetal::EndCommandList(commandList);
}

void EffectManager::onDestructor()
{
    EffekseerRendererMetal::EndCommandList(commandList_);
}

void EffectManager::CreateRenderer(int32_t spriteSize)
{
    auto device = EffekseerGraphicsDevice::create();
    renderer2d = EffekseerRendererMetal::Create(device,
                                                spriteSize,
                                                cocos2d::backend::Utils::getDefaultColorAttachmentPixelFormat(),
                                                cocos2d::backend::Utils::getDefaultDepthStencilAttachmentPixelFormat(),
                                                false);

    memoryPool_ = EffekseerRenderer::CreateSingleFrameMemoryPool(device);
    commandList_ = EffekseerRenderer::CreateCommandList(device, memoryPool_);
    renderer2d->SetCommandList(commandList_);
}

void EffectManager::newFrame()
{
    if(memoryPool_ != nullptr)
    {
        memoryPool_->NewFrame();
    }
    
    auto r = static_cast<::EffekseerRendererLLGI::RendererImplemented*>(renderer2d.Get());
    auto vb = static_cast<::EffekseerRendererLLGI::VertexBuffer*>(r->GetVertexBuffer());
}

void ResetBackground(::EffekseerRenderer::RendererRef renderer)
{
    auto r = static_cast<::EffekseerRendererLLGI::RendererImplemented*>(renderer.Get());
    r->SetBackground(nullptr);
}

}

#endif

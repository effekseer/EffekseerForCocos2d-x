#include "../EffekseerForCocos2d-x.h"
#ifdef CC_USE_METAL

#include "../../EffekseerRendererMetal/EffekseerRenderer/EffekseerRendererMetal.RendererImplemented.h"
#include "../../EffekseerRendererMetal/EffekseerRenderer/EffekseerRendererMetal.VertexBuffer.h"
#include "../../EffekseerRendererMetal/EffekseerRendererMetal.h"
#include "../../3rdParty/LLGI/src/Metal/LLGI.GraphicsMetal.h"
#include "renderer/backend/metal/TextureMTL.h"
#include "renderer/backend/metal/CommandBufferMTL.h"
#include "renderer/backend/metal/Utils.h"
#include <Metal/LLGI.TextureMetal.h>

namespace efk {

void BeginRenderPass(::EffekseerRenderer::Renderer* renderer)
{
    auto r = static_cast<EffekseerRendererMetal::RendererImplemented*>(renderer);
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
    r->SetExternalCommandBuffer(bufferM->getMTLCommandBuffer());
    r->SetExternalRenderEncoder(bufferM->getRenderCommandEncoder());
}


#pragma region DistortingCallbackMetal
class DistortingCallbackMetal
    : public EffekseerRenderer::DistortingCallback
{

    EffekseerRendererMetal::RendererImplemented*    renderer = nullptr;
    id<MTLTexture>                                  texture = nullptr;
    LLGI::Texture*                                  textureLLGI = nullptr;

public:
    DistortingCallbackMetal(EffekseerRendererMetal::RendererImplemented* renderer);

    virtual ~DistortingCallbackMetal();

    virtual bool OnDistorting() override;
};

DistortingCallbackMetal::DistortingCallbackMetal(EffekseerRendererMetal::RendererImplemented* r)
: renderer(r)
{
}

DistortingCallbackMetal::~DistortingCallbackMetal()
{
    if(textureLLGI != nullptr)
    {
        [texture release];
        ES_SAFE_RELEASE(textureLLGI);
    }
}

bool DistortingCallbackMetal::OnDistorting()
{
    // to get viewport
    auto drawable = cocos2d::backend::DeviceMTL::getCurrentDrawable();

    if(textureLLGI == nullptr)
    {
        auto deviceMTL = static_cast<cocos2d::backend::DeviceMTL*>(cocos2d::backend::Device::getInstance());
        
        MTLTextureDescriptor* textureDescriptor =
        [MTLTextureDescriptor texture2DDescriptorWithPixelFormat:cocos2d::backend::Utils::getDefaultColorAttachmentPixelFormat()
                                                           width:drawable.texture.width
                                                          height:drawable.texture.height
                                                       mipmapped:NO];
        
        texture = [deviceMTL->getMTLDevice() newTextureWithDescriptor:textureDescriptor];
        
        auto tex = new LLGI::TextureMetal;
        tex->Reset(texture);
        textureLLGI = tex;
    }

    auto commandBuffer = static_cast<cocos2d::backend::CommandBufferMTL*>(cocos2d::Director::getInstance()->getCommandBuffer());
    commandBuffer->endEncoding();
    
    
    MTLRegion region =
    {
        {0, 0, 0},          // MTLOrigin
        {texture.width, texture.height, 1}  // MTLSize
    };
    
    id<MTLBlitCommandEncoder> blitEncoder = [commandBuffer->getMTLCommandBuffer() blitCommandEncoder];
    
    [blitEncoder copyFromTexture:drawable.texture sourceSlice:0 sourceLevel:0 sourceOrigin:region.origin sourceSize:region.size toTexture:texture destinationSlice:0 destinationLevel:0 destinationOrigin:{0, 0, 0}];
    [blitEncoder endEncoding];
    cocos2d::backend::Device::getInstance()->setFrameBufferOnly(true); // reset
    
    SetMTLObjectsFromCocos2d(renderer);
    
    renderer->SetBackground(textureLLGI);

    return true;
}
#pragma endregion

static ::EffekseerRenderer::GraphicsDevice* g_graphicsDevice = nullptr;

class EffekseerGraphicsDevice : public ::EffekseerRendererLLGI::GraphicsDevice
{
private:

public:
    EffekseerGraphicsDevice(LLGI::Graphics* graphics)
        : ::EffekseerRendererLLGI::GraphicsDevice(graphics)
    {
    }

    virtual ~EffekseerGraphicsDevice()
    {
        g_graphicsDevice = nullptr;
    }

    static ::EffekseerRenderer::GraphicsDevice* create()
    {
        if (g_graphicsDevice == nullptr)
        {
            auto graphics = new LLGI::GraphicsMetal();
            graphics->Initialize(nullptr);

            g_graphicsDevice = new EffekseerGraphicsDevice(graphics);
            ES_SAFE_RELEASE(graphics);
        }
        else
        {
            g_graphicsDevice->AddRef();
        }

        return g_graphicsDevice;
    }
};
Effekseer::ModelLoader* CreateModelLoader(Effekseer::FileInterface* effectFile)
{
    auto device = EffekseerGraphicsDevice::create();
    auto ret = EffekseerRendererMetal::CreateModelLoader(device, effectFile);
    ES_SAFE_RELEASE(device);
    return ret;
}

::Effekseer::MaterialLoader* CreateMaterialLoader(Effekseer::FileInterface* effectFile)
{
    auto device = EffekseerGraphicsDevice::create();
    auto ret = EffekseerRendererMetal::CreateMaterialLoader(device, effectFile);
    ES_SAFE_RELEASE(device);
    return ret;
}

void UpdateTextureData(::Effekseer::TextureData* textureData, cocos2d::Texture2D* texture)
{
    auto textureMTL = static_cast<cocos2d::backend::TextureMTL*>(texture->getBackendTexture());
    auto tex = new LLGI::TextureMetal();
    tex->Reset(textureMTL->getMTLTexture());
    textureData->UserPtr = tex;
}

void CleanupTextureData(::Effekseer::TextureData* textureData)
{
    auto tex = (LLGI::TextureMetal*)textureData->UserPtr;
    tex->Release();
}

::EffekseerRenderer::DistortingCallback* CreateDistortingCallback(::EffekseerRenderer::Renderer* renderer)
{
    auto r = static_cast<::EffekseerRendererMetal::RendererImplemented*>(renderer);
    return new DistortingCallbackMetal(r);
}


void EffectEmitter::preRender()
{
#if EFK_ENABLE_HDR
    auto rt = manager->hdrRenderTexture;
    if (rt == nullptr) return;
    
    //TODO: OPTIMIZATION - Check if this effect has Distortion for this frame
    
    // add commands before actual render command
    
    // detach HDR color attachment
    rt->endSane();
    
    auto texture = manager->hdrRenderTexture->getSprite()->getTexture();
    auto d = cocos2d::Director::getInstance();
    d->pushMatrix(cocos2d::MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION);
    d->pushMatrix(cocos2d::MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
    d->loadIdentityMatrix(cocos2d::MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION);
    d->loadIdentityMatrix(cocos2d::MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
    cocos2d::Rect r(-1, -1, 2, 2);
    // render color attachment texture to drawable
    texture->drawInRect(r, _globalZOrder);
    d->popMatrix(cocos2d::MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION);
    d->popMatrix(cocos2d::MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
    
    // re-attach HDR color attachment
    rt->beginSane();
#endif
}

void EffectManager::CreateRenderer(int32_t spriteSize)
{
    auto device = EffekseerGraphicsDevice::create();
    renderer2d = EffekseerRendererMetal::Create(device,
                                                spriteSize,
#if EFK_ENABLE_HDR
                                                MTLPixelFormatRGBA16Float,
#else
                                                cocos2d::backend::Utils::getDefaultColorAttachmentPixelFormat(),
#endif
                                                cocos2d::backend::Utils::getDefaultDepthStencilAttachmentPixelFormat(),
                                                false);

    memoryPool_ = EffekseerRendererMetal::CreateSingleFrameMemoryPool(renderer2d);
    commandList_ = EffekseerRendererMetal::CreateCommandList(renderer2d, memoryPool_);
    renderer2d->SetCommandList(commandList_);
    renderer2d->SetBackgroundTextureUVStyle(EffekseerRenderer::UVStyle::VerticalFlipped);
    
    ES_SAFE_RELEASE(device);
}

void EffectManager::newFrame()
{
    if(memoryPool_ != nullptr)
    {
        memoryPool_->NewFrame();
    }
    
    auto r = static_cast<::EffekseerRendererMetal::RendererImplemented*>(renderer2d);
    auto vb = static_cast<::EffekseerRendererMetal::VertexBuffer*>(r->GetVertexBuffer());
    vb->NewFrame();
}

void ResetBackground(::EffekseerRenderer::Renderer* renderer)
{
    auto r = static_cast<::EffekseerRendererMetal::RendererImplemented*>(renderer);
    r->SetBackground(nullptr);
}

}

#endif

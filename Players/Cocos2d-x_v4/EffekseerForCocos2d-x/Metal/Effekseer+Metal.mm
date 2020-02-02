#include "cocos2d.h"
#ifdef CC_USE_METAL

#include "../EffekseerForCocos2d-x.h"
#include "../../EffekseerRendererMetal/EffekseerRenderer/EffekseerRendererMetal.RendererImplemented.h"
#include "../../EffekseerRendererMetal/EffekseerRendererMetal.h"
#include "../../3rdParty/LLGI/src/Metal/LLGI.GraphicsMetal.h"
#include "renderer/backend/metal/TextureMTL.h"
#include "renderer/backend/metal/CommandBufferMTL.h"
#include "renderer/backend/metal/Utils.h"
#include <Metal/LLGI.TextureMetal.h>

namespace efk {

void SetMTLObjectsFromCocos2d(EffekseerRendererMetal::RendererImplemented* renderer)
{
    auto d = cocos2d::Director::getInstance();
    auto buffer = d->getCommandBuffer();
    auto bufferM = static_cast<cocos2d::backend::CommandBufferMTL*>(buffer);
    
    // use render pass descriptor from Cocos and add depth test
    auto descriptor = d->getRenderer()->getRenderPassDescriptor();
    descriptor.depthTestEnabled = true;
    // using Cocos render pass
    bufferM->beginRenderPass(descriptor);
    
    // set Command Buffer and Render Encoder from Cocos
    renderer->SetExternalCommandBuffer(bufferM->getMTLCommandBuffer());
    renderer->SetExternalRenderEncoder(bufferM->getRenderCommandEncoder());
}


#pragma region DistortingCallbackMetal
class DistortingCallbackMetal
    : public EffekseerRenderer::DistortingCallback
{

    EffekseerRendererMetal::RendererImplemented*    renderer = nullptr;
    cocos2d::Texture2D*                             texture = nullptr;
    LLGI::Texture*                                  textureLLGI = nullptr;

public:
    DistortingCallbackMetal(EffekseerRendererMetal::RendererImplemented* renderer);

    virtual ~DistortingCallbackMetal();

    virtual bool OnDistorting() override;
};

DistortingCallbackMetal::DistortingCallbackMetal(EffekseerRendererMetal::RendererImplemented* r)
: renderer(r)
{
    auto s = cocos2d::Director::getInstance()->getOpenGLView()->getFrameSize();
    int len = s.width * s.height * 4;
    uint8_t* data = new uint8_t[len];
    texture = new cocos2d::Texture2D;
    texture->initWithData(data, len, cocos2d::backend::PixelFormat::BGRA8888, s.width, s.height, s);
    CC_SAFE_DELETE_ARRAY(data);
    
    auto textureMTL = static_cast<cocos2d::backend::TextureMTL*>(texture->getBackendTexture());
    auto tex = new LLGI::TextureMetal;
    tex->Reset(textureMTL->getMTLTexture());
    textureLLGI = tex;
}

DistortingCallbackMetal::~DistortingCallbackMetal()
{
    CC_SAFE_RELEASE(texture);
    ES_SAFE_RELEASE(textureLLGI);
}

bool DistortingCallbackMetal::OnDistorting()
{
    auto d = cocos2d::Director::getInstance();
    cocos2d::Renderer* renderer = d->getRenderer();
    if (!renderer->isRenderingBlocked())
    {
        // commit previous encodings and block rendering
        auto commandBuffer = d->getCommandBuffer();
        commandBuffer->commitEncoding(CC_CALLBACK_0(DistortingCallbackMetal::OnDistorting, this));
        renderer->blockRenderThread();
        
        // when thread is unblocked
        SetMTLObjectsFromCocos2d(this->renderer);
        return true;
    }
    
    texture->updateWithScreen(0, 0);
    this->renderer->SetBackground(textureLLGI);
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
    return nullptr;
}


void EffectEmitter::preRender(EffekseerRenderer::Renderer* renderer)
{
    auto r = static_cast<::EffekseerRendererMetal::RendererImplemented*>(renderer);
    SetMTLObjectsFromCocos2d(r);
}

void EffectManager::CreateRenderer(int32_t spriteSize)
{
    auto device = EffekseerGraphicsDevice::create();
    renderer2d = EffekseerRendererMetal::Create(device,
                                                spriteSize,
                                                  cocos2d::backend::Utils::getDefaultColorAttachmentPixelFormat(),
                                                  cocos2d::backend::Utils::getDefaultDepthStencilAttachmentPixelFormat(), false);

    memoryPool_ = EffekseerRendererMetal::CreateSingleFrameMemoryPool(renderer2d);
    commandList_ = EffekseerRendererMetal::CreateCommandList(renderer2d, memoryPool_);
    renderer2d->SetCommandList(commandList_);
    
    ES_SAFE_RELEASE(device);
}

void ResetBackground(::EffekseerRenderer::Renderer* renderer)
{
    auto r = static_cast<::EffekseerRendererMetal::RendererImplemented*>(renderer);
    r->SetBackground(nullptr);
}

}

#endif

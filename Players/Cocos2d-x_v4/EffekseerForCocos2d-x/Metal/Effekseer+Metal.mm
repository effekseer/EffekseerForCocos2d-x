#include "cocos2d.h"
#ifdef CC_USE_METAL

#include "../EffekseerForCocos2d-x.h"
#include "../../EffekseerRendererMetal/EffekseerRenderer/EffekseerRendererMetal.RendererImplemented.h"
#include "../../EffekseerRendererMetal/EffekseerRendererMetal.h"
#include "renderer/backend/metal/TextureMTL.h"
#include "renderer/backend/metal/CommandBufferMTL.h"
#include "renderer/backend/metal/Utils.h"
#include <Metal/LLGI.TextureMetal.h>

namespace efk {

Effekseer::ModelLoader* CreateModelLoader(Effekseer::FileInterface* effectFile)
{
    // TODO
    return nullptr;
}

::Effekseer::MaterialLoader* CreateMaterialLoader(Effekseer::FileInterface* effectFile)
{
    // TODO
    return nullptr;
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
    auto buffer = _director->getCommandBuffer();
    auto bufferM = static_cast<cocos2d::backend::CommandBufferMTL*>(buffer);
    
    // use render pass descriptor from Cocos and add depth test
    auto descriptor = _director->getRenderer()->getRenderPassDescriptor();
    descriptor.depthTestEnabled = true;
    // using Cocos render pass
    bufferM->beginRenderPass(descriptor);
    
    // set Command Buffer and Render Encoder from Cocos
    r->SetExternalCommandBuffer(bufferM->getMTLCommandBuffer());
    r->SetExternalRenderEncoder(bufferM->getRenderCommandEncoder());
}

void EffectManager::CreateRenderer(int32_t spriteSize)
{
    renderer2d = EffekseerRendererMetal::Create(spriteSize,
                                                  cocos2d::backend::Utils::getDefaultColorAttachmentPixelFormat(),
                                                  cocos2d::backend::Utils::getDefaultDepthStencilAttachmentPixelFormat(), false);

    memoryPool_ = EffekseerRendererMetal::CreateSingleFrameMemoryPool(renderer2d);
    commandList_ = EffekseerRendererMetal::CreateCommandList(renderer2d, memoryPool_);
    renderer2d->SetCommandList(commandList_);
}

void ResetBackground(::EffekseerRenderer::Renderer* renderer)
{
    // TODO
}

}

#endif

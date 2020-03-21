#include "../EffekseerForCocos2d-x.h"
#ifndef CC_USE_METAL

#include "../../EffekseerRendererGL/EffekseerRendererGL.h"
#include "../../EffekseerRendererGL/EffekseerRenderer/EffekseerRendererGL.ModelLoader.h"
#include "../../EffekseerRendererGL/EffekseerRenderer/EffekseerRendererGL.MaterialLoader.h"
#include "../../EffekseerRendererGL/EffekseerRenderer/EffekseerRendererGL.DeviceObjectCollection.h"
#include "renderer/backend/opengl/TextureGL.h"
#include "renderer/backend/opengl/CommandBufferGL.h"

namespace efk {

#pragma region DistortingCallbackGL
class DistortingCallbackGL
	: public EffekseerRenderer::DistortingCallback
{
	GLuint framebufferForCopy = 0;
	GLuint backGroundTexture = 0;
	uint32_t backGroundTextureWidth = 0;
	uint32_t backGroundTextureHeight = 0;
	GLuint backGroundTextureInternalFormat = 0;

	EffekseerRendererGL::Renderer*	renderer = nullptr;

public:
	DistortingCallbackGL(EffekseerRendererGL::Renderer* renderer);

	virtual ~DistortingCallbackGL();

	void ReleaseTexture();

	// prepare a taget
	void PrepareTexture(uint32_t width, uint32_t height, GLint internalFormat);

	virtual bool OnDistorting() override;
};

DistortingCallbackGL::DistortingCallbackGL(EffekseerRendererGL::Renderer* renderer)
{
	this->renderer = renderer;
	glGenTextures(1, &backGroundTexture);
}

DistortingCallbackGL::~DistortingCallbackGL()
{
	ReleaseTexture();
}

void DistortingCallbackGL::ReleaseTexture()
{
	glDeleteTextures(1, &backGroundTexture);
}

void DistortingCallbackGL::PrepareTexture(uint32_t width, uint32_t height, GLint internalFormat)
{
	glBindTexture(GL_TEXTURE_2D, backGroundTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

	backGroundTextureWidth = width;
	backGroundTextureHeight = height;
	backGroundTextureInternalFormat = internalFormat;
}

bool DistortingCallbackGL::OnDistorting()
{
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	uint32_t width = viewport[2];
	uint32_t height = viewport[3];

	if (backGroundTextureWidth != width ||
		backGroundTextureHeight != height)
	{
		PrepareTexture(width, height, GL_RGBA);
	}

	glBindTexture(GL_TEXTURE_2D, backGroundTexture);
	//glCopyTexSubImage2D( GL_TEXTURE_2D, 0, 0, 0, 0, 0, width, height );
	glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, viewport[0], viewport[1], width, height);
	glBindTexture(GL_TEXTURE_2D, 0);

	renderer->SetBackground(backGroundTexture);

	return true;
}
#pragma endregion

static ::EffekseerRendererGL::DeviceObjectCollection* g_deviceObjectCollection = nullptr;

class EffekseerDeviceObjectCollection : public ::EffekseerRendererGL::DeviceObjectCollection
{
private:

public:
	EffekseerDeviceObjectCollection()
	{
	}

	virtual ~EffekseerDeviceObjectCollection()
	{
		g_deviceObjectCollection = nullptr;
	}

	static ::EffekseerRendererGL::DeviceObjectCollection* create()
	{
		if (g_deviceObjectCollection == nullptr)
		{
			g_deviceObjectCollection = new ::EffekseerRendererGL::DeviceObjectCollection();
		}
		else
		{
			g_deviceObjectCollection->AddRef();
		}

		return g_deviceObjectCollection;
	}
};

Effekseer::ModelLoader* CreateModelLoader(Effekseer::FileInterface* effectFile)
{
    return new ::EffekseerRendererGL::ModelLoader(effectFile);
}

::Effekseer::MaterialLoader* CreateMaterialLoader(Effekseer::FileInterface* effectFile)
{
    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
            return new ::EffekseerRendererGL::MaterialLoader(
                EffekseerRendererGL::OpenGLDeviceType::OpenGLES2, nullptr, EffekseerDeviceObjectCollection::create(), effectFile);
    #else
            return new ::EffekseerRendererGL::MaterialLoader(
                EffekseerRendererGL::OpenGLDeviceType::OpenGL2, nullptr, EffekseerDeviceObjectCollection::create(), effectFile);
    #endif
}

void UpdateTextureData(::Effekseer::TextureData* textureData, cocos2d::Texture2D* texture)
{
	auto textureGL = static_cast<cocos2d::backend::Texture2DGL*>(texture->getBackendTexture());
	textureData->UserID = textureGL->getHandler();
}

void CleanupTextureData(::Effekseer::TextureData* textureData) {}

::EffekseerRenderer::DistortingCallback* CreateDistortingCallback(::EffekseerRenderer::Renderer* renderer)
{
	auto renderGL = static_cast<::EffekseerRendererGL::Renderer*>(renderer);
	return new DistortingCallbackGL(renderGL);
}

void EffectEmitter::preRender(EffekseerRenderer::Renderer* renderer) 
{
	auto d = cocos2d::Director::getInstance();
	auto buffer = d->getCommandBuffer();
	auto bufferGL = static_cast<cocos2d::backend::CommandBufferGL*>(buffer);

	// use render pass descriptor from Cocos and add depth test
	auto descriptor = d->getRenderer()->getRenderPassDescriptor();
	descriptor.depthTestEnabled = true;
	// using Cocos render pass
	bufferGL->beginRenderPass(descriptor);
	auto v = d->getRenderer()->getViewport();
	// important for ensuring znear and zfar are in sync with Cocos

void EffectManager::CreateRenderer(int32_t spriteSize)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	renderer2d = ::EffekseerRendererGL::Renderer::Create(spriteSize, EffekseerRendererGL::OpenGLDeviceType::OpenGLES2, EffekseerDeviceObjectCollection::create());
#else
	renderer2d = ::EffekseerRendererGL::Renderer::Create(spriteSize, EffekseerRendererGL::OpenGLDeviceType::OpenGL2, EffekseerDeviceObjectCollection::create());
#endif
}

void EffectManager::newFrame() {}
void EffectManager::CreateEfkTexture()
{
	//auto size = cocos2d::Director::getInstance()->getVisibleSize();
	//efkTexture = cocos2d::RenderTexture::create(size.width, size.height);
	//efkTexture->setKeepMatrix(true);
	//CC_SAFE_RETAIN(efkTexture);

	//// set format to 16-bits for each channel
	//auto texture2D = efkTexture->getSprite()->getTexture();
	//auto textureGL = static_cast<cocos2d::backend::Texture2DGL*>(texture2D->getBackendTexture());
	//textureGL->setTextureInternalFormat(GL_RGBA16F);
    //textureGL->updateData(NULL, texture2D->getPixelsWide(), texture2D->getPixelsHigh(), 0);

	cocos2d::backend::TextureDescriptor descriptor;
	auto v = cocos2d::Director::getInstance()->getRenderer()->getViewport();
	auto size = cocos2d::Director::getInstance()->getVisibleSize();
	descriptor.width = v.w;
	descriptor.height = v.h;
	descriptor.textureUsage = cocos2d::TextureUsage::RENDER_TARGET;
	descriptor.textureFormat = cocos2d::PixelFormat::RGBA8888;
	auto texture = new (std::nothrow) cocos2d::backend::Texture2DGL(descriptor);
	texture->setTextureInternalFormat(GL_RGBA16F);
	texture->updateData(NULL, v.w, v.h, 0);

	auto texture2D = new (std::nothrow) cocos2d::Texture2D();
	texture2D->initWithBackendTexture(texture, CC_ENABLE_PREMULTIPLIED_ALPHA != 0);
	texture2D->setRenderTarget(true);
	texture2D->autorelease();
	texture->release();

	efkTexture = cocos2d::Sprite::createWithTexture(texture2D);
	efkTexture->setFlippedY(true);
	efkTexture->setContentSize(size);
	CC_SAFE_RETAIN(efkTexture);
}


void ResetBackground(::EffekseerRenderer::Renderer* renderer)
{
    auto r = static_cast<::EffekseerRendererGL::Renderer*>(renderer);
    r->SetBackground(0);
}

}

#endif

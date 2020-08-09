#include "../EffekseerForCocos2d-x.h"
#ifndef CC_USE_METAL

#include "../../EffekseerRendererGL/EffekseerRendererGL.h"
#include "../../EffekseerRendererGL/EffekseerRenderer/EffekseerRendererGL.ModelLoader.h"
#include "../../EffekseerRendererGL/EffekseerRenderer/EffekseerRendererGL.MaterialLoader.h"
#include "../../EffekseerRendererGL/EffekseerRenderer/EffekseerRendererGL.DeviceObjectCollection.h"
#include "renderer/backend/opengl/TextureGL.h"

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
#ifndef _WIN32
	glGenFramebuffers(1, &framebufferForCopy);
#endif
}

DistortingCallbackGL::~DistortingCallbackGL()
{
	ReleaseTexture();
}

void DistortingCallbackGL::ReleaseTexture()
{
#ifndef _WIN32
	glDeleteFramebuffers(1, &framebufferForCopy);
#endif
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

::EffekseerRenderer::DistortingCallback* CreateDistortingCallback(::EffekseerRenderer::Renderer* renderer, ::EffekseerRenderer::CommandList* commandList)
{
	auto renderGL = static_cast<::EffekseerRendererGL::Renderer*>(renderer);
	return new DistortingCallbackGL(renderGL);
}

void EffectEmitter::beforeRender(EffekseerRenderer::Renderer* renderer, EffekseerRenderer::CommandList* commandList)
{
}

void EffectEmitter::afterRender(EffekseerRenderer::Renderer* renderer, EffekseerRenderer::CommandList* commandList)
{
}

void EffectManager::onDestructor()
{
}

void EffectManager::CreateRenderer(int32_t spriteSize)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	renderer2d = ::EffekseerRendererGL::Renderer::Create(spriteSize, EffekseerRendererGL::OpenGLDeviceType::OpenGLES2, EffekseerDeviceObjectCollection::create());
#else
	renderer2d = ::EffekseerRendererGL::Renderer::Create(spriteSize, EffekseerRendererGL::OpenGLDeviceType::OpenGL2, EffekseerDeviceObjectCollection::create());
#endif
}

void EffectManager::newFrame() {}

void ResetBackground(::EffekseerRenderer::Renderer* renderer)
{
    auto r = static_cast<::EffekseerRendererGL::Renderer*>(renderer);
    r->SetBackground(0);
}

}

#endif


#include "EffekseerForCocos2d-x.h"

#ifdef CC_USE_METAL
#else
#include <EffekseerRenderer/EffekseerRendererGL.DeviceObjectCollection.h>
#include <EffekseerRenderer/EffekseerRendererGL.ModelLoader.h>
#include <EffekseerRenderer/EffekseerRendererGL.MaterialLoader.h>
#endif

namespace efk
{
int ccNextPOT(int x)
{
	x = x - 1;
	x = x | (x >> 1);
	x = x | (x >> 2);
	x = x | (x >> 4);
	x = x | (x >> 8);
	x = x | (x >> 16);
	return x + 1;
}

static std::u16string getFilenameWithoutExt(const char16_t* path)
{
	int start = 0;
	int end = 0;

	for (int i = 0; path[i] != 0; i++)
	{
		if (path[i] == u'/' || path[i] == u'\\')
		{
			start = i + 1;
		}
	}

	for (int i = start; path[i] != 0; i++)
	{
		if (path[i] == u'.')
		{
			end = i;
		}
	}

	std::vector<char16_t> ret;

	for (int i = start; i < end; i++)
	{
		ret.push_back(path[i]);
	}
	ret.push_back(0);

	return std::u16string(ret.data());
}

class DistortingCallbackGL : public EffekseerRenderer::DistortingCallback
{
	GLuint framebufferForCopy = 0;
	GLuint backGroundTexture = 0;
	uint32_t backGroundTextureWidth = 0;
	uint32_t backGroundTextureHeight = 0;
	GLuint backGroundTextureInternalFormat = 0;

	EffekseerRendererGL::Renderer* renderer = nullptr;

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

DistortingCallbackGL::~DistortingCallbackGL() { ReleaseTexture(); }

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
#ifndef _WIN32
	GLint backupFramebuffer;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &backupFramebuffer);
	if (backupFramebuffer <= 0)
		return false;
#endif
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	uint32_t width = viewport[2];
	uint32_t height = viewport[3];

	if (backGroundTextureWidth != width || backGroundTextureHeight != height)
	{
		PrepareTexture(width, height, GL_RGBA);
	}

#ifndef _WIN32

	GLint rbtype;
	glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE, &rbtype);

	if (rbtype == GL_RENDERBUFFER)
	{
		GLint renderbuffer;
		glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME, &renderbuffer);

		glBindFramebuffer(GL_FRAMEBUFFER, framebufferForCopy);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, renderbuffer);
	}
	else if (rbtype == GL_TEXTURE_2D)
	{
		GLint renderTexture;
		glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME, &renderTexture);

		glBindFramebuffer(GL_FRAMEBUFFER, framebufferForCopy);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderTexture, 0);
	}
#endif

	glBindTexture(GL_TEXTURE_2D, backGroundTexture);
	// glCopyTexSubImage2D( GL_TEXTURE_2D, 0, 0, 0, 0, 0, width, height );
	glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, viewport[0], viewport[1], width, height);
	glBindTexture(GL_TEXTURE_2D, 0);

#ifndef _WIN32
	glBindFramebuffer(GL_FRAMEBUFFER, backupFramebuffer);
#endif

	renderer->SetBackground(backGroundTexture);

	return true;
}

class EffekseerFileReader : public Effekseer::FileReader
{
	std::vector<uint8_t> data;
	int32_t position;

public:
	EffekseerFileReader(std::vector<uint8_t>& data)
	{
		this->data = data;
		position = 0;
	}

	virtual ~EffekseerFileReader() {}

	size_t Read(void* buffer, size_t size)
	{
		int32_t readable = size;
		if (data.size() - position < size)
			readable = data.size() - position;

		memcpy(buffer, &(data[position]), readable);
		position += readable;
		return readable;
	}

	void Seek(int position)
	{
		this->position = position;
		if (this->position < 0)
			this->position = 0;
		if (this->position > static_cast<int32_t>(data.size()))
			this->position = static_cast<int32_t>(data.size());
	}

	int GetPosition() { return position; }

	size_t GetLength() { return data.size(); }
};

class EffekseerFile : public Effekseer::FileInterface
{
public:
	EffekseerFile();
	virtual ~EffekseerFile();
	Effekseer::FileReader* OpenRead(const EFK_CHAR* path);
	Effekseer::FileWriter* OpenWrite(const EFK_CHAR* path);
};

EffekseerFile::EffekseerFile() {}
EffekseerFile::~EffekseerFile() {}

Effekseer::FileReader* EffekseerFile::OpenRead(const EFK_CHAR* path)
{
	char path_[300];
	::Effekseer::ConvertUtf16ToUtf8((int8_t*)path_, 300, (const int16_t*)path);

	cocos2d::Data data_ = cocos2d::FileUtils::getInstance()->getDataFromFile(path_);

	if (data_.isNull())
	{
		return nullptr;
	}

	std::vector<uint8_t> data;
	data.resize(data_.getSize());
	memcpy(data.data(), data_.getBytes(), data.size());
	return new EffekseerFileReader(data);
}

Effekseer::FileWriter* EffekseerFile::OpenWrite(const EFK_CHAR* path) { return nullptr; }

static std::map<Effekseer::TextureData*, std::basic_string<EFK_CHAR>> g_glTex2FilePath;
static std::map<std::basic_string<EFK_CHAR>, cocos2d::Texture2D*> g_filePath2CTex;
static std::map<std::basic_string<EFK_CHAR>, Effekseer::TextureData*> g_filePath2EffectData;

class TextureLoader : public ::Effekseer::TextureLoader
{
private:
	::Effekseer::FileInterface* m_fileInterface;
	::Effekseer::DefaultFileInterface m_defaultFileInterface;

public:
	TextureLoader(::Effekseer::FileInterface* fileInterface = NULL);
	virtual ~TextureLoader();

public:
	Effekseer::TextureData* Load(const EFK_CHAR* path, ::Effekseer::TextureType textureType) override;

	void Unload(Effekseer::TextureData* data);
};

TextureLoader::TextureLoader(::Effekseer::FileInterface* fileInterface) : m_fileInterface(fileInterface)
{
	if (m_fileInterface == NULL)
	{
		m_fileInterface = &m_defaultFileInterface;
	}
}

TextureLoader::~TextureLoader() {}

Effekseer::TextureData* TextureLoader::Load(const EFK_CHAR* path, ::Effekseer::TextureType textureType)
{
	auto key = std::basic_string<EFK_CHAR>(path);
	if (g_filePath2CTex.find(key) != g_filePath2CTex.end())
	{
		auto texture = g_filePath2CTex[key];
		texture->retain();
		return g_filePath2EffectData[key];
	}

	std::unique_ptr<Effekseer::FileReader> reader(m_fileInterface->OpenRead(path));

	if (reader.get() != NULL)
	{
		size_t size_texture = reader->GetLength();
		char* data_texture = new char[size_texture];
		reader->Read(data_texture, size_texture);

		cocos2d::Image* image = new cocos2d::Image();
		cocos2d::Texture2D* texture = new cocos2d::Texture2D();
		bool hasMipmap = false;
		if (image != nullptr && texture != nullptr && image->initWithImageData((const uint8_t*)data_texture, size_texture))
		{
			if (texture->initWithImage(image))
			{
				if (texture->getPixelsWide() == ccNextPOT(texture->getPixelsWide()) &&
					texture->getPixelsHigh() == ccNextPOT(texture->getPixelsHigh()))
				{
					texture->generateMipmap();
				}
			}
			else
			{
				CC_SAFE_DELETE(texture);
				CC_SAFE_DELETE(image);
			}
		}
		CC_SAFE_DELETE(image);

		delete[] data_texture;

		Effekseer::TextureData* textureData = new Effekseer::TextureData();
		textureData->UserID = texture->getName();
		textureData->Width = texture->getPixelsWide();
		textureData->Height = texture->getPixelsHigh();
		textureData->TextureFormat = Effekseer::TextureFormatType::ABGR8;
		textureData->HasMipmap = texture->hasMipmaps();
		g_filePath2CTex[key] = texture;
		g_filePath2EffectData[key] = textureData;
		g_glTex2FilePath[textureData] = key;

		return textureData;
	}
	return NULL;
}

void TextureLoader::Unload(Effekseer::TextureData* data)
{
	if (data != NULL)
	{
		auto path = g_glTex2FilePath[data];
		auto tex = g_filePath2CTex[path];

		if (tex->getReferenceCount() == 1)
		{
			g_glTex2FilePath.erase(data);
			g_filePath2EffectData.erase(path);
			g_filePath2CTex.erase(path);
			delete data;
		}
		tex->release();
	}
}

class CachedMaterialLoader : public ::Effekseer::MaterialLoader
{
private:
	struct Cached
	{
		::Effekseer::MaterialData* DataPtr;
		int32_t Count;

		Cached()
		{
			DataPtr = nullptr;
			Count = 1;
		}
	};

	::Effekseer::MaterialLoader* loader_;
	std::map<std::basic_string<EFK_CHAR>, Cached> cache_;
	std::map<void*, std::basic_string<EFK_CHAR>> data2key_;

public:
	CachedMaterialLoader(::Effekseer::MaterialLoader* loader) { this->loader_ = loader; }

	virtual ~CachedMaterialLoader() { ES_SAFE_DELETE(loader_); }

	virtual ::Effekseer::MaterialData* Load(const EFK_CHAR* path) override
	{
		auto key = std::basic_string<EFK_CHAR>(path);

		auto it = cache_.find(key);

		if (it != cache_.end())
		{
			it->second.Count++;
			return it->second.DataPtr;
		}

		Cached v;
		v.DataPtr = loader_->Load(path);

		if (v.DataPtr != nullptr)
		{
			cache_[key] = v;
			data2key_[v.DataPtr] = key;
		}

		return v.DataPtr;
	}

	virtual void Unload(::Effekseer::MaterialData* data) override
	{
		if (data == nullptr)
			return;
		auto key = data2key_[data];

		auto it = cache_.find(key);

		if (it != cache_.end())
		{
			it->second.Count--;
			if (it->second.Count == 0)
			{
				loader_->Unload(it->second.DataPtr);
				data2key_.erase(data);
				cache_.erase(key);
			}
		}
	}
};

class EffekseerSetting;

static EffekseerSetting* g_effekseerSetting = nullptr;
static ::EffekseerRendererGL::DeviceObjectCollection* g_deviceObjectCollection = nullptr;

class EffekseerDeviceObjectCollection : public ::EffekseerRendererGL::DeviceObjectCollection
{
private:
public:
	EffekseerDeviceObjectCollection() {}

	virtual ~EffekseerDeviceObjectCollection() { g_deviceObjectCollection = nullptr; }

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

class EffekseerSetting : public ::Effekseer::Setting
{
protected:
	Effekseer::FileInterface* effectFile = nullptr;

	EffekseerSetting()
	{
		effectFile = new EffekseerFile();
		SetEffectLoader(Effekseer::Effect::CreateEffectLoader(effectFile));
		SetTextureLoader(new TextureLoader(effectFile));
		SetModelLoader(new ::EffekseerRendererGL::ModelLoader(effectFile));

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		SetMaterialLoader(new CachedMaterialLoader(new ::EffekseerRendererGL::MaterialLoader(
			EffekseerRendererGL::OpenGLDeviceType::OpenGLES2, nullptr, EffekseerDeviceObjectCollection::create(), effectFile)));
#else
		SetMaterialLoader(new CachedMaterialLoader(new ::EffekseerRendererGL::MaterialLoader(
			EffekseerRendererGL::OpenGLDeviceType::OpenGL2, nullptr, EffekseerDeviceObjectCollection::create(), effectFile)));
#endif
		// TODO sound
	}

	virtual ~EffekseerSetting()
	{
		delete effectFile;
		g_effekseerSetting = nullptr;
	}

public:
	static EffekseerSetting* create()
	{
		if (g_effekseerSetting == nullptr)
		{
			g_effekseerSetting = new EffekseerSetting();
		}
		else
		{
			g_effekseerSetting->AddRef();
		}

		return g_effekseerSetting;
	}
};

struct EffectResource
{
	Effekseer::Effect* effect = nullptr;
	int counter = 0;
};

static InternalManager* g_internalManager = nullptr;

class InternalManager : public Effekseer::ReferenceObject
{
	std::map<std::u16string, EffectResource> path2effect;
	std::map<Effekseer::Effect*, std::u16string> effect2path;

	std::set<Effekseer::Manager*> managers;
	std::vector<Effekseer::Manager*> managersVector;

	Effekseer::Server* server = nullptr;

public:
	InternalManager() {}

	~InternalManager()
	{
		if (server != nullptr)
		{
			server->Stop();
			ES_SAFE_DELETE(server);
		}

		g_internalManager = nullptr;
	}

	Effekseer::Effect* loadEffect(const EFK_CHAR* path, float maginification)
	{
		auto it_effect = path2effect.find(path);

		if (it_effect == path2effect.end())
		{
			EffectResource resource;
			resource.effect = Effekseer::Effect::Create(EffekseerSetting::create(), path, maginification);
			resource.counter = 1;

			if (resource.effect != nullptr)
			{
				path2effect[path] = resource;
				effect2path[resource.effect] = path;

				if (server != nullptr)
				{
					auto key = getFilenameWithoutExt(path);
					server->Register(key.c_str(), resource.effect);
				}

				return resource.effect;
			}
			return nullptr;
		}
		else
		{
			it_effect->second.counter++;
			return it_effect->second.effect;
		}
	}

	void unloadEffect(Effekseer::Effect* effect)
	{
		auto it_path = effect2path.find(effect);
		if (it_path == effect2path.end())
			return;

		auto it_effect = path2effect.find(it_path->second);
		if (it_effect == path2effect.end())
			return;

		it_effect->second.counter--;
		if (it_effect->second.counter == 0)
		{
			if (server != nullptr)
			{
				server->Unregister(it_effect->second.effect);
			}

			ES_SAFE_RELEASE(it_effect->second.effect);
			effect2path.erase(it_path);
			path2effect.erase(it_effect);
		}
	}

	void registerManager(Effekseer::Manager* manager)
	{
		managers.insert(manager);

		managersVector.clear();
		for (auto m : managers)
		{
			managersVector.push_back(m);
		}
	}

	void unregisterManager(Effekseer::Manager* manager)
	{
		managers.erase(manager);
		managersVector.clear();
		for (auto m : managers)
		{
			managersVector.push_back(m);
		}
	}

	bool makeNetworkServerEnabled(uint16_t port)
	{
		if (server != nullptr)
			return false;
		server = Effekseer::Server::Create();
		if (!server->Start(port))
		{
			ES_SAFE_DELETE(server);
			return false;
		}

		return true;
	}

	void update()
	{
		if (server != nullptr)
		{
			if (managersVector.size() > 0)
			{
				server->Update(managersVector.data(), managersVector.size());
			}
			else
			{
				server->Update();
			}
		}
	}
};

InternalManager* getGlobalInternalManager()
{
	if (g_internalManager == nullptr)
	{
		g_internalManager = new InternalManager();
	}
	else
	{
		g_internalManager->AddRef();
	}

	return g_internalManager;
}

Effect* Effect::create(const std::string& filename, float maginification)
{
	EFK_CHAR path_[300];
	::Effekseer::ConvertUtf8ToUtf16((int16_t*)path_, 300, (const int8_t*)filename.c_str());

	auto internalManager = getGlobalInternalManager();

	auto effect = internalManager->loadEffect(path_, maginification);

	if (effect != nullptr)
	{
		auto e = new Effect(internalManager);
		e->effect = effect;
		e->autorelease();
		ES_SAFE_RELEASE(internalManager);
		return e;
	}

	ES_SAFE_RELEASE(internalManager);
	return nullptr;
}

Effect::Effect(InternalManager* internalManager)
{
	internalManager_ = internalManager;
	ES_SAFE_ADDREF(internalManager_);
}

Effect::~Effect()
{
	if (internalManager_ != nullptr)
	{
		internalManager_->unloadEffect(effect);
	}

	ES_SAFE_RELEASE(internalManager_);
}

EffectEmitter* EffectEmitter::create(EffectManager* manager) { return new EffectEmitter(manager); }

EffectEmitter* EffectEmitter::create(EffectManager* manager, const std::string& filename, float maginification)
{
	auto effectEmitter = new EffectEmitter(manager);
	auto effect = Effect::create(filename, maginification);
	effectEmitter->setEffect(effect);
	effectEmitter->playOnEnter = true;
	effect->release();
	return effectEmitter;
}

EffectEmitter::EffectEmitter(EffectManager* manager)
{
	this->manager = manager;

	if (manager != nullptr)
	{
		manager->retain();
	}

	autorelease();
}

EffectEmitter::~EffectEmitter()
{
	if (effect_ != nullptr)
	{
		effect_->release();
	}

	if (manager != nullptr)
	{
		manager->release();
	}
}

Effect* EffectEmitter::getEffect() { return effect_; }

void EffectEmitter::setEffect(Effect* effect)
{
	if (effect_ != nullptr)
		effect_->release();

	effect_ = effect;

	if (effect_ != nullptr)
		effect_->retain();
}

::Effekseer::Handle EffectEmitter::getInternalHandle() const { return handle; }

void EffectEmitter::play() { play(0); }

void EffectEmitter::play(int32_t startTime)
{
	if (effect_ == nullptr)
		return;
	if (manager == nullptr)
		return;

	if (startTime == 0)
	{
		handle = manager->play(effect_, 0, 0, 0, 0);
	}
	else
	{
		handle = manager->play(effect_, 0, 0, 0, startTime);
	}

	auto transform = this->getNodeToWorldTransform();
	manager->setMatrix(handle, transform);
	isPlayedAtLeastOnce = true;

	setTargetPosition(targetPosition_);
	setColor(color_);
	setSpeed(speed_);
}

bool EffectEmitter::getPlayOnEnter() { return playOnEnter; }

void EffectEmitter::setPlayOnEnter(bool value) { playOnEnter = value; }

bool EffectEmitter::getIsLooping() { return isLooping; }

void EffectEmitter::setIsLooping(bool value) { isLooping = value; }

bool EffectEmitter::getRemoveOnStop() { return removeOnStop; }

void EffectEmitter::setRemoveOnStop(bool value) { removeOnStop = value; }

void EffectEmitter::setColor(cocos2d::Color4B color)
{
	color_ = color;
	Effekseer::Color col;
	col.R = color.r;
	col.G = color.g;
	col.B = color.b;
	col.A = color.a;
	manager->getInternalManager()->SetAllColor(handle, col);
}

float EffectEmitter::getSpeed()
{
	return speed_;
	// return manager->getInternalManager()->GetSpeed(handle);
}

void EffectEmitter::setSpeed(float speed)
{
	speed_ = speed;
	manager->getInternalManager()->SetSpeed(handle, speed);
}

void EffectEmitter::setTargetPosition(cocos2d::Vec3 position)
{
	targetPosition_ = position;
	manager->getInternalManager()->SetTargetLocation(handle, position.x, position.y, position.z);
}

bool EffectEmitter::isPlaying() { return manager->getInternalManager()->Exists(handle); }

void EffectEmitter::stop() { manager->getInternalManager()->StopEffect(handle); }

void EffectEmitter::stopRoot() { manager->getInternalManager()->StopRoot(handle); }

void EffectEmitter::onEnter()
{
	cocos2d::Node::onEnter();

	if (playOnEnter)
	{
		play();
	}

	scheduleUpdate();
}

void EffectEmitter::onExit()
{
	auto m = manager->getInternalManager();
	if (m->Exists(handle))
	{
		manager->getInternalManager()->StopEffect(handle);
	}

	cocos2d::Node::onExit();
}

void EffectEmitter::update(float delta)
{
	auto m = manager->getInternalManager();
	if (!m->Exists(handle))
	{
		if (isLooping)
		{
			play();
		}
		else if (removeOnStop && isPlayedAtLeastOnce)
		{
			auto transform = this->getNodeToWorldTransform();
			manager->setMatrix(handle, transform);
			cocos2d::Node::update(delta);

			this->removeFromParent();
			return;
		}
	}

	{
		auto transform = this->getNodeToWorldTransform();
		manager->setMatrix(handle, transform);

		cocos2d::Node::update(delta);
	}
}

void EffectEmitter::draw(cocos2d::Renderer* renderer, const cocos2d::Mat4& parentTransform, uint32_t parentFlags)
{
	renderCommand.init(_globalZOrder);
	auto renderer2d = manager->getInternalRenderer();
	Effekseer::Matrix44 mCamera = renderer2d->GetCameraMatrix();
	Effekseer::Matrix44 mProj = renderer2d->GetProjectionMatrix();
	renderCommand.func = [=]() -> void {
		renderer2d->SetCameraMatrix(mCamera);
		renderer2d->SetProjectionMatrix(mProj);
		if (!manager->isDistorted)
		{
			renderer2d->SetRestorationOfStatesFlag(true);
			renderer2d->BeginRendering();
			manager->distortingCallback->OnDistorting();
			renderer2d->EndRendering();

			manager->isDistorted = true;
		}

		renderer2d->SetRestorationOfStatesFlag(true);
		renderer2d->BeginRendering();
		manager->getInternalManager()->DrawHandle(handle);
		renderer2d->EndRendering();

		// Reset Parameters
#ifdef CC_USE_METAL

#else
		cocos2d::GL::useProgram(0);
		cocos2d::GL::enableVertexAttribs(0);
		cocos2d::GL::bindVAO(0);
		cocos2d::GL::bindTexture2D((GLuint)0);
#endif

		// Count drawcall and vertex
		renderer->addDrawnBatches(renderer2d->GetDrawCallCount());
		renderer->addDrawnVertices(renderer2d->GetDrawVertexCount());
		renderer2d->ResetDrawCallCount();
		renderer2d->ResetDrawVertexCount();
	};

	renderer->addCommand(&renderCommand);

	cocos2d::Node::draw(renderer, parentTransform, parentFlags);
}

::Effekseer::Handle EffectManager::play(Effect* effect, float x, float y, float z)
{
	return manager2d->Play(effect->getInternalPtr(), x, y, z);
}

::Effekseer::Handle EffectManager::play(Effect* effect, float x, float y, float z, int startTime)
{
	return manager2d->Play(effect->getInternalPtr(), Effekseer::Vector3D(x, y, z), startTime);
}

void EffectManager::setMatrix(::Effekseer::Handle handle, const cocos2d::Mat4& mat)
{
	Effekseer::Matrix43 mat_;
	const float* p = mat.m;
	int size = sizeof(float) * 3;
	memcpy(mat_.Value[0], p, size);
	p += 4;
	memcpy(mat_.Value[1], p, size);
	p += 4;
	memcpy(mat_.Value[2], p, size);
	p += 4;
	memcpy(mat_.Value[3], p, size);
	manager2d->SetMatrix(handle, mat_);
}

void EffectManager::setPotation(::Effekseer::Handle handle, float x, float y, float z) { manager2d->SetLocation(handle, x, y, z); }

void EffectManager::setRotation(::Effekseer::Handle handle, float x, float y, float z) { manager2d->SetRotation(handle, x, y, z); }

void EffectManager::setScale(::Effekseer::Handle handle, float x, float y, float z) { manager2d->SetScale(handle, x, y, z); }

bool EffectManager::Initialize(cocos2d::Size visibleSize)
{
	// large buffer make application slow on Android
	int32_t spriteSize = 600;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	spriteSize = 2400;
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	renderer2d = ::EffekseerRendererGL::Renderer::Create(
		spriteSize, EffekseerRendererGL::OpenGLDeviceType::OpenGLES2, EffekseerDeviceObjectCollection::create());
#else
	renderer2d = ::EffekseerRendererGL::Renderer::Create(
		spriteSize, EffekseerRendererGL::OpenGLDeviceType::OpenGL2, EffekseerDeviceObjectCollection::create());
#endif

	manager2d = ::Effekseer::Manager::Create(8000);

	// set camera and projection matrix for 2d
	// If you special camera or 3d, please set yourself with setCameraMatrix and setProjectionMatrix
	renderer2d->SetProjectionMatrix(::Effekseer::Matrix44().OrthographicRH(visibleSize.width, visibleSize.height, 1.0f, 400.0f));

	renderer2d->SetCameraMatrix(
		::Effekseer::Matrix44().LookAtRH(::Effekseer::Vector3D(visibleSize.width / 2.0f, visibleSize.height / 2.0f, 200.0f),
										 ::Effekseer::Vector3D(visibleSize.width / 2.0f, visibleSize.height / 2.0f, -200.0f),
										 ::Effekseer::Vector3D(0.0f, 1.0f, 0.0f)));

	distortingCallback = new DistortingCallbackGL(renderer2d);

	manager2d->SetSpriteRenderer(renderer2d->CreateSpriteRenderer());
	manager2d->SetRibbonRenderer(renderer2d->CreateRibbonRenderer());
	manager2d->SetRingRenderer(renderer2d->CreateRingRenderer());
	manager2d->SetModelRenderer(renderer2d->CreateModelRenderer());
	manager2d->SetTrackRenderer(renderer2d->CreateTrackRenderer());

	internalManager_ = getGlobalInternalManager();
	internalManager_->registerManager(manager2d);

	return true;
}

EffectManager* EffectManager::create(cocos2d::Size visibleSize)
{
	auto ret = new EffectManager();
	if (ret->Initialize(visibleSize))
	{
		return ret;
	}

	ret->release();

	return nullptr;
}

EffectManager::EffectManager() {}

EffectManager::~EffectManager()
{
	if (distortingCallback != nullptr)
	{
		delete distortingCallback;
		distortingCallback = nullptr;
	}

	if (manager2d != nullptr)
	{
		internalManager_->unregisterManager(manager2d);
		manager2d->Destroy();
		manager2d = nullptr;
	}

	if (renderer2d != nullptr)
	{
		renderer2d->Destroy();
		renderer2d = nullptr;
	}

	ES_SAFE_RELEASE(internalManager_);
}

void EffectManager::setIsDistortionEnabled(bool value) { isDistortionEnabled = value; }

void EffectManager::begin(cocos2d::Renderer* renderer, float globalZOrder)
{
	if (isDistortionEnabled)
	{
		isDistorted = false;
	}
	else
	{
		isDistorted = true;
		renderer2d->SetBackground(0);
	}

	// TODO Batch render
	/*
	beginCommand.init(globalZOrder);
	beginCommand.func = [this]() -> void
	{
		renderer2d->SetRestorationOfStatesFlag(true);
		renderer2d->BeginRendering();
		manager2d->Draw();

	};
	
	renderer->addCommand(&beginCommand);
	*/
}

void EffectManager::end(cocos2d::Renderer* renderer, float globalZOrder)
{
	// TODO Batch render
	/*
	endCommand.init(globalZOrder);
	endCommand.func = [this]() -> void
	{
		renderer2d->ResetRenderState();
		renderer2d->EndRendering();
		
		// Reset Parameters
		cocos2d::GL::useProgram(0);
	};

	renderer->addCommand(&endCommand);
	*/
}

void EffectManager::setCameraMatrix(const cocos2d::Mat4& mat)
{
	Effekseer::Matrix44 mat_;
	memcpy(mat_.Values, mat.m, sizeof(float) * 16);
	getInternalRenderer()->SetCameraMatrix(mat_);
}

void EffectManager::setProjectionMatrix(const cocos2d::Mat4& mat)
{
	Effekseer::Matrix44 mat_;
	memcpy(mat_.Values, mat.m, sizeof(float) * 16);
	getInternalRenderer()->SetProjectionMatrix(mat_);
}

void EffectManager::update(float delta)
{
	manager2d->Update();
	time_ += delta;
	renderer2d->SetTime(time_);
}

NetworkServer* NetworkServer::create() { return new NetworkServer(); }

NetworkServer::NetworkServer() { internalManager_ = getGlobalInternalManager(); }

NetworkServer::~NetworkServer() { ES_SAFE_RELEASE(internalManager_); }

bool NetworkServer::makeNetworkServerEnabled(uint16_t port) { return internalManager_->makeNetworkServerEnabled(port); }

void NetworkServer::update() { internalManager_->update(); }
} // namespace efk
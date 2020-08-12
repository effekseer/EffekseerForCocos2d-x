
#include "EffekseerForCocos2d-x.h"

#ifdef CC_USE_METAL
#include "renderer/backend/Device.h"
#endif

namespace efk
{


class ImageAccessor : public cocos2d::Image
{
public:
	static bool getPngPremultipledAlphaEnabled() { return PNG_PREMULTIPLIED_ALPHA_ENABLED; }
};

Effekseer::ModelLoader* CreateModelLoader(Effekseer::FileInterface*);

::Effekseer::MaterialLoader* CreateMaterialLoader(Effekseer::FileInterface*);

void UpdateTextureData(::Effekseer::TextureData* textureData, cocos2d::Texture2D* texture);

void CleanupTextureData(::Effekseer::TextureData* textureData);

::EffekseerRenderer::DistortingCallback* CreateDistortingCallback(::EffekseerRenderer::Renderer*, ::EffekseerRenderer::CommandList*);

void ResetBackground(::EffekseerRenderer::Renderer* renderer);

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

	void Unload(Effekseer::TextureData* data) override;
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

		auto backup = ImageAccessor::getPngPremultipledAlphaEnabled();
		cocos2d::Image::setPNGPremultipliedAlphaEnabled(false);

		if (image != nullptr && texture != nullptr && image->initWithImageData((const uint8_t*)data_texture, size_texture))
		{
			if (texture->initWithImage(image))
			{

#ifdef CC_USE_METAL
            // mipmap sampler filters are not set in metal renderer
#else
				if (texture->getPixelsWide() == ccNextPOT(texture->getPixelsWide()) &&
					texture->getPixelsHigh() == ccNextPOT(texture->getPixelsHigh()))
				{
					texture->generateMipmap();
				}
				else
				{
					char path8[300];
					::Effekseer::ConvertUtf16ToUtf8((int8_t*)path8, 300, (const int16_t*)path);
					CCLOG("%s : The texture is not shown on a mobile. The size is not power of two.", path8);
				}
#endif
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
		UpdateTextureData(textureData, texture);
		textureData->Width = texture->getPixelsWide();
		textureData->Height = texture->getPixelsHigh();
		textureData->TextureFormat = Effekseer::TextureFormatType::ABGR8;
		textureData->HasMipmap = texture->hasMipmaps();
		g_filePath2CTex[key] = texture;
		g_filePath2EffectData[key] = textureData;
		g_glTex2FilePath[textureData] = key;

		cocos2d::Image::setPNGPremultipliedAlphaEnabled(backup);

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
            CleanupTextureData(data);
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

class EffekseerSetting : public ::Effekseer::Setting
{
protected:
	Effekseer::FileInterface* effectFile = nullptr;

	EffekseerSetting()
	{
		effectFile = new EffekseerFile();
		SetEffectLoader(Effekseer::Effect::CreateEffectLoader(effectFile));
		SetTextureLoader(new TextureLoader(effectFile));
		SetModelLoader(CreateModelLoader(effectFile));
        SetMaterialLoader(new CachedMaterialLoader(CreateMaterialLoader(effectFile)));
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
            auto setting = EffekseerSetting::create();
			resource.effect = Effekseer::Effect::Create(setting, path, maginification);
            ES_SAFE_RELEASE(setting);
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

	dynamicInputs_.fill(0.0f);
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

	for(size_t i = 0; i < 4; i++)
	{
		setDynamicInput(i, dynamicInputs_[i]);
	}
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

float EffectEmitter::getDynamicInput(int32_t index)
{
	return dynamicInputs_.at(index);
}

void EffectEmitter::setDynamicInput(int32_t index, float value)
{
	dynamicInputs_.at(index) = value;
	manager->getInternalManager()->SetDynamicInput(handle, index, value);
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
    if (!manager->getInternalManager()->GetShown(handle) ||
        manager->getInternalManager()->GetTotalInstanceCount() < 1)
        return; // nothing to draw
            
#ifdef CC_USE_METAL
    if (!manager->isDistorted)
    {
        // allow frame buffer texture to be copied for distortion
        cocos2d::backend::Device::getInstance()->setFrameBufferOnly(false);
    }
#endif
    
	renderCommand.init(_globalZOrder);

	auto renderer2d = manager->getInternalRenderer();
	Effekseer::Matrix44 mCamera = renderer2d->GetCameraMatrix();
	Effekseer::Matrix44 mProj = renderer2d->GetProjectionMatrix();
	renderCommand.func = [=]() -> void {
		renderer2d->SetCameraMatrix(mCamera);
		renderer2d->SetProjectionMatrix(mProj);

#ifdef CC_USE_METAL
        auto commandList = manager->getInternalCommandList();
        beforeRender(renderer2d, commandList);
#endif
		renderer2d->SetRestorationOfStatesFlag(true);
		renderer2d->BeginRendering();
		manager->getInternalManager()->DrawHandle(handle);
		renderer2d->EndRendering();

		// Count drawcall and vertex
		renderer->addDrawnBatches(renderer2d->GetDrawCallCount());
		renderer->addDrawnVertices(renderer2d->GetDrawVertexCount());
		renderer2d->ResetDrawCallCount();
		renderer2d->ResetDrawVertexCount();
        
        #ifdef CC_USE_METAL
        afterRender(renderer2d, commandList);
        #endif

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
	spriteSize = 4000;
#endif

	CreateRenderer(spriteSize);

	manager2d = ::Effekseer::Manager::Create(8000);

	// set camera and projection matrix for 2d
	// If you special camera or 3d, please set yourself with setCameraMatrix and setProjectionMatrix
	renderer2d->SetProjectionMatrix(::Effekseer::Matrix44().OrthographicRH(visibleSize.width, visibleSize.height, 1.0f, 400.0f));

	renderer2d->SetCameraMatrix(
		::Effekseer::Matrix44().LookAtRH(::Effekseer::Vector3D(visibleSize.width / 2.0f, visibleSize.height / 2.0f, 200.0f),
										 ::Effekseer::Vector3D(visibleSize.width / 2.0f, visibleSize.height / 2.0f, -200.0f),
										 ::Effekseer::Vector3D(0.0f, 1.0f, 0.0f)));

	distortingCallback = CreateDistortingCallback(renderer2d, commandList_);

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
	if (distortingCallback != nullptr &&
        renderer2d->GetDistortingCallback() != distortingCallback)
	{
		delete distortingCallback;
		distortingCallback = nullptr;
	}

	onDestructor();

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

    ES_SAFE_RELEASE(memoryPool_);
    ES_SAFE_RELEASE(commandList_);
	ES_SAFE_RELEASE(internalManager_);
}

void EffectManager::setIsDistortionEnabled(bool value)
{
    isDistortionEnabled = value;
    if (isDistortionEnabled)
    {
        renderer2d->SetDistortingCallback(distortingCallback);
    }
    else
    {
        renderer2d->SetDistortingCallback(nullptr);
    }
}

void EffectManager::begin(cocos2d::Renderer* renderer, float globalZOrder)
{
	if (isDistortionEnabled)
	{
		isDistorted = false;
	}
	else
	{
		isDistorted = true;
        ResetBackground(renderer2d);
	}

    newFrame();

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

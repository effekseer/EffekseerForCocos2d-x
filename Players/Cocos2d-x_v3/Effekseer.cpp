
#include "Effekseer.h"

namespace efk
{
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

		// コピー先のテクスチャを準備
		void PrepareTexture(uint32_t width, uint32_t height, GLint internalFormat);

		virtual void OnDistorting();
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

	void DistortingCallbackGL::OnDistorting()
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

#ifndef _WIN32
		GLint backupFramebuffer;
		glGetIntegerv(GL_FRAMEBUFFER_BINDING, &backupFramebuffer);

		GLint rbtype;
		glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
			GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE, &rbtype);

		if (rbtype == GL_RENDERBUFFER) {
			GLint renderbuffer;
			glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
				GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME, &renderbuffer);

			glBindFramebuffer(GL_FRAMEBUFFER, framebufferForCopy);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, renderbuffer);
		}
		else if (rbtype == GL_TEXTURE_2D) {
			GLint renderTexture;
			glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
				GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME, &renderTexture);

			glBindFramebuffer(GL_FRAMEBUFFER, framebufferForCopy);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderTexture, 0);
		}
#endif

		glBindTexture(GL_TEXTURE_2D, backGroundTexture);
		//glCopyTexSubImage2D( GL_TEXTURE_2D, 0, 0, 0, 0, 0, width, height );
		glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, viewport[0], viewport[1], width, height);
		glBindTexture(GL_TEXTURE_2D, 0);

#ifndef _WIN32
		glBindFramebuffer(GL_FRAMEBUFFER, backupFramebuffer);
#endif

		renderer->SetBackground(backGroundTexture);
	}
#pragma endregion

#pragma region EffekseerFileReader
	class EffekseerFileReader :
		public Effekseer::FileReader
	{
		std::vector<uint8_t>	data;
		int32_t					position;
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
			if (data.size() - position < size) readable = data.size() - position;

			memcpy(buffer, &(data[position]), readable);
			position += readable;
			return readable;
		}

		void Seek(int position)
		{
			this->position = position;
			if (this->position < 0) this->position = 0;
			if (this->position > data.size()) this->position = data.size();
		}

		int GetPosition()
		{
			return position;
		}

		size_t GetLength()
		{
			return data.size();
		}
	};
#pragma endregion

#pragma region EffekseerFile
	class EffekseerFile :
		public Effekseer::FileInterface
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

	Effekseer::FileWriter* EffekseerFile::OpenWrite(const EFK_CHAR* path)
	{
		return nullptr;
	}
#pragma endregion

#pragma region
	static std::map<GLuint, std::basic_string<EFK_CHAR>> g_glTex2FilePath;
	static std::map<std::basic_string<EFK_CHAR>, cocos2d::CCTexture2D*> g_filePath2CTex;

	class TextureLoader
		: public ::Effekseer::TextureLoader
	{
	private:
		::Effekseer::FileInterface* m_fileInterface;
		::Effekseer::DefaultFileInterface m_defaultFileInterface;

	public:
		TextureLoader(::Effekseer::FileInterface* fileInterface = NULL);
		virtual ~TextureLoader();

	public:
		void* Load(const EFK_CHAR* path, ::Effekseer::TextureType textureType) override;

		void Unload(void* data);
	};

	TextureLoader::TextureLoader(::Effekseer::FileInterface* fileInterface)
		: m_fileInterface(fileInterface)
	{
		if (m_fileInterface == NULL)
		{
			m_fileInterface = &m_defaultFileInterface;
		}
	}

	TextureLoader::~TextureLoader()
	{

	}

	void* TextureLoader::Load(const EFK_CHAR* path, ::Effekseer::TextureType textureType)
	{
		auto key = std::basic_string<EFK_CHAR>(path);
		if (g_filePath2CTex.find(key) != g_filePath2CTex.end())
		{
			auto texture = g_filePath2CTex[key];
			texture->retain();
			return (void*)texture->getName();
		}

		std::unique_ptr<Effekseer::FileReader>
			reader(m_fileInterface->OpenRead(path));

		if (reader.get() != NULL)
		{
			size_t size_texture = reader->GetLength();
			char* data_texture = new char[size_texture];
			reader->Read(data_texture, size_texture);

			cocos2d::CCImage* image = new cocos2d::CCImage();
			cocos2d::CCTexture2D* texture = new cocos2d::CCTexture2D();
			if (image != nullptr &&
				texture != nullptr &&
				image->initWithImageData((const uint8_t*)data_texture, size_texture))
			{
				if (texture->initWithImage(image))
				{
					texture->generateMipmap();
				}
				else
				{
					CC_SAFE_DELETE(texture);
					CC_SAFE_DELETE(image);
				}
			}
			CC_SAFE_DELETE(image);

			delete[] data_texture;

			g_filePath2CTex[key] = texture;
			g_glTex2FilePath[texture->getName()] = key;

			return (void*)texture->getName();
		}
		return NULL;
	}

	void TextureLoader::Unload(void* data)
	{
		if (data != NULL)
		{
			GLuint gltex = EffekseerRenderer::TexturePointerToTexture <GLuint>(data);
			auto path = g_glTex2FilePath[gltex];
			auto tex = g_filePath2CTex[path];

			if (tex->getReferenceCount() == 1)
			{
				g_glTex2FilePath.erase(gltex);
				g_filePath2CTex.erase(path);
			}
			tex->release();
		}
	}

#pragma endregion

#pragma region EffekseerSetting
	class EffekseerSetting;

	static EffekseerSetting* g_effekseerSetting = nullptr;

	class EffekseerSetting
		: public ::Effekseer::Setting
	{
	protected:
		Effekseer::FileInterface*	effectFile = nullptr;

		EffekseerSetting()
		{
			effectFile = new efk::EffekseerFile();
			SetEffectLoader(Effekseer::Effect::CreateEffectLoader(effectFile));
			SetTextureLoader(new TextureLoader(effectFile));
			SetModelLoader(new ::EffekseerRendererGL::ModelLoader(effectFile));
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
#pragma endregion

#pragma region Effect
	Effect* Effect::create(const std::string& filename)
	{
		EFK_CHAR path_[300];
		::Effekseer::ConvertUtf8ToUtf16((int16_t*)path_, 300, (const int8_t*)filename.c_str());

		auto effect = Effekseer::Effect::Create(EffekseerSetting::create(), path_);

		if (effect != nullptr)
		{
			auto e = new Effect();
			e->effect = effect;
			return e;
		}

		// TODO
		// キャッシュ
		// autoreleaseをどうするか

		return nullptr;
	}

	Effect::Effect()
	{

	}

	Effect::~Effect()
	{
		ES_SAFE_RELEASE(effect);
	}

#pragma endregion

#pragma region Effect
	EffectEmitter* EffectEmitter::create(EffectManager* manager)
	{
		return new EffectEmitter(manager);
	}

	EffectEmitter::EffectEmitter(EffectManager* manager)
	{
		this->manager = manager;
		
		if (manager != nullptr)
		{
			manager->retain();
		}
	}

	EffectEmitter::~EffectEmitter()
	{
		if (effect != nullptr)
		{
			effect->release();
		}

		if (manager != nullptr)
		{
			manager->release();
		}
	}

	Effect* EffectEmitter::getEffect()
	{
		return effect;
	}

	void EffectEmitter::setEffect(Effect* effect)
	{
		if (this->effect != nullptr) effect->release();

		this->effect = effect;

		if (this->effect != nullptr) effect->retain();
	}

	void EffectEmitter::play()
	{
		if (effect == nullptr) return;
		if (manager == nullptr) return;

		auto pos = this->getPosition();
		auto rot = this->getRotation();
		auto scale = this->getScale();

		handle = manager->play(effect, pos.x, pos.y, 0);
		manager->setRotation(handle, 0, 0, rot);
		manager->setScale(handle, scale, scale, scale);
	}

	bool EffectEmitter::getPlayOnEnter()
	{
		return playOnEnter;
	}

	void EffectEmitter::setPlayOnEnter(bool value)
	{
		playOnEnter = value;
	}

	bool EffectEmitter::getIsLooping()
	{
		return isLooping;
	}

	void EffectEmitter::setIsLooping(bool value)
	{
		isLooping = value;
	}

	bool EffectEmitter::getRemoveOnStop()
	{
		return removeOnStop;
	}

	void EffectEmitter::setRemoveOnStop(bool value)
	{
		removeOnStop = value;
	}

	float EffectEmitter::getAngleY()
	{
		return angleY;
	}

	void EffectEmitter::setAngleY(float value)
	{
		angleY = value;
	}

	bool EffectEmitter::isPlaying()
	{
		return manager->getInternalManager()->Exists(handle);
	}

	void EffectEmitter::stop()
	{
		manager->getInternalManager()->StopEffect(handle);
	}

	void EffectEmitter::stopRoot()
	{
		manager->getInternalManager()->StopRoot(handle);
	}

	void EffectEmitter::onEnter()
	{
		cocos2d::Node::onEnter();

		if (playOnEnter)
		{
			play();
		}

		scheduleUpdate();
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
			else if(removeOnStop)
			{
				this->removeFromParent();
			}
		}

		auto pos = this->getPosition();
		auto rot = this->getRotation();
		auto scale = this->getScale();

		manager->setPotation(handle, pos.x, pos.y, 0);
		manager->setRotation(handle, 0, angleY, rot);
		manager->setScale(handle, scale, scale, scale);

		cocos2d::Node::update(delta);
	}

	void EffectEmitter::draw(cocos2d::Renderer *renderer, const cocos2d::Mat4& parentTransform, uint32_t parentFlags)
	{
		renderCommand.init(_globalZOrder);
		renderCommand.func = [this]() -> void
		{
			manager->getInternalRenderer()->SetRestorationOfStatesFlag(true);
			manager->getInternalRenderer()->BeginRendering();
			manager->getInternalManager()->DrawHandle(handle);
			manager->getInternalRenderer()->EndRendering();

			// Reset Parameters
			cocos2d::GL::useProgram(0);
		};

		renderer->addCommand(&renderCommand);

		cocos2d::Node::draw(renderer, parentTransform, parentFlags);
	}

#pragma endregion

#pragma region Effect
	::Effekseer::Handle EffectManager::play(Effect* effect, float x, float y, float z)
	{
		return manager2d->Play(effect->getInternalPtr(), x, y, z);
	}

	void EffectManager::setPotation(::Effekseer::Handle handle, float x, float y, float z)
	{
		manager2d->SetLocation(handle, x, y, z);
	}

	void EffectManager::setRotation(::Effekseer::Handle handle, float x, float y, float z)
	{
		manager2d->SetRotation(handle, x, y, z);
	}

	void EffectManager::setScale(::Effekseer::Handle handle, float x, float y, float z)
	{
		manager2d->SetScale(handle, x, y, z);
	}

	bool EffectManager::Initialize(cocos2d::Size visibleSize)
	{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		renderer2d = ::EffekseerRendererGL::Renderer::Create(2000, EffekseerRendererGL::OpenGLDeviceType::OpenGLES2);
#else
		renderer2d = ::EffekseerRendererGL::Renderer::Create(2000, EffekseerRendererGL::OpenGLDeviceType::OpenGL2);
#endif
		
		manager2d = ::Effekseer::Manager::Create(2000);

		renderer2d->SetProjectionMatrix(
			::Effekseer::Matrix44().OrthographicRH(visibleSize.width, visibleSize.height, 1.0f, 400.0f));

		// カメラ行列を設定
		renderer2d->SetCameraMatrix(
			::Effekseer::Matrix44().LookAtRH(
				::Effekseer::Vector3D(visibleSize.width / 2.0f, visibleSize.height / 2.0f, 200.0f),
				::Effekseer::Vector3D(visibleSize.width / 2.0f, visibleSize.height / 2.0f, -200.0f),
				::Effekseer::Vector3D(0.0f, 1.0f, 0.0f)));

		renderer2d->SetDistortingCallback(new DistortingCallbackGL(renderer2d));
		manager2d->SetSpriteRenderer(renderer2d->CreateSpriteRenderer());
		manager2d->SetRibbonRenderer(renderer2d->CreateRibbonRenderer());
		manager2d->SetRingRenderer(renderer2d->CreateRingRenderer());
		manager2d->SetModelRenderer(renderer2d->CreateModelRenderer());
		manager2d->SetTrackRenderer(renderer2d->CreateTrackRenderer());

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

	EffectManager::EffectManager()
	{

	}

	EffectManager::~EffectManager()
	{
		if (manager2d != nullptr)
		{
			manager2d->Destroy();
			manager2d = nullptr;
		}
	
		if (renderer2d != nullptr)
		{
			renderer2d->Destory();
			renderer2d = nullptr;
		}
	}

	void EffectManager::begin(cocos2d::Renderer *renderer, float globalZOrder)
	{
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

	void EffectManager::end(cocos2d::Renderer *renderer, float globalZOrder)
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


	void EffectManager::update()
	{
		manager2d->Update();
	}

#pragma endregion
}
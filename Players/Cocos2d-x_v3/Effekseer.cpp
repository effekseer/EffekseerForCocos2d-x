
#include "Effekseer.h"

namespace efk
{
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
	bool EffectManager::Initialize(cocos2d::Size visibleSize)
	{
		renderer2d = ::EffekseerRendererGL::Renderer::Create(2000);
		manager2d = ::Effekseer::Manager::Create(2000);

		renderer2d->SetProjectionMatrix(
			::Effekseer::Matrix44().OrthographicRH(visibleSize.width, visibleSize.height, 1.0f, 400.0f));

		// カメラ行列を設定
		renderer2d->SetCameraMatrix(
			::Effekseer::Matrix44().LookAtRH(
				::Effekseer::Vector3D(visibleSize.width / 2.0f, visibleSize.height / 2.0f, 200.0f),
				::Effekseer::Vector3D(visibleSize.width / 2.0f, visibleSize.height / 2.0f, -200.0f),
				::Effekseer::Vector3D(0.0f, 1.0f, 0.0f)));

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

	void EffectManager::Begin(cocos2d::Renderer *renderer, float globalZOrder)
	{
		beginCommand.init(globalZOrder);
		beginCommand.func = [this]() -> void
		{
			renderer2d->SetRestorationOfStatesFlag(true);
			renderer2d->BeginRendering();
			manager2d->Draw();

		};
		
		renderer->addCommand(&beginCommand);
	}

	void EffectManager::End(cocos2d::Renderer *renderer, float globalZOrder)
	{
		endCommand.init(globalZOrder);
		endCommand.func = [this]() -> void
		{
			// glのフラグか何かでおかしくなっている
			renderer2d->ResetRenderState();
			renderer2d->EndRendering();
		};

		renderer->addCommand(&endCommand);
	}


	void EffectManager::Update()
	{
		manager2d->Update();
	}


	::Effekseer::Handle EffectManager::Play(Effect* effect, float x, float y, float z)
	{
		return manager2d->Play(effect->GetInternalPtr(), x, y, z);
	}

	void EffectManager::SetRotation(::Effekseer::Handle handle, float x, float y, float z)
	{
		manager2d->SetRotation(handle, x, y, z);
	}

	void EffectManager::SetScale(::Effekseer::Handle handle, float x, float y, float z)
	{
		manager2d->SetScale(handle, x, y, z);
	}

#pragma endregion
}
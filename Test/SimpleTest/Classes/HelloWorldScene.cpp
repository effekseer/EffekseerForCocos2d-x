#include "HelloWorldScene.h"
#include "Effekseer/Effekseer.h"

std::map<GLuint, std::basic_string<EFK_CHAR>> glTex2FilePath;
std::map<std::basic_string<EFK_CHAR>, cocos2d::CCTexture2D*> filePath2CTex;

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
	if (filePath2CTex.find(key) != filePath2CTex.end())
	{
		auto texture = filePath2CTex[key];
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

		filePath2CTex[key] = texture;
		glTex2FilePath[texture->getName()] = key;
	
		return (void*)texture->getName();
	}
	return NULL;
}

void TextureLoader::Unload(void* data)
{
	if (data != NULL)
	{
		GLuint gltex = EffekseerRenderer::TexturePointerToTexture <GLuint>(data);
		auto path = glTex2FilePath[gltex];
		auto tex = filePath2CTex[path];
		
		if (tex->getReferenceCount() == 1)
		{
			glTex2FilePath.erase(gltex);
			filePath2CTex.erase(path);
		}
		tex->release();
	}
}

static ::Effekseer::Manager*				g_manager2d = NULL;
static ::EffekseerRendererGL::Renderer*		g_renderer2d = NULL;
static cocos2d::CustomCommand				g_customCommand;
Effekseer::FileInterface*					g_effectFile = nullptr;
USING_NS_CC;

Scene* HelloWorld::createScene()
{
    auto scene = Scene::create();
    auto layer = HelloWorld::create();
    scene->addChild(layer);
    return scene;
}

bool HelloWorld::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto sprite = Sprite::create("HelloWorld.png");
    this->addChild(sprite, 0);
    
	// for update
	this->scheduleUpdate();

	// Init Effekseer
	g_renderer2d = ::EffekseerRendererGL::Renderer::Create(2000);
	g_manager2d = ::Effekseer::Manager::Create(2000);

	// 投影行列を設定を設定する。
	g_renderer2d->SetProjectionMatrix(
		::Effekseer::Matrix44().OrthographicRH(visibleSize.width, visibleSize.height, 1.0f, 400.0f));

	// カメラ行列を設定
	g_renderer2d->SetCameraMatrix(
		::Effekseer::Matrix44().LookAtRH(
			::Effekseer::Vector3D(visibleSize.width/ 2.0f, visibleSize.height / 2.0f, 200.0f),
			::Effekseer::Vector3D(visibleSize.width / 2.0f, visibleSize.height / 2.0f, -200.0f),
			::Effekseer::Vector3D(0.0f, 1.0f, 0.0f)));

	g_effectFile = new Effekseer::EffekseerFile();
	g_manager2d->SetEffectLoader(Effekseer::Effect::CreateEffectLoader(g_effectFile));

	g_manager2d->SetSpriteRenderer(g_renderer2d->CreateSpriteRenderer());
	g_manager2d->SetRibbonRenderer(g_renderer2d->CreateRibbonRenderer());
	g_manager2d->SetRingRenderer(g_renderer2d->CreateRingRenderer());
	g_manager2d->SetModelRenderer(g_renderer2d->CreateModelRenderer());
	g_manager2d->SetTrackRenderer(g_renderer2d->CreateTrackRenderer());

	g_manager2d->SetTextureLoader(new TextureLoader(g_effectFile));
	g_manager2d->SetModelLoader(g_renderer2d->CreateModelLoader(g_effectFile));
    return true;
}

static int count = 0;

void HelloWorld::update(float delta)
{
	if (count % 60 == 0)
	{
		auto effect = Effekseer::Effect::Create(g_manager2d, (const EFK_CHAR*)u"laser.efk");
		if (effect != nullptr)
		{
			auto handle = g_manager2d->Play(effect, 300,300 ,0);
			g_manager2d->SetRotation(handle, 0, 90 / 180.0 * 3.1415, 0);
			g_manager2d->SetScale(handle, 20, 20, 20);
			effect->Release();
		}
	}

	g_manager2d->Update();
	count++;
}

HelloWorld::~HelloWorld()
{
	g_manager2d->Destroy();
	g_renderer2d->Destory();
	ES_SAFE_DELETE(g_effectFile);
}

void HelloWorld::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags)
{
	cocos2d::Layer::draw(renderer, transform, flags);

	g_customCommand.init(_globalZOrder);
	g_customCommand.func = CC_CALLBACK_0(HelloWorld::onDraw, this, renderer, transform, flags);
	renderer->addCommand(&g_customCommand);
}

void HelloWorld::onDraw(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, uint32_t flags)
{
	g_renderer2d->BeginRendering();
	g_manager2d->Draw();
	g_renderer2d->EndRendering();
}


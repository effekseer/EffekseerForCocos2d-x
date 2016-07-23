#include "HelloWorldScene.h"
#include "Effekseer/Effekseer.h"

static ::Effekseer::Manager*				g_manager2d = NULL;
static ::EffekseerRendererGL::Renderer*		g_renderer2d = NULL;
static cocos2d::CustomCommand				g_customCommand;
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

	g_manager2d->SetSpriteRenderer(g_renderer2d->CreateSpriteRenderer());
	g_manager2d->SetRibbonRenderer(g_renderer2d->CreateRibbonRenderer());
	g_manager2d->SetRingRenderer(g_renderer2d->CreateRingRenderer());
	g_manager2d->SetModelRenderer(g_renderer2d->CreateModelRenderer());
	g_manager2d->SetTrackRenderer(g_renderer2d->CreateTrackRenderer());
    return true;
}

static int count = 0;

void HelloWorld::update(float delta)
{
	if (count % 60 == 0)
	{
		auto effect = efk::Effect::create("laser.efk");
		if (effect != nullptr)
		{
			auto handle = g_manager2d->Play(effect->GetInternalPtr(), 300, 300, 0);
			g_manager2d->SetRotation(handle, 0, 90 / 180.0 * 3.1415, 0);
			g_manager2d->SetScale(handle, 20, 20, 20);
			effect->release();
		}
	}

	g_manager2d->Update();
	count++;
}

HelloWorld::~HelloWorld()
{
	g_manager2d->Destroy();
	g_renderer2d->Destory();
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


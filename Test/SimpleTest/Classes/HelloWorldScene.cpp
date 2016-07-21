// based on https://github.com/effekseer/EffekseerForDXLib/blob/master/Dev/EffekseerForDXLib/EffekseerForDXLib.cpp

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
    sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
    this->addChild(sprite, 0);
    
	// for update
	this->scheduleUpdate();

	// Init Effekseer
	g_renderer2d = ::EffekseerRendererGL::Renderer::Create(2000);
	g_manager2d = ::Effekseer::Manager::Create(2000);

	g_manager2d->SetSpriteRenderer(g_renderer2d->CreateSpriteRenderer());
	g_manager2d->SetRibbonRenderer(g_renderer2d->CreateRibbonRenderer());
	g_manager2d->SetRingRenderer(g_renderer2d->CreateRingRenderer());
	g_manager2d->SetModelRenderer(g_renderer2d->CreateModelRenderer());
	g_manager2d->SetTrackRenderer(g_renderer2d->CreateTrackRenderer());

	g_manager2d->SetTextureLoader(g_renderer2d->CreateTextureLoader());
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
			auto handle = g_manager2d->Play(effect, 0, 0, 0);
			g_manager2d->SetRotation(handle, 0, 90, 0);
			g_manager2d->SetScale(handle, 0.5, 0.5, 0.5);
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


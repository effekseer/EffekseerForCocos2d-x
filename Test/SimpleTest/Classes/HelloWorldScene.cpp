#include "HelloWorldScene.h"
#include "Effekseer/Effekseer.h"

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
   // this->addChild(sprite, 0);
    
	// for update
	this->scheduleUpdate();

	manager = efk::EffectManager::create(visibleSize);

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
			auto handle = manager->Play(effect, 300, 300, 0);
			manager->SetRotation(handle, 0, 90 / 180.0 * 3.1415, 0);
			manager->SetScale(handle, 20, 20, 20);
			effect->release();
		}
	}

	manager->Update();
	count++;
}

HelloWorld::~HelloWorld()
{
	manager->release();
}

void HelloWorld::visit(cocos2d::Renderer *renderer, const cocos2d::Mat4& parentTransform, uint32_t parentFlags)
{
	manager->Begin(renderer, _globalZOrder);
	cocos2d::Layer::visit(renderer, parentTransform, parentFlags);
	manager->End(renderer, _globalZOrder);
}


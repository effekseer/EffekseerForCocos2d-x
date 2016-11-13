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
	this->addChild(sprite, 0);
    
	// for update
	this->scheduleUpdate();

	/**
		efk::EffectManagerのインスタンスを生成します。

		You create an instance of efk::EffectManager.

		您創建一個efk::EffectManager的實例。

		您创建一个efk::EffectManager的实例。
	*/
	manager = efk::EffectManager::create(visibleSize);

    return true;
}

void HelloWorld::update(float delta)
{
	if (count % 60 == 0)
	{
		/**
			エフェクトファイルを読み込みます。

			You read an effect file.

			您讀取效果文件。

			您读取效果文件。
		*/
		auto effect = efk::Effect::create("Laser01.efk");
		if (effect != nullptr)
		{
			// create a particle emitter.
			auto emitter = efk::EffectEmitter::create(manager);
			emitter->setEffect(effect);
			emitter->setPlayOnEnter(true);

			emitter->setPosition(Vec2(300, 300));
			emitter->setScale(20);
			this->addChild(emitter, 0);

			effect->release();
		}
	}

	manager->update();
	count++;
}

HelloWorld::~HelloWorld()
{
	manager->release();
}

void HelloWorld::visit(cocos2d::Renderer *renderer, const cocos2d::Mat4& parentTransform, uint32_t parentFlags)
{
	manager->begin(renderer, _globalZOrder);
	cocos2d::Layer::visit(renderer, parentTransform, parentFlags);
	manager->end(renderer, _globalZOrder);
}


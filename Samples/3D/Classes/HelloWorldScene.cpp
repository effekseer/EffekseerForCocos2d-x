#include "HelloWorldScene.h"
#include "Effekseer/Effekseer.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
	return HelloWorld::create();
}

bool HelloWorld::init()
{
	if ( !Scene::init() )
	{
		return false;
	}
	
	auto rsize = Director::getInstance()->getOpenGLView()->getDesignResolutionSize();

	auto sprite = BillBoard::create("HelloWorld.png");
	sprite->setPosition3D(Vec3(0, 0, 0));
	sprite->setScale(0.1);
	sprite->setCameraMask(2);
	this->addChild(sprite);
	
	// 3D
	auto s = Director::getInstance()->getWinSize();
	camera = Camera::createPerspective(60, (GLfloat)s.width / s.height, 1, 1000);
	camera->setPosition3D(Vec3(100, 100, 100));
	camera->lookAt(Vec3(0, 0, 0), Vec3(0, 1, 0));
	camera->setCameraFlag(cocos2d::CameraFlag::USER1);

	this->addChild(camera);

	// for update
	this->scheduleUpdate();

	/**
		efk::EffectManagerのインスタンスを生成します。

		You create an instance of efk::EffectManager.

		您創建一個efk::EffectManager的實例。

		您创建一个efk::EffectManager的实例。
	*/
	manager = efk::EffectManager::create(rsize);

	return true;
}

void HelloWorld::update(float delta)
{
	if (count % 240 == 0)
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
			/**
				エミッターを生成し、パラメーターを設定してレイヤーに追加します。

				You generate an emitter, set parameters and add it to the layer.

				您會生成一個發射極，並通過將參數添加到該層。

				您会生成一个发射极，并通过将参数添加到该层。
			*/
			auto emitter = efk::EffectEmitter::create(manager);
			emitter->setEffect(effect);
			emitter->setPlayOnEnter(true);

			emitter->setPosition3D(Vec3(0, 0, 0));
			emitter->setRotation3D(Vec3(0, 90, 0));
			emitter->setScale(3);
			emitter->setCameraMask(2);
			this->addChild(emitter);

			// No need (because it uses autorelease after 1.41)
			//effect->release();
		}
	}

	/**
		毎フレーム、マネージャーを更新します。

		You update the manager every frame.

		您將更新每一幀，經理。

		您将更新每一帧，经理。
	*/
	manager->update();

	manager->setCameraMatrix(camera->getViewMatrix());
	manager->setProjectionMatrix(camera->getProjectionMatrix());

	count++;
}

HelloWorld::~HelloWorld()
{
	camera->release();

	/**
		終了時にマネージャーを破棄します。

		You will destroy the manager on exit.

		你會在最後摧毀經理。

		你会在最后摧毁经理。
	*/
	manager->release();
}

void HelloWorld::visit(cocos2d::Renderer *renderer, const cocos2d::Mat4& parentTransform, uint32_t parentFlags)
{
	/**
		visitを継承して、エフェクトを実際に描画する処理を追加します。

		You inherit visit and add a process to actually draw the effect.

		你繼承的visit，然後添加的實際繪製效果的過程。

		你继承的visit，然后添加的实际绘制效果的过程。
	*/
	manager->begin(renderer, _globalZOrder);
	cocos2d::Scene::visit(renderer, parentTransform, parentFlags);
	manager->end(renderer, _globalZOrder);
}


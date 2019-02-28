#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

/**
	必ずEffekseer.hをインクルードする必要があります

	You must include Effekseer.h

	你必須包括Effekseer.h

	你必须包括Effekseer.h
*/
#include "Effekseer/Effekseer.h"

class HelloWorld : public cocos2d::Scene
{
private:
	/**
		efk::EffectManager*をレイヤーのメンバ変数に追加します。このクラスはエフェクトを管理します。

		You add efk :: EffectManager * to the layer member variable. This class manages effects.

		您將efk :: EffectManager *添加到圖層成員變量。 這個類管理效果。

		您将efk :: EffectManager *添加到图层成员变量。 这个类管理效果。
	*/
	efk::EffectManager*		manager = nullptr;
	efk::Effect*			effect_retain = nullptr;
	efk::NetworkServer*		networkSever = nullptr;

	int count = 0;
public:
    static cocos2d::Scene* createScene();

    virtual bool init() override;
	virtual void update(float delta) override;

	virtual ~HelloWorld();

	virtual void visit(cocos2d::Renderer *renderer, const cocos2d::Mat4& parentTransform, uint32_t parentFlags) override;
    CREATE_FUNC(HelloWorld);
};

#endif // __HELLOWORLD_SCENE_H__


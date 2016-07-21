#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

class HelloWorld : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init() override;
	virtual void update(float delta) override;

	virtual ~HelloWorld();

	virtual void draw(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, uint32_t flags) override;
	void onDraw(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, uint32_t flags);

    CREATE_FUNC(HelloWorld);
};

#endif // __HELLOWORLD_SCENE_H__

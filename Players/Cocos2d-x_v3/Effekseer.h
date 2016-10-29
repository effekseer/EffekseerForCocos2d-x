
#pragma once


#include "cocos2d.h"
#include "EffekseerNative.h"
#include "EffekseerRendererNative.h"

namespace efk
{
	class EffectManager;

	class EffekseerFile :
		public Effekseer::FileInterface
	{
	public:
		EffekseerFile();
		virtual ~EffekseerFile();
		Effekseer::FileReader* OpenRead(const EFK_CHAR* path);
		Effekseer::FileWriter* OpenWrite(const EFK_CHAR* path);
	};

	class Effect
		: public cocos2d::Ref
	{
	private:
		::Effekseer::Effect*	effect = nullptr;

	public:
		static Effect* create(const std::string& filename);

		Effect();

		virtual ~Effect();

		::Effekseer::Effect* getInternalPtr()
		{
			return effect;
		}
	};

	class EffectEmitter
		: public cocos2d::Node
	{
	private:
		bool playOnEnter = false;
		bool isLooping = false;

		EffectManager* manager = nullptr;
		Effect* effect = nullptr;
		::Effekseer::Handle handle = 0;

		cocos2d::CustomCommand	renderCommand;

	public:
		static EffectEmitter* create(EffectManager* manager);

		EffectEmitter(EffectManager* manager);
		virtual ~EffectEmitter();

		Effect* getEffect();
		void setEffect(Effect* effect);

		/**
			@brief	エフェクトを再生する。
		*/
		void play();

		/**
			@brief	追加時に再生を開始するかどうか、取得する。
			@return	フラグ
		*/
		bool getPlayOnEnter();

		/**
			@brief	追加時に再生を開始するかどうか、設定する。
			@return	value	フラグ
		*/
		void setPlayOnEnter(bool value);

		/**
		@brief	ループ再生するかどうか、取得する。
		@return	フラグ
		*/
		bool getIsLooping();

		/**
		@brief	ループ再生するかどうか、設定する。
		@return	value	フラグ
		*/
		void setIsLooping(bool value);

		void onEnter() override;

		void update(float delta) override;

		void draw(cocos2d::Renderer *renderer, const cocos2d::Mat4& parentTransform, uint32_t parentFlags) override;
	};

	class EffectManager
		: public cocos2d::Ref
	{
	private:
		::Effekseer::Manager*					manager2d = NULL;
		::EffekseerRendererGL::Renderer*		renderer2d = NULL;
		cocos2d::CustomCommand					beginCommand;
		cocos2d::CustomCommand					endCommand;

		bool Initialize(cocos2d::Size visibleSize);
	public:
		static EffectManager* create(cocos2d::Size visibleSize);

		EffectManager();
		virtual ~EffectManager();

		void begin(cocos2d::Renderer *renderer, float globalZOrder);

		void end(cocos2d::Renderer *renderer, float globalZOrder);

		void update();

		::Effekseer::Handle play(Effect* effect, float x, float y, float z);

		void setPotation(::Effekseer::Handle handle, float x, float y, float z);

		void setRotation(::Effekseer::Handle handle, float x, float y, float z);

		void setScale(::Effekseer::Handle handle, float x, float y, float z);

		::Effekseer::Manager* getInternalManager() { return manager2d; }

		::EffekseerRendererGL::Renderer* getInternalRenderer() { return renderer2d; }

	};
}
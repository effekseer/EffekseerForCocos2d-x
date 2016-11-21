
#pragma once


#include "cocos2d.h"
#include "EffekseerNative.h"
#include "EffekseerRendererNative.h"

namespace efk
{
	class EffectManager;

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
		friend class EffectManager;

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

		/**
			@brief	再生終了時にノードを破棄するかどうか、取得する。
		*/

		/**
			@brief	再生終了時にノードを破棄するかどうか、設定する。
		*/

		/**
			@brief	Y軸方向の回転角度を取得する。
		*/

		/**
			@brief	Y軸方向の回転角度を設定する。
		*/

		/**
			@brief	再生中かどうか、取得する。
		*/


		/**
			@brief	エフェクトを停止する。
		*/

		/**
			@brief	エフェクトのルートのみを停止する。
		*/

		void onEnter() override;

		void update(float delta) override;

		void draw(cocos2d::Renderer *renderer, const cocos2d::Mat4& parentTransform, uint32_t parentFlags) override;
	};

	class EffectManager
		: public cocos2d::Ref
	{
		friend class EffectEmitter;

	private:
		::Effekseer::Manager*					manager2d = NULL;
		::EffekseerRendererGL::Renderer*		renderer2d = NULL;
		cocos2d::CustomCommand					beginCommand;
		cocos2d::CustomCommand					endCommand;

		::Effekseer::Handle play(Effect* effect, float x, float y, float z);

		void setPotation(::Effekseer::Handle handle, float x, float y, float z);

		void setRotation(::Effekseer::Handle handle, float x, float y, float z);

		void setScale(::Effekseer::Handle handle, float x, float y, float z);

		bool Initialize(cocos2d::Size visibleSize);
	public:

		/**
			@brief	マネージャークラスを生成する。
			@return	インスタンス
		*/
		static EffectManager* create(cocos2d::Size visibleSize);

		/**
			@brief	コンストラクタ
		*/
		EffectManager();

		/**
			@brief	デストラクタ
		*/
		virtual ~EffectManager();

		void begin(cocos2d::Renderer *renderer, float globalZOrder);

		void end(cocos2d::Renderer *renderer, float globalZOrder);

		void update();

		::Effekseer::Manager* getInternalManager() { return manager2d; }

		::EffekseerRendererGL::Renderer* getInternalRenderer() { return renderer2d; }

	};
}
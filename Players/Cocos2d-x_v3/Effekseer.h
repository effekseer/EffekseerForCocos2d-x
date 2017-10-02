
#pragma once


#include "cocos2d.h"
#include "EffekseerNative.h"
#include "EffekseerRendererNative.h"

namespace efk
{
	class EffectManager;

	/**
		@brief	エフェクトのリソースクラス
	*/
	class Effect
		: public cocos2d::Ref
	{
	private:
		::Effekseer::Effect*	effect = nullptr;

	public:
		static Effect* create(const std::string& filename);

		Effect();

		virtual ~Effect();

		/**
			@brief	EffekseerのEffectのポインタを取得する。
			@return	EffekseerのEffectのポインタ
		*/
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
		bool removeOnStop = true;
		bool isLooping = false;
		float angleY = 0;

		EffectManager* manager = nullptr;
		Effect* effect = nullptr;
		::Effekseer::Handle handle = 0;

		cocos2d::CustomCommand	renderCommand;

	public:
		static EffectEmitter* create(EffectManager* manager);

		EffectEmitter(EffectManager* manager);
		virtual ~EffectEmitter();

		/**
			@brief	エフェクトのリソースを取得する。
			@return	エフェクトのリソース
		*/
		Effect* getEffect();

		/**
		@brief	エフェクトのリソースを設定する。
		@param	effect	エフェクトのリソース
		*/
		void setEffect(Effect* effect);

		/**
			@brief
			\~English	Play the effect.
			\~Japanese	エフェクトを再生する。
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
		@brief
			\~English	Get whether to loop play the effect.
			\~Japanese	ループ再生するかどうか、取得する。
			@return	value
			\~English	Flag
			\~Japanese	フラグ
		*/
		bool getIsLooping();

		/**
			@brief	
			\~English	Set whether to loop play the effect.
			\~Japanese	ループ再生するかどうか、設定する。
			@return	value
			\~English	Flag
			\~Japanese	フラグ
		*/
		void setIsLooping(bool value);

		/**
			@brief
			\~English	Get whether to discard the node at the end of playback.
			\~Japanese	再生終了時にノードを破棄するかどうか、設定する。
		*/
		bool getRemoveOnStop();

		/**
			@brief	
			\~English	Set whether to discard the node at the end of playback.
			\~Japanese	再生終了時にノードを破棄するかどうか、設定する。
		*/
		void setRemoveOnStop(bool value);

		/**
			@brief
			\~English	Get the angle around Y axis.
			\~Japanese	Y軸方向の回転角度を取得する。
		*/
		float getAngleY();

		/**
			@brief	
			\~English	Set the angle around Y axis.
			\~Japanese	Y軸方向の回転角度を設定する。
		*/
		void setAngleY(float value);

		/**
			@brief	
			\~English	Get whether or not playing the effect.
			\~Japanese	再生中かどうか、取得する。
		*/
		bool isPlaying();

		/**
			@brief
			\~English	Stop the effect.
			\~Japanese	エフェクトを停止する。
		*/
		void stop();

		/**
			@brief
			\~English	Stop only the effect's root.
			\~Japanese	エフェクトのルートのみを停止する。	
		*/
		void stopRoot();

		void onEnter() override;

		void update(float delta) override;

		void draw(cocos2d::Renderer *renderer, const cocos2d::Mat4& parentTransform, uint32_t parentFlags) override;
	};

	class EffectManager
		: public cocos2d::Ref
	{
		friend class EffectEmitter;

	private:
		::Effekseer::Manager*						manager2d = NULL;
		::EffekseerRendererGL::Renderer*			renderer2d = NULL;
		::EffekseerRenderer::DistortingCallback*	distortingCallback = NULL;
		bool										isDistortionEnabled = false;

		cocos2d::CustomCommand					distortionCommand;
		cocos2d::CustomCommand					beginCommand;
		cocos2d::CustomCommand					endCommand;

		::Effekseer::Handle play(Effect* effect, float x, float y, float z);

		void setPotation(::Effekseer::Handle handle, float x, float y, float z);

		void setRotation(::Effekseer::Handle handle, float x, float y, float z);

		void setScale(::Effekseer::Handle handle, float x, float y, float z);

		bool Initialize(cocos2d::Size visibleSize);
	public:

		/**
			@brief
			\~English	Create the instance of manager class.
			\~Japanese	マネージャークラスのインスタンスを生成する。

			@return	
			\~English	Instance
			\~Japanese	インスタンス
		*/
		static EffectManager* create(cocos2d::Size visibleSize);

		/**
			@brief
			\~English	Constructor
			\~Japanese	コンストラクタ
		*/
		EffectManager();

		/**
			@brief
			\~English	Destructor
			\~Japanese	デストラクタ
		*/
		virtual ~EffectManager();

		/**
			@brief
			\~English	Set whether distortion is enabled.
			\~Japanese	歪みが有功かどうかを設定する。
		*/
		void setIsDistortionEnabled(bool value);

		/**
			@brief
			\~English	Inherit visit and add a process before drawing the layer.
			\~Japanese	visitを継承してレイヤーの描画を行う前に実行する。
		*/
		void begin(cocos2d::Renderer *renderer, float globalZOrder);

		/**
			@brief	
			\~English	Inherit visit and add a process after drawing the layer.
			\~Japanese	visitを継承してレイヤーの描画を行った後に実行する。
		*/
		void end(cocos2d::Renderer *renderer, float globalZOrder);

		/**
			@brief
			\~English	Update the manager every frame.
			\~Japanese	毎フレーム実行する。
		*/
		void update();

		/**
		@brief	EffekseerのManagerのポインタを取得する。
		@return	EffekseerのManagerのポインタ
		*/
		::Effekseer::Manager* getInternalManager() { return manager2d; }

		/**
			@brief	EffekseerのRendererのポインタを取得する。
			@return	EffekseerのRendererのポインタ
		*/
		::EffekseerRendererGL::Renderer* getInternalRenderer() { return renderer2d; }

	};
}
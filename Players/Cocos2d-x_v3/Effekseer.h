
#pragma once


#include "cocos2d.h"
#include "EffekseerNative.h"
#include "EffekseerRendererNative.h"

namespace efk
{
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

		::Effekseer::Effect* GetInternalPtr()
		{
			return effect;
		}
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

		void Begin(cocos2d::Renderer *renderer, float globalZOrder);

		void End(cocos2d::Renderer *renderer, float globalZOrder);

		void Update();

		::Effekseer::Handle Play(Effect* effect, float x, float y, float z);

		void SetRotation(::Effekseer::Handle handle, float x, float y, float z);

		void SetScale(::Effekseer::Handle handle, float x, float y, float z);

	};
}
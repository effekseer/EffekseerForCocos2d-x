
#pragma once

#include "cocos2d.h"
#include "EffekseerNative.h"
#include "EffekseerRendererNative.h"

namespace Effekseer
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

}
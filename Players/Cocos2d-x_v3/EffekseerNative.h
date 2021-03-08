﻿
#ifndef __EFFEKSEER_BASE_PRE_H__
#define __EFFEKSEER_BASE_PRE_H__

//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------
#include <array>
#include <atomic>
#include <cfloat>
#include <climits>
#include <memory>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <vector>

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#ifdef _WIN32
#define EFK_STDCALL __stdcall
#else
#define EFK_STDCALL
#endif

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------

#ifdef _WIN32
//#include <windows.h>
#elif defined(_PSVITA)
#elif defined(_PS4)
#elif defined(_SWITCH)
#elif defined(_XBOXONE)
#else
#include <pthread.h>
#include <sys/time.h>
#include <unistd.h>
#endif

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
typedef char16_t EFK_CHAR;

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace Effekseer
{
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
struct Vector2D;
struct Vector3D;
struct Matrix43;
struct Matrix44;
struct RectF;

class Manager;
class Effect;
class EffectNode;

class ParticleRenderer;
class SpriteRenderer;
class RibbonRenderer;
class RingRenderer;
class ModelRenderer;
class TrackRenderer;

class Setting;
class EffectLoader;
class TextureLoader;
class MaterialLoader;

class SoundPlayer;
class SoundLoader;

class ModelLoader;

class Model;

typedef int Handle;

/**
	@brief	Memory Allocation function
*/
typedef void*(EFK_STDCALL* MallocFunc)(unsigned int size);

/**
	@brief	Memory Free function
*/
typedef void(EFK_STDCALL* FreeFunc)(void* p, unsigned int size);

/**
	@brief	AlignedMemory Allocation function
*/
typedef void*(EFK_STDCALL* AlignedMallocFunc)(unsigned int size, unsigned int alignment);

/**
	@brief	AlignedMemory Free function
*/
typedef void(EFK_STDCALL* AlignedFreeFunc)(void* p, unsigned int size);

/**
	@brief	Random Function
*/
typedef int(EFK_STDCALL* RandFunc)(void);

/**
	@brief	エフェクトのインスタンス破棄時のコールバックイベント
	@param	manager	[in]	所属しているマネージャー
	@param	handle	[in]	エフェクトのインスタンスのハンドル
	@param	isRemovingManager	[in]	マネージャーを破棄したときにエフェクトのインスタンスを破棄しているか
*/
typedef void(EFK_STDCALL* EffectInstanceRemovingCallback)(Manager* manager, Handle handle, bool isRemovingManager);

#define ES_SAFE_ADDREF(val) \
	if ((val) != NULL)      \
	{                       \
		(val)->AddRef();    \
	}
#define ES_SAFE_RELEASE(val) \
	if ((val) != NULL)       \
	{                        \
		(val)->Release();    \
		(val) = NULL;        \
	}
#define ES_SAFE_DELETE(val) \
	if ((val) != NULL)      \
	{                       \
		delete (val);       \
		(val) = NULL;       \
	}
#define ES_SAFE_DELETE_ARRAY(val) \
	if ((val) != NULL)            \
	{                             \
		delete[](val);            \
		(val) = NULL;             \
	}

#define EFK_ASSERT(x) assert(x)

//! the maximum number of texture slot which can be specified by an user
const int32_t UserTextureSlotMax = 6;

//! the maximum number of texture slot including textures system specified
const int32_t TextureSlotMax = 8;

const int32_t LocalFieldSlotMax = 4;

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
/**
	@brief	アルファブレンド
*/
enum class AlphaBlendType : int32_t
{
	/// <summary>
	/// 不透明
	/// </summary>
	Opacity = 0,
	/// <summary>
	/// 透明
	/// </summary>
	Blend = 1,
	/// <summary>
	/// 加算
	/// </summary>
	Add = 2,
	/// <summary>
	/// 減算
	/// </summary>
	Sub = 3,
	/// <summary>
	/// 乗算
	/// </summary>
	Mul = 4,
};

enum class TextureFilterType : int32_t
{
	Nearest = 0,
	Linear = 1,
};

enum class TextureWrapType : int32_t
{
	Repeat = 0,
	Clamp = 1,
};

enum class CullingType : int32_t
{
	Front = 0,
	Back = 1,
	Double = 2,
};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
enum class BillboardType : int32_t
{
	Billboard = 0,
	YAxisFixed = 1,
	Fixed = 2,
	RotatedBillboard = 3,
};

enum class CoordinateSystem : int32_t
{
	LH,
	RH,
};

enum class CullingShape : int32_t
{
	NoneShape = 0,
	Sphere = 1,
};

enum class TextureType : int32_t
{
	Color,
	Normal,
	Distortion,
};

enum class MaterialFileType : int32_t
{
	Code,
	Compiled,
};

enum class TextureFormatType : int32_t
{
	ABGR8,
	BC1,
	BC2,
	BC3,
};

enum class ZSortType : int32_t
{
	None,
	NormalOrder,
	ReverseOrder,
};

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
enum class RenderMode : int32_t
{
	Normal,	   // 通常描画
	Wireframe, // ワイヤーフレーム描画
};

/**
	@brief
	\~English	A thread where reload function is called
	\~Japanese	リロードの関数が呼ばれるスレッド
*/
enum class ReloadingThreadType
{
	Main,
	Render,
};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
/**
	@brief	最大値取得
*/
template <typename T, typename U>
T Max(T t, U u)
{
	if (t > (T)u)
	{
		return t;
	}
	return u;
}

/**
	@brief	最小値取得
*/
template <typename T, typename U>
T Min(T t, U u)
{
	if (t < (T)u)
	{
		return t;
	}
	return u;
}

/**
	@brief	範囲内値取得
*/
template <typename T, typename U, typename V>
T Clamp(T t, U max_, V min_)
{
	if (t > (T)max_)
	{
		t = (T)max_;
	}

	if (t < (T)min_)
	{
		t = (T)min_;
	}

	return t;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
/**
	@brief	文字コードを変換する。(UTF16 -> UTF8)
	@param	dst	[out]	出力配列の先頭ポインタ
	@param	dst_size	[in]	出力配列の長さ
	@param	src			[in]	入力配列の先頭ポインタ
	@return	文字数
*/
inline int32_t ConvertUtf16ToUtf8(int8_t* dst, int32_t dst_size, const int16_t* src)
{
	int32_t cnt = 0;
	const int16_t* wp = src;
	int8_t* cp = dst;

	if (dst_size == 0)
		return 0;

	dst_size -= 3;

	for (cnt = 0; cnt < dst_size;)
	{
		int16_t wc = *wp++;
		if (wc == 0)
		{
			break;
		}
		if ((wc & ~0x7f) == 0)
		{
			*cp++ = wc & 0x7f;
			cnt += 1;
		}
		else if ((wc & ~0x7ff) == 0)
		{
			*cp++ = ((wc >> 6) & 0x1f) | 0xc0;
			*cp++ = ((wc)&0x3f) | 0x80;
			cnt += 2;
		}
		else
		{
			*cp++ = ((wc >> 12) & 0xf) | 0xe0;
			*cp++ = ((wc >> 6) & 0x3f) | 0x80;
			*cp++ = ((wc)&0x3f) | 0x80;
			cnt += 3;
		}
	}
	*cp = '\0';
	return cnt;
}

/**
	@brief    Convert UTF8 into UTF16
	@param    dst    a pointer to destination buffer
	@param    dst_size    a length of destination buffer
	@param    src            a source buffer
	@return    length except 0
*/
inline int32_t ConvertUtf8ToUtf16(char16_t* dst, int32_t dst_size, const char* src)
{
	int32_t i, code = 0;
	int8_t c0, c1, c2 = 0;
	int8_t* srci = reinterpret_cast<int8_t*>(const_cast<char*>(src));
	if (dst_size == 0)
		return 0;

	dst_size -= 1;

	for (i = 0; i < dst_size; i++)
	{
		uint16_t wc;

		c0 = *srci;
		srci++;
		if (c0 == '\0')
		{
			break;
		}
		// convert UTF8 to UTF16
		code = (uint8_t)c0 >> 4;
		if (code <= 7)
		{
			// 8bit character
			wc = c0;
		}
		else if (code >= 12 && code <= 13)
		{
			// 16bit  character
			c1 = *srci;
			srci++;
			wc = ((c0 & 0x1F) << 6) | (c1 & 0x3F);
		}
		else if (code == 14)
		{
			// 24bit character
			c1 = *srci;
			srci++;
			c2 = *srci;
			srci++;
			wc = ((c0 & 0x0F) << 12) | ((c1 & 0x3F) << 6) | (c2 & 0x3F);
		}
		else
		{
			continue;
		}
		dst[i] = wc;
	}
	dst[i] = 0;
	return i;
}

/**
	@brief	文字コードを変換する。(UTF8 -> UTF16)
	@param	dst	[out]	出力配列の先頭ポインタ
	@param	dst_size	[in]	出力配列の長さ
	@param	src			[in]	入力配列の先頭ポインタ
	@return	文字数
*/
inline int32_t ConvertUtf8ToUtf16(int16_t* dst, int32_t dst_size, const int8_t* src)
{
	int32_t i, code;
	int8_t c0, c1, c2;

	if (dst_size == 0)
		return 0;

	dst_size -= 1;

	for (i = 0; i < dst_size; i++)
	{
		int16_t wc;

		c0 = *src++;
		if (c0 == '\0')
		{
			break;
		}
		// UTF8からUTF16に変換
		code = (uint8_t)c0 >> 4;
		if (code <= 7)
		{
			// 8bit文字
			wc = c0;
		}
		else if (code >= 12 && code <= 13)
		{
			// 16bit文字
			c1 = *src++;
			wc = ((c0 & 0x1F) << 6) | (c1 & 0x3F);
		}
		else if (code == 14)
		{
			// 24bit文字
			c1 = *src++;
			c2 = *src++;
			wc = ((c0 & 0x0F) << 12) | ((c1 & 0x3F) << 6) | (c2 & 0x3F);
		}
		else
		{
			continue;
		}
		dst[i] = wc;
	}
	dst[i] = 0;
	return i;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
/**
@brief	参照カウンタのインターフェース
*/
class IReference
{
public:
	/**
	@brief	参照カウンタを加算する。
	@return	加算後の参照カウンタ
	*/
	virtual int AddRef() = 0;

	/**
	@brief	参照カウンタを取得する。
	@return	参照カウンタ
	*/
	virtual int GetRef() = 0;

	/**
	@brief	参照カウンタを減算する。0になった時、インスタンスを削除する。
	@return	減算後の参照カウンタ
	*/
	virtual int Release() = 0;
};

/**
	@brief	a deleter for IReference
*/
template <typename T>
struct ReferenceDeleter
{
	void operator()(T* ptr) const
	{
		if (ptr != nullptr)
		{
			ptr->Release();
		}
	}
};

template <typename T>
inline std::unique_ptr<T, ReferenceDeleter<T>> CreateUniqueReference(T* ptr, bool addRef = false)
{
	if (ptr == nullptr)
		return std::unique_ptr<T, ReferenceDeleter<T>>(nullptr);

	if (addRef)
	{
		ptr->AddRef();
	}

	return std::unique_ptr<T, ReferenceDeleter<T>>(ptr);
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
/**
@brief	参照カウンタオブジェクト
*/
class ReferenceObject
	: public IReference
{
private:
	mutable std::atomic<int32_t> m_reference;

public:
	ReferenceObject()
		: m_reference(1)
	{
	}

	virtual ~ReferenceObject()
	{
	}

	virtual int AddRef()
	{
		std::atomic_fetch_add_explicit(&m_reference, 1, std::memory_order_consume);

		return m_reference;
	}

	virtual int GetRef()
	{
		return m_reference;
	}

	virtual int Release()
	{
		bool destroy = std::atomic_fetch_sub_explicit(&m_reference, 1, std::memory_order_consume) == 1;
		if (destroy)
		{
			delete this;
			return 0;
		}

		return m_reference;
	}
};

/**
	@brief	This object generates random values.
*/
class IRandObject
{
public:
	IRandObject() = default;
	virtual ~IRandObject() = default;

	virtual float GetRand() = 0;

	virtual float GetRand(float min_, float max_) = 0;
};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------

enum class ColorSpaceType : int32_t
{
	Gamma,
	Linear,
};

/**
	@brief	\~english	Texture data
			\~japanese	テクスチャデータ
*/
struct TextureData
{
	int32_t Width;
	int32_t Height;
	TextureFormatType TextureFormat;
	void* UserPtr;
	int64_t UserID;

	//! for OpenGL, it is ignored in other apis
	bool HasMipmap = true;
};

enum class ShadingModelType : int32_t
{
	Lit,
	Unlit,
};

/**
	@brief	material type
*/
enum class RendererMaterialType : int32_t
{
	Default = 0,
	BackDistortion = 6,
	Lighting = 7,
	File = 128,
};

/**
	@brief	\~english	Material data
			\~japanese	マテリアルデータ
*/
class MaterialData
{
public:
	ShadingModelType ShadingModel = ShadingModelType::Lit;
	bool IsSimpleVertex = false;
	bool IsRefractionRequired = false;
	int32_t CustomData1 = 0;
	int32_t CustomData2 = 0;
	int32_t TextureCount = 0;
	int32_t UniformCount = 0;
	std::array<TextureWrapType, UserTextureSlotMax> TextureWrapTypes;
	void* UserPtr = nullptr;
	void* ModelUserPtr = nullptr;
	void* RefractionUserPtr = nullptr;
	void* RefractionModelUserPtr = nullptr;

	MaterialData() = default;
	virtual ~MaterialData() = default;
};

/**
	@brief	\~english	Textures used by material
			\~japanese	マテリアルに使用されるテクスチャ
*/
struct MaterialTextureParameter
{
	//! 0 - color, 1 - value
	int32_t Type = 0;
	int32_t Index = 0;
};

/**
	@brief	\~english	Material parameter for shaders
			\~japanese	シェーダー向けマテリアルパラメーター
*/
struct MaterialParameter
{
	//! material index in MaterialType::File
	int32_t MaterialIndex = -1;

	//! used textures in MaterialType::File
	std::vector<MaterialTextureParameter> MaterialTextures;

	//! used uniforms in MaterialType::File
	std::vector<std::array<float, 4>> MaterialUniforms;
};

/**
	@brief	\~english	Parameters about a depth which is passed into a renderer
			\~japanese	レンダラーに渡されるデプスに関するパラメーター
*/
struct NodeRendererDepthParameter
{
	float DepthOffset = 0.0f;
	bool IsDepthOffsetScaledWithCamera = false;
	bool IsDepthOffsetScaledWithParticleScale = false;
	ZSortType ZSort = ZSortType::None;
	float SuppressionOfScalingByDepth = 1.0f;
	float DepthClipping = FLT_MAX;
};

/**
	@brief	\~english	Common parameters which is passed into a renderer
			\~japanese	レンダラーに渡される共通に関するパラメーター
*/
struct NodeRendererBasicParameter
{
	RendererMaterialType MaterialType = RendererMaterialType::Default;
	int32_t Texture1Index = -1;
	int32_t Texture2Index = -1;
#ifdef __EFFEKSEER_BUILD_VERSION16__
	int32_t Texture3Index = -1;
#endif
	float DistortionIntensity = 0.0f;
	MaterialParameter* MaterialParameterPtr = nullptr;
	AlphaBlendType AlphaBlend = AlphaBlendType::Blend;

	TextureFilterType TextureFilter1 = TextureFilterType::Nearest;
	TextureWrapType TextureWrap1 = TextureWrapType::Repeat;
	TextureFilterType TextureFilter2 = TextureFilterType::Nearest;
	TextureWrapType TextureWrap2 = TextureWrapType::Repeat;
#ifdef __EFFEKSEER_BUILD_VERSION16__
	TextureFilterType TextureFilter3 = TextureFilterType::Nearest;
	TextureWrapType TextureWrap3 = TextureWrapType::Repeat;

	bool EnableInterpolation = false;
	int32_t UVLoopType = 0;
	int32_t InterpolationType = 0;
	int32_t FlipbookDivideX = 1;
	int32_t FlipbookDivideY = 1;
#endif
};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
} // namespace Effekseer
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#endif // __EFFEKSEER_BASE_PRE_H__

#ifndef __EFFEKSEER_BASE_H__
#define __EFFEKSEER_BASE_H__


#include <assert.h>
#include <float.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <array>
#include <cfloat>
#include <chrono>
#include <list>
#include <map>
#include <mutex>
#include <queue>
#include <set>
#include <string>
#include <thread>
#include <vector>

#ifdef _WIN32
#define EFK_STDCALL __stdcall
#else
#define EFK_STDCALL
#endif

typedef char16_t EFK_CHAR;

namespace Effekseer
{

struct Color;

class Manager;
class ManagerImplemented;
class Effect;
class EffectImplemented;
class EffectNode;
class EffectNodeImplemented;
class EffectNodeRoot;
class EffectNodeSprite;
class EffectNodeRibbon;
class EffectNodeModel;

class InstanceGlobal;
class InstanceContainer;
class Instance;
class InstanceChunk;
class InstanceGroup;

class ParticleRenderer;
class SpriteRenderer;
class RibbonRenderer;
class ModelRenderer;
class TrackRenderer;

class Setting;
class FileReader;
class FileWriter;
class FileInterface;
class EffectLoader;
class TextureLoader;
class SoundLoader;
class ModelLoader;
class MaterialLoader;

class Model;
class InternalScript;

#ifdef _DEBUG_EFFEKSEER
#define EffekseerPrintDebug(...) printf(__VA_ARGS__)
#else
#define EffekseerPrintDebug(...)
#endif

/**
	@brief	A state of instances
*/
enum eInstanceState
{
	/**
		@brief	Active
	*/
	INSTANCE_STATE_ACTIVE,

	/**
		@brief	Removing
	*/
	INSTANCE_STATE_REMOVING,
	/**
		@brief	Removed
	*/
	INSTANCE_STATE_REMOVED,

	INSTANCE_STATE_DWORD = 0x7fffffff,
};

/**
	@brief	A type of node
*/
enum eEffectNodeType
{
	EFFECT_NODE_TYPE_ROOT = -1,
	EFFECT_NODE_TYPE_NONE = 0,
	EFFECT_NODE_TYPE_SPRITE = 2,
	EFFECT_NODE_TYPE_RIBBON = 3,
	EFFECT_NODE_TYPE_RING = 4,
	EFFECT_NODE_TYPE_MODEL = 5,
	EFFECT_NODE_TYPE_TRACK = 6,

	EFFECT_NODE_TYPE_DWORD = 0x7fffffff,
};

class StringHelper
{
public:
	template <typename T>
	static std::vector<std::basic_string<T>> Split(const std::basic_string<T>& s, T delim)
	{
		std::vector<std::basic_string<T>> elems;

		int32_t start = 0;

		for (size_t i = 0; i < s.size(); i++)
		{
			if (s[i] == delim)
			{
				elems.emplace_back(s.substr(start, i - start));
				start = i + 1;
			}
		}

		if (start == s.size())
		{
			elems.emplace_back(std::basic_string<T>());
		}
		else
		{
			elems.emplace_back(s.substr(start, s.size() - start));
		}

		return elems;
	}

	template <typename T>
	static std::basic_string<T> Replace(std::basic_string<T> target, std::basic_string<T> from_, std::basic_string<T> to_)
	{
		auto Pos = target.find(from_);

		while (Pos != std::basic_string<T>::npos)
		{
			target.replace(Pos, from_.length(), to_);
			Pos = target.find(from_, Pos + to_.length());
		}

		return target;
	}

	template <typename T, typename U>
	static std::basic_string<T> To(const U* str)
	{
		std::basic_string<T> ret;
		size_t len = 0;
		while (str[len] != 0)
		{
			len++;
		}

		ret.resize(len);

		for (size_t i = 0; i < len; i++)
		{
			ret[i] = static_cast<T>(str[i]);
		}

		return ret;
	}
};

class PathHelper
{
private:
	template <typename T>
	static std::basic_string<T> Normalize(const std::vector<std::basic_string<T>>& paths)
	{
		std::vector<std::basic_string<T>> elems;

		for (size_t i = 0; i < paths.size(); i++)
		{
			if (paths[i] == StringHelper::To<T>(".."))
			{
				if (elems.size() > 0 && elems.back() != StringHelper::To<T>(".."))
				{
					elems.pop_back();
				}
				else
				{
					elems.push_back(StringHelper::To<T>(".."));
				}
			}
			else
			{
				elems.push_back(paths[i]);
			}
		}

		std::basic_string<T> ret;

		for (size_t i = 0; i < elems.size(); i++)
		{
			ret += elems[i];

			if (i != elems.size() - 1)
			{
				ret += StringHelper::To<T>("/");
			}
		}

		return ret;
	}

public:
	template <typename T>
	static std::basic_string<T> Normalize(const std::basic_string<T>& path)
	{
		if (path.size() == 0)
			return path;

		auto paths =
			StringHelper::Split(StringHelper::Replace(path, StringHelper::To<T>("\\"), StringHelper::To<T>("/")), static_cast<T>('/'));

		return Normalize(paths);
	}

	template <typename T>
	static std::basic_string<T> Relative(const std::basic_string<T>& targetPath, const std::basic_string<T>& basePath)
	{
		if (basePath.size() == 0 || targetPath.size() == 0)
		{
			return targetPath;
		}

		auto targetPaths = StringHelper::Split(StringHelper::Replace(targetPath, StringHelper::To<T>("\\"), StringHelper::To<T>("/")),
											   static_cast<T>('/'));
		auto basePaths =
			StringHelper::Split(StringHelper::Replace(basePath, StringHelper::To<T>("\\"), StringHelper::To<T>("/")), static_cast<T>('/'));

		if (*(basePath.end() - 1) != static_cast<T>('/') && *(basePath.end() - 1) != static_cast<T>('\\'))
		{
			basePaths.pop_back();
		}

		int32_t offset = 0;
		while (targetPaths.size() > offset && basePaths.size() > offset)
		{
			if (targetPaths[offset] == basePaths[offset])
			{
				offset++;
			}
			else
			{
				break;
			}
		}

		std::basic_string<T> ret;

		for (size_t i = offset; i < basePaths.size(); i++)
		{
			ret += StringHelper::To<T>("../");
		}

		for (size_t i = offset; i < targetPaths.size(); i++)
		{
			ret += targetPaths[i];

			if (i != targetPaths.size() - 1)
			{
				ret += StringHelper::To<T>("/");
			}
		}

		return ret;
	}

	template <typename T>
	static std::basic_string<T> Absolute(const std::basic_string<T>& targetPath, const std::basic_string<T>& basePath)
	{
		if (targetPath == StringHelper::To<T>(""))
			return StringHelper::To<T>("");

		if (basePath == StringHelper::To<T>(""))
			return targetPath;

		auto targetPaths = StringHelper::Split(StringHelper::Replace(targetPath, StringHelper::To<T>("\\"), StringHelper::To<T>("/")),
											   static_cast<T>('/'));
		auto basePaths =
			StringHelper::Split(StringHelper::Replace(basePath, StringHelper::To<T>("\\"), StringHelper::To<T>("/")), static_cast<T>('/'));

		if (*(basePath.end() - 1) != static_cast<T>('/') && *(basePath.end() - 1) != static_cast<T>('\\'))
		{
			basePaths.pop_back();
		}

		std::copy(targetPaths.begin(), targetPaths.end(), std::back_inserter(basePaths));

		return Normalize(basePaths);
	}
};

} // namespace Effekseer

#endif // __EFFEKSEER_BASE_H__

#ifndef __EFFEKSEER_PARAMETERS_H__
#define __EFFEKSEER_PARAMETERS_H__


namespace Effekseer
{

/**
	@brief
	\~English	How to treat an uv of texture type for ribbon and track
	\~Japanese	リボンと軌跡向けのテクスチャのUVの扱い方
*/
enum class TextureUVType : int32_t
{
	Strech,
	Tile,
};

/**
	@brief	\~english	uv texture parameters which is passed into a renderer
			\~japanese	レンダラーに渡されるUVTextureに関するパラメーター
*/
struct NodeRendererTextureUVTypeParameter
{
	TextureUVType Type = TextureUVType::Strech;
	float TileLength = 0.0f;
	int32_t TileEdgeHead = 0;
	int32_t TileEdgeTail = 0;
	float TileLoopAreaBegin = 0.0f;
	float TileLoopAreaEnd = 0.0f;

	void Load(uint8_t*& pos, int32_t version);
};

} // namespace Effekseer

#endif // __EFFEKSEER_PARAMETERS_H__

#ifndef __EFFEKSEER_CUSTOM_ALLOCATOR_H__
#define __EFFEKSEER_CUSTOM_ALLOCATOR_H__

#include <list>
#include <map>
#include <new>
#include <set>
#include <vector>

namespace Effekseer
{
/**
	@brief
	\~English get an allocator
	\~Japanese メモリ確保関数を取得する。
*/
MallocFunc GetMallocFunc();

/**
	\~English specify an allocator
	\~Japanese メモリ確保関数を設定する。
*/
void SetMallocFunc(MallocFunc func);

/**
	@brief
	\~English get a deallocator
	\~Japanese メモリ破棄関数を取得する。
*/
FreeFunc GetFreeFunc();

/**
	\~English specify a deallocator
	\~Japanese メモリ破棄関数を設定する。
*/
void SetFreeFunc(FreeFunc func);

/**
	@brief
	\~English get an allocator
	\~Japanese メモリ確保関数を取得する。
*/
AlignedMallocFunc GetAlignedMallocFunc();

/**
	\~English specify an allocator
	\~Japanese メモリ確保関数を設定する。
*/
void SetAlignedMallocFunc(AlignedMallocFunc func);

/**
	@brief
	\~English get a deallocator
	\~Japanese メモリ破棄関数を取得する。
*/
AlignedFreeFunc GetAlignedFreeFunc();

/**
	\~English specify a deallocator
	\~Japanese メモリ破棄関数を設定する。
*/
void SetAlignedFreeFunc(AlignedFreeFunc func);

/**
	@brief
	\~English get an allocator
	\~Japanese メモリ確保関数を取得する。
*/
MallocFunc GetMallocFunc();

/**
	\~English specify an allocator
	\~Japanese メモリ確保関数を設定する。
*/
void SetMallocFunc(MallocFunc func);

/**
	@brief
	\~English get a deallocator
	\~Japanese メモリ破棄関数を取得する。
*/
FreeFunc GetFreeFunc();

/**
	\~English specify a deallocator
	\~Japanese メモリ破棄関数を設定する。
*/
void SetFreeFunc(FreeFunc func);

template <class T>
struct CustomAllocator
{
	using value_type = T;

	CustomAllocator()
	{
	}

	template <class U>
	CustomAllocator(const CustomAllocator<U>&)
	{
	}

	T* allocate(std::size_t n)
	{
		return reinterpret_cast<T*>(GetMallocFunc()(sizeof(T) * static_cast<uint32_t>(n)));
	}
	void deallocate(T* p, std::size_t n)
	{
		GetFreeFunc()(p, sizeof(T) * static_cast<uint32_t>(n));
	}
};

template <class T>
struct CustomAlignedAllocator
{
	using value_type = T;

	CustomAlignedAllocator()
	{
	}

	template <class U>
	CustomAlignedAllocator(const CustomAlignedAllocator<U>&)
	{
	}

	T* allocate(std::size_t n)
	{
		return reinterpret_cast<T*>(GetAlignedMallocFunc()(sizeof(T) * static_cast<uint32_t>(n), 16));
	}
	void deallocate(T* p, std::size_t n)
	{
		GetAlignedFreeFunc()(p, sizeof(T) * static_cast<uint32_t>(n));
	}
};

template <class T, class U>
bool operator==(const CustomAllocator<T>&, const CustomAllocator<U>&)
{
	return true;
}

template <class T, class U>
bool operator!=(const CustomAllocator<T>&, const CustomAllocator<U>&)
{
	return false;
}

template <class T>
using CustomVector = std::vector<T, CustomAllocator<T>>;
template <class T>
using CustomAlignedVector = std::vector<T, CustomAlignedAllocator<T>>;
template <class T>
using CustomList = std::list<T, CustomAllocator<T>>;
template <class T>
using CustomSet = std::set<T, std::less<T>, CustomAllocator<T>>;
template <class T, class U>
using CustomMap = std::map<T, U, std::less<T>, CustomAllocator<std::pair<const T, U>>>;
template <class T, class U>
using CustomAlignedMap = std::map<T, U, std::less<T>, CustomAlignedAllocator<std::pair<const T, U>>>;

} // namespace Effekseer

#endif // __EFFEKSEER_BASE_PRE_H__

#ifndef __EFFEKSEER_BINARY_READER_H__
#define __EFFEKSEER_BINARY_READER_H__


namespace Effekseer
{

enum class BinaryReaderStatus
{
	Reading,
	Complete,
	Failed,
};

/**
	@brief	utility for reading binary data
*/
template <bool IsValidationEnabled>
class BinaryReader
{
private:
	uint8_t* data_ = nullptr;
	size_t size_ = 0;
	size_t offset = 0;
	BinaryReaderStatus status_ = BinaryReaderStatus::Reading;

public:
	BinaryReader(uint8_t* data, size_t size)
	{
		data_ = data;
		size_ = size;
	}

	template <typename T>
	bool Read(T& value)
	{
		if (IsValidationEnabled)
		{

			if (offset + sizeof(T) > size_ || status_ == BinaryReaderStatus::Failed)
			{
				status_ = BinaryReaderStatus::Failed;
				return false;
			}
		}

		memcpy(&value, data_ + offset, sizeof(T));
		offset += sizeof(T);
		return true;
	}

	/**
@brief	read with validation
*/
	template <typename T>
	bool Read(T& value, const T& min_, const T& max_)
	{
		if (IsValidationEnabled)
		{
			if (offset + sizeof(T) > size_ || status_ == BinaryReaderStatus::Failed)
			{
				status_ = BinaryReaderStatus::Failed;
				return false;
			}
		}

		memcpy(&value, data_ + offset, sizeof(T));
		offset += sizeof(T);

		if (IsValidationEnabled)
		{
			if (value < min_ || value > max_)
			{
				status_ = BinaryReaderStatus::Failed;
				return false;
			}
		}

		return true;
	}

	/**
		@brief	read with validation
	*/
	template <typename T, typename U>
	bool Read(T& value, const U& min_, const U& max_)
	{
		if (IsValidationEnabled)
		{
			if (offset + sizeof(T) > size_ || status_ == BinaryReaderStatus::Failed)
			{
				status_ = BinaryReaderStatus::Failed;
				return false;
			}
		}

		memcpy(&value, data_ + offset, sizeof(T));
		offset += sizeof(T);

		if (IsValidationEnabled)
		{
			if (reinterpret_cast<U>(value) < min_ || reinterpret_cast<U>(value) > max_)
			{
				status_ = BinaryReaderStatus::Failed;
				return false;
			}
		}

		return true;
	}

	template <typename T>
	bool Read(T* value, int32_t count)
	{
		if (IsValidationEnabled)
		{
			if (count < 0 || offset + sizeof(T) * count > size_ || status_ == BinaryReaderStatus::Failed)
			{
				status_ = BinaryReaderStatus::Failed;
				return false;
			}
		}

		memcpy(value, data_ + offset, sizeof(T) * count);
		offset += sizeof(T) * count;
		return true;
	}

	template <typename T, typename _Alloc>
	bool Read(std::vector<T, _Alloc>& value, int32_t count)
	{
		if (IsValidationEnabled)
		{
			if (count < 0 || offset + sizeof(T) * count > size_ || status_ == BinaryReaderStatus::Failed)
			{
				status_ = BinaryReaderStatus::Failed;
				return false;
			}
		}

		value.resize(count);

		if (count > 0)
		{
			memcpy(value.data(), data_ + offset, sizeof(T) * count);
			offset += sizeof(T) * count;
		}

		return true;
	}

	BinaryReaderStatus GetStatus() const
	{
		if (status_ == BinaryReaderStatus::Failed)
			return status_;

		return offset == size_ ? BinaryReaderStatus::Complete : BinaryReaderStatus::Reading;
	}

	void AddOffset(size_t length)
	{
		offset += length;
	}

	size_t GetOffset() const
	{
		return offset;
	}
};

} // namespace Effekseer

#endif // __EFFEKSEER_READER_H__


#ifndef __EFFEKSEER_MATH_H__
#define __EFFEKSEER_MATH_H__

#include <cmath>
#include <cstdint>

namespace Effekseer
{

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
const float DefaultEpsilon = 1e-6f;

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
inline float NormalizeAngle(float angle)
{
	union {
		float f;
		int32_t i;
	} ofs, anglebits = {angle};

	ofs.i = (anglebits.i & 0x80000000) | 0x3F000000;
	return angle - ((int)(angle * 0.159154943f + ofs.f) * 6.283185307f);
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
inline void SinCos(float x, float& s, float& c)
{
	x = NormalizeAngle(x);
	float x2 = x * x;
	float x4 = x * x * x * x;
	float x6 = x * x * x * x * x * x;
	float x8 = x * x * x * x * x * x * x * x;
	float x10 = x * x * x * x * x * x * x * x * x * x;
	s = x * (1.0f - x2 / 6.0f + x4 / 120.0f - x6 / 5040.0f + x8 / 362880.0f - x10 / 39916800.0f);
	c = 1.0f - x2 / 2.0f + x4 / 24.0f - x6 / 720.0f + x8 / 40320.0f - x10 / 3628800.0f;
}

} // namespace Effekseer

#endif // __EFFEKSEER_MATH_H__

#ifndef __EFFEKSEER_VECTOR2D_H__
#define __EFFEKSEER_VECTOR2D_H__

//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace Effekseer
{
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
/**
	@brief	3次元ベクトル
*/
struct Vector2D
{
public:
	/**
		@brief	X
	*/
	float X;

	/**
		@brief	Y
	*/
	float Y;

	/**
		@brief	コンストラクタ
	*/
	Vector2D();

	/**
		@brief	コンストラクタ
	*/
	Vector2D(float x, float y);

	Vector2D& operator+=(const Vector2D& value);
};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
} // namespace Effekseer
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#endif // __EFFEKSEER_VECTOR3D_H__


#ifndef __EFFEKSEER_VECTOR3D_H__
#define __EFFEKSEER_VECTOR3D_H__

//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace Effekseer
{
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
/**
	@brief	3次元ベクトル
*/
struct Vector3D
{
public:
	/**
		@brief	X
	*/
	float X;

	/**
		@brief	Y
	*/
	float Y;

	/**
		@brief	Z
	*/
	float Z;

	/**
		@brief	コンストラクタ
	*/
	Vector3D();

	/**
		@brief	コンストラクタ
	*/
	Vector3D(float x, float y, float z);

	Vector3D operator-();

	Vector3D operator+(const Vector3D& o) const;

	Vector3D operator-(const Vector3D& o) const;

	Vector3D operator*(const float& o) const;

	Vector3D operator/(const float& o) const;

	Vector3D operator*(const Vector3D& o) const;

	Vector3D operator/(const Vector3D& o) const;

	Vector3D& operator+=(const Vector3D& o);

	Vector3D& operator-=(const Vector3D& o);

	Vector3D& operator*=(const float& o);

	Vector3D& operator/=(const float& o);

	bool operator==(const Vector3D& o);

	/**
		@brief	加算
	*/
	static void Add(Vector3D* pOut, const Vector3D* pIn1, const Vector3D* pIn2);

	/**
		@brief	減算
	*/
	static Vector3D& Sub(Vector3D& o, const Vector3D& in1, const Vector3D& in2);

	/**
		@brief	長さ
	*/
	static float Length(const Vector3D& in);

	/**
		@brief	長さの二乗
	*/
	static float LengthSq(const Vector3D& in);

	/**
		@brief	内積
	*/
	static float Dot(const Vector3D& in1, const Vector3D& in2);

	/**
		@brief	単位ベクトル
	*/
	static void Normal(Vector3D& o, const Vector3D& in);

	/**
		@brief	外積
		@note
		右手系の場合、右手の親指がin1、人差し指がin2としたとき、中指の方向を返す。<BR>
		左手系の場合、左手の親指がin1、人差し指がin2としたとき、中指の方向を返す。<BR>
	*/
	static Vector3D& Cross(Vector3D& o, const Vector3D& in1, const Vector3D& in2);

	static Vector3D& Transform(Vector3D& o, const Vector3D& in, const Matrix43& mat);

	static Vector3D& Transform(Vector3D& o, const Vector3D& in, const Matrix44& mat);

	static Vector3D& TransformWithW(Vector3D& o, const Vector3D& in, const Matrix44& mat);
};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
} // namespace Effekseer
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#endif // __EFFEKSEER_VECTOR3D_H__


#ifndef __EFFEKSEER_COLOR_H__
#define __EFFEKSEER_COLOR_H__

//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace Effekseer
{
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
enum ColorMode
{
	COLOR_MODE_RGBA,
	COLOR_MODE_HSVA,
	COLOR_MODE_DWORD = 0x7FFFFFFF
};

/**
	@brief	色
*/
#pragma pack(push, 1)
struct Color
{
	/**
		@brief	赤
	*/
	uint8_t R;

	/**
		@brief	緑
	*/
	uint8_t G;

	/**
		@brief	青
	*/
	uint8_t B;

	/**
		@brief	透明度
	*/
	uint8_t A;

	/**
		@brief	コンストラクタ
	*/
	Color() = default;

	/**
		@brief	コンストラクタ
	*/
	Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255);

	/**
		@brief	乗算
	*/
	static Color Mul(Color in1, Color in2);
	static Color Mul(Color in1, float in2);

	/**
		@brief	線形補間
	*/
	static Color Lerp(const Color in1, const Color in2, float t);
};
#pragma pack(pop)
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
} // namespace Effekseer
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#endif // __EFFEKSEER_COLOR_H__


#ifndef __EFFEKSEER_RECTF_H__
#define __EFFEKSEER_RECTF_H__

//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace Effekseer
{
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
/**
	@brief	四角形
*/
struct RectF
{
private:
public:
	float X;

	float Y;

	float Width;

	float Height;

	RectF();

	RectF(float x, float y, float width, float height);

	Vector2D Position() const;

	Vector2D Size() const;
};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
} // namespace Effekseer
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#endif // __EFFEKSEER_RECTF_H__


#ifndef __EFFEKSEER_MATRIX43_H__
#define __EFFEKSEER_MATRIX43_H__

//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace Effekseer
{
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------

struct Matrix44;

/**
	@brief	4x3行列
	@note
	右手系(回転:反時計回り)<BR>
	V[x,y,z,1] * M の形でベクトルとの乗算が可能である。<BR>
	[0,0][0,1][0,2]<BR>
	[1,0][1,1][1,2]<BR>
	[2,0][2,1][2,2]<BR>
	[3,0][3,1][3,2]<BR>
*/
struct Matrix43
{
private:
public:
	/**
		@brief	行列の値
	*/
	float Value[4][3];

	/**
		@brief	単位行列化を行う。
	*/
	void Indentity();

	/**
		@brief	拡大行列化を行う。
		@param	x	[in]	X方向拡大率
		@param	y	[in]	Y方向拡大率
		@param	z	[in]	Z方向拡大率
	*/
	void Scaling(float x, float y, float z);

	/**
		@brief	反時計周り方向のX軸回転行列化を行う。
		@param	angle	[in]	角度(ラジアン)
	*/
	void RotationX(float angle);

	/**
		@brief	反時計周り方向のY軸回転行列化を行う。
		@param	angle	[in]	角度(ラジアン)
	*/
	void RotationY(float angle);

	/**
		@brief	反時計周り方向のZ軸回転行列化を行う。
		@param	angle	[in]	角度(ラジアン)
	*/
	void RotationZ(float angle);

	/**
		@brief	反時計周り方向のXYZ軸回転行列化を行う。
		@param	rx	[in]	角度(ラジアン)
		@param	ry	[in]	角度(ラジアン)
		@param	rz	[in]	角度(ラジアン)
	*/
	void RotationXYZ(float rx, float ry, float rz);

	/**
		@brief	反時計周り方向のZXY軸回転行列化を行う。
		@param	rz	[in]	角度(ラジアン)
		@param	rx	[in]	角度(ラジアン)
		@param	ry	[in]	角度(ラジアン)
	*/
	void RotationZXY(float rz, float rx, float ry);

	/**
		@brief	任意軸に対する反時計周り方向回転行列化を行う。
		@param	axis	[in]	回転軸
		@param	angle	[in]	角度(ラジアン)
	*/
	void RotationAxis(const Vector3D& axis, float angle);

	/**
		@brief	任意軸に対する反時計周り方向回転行列化を行う。
		@param	axis	[in]	回転軸
		@param	s	[in]	サイン
		@param	c	[in]	コサイン
	*/
	void RotationAxis(const Vector3D& axis, float s, float c);

	/**
		@brief	移動行列化を行う。
		@param	x	[in]	X方向移動
		@param	y	[in]	Y方向移動
		@param	z	[in]	Z方向移動
	*/
	void Translation(float x, float y, float z);

	/**
		@brief	行列を、拡大、回転、移動の行列とベクトルに分解する。
		@param	s	[out]	拡大行列
		@param	r	[out]	回転行列
		@param	t	[out]	位置
	*/
	void GetSRT(Vector3D& s, Matrix43& r, Vector3D& t) const;

	/**
		@brief	行列から拡大ベクトルを取得する。
		@param	s	[out]	拡大ベクトル
	*/
	void GetScale(Vector3D& s) const;

	/**
		@brief	行列から回転行列を取得する。
		@param	s	[out]	回転行列
	*/
	void GetRotation(Matrix43& r) const;

	/**
		@brief	行列から移動ベクトルを取得する。
		@param	t	[out]	移動ベクトル
	*/
	void GetTranslation(Vector3D& t) const;

	/**
		@brief	行列の拡大、回転、移動を設定する。
		@param	s	[in]	拡大行列
		@param	r	[in]	回転行列
		@param	t	[in]	位置
	*/
	void SetSRT(const Vector3D& s, const Matrix43& r, const Vector3D& t);

	/**
		@brief	convert into matrix44
	*/
	void ToMatrix44(Matrix44& dst);

	/**
		@brief	check whether all values are not valid number(not nan, not inf)
	*/
	bool IsValid() const;

	/**
		@brief	行列同士の乗算を行う。
		@param	out	[out]	結果
		@param	in1	[in]	乗算の左側
		@param	in2	[in]	乗算の右側
	*/
	static void Multiple(Matrix43& out, const Matrix43& in1, const Matrix43& in2);
};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
} // namespace Effekseer
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#endif // __EFFEKSEER_MATRIX43_H__


#ifndef __EFFEKSEER_MATRIX44_H__
#define __EFFEKSEER_MATRIX44_H__

//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace Effekseer
{
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------

/**
	@brief	行列
	@note
	右手系<BR>
	左手系<BR>
	V[x,y,z,1] * M の形<BR>
	[0,0][0,1][0,2][0,3]
	[1,0][1,1][1,2][1,3]
	[2,0][2,1][2,2][2,3]
	[3,0][3,1][3,2][3,3]
*/
#pragma pack(push, 1)
struct Matrix44
{
private:
public:
	/**
		@brief	コンストラクタ
	*/
	Matrix44();

	/**
		@brief	行列の値
	*/
	float Values[4][4];

	/**
		@brief	単位行列化
	*/
	Matrix44& Indentity();

	/**
	@brief	転置行列化
	*/
	Matrix44& Transpose();

	/**
		@brief	カメラ行列化(右手系)
	*/
	Matrix44& LookAtRH(const Vector3D& eye, const Vector3D& at, const Vector3D& up);

	/**
		@brief	カメラ行列化(左手系)
	*/
	Matrix44& LookAtLH(const Vector3D& eye, const Vector3D& at, const Vector3D& up);

	/**
		@brief	射影行列化(右手系)
	*/
	Matrix44& PerspectiveFovRH(float ovY, float aspect, float zn, float zf);

	/**
		@brief	OpenGL用射影行列化(右手系)
	*/
	Matrix44& PerspectiveFovRH_OpenGL(float ovY, float aspect, float zn, float zf);

	/**
		@brief	射影行列化(左手系)
	*/
	Matrix44& PerspectiveFovLH(float ovY, float aspect, float zn, float zf);

	/**
	 @brief	OpenGL用射影行列化(左手系)
	 */
	Matrix44& PerspectiveFovLH_OpenGL(float ovY, float aspect, float zn, float zf);

	/**
		@brief	正射影行列化(右手系)
	*/
	Matrix44& OrthographicRH(float width, float height, float zn, float zf);

	/**
		@brief	正射影行列化(左手系)
	*/
	Matrix44& OrthographicLH(float width, float height, float zn, float zf);

	/**
		@brief	拡大行列化
	*/
	void Scaling(float x, float y, float z);

	/**
		@brief	X軸回転行列(右手)
	*/
	void RotationX(float angle);

	/**
		@brief	Y軸回転行列(右手)
	*/
	void RotationY(float angle);

	/**
		@brief	Z軸回転行列(右手)
	*/
	void RotationZ(float angle);

	/**
		@brief	移動行列
	*/
	void Translation(float x, float y, float z);

	/**
		@brief	任意軸反時計回転行列
	*/
	void RotationAxis(const Vector3D& axis, float angle);

	/**
		@brief	クオータニオンから行列に変換
	*/
	void Quaternion(float x, float y, float z, float w);

	/**
		@brief	乗算
	*/
	static Matrix44& Mul(Matrix44& o, const Matrix44& in1, const Matrix44& in2);

	/**
		@brief	逆行列
	*/
	static Matrix44& Inverse(Matrix44& o, const Matrix44& in);
};

#pragma pack(pop)
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
} // namespace Effekseer
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#endif // __EFFEKSEER_MATRIX44_H__


#ifndef __EFFEKSEER_FILE_H__
#define __EFFEKSEER_FILE_H__

//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace Effekseer
{
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
/**
	@brief	ファイル読み込みクラス
*/
class FileReader
{
private:
public:
	FileReader()
	{
	}

	virtual ~FileReader()
	{
	}

	virtual size_t Read(void* buffer, size_t size) = 0;

	virtual void Seek(int position) = 0;

	virtual int GetPosition() = 0;

	virtual size_t GetLength() = 0;
};

/**
	@brief	ファイル書き込みクラス
*/
class FileWriter
{
private:
public:
	FileWriter()
	{
	}

	virtual ~FileWriter()
	{
	}

	virtual size_t Write(const void* buffer, size_t size) = 0;

	virtual void Flush() = 0;

	virtual void Seek(int position) = 0;

	virtual int GetPosition() = 0;

	virtual size_t GetLength() = 0;
};

/**
	@brief
	\~English	factory class for io
	\~Japanese	IOのためのファクトリークラス
*/
class FileInterface
{
private:
public:
	FileInterface() = default;
	virtual ~FileInterface() = default;

	virtual FileReader* OpenRead(const EFK_CHAR* path) = 0;

	/**
		@brief
		\~English	try to open a reader. It need not to succeeds in opening it.
		\~Japanese	リーダーを開くことを試します。成功する必要はありません。
	*/
	virtual FileReader* TryOpenRead(const EFK_CHAR* path)
	{
		return OpenRead(path);
	}

	virtual FileWriter* OpenWrite(const EFK_CHAR* path) = 0;
};

} // namespace Effekseer
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#endif // __EFFEKSEER_FILE_H__


#ifndef __EFFEKSEER_DEFAULT_FILE_H__
#define __EFFEKSEER_DEFAULT_FILE_H__

//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace Effekseer
{
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
/**
	@brief	標準のファイル読み込みクラス
*/

class DefaultFileReader : public FileReader
{
private:
	FILE* m_filePtr;

public:
	DefaultFileReader(FILE* filePtr);

	~DefaultFileReader();

	size_t Read(void* buffer, size_t size);

	void Seek(int position);

	int GetPosition();

	size_t GetLength();
};

class DefaultFileWriter : public FileWriter
{
private:
	FILE* m_filePtr;

public:
	DefaultFileWriter(FILE* filePtr);

	~DefaultFileWriter();

	size_t Write(const void* buffer, size_t size);

	void Flush();

	void Seek(int position);

	int GetPosition();

	size_t GetLength();
};

class DefaultFileInterface : public FileInterface
{
private:
public:
	FileReader* OpenRead(const EFK_CHAR* path);

	FileWriter* OpenWrite(const EFK_CHAR* path);
};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
} // namespace Effekseer
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#endif // __EFFEKSEER_DEFAULT_FILE_H__


#ifndef __EFFEKSEER_EFFECT_H__
#define __EFFEKSEER_EFFECT_H__

//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace Effekseer
{
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------

/**
@brief
\~English	Terms where an effect exists
\~Japanese	エフェクトが存在する期間
*/
struct EffectTerm
{
	/**
@brief
\~English	Minimum end time that the effect may exist
\~Japanese	エフェクトが存在する可能性のある最小の終了時間
*/
	int32_t TermMin;

	/**
	@brief
	\~English	Maximum end time that the effect may exist
	\~Japanese	エフェクトが存在する可能性のある最大の終了時間
	*/
	int32_t TermMax;
};

/**
@brief
\~English	Terms where instances exists
\~Japanese	インスタンスが存在する期間
*/
struct EffectInstanceTerm
{
	/**
	@brief
	\~English	Minimum start time that the first instance may exist
	\~Japanese	最初のインスタンスが存在する可能性のある最小の開始時間
	*/
	int32_t FirstInstanceStartMin = 0;

	/**
	@brief
	\~English	Maximum start time that the first instance may exist
	\~Japanese	最初のインスタンスが存在する可能性のある最大の開始時間
	*/
	int32_t FirstInstanceStartMax = 0;

	/**
	@brief
	\~English	Minimum end time that the first instance may exist
	\~Japanese	最初のインスタンスが存在する可能性のある最小の終了時間
	*/
	int32_t FirstInstanceEndMin = INT_MAX;

	/**
	@brief
	\~English	Maximum end time that the first instance may exist
	\~Japanese	最初のインスタンスが存在する可能性のある最大の終了時間
	*/
	int32_t FirstInstanceEndMax = INT_MAX;

	/**
	@brief
	\~English	Minimum start time that the last instance may exist
	\~Japanese	最後のインスタンスが存在する可能性のある最小の開始時間
	*/
	int32_t LastInstanceStartMin = 0;

	/**
	@brief
	\~English	Maximum start time that the last instance may exist
	\~Japanese	最後のインスタンスが存在する可能性のある最大の開始時間
	*/
	int32_t LastInstanceStartMax = 0;

	/**
	@brief
	\~English	Minimum end time that the last instance may exist
	\~Japanese	最後のインスタンスが存在する可能性のある最小の終了時間
	*/
	int32_t LastInstanceEndMin = INT_MAX;

	/**
	@brief
	\~English	Maximum end time that the last instance may exist
	\~Japanese	最後のインスタンスが存在する可能性のある最大の終了時間
	*/
	int32_t LastInstanceEndMax = INT_MAX;
};

/**
	@brief
	\~English A class to edit an instance of EffectParameter for supporting original format when a binary is loaded.
	\~Japanese	独自フォーマットをサポートするための、バイナリが読み込まれた時にEffectParameterのインスタンスを編集するクラス
*/
class EffectFactory : public ReferenceObject
{
public:
	EffectFactory();

	virtual ~EffectFactory();

	/**
	@brief
	\~English load body data(parameters of effect) from a binary
	\~Japanese	バイナリから本体(エフェクトのパラメーター)を読み込む。
	*/
	bool LoadBody(Effect* effect, const void* data, int32_t size, float magnification, const EFK_CHAR* materialPath);

	/**
	@brief
	\~English set texture data into specified index
	\~Japanese	指定されたインデックスにテクスチャを設定する。
	*/
	void SetTexture(Effect* effect, int32_t index, TextureType type, TextureData* data);

	/**
	@brief
	\~English set sound data into specified index
	\~Japanese	指定されたインデックスに音を設定する。
	*/

	void SetSound(Effect* effect, int32_t index, void* data);

	/**
	@brief
	\~English set model data into specified index
	\~Japanese	指定されたインデックスにモデルを設定する。
	*/
	void SetModel(Effect* effect, int32_t index, void* data);

	/**
	@brief
	\~English set material data into specified index
	\~Japanese	指定されたインデックスにマテリアルを設定する。
	*/
	void SetMaterial(Effect* effect, int32_t index, MaterialData* data);

	/**
	@brief
	\~English set loading data
	\~Japanese	ロード用データを設定する。
	*/
	void SetLoadingParameter(Effect* effect, ReferenceObject* obj);

	/**
		@brief
		\~English this method is called to check whether loaded binary are supported. 
		\~Japanese	バイナリがサポートされているか確認するためにこのメソッドが呼ばれる。
	*/
	virtual bool OnCheckIsBinarySupported(const void* data, int32_t size);

	/**
		@brief
		\~English this method is called to check whether reloading are supported.
		\~Japanese	リロードがサポートされているか確認するためにこのメソッドが呼ばれる。
	*/
	virtual bool OnCheckIsReloadSupported();

	/**
		@brief
		\~English this method is called when load a effect from binary
		\~Japanese	バイナリからエフェクトを読み込む時に、このメソッドが呼ばれる。
	*/
	virtual bool OnLoading(Effect* effect, const void* data, int32_t size, float magnification, const EFK_CHAR* materialPath);

	/**
		@brief
		\~English this method is called when load resources
		\~Japanese	リソースを読み込む時に、このメソッドが呼ばれる。
	*/
	virtual void OnLoadingResource(Effect* effect, const void* data, int32_t size, const EFK_CHAR* materialPath);

	/**
	@brief
	\~English this method is called when unload resources
	\~Japanese	リソースを廃棄される時に、このメソッドが呼ばれる。
	*/
	virtual void OnUnloadingResource(Effect* effect);

	/**
	\~English get factory's name
	\~Japanese	ファクトリーの名称を取得する。
	*/
	virtual const char* GetName() const;

	/**
	\~English get whether resources are loaded automatically when a binary is loaded
	\~Japanese	バイナリを読み込んだときに自動的にリソースを読み込むか取得する。
	*/
	virtual bool GetIsResourcesLoadedAutomatically() const;
};

/**
	@brief	
	\~English	Effect parameters
	\~Japanese	エフェクトパラメータークラス
*/
class Effect
	: public IReference
{
protected:
	Effect()
	{
	}
	virtual ~Effect()
	{
	}

public:
	/**
		@brief	エフェクトを生成する。
		@param	manager			[in]	管理クラス
		@param	data			[in]	データ配列の先頭のポインタ
		@param	size			[in]	データ配列の長さ
		@param	magnification	[in]	読み込み時の拡大率
		@param	materialPath	[in]	素材ロード時の基準パス
		@return	エフェクト。失敗した場合はNULLを返す。
	*/
	static Effect* Create(Manager* manager, void* data, int32_t size, float magnification = 1.0f, const EFK_CHAR* materialPath = NULL);

	/**
		@brief	エフェクトを生成する。
		@param	manager			[in]	管理クラス
		@param	path			[in]	読込元のパス
		@param	magnification	[in]	読み込み時の拡大率
		@param	materialPath	[in]	素材ロード時の基準パス
		@return	エフェクト。失敗した場合はNULLを返す。
	*/
	static Effect* Create(Manager* manager, const EFK_CHAR* path, float magnification = 1.0f, const EFK_CHAR* materialPath = NULL);

	/**
		@brief	エフェクトを生成する。
		@param	setting			[in]	設定クラス
		@param	data			[in]	データ配列の先頭のポインタ
		@param	size			[in]	データ配列の長さ
		@param	magnification	[in]	読み込み時の拡大率
		@param	materialPath	[in]	素材ロード時の基準パス
		@return	エフェクト。失敗した場合はNULLを返す。
	*/
	static Effect* Create(Setting* setting, void* data, int32_t size, float magnification = 1.0f, const EFK_CHAR* materialPath = NULL);

	/**
		@brief	エフェクトを生成する。
		@param	setting			[in]	設定クラス
		@param	path			[in]	読込元のパス
		@param	magnification	[in]	読み込み時の拡大率
		@param	materialPath	[in]	素材ロード時の基準パス
		@return	エフェクト。失敗した場合はNULLを返す。
	*/
	static Effect* Create(Setting* setting, const EFK_CHAR* path, float magnification = 1.0f, const EFK_CHAR* materialPath = NULL);

	/**
	@brief	標準のエフェクト読込インスタンスを生成する。
	*/
	static ::Effekseer::EffectLoader* CreateEffectLoader(::Effekseer::FileInterface* fileInterface = NULL);

	/**
	@brief	
	\~English	Get this effect's name. If this effect is loaded from file, default name is file name without extention.
	\~Japanese	エフェクトの名前を取得する。もしファイルからエフェクトを読み込んだ場合、名前は拡張子を除いたファイル名である。
	*/
	virtual const char16_t* GetName() const = 0;

	/**
		\~English	Set this effect's name
	\~Japanese	エフェクトの名前を設定する。
	*/
	virtual void SetName(const char16_t* name) = 0;

	/**
	@brief	設定を取得する。
	@return	設定
	*/
	virtual Setting* GetSetting() const = 0;

	/**
	@brief	\~English	Get the magnification multiplied by the magnification at the time of loaded and exported.
			\~Japanese	読み込み時と出力時の拡大率をかけた拡大率を取得する。
	*/
	virtual float GetMaginification() const = 0;

	/**
		@brief	エフェクトデータのバージョン取得
	*/
	virtual int GetVersion() const = 0;

	/**
		@brief
		\~English	Get loading parameter supecfied by EffectFactory. This parameter is not used unless EffectFactory is used
		\~Japanese	EffectFactoryによって指定されたロード用パラメーターを取得する。EffectFactoryを使用しない限り、子のパラメーターは使用しない。
	*/
	virtual ReferenceObject* GetLoadingParameter() const = 0;

	/**
		@brief	格納されている色画像のポインタを取得する。
		@param	n	[in]	画像のインデックス
		@return	画像のポインタ
	*/
	virtual TextureData* GetColorImage(int n) const = 0;

	/**
	@brief	格納されている画像のポインタの個数を取得する。
	*/
	virtual int32_t GetColorImageCount() const = 0;

	/**
	@brief	\~English	Get a color image's path
	\~Japanese	色画像のパスを取得する。
	*/
	virtual const EFK_CHAR* GetColorImagePath(int n) const = 0;

	/**
	@brief	格納されている法線画像のポインタを取得する。
	@param	n	[in]	画像のインデックス
	@return	画像のポインタ
	*/
	virtual TextureData* GetNormalImage(int n) const = 0;

	/**
	@brief	格納されている法線画像のポインタの個数を取得する。
	*/
	virtual int32_t GetNormalImageCount() const = 0;

	/**
	@brief	\~English	Get a normal image's path
	\~Japanese	法線画像のパスを取得する。
	*/
	virtual const EFK_CHAR* GetNormalImagePath(int n) const = 0;

	/**
	@brief	格納されている歪み画像のポインタを取得する。
	@param	n	[in]	画像のインデックス
	@return	画像のポインタ
	*/
	virtual TextureData* GetDistortionImage(int n) const = 0;

	/**
	@brief	格納されている歪み画像のポインタの個数を取得する。
	*/
	virtual int32_t GetDistortionImageCount() const = 0;

	/**
	@brief	\~English	Get a distortion image's path
	\~Japanese	歪み画像のパスを取得する。
	*/
	virtual const EFK_CHAR* GetDistortionImagePath(int n) const = 0;

	/**
		@brief	格納されている音波形のポインタを取得する。
	*/
	virtual void* GetWave(int n) const = 0;

	/**
	@brief	格納されている音波形のポインタの個数を取得する。
	*/
	virtual int32_t GetWaveCount() const = 0;

	/**
	@brief	\~English	Get a wave's path
	\~Japanese	音波形のパスを取得する。
	*/
	virtual const EFK_CHAR* GetWavePath(int n) const = 0;

	/**
		@brief	格納されているモデルのポインタを取得する。
	*/
	virtual void* GetModel(int n) const = 0;

	/**
	@brief	格納されているモデルのポインタの個数を取得する。
	*/
	virtual int32_t GetModelCount() const = 0;

	/**
	@brief	\~English	Get a model's path
	\~Japanese	モデルのパスを取得する。
	*/
	virtual const EFK_CHAR* GetModelPath(int n) const = 0;

	/**
	@brief	\~English	Get a material's pointer
	\~Japanese	格納されているマテリアルのポインタを取得する。
	*/
	virtual MaterialData* GetMaterial(int n) const = 0;

	/**
	@brief	\~English	Get the number of stored material pointer 
	\~Japanese	格納されているマテリアルのポインタの個数を取得する。
	*/
	virtual int32_t GetMaterialCount() const = 0;

	/**
	@brief	\~English	Get a material's path
	\~Japanese	マテリアルのパスを取得する。
	*/
	virtual const EFK_CHAR* GetMaterialPath(int n) const = 0;

	/**
		@brief
		\~English set texture data into specified index
		\~Japanese	指定されたインデックスにテクスチャを設定する。
	*/
	virtual void SetTexture(int32_t index, TextureType type, TextureData* data) = 0;

	/**
		@brief
		\~English set sound data into specified index
		\~Japanese	指定されたインデックスに音を設定する。
	*/

	virtual void SetSound(int32_t index, void* data) = 0;

	/**
		@brief
		\~English set model data into specified index
		\~Japanese	指定されたインデックスにモデルを設定する。
	*/
	virtual void SetModel(int32_t index, void* data) = 0;

	/**
		@brief
		\~English set material data into specified index
		\~Japanese	指定されたインデックスにマテリアルを設定する。
	*/
	virtual void SetMaterial(int32_t index, MaterialData* data) = 0;

	/**
		@brief
		\~English	Reload this effect
		\~Japanese	エフェクトのリロードを行う。
		@param	data
		\~English	An effect's data
		\~Japanese	エフェクトのデータ
		@param	size
		\~English	An effect's size
		\~Japanese	エフェクトのデータサイズ
		@param	materialPath
		\~English	A path where reaources are loaded
		\~Japanese	リソースの読み込み元
		@param	reloadingThreadType
		\~English	A thread where reload function is called
		\~Japanese	リロードの関数が呼ばれるスレッド
		@return
		\~English	Result
		\~Japanese	結果
		@note
		\~English
		If reloadingThreadType is RenderThread, new resources aren't loaded and old resources aren't disposed.
		\~Japanese
		もし、reloadingThreadType が RenderThreadの場合、新規のリソースは読み込まれず、古いリソースは破棄されない。
	*/
	virtual bool Reload(void* data, int32_t size, const EFK_CHAR* materialPath = nullptr, ReloadingThreadType reloadingThreadType = ReloadingThreadType::Main) = 0;

	/**
		@brief
		\~English	Reload this effect
		\~Japanese	エフェクトのリロードを行う。
		@param	path
		\~English	An effect's path
		\~Japanese	エフェクトのパス
		@param	materialPath
		\~English	A path where reaources are loaded
		\~Japanese	リソースの読み込み元
		@param	reloadingThreadType
		\~English	A thread where reload function is called
		\~Japanese	リロードの関数が呼ばれるスレッド
		@return
		\~English	Result
		\~Japanese	結果
		@note
		\~English
		If reloadingThreadType is RenderThread, new resources aren't loaded and old resources aren't disposed.
		\~Japanese
		もし、reloadingThreadType が RenderThreadの場合、新規のリソースは読み込まれず、古いリソースは破棄されない。
	*/
	virtual bool Reload(const EFK_CHAR* path, const EFK_CHAR* materialPath = nullptr, ReloadingThreadType reloadingThreadType = ReloadingThreadType::Main) = 0;

	/**
		@brief
		\~English	Reload this effect
		\~Japanese	エフェクトのリロードを行う。
		@param	managers
		\~English	An array of manager instances
		\~Japanese	マネージャーの配列
		@param	managersCount
		\~English	Length of array
		\~Japanese	マネージャーの個数
		@param	data
		\~English	An effect's data
		\~Japanese	エフェクトのデータ
		@param	size
		\~English	An effect's size
		\~Japanese	エフェクトのデータサイズ
		@param	materialPath
		\~English	A path where reaources are loaded
		\~Japanese	リソースの読み込み元
		@param	reloadingThreadType
		\~English	A thread where reload function is called
		\~Japanese	リロードの関数が呼ばれるスレッド
		@return
		\~English	Result
		\~Japanese	結果
		@note
		\~English
		If an effect is generated with Setting, the effect in managers is reloaded with managers
		If reloadingThreadType is RenderThread, new resources aren't loaded and old resources aren't disposed.
		\~Japanese
		Settingを用いてエフェクトを生成したときに、Managerを指定することで対象のManager内のエフェクトのリロードを行う。
		もし、reloadingThreadType が RenderThreadの場合、新規のリソースは読み込まれず、古いリソースは破棄されない。
	*/
	virtual bool Reload(Manager** managers, int32_t managersCount, void* data, int32_t size, const EFK_CHAR* materialPath = nullptr, ReloadingThreadType reloadingThreadType = ReloadingThreadType::Main) = 0;

	/**
		@brief
		\~English	Reload this effect
		\~Japanese	エフェクトのリロードを行う。
		@param	managers
		\~English	An array of manager instances
		\~Japanese	マネージャーの配列
		@param	managersCount
		\~English	Length of array
		\~Japanese	マネージャーの個数
		@param	path
		\~English	An effect's path
		\~Japanese	エフェクトのパス
		@param	materialPath
		\~English	A path where reaources are loaded
		\~Japanese	リソースの読み込み元
		@param	reloadingThreadType
		\~English	A thread where reload function is called
		\~Japanese	リロードの関数が呼ばれるスレッド
		@return
		\~English	Result
		\~Japanese	結果
		@note
		\~English
		If an effect is generated with Setting, the effect in managers is reloaded with managers
		If reloadingThreadType is RenderThread, new resources aren't loaded and old resources aren't disposed.
		\~Japanese
		Settingを用いてエフェクトを生成したときに、Managerを指定することで対象のManager内のエフェクトのリロードを行う。
		もし、reloadingThreadType が RenderThreadの場合、新規のリソースは読み込まれず、古いリソースは破棄されない。
	*/
	virtual bool Reload(Manager** managers, int32_t managersCount, const EFK_CHAR* path, const EFK_CHAR* materialPath = nullptr, ReloadingThreadType reloadingThreadType = ReloadingThreadType::Main) = 0;

	/**
		@brief	画像等リソースの再読み込みを行う。
	*/
	virtual void ReloadResources(const void* data = nullptr, int32_t size = 0, const EFK_CHAR* materialPath = nullptr) = 0;

	/**
		@brief	画像等リソースの破棄を行う。
	*/
	virtual void UnloadResources() = 0;

	/**
	@brief	Rootを取得する。
	*/
	virtual EffectNode* GetRoot() const = 0;

	/**
		@brief
	\~English	Calculate a term of instances where the effect exists
	\~Japanese	エフェクトが存在する期間を計算する。
	*/
	virtual EffectTerm CalculateTerm() const = 0;
};

/**
@brief	共通描画パラメーター
@note
大きく変更される可能性があります。
*/
struct EffectBasicRenderParameter
{
	RendererMaterialType MaterialType;
	int32_t ColorTextureIndex;
	AlphaBlendType AlphaBlend;
	TextureFilterType FilterType;
	TextureWrapType WrapType;
	bool ZWrite;
	bool ZTest;
	bool Distortion;
	float DistortionIntensity;
};

/**
@brief	
	\~English	Model parameter
	\~Japanese	モデルパラメーター
@note
	\~English	It may change greatly.
	\~Japanese	大きく変更される可能性があります。

*/
struct EffectModelParameter
{
	bool Lighting;
};

/**
@brief	ノードインスタンス生成クラス
@note
エフェクトのノードの実体を生成する。
*/
class EffectNode
{
public:
	EffectNode()
	{
	}
	virtual ~EffectNode()
	{
	}

	/**
	@brief	ノードが所属しているエフェクトを取得する。
	*/
	virtual Effect* GetEffect() const = 0;

	/**
	@brief	
	\~English	Get a generation in the node tree. The generation increases by 1 as it moves a child node.
	\~Japanese	ノードツリーの世代を取得する。世代は子のノードになるにしたがって1増える。
	*/
	virtual int GetGeneration() const = 0;

	/**
	@brief	子のノードの数を取得する。
	*/
	virtual int GetChildrenCount() const = 0;

	/**
	@brief	子のノードを取得する。
	*/
	virtual EffectNode* GetChild(int index) const = 0;

	/**
	@brief	共通描画パラメーターを取得する。
	*/
	virtual EffectBasicRenderParameter GetBasicRenderParameter() = 0;

	/**
	@brief	共通描画パラメーターを設定する。
	*/
	virtual void SetBasicRenderParameter(EffectBasicRenderParameter param) = 0;

	/**
	@brief	
	\~English	Get a model parameter
	\~Japanese	モデルパラメーターを取得する。
	*/
	virtual EffectModelParameter GetEffectModelParameter() = 0;

	/**
	@brief
	\~English	Calculate a term of instances where instances exists
	\~Japanese	インスタンスが存在する期間を計算する。
	*/
	virtual EffectInstanceTerm CalculateInstanceTerm(EffectInstanceTerm& parentTerm) const = 0;
};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
} // namespace Effekseer
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#endif // __EFFEKSEER_EFFECT_H__


#ifndef __EFFEKSEER_MANAGER_H__
#define __EFFEKSEER_MANAGER_H__

//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace Effekseer
{
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------

/**
	@brief エフェクト管理クラス
*/
class Manager
	: public IReference
{
public:
	/**
	@brief
		@brief
		\~English Parameters for Manager::Draw and Manager::DrawHandle
		\~Japanese Manager::Draw and Manager::DrawHandleに使用するパラメーター
	*/
	struct DrawParameter
	{
		Vector3D CameraPosition;
		Vector3D CameraDirection;

		/**
			@brief
			\~English A bitmask to show effects
			\~Japanese エフェクトを表示するためのビットマスク
			@note
			\~English For example, if effect's layer is 1 and CameraCullingMask's first bit is 1, this effect is shown.
			\~Japanese 例えば、エフェクトのレイヤーが0でカリングマスクの最初のビットが1のときエフェクトは表示される。
		*/
		int32_t CameraCullingMask;

		DrawParameter();
	};

protected:
	Manager()
	{
	}
	virtual ~Manager()
	{
	}

public:
	/**
		@brief マネージャーを生成する。
		@param	instance_max	[in]	最大インスタンス数
		@param	autoFlip		[in]	自動でスレッド間のデータを入れ替えるかどうか、を指定する。trueの場合、Update時に入れ替わる。
		@return	マネージャー
	*/
	static Manager* Create(int instance_max, bool autoFlip = true);

	/**
		@brief マネージャーを破棄する。
		@note
		このマネージャーから生成されたエフェクトは全て強制的に破棄される。
	*/
	virtual void Destroy() = 0;

	/**
		@brief
		\~English get an allocator
		\~Japanese メモリ確保関数を取得する。
	*/
	virtual MallocFunc GetMallocFunc() const = 0;

	/**
		\~English specify an allocator
		\~Japanese メモリ確保関数を設定する。
	*/
	virtual void SetMallocFunc(MallocFunc func) = 0;

	/**
		@brief
		\~English get a deallocator
		\~Japanese メモリ破棄関数を取得する。
	*/
	virtual FreeFunc GetFreeFunc() const = 0;

	/**
		\~English specify a deallocator
		\~Japanese メモリ破棄関数を設定する。
	*/
	virtual void SetFreeFunc(FreeFunc func) = 0;

	/**
		@brief	ランダム関数を取得する。
	*/
	virtual RandFunc GetRandFunc() const = 0;

	/**
		@brief	ランダム関数を設定する。
	*/
	virtual void SetRandFunc(RandFunc func) = 0;

	/**
		@brief	ランダム最大値を取得する。
	*/
	virtual int GetRandMax() const = 0;

	/**
		@brief	ランダム関数を設定する。
	*/
	virtual void SetRandMax(int max_) = 0;

	/**
		@brief	座標系を取得する。
		@return	座標系
	*/
	virtual CoordinateSystem GetCoordinateSystem() const = 0;

	/**
		@brief	座標系を設定する。
		@param	coordinateSystem	[in]	座標系
		@note
		座標系を設定する。
		エフェクトファイルを読み込む前に設定する必要がある。
	*/
	virtual void SetCoordinateSystem(CoordinateSystem coordinateSystem) = 0;

	/**
		@brief	スプライト描画機能を取得する。
	*/
	virtual SpriteRenderer* GetSpriteRenderer() = 0;

	/**
		@brief	スプライト描画機能を設定する。
	*/
	virtual void SetSpriteRenderer(SpriteRenderer* renderer) = 0;

	/**
		@brief	ストライプ描画機能を取得する。
	*/
	virtual RibbonRenderer* GetRibbonRenderer() = 0;

	/**
		@brief	ストライプ描画機能を設定する。
	*/
	virtual void SetRibbonRenderer(RibbonRenderer* renderer) = 0;

	/**
		@brief	リング描画機能を取得する。
	*/
	virtual RingRenderer* GetRingRenderer() = 0;

	/**
		@brief	リング描画機能を設定する。
	*/
	virtual void SetRingRenderer(RingRenderer* renderer) = 0;

	/**
		@brief	モデル描画機能を取得する。
	*/
	virtual ModelRenderer* GetModelRenderer() = 0;

	/**
		@brief	モデル描画機能を設定する。
	*/
	virtual void SetModelRenderer(ModelRenderer* renderer) = 0;

	/**
		@brief	軌跡描画機能を取得する。
	*/
	virtual TrackRenderer* GetTrackRenderer() = 0;

	/**
		@brief	軌跡描画機能を設定する。
	*/
	virtual void SetTrackRenderer(TrackRenderer* renderer) = 0;

	/**
		@brief	設定クラスを取得する。
	*/
	virtual Setting* GetSetting() = 0;

	/**
		@brief	設定クラスを設定する。
		@param	setting	[in]	設定
	*/
	virtual void SetSetting(Setting* setting) = 0;

	/**
		@brief	エフェクト読込クラスを取得する。
	*/
	virtual EffectLoader* GetEffectLoader() = 0;

	/**
		@brief	エフェクト読込クラスを設定する。
	*/
	virtual void SetEffectLoader(EffectLoader* effectLoader) = 0;

	/**
		@brief	テクスチャ読込クラスを取得する。
	*/
	virtual TextureLoader* GetTextureLoader() = 0;

	/**
		@brief	テクスチャ読込クラスを設定する。
	*/
	virtual void SetTextureLoader(TextureLoader* textureLoader) = 0;

	/**
		@brief	サウンド再生機能を取得する。
	*/
	virtual SoundPlayer* GetSoundPlayer() = 0;

	/**
		@brief	サウンド再生機能を設定する。
	*/
	virtual void SetSoundPlayer(SoundPlayer* soundPlayer) = 0;

	/**
		@brief	サウンド読込クラスを取得する
	*/
	virtual SoundLoader* GetSoundLoader() = 0;

	/**
		@brief	サウンド読込クラスを設定する。
	*/
	virtual void SetSoundLoader(SoundLoader* soundLoader) = 0;

	/**
		@brief	モデル読込クラスを取得する。
	*/
	virtual ModelLoader* GetModelLoader() = 0;

	/**
		@brief	モデル読込クラスを設定する。
	*/
	virtual void SetModelLoader(ModelLoader* modelLoader) = 0;

	/**
		@brief
		\~English get a material loader
		\~Japanese マテリアルローダーを取得する。
		@return
		\~English	loader
		\~Japanese ローダー
	*/
	virtual MaterialLoader* GetMaterialLoader() = 0;

	/**
		@brief
		\~English specfiy a material loader
		\~Japanese マテリアルローダーを設定する。
		@param	loader
		\~English	loader
		\~Japanese ローダー
	*/
	virtual void SetMaterialLoader(MaterialLoader* loader) = 0;

	/**
		@brief	エフェクトを停止する。
		@param	handle	[in]	インスタンスのハンドル
	*/
	virtual void StopEffect(Handle handle) = 0;

	/**
		@brief	全てのエフェクトを停止する。
	*/
	virtual void StopAllEffects() = 0;

	/**
		@brief	エフェクトのルートだけを停止する。
		@param	handle	[in]	インスタンスのハンドル
	*/
	virtual void StopRoot(Handle handle) = 0;

	/**
		@brief	エフェクトのルートだけを停止する。
		@param	effect	[in]	エフェクト
	*/
	virtual void StopRoot(Effect* effect) = 0;

	/**
		@brief	エフェクトのインスタンスが存在しているか取得する。
		@param	handle	[in]	インスタンスのハンドル
		@return	存在してるか?
	*/
	virtual bool Exists(Handle handle) = 0;

	/**
		@brief	エフェクトに使用されているインスタンス数を取得する。
		@param	handle	[in]	インスタンスのハンドル
		@return	インスタンス数
		@note
		Rootも個数に含まれる。つまり、Root削除をしていない限り、
		Managerに残っているインスタンス数+エフェクトに使用されているインスタンス数は存在しているRootの数だけ
		最初に確保した個数よりも多く存在する。
	*/
	virtual int32_t GetInstanceCount(Handle handle) = 0;

	/**
		@brief
		\~English Get the number of instances which is used in playing effects
		\~Japanese 全てのエフェクトに使用されているインスタンス数を取得する。
		@return	
		\~English The number of instances
		\~Japanese インスタンス数
		@note
		\~English 
		The number of Root is included. 
		This means that the number of used instances added resting resting instances is larger than the number of allocated onces by the number of root.
		\~Japanese 
		Rootも個数に含まれる。つまり、Root削除をしていない限り、
		Managerに残っているインスタンス数+エフェクトに使用されているインスタンス数は、最初に確保した個数よりも存在しているRootの数の分だけ多く存在する。
	*/
	virtual int32_t GetTotalInstanceCount() const = 0;

	/**
		@brief	エフェクトのインスタンスに設定されている行列を取得する。
		@param	handle	[in]	インスタンスのハンドル
		@return	行列
	*/
	virtual Matrix43 GetMatrix(Handle handle) = 0;

	/**
		@brief	エフェクトのインスタンスに変換行列を設定する。
		@param	handle	[in]	インスタンスのハンドル
		@param	mat		[in]	変換行列
	*/
	virtual void SetMatrix(Handle handle, const Matrix43& mat) = 0;

	/**
		@brief	エフェクトのインスタンスの位置を取得する。
		@param	handle	[in]	インスタンスのハンドル
		@return	位置
	*/
	virtual Vector3D GetLocation(Handle handle) = 0;

	/**
		@brief	エフェクトのインスタンスの位置を指定する。
		@param	x	[in]	X座標
		@param	y	[in]	Y座標
		@param	z	[in]	Z座標
	*/
	virtual void SetLocation(Handle handle, float x, float y, float z) = 0;

	/**
		@brief	エフェクトのインスタンスの位置を指定する。
		@param	location	[in]	位置
	*/
	virtual void SetLocation(Handle handle, const Vector3D& location) = 0;

	/**
		@brief	エフェクトのインスタンスの位置に加算する。
		@param	location	[in]	加算する値
	*/
	virtual void AddLocation(Handle handle, const Vector3D& location) = 0;

	/**
		@brief	エフェクトのインスタンスの回転角度を指定する。(ラジアン)
	*/
	virtual void SetRotation(Handle handle, float x, float y, float z) = 0;

	/**
		@brief	エフェクトのインスタンスの任意軸周りの反時計周りの回転角度を指定する。
		@param	handle	[in]	インスタンスのハンドル
		@param	axis	[in]	軸
		@param	angle	[in]	角度(ラジアン)
	*/
	virtual void SetRotation(Handle handle, const Vector3D& axis, float angle) = 0;

	/**
		@brief	エフェクトのインスタンスの拡大率を指定する。
		@param	handle	[in]	インスタンスのハンドル
		@param	x		[in]	X方向拡大率
		@param	y		[in]	Y方向拡大率
		@param	z		[in]	Z方向拡大率
	*/
	virtual void SetScale(Handle handle, float x, float y, float z) = 0;

	/**
	@brief
		\~English	Specify the color of overall effect.
		\~Japanese	エフェクト全体の色を指定する。
	*/
	virtual void SetAllColor(Handle handle, Color color) = 0;

	/**
		@brief	エフェクトのインスタンスのターゲット位置を指定する。
		@param	x	[in]	X座標
		@param	y	[in]	Y座標
		@param	z	[in]	Z座標
	*/
	virtual void SetTargetLocation(Handle handle, float x, float y, float z) = 0;

	/**
		@brief	エフェクトのインスタンスのターゲット位置を指定する。
		@param	location	[in]	位置
	*/
	virtual void SetTargetLocation(Handle handle, const Vector3D& location) = 0;

	/**
		@brief
		\~English get a dynamic parameter, which changes effect parameters dynamically while playing
		\~Japanese 再生中にエフェクトのパラメーターを変更する動的パラメーターを取得する。
	*/
	virtual float GetDynamicInput(Handle handle, int32_t index) = 0;

	/**
		@brief
		\~English specfiy a dynamic parameter, which changes effect parameters dynamically while playing
		\~Japanese 再生中にエフェクトのパラメーターを変更する動的パラメーターを設定する。
	*/
	virtual void SetDynamicInput(Handle handle, int32_t index, float value) = 0;

	/**
		@brief	エフェクトのベース行列を取得する。
		@param	handle	[in]	インスタンスのハンドル
		@return	ベース行列
	*/
	virtual Matrix43 GetBaseMatrix(Handle handle) = 0;

	/**
		@brief	エフェクトのベース行列を設定する。
		@param	handle	[in]	インスタンスのハンドル
		@param	mat		[in]	設定する行列
		@note
		エフェクト全体の表示位置を指定する行列を設定する。
	*/
	virtual void SetBaseMatrix(Handle handle, const Matrix43& mat) = 0;

	/**
		@brief	エフェクトのインスタンスに廃棄時のコールバックを設定する。
		@param	handle	[in]	インスタンスのハンドル
		@param	callback	[in]	コールバック
	*/
	virtual void SetRemovingCallback(Handle handle, EffectInstanceRemovingCallback callback) = 0;

	/**
	@brief	\~English	Get status that a particle of effect specified is shown.
	\~Japanese	指定したエフェクトのパーティクルが表示されているか取得する。

	@param	handle	\~English	Particle's handle
	\~Japanese	パーティクルのハンドル
	*/
	virtual bool GetShown(Handle handle) = 0;

	/**
		@brief	エフェクトのインスタンスをDraw時に描画するか設定する。
		@param	handle	[in]	インスタンスのハンドル
		@param	shown	[in]	描画するか?
	*/
	virtual void SetShown(Handle handle, bool shown) = 0;

	/**
	@brief	\~English	Get status that a particle of effect specified is paused.
	\~Japanese	指定したエフェクトのパーティクルが一時停止されているか取得する。

	@param	handle	\~English	Particle's handle
			\~Japanese	パーティクルのハンドル
	*/
	virtual bool GetPaused(Handle handle) = 0;

	/**
		@brief	\~English	Pause or resume a particle of effect specified.
		\~Japanese	指定したエフェクトのパーティクルを一時停止、もしくは再開する。

		@param	handle	[in]	インスタンスのハンドル
		@param	paused	[in]	更新するか?
	*/
	virtual void SetPaused(Handle handle, bool paused) = 0;

	/**
			@brief	\~English	Pause or resume all particle of effects.
			\~Japanese	全てのエフェクトのパーティクルを一時停止、もしくは再開する。
			@param	paused \~English	Pause or resume
			\~Japanese	一時停止、もしくは再開
	*/
	virtual void SetPausedToAllEffects(bool paused) = 0;

	/**
		@brief
		\~English	Get a layer index
		\~Japanese	レイヤーのインデックスを取得する
		@note
		\~English For example, if effect's layer is 1 and CameraCullingMask's first bit is 1, this effect is shown.
		\~Japanese 例えば、エフェクトのレイヤーが0でカリングマスクの最初のビットが1のときエフェクトは表示される。
	*/
	virtual int GetLayer(Handle handle) = 0;

	/**
		@brief
		\~English	Set a layer index
		\~Japanese	レイヤーのインデックスを設定する
	*/
	virtual void SetLayer(Handle handle, int32_t layer) = 0;

	/**
	@brief
	\~English	Get a playing speed of particle of effect.
	\~Japanese	エフェクトのパーティクルの再生スピードを取得する。
	@param	handle
	\~English	Particle's handle
	\~Japanese	パーティクルのハンドル
	@return
	\~English	Speed
	\~Japanese	スピード
	*/
	virtual float GetSpeed(Handle handle) const = 0;

	/**
		@brief	エフェクトのインスタンスを再生スピードを設定する。
		@param	handle	[in]	インスタンスのハンドル
		@param	speed	[in]	スピード
	*/
	virtual void SetSpeed(Handle handle, float speed) = 0;

	/**
		@brief	エフェクトがDrawで描画されるか設定する。
				autoDrawがfalseの場合、DrawHandleで描画する必要がある。
		@param	autoDraw	[in]	自動描画フラグ
	*/
	virtual void SetAutoDrawing(Handle handle, bool autoDraw) = 0;

	/**
		@brief	今までのPlay等の処理をUpdate実行時に適用するようにする。
	*/
	virtual void Flip() = 0;

	/**
		@brief
		\~English	Update all effects.
		\~Japanese	全てのエフェクトの更新処理を行う。
		@param	deltaFrame
		\~English	passed time (1 is 1/60 seconds)
		\~Japanese	更新するフレーム数(60fps基準)
	*/
	virtual void Update(float deltaFrame = 1.0f) = 0;

	/**
		@brief
		\~English	Start to update effects.
		\~Japanese	更新処理を開始する。
		@note
		\~English	It is not required if Update is called.
		\~Japanese	Updateを実行する際は、実行する必要はない。
	*/
	virtual void BeginUpdate() = 0;

	/**
		@brief
		\~English	Stop to update effects.
		\~Japanese	更新処理を終了する。
		@note
		\~English	It is not required if Update is called.
		\~Japanese	Updateを実行する際は、実行する必要はない。
	*/
	virtual void EndUpdate() = 0;

	/**
		@brief	
		\~English	Update an effect by a handle.
		\~Japanese	ハンドル単位の更新を行う。
		@param	handle
		\~English	a handle.
		\~Japanese	ハンドル
		@param	deltaFrame
		\~English	passed time (1 is 1/60 seconds)
		\~Japanese	更新するフレーム数(60fps基準)
		@note
		\~English
		You need to call BeginUpdate before starting update and EndUpdate after stopping update.
		\~Japanese	
		更新する前にBeginUpdate、更新し終わった後にEndUpdateを実行する必要がある。
	*/
	virtual void UpdateHandle(Handle handle, float deltaFrame = 1.0f) = 0;

	/**
	@brief	
	\~English	Draw particles.
	\~Japanese	描画処理を行う。
	*/
	virtual void Draw(const Manager::DrawParameter& drawParameter = Manager::DrawParameter()) = 0;

	/**
	@brief
	\~English	Draw particles in the back of priority 0.
	\~Japanese	背面の描画処理を行う。
	*/
	virtual void DrawBack(const Manager::DrawParameter& drawParameter = Manager::DrawParameter()) = 0;

	/**
	@brief
	\~English	Draw particles in the front of priority 0.
	\~Japanese	前面の描画処理を行う。
	*/
	virtual void DrawFront(const Manager::DrawParameter& drawParameter = Manager::DrawParameter()) = 0;

	/**
	@brief
	\~English	Draw particles with a handle.
	\~Japanese	ハンドル単位の描画処理を行う。
	*/
	virtual void DrawHandle(Handle handle, const Manager::DrawParameter& drawParameter = Manager::DrawParameter()) = 0;

	/**
	@brief
	\~English	Draw particles in the back of priority 0.
	\~Japanese	背面のハンドル単位の描画処理を行う。
	*/
	virtual void DrawHandleBack(Handle handle, const Manager::DrawParameter& drawParameter = Manager::DrawParameter()) = 0;

	/**
	@brief
	\~English	Draw particles in the front of priority 0.
	\~Japanese	前面のハンドル単位の描画処理を行う。
	*/
	virtual void DrawHandleFront(Handle handle, const Manager::DrawParameter& drawParameter = Manager::DrawParameter()) = 0;

	/**
		@brief	再生する。
		@param	effect	[in]	エフェクト
		@param	x	[in]	X座標
		@param	y	[in]	Y座標
		@param	z	[in]	Z座標
		@return	エフェクトのインスタンスのハンドル
	*/
	virtual Handle Play(Effect* effect, float x, float y, float z) = 0;

	/**
		@brief
		\~English	Play an effect.
		\~Japanese	エフェクトを再生する。
		@param	effect
		\~English	Played effect
		\~Japanese	再生されるエフェクト
		@param	position
		\~English	Initial position
		\~Japanese	初期位置
		@param	startFrame
		\~English	A time to play from middle
		\~Japanese	途中から再生するための時間
	*/
	virtual Handle Play(Effect* effect, const Vector3D& position, int32_t startFrame = 0) = 0;

	/**
		@brief
		\~English	Get a camera's culling mask to show all effects
		\~Japanese	全てのエフェクトを表示するためのカメラのカリングマスクを取得する。
	*/
	virtual int GetCameraCullingMaskToShowAllEffects() = 0;

	/**
		@brief	Update処理時間を取得。
	*/
	virtual int GetUpdateTime() const = 0;

	/**
		@brief	Draw処理時間を取得。
	*/
	virtual int GetDrawTime() const = 0;

	/**
		@brief
		\~English	Gets the number of remaining allocated instances.
		\~Japanese	残りの確保したインスタンス数を取得する。
	*/
	virtual int32_t GetRestInstancesCount() const = 0;

	/**
		@brief	エフェクトをカリングし描画負荷を減らすための空間を生成する。
		@param	xsize	X方向幅
		@param	ysize	Y方向幅
		@param	zsize	Z方向幅
		@param	layerCount	層数(大きいほどカリングの効率は上がるがメモリも大量に使用する)
	*/
	virtual void CreateCullingWorld(float xsize, float ysize, float zsize, int32_t layerCount) = 0;

	/**
		@brief	カリングを行い、カリングされたオブジェクトのみを描画するようにする。
		@param	cameraProjMat	カメラプロジェクション行列
		@param	isOpenGL		OpenGLによる描画か?
	*/
	virtual void CalcCulling(const Matrix44& cameraProjMat, bool isOpenGL) = 0;

	/**
		@brief	現在存在するエフェクトのハンドルからカリングの空間を配置しなおす。
	*/
	virtual void RessignCulling() = 0;
};
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
} // namespace Effekseer
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#endif // __EFFEKSEER_MANAGER_H__


#ifndef __EFFEKSEER_SIMDTYPE_H__
#define __EFFEKSEER_SIMDTYPE_H__

#include <cstdint>
#include <cmath>

#if defined(__ARM_NEON__) || defined(__ARM_NEON)
// ARMv7/ARM64 NEON

#define EFK_SIMD_NEON

#if defined(_M_ARM64) || defined(__aarch64__)
#define EFK_SIMD_NEON_ARM64
#endif

#include <arm_neon.h>

#elif (defined(_M_AMD64) || defined(_M_X64)) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2) || defined(__SSE2__)
// x86/x86-64 SSE2/AVX2

#define EFK_SIMD_SSE2

#if defined(__AVX2__)
#define EFK_SIMD_AVX2
#endif
#if defined(__AVX__) || defined(EFK_SIMD_AVX2)
#define EFK_SIMD_AVX
#endif
#if defined(__SSE4_2__) || defined(EFK_SIMD_AVX)
#define EFK_SIMD_SSE4_2
#endif
#if defined(__SSE4_1__) || defined(EFK_SIMD_SSE4_2)
#define EFK_SIMD_SSE4_1
#endif
#if defined(__SSSE3__) || defined(EFK_SIMD_SSE4_1)
#define EFK_SIMD_SSSE3
#endif
#if defined(__SSE3__) || defined(EFK_SIMD_SSSE3)
#define EFK_SIMD_SSE3
#endif

#if defined(EFK_SIMD_AVX) || defined(EFK_SIMD_AVX2)
#include <immintrin.h>
#elif defined(EFK_SIMD_SSE4_2)
#include <nmmintrin.h>
#elif defined(EFK_SIMD_SSE4_1)
#include <smmintrin.h>
#elif defined(EFK_SIMD_SSSE3)
#include <tmmintrin.h>
#elif defined(EFK_SIMD_SSE3)
#include <pmmintrin.h>
#elif defined(EFK_SIMD_SSE2)
#include <emmintrin.h>
#endif

#else
// C++ Generic Implementation (Pseudo SIMD)

#define EFK_SIMD_GEN

#endif

#endif // __EFFEKSEER_SIMDTYPE_H__


#ifndef __EFFEKSEER_SIMD4I_GEN_H__
#define __EFFEKSEER_SIMD4I_GEN_H__


#if defined(EFK_SIMD_GEN)

#include <cstring>
#include <algorithm>

namespace Effekseer
{

struct SIMD4f;

/**
	@brief	simd class for generic
*/
struct alignas(16) SIMD4i
{
	union {
		float vf[4];
		int32_t vi[4];
		uint32_t vu[4];
	};

	SIMD4i() = default;
	SIMD4i(const SIMD4i& rhs) = default;
	SIMD4i(int32_t x, int32_t y, int32_t z, int32_t w) { vi[0] = x; vi[1] = y; vi[2] = z; vi[3] = w; }
	SIMD4i(int32_t i) { vi[0] = i; vi[1] = i; vi[2] = i; vi[3] = i; }

	int32_t GetX() const { return vi[0]; }
	int32_t GetY() const { return vi[1]; }
	int32_t GetZ() const { return vi[2]; }
	int32_t GetW() const { return vi[3]; }

	void SetX(int32_t o) { vi[0] = o; }
	void SetY(int32_t o) { vi[1] = o; }
	void SetZ(int32_t o) { vi[2] = o; }
	void SetW(int32_t o) { vi[3] = o; }

	SIMD4f Convert4f() const;
	SIMD4f Cast4f() const;

	SIMD4i& operator+=(const SIMD4i& rhs)
	{
		for (size_t i = 0; i < 4; i++)
		{
			vi[i] += rhs.vi[i];
		}
		return *this;
	}

	SIMD4i& operator-=(const SIMD4i& rhs)
	{
		for (size_t i = 0; i < 4; i++)
		{
			vi[i] -= rhs.vi[i];
		}
		return *this;
	}

	SIMD4i& operator*=(const SIMD4i& rhs)
	{
		for (size_t i = 0; i < 4; i++)
		{
			vi[i] *= rhs.vi[i];
		}
		return *this;
	}

	SIMD4i& operator*=(int32_t rhs)
	{
		for (size_t i = 0; i < 4; i++)
		{
			vi[i] *= rhs;
		}
		return *this;
	}

	SIMD4i& operator/=(const SIMD4i& rhs)
	{
		for (size_t i = 0; i < 4; i++)
		{
			vi[i] /= rhs.vi[i];
		}
		return *this;
	}

	SIMD4i& operator/=(int32_t rhs)
	{
		for (size_t i = 0; i < 4; i++)
		{
			vi[i] /= rhs;
		}
		return *this;
	}

	static SIMD4i Load2(const void* mem);
	static void Store2(void* mem, const SIMD4i& i);
	static SIMD4i Load3(const void* mem);
	static void Store3(void* mem, const SIMD4i& i);
	static SIMD4i Load4(const void* mem);
	static void Store4(void* mem, const SIMD4i& i);

	static SIMD4i SetZero();
	static SIMD4i Abs(const SIMD4i& in);
	static SIMD4i Min(const SIMD4i& lhs, const SIMD4i& rhs);
	static SIMD4i Max(const SIMD4i& lhs, const SIMD4i& rhs);
	static SIMD4i MulAdd(const SIMD4i& a, const SIMD4i& b, const SIMD4i& c);
	static SIMD4i MulSub(const SIMD4i& a, const SIMD4i& b, const SIMD4i& c);

	template<size_t LANE>
	static SIMD4i MulLane(const SIMD4i& lhs, const SIMD4i& rhs);
	template<size_t LANE>
	static SIMD4i MulAddLane(const SIMD4i& a, const SIMD4i& b, const SIMD4i& c);
	template<size_t LANE>
	static SIMD4i MulSubLane(const SIMD4i& a, const SIMD4i& b, const SIMD4i& c);
	template <uint32_t indexX, uint32_t indexY, uint32_t indexZ, uint32_t indexW>
	static SIMD4i Swizzle(const SIMD4i& in);

	template <int COUNT>
	static SIMD4i ShiftL(const SIMD4i& in);
	template <int COUNT>
	static SIMD4i ShiftR(const SIMD4i& in);
	template <int COUNT>
	static SIMD4i ShiftRA(const SIMD4i& in);

	template <uint32_t X, uint32_t Y, uint32_t Z, uint32_t W>
	static SIMD4i Mask();
	static uint32_t MoveMask(const SIMD4i& in);
	static SIMD4i Equal(const SIMD4i& lhs, const SIMD4i& rhs);
	static SIMD4i NotEqual(const SIMD4i& lhs, const SIMD4i& rhs);
	static SIMD4i LessThan(const SIMD4i& lhs, const SIMD4i& rhs);
	static SIMD4i LessEqual(const SIMD4i& lhs, const SIMD4i& rhs);
	static SIMD4i GreaterThan(const SIMD4i& lhs, const SIMD4i& rhs);
	static SIMD4i GreaterEqual(const SIMD4i& lhs, const SIMD4i& rhs);
	static SIMD4i NearEqual(const SIMD4i& lhs, const SIMD4i& rhs, float epsilon = DefaultEpsilon);
	static SIMD4i IsZero(const SIMD4i& in, float epsilon = DefaultEpsilon);
	static void Transpose(SIMD4i& s0, SIMD4i& s1, SIMD4i& s2, SIMD4i& s3);
};

} // namespace Effekseer

namespace Effekseer
{

inline SIMD4i operator+(const SIMD4i& lhs, const SIMD4i& rhs)
{
	SIMD4i ret;
	for (size_t i = 0; i < 4; i++)
	{
		ret.vi[i] = lhs.vi[i] + rhs.vi[i];
	}
	return ret;
}

inline SIMD4i operator-(const SIMD4i& lhs, const SIMD4i& rhs)
{
	SIMD4i ret;
	for (size_t i = 0; i < 4; i++)
	{
		ret.vi[i] = lhs.vi[i] - rhs.vi[i];
	}
	return ret;
}

inline SIMD4i operator*(const SIMD4i& lhs, const SIMD4i& rhs)
{
	SIMD4i ret;
	for (size_t i = 0; i < 4; i++)
	{
		ret.vi[i] = lhs.vi[i] * rhs.vi[i];
	}
	return ret;
}

inline SIMD4i operator*(const SIMD4i& lhs, int32_t rhs)
{
	SIMD4i ret;
	for (size_t i = 0; i < 4; i++)
	{
		ret.vi[i] = lhs.vi[i] * rhs;
	}
	return ret;
}

inline SIMD4i operator/(const SIMD4i& lhs, const SIMD4i& rhs)
{
	SIMD4i ret;
	for (size_t i = 0; i < 4; i++)
	{
		ret.vi[i] = lhs.vi[i] / rhs.vi[i];
	}
	return ret;
}

inline SIMD4i operator/(const SIMD4i& lhs, int32_t rhs)
{
	SIMD4i ret;
	for (size_t i = 0; i < 4; i++)
	{
		ret.vi[i] = lhs.vi[i] / rhs;
	}
	return ret;
}

inline SIMD4i operator&(const SIMD4i& lhs, const SIMD4i& rhs)
{
	SIMD4i ret;
	for (size_t i = 0; i < 4; i++)
	{
		ret.vu[i] = lhs.vu[i] & rhs.vu[i];
	}
	return ret;
}

inline SIMD4i operator|(const SIMD4i& lhs, const SIMD4i& rhs)
{
	SIMD4i ret;
	for (size_t i = 0; i < 4; i++)
	{
		ret.vu[i] = lhs.vu[i] | rhs.vu[i];
	}
	return ret;
}

inline SIMD4i operator^(const SIMD4i& lhs, const SIMD4i& rhs)
{
	SIMD4i ret;
	for (size_t i = 0; i < 4; i++)
	{
		ret.vu[i] = lhs.vu[i] ^ rhs.vu[i];
	}
	return ret;
}

inline bool operator==(const SIMD4i& lhs, const SIMD4i& rhs)
{
	bool ret = true;
	for (size_t i = 0; i < 4; i++)
	{
		ret &= lhs.vi[i] == rhs.vi[i];
	}
	return ret;
}

inline bool operator!=(const SIMD4i& lhs, const SIMD4i& rhs)
{
	bool ret = true;
	for (size_t i = 0; i < 4; i++)
	{
		ret &= lhs.vi[i] == rhs.vi[i];
	}
	return !ret;
}

inline SIMD4i SIMD4i::Load2(const void* mem)
{
	SIMD4i ret;
	memcpy(ret.vi, mem, sizeof(float) * 2);
	// This code causes bugs in asmjs
	// ret.vi[0] = *((float*)mem + 0);
	// ret.vi[1] = *((float*)mem + 1);
	return ret;
}

inline void SIMD4i::Store2(void* mem, const SIMD4i& i)
{
	memcpy(mem, i.vi, sizeof(float) * 2);
	// This code causes bugs in asmjs
	// *((float*)mem + 0) = i.vi[0];
	// *((float*)mem + 1) = i.vi[1];
}

inline SIMD4i SIMD4i::Load3(const void* mem)
{
	SIMD4i ret;
	memcpy(ret.vi, mem, sizeof(float) * 3);
	// This code causes bugs in asmjs
	// ret.vi[0] = *((float*)mem + 0);
	// ret.vi[1] = *((float*)mem + 1);
	// ret.vi[2] = *((float*)mem + 2);
	return ret;
}

inline void SIMD4i::Store3(void* mem, const SIMD4i& i)
{
	memcpy(mem, i.vi, sizeof(float) * 3);
	// This code causes bugs in asmjs
	// *((float*)mem + 0) = i.vi[0];
	// *((float*)mem + 1) = i.vi[1];
	// *((float*)mem + 2) = i.vi[2];
}

inline SIMD4i SIMD4i::Load4(const void* mem)
{
	SIMD4i ret;
	memcpy(ret.vi, mem, sizeof(float) * 4);
	// This code causes bugs in emscripten
	// ret.vi[0] = *((float*)mem + 0);
	// ret.vi[1] = *((float*)mem + 1);
	// ret.vi[2] = *((float*)mem + 2);
	// ret.vi[3] = *((float*)mem + 3);
	return ret;
}

inline void SIMD4i::Store4(void* mem, const SIMD4i& i)
{
	memcpy(mem, i.vi, sizeof(float) * 4);
	// This code causes bugs in asmjs
	// *((float*)mem + 0) = i.vi[0];
	// *((float*)mem + 1) = i.vi[1];
	// *((float*)mem + 2) = i.vi[2];
	// *((float*)mem + 3) = i.vi[3];
}

inline SIMD4i SIMD4i::SetZero()
{
	SIMD4i ret;
	ret.vi[0] = 0;
	ret.vi[1] = 0;
	ret.vi[2] = 0;
	ret.vi[3] = 0;
	return ret;
}

inline SIMD4i SIMD4i::Abs(const SIMD4i& in)
{
	SIMD4i ret;
	for (size_t i = 0; i < 4; i++)
	{
		ret.vi[i] = std::abs(in.vi[i]);
	}
	return ret;
}

inline SIMD4i SIMD4i::Min(const SIMD4i& lhs, const SIMD4i& rhs)
{
	SIMD4i ret;
	for (size_t i = 0; i < 4; i++)
	{
		ret.vi[i] = (lhs.vi[i] < rhs.vi[i]) ? lhs.vi[i] : rhs.vi[i];
	}
	return ret;
}

inline SIMD4i SIMD4i::Max(const SIMD4i& lhs, const SIMD4i& rhs)
{
	SIMD4i ret;
	for (size_t i = 0; i < 4; i++)
	{
		ret.vi[i] = (lhs.vi[i] > rhs.vi[i]) ? lhs.vi[i] : rhs.vi[i];
	}
	return ret;
}

inline SIMD4i SIMD4i::MulAdd(const SIMD4i& a, const SIMD4i& b, const SIMD4i& c)
{
	SIMD4i ret;
	for (size_t i = 0; i < 4; i++)
	{
		ret.vi[i] = a.vi[i] + b.vi[i] * c.vi[i];
}
	return ret;
}

inline SIMD4i SIMD4i::MulSub(const SIMD4i& a, const SIMD4i& b, const SIMD4i& c)
{
	SIMD4i ret;
	for (size_t i = 0; i < 4; i++)
	{
		ret.vi[i] = a.vi[i] - b.vi[i] * c.vi[i];
}
	return ret;
}

template<size_t LANE>
SIMD4i SIMD4i::MulLane(const SIMD4i& lhs, const SIMD4i& rhs)
{
	static_assert(LANE < 4, "LANE is must be less than 4.");
	return lhs * rhs.vi[LANE];
}

template<size_t LANE>
SIMD4i SIMD4i::MulAddLane(const SIMD4i& a, const SIMD4i& b, const SIMD4i& c)
{
	static_assert(LANE < 4, "LANE is must be less than 4.");
	return a + b * c.vi[LANE];
}

template<size_t LANE>
SIMD4i SIMD4i::MulSubLane(const SIMD4i& a, const SIMD4i& b, const SIMD4i& c)
{
	static_assert(LANE < 4, "LANE is must be less than 4.");
	return a - b * c.vi[LANE];
}

template <uint32_t indexX, uint32_t indexY, uint32_t indexZ, uint32_t indexW>
SIMD4i SIMD4i::Swizzle(const SIMD4i& in)
{
	static_assert(indexX < 4, "indexX is must be less than 4.");
	static_assert(indexY < 4, "indexY is must be less than 4.");
	static_assert(indexZ < 4, "indexZ is must be less than 4.");
	static_assert(indexW < 4, "indexW is must be less than 4.");
	return SIMD4i{in.vi[indexX], in.vi[indexY], in.vi[indexZ], in.vi[indexW]};
}

template <int COUNT>
inline SIMD4i Effekseer::SIMD4i::ShiftL(const SIMD4i& lhs)
{
	SIMD4i ret;
	for (size_t i = 0; i < 4; i++)
	{
		ret.vu[i] = lhs.vu[i] << COUNT;
	}
	return ret;
}

template <int COUNT>
inline SIMD4i Effekseer::SIMD4i::ShiftR(const SIMD4i& lhs)
{
	SIMD4i ret;
	for (size_t i = 0; i < 4; i++)
	{
		ret.vu[i] = lhs.vu[i] >> COUNT;
	}
	return ret;
}

template <int COUNT>
inline SIMD4i Effekseer::SIMD4i::ShiftRA(const SIMD4i& lhs)
{
	SIMD4i ret;
	for (size_t i = 0; i < 4; i++)
	{
		ret.vi[i] = lhs.vi[i] >> COUNT;
	}
	return ret;
}

template <uint32_t X, uint32_t Y, uint32_t Z, uint32_t W>
SIMD4i SIMD4i::Mask()
{
	static_assert(X >= 2, "indexX is must be set 0 or 1.");
	static_assert(Y >= 2, "indexY is must be set 0 or 1.");
	static_assert(Z >= 2, "indexZ is must be set 0 or 1.");
	static_assert(W >= 2, "indexW is must be set 0 or 1.");
	SIMD4i ret;
	ret.vu[0] = 0xffffffff * X;
	ret.vu[1] = 0xffffffff * Y;
	ret.vu[2] = 0xffffffff * Z;
	ret.vu[3] = 0xffffffff * W;
	return ret;
}

inline uint32_t SIMD4i::MoveMask(const SIMD4i& in)
{
	return (in.vu[0] & 0x1) | (in.vu[1] & 0x2) | (in.vu[2] & 0x4) | (in.vu[3] & 0x8);
}

inline SIMD4i SIMD4i::Equal(const SIMD4i& lhs, const SIMD4i& rhs)
{
	SIMD4i ret;
	for (size_t i = 0; i < 4; i++)
	{
		ret.vu[i] = (lhs.vi[i] == rhs.vi[i]) ? 0xffffffff : 0;
	}
	return ret;
}

inline SIMD4i SIMD4i::NotEqual(const SIMD4i& lhs, const SIMD4i& rhs)
{
	SIMD4i ret;
	for (size_t i = 0; i < 4; i++)
	{
		ret.vu[i] = (lhs.vi[i] != rhs.vi[i]) ? 0xffffffff : 0;
	}
	return ret;
}

inline SIMD4i SIMD4i::LessThan(const SIMD4i& lhs, const SIMD4i& rhs)
{
	SIMD4i ret;
	for (size_t i = 0; i < 4; i++)
	{
		ret.vu[i] = (lhs.vi[i] < rhs.vi[i]) ? 0xffffffff : 0;
	}
	return ret;
}

inline SIMD4i SIMD4i::LessEqual(const SIMD4i& lhs, const SIMD4i& rhs)
{
	SIMD4i ret;
	for (size_t i = 0; i < 4; i++)
	{
		ret.vu[i] = (lhs.vi[i] <= rhs.vi[i]) ? 0xffffffff : 0;
	}
	return ret;
}

inline SIMD4i SIMD4i::GreaterThan(const SIMD4i& lhs, const SIMD4i& rhs)
{
	SIMD4i ret;
	for (size_t i = 0; i < 4; i++)
	{
		ret.vu[i] = (lhs.vi[i] > rhs.vi[i]) ? 0xffffffff : 0;
	}
	return ret;
}

inline SIMD4i SIMD4i::GreaterEqual(const SIMD4i& lhs, const SIMD4i& rhs)
{
	SIMD4i ret;
	for (size_t i = 0; i < 4; i++)
	{
		ret.vu[i] = (lhs.vi[i] >= rhs.vi[i]) ? 0xffffffff : 0;
	}
	return ret;
}

inline SIMD4i SIMD4i::NearEqual(const SIMD4i& lhs, const SIMD4i& rhs, float epsilon)
{
	SIMD4i ret;
	for (size_t i = 0; i < 4; i++)
	{
		ret.vu[i] = (std::abs(lhs.vi[i] - rhs.vi[i]) <= epsilon) ? 0xffffffff : 0;
	}
	return ret;
}

inline SIMD4i SIMD4i::IsZero(const SIMD4i& in, float epsilon)
{
	SIMD4i ret;
	for (size_t i = 0; i < 4; i++)
	{
		ret.vu[i] = (std::abs(in.vi[i]) <= epsilon) ? 0xffffffff : 0;
	}
	return ret;
}

inline void SIMD4i::Transpose(SIMD4i& s0, SIMD4i& s1, SIMD4i& s2, SIMD4i& s3)
{
	std::swap(s0.vi[1], s1.vi[0]);
	std::swap(s0.vi[2], s2.vi[0]);
	std::swap(s0.vi[3], s3.vi[0]);
	std::swap(s1.vi[2], s2.vi[1]);
	std::swap(s2.vi[3], s3.vi[2]);
	std::swap(s1.vi[3], s3.vi[1]);
}

} // namespace Effekseer

#endif

#endif // __EFFEKSEER_SIMD4I_GEN_H__

#ifndef __EFFEKSEER_SIMD4I_NEON_H__
#define __EFFEKSEER_SIMD4I_NEON_H__


#if defined(EFK_SIMD_NEON)

namespace Effekseer
{

struct SIMD4f;

/**
 @brief    simd class for sse
 */

struct alignas(16) SIMD4i
{
	int32x4_t s;
	
	SIMD4i() = default;
	SIMD4i(const SIMD4i& rhs) = default;
	SIMD4i(int32x4_t rhs) { s = rhs; }
	SIMD4i(int32_t x, int32_t y, int32_t z, int32_t w) { const int32_t v[4] = {x, y, z, w}; s = vld1q_s32(v); }
	SIMD4i(int32_t i) { s = vdupq_n_s32(i); }
	
	int32_t GetX() const { return vgetq_lane_s32(s, 0); }
	int32_t GetY() const { return vgetq_lane_s32(s, 1); }
	int32_t GetZ() const { return vgetq_lane_s32(s, 2); }
	int32_t GetW() const { return vgetq_lane_s32(s, 3); }
	
	void SetX(int32_t i) { s = vsetq_lane_s32(i, s, 0); }
	void SetY(int32_t i) { s = vsetq_lane_s32(i, s, 1); }
	void SetZ(int32_t i) { s = vsetq_lane_s32(i, s, 2); }
	void SetW(int32_t i) { s = vsetq_lane_s32(i, s, 3); }
	
	SIMD4f Convert4f() const;
	SIMD4f Cast4f() const;
	
	SIMD4i& operator+=(const SIMD4i& rhs);
	SIMD4i& operator-=(const SIMD4i& rhs);
	SIMD4i& operator*=(const SIMD4i& rhs);
	SIMD4i& operator*=(int32_t rhs);
	SIMD4i& operator/=(const SIMD4i& rhs);
	SIMD4i& operator/=(int32_t rhs);
	
	static SIMD4i Load2(const void* mem);
	static void Store2(void* mem, const SIMD4i& i);
	static SIMD4i Load3(const void* mem);
	static void Store3(void* mem, const SIMD4i& i);
	static SIMD4i Load4(const void* mem);
	static void Store4(void* mem, const SIMD4i& i);
	
	static SIMD4i SetZero();
	static SIMD4i Abs(const SIMD4i& in);
	static SIMD4i Min(const SIMD4i& lhs, const SIMD4i& rhs);
	static SIMD4i Max(const SIMD4i& lhs, const SIMD4i& rhs);
	static SIMD4i MulAdd(const SIMD4i& a, const SIMD4i& b, const SIMD4i& c);
	static SIMD4i MulSub(const SIMD4i& a, const SIMD4i& b, const SIMD4i& c);
	
	template<size_t LANE>
	static SIMD4i MulLane(const SIMD4i& lhs, const SIMD4i& rhs);
	template<size_t LANE>
	static SIMD4i MulAddLane(const SIMD4i& a, const SIMD4i& b, const SIMD4i& c);
	template<size_t LANE>
	static SIMD4i MulSubLane(const SIMD4i& a, const SIMD4i& b, const SIMD4i& c);
	template <uint32_t indexX, uint32_t indexY, uint32_t indexZ, uint32_t indexW>
	static SIMD4i Swizzle(const SIMD4i& v);
	
	template <int COUNT>
	static SIMD4i ShiftL(const SIMD4i& in);
	template <int COUNT>
	static SIMD4i ShiftR(const SIMD4i& in);
	template <int COUNT>
	static SIMD4i ShiftRA(const SIMD4i& in);
	
	template <uint32_t X, uint32_t Y, uint32_t Z, uint32_t W>
	static SIMD4i Mask();
	static uint32_t MoveMask(const SIMD4i& in);
	static SIMD4i Equal(const SIMD4i& lhs, const SIMD4i& rhs);
	static SIMD4i NotEqual(const SIMD4i& lhs, const SIMD4i& rhs);
	static SIMD4i LessThan(const SIMD4i& lhs, const SIMD4i& rhs);
	static SIMD4i LessEqual(const SIMD4i& lhs, const SIMD4i& rhs);
	static SIMD4i GreaterThan(const SIMD4i& lhs, const SIMD4i& rhs);
	static SIMD4i GreaterEqual(const SIMD4i& lhs, const SIMD4i& rhs);
	static SIMD4i NearEqual(const SIMD4i& lhs, const SIMD4i& rhs, int32_t epsilon = DefaultEpsilon);
	static SIMD4i IsZero(const SIMD4i& in, int32_t epsilon = DefaultEpsilon);
	static void Transpose(SIMD4i& s0, SIMD4i& s1, SIMD4i& s2, SIMD4i& s3);
	
private:
	static SIMD4i SwizzleYZX(const SIMD4i& in);
	static SIMD4i SwizzleZXY(const SIMD4i& in);
};

} // namespace Effekseer

namespace Effekseer
{

inline SIMD4i operator+(const SIMD4i& lhs, const SIMD4i& rhs)
{
	return vaddq_s32(lhs.s, rhs.s);
}

inline SIMD4i operator-(const SIMD4i& lhs, const SIMD4i& rhs)
{
	return vsubq_s32(lhs.s, rhs.s);
}

inline SIMD4i operator*(const SIMD4i& lhs, const SIMD4i& rhs)
{
	return vmulq_s32(lhs.s, rhs.s);
}

inline SIMD4i operator*(const SIMD4i& lhs, int32_t rhs)
{
	return vmulq_n_s32(lhs.s, rhs);
}

inline SIMD4i operator/(const SIMD4i& lhs, const SIMD4i& rhs)
{
#if defined(EFK_NEON_ARM64)
	return vdivq_s32(lhs.s, rhs.s);
#else
	return SIMD4i(
		lhs.GetX() / rhs.GetX(),
		lhs.GetY() / rhs.GetY(),
		lhs.GetZ() / rhs.GetZ(),
		lhs.GetW() / rhs.GetW());
#endif
}

inline SIMD4i operator/(const SIMD4i& lhs, int32_t rhs)
{
	return lhs * (1.0f / rhs);
}

inline SIMD4i operator&(const SIMD4i& lhs, const SIMD4i& rhs)
{
	uint32x4_t lhsi = vreinterpretq_u32_s32(lhs.s);
	uint32x4_t rhsi = vreinterpretq_u32_s32(rhs.s);
	return vreinterpretq_s32_u32(vandq_u32(lhsi, rhsi));
}

inline SIMD4i operator|(const SIMD4i& lhs, const SIMD4i& rhs)
{
	uint32x4_t lhsi = vreinterpretq_u32_s32(lhs.s);
	uint32x4_t rhsi = vreinterpretq_u32_s32(rhs.s);
	return vreinterpretq_s32_u32(vorrq_u32(lhsi, rhsi));
}

inline bool operator==(const SIMD4i& lhs, const SIMD4i& rhs)
{
	return SIMD4i::MoveMask(SIMD4i::Equal(lhs, rhs)) == 0xf;
}

inline bool operator!=(const SIMD4i& lhs, const SIMD4i& rhs)
{
	return SIMD4i::MoveMask(SIMD4i::Equal(lhs, rhs)) != 0xf;
}

inline SIMD4i& SIMD4i::operator+=(const SIMD4i& rhs) { return *this = *this + rhs; }
inline SIMD4i& SIMD4i::operator-=(const SIMD4i& rhs) { return *this = *this - rhs; }
inline SIMD4i& SIMD4i::operator*=(const SIMD4i& rhs) { return *this = *this * rhs; }
inline SIMD4i& SIMD4i::operator*=(int32_t rhs) { return *this = *this * rhs; }
inline SIMD4i& SIMD4i::operator/=(const SIMD4i& rhs) { return *this = *this / rhs; }
inline SIMD4i& SIMD4i::operator/=(int32_t rhs) { return *this = *this / rhs; }

inline SIMD4i SIMD4i::Load2(const void* mem)
{
	int32x2_t low = vld1_s32((const int32_t*)mem);
	int32x2_t high = vdup_n_s32(0.0f);
	return vcombine_s32(low, high);
}

inline void SIMD4i::Store2(void* mem, const SIMD4i& i)
{
	vst1_s32((int32_t*)mem, vget_low_s32(i.s));
}

inline SIMD4i SIMD4i::Load3(const void* mem)
{
	int32x2_t low = vld1_s32((const int32_t*)mem);
	int32x2_t high = vld1_lane_s32((const int32_t*)mem + 2, vdup_n_s32(0.0f), 0);
	return vcombine_s32(low, high);
}

inline void SIMD4i::Store3(void* mem, const SIMD4i& i)
{
	vst1_s32((int32_t*)mem, vget_low_s32(i.s));
	vst1q_lane_s32((int32_t*)mem + 2, i.s, 2);
}

inline SIMD4i SIMD4i::Load4(const void* mem)
{
	return vld1q_s32((const int32_t*)mem);
}

inline void SIMD4i::Store4(void* mem, const SIMD4i& i)
{
	vst1q_s32((int32_t*)mem, i.s);
}

inline SIMD4i SIMD4i::SetZero()
{
	return vdupq_n_s32(0.0f);
}

inline SIMD4i SIMD4i::Abs(const SIMD4i& in)
{
	return vabsq_s32(in.s);
}

inline SIMD4i SIMD4i::Min(const SIMD4i& lhs, const SIMD4i& rhs)
{
	return vminq_s32(lhs.s, rhs.s);
}

inline SIMD4i SIMD4i::Max(const SIMD4i& lhs, const SIMD4i& rhs)
{
	return vmaxq_s32(lhs.s, rhs.s);
}

inline SIMD4i SIMD4i::MulAdd(const SIMD4i& a, const SIMD4i& b, const SIMD4i& c)
{
	return vmlaq_s32(a.s, b.s, c.s);
}

inline SIMD4i SIMD4i::MulSub(const SIMD4i& a, const SIMD4i& b, const SIMD4i& c)
{
	return vmlsq_s32(a.s, b.s, c.s);
}

template<size_t LANE>
inline SIMD4i SIMD4i::MulLane(const SIMD4i& lhs, const SIMD4i& rhs)
{
	static_assert(LANE < 4, "LANE is must be less than 4.");
	int32x2_t rhs2 = (LANE < 2) ? vget_low_s32(rhs.s) : vget_high_s32(rhs.s);
	return vmulq_lane_s32(lhs.s, rhs2, LANE & 1);
}

template<size_t LANE>
inline SIMD4i SIMD4i::MulAddLane(const SIMD4i& a, const SIMD4i& b, const SIMD4i& c)
{
	static_assert(LANE < 4, "LANE is must be less than 4.");
	int32x2_t c2 = (LANE < 2) ? vget_low_s32(c.s) : vget_high_s32(c.s);
	return vmlaq_lane_s32(a.s, b.s, c2, LANE & 1);
}

template<size_t LANE>
inline SIMD4i SIMD4i::MulSubLane(const SIMD4i& a, const SIMD4i& b, const SIMD4i& c)
{
	static_assert(LANE < 4, "LANE is must be less than 4.");
	int32x2_t c2 = (LANE < 2) ? vget_low_s32(c.s) : vget_high_s32(c.s);
	return vmlsq_lane_s32(a.s, b.s, c2, LANE & 1);
}

//template <uint32_t indexX, uint32_t indexY, uint32_t indexZ, uint32_t indexW>
//inline SIMD4i SIMD4i::Swizzle(const SIMD4i& v)
//{
//	static_assert(indexX < 4, "indexX is must be less than 4.");
//	static_assert(indexY < 4, "indexY is must be less than 4.");
//	static_assert(indexZ < 4, "indexZ is must be less than 4.");
//	static_assert(indexW < 4, "indexW is must be less than 4.");
//}

template <int COUNT>
inline SIMD4i Effekseer::SIMD4i::ShiftL(const SIMD4i& lhs)
{
	return vreinterpretq_s32_u32(vshlq_n_u32(vreinterpretq_u32_s32(lhs.s), COUNT));
}

template <int COUNT>
inline SIMD4i Effekseer::SIMD4i::ShiftR(const SIMD4i& lhs)
{
	return vreinterpretq_s32_u32(vshrq_n_u32(vreinterpretq_u32_s32(lhs.s), COUNT));
}

template <int COUNT>
inline SIMD4i Effekseer::SIMD4i::ShiftRA(const SIMD4i& lhs)
{
	return vshrq_n_s32(lhs.s, COUNT);
}

template <uint32_t X, uint32_t Y, uint32_t Z, uint32_t W>
inline SIMD4i SIMD4i::Mask()
{
	static_assert(X >= 2, "indexX is must be set 0 or 1.");
	static_assert(Y >= 2, "indexY is must be set 0 or 1.");
	static_assert(Z >= 2, "indexZ is must be set 0 or 1.");
	static_assert(W >= 2, "indexW is must be set 0 or 1.");
	const uint32_t in[4] = {0xffffffff * X, 0xffffffff * Y, 0xffffffff * Z, 0xffffffff * W};
	return vld1q_u32(in);
}

inline uint32_t SIMD4i::MoveMask(const SIMD4i& in)
{
	uint16x4_t u16x4 = vmovn_u32(vreinterpretq_u32_s32(in.s));
	uint16_t u16[4];
	vst1_u16(u16, u16x4);
	return (u16[0] & 1) | (u16[1] & 2) | (u16[2] & 4) | (u16[3] & 8);
}

inline SIMD4i SIMD4i::Equal(const SIMD4i& lhs, const SIMD4i& rhs)
{
	return vreinterpretq_s32_u32(vceqq_s32(lhs.s, rhs.s));
}

inline SIMD4i SIMD4i::NotEqual(const SIMD4i& lhs, const SIMD4i& rhs)
{
	return vreinterpretq_s32_u32(vmvnq_u32(vceqq_s32(lhs.s, rhs.s)));
}

inline SIMD4i SIMD4i::LessThan(const SIMD4i& lhs, const SIMD4i& rhs)
{
	return vreinterpretq_s32_u32(vcltq_s32(lhs.s, rhs.s)); }

inline SIMD4i SIMD4i::LessEqual(const SIMD4i& lhs, const SIMD4i& rhs)
{
	return vreinterpretq_s32_u32(vcleq_s32(lhs.s, rhs.s)); 
}

inline SIMD4i SIMD4i::GreaterThan(const SIMD4i& lhs, const SIMD4i& rhs)
{
	return vreinterpretq_s32_u32(vcgtq_s32(lhs.s, rhs.s)); }

inline SIMD4i SIMD4i::GreaterEqual(const SIMD4i& lhs, const SIMD4i& rhs)
{
	return vreinterpretq_s32_u32(vcgeq_s32(lhs.s, rhs.s)); 
}

inline SIMD4i SIMD4i::NearEqual(const SIMD4i& lhs, const SIMD4i& rhs, int32_t epsilon)
{
	return LessEqual(Abs(lhs - rhs), SIMD4i(epsilon));
}

inline SIMD4i SIMD4i::IsZero(const SIMD4i& in, int32_t epsilon)
{
	return LessEqual(Abs(in), SIMD4i(epsilon));
}

inline void SIMD4i::Transpose(SIMD4i& s0, SIMD4i& s1, SIMD4i& s2, SIMD4i& s3)
{
	int32x4x2_t t0 = vzipq_s32(s0.s, s2.s);
	int32x4x2_t t1 = vzipq_s32(s1.s, s3.s);
	int32x4x2_t t2 = vzipq_s32(t0.val[0], t1.val[0]);
	int32x4x2_t t3 = vzipq_s32(t0.val[1], t1.val[1]);
	
	s0 = t2.val[0];
	s1 = t2.val[1];
	s2 = t3.val[0];
	s3 = t3.val[1];
}

inline SIMD4i SIMD4i::SwizzleYZX(const SIMD4i& in)
{
	int32x4_t ex = vextq_s32(in.s, in.s, 1);
	return vsetq_lane_s32(vgetq_lane_s32(ex, 3), ex, 2);
}

inline SIMD4i SIMD4i::SwizzleZXY(const SIMD4i& in)
{
	int32x4_t ex = vextq_s32(in.s, in.s, 3);
	return vsetq_lane_s32(vgetq_lane_s32(ex, 3), ex, 0);
}

} // namespace Effekseer

#endif
#endif // __EFFEKSEER_SIMD4I_NEON_H__


#ifndef __EFFEKSEER_SIMD4I_SSE_H__
#define __EFFEKSEER_SIMD4I_SSE_H__


#if defined(EFK_SIMD_SSE2)

namespace Effekseer
{

struct SIMD4f;

/**
	@brief	simd class for sse
*/

struct alignas(16) SIMD4i
{
	__m128i s;

	SIMD4i() = default;
	SIMD4i(const SIMD4i& rhs) = default;
	SIMD4i(__m128i rhs) { s = rhs; }
	SIMD4i(__m128 rhs) { s = _mm_castps_si128(rhs); }
	SIMD4i(int32_t x, int32_t y, int32_t z, int32_t w) { s = _mm_setr_epi32((int)x, (int)y, (int)z, (int)w); }
	SIMD4i(int32_t i) { s = _mm_set1_epi32((int)i); }

	int32_t GetX() const { return _mm_cvtsi128_si32(s); }
	int32_t GetY() const { return _mm_cvtsi128_si32(Swizzle<1,1,1,1>(s).s); }
	int32_t GetZ() const { return _mm_cvtsi128_si32(Swizzle<2,2,2,2>(s).s); }
	int32_t GetW() const { return _mm_cvtsi128_si32(Swizzle<3,3,3,3>(s).s); }

	void SetX(int32_t i) { s = _mm_castps_si128(_mm_move_ss(_mm_castsi128_ps(s), _mm_castsi128_ps(_mm_cvtsi32_si128(i)))); }
	void SetY(int32_t i) { s = Swizzle<1,0,2,3>(_mm_castps_si128(_mm_move_ss(_mm_castsi128_ps(Swizzle<1,0,2,3>(s).s), _mm_castsi128_ps(_mm_cvtsi32_si128(i))))).s; }
	void SetZ(int32_t i) { s = Swizzle<2,1,0,3>(_mm_castps_si128(_mm_move_ss(_mm_castsi128_ps(Swizzle<2,1,0,3>(s).s), _mm_castsi128_ps(_mm_cvtsi32_si128(i))))).s; }
	void SetW(int32_t i) { s = Swizzle<3,1,2,0>(_mm_castps_si128(_mm_move_ss(_mm_castsi128_ps(Swizzle<3,1,2,0>(s).s), _mm_castsi128_ps(_mm_cvtsi32_si128(i))))).s; }

	SIMD4f Convert4f() const;
	SIMD4f Cast4f() const;

	SIMD4i& operator+=(const SIMD4i& rhs);
	SIMD4i& operator-=(const SIMD4i& rhs);
	SIMD4i& operator*=(const SIMD4i& rhs);
	SIMD4i& operator*=(int32_t rhs);
	SIMD4i& operator/=(const SIMD4i& rhs);
	SIMD4i& operator/=(int32_t rhs);

	static SIMD4i Load2(const void* mem);
	static void Store2(void* mem, const SIMD4i& i);
	static SIMD4i Load3(const void* mem);
	static void Store3(void* mem, const SIMD4i& i);
	static SIMD4i Load4(const void* mem);
	static void Store4(void* mem, const SIMD4i& i);

	static SIMD4i SetZero();
	static SIMD4i Abs(const SIMD4i& in);
	static SIMD4i Min(const SIMD4i& lhs, const SIMD4i& rhs);
	static SIMD4i Max(const SIMD4i& lhs, const SIMD4i& rhs);
	static SIMD4i MulAdd(const SIMD4i& a, const SIMD4i& b, const SIMD4i& c);
	static SIMD4i MulSub(const SIMD4i& a, const SIMD4i& b, const SIMD4i& c);

	template<size_t LANE>
	static SIMD4i MulLane(const SIMD4i& lhs, const SIMD4i& rhs);
	template<size_t LANE>
	static SIMD4i MulAddLane(const SIMD4i& a, const SIMD4i& b, const SIMD4i& c);
	template<size_t LANE>
	static SIMD4i MulSubLane(const SIMD4i& a, const SIMD4i& b, const SIMD4i& c);
	template <uint32_t indexX, uint32_t indexY, uint32_t indexZ, uint32_t indexW>
	static SIMD4i Swizzle(const SIMD4i& v);

	template <int COUNT>
	static SIMD4i ShiftL(const SIMD4i& in);
	template <int COUNT>
	static SIMD4i ShiftR(const SIMD4i& in);
	template <int COUNT>
	static SIMD4i ShiftRA(const SIMD4i& in);

	template <uint32_t X, uint32_t Y, uint32_t Z, uint32_t W>
	static SIMD4i Mask();
	static uint32_t MoveMask(const SIMD4i& in);
	static SIMD4i Equal(const SIMD4i& lhs, const SIMD4i& rhs);
	static SIMD4i NotEqual(const SIMD4i& lhs, const SIMD4i& rhs);
	static SIMD4i LessThan(const SIMD4i& lhs, const SIMD4i& rhs);
	static SIMD4i LessEqual(const SIMD4i& lhs, const SIMD4i& rhs);
	static SIMD4i GreaterThan(const SIMD4i& lhs, const SIMD4i& rhs);
	static SIMD4i GreaterEqual(const SIMD4i& lhs, const SIMD4i& rhs);
};

} // namespace Effekseer

namespace Effekseer
{

inline SIMD4i operator+(const SIMD4i& lhs, const SIMD4i& rhs)
{
	return SIMD4i{_mm_add_epi32(lhs.s, rhs.s)};
}

inline SIMD4i operator-(const SIMD4i& lhs, const SIMD4i& rhs)
{
	return SIMD4i{_mm_sub_epi32(lhs.s, rhs.s)};
}

inline SIMD4i operator*(const SIMD4i& lhs, const SIMD4i& rhs)
{
#if defined(EFK_SIMD_SSE4_1)
	return _mm_mullo_epi32(lhs.s, rhs.s);
#else
	__m128i tmp1 = _mm_mul_epu32(lhs.s, rhs.s);
	__m128i tmp2 = _mm_mul_epu32(_mm_srli_si128(lhs.s, 4), _mm_srli_si128(rhs.s, 4));
	return _mm_unpacklo_epi32(
		_mm_shuffle_epi32(tmp1, _MM_SHUFFLE(0,0,2,0)),
		_mm_shuffle_epi32(tmp2, _MM_SHUFFLE(0,0,2,0)));
#endif
}

inline SIMD4i operator*(const SIMD4i& lhs, int32_t rhs)
{
#if defined(EFK_SIMD_SSE4_1)
	return _mm_mullo_epi32(lhs.s, _mm_set1_epi32(rhs));
#else
	__m128i tmp1 = _mm_mul_epu32(lhs.s, _mm_set1_epi32(rhs));
	__m128i tmp2 = _mm_mul_epu32(_mm_srli_si128(lhs.s, 4), _mm_set1_epi32(rhs));
	return _mm_unpacklo_epi32(
		_mm_shuffle_epi32(tmp1, _MM_SHUFFLE(0,0,2,0)),
		_mm_shuffle_epi32(tmp2, _MM_SHUFFLE(0,0,2,0)));
#endif
}

inline SIMD4i operator/(const SIMD4i& lhs, const SIMD4i& rhs)
{
	return SIMD4i(
		lhs.GetX() * rhs.GetX(),
		lhs.GetY() * rhs.GetY(),
		lhs.GetZ() * rhs.GetZ(),
		lhs.GetW() * rhs.GetW());
}

inline SIMD4i operator/(const SIMD4i& lhs, int32_t rhs)
{
	return SIMD4i(
		lhs.GetX() * rhs,
		lhs.GetY() * rhs,
		lhs.GetZ() * rhs,
		lhs.GetW() * rhs);
}

inline SIMD4i operator&(const SIMD4i& lhs, const SIMD4i& rhs)
{
	return SIMD4i{_mm_and_si128(lhs.s, rhs.s)};
}

inline SIMD4i operator|(const SIMD4i& lhs, const SIMD4i& rhs)
{
	return SIMD4i{_mm_or_si128(lhs.s, rhs.s)};
}

inline SIMD4i operator^(const SIMD4i& lhs, const SIMD4i& rhs)
{
	return SIMD4i{_mm_xor_si128(lhs.s, rhs.s)};
}

inline bool operator==(const SIMD4i& lhs, const SIMD4i& rhs)
{
	return SIMD4i::MoveMask(SIMD4i::Equal(lhs, rhs)) == 0xf;
}

inline bool operator!=(const SIMD4i& lhs, const SIMD4i& rhs)
{
	return SIMD4i::MoveMask(SIMD4i::Equal(lhs, rhs)) != 0xf;
}

inline SIMD4i& SIMD4i::operator+=(const SIMD4i& rhs) { return *this = *this + rhs; }
inline SIMD4i& SIMD4i::operator-=(const SIMD4i& rhs) { return *this = *this - rhs; }
inline SIMD4i& SIMD4i::operator*=(const SIMD4i& rhs) { return *this = *this * rhs; }
inline SIMD4i& SIMD4i::operator*=(int32_t rhs) { return *this = *this * rhs; }
inline SIMD4i& SIMD4i::operator/=(const SIMD4i& rhs) { return *this = *this / rhs; }
inline SIMD4i& SIMD4i::operator/=(int32_t rhs) { return *this = *this / rhs; }

inline SIMD4i SIMD4i::Load2(const void* mem)
{
	__m128 x = _mm_load_ss((const float*)mem + 0);
	__m128 y = _mm_load_ss((const float*)mem + 1);
	return _mm_castps_si128(_mm_unpacklo_ps(x, y));
}

inline void SIMD4i::Store2(void* mem, const SIMD4i& i)
{
	SIMD4i t1 = Swizzle<1,1,1,1>(i);
	_mm_store_ss((float*)mem + 0, _mm_castsi128_ps(i.s));
	_mm_store_ss((float*)mem + 1, _mm_castsi128_ps(t1.s));
}

inline SIMD4i SIMD4i::Load3(const void* mem)
{
	__m128 x = _mm_load_ss((const float*)mem + 0);
	__m128 y = _mm_load_ss((const float*)mem + 1);
	__m128 z = _mm_load_ss((const float*)mem + 2);
	__m128 xy = _mm_unpacklo_ps(x, y);
	return _mm_castps_si128(_mm_movelh_ps(xy, z));
}

inline void SIMD4i::Store3(void* mem, const SIMD4i& i)
{
	SIMD4i t1 = Swizzle<1,1,1,1>(i);
	SIMD4i t2 = Swizzle<2,2,2,2>(i);
	_mm_store_ss((float*)mem + 0, _mm_castsi128_ps(i.s));
	_mm_store_ss((float*)mem + 1, _mm_castsi128_ps(t1.s));
	_mm_store_ss((float*)mem + 2, _mm_castsi128_ps(t2.s));
}

inline SIMD4i SIMD4i::Load4(const void* mem)
{
	return _mm_loadu_si128((const __m128i*)mem);
}

inline void SIMD4i::Store4(void* mem, const SIMD4i& i)
{
	_mm_storeu_si128((__m128i*)mem, i.s);
}

inline SIMD4i SIMD4i::SetZero()
{
	return _mm_setzero_si128();
}

inline SIMD4i SIMD4i::Abs(const SIMD4i& in)
{
#if defined(EFK_SIMD_SSSE3)
	return _mm_abs_epi32(in.s);
#else
	__m128i sign = _mm_srai_epi32(in.s, 31);
	return _mm_sub_epi32(_mm_xor_si128(in.s, sign), sign);
#endif
}

inline SIMD4i SIMD4i::Min(const SIMD4i& lhs, const SIMD4i& rhs)
{
#if defined(EFK_SIMD_SSE4_1)
	return _mm_min_epi32(lhs.s, rhs.s);
#else
	__m128i mask = _mm_cmplt_epi32(lhs.s, rhs.s);
	return _mm_or_si128(_mm_and_si128(mask, lhs.s), _mm_andnot_si128(mask, rhs.s));
#endif
}

inline SIMD4i SIMD4i::Max(const SIMD4i& lhs, const SIMD4i& rhs)
{
#if defined(EFK_SIMD_SSE4_1)
	return _mm_max_epi32(lhs.s, rhs.s);
#else
	__m128i mask = _mm_cmpgt_epi32(lhs.s, rhs.s);
	return _mm_or_si128(_mm_and_si128(mask, lhs.s), _mm_andnot_si128(mask, rhs.s));
#endif
}

inline SIMD4i SIMD4i::MulAdd(const SIMD4i& a, const SIMD4i& b, const SIMD4i& c)
{
	return a + b * c;
}

inline SIMD4i SIMD4i::MulSub(const SIMD4i& a, const SIMD4i& b, const SIMD4i& c)
{
	return a - b * c;
}

template<size_t LANE>
SIMD4i SIMD4i::MulLane(const SIMD4i& lhs, const SIMD4i& rhs)
{
	static_assert(LANE < 4, "LANE is must be less than 4.");
	return lhs * SIMD4i::Swizzle<LANE,LANE,LANE,LANE>(rhs);
}

template<size_t LANE>
SIMD4i SIMD4i::MulAddLane(const SIMD4i& a, const SIMD4i& b, const SIMD4i& c)
{
	static_assert(LANE < 4, "LANE is must be less than 4.");
	return a + b * SIMD4i::Swizzle<LANE,LANE,LANE,LANE>(c);
}

template<size_t LANE>
SIMD4i SIMD4i::MulSubLane(const SIMD4i& a, const SIMD4i& b, const SIMD4i& c)
{
	static_assert(LANE < 4, "LANE is must be less than 4.");
	return a - b * SIMD4i::Swizzle<LANE,LANE,LANE,LANE>(c);
}

template <uint32_t indexX, uint32_t indexY, uint32_t indexZ, uint32_t indexW>
SIMD4i SIMD4i::Swizzle(const SIMD4i& v)
{
	static_assert(indexX < 4, "indexX is must be less than 4.");
	static_assert(indexY < 4, "indexY is must be less than 4.");
	static_assert(indexZ < 4, "indexZ is must be less than 4.");
	static_assert(indexW < 4, "indexW is must be less than 4.");
	return SIMD4i{_mm_shuffle_epi32(v.s, _MM_SHUFFLE(indexW, indexZ, indexY, indexX))};
}

template <int COUNT>
inline SIMD4i Effekseer::SIMD4i::ShiftL(const SIMD4i& lhs)
{
	return _mm_slli_epi32(lhs.s, COUNT);
}

template <int COUNT>
inline SIMD4i Effekseer::SIMD4i::ShiftR(const SIMD4i& lhs)
{
	return _mm_srli_epi32(lhs.s, COUNT);
}

template <int COUNT>
inline SIMD4i Effekseer::SIMD4i::ShiftRA(const SIMD4i& lhs)
{
	return _mm_srai_epi32(lhs.s, COUNT);
}

template <uint32_t X, uint32_t Y, uint32_t Z, uint32_t W>
inline SIMD4i SIMD4i::Mask()
{
	static_assert(X >= 2, "indexX is must be set 0 or 1.");
	static_assert(Y >= 2, "indexY is must be set 0 or 1.");
	static_assert(Z >= 2, "indexZ is must be set 0 or 1.");
	static_assert(W >= 2, "indexW is must be set 0 or 1.");
	return _mm_setr_epi32(
		(int)(0xffffffff * X), 
		(int)(0xffffffff * Y), 
		(int)(0xffffffff * Z), 
		(int)(0xffffffff * W));
}

inline uint32_t SIMD4i::MoveMask(const SIMD4i& in)
{
	return (uint32_t)_mm_movemask_ps(_mm_castsi128_ps(in.s));
}

inline SIMD4i SIMD4i::Equal(const SIMD4i& lhs, const SIMD4i& rhs)
{
	return SIMD4i{_mm_cmpeq_epi32(lhs.s, rhs.s)};
}

inline SIMD4i SIMD4i::NotEqual(const SIMD4i& lhs, const SIMD4i& rhs)
{
	return SIMD4i{_mm_andnot_si128(_mm_cmpeq_epi32(lhs.s, rhs.s), _mm_set1_epi32(-1))};
}

inline SIMD4i SIMD4i::LessThan(const SIMD4i& lhs, const SIMD4i& rhs)
{
	return SIMD4i{_mm_cmplt_epi32(lhs.s, rhs.s)};
}

inline SIMD4i SIMD4i::LessEqual(const SIMD4i& lhs, const SIMD4i& rhs)
{
	return SIMD4i{_mm_andnot_si128(_mm_cmpgt_epi32(lhs.s, rhs.s), _mm_set1_epi32(-1))};
}

inline SIMD4i SIMD4i::GreaterThan(const SIMD4i& lhs, const SIMD4i& rhs)
{
	return SIMD4i{_mm_cmpgt_epi32(lhs.s, rhs.s)};
}

inline SIMD4i SIMD4i::GreaterEqual(const SIMD4i& lhs, const SIMD4i& rhs)
{
	return SIMD4i{_mm_andnot_si128(_mm_cmplt_epi32(lhs.s, rhs.s), _mm_set1_epi32(-1))};
}

} // namespace Effekseer

#endif

#endif // __EFFEKSEER_SIMD4I_SSE_H__

#ifndef __EFFEKSEER_SIMD4F_GEN_H__
#define __EFFEKSEER_SIMD4F_GEN_H__


#if defined(EFK_SIMD_GEN)

#include <cstring>
#include <algorithm>

namespace Effekseer
{

inline float Sqrt(float x)
{
	return std::sqrt(x);
}
inline float Rsqrt(float x)
{
	return 1.0f / std::sqrt(x);
}

struct SIMD4i;

/**
	@brief	simd class for generic
*/
struct alignas(16) SIMD4f
{
	union {
		float vf[4];
		int32_t vi[4];
		uint32_t vu[4];
	};

	SIMD4f() = default;
	SIMD4f(const SIMD4f& rhs) = default;
	SIMD4f(float x, float y, float z, float w) { vf[0] = x; vf[1] = y; vf[2] = z; vf[3] = w; }
	SIMD4f(float i) { vf[0] = i; vf[1] = i; vf[2] = i; vf[3] = i; }

	float GetX() const { return vf[0]; }
	float GetY() const { return vf[1]; }
	float GetZ() const { return vf[2]; }
	float GetW() const { return vf[3]; }

	void SetX(float o) { vf[0] = o; }
	void SetY(float o) { vf[1] = o; }
	void SetZ(float o) { vf[2] = o; }
	void SetW(float o) { vf[3] = o; }

	template <size_t LANE>
	SIMD4f Dup() { return SIMD4f(vf[LANE], vf[LANE], vf[LANE], vf[LANE]); }

	SIMD4i Convert4i() const;
	SIMD4i Cast4i() const;

	SIMD4f& operator+=(const SIMD4f& rhs)
	{
		for (size_t i = 0; i < 4; i++)
		{
			vf[i] += rhs.vf[i];
		}
		return *this;
	}

	SIMD4f& operator-=(const SIMD4f& rhs)
	{
		for (size_t i = 0; i < 4; i++)
		{
			vf[i] -= rhs.vf[i];
		}
		return *this;
	}

	SIMD4f& operator*=(const SIMD4f& rhs)
	{
		for (size_t i = 0; i < 4; i++)
		{
			vf[i] *= rhs.vf[i];
		}
		return *this;
	}

	SIMD4f& operator*=(float rhs)
	{
		for (size_t i = 0; i < 4; i++)
		{
			vf[i] *= rhs;
		}
		return *this;
	}

	SIMD4f& operator/=(const SIMD4f& rhs)
	{
		for (size_t i = 0; i < 4; i++)
		{
			vf[i] /= rhs.vf[i];
		}
		return *this;
	}

	SIMD4f& operator/=(float rhs)
	{
		for (size_t i = 0; i < 4; i++)
		{
			vf[i] /= rhs;
		}
		return *this;
	}

	static SIMD4f Load2(const void* mem);
	static void Store2(void* mem, const SIMD4f& i);
	static SIMD4f Load3(const void* mem);
	static void Store3(void* mem, const SIMD4f& i);
	static SIMD4f Load4(const void* mem);
	static void Store4(void* mem, const SIMD4f& i);

	static SIMD4f SetZero();
	static SIMD4f SetInt(int32_t x, int32_t y, int32_t z, int32_t w);
	static SIMD4f SetUInt(uint32_t x, uint32_t y, uint32_t z, uint32_t w);
	static SIMD4f Sqrt(const SIMD4f& in);
	static SIMD4f Rsqrt(const SIMD4f& in);
	static SIMD4f Abs(const SIMD4f& in);
	static SIMD4f Min(const SIMD4f& lhs, const SIMD4f& rhs);
	static SIMD4f Max(const SIMD4f& lhs, const SIMD4f& rhs);
	static SIMD4f Floor(const SIMD4f& in);
	static SIMD4f Ceil(const SIMD4f& in);
	static SIMD4f MulAdd(const SIMD4f& a, const SIMD4f& b, const SIMD4f& c);
	static SIMD4f MulSub(const SIMD4f& a, const SIMD4f& b, const SIMD4f& c);

	template<size_t LANE>
	static SIMD4f MulLane(const SIMD4f& lhs, const SIMD4f& rhs);
	template<size_t LANE>
	static SIMD4f MulAddLane(const SIMD4f& a, const SIMD4f& b, const SIMD4f& c);
	template<size_t LANE>
	static SIMD4f MulSubLane(const SIMD4f& a, const SIMD4f& b, const SIMD4f& c);
	template <uint32_t indexX, uint32_t indexY, uint32_t indexZ, uint32_t indexW>
	static SIMD4f Swizzle(const SIMD4f& in);

	static SIMD4f Dot3(const SIMD4f& lhs, const SIMD4f& rhs);
	static SIMD4f Cross3(const SIMD4f& lhs, const SIMD4f& rhs);

	template <uint32_t X, uint32_t Y, uint32_t Z, uint32_t W>
	static SIMD4f Mask();
	static uint32_t MoveMask(const SIMD4f& in);
	static SIMD4f Select(const SIMD4f& mask, const SIMD4f& sel1, const SIMD4f& sel2);
	static SIMD4f Equal(const SIMD4f& lhs, const SIMD4f& rhs);
	static SIMD4f NotEqual(const SIMD4f& lhs, const SIMD4f& rhs);
	static SIMD4f LessThan(const SIMD4f& lhs, const SIMD4f& rhs);
	static SIMD4f LessEqual(const SIMD4f& lhs, const SIMD4f& rhs);
	static SIMD4f GreaterThan(const SIMD4f& lhs, const SIMD4f& rhs);
	static SIMD4f GreaterEqual(const SIMD4f& lhs, const SIMD4f& rhs);
	static SIMD4f NearEqual(const SIMD4f& lhs, const SIMD4f& rhs, float epsilon = DefaultEpsilon);
	static SIMD4f IsZero(const SIMD4f& in, float epsilon = DefaultEpsilon);
	static void Transpose(SIMD4f& s0, SIMD4f& s1, SIMD4f& s2, SIMD4f& s3);
};

} // namespace Effekseer

namespace Effekseer
{

inline SIMD4f operator+(const SIMD4f& lhs, const SIMD4f& rhs)
{
	SIMD4f ret;
	for (size_t i = 0; i < 4; i++)
	{
		ret.vf[i] = lhs.vf[i] + rhs.vf[i];
	}
	return ret;
}

inline SIMD4f operator-(const SIMD4f& lhs, const SIMD4f& rhs)
{
	SIMD4f ret;
	for (size_t i = 0; i < 4; i++)
	{
		ret.vf[i] = lhs.vf[i] - rhs.vf[i];
	}
	return ret;
}

inline SIMD4f operator*(const SIMD4f& lhs, const SIMD4f& rhs)
{
	SIMD4f ret;
	for (size_t i = 0; i < 4; i++)
	{
		ret.vf[i] = lhs.vf[i] * rhs.vf[i];
	}
	return ret;
}

inline SIMD4f operator*(const SIMD4f& lhs, float rhs)
{
	SIMD4f ret;
	for (size_t i = 0; i < 4; i++)
	{
		ret.vf[i] = lhs.vf[i] * rhs;
	}
	return ret;
}

inline SIMD4f operator/(const SIMD4f& lhs, const SIMD4f& rhs)
{
	SIMD4f ret;
	for (size_t i = 0; i < 4; i++)
	{
		ret.vf[i] = lhs.vf[i] / rhs.vf[i];
	}
	return ret;
}

inline SIMD4f operator/(const SIMD4f& lhs, float rhs)
{
	SIMD4f ret;
	for (size_t i = 0; i < 4; i++)
	{
		ret.vf[i] = lhs.vf[i] / rhs;
	}
	return ret;
}

inline SIMD4f operator&(const SIMD4f& lhs, const SIMD4f& rhs)
{
	SIMD4f ret;
	for (size_t i = 0; i < 4; i++)
	{
		ret.vu[i] = lhs.vu[i] & rhs.vu[i];
	}
	return ret;
}

inline SIMD4f operator|(const SIMD4f& lhs, const SIMD4f& rhs)
{
	SIMD4f ret;
	for (size_t i = 0; i < 4; i++)
	{
		ret.vu[i] = lhs.vu[i] | rhs.vu[i];
	}
	return ret;
}

inline SIMD4f operator^(const SIMD4f& lhs, const SIMD4f& rhs)
{
	SIMD4f ret;
	for (size_t i = 0; i < 4; i++)
	{
		ret.vu[i] = lhs.vu[i] ^ rhs.vu[i];
	}
	return ret;
}

inline bool operator==(const SIMD4f& lhs, const SIMD4f& rhs)
{
	bool ret = true;
	for (size_t i = 0; i < 4; i++)
	{
		ret &= lhs.vf[i] == rhs.vf[i];
	}
	return ret;
}

inline bool operator!=(const SIMD4f& lhs, const SIMD4f& rhs)
{
	bool ret = true;
	for (size_t i = 0; i < 4; i++)
	{
		ret &= lhs.vf[i] == rhs.vf[i];
	}
	return !ret;
}

inline SIMD4f SIMD4f::Load2(const void* mem)
{
	SIMD4f ret;
	memcpy(ret.vf, mem, sizeof(float) * 2);
	// This code causes bugs in asmjs
	// ret.vf[0] = *((float*)mem + 0);
	// ret.vf[1] = *((float*)mem + 1);
	return ret;
}

inline void SIMD4f::Store2(void* mem, const SIMD4f& i)
{
	memcpy(mem, i.vf, sizeof(float) * 2);
	// This code causes bugs in asmjs
	// *((float*)mem + 0) = i.vf[0];
	// *((float*)mem + 1) = i.vf[1];
}

inline SIMD4f SIMD4f::Load3(const void* mem)
{
	SIMD4f ret;
	memcpy(ret.vf, mem, sizeof(float) * 3);
	// This code causes bugs in asmjs
	// ret.vf[0] = *((float*)mem + 0);
	// ret.vf[1] = *((float*)mem + 1);
	// ret.vf[2] = *((float*)mem + 2);
	return ret;
}

inline void SIMD4f::Store3(void* mem, const SIMD4f& i)
{
	memcpy(mem, i.vf, sizeof(float) * 3);
	// This code causes bugs in asmjs
	// *((float*)mem + 0) = i.vf[0];
	// *((float*)mem + 1) = i.vf[1];
	// *((float*)mem + 2) = i.vf[2];
}

inline SIMD4f SIMD4f::Load4(const void* mem)
{
	SIMD4f ret;
	memcpy(ret.vf, mem, sizeof(float) * 4);
	// This code causes bugs in emscripten
	// ret.vf[0] = *((float*)mem + 0);
	// ret.vf[1] = *((float*)mem + 1);
	// ret.vf[2] = *((float*)mem + 2);
	// ret.vf[3] = *((float*)mem + 3);
	return ret;
}

inline void SIMD4f::Store4(void* mem, const SIMD4f& i)
{
	memcpy(mem, i.vf, sizeof(float) * 4);
	// This code causes bugs in asmjs
	// *((float*)mem + 0) = i.vf[0];
	// *((float*)mem + 1) = i.vf[1];
	// *((float*)mem + 2) = i.vf[2];
	// *((float*)mem + 3) = i.vf[3];
}

inline SIMD4f SIMD4f::SetZero()
{
	SIMD4f ret;
	ret.vf[0] = 0.0f;
	ret.vf[1] = 0.0f;
	ret.vf[2] = 0.0f;
	ret.vf[3] = 0.0f;
	return ret;
}

inline SIMD4f SIMD4f::SetInt(int32_t x, int32_t y, int32_t z, int32_t w)
{
	SIMD4f ret;
	ret.vu[0] = (uint32_t)x;
	ret.vu[1] = (uint32_t)y;
	ret.vu[2] = (uint32_t)z;
	ret.vu[3] = (uint32_t)w;
	return ret;
}

inline SIMD4f SIMD4f::SetUInt(uint32_t x, uint32_t y, uint32_t z, uint32_t w)
{
	SIMD4f ret;
	ret.vu[0] = (uint32_t)x;
	ret.vu[1] = (uint32_t)y;
	ret.vu[2] = (uint32_t)z;
	ret.vu[3] = (uint32_t)w;
	return ret;
}

inline SIMD4f SIMD4f::Sqrt(const SIMD4f& in)
{
	SIMD4f ret;
	for (size_t i = 0; i < 4; i++)
	{
		ret.vf[i] = std::sqrt(in.vf[i]);
	}
	return ret;
}

inline SIMD4f SIMD4f::Rsqrt(const SIMD4f& in)
{
	SIMD4f ret;
	for (size_t i = 0; i < 4; i++)
	{
		ret.vf[i] = 1.0f / std::sqrt(in.vf[i]);
	}
	return ret;
}

inline SIMD4f SIMD4f::Abs(const SIMD4f& in)
{
	SIMD4f ret;
	for (size_t i = 0; i < 4; i++)
	{
		ret.vf[i] = std::abs(in.vf[i]);
	}
	return ret;
}

inline SIMD4f SIMD4f::Min(const SIMD4f& lhs, const SIMD4f& rhs)
{
	SIMD4f ret;
	for (size_t i = 0; i < 4; i++)
	{
		ret.vf[i] = std::fmin(lhs.vf[i], rhs.vf[i]);
	}
	return ret;
}

inline SIMD4f SIMD4f::Max(const SIMD4f& lhs, const SIMD4f& rhs)
{
	SIMD4f ret;
	for (size_t i = 0; i < 4; i++)
	{
		ret.vf[i] = std::fmax(lhs.vf[i], rhs.vf[i]);
	}
	return ret;
}

inline SIMD4f SIMD4f::Floor(const SIMD4f& in)
{
	SIMD4f ret;
	for (size_t i = 0; i < 4; i++)
	{
		ret.vf[i] = std::floor(in.vf[i]);
	}
	return ret;
}

inline SIMD4f SIMD4f::Ceil(const SIMD4f& in)
{
	SIMD4f ret;
	for (size_t i = 0; i < 4; i++)
	{
		ret.vf[i] = std::ceil(in.vf[i]);
	}
	return ret;
}

inline SIMD4f SIMD4f::MulAdd(const SIMD4f& a, const SIMD4f& b, const SIMD4f& c)
{
	SIMD4f ret;
	for (size_t i = 0; i < 4; i++)
	{
		ret.vf[i] = a.vf[i] + b.vf[i] * c.vf[i];
}
	return ret;
}

inline SIMD4f SIMD4f::MulSub(const SIMD4f& a, const SIMD4f& b, const SIMD4f& c)
{
	SIMD4f ret;
	for (size_t i = 0; i < 4; i++)
	{
		ret.vf[i] = a.vf[i] - b.vf[i] * c.vf[i];
}
	return ret;
}

inline SIMD4f SIMD4f::Dot3(const SIMD4f& lhs, const SIMD4f& rhs)
{
	SIMD4f muled = lhs * rhs;
	return SIMD4f{muled.vf[0] + muled.vf[1] + muled.vf[2], 0.0f, 0.0f, 0.0f};
}

inline SIMD4f SIMD4f::Cross3(const SIMD4f& lhs, const SIMD4f& rhs)
{
	return SIMD4f::Swizzle<1,2,0,3>(lhs) * SIMD4f::Swizzle<2,0,1,3>(rhs) -
		SIMD4f::Swizzle<2,0,1,3>(lhs) * SIMD4f::Swizzle<1,2,0,3>(rhs);
}

template<size_t LANE>
SIMD4f SIMD4f::MulLane(const SIMD4f& lhs, const SIMD4f& rhs)
{
	static_assert(LANE < 4, "LANE is must be less than 4.");
	return lhs * rhs.vf[LANE];
}

template<size_t LANE>
SIMD4f SIMD4f::MulAddLane(const SIMD4f& a, const SIMD4f& b, const SIMD4f& c)
{
	static_assert(LANE < 4, "LANE is must be less than 4.");
	return a + b * c.vf[LANE];
}

template<size_t LANE>
SIMD4f SIMD4f::MulSubLane(const SIMD4f& a, const SIMD4f& b, const SIMD4f& c)
{
	static_assert(LANE < 4, "LANE is must be less than 4.");
	return a - b * c.vf[LANE];
}

template <uint32_t indexX, uint32_t indexY, uint32_t indexZ, uint32_t indexW>
SIMD4f SIMD4f::Swizzle(const SIMD4f& in)
{
	static_assert(indexX < 4, "indexX is must be less than 4.");
	static_assert(indexY < 4, "indexY is must be less than 4.");
	static_assert(indexZ < 4, "indexZ is must be less than 4.");
	static_assert(indexW < 4, "indexW is must be less than 4.");
	return SIMD4f{in.vf[indexX], in.vf[indexY], in.vf[indexZ], in.vf[indexW]};
}


template <uint32_t X, uint32_t Y, uint32_t Z, uint32_t W>
SIMD4f SIMD4f::Mask()
{
	static_assert(X >= 2, "indexX is must be set 0 or 1.");
	static_assert(Y >= 2, "indexY is must be set 0 or 1.");
	static_assert(Z >= 2, "indexZ is must be set 0 or 1.");
	static_assert(W >= 2, "indexW is must be set 0 or 1.");
	SIMD4f ret;
	ret.vu[0] = 0xffffffff * X;
	ret.vu[1] = 0xffffffff * Y;
	ret.vu[2] = 0xffffffff * Z;
	ret.vu[3] = 0xffffffff * W;
	return ret;
}

inline uint32_t SIMD4f::MoveMask(const SIMD4f& in)
{
	return (in.vu[0] & 0x1) | (in.vu[1] & 0x2) | (in.vu[2] & 0x4) | (in.vu[3] & 0x8);
}

inline SIMD4f SIMD4f::Select(const SIMD4f& mask, const SIMD4f& sel1, const SIMD4f& sel2)
{
	SIMD4f ret;
	for (size_t i = 0; i < 4; i++)
	{
		ret.vu[i] = (mask.vu[i] & sel1.vu[i]) | (~mask.vu[i] & sel2.vu[i]);
	}
	return ret;
}

inline SIMD4f SIMD4f::Equal(const SIMD4f& lhs, const SIMD4f& rhs)
{
	SIMD4f ret;
	for (size_t i = 0; i < 4; i++)
	{
		ret.vu[i] = (lhs.vf[i] == rhs.vf[i]) ? 0xffffffff : 0;
	}
	return ret;
}

inline SIMD4f SIMD4f::NotEqual(const SIMD4f& lhs, const SIMD4f& rhs)
{
	SIMD4f ret;
	for (size_t i = 0; i < 4; i++)
	{
		ret.vu[i] = (lhs.vf[i] != rhs.vf[i]) ? 0xffffffff : 0;
	}
	return ret;
}

inline SIMD4f SIMD4f::LessThan(const SIMD4f& lhs, const SIMD4f& rhs)
{
	SIMD4f ret;
	for (size_t i = 0; i < 4; i++)
	{
		ret.vu[i] = (lhs.vf[i] < rhs.vf[i]) ? 0xffffffff : 0;
	}
	return ret;
}

inline SIMD4f SIMD4f::LessEqual(const SIMD4f& lhs, const SIMD4f& rhs)
{
	SIMD4f ret;
	for (size_t i = 0; i < 4; i++)
	{
		ret.vu[i] = (lhs.vf[i] <= rhs.vf[i]) ? 0xffffffff : 0;
	}
	return ret;
}

inline SIMD4f SIMD4f::GreaterThan(const SIMD4f& lhs, const SIMD4f& rhs)
{
	SIMD4f ret;
	for (size_t i = 0; i < 4; i++)
	{
		ret.vu[i] = (lhs.vf[i] > rhs.vf[i]) ? 0xffffffff : 0;
	}
	return ret;
}

inline SIMD4f SIMD4f::GreaterEqual(const SIMD4f& lhs, const SIMD4f& rhs)
{
	SIMD4f ret;
	for (size_t i = 0; i < 4; i++)
	{
		ret.vu[i] = (lhs.vf[i] >= rhs.vf[i]) ? 0xffffffff : 0;
	}
	return ret;
}

inline SIMD4f SIMD4f::NearEqual(const SIMD4f& lhs, const SIMD4f& rhs, float epsilon)
{
	SIMD4f ret;
	for (size_t i = 0; i < 4; i++)
	{
		ret.vu[i] = (std::abs(lhs.vf[i] - rhs.vf[i]) <= epsilon) ? 0xffffffff : 0;
	}
	return ret;
}

inline SIMD4f SIMD4f::IsZero(const SIMD4f& in, float epsilon)
{
	SIMD4f ret;
	for (size_t i = 0; i < 4; i++)
	{
		ret.vu[i] = (std::abs(in.vf[i]) <= epsilon) ? 0xffffffff : 0;
	}
	return ret;
}

inline void SIMD4f::Transpose(SIMD4f& s0, SIMD4f& s1, SIMD4f& s2, SIMD4f& s3)
{
	std::swap(s0.vf[1], s1.vf[0]);
	std::swap(s0.vf[2], s2.vf[0]);
	std::swap(s0.vf[3], s3.vf[0]);
	std::swap(s1.vf[2], s2.vf[1]);
	std::swap(s2.vf[3], s3.vf[2]);
	std::swap(s1.vf[3], s3.vf[1]);
}

} // namespace Effekseer

#endif

#endif // __EFFEKSEER_SIMD4F_GEN_H__
#ifndef __EFFEKSEER_SIMD4F_NEON_H__
#define __EFFEKSEER_SIMD4F_NEON_H__


#if defined(EFK_SIMD_NEON)

namespace Effekseer
{

inline float Sqrt(float x)
{
	return sqrt(x);
}

inline float Rsqrt(float x)
{
	return 1.0f / sqrt(x);
}

struct SIMD4i;

/**
 @brief    simd class for sse
 */

struct alignas(16) SIMD4f
{
	float32x4_t s;
	
	SIMD4f() = default;
	SIMD4f(const SIMD4f& rhs) = default;
	SIMD4f(float32x4_t rhs) { s = rhs; }
	SIMD4f(uint32x4_t rhs) { s = vreinterpretq_f32_u32(rhs); }
	SIMD4f(float x, float y, float z, float w) { const float f[4] = {x, y, z, w}; s = vld1q_f32(f); }
	SIMD4f(float i) { s = vdupq_n_f32(i); }
	
	float GetX() const { return vgetq_lane_f32(s, 0); }
	float GetY() const { return vgetq_lane_f32(s, 1); }
	float GetZ() const { return vgetq_lane_f32(s, 2); }
	float GetW() const { return vgetq_lane_f32(s, 3); }
	
	void SetX(float i) { s = vsetq_lane_f32(i, s, 0); }
	void SetY(float i) { s = vsetq_lane_f32(i, s, 1); }
	void SetZ(float i) { s = vsetq_lane_f32(i, s, 2); }
	void SetW(float i) { s = vsetq_lane_f32(i, s, 3); }
	
	template <size_t LANE>
	SIMD4f Dup();
	
	SIMD4i Convert4i() const;
	SIMD4i Cast4i() const;
	
	SIMD4f& operator+=(const SIMD4f& rhs);
	SIMD4f& operator-=(const SIMD4f& rhs);
	SIMD4f& operator*=(const SIMD4f& rhs);
	SIMD4f& operator*=(float rhs);
	SIMD4f& operator/=(const SIMD4f& rhs);
	SIMD4f& operator/=(float rhs);
	
	static SIMD4f Load2(const void* mem);
	static void Store2(void* mem, const SIMD4f& i);
	static SIMD4f Load3(const void* mem);
	static void Store3(void* mem, const SIMD4f& i);
	static SIMD4f Load4(const void* mem);
	static void Store4(void* mem, const SIMD4f& i);
	
	static SIMD4f SetZero();
	static SIMD4f SetInt(int32_t x, int32_t y, int32_t z, int32_t w);
	static SIMD4f SetUInt(uint32_t x, uint32_t y, uint32_t z, uint32_t w);
	static SIMD4f Sqrt(const SIMD4f& in);
	static SIMD4f Rsqrt(const SIMD4f& in);
	static SIMD4f Abs(const SIMD4f& in);
	static SIMD4f Min(const SIMD4f& lhs, const SIMD4f& rhs);
	static SIMD4f Max(const SIMD4f& lhs, const SIMD4f& rhs);
	static SIMD4f Floor(const SIMD4f& in);
	static SIMD4f Ceil(const SIMD4f& in);
	static SIMD4f MulAdd(const SIMD4f& a, const SIMD4f& b, const SIMD4f& c);
	static SIMD4f MulSub(const SIMD4f& a, const SIMD4f& b, const SIMD4f& c);
	
	template<size_t LANE>
	static SIMD4f MulLane(const SIMD4f& lhs, const SIMD4f& rhs);
	template<size_t LANE>
	static SIMD4f MulAddLane(const SIMD4f& a, const SIMD4f& b, const SIMD4f& c);
	template<size_t LANE>
	static SIMD4f MulSubLane(const SIMD4f& a, const SIMD4f& b, const SIMD4f& c);
	template <uint32_t indexX, uint32_t indexY, uint32_t indexZ, uint32_t indexW>
	static SIMD4f Swizzle(const SIMD4f& v);
	
	static SIMD4f Dot3(const SIMD4f& lhs, const SIMD4f& rhs);
	static SIMD4f Cross3(const SIMD4f& lhs, const SIMD4f& rhs);
	
	template <uint32_t X, uint32_t Y, uint32_t Z, uint32_t W>
	static SIMD4f Mask();
	static uint32_t MoveMask(const SIMD4f& in);
	static SIMD4f Select(const SIMD4f& mask, const SIMD4f& sel1, const SIMD4f& sel2);
	static SIMD4f Equal(const SIMD4f& lhs, const SIMD4f& rhs);
	static SIMD4f NotEqual(const SIMD4f& lhs, const SIMD4f& rhs);
	static SIMD4f LessThan(const SIMD4f& lhs, const SIMD4f& rhs);
	static SIMD4f LessEqual(const SIMD4f& lhs, const SIMD4f& rhs);
	static SIMD4f GreaterThan(const SIMD4f& lhs, const SIMD4f& rhs);
	static SIMD4f GreaterEqual(const SIMD4f& lhs, const SIMD4f& rhs);
	static SIMD4f NearEqual(const SIMD4f& lhs, const SIMD4f& rhs, float epsilon = DefaultEpsilon);
	static SIMD4f IsZero(const SIMD4f& in, float epsilon = DefaultEpsilon);
	static void Transpose(SIMD4f& s0, SIMD4f& s1, SIMD4f& s2, SIMD4f& s3);
	
private:
	static SIMD4f SwizzleYZX(const SIMD4f& in);
	static SIMD4f SwizzleZXY(const SIMD4f& in);
};

} // namespace Effekseer

namespace Effekseer
{

template <size_t LANE>
SIMD4f SIMD4f::Dup()
{
	return (LANE < 2) ?
		vdupq_lane_f32(vget_low_f32(s), LANE) :
		vdupq_lane_f32(vget_high_f32(s), LANE & 1);
}

inline SIMD4f operator+(const SIMD4f& lhs, const SIMD4f& rhs)
{
	return vaddq_f32(lhs.s, rhs.s);
}

inline SIMD4f operator-(const SIMD4f& lhs, const SIMD4f& rhs)
{
	return vsubq_f32(lhs.s, rhs.s);
}

inline SIMD4f operator*(const SIMD4f& lhs, const SIMD4f& rhs)
{
	return vmulq_f32(lhs.s, rhs.s);
}

inline SIMD4f operator*(const SIMD4f& lhs, float rhs)
{
	return vmulq_n_f32(lhs.s, rhs);
}

inline SIMD4f operator/(const SIMD4f& lhs, const SIMD4f& rhs)
{
#if defined(_M_ARM64) || __aarch64__
	return vdivq_f32(lhs.s, rhs.s);
#else
	float32x4_t recp = vrecpeq_f32(rhs.s);
	float32x4_t s = vrecpsq_f32(recp, rhs.s);
	recp = vmulq_f32(s, recp);
	s = vrecpsq_f32(recp, rhs.s);
	recp = vmulq_f32(s, recp);
	return vmulq_f32(lhs.s, recp);
#endif
}

inline SIMD4f operator/(const SIMD4f& lhs, float rhs)
{
	return lhs * (1.0f / rhs);
}

inline SIMD4f operator&(const SIMD4f& lhs, const SIMD4f& rhs)
{
	uint32x4_t lhsi = vreinterpretq_u32_f32(lhs.s);
	uint32x4_t rhsi = vreinterpretq_u32_f32(rhs.s);
	return vreinterpretq_f32_u32(vandq_u32(lhsi, rhsi));
}

inline SIMD4f operator|(const SIMD4f& lhs, const SIMD4f& rhs)
{
	uint32x4_t lhsi = vreinterpretq_u32_f32(lhs.s);
	uint32x4_t rhsi = vreinterpretq_u32_f32(rhs.s);
	return vreinterpretq_f32_u32(vorrq_u32(lhsi, rhsi));
}

inline SIMD4f operator^(const SIMD4f& lhs, const SIMD4f& rhs)
{
	uint32x4_t lhsi = vreinterpretq_u32_f32(lhs.s);
	uint32x4_t rhsi = vreinterpretq_u32_f32(rhs.s);
	return vreinterpretq_f32_u32(veorq_u32(lhsi, rhsi));
}

inline bool operator==(const SIMD4f& lhs, const SIMD4f& rhs)
{
	return SIMD4f::MoveMask(SIMD4f::Equal(lhs, rhs)) == 0xf;
}

inline bool operator!=(const SIMD4f& lhs, const SIMD4f& rhs)
{
	return SIMD4f::MoveMask(SIMD4f::Equal(lhs, rhs)) != 0xf;
}

inline SIMD4f& SIMD4f::operator+=(const SIMD4f& rhs) { return *this = *this + rhs; }
inline SIMD4f& SIMD4f::operator-=(const SIMD4f& rhs) { return *this = *this - rhs; }
inline SIMD4f& SIMD4f::operator*=(const SIMD4f& rhs) { return *this = *this * rhs; }
inline SIMD4f& SIMD4f::operator*=(float rhs) { return *this = *this * rhs; }
inline SIMD4f& SIMD4f::operator/=(const SIMD4f& rhs) { return *this = *this / rhs; }
inline SIMD4f& SIMD4f::operator/=(float rhs) { return *this = *this / rhs; }

inline SIMD4f SIMD4f::Load2(const void* mem)
{
	float32x2_t low = vld1_f32((const float*)mem);
	float32x2_t high = vdup_n_f32(0.0f);
	return vcombine_f32(low, high);
}

inline void SIMD4f::Store2(void* mem, const SIMD4f& i)
{
	vst1_f32((float*)mem, vget_low_f32(i.s));
}

inline SIMD4f SIMD4f::Load3(const void* mem)
{
	float32x2_t low = vld1_f32((const float*)mem);
	float32x2_t high = vld1_lane_f32((const float*)mem + 2, vdup_n_f32(0.0f), 0);
	return vcombine_f32(low, high);
}

inline void SIMD4f::Store3(void* mem, const SIMD4f& i)
{
	vst1_f32((float*)mem, vget_low_f32(i.s));
	vst1q_lane_f32((float*)mem + 2, i.s, 2);
}

inline SIMD4f SIMD4f::Load4(const void* mem)
{
	return vld1q_f32((const float*)mem);
}

inline void SIMD4f::Store4(void* mem, const SIMD4f& i)
{
	vst1q_f32((float*)mem, i.s);
}

inline SIMD4f SIMD4f::SetZero()
{
	return vdupq_n_f32(0.0f);
}

inline SIMD4f SIMD4f::SetInt(int32_t x, int32_t y, int32_t z, int32_t w)
{
	const int32_t i[4] = {x, y, z, w};
	return vreinterpretq_f32_s32(vld1q_s32(i));
}

inline SIMD4f SIMD4f::SetUInt(uint32_t x, uint32_t y, uint32_t z, uint32_t w)
{
	const uint32_t i[4] = {x, y, z, w};
	return vreinterpretq_u32_f32(vld1q_u32(i));
}

inline SIMD4f SIMD4f::Sqrt(const SIMD4f& in)
{
#if defined(_M_ARM64) || __aarch64__
	return vsqrtq_f32(in.s);
#else
	return SIMD4f(1.0f) / SIMD4f::Rsqrt(in);
#endif
}

inline SIMD4f SIMD4f::Rsqrt(const SIMD4f& in)
{
	float32x4_t s0 = vrsqrteq_f32(in.s);
	float32x4_t p0 = vmulq_f32(in.s, s0);
	float32x4_t r0 = vrsqrtsq_f32(p0, s0);
	float32x4_t s1 = vmulq_f32(s0, r0);
	return s1;
}

inline SIMD4f SIMD4f::Abs(const SIMD4f& in)
{
	return vabsq_f32(in.s);
}

inline SIMD4f SIMD4f::Min(const SIMD4f& lhs, const SIMD4f& rhs)
{
	return vminq_f32(lhs.s, rhs.s);
}

inline SIMD4f SIMD4f::Max(const SIMD4f& lhs, const SIMD4f& rhs)
{
	return vmaxq_f32(lhs.s, rhs.s);
}

inline SIMD4f SIMD4f::Floor(const SIMD4f& in)
{
#if defined(_M_ARM64) || __aarch64__
	return vrndmq_f32(in.s);
#else
	int32x4_t in_i = vcvtq_s32_f32(in.s);
	float32x4_t result = vcvtq_f32_s32(in_i);
	float32x4_t larger = vcgtq_f32(result, in.s);
	larger = vcvtq_f32_s32(larger);
	return vaddq_f32(result, larger);
#endif
}

inline SIMD4f SIMD4f::Ceil(const SIMD4f& in)
{
#if defined(_M_ARM64) || __aarch64__
	return vrndpq_f32(in.s);
#else
	int32x4_t in_i = vcvtq_s32_f32(in.s);
	float32x4_t result = vcvtq_f32_s32(in_i);
	float32x4_t smaller = vcltq_f32(result, in.s);
	smaller = vcvtq_f32_s32(smaller);
	return vsubq_f32(result, smaller);
#endif
}

inline SIMD4f SIMD4f::MulAdd(const SIMD4f& a, const SIMD4f& b, const SIMD4f& c)
{
	return vmlaq_f32(a.s, b.s, c.s);
}

inline SIMD4f SIMD4f::MulSub(const SIMD4f& a, const SIMD4f& b, const SIMD4f& c)
{
	return vmlsq_f32(a.s, b.s, c.s);
}

template<size_t LANE>
inline SIMD4f SIMD4f::MulLane(const SIMD4f& lhs, const SIMD4f& rhs)
{
	static_assert(LANE < 4, "LANE is must be less than 4.");
	float32x2_t rhs2 = (LANE < 2) ? vget_low_f32(rhs.s) : vget_high_f32(rhs.s);
	return vmulq_lane_f32(lhs.s, rhs2, LANE & 1);
}

template<size_t LANE>
inline SIMD4f SIMD4f::MulAddLane(const SIMD4f& a, const SIMD4f& b, const SIMD4f& c)
{
	static_assert(LANE < 4, "LANE is must be less than 4.");
	float32x2_t c2 = (LANE < 2) ? vget_low_f32(c.s) : vget_high_f32(c.s);
	return vmlaq_lane_f32(a.s, b.s, c2, LANE & 1);
}

template<size_t LANE>
inline SIMD4f SIMD4f::MulSubLane(const SIMD4f& a, const SIMD4f& b, const SIMD4f& c)
{
	static_assert(LANE < 4, "LANE is must be less than 4.");
	float32x2_t c2 = (LANE < 2) ? vget_low_f32(c.s) : vget_high_f32(c.s);
	return vmlsq_lane_f32(a.s, b.s, c2, LANE & 1);
}

//template <uint32_t indexX, uint32_t indexY, uint32_t indexZ, uint32_t indexW>
//inline SIMD4f SIMD4f::Swizzle(const SIMD4f& v)
//{
//	static_assert(indexX < 4, "indexX is must be less than 4.");
//	static_assert(indexY < 4, "indexY is must be less than 4.");
//	static_assert(indexZ < 4, "indexZ is must be less than 4.");
//	static_assert(indexW < 4, "indexW is must be less than 4.");
//}

inline SIMD4f SIMD4f::Dot3(const SIMD4f& lhs, const SIMD4f& rhs)
{
	float32x4_t mul = vmulq_f32(lhs.s, rhs.s);
	float32x2_t xy = vpadd_f32(vget_low_f32(mul), vget_low_f32(mul));
	float32x2_t dot = vadd_f32(xy, vget_high_f32(mul));
	return vcombine_f32(dot, vdup_n_f32(0.0f));
}

inline SIMD4f SIMD4f::Cross3(const SIMD4f& lhs, const SIMD4f& rhs)
{
	return MulSub(SwizzleYZX(lhs.s) * SwizzleZXY(rhs.s), SwizzleZXY(lhs.s), SwizzleYZX(rhs.s));
}

template <uint32_t X, uint32_t Y, uint32_t Z, uint32_t W>
inline SIMD4f SIMD4f::Mask()
{
	static_assert(X >= 2, "indexX is must be set 0 or 1.");
	static_assert(Y >= 2, "indexY is must be set 0 or 1.");
	static_assert(Z >= 2, "indexZ is must be set 0 or 1.");
	static_assert(W >= 2, "indexW is must be set 0 or 1.");
	const uint32_t in[4] = {0xffffffff * X, 0xffffffff * Y, 0xffffffff * Z, 0xffffffff * W};
	return vld1q_f32((const float*)in);
}

inline uint32_t SIMD4f::MoveMask(const SIMD4f& in)
{
	uint16x4_t u16x4 = vmovn_u32(vreinterpretq_u32_f32(in.s));
	uint16_t u16[4];
	vst1_u16(u16, u16x4);
	return (u16[0] & 1) | (u16[1] & 2) | (u16[2] & 4) | (u16[3] & 8);
}

inline SIMD4f SIMD4f::Select(const SIMD4f& mask, const SIMD4f& sel1, const SIMD4f& sel2)
{
	uint32x4_t maski = vreinterpretq_u32_f32(mask.s);
	return vbslq_f32(maski, sel1.s, sel2.s);
}

inline SIMD4f SIMD4f::Equal(const SIMD4f& lhs, const SIMD4f& rhs)
{
	return vceqq_f32(lhs.s, rhs.s);
}

inline SIMD4f SIMD4f::NotEqual(const SIMD4f& lhs, const SIMD4f& rhs)
{
	return vmvnq_u32(vceqq_f32(lhs.s, rhs.s));
}

inline SIMD4f SIMD4f::LessThan(const SIMD4f& lhs, const SIMD4f& rhs)
{
	return vcltq_f32(lhs.s, rhs.s);
}

inline SIMD4f SIMD4f::LessEqual(const SIMD4f& lhs, const SIMD4f& rhs)
{
	return vcleq_f32(lhs.s, rhs.s);
}

inline SIMD4f SIMD4f::GreaterThan(const SIMD4f& lhs, const SIMD4f& rhs)
{
	return vcgtq_f32(lhs.s, rhs.s);
}

inline SIMD4f SIMD4f::GreaterEqual(const SIMD4f& lhs, const SIMD4f& rhs)
{
	return vcgeq_f32(lhs.s, rhs.s);
}

inline SIMD4f SIMD4f::NearEqual(const SIMD4f& lhs, const SIMD4f& rhs, float epsilon)
{
	return LessEqual(Abs(lhs - rhs), SIMD4f(epsilon));
}

inline SIMD4f SIMD4f::IsZero(const SIMD4f& in, float epsilon)
{
	return LessEqual(Abs(in), SIMD4f(epsilon));
}

inline void SIMD4f::Transpose(SIMD4f& s0, SIMD4f& s1, SIMD4f& s2, SIMD4f& s3)
{
	float32x4x2_t t0 = vzipq_f32(s0.s, s2.s);
	float32x4x2_t t1 = vzipq_f32(s1.s, s3.s);
	float32x4x2_t t2 = vzipq_f32(t0.val[0], t1.val[0]);
	float32x4x2_t t3 = vzipq_f32(t0.val[1], t1.val[1]);
	
	s0 = t2.val[0];
	s1 = t2.val[1];
	s2 = t3.val[0];
	s3 = t3.val[1];
}

inline SIMD4f SIMD4f::SwizzleYZX(const SIMD4f& in)
{
	float32x4_t ex = vextq_f32(in.s, in.s, 1);
	return vsetq_lane_f32(vgetq_lane_f32(ex, 3), ex, 2);
}

inline SIMD4f SIMD4f::SwizzleZXY(const SIMD4f& in)
{
	float32x4_t ex = vextq_f32(in.s, in.s, 3);
	return vsetq_lane_f32(vgetq_lane_f32(ex, 3), ex, 0);
}

} // namespace Effekseer

#endif
#endif // __EFFEKSEER_SIMD4F_NEON_H__

#ifndef __EFFEKSEER_SIMD4F_SSE_H__
#define __EFFEKSEER_SIMD4F_SSE_H__


#if defined(EFK_SIMD_SSE2)

namespace Effekseer
{

inline float Sqrt(float x)
{
	_mm_store_ss(&x, _mm_sqrt_ss(_mm_load_ss(&x)));
	return x;
}
inline float Rsqrt(float x)
{
	_mm_store_ss(&x, _mm_rsqrt_ss(_mm_load_ss(&x)));
	return x;
}

struct SIMD4i;

/**
	@brief	simd class for sse
*/

struct alignas(16) SIMD4f
{
	__m128 s;

	SIMD4f() = default;
	SIMD4f(const SIMD4f& rhs) = default;
	SIMD4f(__m128 rhs) { s = rhs; }
	SIMD4f(__m128i rhs) { s = _mm_castsi128_ps(rhs); }
	SIMD4f(float x, float y, float z, float w) { s = _mm_setr_ps(x, y, z, w); }
	SIMD4f(float i) { s = _mm_set_ps1(i); }

	float GetX() const { return _mm_cvtss_f32(s); }
	float GetY() const { return _mm_cvtss_f32(Swizzle<1,1,1,1>(s).s); }
	float GetZ() const { return _mm_cvtss_f32(Swizzle<2,2,2,2>(s).s); }
	float GetW() const { return _mm_cvtss_f32(Swizzle<3,3,3,3>(s).s); }

	void SetX(float i) { s = _mm_move_ss(s, _mm_set_ss(i)); }
	void SetY(float i) { s = Swizzle<1,0,2,3>(_mm_move_ss(Swizzle<1,0,2,3>(s).s, _mm_set_ss(i))).s; }
	void SetZ(float i) { s = Swizzle<2,1,0,3>(_mm_move_ss(Swizzle<2,1,0,3>(s).s, _mm_set_ss(i))).s; }
	void SetW(float i) { s = Swizzle<3,1,2,0>(_mm_move_ss(Swizzle<3,1,2,0>(s).s, _mm_set_ss(i))).s; }

	template <size_t LANE>
	SIMD4f Dup() { return Swizzle<LANE,LANE,LANE,LANE>(s); }

	SIMD4i Convert4i() const;
	SIMD4i Cast4i() const;

	SIMD4f& operator+=(const SIMD4f& rhs);
	SIMD4f& operator-=(const SIMD4f& rhs);
	SIMD4f& operator*=(const SIMD4f& rhs);
	SIMD4f& operator*=(float rhs);
	SIMD4f& operator/=(const SIMD4f& rhs);
	SIMD4f& operator/=(float rhs);

	static SIMD4f Load2(const void* mem);
	static void Store2(void* mem, const SIMD4f& i);
	static SIMD4f Load3(const void* mem);
	static void Store3(void* mem, const SIMD4f& i);
	static SIMD4f Load4(const void* mem);
	static void Store4(void* mem, const SIMD4f& i);

	static SIMD4f SetZero();
	static SIMD4f SetInt(int32_t x, int32_t y, int32_t z, int32_t w);
	static SIMD4f SetUInt(uint32_t x, uint32_t y, uint32_t z, uint32_t w);
	static SIMD4f Sqrt(const SIMD4f& in);
	static SIMD4f Rsqrt(const SIMD4f& in);
	static SIMD4f Abs(const SIMD4f& in);
	static SIMD4f Min(const SIMD4f& lhs, const SIMD4f& rhs);
	static SIMD4f Max(const SIMD4f& lhs, const SIMD4f& rhs);
	static SIMD4f Floor(const SIMD4f& in);
	static SIMD4f Ceil(const SIMD4f& in);
	static SIMD4f MulAdd(const SIMD4f& a, const SIMD4f& b, const SIMD4f& c);
	static SIMD4f MulSub(const SIMD4f& a, const SIMD4f& b, const SIMD4f& c);

	template<size_t LANE>
	static SIMD4f MulLane(const SIMD4f& lhs, const SIMD4f& rhs);
	template<size_t LANE>
	static SIMD4f MulAddLane(const SIMD4f& a, const SIMD4f& b, const SIMD4f& c);
	template<size_t LANE>
	static SIMD4f MulSubLane(const SIMD4f& a, const SIMD4f& b, const SIMD4f& c);
	template <uint32_t indexX, uint32_t indexY, uint32_t indexZ, uint32_t indexW>
	static SIMD4f Swizzle(const SIMD4f& v);

	static SIMD4f Dot3(const SIMD4f& lhs, const SIMD4f& rhs);
	static SIMD4f Cross3(const SIMD4f& lhs, const SIMD4f& rhs);

	template <uint32_t X, uint32_t Y, uint32_t Z, uint32_t W>
	static SIMD4f Mask();
	static uint32_t MoveMask(const SIMD4f& in);
	static SIMD4f Select(const SIMD4f& mask, const SIMD4f& sel1, const SIMD4f& sel2);
	static SIMD4f Equal(const SIMD4f& lhs, const SIMD4f& rhs);
	static SIMD4f NotEqual(const SIMD4f& lhs, const SIMD4f& rhs);
	static SIMD4f LessThan(const SIMD4f& lhs, const SIMD4f& rhs);
	static SIMD4f LessEqual(const SIMD4f& lhs, const SIMD4f& rhs);
	static SIMD4f GreaterThan(const SIMD4f& lhs, const SIMD4f& rhs);
	static SIMD4f GreaterEqual(const SIMD4f& lhs, const SIMD4f& rhs);
	static SIMD4f NearEqual(const SIMD4f& lhs, const SIMD4f& rhs, float epsilon = DefaultEpsilon);
	static SIMD4f IsZero(const SIMD4f& in, float epsilon = DefaultEpsilon);
	static void Transpose(SIMD4f& s0, SIMD4f& s1, SIMD4f& s2, SIMD4f& s3);
};

} // namespace Effekseer

namespace Effekseer
{

inline SIMD4f operator+(const SIMD4f& lhs, const SIMD4f& rhs)
{
	return SIMD4f{_mm_add_ps(lhs.s, rhs.s)};
}

inline SIMD4f operator-(const SIMD4f& lhs, const SIMD4f& rhs)
{
	return SIMD4f{_mm_sub_ps(lhs.s, rhs.s)};
}

inline SIMD4f operator*(const SIMD4f& lhs, const SIMD4f& rhs)
{
	return SIMD4f{_mm_mul_ps(lhs.s, rhs.s)};
}

inline SIMD4f operator*(const SIMD4f& lhs, float rhs)
{
	return SIMD4f{_mm_mul_ps(lhs.s, _mm_set1_ps(rhs))};
}

inline SIMD4f operator/(const SIMD4f& lhs, const SIMD4f& rhs)
{
	return SIMD4f{_mm_div_ps(lhs.s, rhs.s)};
}

inline SIMD4f operator/(const SIMD4f& lhs, float rhs)
{
	return SIMD4f{_mm_div_ps(lhs.s, _mm_set1_ps(rhs))};
}

inline SIMD4f operator&(const SIMD4f& lhs, const SIMD4f& rhs)
{
	return SIMD4f{_mm_and_ps(lhs.s, rhs.s)};
}

inline SIMD4f operator|(const SIMD4f& lhs, const SIMD4f& rhs)
{
	return SIMD4f{_mm_or_ps(lhs.s, rhs.s)};
}

inline SIMD4f operator^(const SIMD4f& lhs, const SIMD4f& rhs)
{
	return SIMD4f{_mm_xor_ps(lhs.s, rhs.s)};
}

inline bool operator==(const SIMD4f& lhs, const SIMD4f& rhs)
{
	return SIMD4f::MoveMask(SIMD4f::Equal(lhs, rhs)) == 0xf;
}

inline bool operator!=(const SIMD4f& lhs, const SIMD4f& rhs)
{
	return SIMD4f::MoveMask(SIMD4f::Equal(lhs, rhs)) != 0xf;
}

inline SIMD4f& SIMD4f::operator+=(const SIMD4f& rhs) { return *this = *this + rhs; }
inline SIMD4f& SIMD4f::operator-=(const SIMD4f& rhs) { return *this = *this - rhs; }
inline SIMD4f& SIMD4f::operator*=(const SIMD4f& rhs) { return *this = *this * rhs; }
inline SIMD4f& SIMD4f::operator*=(float rhs) { return *this = *this * rhs; }
inline SIMD4f& SIMD4f::operator/=(const SIMD4f& rhs) { return *this = *this / rhs; }
inline SIMD4f& SIMD4f::operator/=(float rhs) { return *this = *this / rhs; }

inline SIMD4f SIMD4f::Load2(const void* mem)
{
	__m128 x = _mm_load_ss((const float*)mem + 0);
	__m128 y = _mm_load_ss((const float*)mem + 1);
	return _mm_unpacklo_ps(x, y);
}

inline void SIMD4f::Store2(void* mem, const SIMD4f& i)
{
	SIMD4f t1 = Swizzle<1,1,1,1>(i.s);
	_mm_store_ss((float*)mem + 0, i.s);
	_mm_store_ss((float*)mem + 1, t1.s);
}

inline SIMD4f SIMD4f::Load3(const void* mem)
{
	__m128 x = _mm_load_ss((const float*)mem + 0);
	__m128 y = _mm_load_ss((const float*)mem + 1);
	__m128 z = _mm_load_ss((const float*)mem + 2);
	__m128 xy = _mm_unpacklo_ps(x, y);
	return _mm_movelh_ps(xy, z);
}

inline void SIMD4f::Store3(void* mem, const SIMD4f& i)
{
	SIMD4f t1 = Swizzle<1,1,1,1>(i.s);
	SIMD4f t2 = Swizzle<2,2,2,2>(i.s);
	_mm_store_ss((float*)mem + 0, i.s);
	_mm_store_ss((float*)mem + 1, t1.s);
	_mm_store_ss((float*)mem + 2, t2.s);
}

inline SIMD4f SIMD4f::Load4(const void* mem)
{
	return _mm_loadu_ps((const float*)mem);
}

inline void SIMD4f::Store4(void* mem, const SIMD4f& i)
{
	_mm_storeu_ps((float*)mem, i.s);
}

inline SIMD4f SIMD4f::SetZero()
{
	return _mm_setzero_ps();
}

inline SIMD4f SIMD4f::SetInt(int32_t x, int32_t y, int32_t z, int32_t w)
{
	return SIMD4f{_mm_setr_epi32((int)x, (int)y, (int)z, (int)w)};
}

inline SIMD4f SIMD4f::SetUInt(uint32_t x, uint32_t y, uint32_t z, uint32_t w)
{
	return SIMD4f{_mm_setr_epi32((int)x, (int)y, (int)z, (int)w)};
}

inline SIMD4f SIMD4f::Sqrt(const SIMD4f& in)
{
	return SIMD4f{_mm_sqrt_ps(in.s)};
}

inline SIMD4f SIMD4f::Rsqrt(const SIMD4f& in)
{
	return SIMD4f{_mm_rsqrt_ps(in.s)};
}

inline SIMD4f SIMD4f::Abs(const SIMD4f& in)
{
	return _mm_andnot_ps(_mm_set1_ps(-0.0f), in.s);
}

inline SIMD4f SIMD4f::Min(const SIMD4f& lhs, const SIMD4f& rhs)
{
	return SIMD4f{_mm_min_ps(lhs.s, rhs.s)};
}

inline SIMD4f SIMD4f::Max(const SIMD4f& lhs, const SIMD4f& rhs)
{
	return SIMD4f{_mm_max_ps(lhs.s, rhs.s)};
}

inline SIMD4f SIMD4f::Floor(const SIMD4f& in)
{
#if defined(EFK_SIMD_SSE4_2)
	return _mm_floor_ps(in.s);
#else
	__m128i in_i = _mm_cvttps_epi32(in.s);
	__m128 result = _mm_cvtepi32_ps(in_i);
	__m128 larger = _mm_cmpgt_ps(result, in.s);
	larger = _mm_cvtepi32_ps(_mm_castps_si128(larger));
	return _mm_add_ps(result, larger);
#endif
}

inline SIMD4f SIMD4f::Ceil(const SIMD4f& in)
{
#if defined(EFK_SIMD_SSE4_2)
	return _mm_ceil_ps(in.s);
#else
	__m128i in_i = _mm_cvttps_epi32(in.s);
	__m128 result = _mm_cvtepi32_ps(in_i);
	__m128 smaller = _mm_cmplt_ps(result, in.s);
	smaller = _mm_cvtepi32_ps(_mm_castps_si128(smaller));
	return _mm_sub_ps(result, smaller);
#endif
}

inline SIMD4f SIMD4f::MulAdd(const SIMD4f& a, const SIMD4f& b, const SIMD4f& c)
{
#if defined(EFK_SIMD_AVX2)
	return SIMD4f{_mm_fmadd_ps(b.s, c.s, a.s)};
#else
	return SIMD4f{_mm_add_ps(a.s, _mm_mul_ps(b.s, c.s))};
#endif
}

inline SIMD4f SIMD4f::MulSub(const SIMD4f& a, const SIMD4f& b, const SIMD4f& c)
{
#if defined(EFK_SIMD_AVX2)
	return SIMD4f{_mm_fnmadd_ps(b.s, c.s, a.s)};
#else
	return SIMD4f{_mm_sub_ps(a.s, _mm_mul_ps(b.s, c.s))};
#endif
}

template<size_t LANE>
SIMD4f SIMD4f::MulLane(const SIMD4f& lhs, const SIMD4f& rhs)
{
	static_assert(LANE < 4, "LANE is must be less than 4.");
	return _mm_mul_ps(lhs.s, Swizzle<LANE,LANE,LANE,LANE>(rhs).s);
}

template<size_t LANE>
SIMD4f SIMD4f::MulAddLane(const SIMD4f& a, const SIMD4f& b, const SIMD4f& c)
{
	static_assert(LANE < 4, "LANE is must be less than 4.");
#if defined(EFK_SIMD_AVX2)
	return _mm_fmadd_ps(b.s, Swizzle<LANE,LANE,LANE,LANE>(c).s, a.s);
#else
	return _mm_add_ps(a.s, _mm_mul_ps(b.s, Swizzle<LANE,LANE,LANE,LANE>(c).s));
#endif
}

template<size_t LANE>
SIMD4f SIMD4f::MulSubLane(const SIMD4f& a, const SIMD4f& b, const SIMD4f& c)
{
	static_assert(LANE < 4, "LANE is must be less than 4.");
#if defined(EFK_SIMD_AVX2)
	return _mm_fnmadd_ps(b.s, Swizzle<LANE,LANE,LANE,LANE>(c).s, a.s);
#else
	return _mm_sub_ps(a.s, _mm_mul_ps(b.s, Swizzle<LANE,LANE,LANE,LANE>(c).s));
#endif
}

template <uint32_t indexX, uint32_t indexY, uint32_t indexZ, uint32_t indexW>
SIMD4f SIMD4f::Swizzle(const SIMD4f& v)
{
	static_assert(indexX < 4, "indexX is must be less than 4.");
	static_assert(indexY < 4, "indexY is must be less than 4.");
	static_assert(indexZ < 4, "indexZ is must be less than 4.");
	static_assert(indexW < 4, "indexW is must be less than 4.");

#if defined(EFK_SIMD_AVX)
	return _mm_permute_ps(v.s, _MM_SHUFFLE(indexW, indexZ, indexY, indexX));
#else
	return _mm_shuffle_ps(v.s, v.s, _MM_SHUFFLE(indexW, indexZ, indexY, indexX));
#endif
}

inline SIMD4f SIMD4f::Dot3(const SIMD4f& lhs, const SIMD4f& rhs)
{
	SIMD4f muled = lhs * rhs;
	return _mm_add_ss(_mm_add_ss(muled.s, SIMD4f::Swizzle<1,1,1,1>(muled).s), SIMD4f::Swizzle<2,2,2,2>(muled).s);
}

inline SIMD4f SIMD4f::Cross3(const SIMD4f& lhs, const SIMD4f& rhs)
{
	return SIMD4f::Swizzle<1,2,0,3>(lhs) * SIMD4f::Swizzle<2,0,1,3>(rhs) -
		SIMD4f::Swizzle<2,0,1,3>(lhs) * SIMD4f::Swizzle<1,2,0,3>(rhs);
}

template <uint32_t X, uint32_t Y, uint32_t Z, uint32_t W>
inline SIMD4f SIMD4f::Mask()
{
	static_assert(X >= 2, "indexX is must be set 0 or 1.");
	static_assert(Y >= 2, "indexY is must be set 0 or 1.");
	static_assert(Z >= 2, "indexZ is must be set 0 or 1.");
	static_assert(W >= 2, "indexW is must be set 0 or 1.");
	return _mm_setr_epi32(
		(int)(0xffffffff * X), 
		(int)(0xffffffff * Y), 
		(int)(0xffffffff * Z), 
		(int)(0xffffffff * W));
}

inline uint32_t SIMD4f::MoveMask(const SIMD4f& in)
{
	return (uint32_t)_mm_movemask_ps(in.s);
}

inline SIMD4f SIMD4f::Select(const SIMD4f& mask, const SIMD4f& sel1, const SIMD4f& sel2)
{
	return _mm_or_ps(_mm_and_ps(mask.s, sel1.s), _mm_andnot_ps(mask.s, sel2.s));
}

inline SIMD4f SIMD4f::Equal(const SIMD4f& lhs, const SIMD4f& rhs)
{
	return SIMD4f{_mm_cmpeq_ps(lhs.s, rhs.s)};
}

inline SIMD4f SIMD4f::NotEqual(const SIMD4f& lhs, const SIMD4f& rhs)
{
	return SIMD4f{_mm_cmpneq_ps(lhs.s, rhs.s)};
}

inline SIMD4f SIMD4f::LessThan(const SIMD4f& lhs, const SIMD4f& rhs)
{
	return SIMD4f{_mm_cmplt_ps(lhs.s, rhs.s)};
}

inline SIMD4f SIMD4f::LessEqual(const SIMD4f& lhs, const SIMD4f& rhs)
{
	return SIMD4f{_mm_cmple_ps(lhs.s, rhs.s)};
}

inline SIMD4f SIMD4f::GreaterThan(const SIMD4f& lhs, const SIMD4f& rhs)
{
	return SIMD4f{_mm_cmpgt_ps(lhs.s, rhs.s)};
}

inline SIMD4f SIMD4f::GreaterEqual(const SIMD4f& lhs, const SIMD4f& rhs)
{
	return SIMD4f{_mm_cmpge_ps(lhs.s, rhs.s)};
}

inline SIMD4f SIMD4f::NearEqual(const SIMD4f& lhs, const SIMD4f& rhs, float epsilon)
{
	return LessEqual(Abs(lhs - rhs), SIMD4f(epsilon));
}

inline SIMD4f SIMD4f::IsZero(const SIMD4f& in, float epsilon)
{
	return LessEqual(Abs(in), SIMD4f(epsilon));
}

inline void SIMD4f::Transpose(SIMD4f& s0, SIMD4f& s1, SIMD4f& s2, SIMD4f& s3)
{
	_MM_TRANSPOSE4_PS(s0.s, s1.s, s2.s, s3.s);
}

} // namespace Effekseer

#endif

#endif // __EFFEKSEER_SIMD4F_SSE_H__

#ifndef __EFFEKSEER_SIMD4BRIDGE_GEN_H__
#define __EFFEKSEER_SIMD4BRIDGE_GEN_H__


#if defined(EFK_SIMD_GEN)

namespace Effekseer
{

inline SIMD4i SIMD4f::Convert4i() const { return SIMD4i((int32_t)vf[0], (int32_t)vf[1], (int32_t)vf[2], (int32_t)vf[3]); }

inline SIMD4i SIMD4f::Cast4i() const { return SIMD4i(vu[0], vu[1], vu[2], vu[3]); }

inline SIMD4f SIMD4i::Convert4f() const { return SIMD4f((float)vi[0], (float)vi[1], (float)vi[2], (float)vi[3]); }

inline SIMD4f SIMD4i::Cast4f() const { return SIMD4f(vf[0], vf[1], vf[2], vf[3]); }

} // namespace Effekseer

#endif

#endif // __EFFEKSEER_SIMD4F_GEN_H__
#ifndef __EFFEKSEER_SIMD4BRIDGE_NEON_H__
#define __EFFEKSEER_SIMD4BRIDGE_NEON_H__


#if defined(EFK_SIMD_NEON)

namespace Effekseer
{

inline SIMD4i SIMD4f::Convert4i() const { return vcvtq_s32_f32(s); }

inline SIMD4i SIMD4f::Cast4i() const { return vreinterpretq_s32_f32(s); }

inline SIMD4f SIMD4i::Convert4f() const { return vcvtq_f32_s32(s); }

inline SIMD4f SIMD4i::Cast4f() const { return vreinterpretq_f32_s32(s); }

} // namespace Effekseer

#endif
#endif // __EFFEKSEER_SIMD4F_NEON_H__

#ifndef __EFFEKSEER_SIMD4BRIDGE_SSE_H__
#define __EFFEKSEER_SIMD4BRIDGE_SSE_H__


#if defined(EFK_SIMD_SSE2)

namespace Effekseer
{

inline SIMD4i SIMD4f::Convert4i() const { return _mm_cvtps_epi32(s); }

inline SIMD4i SIMD4f::Cast4i() const { return _mm_castps_si128(s); }

inline SIMD4f SIMD4i::Convert4f() const { return _mm_cvtepi32_ps(s); }

inline SIMD4f SIMD4i::Cast4f() const { return _mm_castsi128_ps(s); }

} // namespace Effekseer

#endif

#endif // __EFFEKSEER_SIMD4F_SSE_H__


#ifndef __EFFEKSEER_VEC2F_H__
#define __EFFEKSEER_VEC2F_H__


namespace Effekseer
{

struct Vector2D;
struct vector2d;

struct Vec2f
{
	SIMD4f s;

	explicit Vec2f() = default;
	Vec2f(const Vec2f& vec) = default;
	Vec2f(float x, float y): s(x, y, 0.0f, 1.0f) {}
	Vec2f(const SIMD4f& vec): s(vec) {}
	Vec2f(const Vector2D& vec);
	Vec2f(const vector2d& vec);

	float GetX() const { return s.GetX(); }
	float GetY() const { return s.GetY(); }

	void SetX(float o) { s.SetX(o); }
	void SetY(float o) { s.SetY(o); }

	Vec2f& operator+=(const Vec2f& o) { s += o.s; return *this; }
	Vec2f& operator-=(const Vec2f& o) { s -= o.s; return *this; }
	Vec2f& operator*=(const Vec2f& o) { s *= o.s; return *this; }
	Vec2f& operator*=(float o) { s *= o; return *this; }
	Vec2f& operator/=(const Vec2f& o) { s /= o.s; return *this; }
	Vec2f& operator/=(float o) { s /= o; return *this; }

	float LengthSq() const;
	float Length() const;
	bool IsZero(float range = DefaultEpsilon) const;
	Vec2f Normalize() const;

	static Vec2f Load(const void* mem);
	static void Store(void* mem, const Vec2f& i);

	static Vec2f Sqrt(const Vec2f& i);
	static Vec2f Rsqrt(const Vec2f& i);
	static Vec2f Abs(const Vec2f& i);
	static Vec2f Min(const Vec2f& lhs, const Vec2f& rhs);
	static Vec2f Max(const Vec2f& lhs, const Vec2f& rhs);
	static bool Equal(const Vec2f& lhs, const Vec2f& rhs, float epsilon);
};

inline Vec2f operator+(const Vec2f& lhs, const Vec2f& rhs)
{
	return Vec2f{lhs.s + rhs.s};
}

inline Vec2f operator-(const Vec2f& lhs, const Vec2f& rhs)
{
	return Vec2f{lhs.s - rhs.s};
}

inline Vec2f operator*(const Vec2f& lhs, const Vec2f& rhs)
{
	return Vec2f{lhs.s * rhs.s};
}

inline Vec2f operator*(const Vec2f& lhs, float rhs)
{
	return Vec2f{lhs.s * rhs};
}

inline Vec2f operator/(const Vec2f& lhs, const Vec2f& rhs)
{
	return Vec2f{lhs.s / rhs.s};
}

inline Vec2f operator/(const Vec2f& lhs, float rhs)
{
	return Vec2f{lhs.s / rhs};
}

inline bool operator==(const Vec2f& lhs, const Vec2f& rhs)
{
	return (SIMD4f::MoveMask(SIMD4f::Equal(lhs.s, rhs.s)) & 0x03) == 0x3;
}

inline bool operator!=(const Vec2f& lhs, const Vec2f& rhs)
{
	return (SIMD4f::MoveMask(SIMD4f::Equal(lhs.s, rhs.s)) & 0x03) != 0x3;
}

inline Vec2f Vec2f::Load(const void* mem)
{
	return SIMD4f::Load2(mem);
}

inline void Vec2f::Store(void* mem, const Vec2f& i)
{
	SIMD4f::Store2(mem, i.s);
}

inline Vec2f Vec2f::Sqrt(const Vec2f& i)
{
	return Vec2f{SIMD4f::Sqrt(i.s)};
}

inline Vec2f Vec2f::Rsqrt(const Vec2f& i)
{
	return Vec2f{SIMD4f::Rsqrt(i.s)};
}

inline Vec2f Vec2f::Abs(const Vec2f& i)
{
	return Vec2f{SIMD4f::Abs(i.s)};
}

inline Vec2f Vec2f::Min(const Vec2f& lhs, const Vec2f& rhs)
{
	return Vec2f{SIMD4f::Min(lhs.s, rhs.s)};
}

inline Vec2f Vec2f::Max(const Vec2f& lhs, const Vec2f& rhs)
{
	return Vec2f{SIMD4f::Max(lhs.s, rhs.s)};
}

inline bool Vec2f::Equal(const Vec2f& lhs, const Vec2f& rhs, float epsilon)
{
	return (SIMD4f::MoveMask(SIMD4f::NearEqual(lhs.s, rhs.s, epsilon)) & 0x3) == 0x3;
}

inline float Vec2f::LengthSq() const
{
	auto o = s * s;
	return o.GetX() + o.GetY();
}

inline float Vec2f::Length() const
{
	return Effekseer::Sqrt(LengthSq());
}

inline bool Vec2f::IsZero(float range) const
{
	return LengthSq() < range * range;
}

inline Vec2f Vec2f::Normalize() const
{
	return *this * Effekseer::Rsqrt(LengthSq());
}

} // namespace Effekseer

#endif // __EFFEKSEER_VEC2F_H__

#ifndef __EFFEKSEER_VEC3F_H__
#define __EFFEKSEER_VEC3F_H__


namespace Effekseer
{

struct Vector3D;
struct vector3d;
struct Mat43f;
struct Mat44f;

struct Vec3f
{
	SIMD4f s;

	explicit Vec3f() = default;
	Vec3f(const Vec3f& vec) = default;
	Vec3f(float x, float y, float z): s(x, y, z, 1.0f) {}
	Vec3f(const SIMD4f& vec): s(vec) {}
	Vec3f(const Vector3D& vec);
	Vec3f(const vector3d& vec);

	float GetX() const { return s.GetX(); }
	float GetY() const { return s.GetY(); }
	float GetZ() const { return s.GetZ(); }

	void SetX(float o) { s.SetX(o); }
	void SetY(float o) { s.SetY(o); }
	void SetZ(float o) { s.SetZ(o); }

	Vec3f& operator+=(const Vec3f& o) { s += o.s; return *this; }
	Vec3f& operator-=(const Vec3f& o) { s -= o.s; return *this; }
	Vec3f& operator*=(const Vec3f& o) { s *= o.s; return *this; }
	Vec3f& operator*=(float o) { s *= o; return *this; }
	Vec3f& operator/=(const Vec3f& o) { s /= o.s; return *this; }
	Vec3f& operator/=(float o) { s /= o; return *this; }

	float GetSquaredLength() const;
	float GetLength() const;
	bool IsZero(float epsiron = DefaultEpsilon) const;
	Vec3f Normalize() const;
	Vec3f NormalizePrecisely() const;
	Vec3f NormalizeFast() const;

	static Vec3f Load(const void* mem);
	static void Store(void* mem, const Vec3f& i);

	static Vec3f Sqrt(const Vec3f& i);
	static Vec3f Rsqrt(const Vec3f& i);
	static Vec3f Abs(const Vec3f& i);
	static Vec3f Min(const Vec3f& lhs, const Vec3f& rhs);
	static Vec3f Max(const Vec3f& lhs, const Vec3f& rhs);
	static float Dot(const Vec3f& lhs, const Vec3f& rhs);
	static Vec3f Cross(const Vec3f& lhs, const Vec3f& rhs);
	static bool Equal(const Vec3f& lhs, const Vec3f& rhs, float epsilon = DefaultEpsilon);
	static Vec3f Transform(const Vec3f& lhs, const Mat43f& rhs);
	static Vec3f Transform(const Vec3f& lhs, const Mat44f& rhs);
};

inline Vec3f operator-(const Vec3f& i) { return Vec3f(-i.GetX(), -i.GetY(), -i.GetZ()); }

inline Vec3f operator+(const Vec3f& lhs, const Vec3f& rhs)
{
	return Vec3f{lhs.s + rhs.s};
}

inline Vec3f operator-(const Vec3f& lhs, const Vec3f& rhs)
{
	return Vec3f{lhs.s - rhs.s};
}

inline Vec3f operator*(const Vec3f& lhs, const Vec3f& rhs)
{
	return Vec3f{lhs.s * rhs.s};
}

inline Vec3f operator*(const Vec3f& lhs, float rhs)
{
	return Vec3f{lhs.s * rhs};
}

inline Vec3f operator/(const Vec3f& lhs, const Vec3f& rhs)
{
	return Vec3f{lhs.s / rhs.s};
}

inline Vec3f operator/(const Vec3f& lhs, float rhs)
{
	return Vec3f{lhs.s / rhs};
}

inline bool operator==(const Vec3f& lhs, const Vec3f& rhs)
{
	return (SIMD4f::MoveMask(SIMD4f::Equal(lhs.s, rhs.s)) & 0x07) == 0x7;
}

inline bool operator!=(const Vec3f& lhs, const Vec3f& rhs)
{
	return (SIMD4f::MoveMask(SIMD4f::Equal(lhs.s, rhs.s)) & 0x07) != 0x7;
}

inline Vec3f Vec3f::Load(const void* mem)
{
	return SIMD4f::Load3(mem);
}

inline void Vec3f::Store(void* mem, const Vec3f& i)
{
	SIMD4f::Store3(mem, i.s);
}

inline Vec3f Vec3f::Sqrt(const Vec3f& i)
{
	return Vec3f{SIMD4f::Sqrt(i.s)};
}

inline Vec3f Vec3f::Rsqrt(const Vec3f& i)
{
	return Vec3f{SIMD4f::Rsqrt(i.s)};
}

inline Vec3f Vec3f::Abs(const Vec3f& i)
{
	return Vec3f{SIMD4f::Abs(i.s)};
}

inline Vec3f Vec3f::Min(const Vec3f& lhs, const Vec3f& rhs)
{
	return Vec3f{SIMD4f::Min(lhs.s, rhs.s)};
}

inline Vec3f Vec3f::Max(const Vec3f& lhs, const Vec3f& rhs)
{
	return Vec3f{SIMD4f::Max(lhs.s, rhs.s)};
}

inline float Vec3f::Dot(const Vec3f& lhs, const Vec3f& rhs)
{
	return SIMD4f::Dot3(lhs.s, rhs.s).GetX();
}

inline Vec3f Vec3f::Cross(const Vec3f& lhs, const Vec3f& rhs)
{
	return SIMD4f::Cross3(lhs.s, rhs.s);
}

inline bool Vec3f::Equal(const Vec3f& lhs, const Vec3f& rhs, float epsilon)
{
	return (SIMD4f::MoveMask(SIMD4f::NearEqual(lhs.s, rhs.s, epsilon)) & 0x7) == 0x7;
}

inline float Vec3f::GetSquaredLength() const
{
	auto o = s * s;
	return o.GetX() + o.GetY() + o.GetZ();
}

inline float Vec3f::GetLength() const
{
	return Effekseer::Sqrt(GetSquaredLength());
}

inline bool Vec3f::IsZero(float epsiron) const
{
	return (SIMD4f::MoveMask(SIMD4f::IsZero(s, epsiron)) & 0x7) == 0x7;
}

inline Vec3f Vec3f::Normalize() const
{
	return *this * Effekseer::Rsqrt(GetSquaredLength());
}

inline Vec3f Vec3f::NormalizePrecisely() const { return *this / Effekseer::Sqrt(GetSquaredLength()); }

inline Vec3f Vec3f::NormalizeFast() const { return *this * Effekseer::Rsqrt(GetSquaredLength()); }

} // namespace Effekseer

#endif // __EFFEKSEER_VEC3F_H__

#ifndef __EFFEKSEER_VEC4F_H__
#define __EFFEKSEER_VEC4F_H__


namespace Effekseer
{

struct Vec4f
{
	SIMD4f s;

	Vec4f() = default;
	Vec4f(const Vec4f& vec) = default;
	Vec4f(const SIMD4f& vec): s(vec) {}

	float GetX() const { return s.GetX(); }
	float GetY() const { return s.GetY(); }
	float GetZ() const { return s.GetZ(); }
	float GetW() const { return s.GetW(); }

	void SetX(float o) { s.SetX(o); }
	void SetY(float o) { s.SetY(o); }
	void SetZ(float o) { s.SetZ(o); }
	void SetW(float o) { s.SetW(o); }

	Vec4f& operator+=(const Vec4f& o)
	{
		this->s = this->s + o.s;
		return *this;
	}

	Vec4f& operator-=(const Vec4f& o)
	{
		this->s = this->s - o.s;
		return *this;
	}

	Vec4f& operator*=(const Vec4f& o)
	{
		this->s = this->s * o.s;
		return *this;
	}

	Vec4f& operator/=(const Vec4f& o)
	{
		this->s = this->s / o.s;
		return *this;
	}

	static Vec4f Sqrt(const Vec4f& i);
	static Vec4f Rsqrt(const Vec4f& i);
	static Vec4f Abs(const Vec4f& i);
	static Vec4f Min(const Vec4f& lhs, const Vec4f& rhs);
	static Vec4f Max(const Vec4f& lhs, const Vec4f& rhs);
	static bool Equal(const Vec4f& lhs, const Vec4f& rhs, float epsilon);
	static Vec4f Transform(const Vec4f& lhs, const Mat43f& rhs);
	static Vec4f Transform(const Vec4f& lhs, const Mat44f& rhs);
};

inline Vec4f operator+(const Vec4f& lhs, const Vec4f& rhs) { return Vec4f{lhs.s + rhs.s}; }

inline Vec4f operator-(const Vec4f& lhs, const Vec4f& rhs) { return Vec4f{lhs.s - rhs.s}; }

inline Vec4f operator*(const Vec4f& lhs, const Vec4f& rhs) { return Vec4f{lhs.s * rhs.s}; }

inline Vec4f operator/(const Vec4f& lhs, const Vec4f& rhs) { return Vec4f{lhs.s / rhs.s}; }

inline bool operator==(const Vec4f& lhs, const Vec4f& rhs)
{
	return SIMD4f::MoveMask(SIMD4f::Equal(lhs.s, rhs.s)) == 0xf;
}

inline bool operator!=(const Vec4f& lhs, const Vec4f& rhs)
{
	return SIMD4f::MoveMask(SIMD4f::Equal(lhs.s, rhs.s)) != 0xf;
}

inline Vec4f Vec4f::Sqrt(const Vec4f& i)
{
	return Vec4f{SIMD4f::Sqrt(i.s)};
}

inline Vec4f Vec4f::Rsqrt(const Vec4f& i)
{
	return Vec4f{SIMD4f::Rsqrt(i.s)};
}

inline Vec4f Vec4f::Abs(const Vec4f& i)
{
	return Vec4f{SIMD4f::Abs(i.s)};
}

inline Vec4f Vec4f::Min(const Vec4f& lhs, const Vec4f& rhs)
{
	return Vec4f{SIMD4f::Min(lhs.s, rhs.s)};
}

inline Vec4f Vec4f::Max(const Vec4f& lhs, const Vec4f& rhs)
{
	return Vec4f{SIMD4f::Max(lhs.s, rhs.s)};
}

inline bool Vec4f::Equal(const Vec4f& lhs, const Vec4f& rhs, float epsilon)
{
	return (SIMD4f::MoveMask(SIMD4f::NearEqual(lhs.s, rhs.s, epsilon)) & 0xf) == 0xf;
}

} // namespace Effekseer

#endif // __EFFEKSEER_VEC4F_H__

#ifndef __EFFEKSEER_MAT43F_H__
#define __EFFEKSEER_MAT43F_H__


namespace Effekseer
{

struct Matrix43;

struct Mat43f
{
	SIMD4f X;
	SIMD4f Y;
	SIMD4f Z;

	Mat43f() = default;
	Mat43f(const Mat43f& rhs) = default;
	Mat43f(float m11, float m12, float m13,
		   float m21, float m22, float m23,
		   float m31, float m32, float m33,
		   float m41, float m42, float m43);
	Mat43f(const Matrix43& mat);

	bool IsValid() const;

	Vec3f GetScale() const;

	Mat43f GetRotation() const;

	Vec3f GetTranslation() const;

	void GetSRT(Vec3f& s, Mat43f& r, Vec3f& t) const;

	void SetTranslation(const Vec3f& t);

	Mat43f& operator*=(const Mat43f& rhs);

	Mat43f& operator*=(float rhs);

	static const Mat43f Identity;

	static bool Equal(const Mat43f& lhs, const Mat43f& rhs, float epsilon = DefaultEpsilon);

	static Mat43f SRT(const Vec3f& s, const Mat43f& r, const Vec3f& t);

	static Mat43f Scaling(float x, float y, float z);

	static Mat43f Scaling(const Vec3f& scale);

	static Mat43f RotationX(float angle);

	static Mat43f RotationY(float angle);

	static Mat43f RotationZ(float angle);

	static Mat43f RotationXYZ(float rx, float ry, float rz);

	static Mat43f RotationZXY(float rz, float rx, float ry);

	static Mat43f RotationAxis(const Vec3f& axis, float angle);

	static Mat43f RotationAxis(const Vec3f& axis, float s, float c);

	static Mat43f Translation(float x, float y, float z);

	static Mat43f Translation(const Vec3f& pos);
};

inline Mat43f::Mat43f(
	float m11, float m12, float m13,
	float m21, float m22, float m23,
	float m31, float m32, float m33,
	float m41, float m42, float m43)
	: X(m11, m21, m31, m41)
	, Y(m12, m22, m32, m42)
	, Z(m13, m23, m33, m43)
{
}

inline bool operator==(const Mat43f& lhs, const Mat43f& rhs)
{
	return lhs.X == rhs.X && lhs.Y == rhs.Y && lhs.Z == rhs.Z;
}

inline bool operator!=(const Mat43f& lhs, const Mat43f& rhs)
{
	return lhs.X != rhs.X && lhs.Y != rhs.Y && lhs.Z != rhs.Z;
}

inline Mat43f operator*(const Mat43f& lhs, const Mat43f& rhs)
{
	const SIMD4f mask = SIMD4f::SetUInt(0, 0, 0, 0xffffffff);

	Mat43f res;
	res.X = mask & rhs.X;
	res.X = SIMD4f::MulAddLane<0>(res.X, lhs.X, rhs.X);
	res.X = SIMD4f::MulAddLane<1>(res.X, lhs.Y, rhs.X);
	res.X = SIMD4f::MulAddLane<2>(res.X, lhs.Z, rhs.X);

	res.Y = mask & rhs.Y;
	res.Y = SIMD4f::MulAddLane<0>(res.Y, lhs.X, rhs.Y);
	res.Y = SIMD4f::MulAddLane<1>(res.Y, lhs.Y, rhs.Y);
	res.Y = SIMD4f::MulAddLane<2>(res.Y, lhs.Z, rhs.Y);

	res.Z = mask & rhs.Z;
	res.Z = SIMD4f::MulAddLane<0>(res.Z, lhs.X, rhs.Z);
	res.Z = SIMD4f::MulAddLane<1>(res.Z, lhs.Y, rhs.Z);
	res.Z = SIMD4f::MulAddLane<2>(res.Z, lhs.Z, rhs.Z);
	return res;
}

inline Vec3f Vec3f::Transform(const Vec3f& lhs, const Mat43f& rhs)
{
	SIMD4f s0 = rhs.X;
	SIMD4f s1 = rhs.Y;
	SIMD4f s2 = rhs.Z;
	SIMD4f s3 = SIMD4f::SetZero();
	SIMD4f::Transpose(s0, s1, s2, s3);

	SIMD4f res = SIMD4f::MulAddLane<0>(s3, s0, lhs.s);
	res = SIMD4f::MulAddLane<1>(res, s1, lhs.s);
	res = SIMD4f::MulAddLane<2>(res, s2, lhs.s);
	return Vec3f{res};
}

inline Vec4f Vec4f::Transform(const Vec4f& lhs, const Mat43f& rhs)
{
	SIMD4f s0 = rhs.X;
	SIMD4f s1 = rhs.Y;
	SIMD4f s2 = rhs.Z;
	SIMD4f s3 = SIMD4f(0.0f, 0.0f, 0.0f, 1.0f);
	SIMD4f::Transpose(s0, s1, s2, s3);

	SIMD4f res = SIMD4f::MulLane<0>(s0, lhs.s);
	res = SIMD4f::MulAddLane<1>(res, s1, lhs.s);
	res = SIMD4f::MulAddLane<2>(res, s2, lhs.s);
	res = SIMD4f::MulAddLane<3>(res, s3, lhs.s);
	return res;
}

inline Mat43f& Mat43f::operator*=(const Mat43f& rhs)
{
	*this = *this * rhs;
	return *this;
}

inline Mat43f& Mat43f::operator*=(float rhs)
{
	X *= rhs;
	Y *= rhs;
	Z *= rhs;
	return *this;
}

} // namespace Effekseer

#endif // __EFFEKSEER_MAT43F_H__

#ifndef __EFFEKSEER_MAT44F_H__
#define __EFFEKSEER_MAT44F_H__


namespace Effekseer
{

struct Matrix44;

struct Mat44f
{
	SIMD4f X;
	SIMD4f Y;
	SIMD4f Z;
	SIMD4f W;
	
	Mat44f() = default;
	Mat44f(const Mat44f& rhs) = default;
	Mat44f(float m11, float m12, float m13, float m14,
		   float m21, float m22, float m23, float m24,
		   float m31, float m32, float m33, float m34,
		   float m41, float m42, float m43, float m44);
	Mat44f(const Mat43f& mat);
	Mat44f(const Matrix44& mat);

	bool IsValid() const;

	Vec3f GetScale() const;

	Mat44f GetRotation() const;

	Vec3f GetTranslation() const;

	void GetSRT(Vec3f& s, Mat44f& r, Vec3f& t) const;

	void SetTranslation(const Vec3f& t);

	Mat44f Transpose() const;

	Mat44f& operator*=(const Mat44f& rhs);
	
	Mat44f& operator*=(float rhs);

	static const Mat44f Identity;

	static bool Equal(const Mat44f& lhs, const Mat44f& rhs, float epsilon = DefaultEpsilon);

	static Mat44f SRT(const Vec3f& s, const Mat44f& r, const Vec3f& t);

	static Mat44f Scaling(float x, float y, float z);

	static Mat44f Scaling(const Vec3f& scale);

	static Mat44f RotationX(float angle);

	static Mat44f RotationY(float angle);

	static Mat44f RotationZ(float angle);

	static Mat44f RotationXYZ(float rx, float ry, float rz);

	static Mat44f RotationZXY(float rz, float rx, float ry);

	static Mat44f RotationAxis(const Vec3f& axis, float angle);

	static Mat44f RotationAxis(const Vec3f& axis, float s, float c);

	static Mat44f Translation(float x, float y, float z);

	static Mat44f Translation(const Vec3f& pos);
};

inline Mat44f::Mat44f(
	float m11, float m12, float m13, float m14,
	float m21, float m22, float m23, float m24,
	float m31, float m32, float m33, float m34,
	float m41, float m42, float m43, float m44)
	: X(m11, m21, m31, m41)
	, Y(m12, m22, m32, m42)
	, Z(m13, m23, m33, m43)
	, W(m14, m24, m34, m44)
{
}

inline Mat44f::Mat44f(const Mat43f& mat)
	: X(mat.X)
	, Y(mat.Y)
	, Z(mat.Z)
	, W(0.0f, 0.0f, 0.0f, 1.0f)
{
}

inline bool operator==(const Mat44f& lhs, const Mat44f& rhs)
{
	return lhs.X == rhs.X && lhs.Y == rhs.Y && lhs.Z == rhs.Z && lhs.W == rhs.W;
}

inline bool operator!=(const Mat44f& lhs, const Mat44f& rhs)
{
	return lhs.X != rhs.X && lhs.Y != rhs.Y && lhs.Z != rhs.Z && lhs.W != rhs.W;
}

inline Mat44f operator*(const Mat44f& lhs, const Mat44f& rhs)
{
	Mat44f res;
	res.X = SIMD4f::MulLane<0>(lhs.X, rhs.X);
	res.X = SIMD4f::MulAddLane<1>(res.X, lhs.Y, rhs.X);
	res.X = SIMD4f::MulAddLane<2>(res.X, lhs.Z, rhs.X);
	res.X = SIMD4f::MulAddLane<3>(res.X, lhs.W, rhs.X);

	res.Y = SIMD4f::MulLane<0>(lhs.X, rhs.Y);
	res.Y = SIMD4f::MulAddLane<1>(res.Y, lhs.Y, rhs.Y);
	res.Y = SIMD4f::MulAddLane<2>(res.Y, lhs.Z, rhs.Y);
	res.Y = SIMD4f::MulAddLane<3>(res.Y, lhs.W, rhs.Y);

	res.Z = SIMD4f::MulLane<0>(lhs.X, rhs.Z);
	res.Z = SIMD4f::MulAddLane<1>(res.Z, lhs.Y, rhs.Z);
	res.Z = SIMD4f::MulAddLane<2>(res.Z, lhs.Z, rhs.Z);
	res.Z = SIMD4f::MulAddLane<3>(res.Z, lhs.W, rhs.Z);

	res.W = SIMD4f::MulLane<0>(lhs.X, rhs.W);
	res.W = SIMD4f::MulAddLane<1>(res.W, lhs.Y, rhs.W);
	res.W = SIMD4f::MulAddLane<2>(res.W, lhs.Z, rhs.W);
	res.W = SIMD4f::MulAddLane<3>(res.W, lhs.W, rhs.W);
	return res;
}

inline Vec3f Vec3f::Transform(const Vec3f& lhs, const Mat44f& rhs)
{
	SIMD4f s0 = rhs.X;
	SIMD4f s1 = rhs.Y;
	SIMD4f s2 = rhs.Z;
	SIMD4f s3 = rhs.W;
	SIMD4f::Transpose(s0, s1, s2, s3);

	SIMD4f res = SIMD4f::MulAddLane<0>(s3, s0, lhs.s);
	res = SIMD4f::MulAddLane<1>(res, s1, lhs.s);
	res = SIMD4f::MulAddLane<2>(res, s2, lhs.s);
	return Vec3f{res};
}

inline Vec4f Vec4f::Transform(const Vec4f& lhs, const Mat44f& rhs)
{
	SIMD4f s0 = rhs.X;
	SIMD4f s1 = rhs.Y;
	SIMD4f s2 = rhs.Z;
	SIMD4f s3 = rhs.W;
	SIMD4f::Transpose(s0, s1, s2, s3);

	SIMD4f res = SIMD4f::MulLane<0>(s0, lhs.s);
	res = SIMD4f::MulAddLane<1>(res, s1, lhs.s);
	res = SIMD4f::MulAddLane<2>(res, s2, lhs.s);
	res = SIMD4f::MulAddLane<3>(res, s3, lhs.s);
	return res;
}

inline Mat44f& Mat44f::operator*=(const Mat44f& rhs)
{
	*this = *this * rhs;
	return *this;
}

inline Mat44f& Mat44f::operator*=(float rhs)
{
	X *= rhs;
	Y *= rhs;
	Z *= rhs;
	W *= rhs;
	return *this;
}

} // namespace Effekseer

#endif // __EFFEKSEER_VEC4F_H__

#ifndef __EFFEKSEER_SIMD_UTILS_H__
#define __EFFEKSEER_SIMD_UTILS_H__

#include <stdlib.h>

namespace Effekseer
{
	
template <size_t align>
class AlignedAllocationPolicy {
public:
	static void* operator new(size_t size) {
#if defined(__EMSCRIPTEN__) && __EMSCRIPTEN_minor__ < 38
		return malloc(size);
#elif defined(_MSC_VER)
		return _mm_malloc(size, align);
#else
		void *ptr = nullptr;
		posix_memalign(&ptr, align, size);
		return ptr;
#endif
	}
	static void operator delete(void* ptr) {
#if defined(__EMSCRIPTEN__) && __EMSCRIPTEN_minor__ < 38
		free(ptr);
#elif defined(_MSC_VER)
		_mm_free(ptr);
#else
		return free(ptr);
#endif
	}
};

inline Vector2D ToStruct(const Vec2f& o)
{
	Vector2D ret;
	Vec2f::Store(&ret, o);
	return ret;
}

inline Vector3D ToStruct(const Vec3f& o)
{
	Vector3D ret;
	Vec3f::Store(&ret, o);
	return ret;
}

inline Matrix43 ToStruct(const Mat43f& o)
{
	SIMD4f tx = o.X;
	SIMD4f ty = o.Y;
	SIMD4f tz = o.Z;
	SIMD4f tw = SIMD4f::SetZero();
	SIMD4f::Transpose(tx, ty, tz, tw);

	Matrix43 ret;
	SIMD4f::Store3(ret.Value[0], tx);
	SIMD4f::Store3(ret.Value[1], ty);
	SIMD4f::Store3(ret.Value[2], tz);
	SIMD4f::Store3(ret.Value[3], tw);
	return ret;
}

inline Matrix44 ToStruct(const Mat44f& o)
{
	SIMD4f tx = o.X;
	SIMD4f ty = o.Y;
	SIMD4f tz = o.Z;
	SIMD4f tw = o.W;
	SIMD4f::Transpose(tx, ty, tz, tw);

	Matrix44 ret;
	SIMD4f::Store4(ret.Values[0], tx);
	SIMD4f::Store4(ret.Values[1], ty);
	SIMD4f::Store4(ret.Values[2], tz);
	SIMD4f::Store4(ret.Values[3], tw);
	return ret;
}

} // namespace Effekseer

#endif

#ifndef __EFFEKSEER_EFK_EFC_LOADER_H__
#define __EFFEKSEER_EFK_EFC_LOADER_H__

#include <string>
#include <vector>

namespace Effekseer
{

/**
	@brief a factory for effect efc format
*/
class EfkEfcFactory : public EffectFactory
{
private:
public:
	bool OnLoading(Effect* effect, const void* data, int32_t size, float magnification, const EFK_CHAR* materialPath) override;

	bool OnCheckIsBinarySupported(const void* data, int32_t size) override;
};

/**
	@brief	a loader to load properties from efc format
*/
class EfkEfcProperty
{
private:
	std::vector<std::u16string> colorImages_;
	std::vector<std::u16string> normalImages_;
	std::vector<std::u16string> distortionImages_;
	std::vector<std::u16string> sounds_;
	std::vector<std::u16string> models_;
	std::vector<std::u16string> materials_;

public:
	bool Load(const void* data, int32_t size);

	const std::vector<std::u16string>& GetColorImages() const;
	const std::vector<std::u16string>& GetNormalImages() const;
	const std::vector<std::u16string>& GetDistortionImages() const;
	const std::vector<std::u16string>& GetSounds() const;
	const std::vector<std::u16string>& GetModels() const;
	const std::vector<std::u16string>& GetMaterials() const;
};

} // namespace Effekseer

#endif

#ifndef __EFFEKSEER_MATERIAL_H__
#define __EFFEKSEER_MATERIAL_H__

#include <array>
#include <assert.h>
#include <map>
#include <sstream>
#include <string.h>
#include <vector>

namespace Effekseer
{

class Material
{
private:
	const int32_t customDataMinCount_ = 2;

	struct Texture
	{
		std::string Name;
		int32_t Index;
		TextureWrapType Wrap;
	};

	struct Uniform
	{
		std::string Name;
		int32_t Index;
	};

	uint64_t guid_ = 0;

	std::string genericCode_;

	bool hasRefraction_ = false;

	bool isSimpleVertex_ = false;

	ShadingModelType shadingModel_;

	int32_t customData1Count_ = 0;
	int32_t customData2Count_ = 0;

	std::vector<Texture> textures_;

	std::vector<Uniform> uniforms_;

public:
	Material() = default;
	virtual ~Material() = default;

	virtual bool Load(const uint8_t* data, int32_t size);

	virtual ShadingModelType GetShadingModel() const;

	virtual void SetShadingModel(ShadingModelType shadingModel);

	virtual bool GetIsSimpleVertex() const;

	virtual void SetIsSimpleVertex(bool isSimpleVertex);

	virtual bool GetHasRefraction() const;

	virtual void SetHasRefraction(bool hasRefraction);

	virtual const char* GetGenericCode() const;

	virtual void SetGenericCode(const char* code);

	virtual uint64_t GetGUID() const;

	virtual void SetGUID(uint64_t guid);

	virtual TextureWrapType GetTextureWrap(int32_t index) const;

	virtual void SetTextureWrap(int32_t index, TextureWrapType value);

	virtual int32_t GetTextureIndex(int32_t index) const;

	virtual void SetTextureIndex(int32_t index, int32_t value);

	virtual const char* GetTextureName(int32_t index) const;

	virtual void SetTextureName(int32_t index, const char* name);

	virtual int32_t GetTextureCount() const;

	virtual void SetTextureCount(int32_t count);

	virtual int32_t GetUniformIndex(int32_t index) const;

	virtual void SetUniformIndex(int32_t index, int32_t value);

	virtual const char* GetUniformName(int32_t index) const;

	virtual void SetUniformName(int32_t index, const char* name);

	virtual int32_t GetUniformCount() const;

	virtual void SetUniformCount(int32_t count);

	virtual int32_t GetCustomData1Count() const;

	virtual void SetCustomData1Count(int32_t count);

	virtual int32_t GetCustomData2Count() const;

	virtual void SetCustomData2Count(int32_t count);
};

} // namespace Effekseer

#endif // __EFFEKSEER_MATERIAL_H__
#ifndef __EFFEKSEER_MATERIAL_COMPILER_H__
#define __EFFEKSEER_MATERIAL_COMPILER_H__

#include <map>
#include <stdint.h>
#include <stdio.h>
#include <vector>

namespace Effekseer
{

enum class MaterialShaderType : int32_t
{
	Standard,
	Model,
	Refraction,
	RefractionModel,
	Max,
};

class CompiledMaterialBinary : public IReference
{
private:
public:
	CompiledMaterialBinary() = default;
	virtual ~CompiledMaterialBinary() = default;

	virtual const uint8_t* GetVertexShaderData(MaterialShaderType type) const = 0;

	virtual int32_t GetVertexShaderSize(MaterialShaderType type) const = 0;

	virtual const uint8_t* GetPixelShaderData(MaterialShaderType type) const = 0;

	virtual int32_t GetPixelShaderSize(MaterialShaderType type) const = 0;
};

class MaterialCompiler : public IReference
{

public:
	MaterialCompiler() = default;

	virtual ~MaterialCompiler() = default;

	/**
	 * @bbrief compile and store data into the cache
	 */
	virtual CompiledMaterialBinary* Compile(Material* material) = 0;
};

} // namespace Effekseer

#endif


#ifndef __EFFEKSEER_COMPILED_MATERIAL_H__
#define __EFFEKSEER_COMPILED_MATERIAL_H__

#include <array>
#include <assert.h>
#include <map>
#include <sstream>
#include <string.h>
#include <vector>

namespace Effekseer
{

enum class CompiledMaterialPlatformType : int32_t
{
	DirectX9 = 0,
	// DirectX10 = 1,
	DirectX11 = 2,
	DirectX12 = 3,
	OpenGL = 10,
	Metal = 20,
	Vulkan = 30,
	PS4 = 40,
	Switch = 50,
	XBoxOne = 60,
};

class CompiledMaterial
{
	static const int32_t Version = 1;

	std::map<CompiledMaterialPlatformType, std::unique_ptr<CompiledMaterialBinary, ReferenceDeleter<CompiledMaterialBinary>>> platforms;
	std::vector<uint8_t> originalData_;

public:
	uint64_t GUID = 0;

	const std::vector<uint8_t>& GetOriginalData() const;

	bool Load(const uint8_t* data, int32_t size);

	void Save(std::vector<uint8_t>& dst, uint64_t guid, std::vector<uint8_t>& originalData);

	bool GetHasValue(CompiledMaterialPlatformType type) const;

	CompiledMaterialBinary* GetBinary(CompiledMaterialPlatformType type) const;

	void UpdateData(const std::vector<uint8_t>& standardVS,
					const std::vector<uint8_t>& standardPS,
					const std::vector<uint8_t>& modelVS,
					const std::vector<uint8_t>& modelPS,
					const std::vector<uint8_t>& standardRefractionVS,
					const std::vector<uint8_t>& standardRefractionPS,
					const std::vector<uint8_t>& modelRefractionVS,
					const std::vector<uint8_t>& modelRefractionPS,
					CompiledMaterialPlatformType type);
};

} // namespace Effekseer

#endif

#ifndef __EFFEKSEER_SPRITE_RENDERER_H__
#define __EFFEKSEER_SPRITE_RENDERER_H__

//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace Effekseer
{
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------

class SpriteRenderer
{
public:
	struct NodeParameter
	{
		Effect* EffectPointer;
		//int32_t				ColorTextureIndex;
		//AlphaBlendType			AlphaBlend;
		//TextureFilterType	TextureFilter;
		//TextureWrapType	TextureWrap;
		bool ZTest;
		bool ZWrite;
		BillboardType Billboard;
		bool IsRightHand;

		//bool				Distortion;
		//float				DistortionIntensity;

		//float				DepthOffset;
		//bool				IsDepthOffsetScaledWithCamera;
		//bool				IsDepthOffsetScaledWithParticleScale;

		ZSortType ZSort;

		NodeRendererDepthParameter* DepthParameterPtr = nullptr;
		NodeRendererBasicParameter* BasicParameterPtr = nullptr;

		//RendererMaterialType MaterialType = RendererMaterialType::Default;
		//MaterialParameter* MaterialParameterPtr = nullptr;
	};

	struct InstanceParameter
	{
		Mat43f SRTMatrix43;
		Color AllColor;

		// Lower left, Lower right, Upper left, Upper right
		Color Colors[4];

		Vec2f Positions[4];

		RectF UV;

#ifdef __EFFEKSEER_BUILD_VERSION16__
		RectF AlphaUV;

		float FlipbookIndexAndNextRate;

		float AlphaThreshold;
#endif

		std::array<float, 4> CustomData1;
		std::array<float, 4> CustomData2;
	};

public:
	SpriteRenderer()
	{
	}

	virtual ~SpriteRenderer()
	{
	}

	virtual void BeginRendering(const NodeParameter& parameter, int32_t count, void* userData)
	{
	}

	virtual void Rendering(const NodeParameter& parameter, const InstanceParameter& instanceParameter, void* userData)
	{
	}

	virtual void EndRendering(const NodeParameter& parameter, void* userData)
	{
	}
};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
} // namespace Effekseer
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#endif // __EFFEKSEER_SPRITE_RENDERER_H__


#ifndef __EFFEKSEER_RIBBON_RENDERER_H__
#define __EFFEKSEER_RIBBON_RENDERER_H__

//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace Effekseer
{

struct NodeRendererTextureUVTypeParameter;

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------

class RibbonRenderer
{
public:
	struct NodeParameter
	{
		Effect* EffectPointer;
		//int32_t				ColorTextureIndex;
		//AlphaBlendType			AlphaBlend;
		//TextureFilterType	TextureFilter;
		//TextureWrapType	TextureWrap;
		bool ZTest;
		bool ZWrite;
		bool ViewpointDependent;

		//bool				Distortion;
		//float				DistortionIntensity;

		bool IsRightHand;
		int32_t SplineDivision;
		NodeRendererDepthParameter* DepthParameterPtr = nullptr;
		NodeRendererBasicParameter* BasicParameterPtr = nullptr;
		NodeRendererTextureUVTypeParameter* TextureUVTypeParameterPtr = nullptr;
		//RendererMaterialType MaterialType = RendererMaterialType::Default;
		//MaterialParameter* MaterialParameterPtr = nullptr;
	};

	struct InstanceParameter
	{
		int32_t InstanceCount;
		int32_t InstanceIndex;
		Mat43f SRTMatrix43;
		Color AllColor;

		// Lower left, Lower right, Upper left, Upper right
		Color Colors[4];

		float Positions[4];

		RectF UV;
#ifdef __EFFEKSEER_BUILD_VERSION16__
		RectF AlphaUV;

		float FlipbookIndexAndNextRate;

		float AlphaThreshold;
#endif
		std::array<float, 4> CustomData1;
		std::array<float, 4> CustomData2;
	};

public:
	RibbonRenderer()
	{
	}

	virtual ~RibbonRenderer()
	{
	}

	virtual void BeginRendering(const NodeParameter& parameter, int32_t count, void* userData)
	{
	}

	virtual void Rendering(const NodeParameter& parameter, const InstanceParameter& instanceParameter, void* userData)
	{
	}

	virtual void EndRendering(const NodeParameter& parameter, void* userData)
	{
	}

	virtual void BeginRenderingGroup(const NodeParameter& parameter, int32_t count, void* userData)
	{
	}

	virtual void EndRenderingGroup(const NodeParameter& parameter, int32_t count, void* userData)
	{
	}
};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
} // namespace Effekseer
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#endif // __EFFEKSEER_RIBBON_RENDERER_H__

#ifndef __EFFEKSEER_RING_RENDERER_H__
#define __EFFEKSEER_RING_RENDERER_H__

//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace Effekseer
{
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------

class RingRenderer
{
public:
	struct NodeParameter
	{
		Effect* EffectPointer;
		//int32_t				ColorTextureIndex;
		//AlphaBlendType			AlphaBlend;
		//TextureFilterType	TextureFilter;
		//TextureWrapType	TextureWrap;
		bool ZTest;
		bool ZWrite;
		BillboardType Billboard;
		int32_t VertexCount;
		bool IsRightHand;
		float StartingFade = 0.0f;
		float EndingFade = 0.0f;
		//bool				Distortion;
		//float				DistortionIntensity;

		NodeRendererDepthParameter* DepthParameterPtr = nullptr;
		NodeRendererBasicParameter* BasicParameterPtr = nullptr;

		//RendererMaterialType MaterialType = RendererMaterialType::Default;
		//MaterialParameter* MaterialParameterPtr = nullptr;

		//float				DepthOffset;
		//bool				IsDepthOffsetScaledWithCamera;
		//bool				IsDepthOffsetScaledWithParticleScale;

		NodeRendererBasicParameter BasicParameter;
	};

	struct InstanceParameter
	{
		Mat43f SRTMatrix43;
		Vec2f OuterLocation;
		Vec2f InnerLocation;
		float ViewingAngleStart;
		float ViewingAngleEnd;
		float CenterRatio;
		Color OuterColor;
		Color CenterColor;
		Color InnerColor;

		RectF UV;
#ifdef __EFFEKSEER_BUILD_VERSION16__
		RectF AlphaUV;

		float FlipbookIndexAndNextRate;

		float AlphaThreshold;
#endif
		std::array<float, 4> CustomData1;
		std::array<float, 4> CustomData2;
	};

public:
	RingRenderer()
	{
	}

	virtual ~RingRenderer()
	{
	}

	virtual void BeginRendering(const NodeParameter& parameter, int32_t count, void* userData)
	{
	}

	virtual void Rendering(const NodeParameter& parameter, const InstanceParameter& instanceParameter, void* userData)
	{
	}

	virtual void EndRendering(const NodeParameter& parameter, void* userData)
	{
	}
};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
} // namespace Effekseer
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#endif // __EFFEKSEER_RING_RENDERER_H__


#ifndef __EFFEKSEER_MODEL_RENDERER_H__
#define __EFFEKSEER_MODEL_RENDERER_H__

//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace Effekseer
{
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------

class ModelRenderer
{
public:
	struct NodeParameter
	{
		Effect* EffectPointer;
		//AlphaBlendType		AlphaBlend;
		//TextureFilterType	TextureFilter;
		//TextureWrapType	TextureWrap;
		bool ZTest;
		bool ZWrite;
		BillboardType Billboard;

		//bool				Lighting;
		CullingType Culling;
		int32_t ModelIndex;
		//int32_t				ColorTextureIndex;
		//int32_t				NormalTextureIndex;
		float Magnification;
		bool IsRightHand;

		//bool				Distortion;
		//float				DistortionIntensity;

		NodeRendererDepthParameter* DepthParameterPtr = nullptr;
		NodeRendererBasicParameter* BasicParameterPtr = nullptr;

		//RendererMaterialType MaterialType = RendererMaterialType::Default;
		//MaterialParameter* MaterialParameterPtr = nullptr;

		//float				DepthOffset;
		//bool				IsDepthOffsetScaledWithCamera;
		//bool				IsDepthOffsetScaledWithParticleScale;
	};

	struct InstanceParameter
	{
		Mat43f SRTMatrix43;
		RectF UV;
#ifdef __EFFEKSEER_BUILD_VERSION16__
		RectF AlphaUV;

		float FlipbookIndexAndNextRate;

		float AlphaThreshold;
#endif
		Color AllColor;
		int32_t Time;
		std::array<float, 4> CustomData1;
		std::array<float, 4> CustomData2;
	};

public:
	ModelRenderer()
	{
	}

	virtual ~ModelRenderer()
	{
	}

	virtual void BeginRendering(const NodeParameter& parameter, int32_t count, void* userData)
	{
	}

	virtual void Rendering(const NodeParameter& parameter, const InstanceParameter& instanceParameter, void* userData)
	{
	}

	virtual void EndRendering(const NodeParameter& parameter, void* userData)
	{
	}
};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
} // namespace Effekseer
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#endif // __EFFEKSEER_MODEL_RENDERER_H__


#ifndef __EFFEKSEER_TRACK_RENDERER_H__
#define __EFFEKSEER_TRACK_RENDERER_H__

//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace Effekseer
{

struct NodeRendererTextureUVTypeParameter;

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------

class TrackRenderer
{
public:
	struct NodeParameter
	{
		Effect* EffectPointer;
		//int32_t				ColorTextureIndex;
		//AlphaBlendType			AlphaBlend;
		//TextureFilterType	TextureFilter;
		//TextureWrapType		TextureWrap;
		bool ZTest;
		bool ZWrite;

		//bool				Distortion;
		//float				DistortionIntensity;

		int32_t SplineDivision;

		bool IsRightHand;
		NodeRendererDepthParameter* DepthParameterPtr = nullptr;
		NodeRendererBasicParameter* BasicParameterPtr = nullptr;
		NodeRendererTextureUVTypeParameter* TextureUVTypeParameterPtr = nullptr;

		RendererMaterialType MaterialType = RendererMaterialType::Default;
		MaterialParameter* MaterialParameterPtr = nullptr;
	};

	struct InstanceGroupParameter
	{
	};

	struct InstanceParameter
	{
		int32_t InstanceCount;
		int32_t InstanceIndex;
		Mat43f SRTMatrix43;

		Color ColorLeft;
		Color ColorCenter;
		Color ColorRight;

		Color ColorLeftMiddle;
		Color ColorCenterMiddle;
		Color ColorRightMiddle;

		float SizeFor;
		float SizeMiddle;
		float SizeBack;

		RectF UV;
#ifdef __EFFEKSEER_BUILD_VERSION16__
		RectF AlphaUV;

		float FlipbookIndexAndNextRate;

		float AlphaThreshold;
#endif
		std::array<float, 4> CustomData1;
		std::array<float, 4> CustomData2;
	};

public:
	TrackRenderer()
	{
	}

	virtual ~TrackRenderer()
	{
	}

	virtual void BeginRendering(const NodeParameter& parameter, int32_t count, void* userData)
	{
	}

	virtual void Rendering(const NodeParameter& parameter, const InstanceParameter& instanceParameter, void* userData)
	{
	}

	virtual void EndRendering(const NodeParameter& parameter, void* userData)
	{
	}

	virtual void BeginRenderingGroup(const NodeParameter& parameter, int32_t count, void* userData)
	{
	}

	virtual void EndRenderingGroup(const NodeParameter& parameter, int32_t count, void* userData)
	{
	}
};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
} // namespace Effekseer
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#endif // __EFFEKSEER_TRACK_RENDERER_H__

#ifndef __EFFEKSEER_EFFECTLOADER_H__
#define __EFFEKSEER_EFFECTLOADER_H__

//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace Effekseer
{
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
/**
	@brief	エフェクトファイル読み込み破棄関数指定クラス
*/
class EffectLoader
{
public:
	/**
		@brief	コンストラクタ
	*/
	EffectLoader()
	{
	}

	/**
		@brief	デストラクタ
	*/
	virtual ~EffectLoader()
	{
	}

	/**
		@brief	エフェクトファイルを読み込む。
		@param	path	[in]	読み込み元パス
		@param	data	[out]	データ配列の先頭のポインタを出力する先
		@param	size	[out]	データ配列の長さを出力する先
		@return	成否
		@note
		エフェクトファイルを読み込む。
		::Effekseer::Effect::Create実行時に使用される。
	*/
	virtual bool Load(const EFK_CHAR* path, void*& data, int32_t& size) = 0;

	/**
		@brief	エフェクトファイルを破棄する。
		@param	data	[in]	データ配列の先頭のポインタ
		@param	size	[int]	データ配列の長さ
		@note
		エフェクトファイルを破棄する。
		::Effekseer::Effect::Create実行終了時に使用される。
	*/
	virtual void Unload(void* data, int32_t size) = 0;
};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
} // namespace Effekseer
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#endif // __EFFEKSEER_EFFECTLOADER_H__


#ifndef __EFFEKSEER_TEXTURELOADER_H__
#define __EFFEKSEER_TEXTURELOADER_H__

//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace Effekseer
{
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
/**
	@brief	テクスチャ読み込み破棄関数指定クラス
*/
class TextureLoader
{
public:
	/**
		@brief	コンストラクタ
	*/
	TextureLoader()
	{
	}

	/**
		@brief	デストラクタ
	*/
	virtual ~TextureLoader()
	{
	}

	/**
		@brief	テクスチャを読み込む。
		@param	path	[in]	読み込み元パス
		@param	textureType	[in]	テクスチャの種類
		@return	テクスチャのポインタ
		@note
		テクスチャを読み込む。
		::Effekseer::Effect::Create実行時に使用される。
	*/
	virtual TextureData* Load(const EFK_CHAR* path, TextureType textureType)
	{
		return nullptr;
	}

	/**
		@brief
		\~English	a function called when texture is loaded
		\~Japanese	テクスチャが読み込まれるときに呼ばれる関数
		@param	data
		\~English	data pointer
		\~Japanese	データのポインタ
		@param	size
		\~English	the size of data
		\~Japanese	データの大きさ
		@param	textureType
		\~English	a kind of texture
		\~Japanese	テクスチャの種類
		@return
		\~English	a pointer of loaded texture
		\~Japanese	読み込まれたテクスチャのポインタ
	*/
	virtual TextureData* Load(const void* data, int32_t size, TextureType textureType)
	{
		return nullptr;
	}

	/**
		@brief	テクスチャを破棄する。
		@param	data	[in]	テクスチャ
		@note
		テクスチャを破棄する。
		::Effekseer::Effectのインスタンスが破棄された時に使用される。
	*/
	virtual void Unload(TextureData* data)
	{
	}
};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
} // namespace Effekseer
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#endif // __EFFEKSEER_TEXTURELOADER_H__


#ifndef __EFFEKSEER_MODELLOADER_H__
#define __EFFEKSEER_MODELLOADER_H__

//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace Effekseer
{
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
/**
	@brief	モデル読み込み破棄関数指定クラス
*/
class ModelLoader
{
public:
	/**
		@brief	コンストラクタ
	*/
	ModelLoader()
	{
	}

	/**
		@brief	デストラクタ
	*/
	virtual ~ModelLoader()
	{
	}

	/**
		@brief	モデルを読み込む。
		@param	path	[in]	読み込み元パス
		@return	モデルのポインタ
		@note
		モデルを読み込む。
		::Effekseer::Effect::Create実行時に使用される。
	*/
	virtual void* Load(const EFK_CHAR* path)
	{
		return NULL;
	}

	/**
		@brief
		\~English	a function called when model is loaded
		\~Japanese	モデルが読み込まれるときに呼ばれる関数
		@param	data
		\~English	data pointer
		\~Japanese	データのポインタ
		@param	size
		\~English	the size of data
		\~Japanese	データの大きさ
		@return
		\~English	a pointer of loaded texture
		\~Japanese	読み込まれたモデルのポインタ
	*/
	virtual void* Load(const void* data, int32_t size)
	{
		return nullptr;
	}

	/**
		@brief	モデルを破棄する。
		@param	data	[in]	モデル
		@note
		モデルを破棄する。
		::Effekseer::Effectのインスタンスが破棄された時に使用される。
	*/
	virtual void Unload(void* data)
	{
	}
};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
} // namespace Effekseer
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#endif // __EFFEKSEER_MODELLOADER_H__


#ifndef __EFFEKSEER_MATERIALLOADER_H__
#define __EFFEKSEER_MATERIALLOADER_H__


namespace Effekseer
{

/**
	@brief	
	\~English	Material loader
	\~Japanese	マテリアル読み込み破棄関数指定クラス
*/
class MaterialLoader
{
public:
	/**
	@brief	
	\~English	Constructor
	\~Japanese	コンストラクタ
	*/
	MaterialLoader() = default;

	/**
	@brief
	\~English	Destructor
	\~Japanese	デストラクタ
	*/
	virtual ~MaterialLoader() = default;

	/**
		@brief
		\~English	load a material
		\~Japanese	マテリアルを読み込む。
		@param	path	
		\~English	a file path
		\~Japanese	読み込み元パス
		@return
		\~English	a pointer of loaded a material
		\~Japanese	読み込まれたマテリアルのポインタ
	*/
	virtual MaterialData* Load(const EFK_CHAR* path)
	{
		return nullptr;
	}

	/**
		@brief
		\~English	a function called when a material is loaded
		\~Japanese	マテリアルが読み込まれるときに呼ばれる関数
		@param	data
		\~English	data pointer
		\~Japanese	データのポインタ
		@param	size
		\~English	the size of data
		\~Japanese	データの大きさ
		@param	fileType
		\~English	file type
		\~Japanese	ファイルの種類
		@return
		\~English	a pointer of loaded a material
		\~Japanese	読み込まれたマテリアルのポインタ
	*/
	virtual MaterialData* Load(const void* data, int32_t size, MaterialFileType fileType)
	{
		return nullptr;
	}

	/**
		@brief
		\~English	dispose a material
		\~Japanese	マテリアルを破棄する。
		@param	data
		\~English	a pointer of loaded a material
		\~Japanese	読み込まれたマテリアルのポインタ
	*/
	virtual void Unload(MaterialData* data)
	{
	}
};

} // namespace Effekseer

#endif // __EFFEKSEER_TEXTURELOADER_H__


#ifndef __EFFEKSEER_MODEL_H__
#define __EFFEKSEER_MODEL_H__

//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace Effekseer
{
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
/**
	@brief
	\~English	Model class
	\~Japanese	モデルクラス
*/
class Model
{
public:
	static const int32_t Version = 1;

	struct Vertex
	{
		Vector3D Position;
		Vector3D Normal;
		Vector3D Binormal;
		Vector3D Tangent;
		Vector2D UV;
		Color VColor;
	};

	struct VertexWithIndex
	{
		Vector3D Position;
		Vector3D Normal;
		Vector3D Binormal;
		Vector3D Tangent;
		Vector2D UV;
		Color VColor;
		uint8_t Index[4];
	};

	struct Face
	{
		int32_t Indexes[3];
	};

	struct Emitter
	{
		Vector3D Position;
		Vector3D Normal;
		Vector3D Binormal;
		Vector3D Tangent;
	};

private:
	uint8_t* m_data;
	int32_t m_size;

	int32_t m_version;

	struct InternalModel
	{
		int32_t m_vertexCount;
		Vertex* m_vertexes;

		int32_t m_faceCount;
		Face* m_faces;
	};

	InternalModel* models;

	int32_t m_modelCount;
	int32_t m_frameCount;

protected:
	int32_t m_vertexSize = sizeof(Vertex);

public:
	/**
	@brief
	\~English	Constructor
	\~Japanese	コンストラクタ
	*/
	Model(void* data, int32_t size)
		: m_data(NULL)
		, m_size(size)
		, m_version(0)
		, models(nullptr)
	{
		m_data = new uint8_t[m_size];
		memcpy(m_data, data, m_size);

		uint8_t* p = (uint8_t*)m_data;

		memcpy(&m_version, p, sizeof(int32_t));
		p += sizeof(int32_t);

		// load scale except version 3(for compatibility)
		if (m_version == 2 || m_version >= 5)
		{
			// Scale
			p += sizeof(int32_t);
		}

		memcpy(&m_modelCount, p, sizeof(int32_t));
		p += sizeof(int32_t);

		if (m_version >= 5)
		{
			memcpy(&m_frameCount, p, sizeof(int32_t));
			p += sizeof(int32_t);
		}
		else
		{
			m_frameCount = 1;
		}

		models = new InternalModel[m_frameCount];

		for (int32_t f = 0; f < m_frameCount; f++)
		{
			memcpy(&models[f].m_vertexCount, p, sizeof(int32_t));
			p += sizeof(int32_t);

			if (m_version >= 1)
			{
				models[f].m_vertexes = (Vertex*)p;
				p += (sizeof(Vertex) * models[f].m_vertexCount);
			}
			else
			{
				// allocate new buffer
				models[f].m_vertexes = new Vertex[models[f].m_vertexCount];

				for (int32_t i = 0; i < models[f].m_vertexCount; i++)
				{
					memcpy((void*)&models[f].m_vertexes[i], p, sizeof(Vertex) - sizeof(Color));
					models[f].m_vertexes[i].VColor = Color(255, 255, 255, 255);

					p += sizeof(Vertex) - sizeof(Color);
				}
			}

			memcpy(&models[f].m_faceCount, p, sizeof(int32_t));
			p += sizeof(int32_t);

			models[f].m_faces = (Face*)p;
			p += (sizeof(Face) * models[f].m_faceCount);
		}
	}

	Vertex* GetVertexes(int32_t index = 0) const
	{
		return models[index].m_vertexes;
	}
	int32_t GetVertexCount(int32_t index = 0)
	{
		return models[index].m_vertexCount;
	}

	Face* GetFaces(int32_t index = 0) const
	{
		return models[index].m_faces;
	}
	int32_t GetFaceCount(int32_t index = 0)
	{
		return models[index].m_faceCount;
	}

	int32_t GetFrameCount() const
	{
		return m_frameCount;
	}

	int32_t GetModelCount()
	{
		return m_modelCount;
	}

	int32_t GetVertexSize() const
	{
		return m_vertexSize;
	}

	/**
		@brief
		\~English	Destructor
		\~Japanese	デストラクタ
	*/
	virtual ~Model()
	{
		if (m_version == 0)
		{
			ES_SAFE_DELETE_ARRAY(models[0].m_vertexes);
		}

		ES_SAFE_DELETE_ARRAY(models);
		ES_SAFE_DELETE_ARRAY(m_data);
	}

	Emitter GetEmitter(IRandObject* g, int32_t time, CoordinateSystem coordinate, float magnification)
	{
		time = time % GetFrameCount();

		int32_t faceInd = (int32_t)((GetFaceCount(time) - 1) * (g->GetRand()));
		faceInd = Clamp(faceInd, GetFaceCount(time) - 1, 0);
		Face& face = GetFaces(time)[faceInd];
		Vertex& v0 = GetVertexes(time)[face.Indexes[0]];
		Vertex& v1 = GetVertexes(time)[face.Indexes[1]];
		Vertex& v2 = GetVertexes(time)[face.Indexes[2]];

		float p1 = g->GetRand();
		float p2 = g->GetRand();

		// Fit within plane
		if (p1 + p2 > 1.0f)
		{
			p1 = 1.0f - p1;
			p2 = 1.0f - p2;
		}

		float p0 = 1.0f - p1 - p2;

		Emitter emitter;
		emitter.Position = (v0.Position * p0 + v1.Position * p1 + v2.Position * p2) * magnification;
		emitter.Normal = v0.Normal * p0 + v1.Normal * p1 + v2.Normal * p2;
		emitter.Binormal = v0.Binormal * p0 + v1.Binormal * p1 + v2.Binormal * p2;
		emitter.Tangent = v0.Tangent * p0 + v1.Tangent * p1 + v2.Tangent * p2;

		if (coordinate == CoordinateSystem::LH)
		{
			emitter.Position.Z = -emitter.Position.Z;
			emitter.Normal.Z = -emitter.Normal.Z;
			emitter.Binormal.Z = -emitter.Binormal.Z;
			emitter.Tangent.Z = -emitter.Tangent.Z;
		}

		return emitter;
	}

	Emitter GetEmitterFromVertex(IRandObject* g, int32_t time, CoordinateSystem coordinate, float magnification)
	{
		time = time % GetFrameCount();

		int32_t vertexInd = (int32_t)((GetVertexCount(time) - 1) * (g->GetRand()));
		vertexInd = Clamp(vertexInd, GetVertexCount(time) - 1, 0);
		Vertex& v = GetVertexes(time)[vertexInd];

		Emitter emitter;
		emitter.Position = v.Position * magnification;
		emitter.Normal = v.Normal;
		emitter.Binormal = v.Binormal;
		emitter.Tangent = v.Tangent;

		if (coordinate == CoordinateSystem::LH)
		{
			emitter.Position.Z = -emitter.Position.Z;
			emitter.Normal.Z = -emitter.Normal.Z;
			emitter.Binormal.Z = -emitter.Binormal.Z;
			emitter.Tangent.Z = -emitter.Tangent.Z;
		}

		return emitter;
	}

	Emitter GetEmitterFromVertex(int32_t index, int32_t time, CoordinateSystem coordinate, float magnification)
	{
		time = time % GetFrameCount();

		int32_t vertexInd = index % GetVertexCount(time);
		Vertex& v = GetVertexes(time)[vertexInd];

		Emitter emitter;
		emitter.Position = v.Position * magnification;
		emitter.Normal = v.Normal;
		emitter.Binormal = v.Binormal;
		emitter.Tangent = v.Tangent;

		if (coordinate == CoordinateSystem::LH)
		{
			emitter.Position.Z = -emitter.Position.Z;
			emitter.Normal.Z = -emitter.Normal.Z;
			emitter.Binormal.Z = -emitter.Binormal.Z;
			emitter.Tangent.Z = -emitter.Tangent.Z;
		}

		return emitter;
	}

	Emitter GetEmitterFromFace(IRandObject* g, int32_t time, CoordinateSystem coordinate, float magnification)
	{
		time = time % GetFrameCount();

		int32_t faceInd = (int32_t)((GetFaceCount(time) - 1) * (g->GetRand()));
		faceInd = Clamp(faceInd, GetFaceCount(time) - 1, 0);
		Face& face = GetFaces(time)[faceInd];
		Vertex& v0 = GetVertexes(time)[face.Indexes[0]];
		Vertex& v1 = GetVertexes(time)[face.Indexes[1]];
		Vertex& v2 = GetVertexes(time)[face.Indexes[2]];

		float p0 = 1.0f / 3.0f;
		float p1 = 1.0f / 3.0f;
		float p2 = 1.0f / 3.0f;

		Emitter emitter;
		emitter.Position = (v0.Position * p0 + v1.Position * p1 + v2.Position * p2) * magnification;
		emitter.Normal = v0.Normal * p0 + v1.Normal * p1 + v2.Normal * p2;
		emitter.Binormal = v0.Binormal * p0 + v1.Binormal * p1 + v2.Binormal * p2;
		emitter.Tangent = v0.Tangent * p0 + v1.Tangent * p1 + v2.Tangent * p2;

		if (coordinate == CoordinateSystem::LH)
		{
			emitter.Position.Z = -emitter.Position.Z;
			emitter.Normal.Z = -emitter.Normal.Z;
			emitter.Binormal.Z = -emitter.Binormal.Z;
			emitter.Tangent.Z = -emitter.Tangent.Z;
		}

		return emitter;
	}

	Emitter GetEmitterFromFace(int32_t index, int32_t time, CoordinateSystem coordinate, float magnification)
	{
		time = time % GetFrameCount();

		int32_t faceInd = index % (GetFaceCount(time) - 1);
		Face& face = GetFaces(time)[faceInd];
		Vertex& v0 = GetVertexes(time)[face.Indexes[0]];
		Vertex& v1 = GetVertexes(time)[face.Indexes[1]];
		Vertex& v2 = GetVertexes(time)[face.Indexes[2]];

		float p0 = 1.0f / 3.0f;
		float p1 = 1.0f / 3.0f;
		float p2 = 1.0f / 3.0f;

		Emitter emitter;
		emitter.Position = (v0.Position * p0 + v1.Position * p1 + v2.Position * p2) * magnification;
		emitter.Normal = v0.Normal * p0 + v1.Normal * p1 + v2.Normal * p2;
		emitter.Binormal = v0.Binormal * p0 + v1.Binormal * p1 + v2.Binormal * p2;
		emitter.Tangent = v0.Tangent * p0 + v1.Tangent * p1 + v2.Tangent * p2;

		if (coordinate == CoordinateSystem::LH)
		{
			emitter.Position.Z = -emitter.Position.Z;
			emitter.Normal.Z = -emitter.Normal.Z;
			emitter.Binormal.Z = -emitter.Binormal.Z;
			emitter.Tangent.Z = -emitter.Tangent.Z;
		}

		return emitter;
	}
};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
} // namespace Effekseer
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#endif // __EFFEKSEER_MODEL_H__


#ifndef	__EFFEKSEER_SOUND_PLAYER_H__
#define	__EFFEKSEER_SOUND_PLAYER_H__

//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace Effekseer
{
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------

typedef void* SoundHandle;
typedef void* SoundTag;

class SoundPlayer
{
public:
	struct InstanceParameter
	{
		void*		Data;
		float		Volume;
		float		Pan;
		float		Pitch;
		bool		Mode3D;
		Vector3D	Position;
		float		Distance;
	};

public:
	SoundPlayer() {}

	virtual ~SoundPlayer() {}

	virtual SoundHandle Play( SoundTag tag, const InstanceParameter& parameter ) = 0;
	
	virtual void Stop( SoundHandle handle, SoundTag tag ) = 0;

	virtual void Pause( SoundHandle handle, SoundTag tag, bool pause ) = 0;

	virtual bool CheckPlaying( SoundHandle handle, SoundTag tag ) = 0;

	virtual void StopTag( SoundTag tag ) = 0;

	virtual void PauseTag( SoundTag tag, bool pause ) = 0;

	virtual bool CheckPlayingTag( SoundTag tag ) = 0;

	virtual void StopAll() = 0;
};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
}
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#endif	// __EFFEKSEER_SOUND_PLAYER_H__


#ifndef __EFFEKSEER_SOUNDLOADER_H__
#define __EFFEKSEER_SOUNDLOADER_H__

//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace Effekseer
{
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
/**
	@brief	サウンド読み込み破棄関数指定クラス
*/
class SoundLoader
{
public:
	/**
		@brief	コンストラクタ
	*/
	SoundLoader()
	{
	}

	/**
		@brief	デストラクタ
	*/
	virtual ~SoundLoader()
	{
	}

	/**
		@brief	サウンドを読み込む。
		@param	path	[in]	読み込み元パス
		@return	サウンドのポインタ
		@note
		サウンドを読み込む。
		::Effekseer::Effect::Create実行時に使用される。
	*/
	virtual void* Load(const EFK_CHAR* path)
	{
		return NULL;
	}

	/**
		@brief
		\~English	a function called when sound is loaded
		\~Japanese	サウンドが読み込まれるときに呼ばれる関数
		@param	data
		\~English	data pointer
		\~Japanese	データのポインタ
		@param	size
		\~English	the size of data
		\~Japanese	データの大きさ
		@return
		\~English	a pointer of loaded texture
		\~Japanese	読み込まれたサウンドのポインタ
	*/
	virtual void* Load(const void* data, int32_t size)
	{
		return nullptr;
	}

	/**
		@brief	サウンドを破棄する。
		@param	data	[in]	サウンド
		@note
		サウンドを破棄する。
		::Effekseer::Effectのインスタンスが破棄された時に使用される。
	*/
	virtual void Unload(void* source)
	{
	}
};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
} // namespace Effekseer
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#endif // __EFFEKSEER_SOUNDLOADER_H__


#ifndef __EFFEKSEER_LOADER_H__
#define __EFFEKSEER_LOADER_H__

//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace Effekseer
{
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------

class EffectFactory;

/**
	@brief	設定クラス
	@note
	EffectLoader等、ファイル読み込みに関する設定することができる。
	Managerの代わりにエフェクト読み込み時に使用することで、Managerとは独立してEffectインスタンスを生成することができる。
*/
class Setting
	: public ReferenceObject
{
private:
	//! coordinate system
	CoordinateSystem m_coordinateSystem;

	EffectLoader* m_effectLoader;
	TextureLoader* m_textureLoader;
	SoundLoader* m_soundLoader;
	ModelLoader* m_modelLoader;
	MaterialLoader* m_materialLoader = nullptr;

	std::vector<EffectFactory*> effectFactories;

protected:
	/**
			@brief	コンストラクタ
			*/
	Setting();

	/**
			@brief	デストラクタ
			*/
	~Setting();

public:
	/**
			@brief	設定インスタンスを生成する。
		*/
	static Setting* Create();

	/**
		@brief	座標系を取得する。
		@return	座標系
		*/
	CoordinateSystem GetCoordinateSystem() const;

	/**
		@brief	座標系を設定する。
		@param	coordinateSystem	[in]	座標系
		@note
		座標系を設定する。
		エフェクトファイルを読み込む前に設定する必要がある。
		*/
	void SetCoordinateSystem(CoordinateSystem coordinateSystem);

	/**
			@brief	エフェクトローダーを取得する。
			@return	エフェクトローダー
			*/
	EffectLoader* GetEffectLoader();

	/**
			@brief	エフェクトローダーを設定する。
			@param	loader	[in]		ローダー
			*/
	void SetEffectLoader(EffectLoader* loader);

	/**
			@brief	テクスチャローダーを取得する。
			@return	テクスチャローダー
			*/
	TextureLoader* GetTextureLoader();

	/**
			@brief	テクスチャローダーを設定する。
			@param	loader	[in]		ローダー
			*/
	void SetTextureLoader(TextureLoader* loader);

	/**
			@brief	モデルローダーを取得する。
			@return	モデルローダー
			*/
	ModelLoader* GetModelLoader();

	/**
			@brief	モデルローダーを設定する。
			@param	loader	[in]		ローダー
			*/
	void SetModelLoader(ModelLoader* loader);

	/**
			@brief	サウンドローダーを取得する。
			@return	サウンドローダー
			*/
	SoundLoader* GetSoundLoader();

	/**
			@brief	サウンドローダーを設定する。
			@param	loader	[in]		ローダー
			*/
	void SetSoundLoader(SoundLoader* loader);

	/**
			@brief
			\~English get a material loader
			\~Japanese マテリアルローダーを取得する。
			@return
			\~English	loader
			\~Japanese ローダー
		*/
	MaterialLoader* GetMaterialLoader();

	/**
			@brief
			\~English specfiy a material loader
			\~Japanese マテリアルローダーを設定する。
			@param	loader
			\~English	loader
			\~Japanese ローダー
			*/
	void SetMaterialLoader(MaterialLoader* loader);

	/**
			@brief
			\~English	Add effect factory
			\~Japanese Effect factoryを追加する。
		*/
	void AddEffectFactory(EffectFactory* effectFactory);

	/**
			@brief
			\~English	Get effect factory
			\~Japanese Effect Factoryを取得する。
		*/
	EffectFactory* GetEffectFactory(int32_t ind) const;

	/**
			@brief
			\~English	clear effect factories
			\~Japanese 全てのEffect Factoryを削除する。
		*/
	void ClearEffectFactory();

	/**
			@brief
			\~English	Get the number of effect factory
			\~Japanese Effect Factoryの数を取得する。
		*/
	int32_t GetEffectFactoryCount() const;
};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
} // namespace Effekseer
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#endif // __EFFEKSEER_LOADER_H__


#ifndef __EFFEKSEER_SERVER_H__
#define __EFFEKSEER_SERVER_H__

#if !(defined(_PSVITA) || defined(_XBOXONE))

//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace Effekseer
{
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
/**
	@brief
	\~English	A server to edit effect from client such an editor
	\~Japanese	エディタといったクライアントからエフェクトを編集するためのサーバー
*/
class Server
{
public:
	Server()
	{
	}
	virtual ~Server()
	{
	}

	/**
		@brief
		\~English	create a server instance
		\~Japanese	サーバーのインスタンスを生成する。
	*/
	static Server* Create();

	/**
		@brief
		\~English	start a server
		\~Japanese	サーバーを開始する。
	*/
	virtual bool Start(uint16_t port) = 0;

	/**
		@brief
		\~English	stop a server
		\~Japanese	サーバーを終了する。
	*/
	virtual void Stop() = 0;

	/**
		@brief
		\~English	register an effect as a target to edit.
		\~Japanese	エフェクトを編集の対象として登録する。
		@param	key	
		\~English	a key to search an effect
		\~Japanese	検索用キー
		@param	effect
		\~English	an effect to be edit
		\~Japanese	編集される対象のエフェクト
	*/
	virtual void Register(const EFK_CHAR* key, Effect* effect) = 0;

	/**
		@brief
		\~English	unregister an effect
		\~Japanese	エフェクトを対象から外す。
		@param	effect
		\~English	an effect registered
		\~Japanese	登録されているエフェクト
	*/
	virtual void Unregister(Effect* effect) = 0;

	/**
		@brief	
		\~English	update a server and reload effects
		\~Japanese	サーバーを更新し、エフェクトのリロードを行う。
		@brief	managers
		\~English	all managers which is playing effects.
		\~Japanese	エフェクトを再生している全てのマネージャー
		@brief	managerCount
		\~English	the number of manager
		\~Japanese	マネージャーの個数

	*/
	virtual void Update(Manager** managers = nullptr, int32_t managerCount = 0, ReloadingThreadType reloadingThreadType = ReloadingThreadType::Main) = 0;

	/**
		@brief
		\~English	Specify root path to load materials
		\~Japanese	素材のルートパスを設定する。
	*/
	virtual void SetMaterialPath(const EFK_CHAR* materialPath) = 0;

	/**
		@brief
		\~English	deprecated
		\~Japanese	非推奨
	*/
	virtual void Regist(const EFK_CHAR* key, Effect* effect) = 0;

	/**
		@brief
		\~English	deprecated
		\~Japanese	非推奨
	*/
	virtual void Unregist(Effect* effect) = 0;
};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
} // namespace Effekseer
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------

#endif // #if !( defined(_PSVITA) || defined(_XBOXONE) )

#endif // __EFFEKSEER_SERVER_H__


#ifndef __EFFEKSEER_CLIENT_H__
#define __EFFEKSEER_CLIENT_H__

#if !(defined(_PSVITA) || defined(_PS4) || defined(_SWITCH) || defined(_XBOXONE))

//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace Effekseer
{
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
class Client
{
public:
	Client()
	{
	}
	virtual ~Client()
	{
	}

	static Client* Create();

	virtual bool Start(char* host, uint16_t port) = 0;
	virtual void Stop() = 0;

	virtual void Reload(const EFK_CHAR* key, void* data, int32_t size) = 0;
	virtual void Reload(Manager* manager, const EFK_CHAR* path, const EFK_CHAR* key) = 0;
	virtual bool IsConnected() = 0;
};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
} // namespace Effekseer
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------

#endif // #if !( defined(_PSVITA) || defined(_PS4) || defined(_SWITCH) || defined(_XBOXONE) )

#endif // __EFFEKSEER_CLIENT_H__


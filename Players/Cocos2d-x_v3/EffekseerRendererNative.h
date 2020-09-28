#pragma once
#include "cocos2d.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#define __EFFEKSEER_RENDERER_GLES2__
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#define __EFFEKSEER_RENDERER_GLES2__
#else
#define __EFFEKSEER_RENDERER_GL2__
#endif

#ifndef __EFFEKSEERRENDERER_COMMON_UTILS_H__
#define __EFFEKSEERRENDERER_COMMON_UTILS_H__


#include "EffekseerNative.h"
#include <algorithm>
#include <array>
#include <assert.h>
#include <functional>
#include <math.h>
#include <string.h>

namespace EffekseerRenderer
{

using VertexFloat3 = ::Effekseer::Vector3D;
using VertexColor = ::Effekseer::Color;

struct DynamicVertex
{
	VertexFloat3 Pos;
	VertexColor Col;
	//! packed vector
	VertexColor Normal;
	//! packed vector
	VertexColor Tangent;

	union {
		//! UV1 (for template)
		float UV[2];
		float UV1[2];
	};

	float UV2[2];

#ifdef __EFFEKSEER_BUILD_VERSION16__
	float AlphaUV[2];

	float FlipbookIndexAndNextRate;

	float AlphaThreshold;
#endif

	void SetColor(const VertexColor& color) { Col = color; }
};

struct DynamicVertexWithCustomData
{
	DynamicVertex V;
	std::array<float, 4> CustomData1;
	std::array<float, 4> CustomData2;
};

struct SimpleVertex
{
	VertexFloat3 Pos;
	uint8_t Col[4];

	union {
		float UV[2];
		//! dummy for template
		float UV2[2];
	};

#ifdef __EFFEKSEER_BUILD_VERSION16__
	float AlphaUV[2];

	float FlipbookIndexAndNextRate;

	float AlphaThreshold;
#endif

	void SetColor(const ::Effekseer::Color& color)
	{
		Col[0] = color.R;
		Col[1] = color.G;
		Col[2] = color.B;
		Col[3] = color.A;
	}
};

struct SimpleVertexDX9
{
	VertexFloat3 Pos;
	uint8_t Col[4];

	union {
		float UV[2];
		//! dummy for template
		float UV2[2];
	};

#ifdef __EFFEKSEER_BUILD_VERSION16__
	float AlphaUV[2];

	float FlipbookIndexAndNextRate;

	float AlphaThreshold;
#endif

	void SetColor(const ::Effekseer::Color& color)
	{
		Col[0] = color.B;
		Col[1] = color.G;
		Col[2] = color.R;
		Col[3] = color.A;
	}
};

struct VertexDistortion
{
	VertexFloat3 Pos;
	uint8_t Col[4];

	union {
		float UV[2];
		//! dummy for template
		float UV2[2];
	};

	VertexFloat3 Binormal;
	VertexFloat3 Tangent;

#ifdef __EFFEKSEER_BUILD_VERSION16__
	float AlphaUV[2];

	float FlipbookIndexAndNextRate;

	float AlphaThreshold;
#endif

	void SetColor(const ::Effekseer::Color& color)
	{
		Col[0] = color.R;
		Col[1] = color.G;
		Col[2] = color.B;
		Col[3] = color.A;
	}
};

struct VertexDistortionDX9
{
	VertexFloat3 Pos;
	uint8_t Col[4];

	union {
		float UV[2];
		//! dummy for template
		float UV2[2];
	};

	VertexFloat3 Binormal;
	VertexFloat3 Tangent;

#ifdef __EFFEKSEER_BUILD_VERSION16__
	float AlphaUV[2];

	float FlipbookIndexAndNextRate;

	float AlphaThreshold;
#endif

	void SetColor(const ::Effekseer::Color& color)
	{
		Col[0] = color.B;
		Col[1] = color.G;
		Col[2] = color.R;
		Col[3] = color.A;
	}
};

static int32_t GetMaximumVertexSizeInAllTypes()
{
	size_t size = sizeof(SimpleVertex);
	size = (std::max)(size, sizeof(VertexDistortion));
	size = (std::max)(size, sizeof(DynamicVertexWithCustomData));
	return static_cast<int32_t>(size);
};

/**
	@brief	a view class to access an array with a stride
*/
template <typename T> struct StrideView
{
	int32_t stride_;
	uint8_t* pointer_;
	uint8_t* pointerOrigin_;

#ifndef NDEBUG
	int32_t offset_;
	int32_t elementCount_;
#endif

	StrideView(void* pointer, int32_t stride, int32_t elementCount)
		: stride_(stride)
		, pointer_(reinterpret_cast<uint8_t*>(pointer))
		, pointerOrigin_(reinterpret_cast<uint8_t*>(pointer))
#ifndef NDEBUG
		, offset_(0)
		, elementCount_(elementCount)
#endif
	{
	}

	T& operator[](int i) const
	{
#ifndef NDEBUG
		assert(i >= 0);
		assert(i + offset_ < elementCount_);
#endif
		return *reinterpret_cast<T*>((pointer_ + stride_ * i));
	}

	StrideView& operator+=(const int& rhs)
	{
#ifndef NDEBUG
		offset_ += rhs;
#endif
		pointer_ += stride_ * rhs;
		return *this;
	}

	void Reset()
	{
#ifndef NDEBUG
		offset_ = 0;
#endif
		pointer_ = pointerOrigin_;
	}
};

/**
	@brief	a view class to access an array with a stride
*/
template <> struct StrideView<SimpleVertex>
{
	static const int32_t stride_ = sizeof(SimpleVertex);
	uint8_t* pointer_;
	uint8_t* pointerOrigin_;

#ifndef NDEBUG
	int32_t offset_;
	int32_t elementCount_;
#endif

	StrideView(void* pointer, int32_t stride, int32_t elementCount)
		: pointer_(reinterpret_cast<uint8_t*>(pointer))
		, pointerOrigin_(reinterpret_cast<uint8_t*>(pointer))
#ifndef NDEBUG
		, offset_(0)
		, elementCount_(elementCount)
#endif
	{
		assert(stride_ == stride);
	}

	SimpleVertex& operator[](int i) const
	{
#ifndef NDEBUG
		assert(i >= 0);
		assert(i + offset_ < elementCount_);
#endif
		return *reinterpret_cast<SimpleVertex*>((pointer_ + stride_ * i));
	}

	StrideView& operator+=(const int& rhs)
	{
#ifndef NDEBUG
		offset_ += rhs;
#endif
		pointer_ += stride_ * rhs;
		return *this;
	}

	void Reset()
	{
#ifndef NDEBUG
		offset_ = 0;
#endif
		pointer_ = pointerOrigin_;
	}
};

/**
	@brief	a view class to access an array with a stride
*/
template <> struct StrideView<SimpleVertexDX9>
{
	static const int32_t stride_ = sizeof(SimpleVertexDX9);
	uint8_t* pointer_;
	uint8_t* pointerOrigin_;

#ifndef NDEBUG
	int32_t offset_;
	int32_t elementCount_;
#endif

	StrideView(void* pointer, int32_t stride, int32_t elementCount)
		: pointer_(reinterpret_cast<uint8_t*>(pointer))
		, pointerOrigin_(reinterpret_cast<uint8_t*>(pointer))
#ifndef NDEBUG
		, offset_(0)
		, elementCount_(elementCount)
#endif
	{
		assert(stride_ == stride);
	}

	SimpleVertexDX9& operator[](int i) const
	{
#ifndef NDEBUG
		assert(i >= 0);
		assert(i + offset_ < elementCount_);
#endif
		return *reinterpret_cast<SimpleVertexDX9*>((pointer_ + stride_ * i));
	}

	StrideView& operator+=(const int& rhs)
	{
#ifndef NDEBUG
		offset_ += rhs;
#endif
		pointer_ += stride_ * rhs;
		return *this;
	}

	void Reset()
	{
#ifndef NDEBUG
		offset_ = 0;
#endif
		pointer_ = pointerOrigin_;
	}
};

void CalcBillboard(::Effekseer::BillboardType billboardType,
				   Effekseer::Mat43f& dst,
				   ::Effekseer::Vec3f& s,
				   ::Effekseer::Vec3f& R,
				   ::Effekseer::Vec3f& F,
				   const ::Effekseer::Mat43f& src,
				   const ::Effekseer::Vec3f& frontDirection);

/**
	@brief Spline generator
	@note
	Reference https://qiita.com/edo_m18/items/f2f0c6bf9032b0ec12d4
*/
class SplineGenerator
{
	std::vector<Effekseer::Vec3f> a;
	std::vector<Effekseer::Vec3f> b;
	std::vector<Effekseer::Vec3f> c;
	std::vector<Effekseer::Vec3f> d;
	std::vector<Effekseer::Vec3f> w;
	std::vector<bool> isSame;

public:
	void AddVertex(const Effekseer::Vec3f& v);

	void Calculate();

	void Reset();

	Effekseer::Vec3f GetValue(float t) const;
};

void ApplyDepthParameters(::Effekseer::Mat43f& mat,
						  const ::Effekseer::Vec3f& cameraFront,
						  const ::Effekseer::Vec3f& cameraPos,
						  ::Effekseer::NodeRendererDepthParameter* depthParameter,
						  bool isRightHand);

void ApplyDepthParameters(::Effekseer::Mat43f& mat,
						  const ::Effekseer::Vec3f& cameraFront,
						  const ::Effekseer::Vec3f& cameraPos,
						  ::Effekseer::Vec3f& scaleValues,
						  ::Effekseer::NodeRendererDepthParameter* depthParameter,
						  bool isRightHand);

void ApplyDepthParameters(::Effekseer::Mat43f& mat,
						  ::Effekseer::Vec3f& translationValues,
						  ::Effekseer::Vec3f& scaleValues,
						  const ::Effekseer::Vec3f& cameraFront,
						  const ::Effekseer::Vec3f& cameraPos,
						  ::Effekseer::NodeRendererDepthParameter* depthParameter,
						  bool isRightHand);

void ApplyDepthParameters(::Effekseer::Mat44f& mat,
						  const ::Effekseer::Vec3f& cameraFront,
						  const ::Effekseer::Vec3f& cameraPos,
						  ::Effekseer::NodeRendererDepthParameter* depthParameter,
						  bool isRightHand);

template <typename Vertex> inline void TransformStandardVertexes(Vertex& vertexes, int32_t count, const ::Effekseer::Mat43f& mat)
{
	using namespace Effekseer;

	SIMD4f m0 = mat.X;
	SIMD4f m1 = mat.Y;
	SIMD4f m2 = mat.Z;
	SIMD4f m3 = SIMD4f::SetZero();
	SIMD4f::Transpose(m0, m1, m2, m3);

	for (int i = 0; i < count; i++)
	{
		SIMD4f iPos = SIMD4f::Load3(&vertexes[i].Pos);

		SIMD4f oPos = SIMD4f::MulAddLane<0>(m3, m0, iPos);
		oPos = SIMD4f::MulAddLane<1>(oPos, m1, iPos);
		oPos = SIMD4f::MulAddLane<2>(oPos, m2, iPos);

		SIMD4f::Store3(&vertexes[i].Pos, oPos);
	}
}

template <typename VertexDistortion>
inline void TransformDistortionVertexes(VertexDistortion& vertexes, int32_t count, const ::Effekseer::Mat43f& mat)
{
	using namespace Effekseer;

	SIMD4f m0 = mat.X;
	SIMD4f m1 = mat.Y;
	SIMD4f m2 = mat.Z;
	SIMD4f m3 = SIMD4f::SetZero();
	SIMD4f::Transpose(m0, m1, m2, m3);

	for (int i = 0; i < count; i++)
	{
		SIMD4f iPos = SIMD4f::Load3(&vertexes[i].Pos);
		SIMD4f iTangent = SIMD4f::Load3(&vertexes[i].Tangent);
		SIMD4f iBinormal = SIMD4f::Load3(&vertexes[i].Binormal);

		SIMD4f oPos = SIMD4f::MulAddLane<0>(m3, m0, iPos);
		oPos = SIMD4f::MulAddLane<1>(oPos, m1, iPos);
		oPos = SIMD4f::MulAddLane<2>(oPos, m2, iPos);

		SIMD4f oTangent = SIMD4f::MulLane<0>(m0, iTangent);
		oTangent = SIMD4f::MulAddLane<1>(oTangent, m1, iTangent);
		oTangent = SIMD4f::MulAddLane<2>(oTangent, m2, iTangent);

		SIMD4f oBinormal = SIMD4f::MulLane<0>(m0, iBinormal);
		oBinormal = SIMD4f::MulAddLane<1>(oBinormal, m1, iBinormal);
		oBinormal = SIMD4f::MulAddLane<2>(oBinormal, m2, iBinormal);

		SIMD4f::Store3(&vertexes[i].Pos, oPos);
		SIMD4f::Store3(&vertexes[i].Tangent, oTangent);
		SIMD4f::Store3(&vertexes[i].Binormal, oBinormal);
	}
}

inline void TransformVertexes(StrideView<VertexDistortion>& v, int32_t count, const ::Effekseer::Mat43f& mat)
{
	TransformDistortionVertexes(v, count, mat);
}

inline void TransformVertexes(StrideView<VertexDistortionDX9>& v, int32_t count, const ::Effekseer::Mat43f& mat)
{
	TransformDistortionVertexes(v, count, mat);
}

inline void TransformVertexes(StrideView<SimpleVertex>& v, int32_t count, const ::Effekseer::Mat43f& mat)
{
	TransformStandardVertexes(v, count, mat);
}

inline void TransformVertexes(StrideView<SimpleVertexDX9>& v, int32_t count, const ::Effekseer::Mat43f& mat)
{
	TransformStandardVertexes(v, count, mat);
}

inline void TransformVertexes(StrideView<DynamicVertex>& v, int32_t count, const ::Effekseer::Mat43f& mat)
{
	TransformStandardVertexes(v, count, mat);
}

inline void SwapRGBAToBGRA(Effekseer::Color& color)
{
	auto temp = color;
	color.B = temp.R;
	color.R = temp.B;
}

inline Effekseer::Color PackVector3DF(const Effekseer::Vec3f& v)
{
	Effekseer::Color ret;
	ret.R = static_cast<uint8_t>(Effekseer::Clamp(((v.GetX() + 1.0f) / 2.0f + 0.5f / 255.0f) * 255.0f, 255, 0));
	ret.G = static_cast<uint8_t>(Effekseer::Clamp(((v.GetY() + 1.0f) / 2.0f + 0.5f / 255.0f) * 255.0f, 255, 0));
	ret.B = static_cast<uint8_t>(Effekseer::Clamp(((v.GetZ() + 1.0f) / 2.0f + 0.5f / 255.0f) * 255.0f, 255, 0));
	ret.A = 255;
	return ret;
}

inline Effekseer::Vec3f SafeNormalize(const Effekseer::Vec3f& v)
{
	auto lengthSq = v.GetSquaredLength();
	auto e = 0.0001f;
	if (lengthSq < e * e)
	{
		return v;
	}

	return v * Effekseer::Rsqrt(lengthSq);
}

struct MaterialShaderParameterGenerator
{
	int32_t VertexSize = 0;
	int32_t VertexShaderUniformBufferSize = 0;
	int32_t PixelShaderUniformBufferSize = 0;

	int32_t VertexCameraMatrixOffset = -1;
	int32_t VertexProjectionMatrixOffset = -1;
	int32_t VertexInversedFlagOffset = -1;
	int32_t VertexPredefinedOffset = -1;
	int32_t VertexCameraPositionOffset = -1;
	int32_t VertexUserUniformOffset = -1;

	int32_t PixelInversedFlagOffset = -1;
	int32_t PixelPredefinedOffset = -1;
	int32_t PixelCameraPositionOffset = -1;
	int32_t PixelLightDirectionOffset = -1;
	int32_t PixelLightColorOffset = -1;
	int32_t PixelLightAmbientColorOffset = -1;
	int32_t PixelCameraMatrixOffset = -1;
	int32_t PixelUserUniformOffset = -1;

	int32_t VertexModelMatrixOffset = -1;
	int32_t VertexModelUVOffset = -1;
	int32_t VertexModelColorOffset = -1;

	int32_t VertexModelCustomData1Offset = -1;
	int32_t VertexModelCustomData2Offset = -1;

	MaterialShaderParameterGenerator(const ::Effekseer::Material& material, bool isModel, int32_t stage, int32_t instanceCount)
	{
		if (isModel)
		{
			VertexSize = sizeof(::Effekseer::Model::Vertex);
		}
		else if (material.GetIsSimpleVertex())
		{
			VertexSize = sizeof(EffekseerRenderer::SimpleVertex);
		}
		else
		{
			VertexSize = sizeof(EffekseerRenderer::DynamicVertex) +
						 sizeof(float) * (material.GetCustomData1Count() + material.GetCustomData2Count());
		}

		if (isModel)
		{
			int32_t vsOffset = 0;
			VertexProjectionMatrixOffset = vsOffset;
			vsOffset += sizeof(Effekseer::Mat44f);

			VertexModelMatrixOffset = vsOffset;
			vsOffset += sizeof(Effekseer::Mat44f) * instanceCount;

			VertexModelUVOffset = vsOffset;
			vsOffset += sizeof(float) * 4 * instanceCount;

			VertexModelColorOffset = vsOffset;
			vsOffset += sizeof(float) * 4 * instanceCount;

			VertexInversedFlagOffset = vsOffset;
			vsOffset += sizeof(float) * 4;

			VertexPredefinedOffset = vsOffset;
			vsOffset += sizeof(float) * 4;

			VertexCameraPositionOffset = vsOffset;
			vsOffset += sizeof(float) * 4;

			if (material.GetCustomData1Count() > 0)
			{
				VertexModelCustomData1Offset = vsOffset;
				vsOffset += sizeof(float) * 4 * instanceCount;
			}

			if (material.GetCustomData2Count() > 0)
			{
				VertexModelCustomData2Offset = vsOffset;
				vsOffset += sizeof(float) * 4 * instanceCount;
			}

			VertexUserUniformOffset = vsOffset;
			vsOffset += sizeof(float) * 4 * material.GetUniformCount();

			VertexShaderUniformBufferSize = vsOffset;
		}
		else
		{
			int32_t vsOffset = 0;
			VertexCameraMatrixOffset = vsOffset;
			vsOffset += sizeof(Effekseer::Mat44f);

			VertexProjectionMatrixOffset = vsOffset;
			vsOffset += sizeof(Effekseer::Mat44f);

			VertexInversedFlagOffset = vsOffset;
			vsOffset += sizeof(float) * 4;

			VertexPredefinedOffset = vsOffset;
			vsOffset += sizeof(float) * 4;

			VertexCameraPositionOffset = vsOffset;
			vsOffset += sizeof(float) * 4;

			VertexUserUniformOffset = vsOffset;
			vsOffset += sizeof(float) * 4 * material.GetUniformCount();

			VertexShaderUniformBufferSize = vsOffset;
		}

		int32_t psOffset = 0;

		PixelInversedFlagOffset = psOffset;
		psOffset += sizeof(float) * 4;

		PixelPredefinedOffset = psOffset;
		psOffset += sizeof(float) * 4;

		PixelCameraPositionOffset = psOffset;
		psOffset += sizeof(float) * 4;

		if (material.GetShadingModel() == ::Effekseer::ShadingModelType::Lit)
		{
			PixelLightDirectionOffset = psOffset;
			psOffset += sizeof(float) * 4;

			PixelLightColorOffset = psOffset;
			psOffset += sizeof(float) * 4;

			PixelLightAmbientColorOffset = psOffset;
			psOffset += sizeof(float) * 4;
		}

		if (material.GetHasRefraction() && stage == 1)
		{
			PixelCameraMatrixOffset = psOffset;
			psOffset += sizeof(Effekseer::Mat44f);
		}

		PixelUserUniformOffset = psOffset;
		psOffset += sizeof(float) * 4 * material.GetUniformCount();

		PixelShaderUniformBufferSize = psOffset;
	}
};

} // namespace EffekseerRenderer
#endif // __EFFEKSEERRENDERER_COMMON_UTILS_H__

#ifndef	__EFFEKSEERRENDERER_RENDERER_H__
#define	__EFFEKSEERRENDERER_RENDERER_H__

//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------
#include "EffekseerNative.h"

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
namespace EffekseerRenderer
{
//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------

/**
	@brief	背景を歪ませるエフェクトを描画する前に実行されるコールバック
*/
class DistortingCallback
{
public:
	DistortingCallback() {}
	virtual ~DistortingCallback() {}

	virtual bool OnDistorting() { return false; }
};

/**
	@brief	
	\~english A status of UV when particles are rendered.
	\~japanese パーティクルを描画する時のUVの状態
*/
enum class UVStyle
{
	Normal,
	VerticalFlipped,
};

/**
	@brief
	\~english A type of texture which is rendered when textures are not assigned.
	\~japanese テクスチャが設定されていないときに描画されるテクスチャの種類
*/
enum class ProxyTextureType
{
	White,
	Normal,
};

/**
	@brief
	\~english A class which contains a graphics device
	\~japanese グラフィックデバイスを格納しているクラス
*/
class GraphicsDevice : public ::Effekseer::IReference
{
public:
	GraphicsDevice() = default;
	virtual ~GraphicsDevice() = default;
};	
	
class CommandList : public ::Effekseer::IReference
{
public:
	CommandList() = default;
	virtual ~CommandList() = default;
};

class SingleFrameMemoryPool : public ::Effekseer::IReference
{
public:
	SingleFrameMemoryPool() = default;
	virtual ~SingleFrameMemoryPool() = default;

	/**
		@brief
		\~English	notify that new frame is started.
		\~Japanese	新規フレームが始ったことを通知する。
	*/
	virtual void NewFrame() {}
};

class Renderer
	: public ::Effekseer::IReference
{
protected:
	Renderer();
	virtual ~Renderer();

	class Impl;
	Impl* impl = nullptr;

public:

	/**
		@brief	only for Effekseer backend developer. Effekseer User doesn't need it.
	*/
	Impl* GetImpl();

	/**
		@brief	デバイスロストが発生した時に実行する。
	*/
	virtual void OnLostDevice() = 0;

	/**
		@brief	デバイスがリセットされた時に実行する。
	*/
	virtual void OnResetDevice() = 0;

	/**
		@brief	このインスタンスを破棄する。
	*/
	virtual void Destroy() = 0;

	/**
		@brief	ステートを復帰するかどうかのフラグを設定する。
	*/
	virtual void SetRestorationOfStatesFlag(bool flag) = 0;

	/**
		@brief	描画を開始する時に実行する。
	*/
	virtual bool BeginRendering() = 0;

	/**
		@brief	描画を終了する時に実行する。
	*/
	virtual bool EndRendering() = 0;

	/**
		@brief	Get the direction of light
	*/
	virtual ::Effekseer::Vector3D GetLightDirection() const;

	/**
		@brief	Specifiy the direction of light
	*/
	virtual void SetLightDirection(const ::Effekseer::Vector3D& direction);

	/**
		@brief	Get the color of light
	*/
	virtual const ::Effekseer::Color& GetLightColor() const;

	/**
		@brief	Specify the color of light
	*/
	virtual void SetLightColor(const ::Effekseer::Color& color);

	/**
		@brief	Get the color of ambient
	*/
	virtual const ::Effekseer::Color& GetLightAmbientColor() const;

	/**
		@brief	Specify the color of ambient
	*/
	virtual void SetLightAmbientColor(const ::Effekseer::Color& color);

	/**
		@brief	最大描画スプライト数を取得する。
	*/
	virtual int32_t GetSquareMaxCount() const = 0;

	/**
		@brief	Get a projection matrix
	*/
	virtual ::Effekseer::Matrix44 GetProjectionMatrix() const;

	/**
		@brief	Set a projection matrix
	*/
	virtual void SetProjectionMatrix( const ::Effekseer::Matrix44& mat );

	/**
		@brief	Get a camera matrix
	*/
	virtual ::Effekseer::Matrix44 GetCameraMatrix() const;

	/**
		@brief	Set a camera matrix
	*/
	virtual void SetCameraMatrix( const ::Effekseer::Matrix44& mat );

	/**
		@brief	Get a camera projection matrix
	*/
	virtual ::Effekseer::Matrix44 GetCameraProjectionMatrix() const;

	/**
		@brief	Get a front direction of camera
		@note
		We don't recommend to use it without understanding of internal code.
	*/
	virtual ::Effekseer::Vector3D GetCameraFrontDirection() const;

	/**
		@brief	Get a position of camera
		@note
		We don't recommend to use it without understanding of internal code.
	*/
	virtual ::Effekseer::Vector3D GetCameraPosition() const;

	/**
		@brief	Set a front direction and position of camera manually
		@param front (Right Hand) a direction from focus to eye, (Left Hand) a direction from eye to focus, 
		@note
		These are set based on camera matrix automatically.
		It is failed on some platform.
	*/
	virtual void SetCameraParameter(const ::Effekseer::Vector3D& front, const ::Effekseer::Vector3D& position);

	/**
		@brief	スプライトレンダラーを生成する。
	*/
	virtual ::Effekseer::SpriteRenderer* CreateSpriteRenderer() = 0;

	/**
		@brief	リボンレンダラーを生成する。
	*/
	virtual ::Effekseer::RibbonRenderer* CreateRibbonRenderer() = 0;

	/**
		@brief	リングレンダラーを生成する。
	*/
	virtual ::Effekseer::RingRenderer* CreateRingRenderer() = 0;

	/**
		@brief	モデルレンダラーを生成する。
	*/
	virtual ::Effekseer::ModelRenderer* CreateModelRenderer() = 0;

	/**
		@brief	軌跡レンダラーを生成する。
	*/
	virtual ::Effekseer::TrackRenderer* CreateTrackRenderer() = 0;

	/**
		@brief	標準のテクスチャ読込クラスを生成する。
	*/
	virtual ::Effekseer::TextureLoader* CreateTextureLoader( ::Effekseer::FileInterface* fileInterface = NULL ) = 0;

	/**
		@brief	標準のモデル読込クラスを生成する。
	*/
	virtual ::Effekseer::ModelLoader* CreateModelLoader( ::Effekseer::FileInterface* fileInterface = NULL ) = 0;

	/**
	@brief	
	\~english Create default material loader
	\~japanese 標準のマテリアル読込クラスを生成する。

	*/
	virtual ::Effekseer::MaterialLoader* CreateMaterialLoader(::Effekseer::FileInterface* fileInterface = nullptr) = 0;

	/**
		@brief	レンダーステートを強制的にリセットする。
	*/
	virtual void ResetRenderState() = 0;

	/**
	@brief	背景を歪ませるエフェクトが描画される前に呼ばれるコールバックを取得する。
	*/
	virtual DistortingCallback* GetDistortingCallback() = 0;

	/**
	@brief	背景を歪ませるエフェクトが描画される前に呼ばれるコールバックを設定する。
	*/
	virtual void SetDistortingCallback(DistortingCallback* callback) = 0;

	/**
	@brief	
	\~english Get draw call count
	\~japanese ドローコールの回数を取得する
	*/
	virtual int32_t GetDrawCallCount() const;

	/**
	@brief
	\~english Get the number of vertex drawn
	\~japanese 描画された頂点数をリセットする
	*/
	virtual int32_t GetDrawVertexCount() const;

	/**
	@brief
	\~english Reset draw call count
	\~japanese ドローコールの回数をリセットする
	*/
	virtual void ResetDrawCallCount();

	/**
	@brief
	\~english Reset the number of vertex drawn
	\~japanese 描画された頂点数をリセットする
	*/
	virtual void ResetDrawVertexCount();

	/**
	@brief
	\~english Get a render mode.
	\~japanese 描画モードを取得する。
	*/
	virtual Effekseer::RenderMode GetRenderMode() const;

	/**
	@brief	
	\~english Specify a render mode.
	\~japanese 描画モードを設定する。
	*/
	virtual void SetRenderMode(Effekseer::RenderMode renderMode);

	/**
	@brief
	\~english Get an UV Style of texture when particles are rendered.
	\~japanese パーティクルを描画するときのUVの状態を取得する。
	*/
	virtual UVStyle GetTextureUVStyle() const;

	/**
	@brief
	\~english Set an UV Style of texture when particles are rendered.
	\~japanese パーティクルを描画するときのUVの状態を設定する。
	*/
	virtual void SetTextureUVStyle(UVStyle style);

	/**
	@brief
	\~english Get an UV Style of background when particles are rendered.
	\~japanese パーティクルを描画するときの背景のUVの状態を取得する。
	*/
	virtual UVStyle GetBackgroundTextureUVStyle() const;

	/**
	@brief
	\~english Set an UV Style of background when particles are rendered.
	\~japanese パーティクルを描画するときの背景のUVの状態を設定する。
	*/
	virtual void SetBackgroundTextureUVStyle(UVStyle style);

	/**
	@brief
	\~english Get a current time (s)
	\~japanese 現在の時間を取得する。(秒)
	*/
	virtual float GetTime() const;

	/**
	@brief
	\~english Set a current time (s)
	\~japanese 現在の時間を設定する。(秒)
	*/
	virtual void SetTime(float time);

	/**
	@brief
	\~English	specify a command list to render.  This function is available except DirectX9, DirectX11 and OpenGL.
	\~Japanese	描画に使用するコマンドリストを設定する。この関数はDirectX9、DirectX11、OpenGL以外で使用できる。
	*/
	virtual void SetCommandList(CommandList* commandList) {}

	/**
	@brief
	\~English	Specify a background texture.
	\~Japanese	背景のテクスチャを設定する。
	@note
	\~English	Specified texture is not deleted by the renderer. This function is available except DirectX9, DirectX11.
	\~Japanese	設定されたテクスチャはレンダラーによって削除されない。この関数はDirectX9、DirectX11以外で使用できる。
	*/
	virtual void SetBackgroundTexture(::Effekseer::TextureData* textureData);

	/**
	@brief
	\~English	Create a proxy texture
	\~Japanese	代替のテクスチャを生成する
	*/
	virtual Effekseer::TextureData* CreateProxyTexture(ProxyTextureType type) { return nullptr; }

	/**
	@brief
	\~English	Delete a proxy texture
	\~Japanese	代替のテクスチャを削除する
	*/
	virtual void DeleteProxyTexture(Effekseer::TextureData* data) { }
};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
}
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#endif	// __EFFEKSEERRENDERER_RENDERER_H__

#ifndef __EFFEKSEERRENDERER_RENDERER_IMPL_H__
#define __EFFEKSEERRENDERER_RENDERER_IMPL_H__


#include "EffekseerNative.h"


namespace EffekseerRenderer
{

class Renderer::Impl : public ::Effekseer::AlignedAllocationPolicy<16>
{
private:
	::Effekseer::Mat44f projectionMat_;
	::Effekseer::Mat44f cameraMat_;
	::Effekseer::Mat44f cameraProjMat_;

	::Effekseer::Vec3f cameraPosition_;
	::Effekseer::Vec3f cameraFrontDirection_;

	::Effekseer::Vec3f lightDirection_ = ::Effekseer::Vec3f(1.0f, 1.0f, 1.0f);
	::Effekseer::Color lightColor_ = ::Effekseer::Color(255, 255, 255, 255);
	::Effekseer::Color lightAmbient_ = ::Effekseer::Color(40, 40, 40, 255);

	UVStyle textureUVStyle = UVStyle::Normal;
	UVStyle backgroundTextureUVStyle = UVStyle::Normal;
	float time_ = 0.0f;

	Effekseer::RenderMode renderMode_ = Effekseer::RenderMode::Normal;

	::Effekseer::TextureData* whiteProxyTexture_ = nullptr;
	::Effekseer::TextureData* normalProxyTexture_ = nullptr;

public:
	int32_t drawcallCount = 0;
	int32_t drawvertexCount = 0;
	bool isRenderModeValid = true;

	::Effekseer::Vector3D GetLightDirection() const;

	void SetLightDirection(const ::Effekseer::Vector3D& direction);

	const ::Effekseer::Color& GetLightColor() const;

	void SetLightColor(const ::Effekseer::Color& color);

	const ::Effekseer::Color& GetLightAmbientColor() const;

	void SetLightAmbientColor(const ::Effekseer::Color& color);

	void CalculateCameraProjectionMatrix();

	::Effekseer::Matrix44 GetProjectionMatrix() const;

	void SetProjectionMatrix(const ::Effekseer::Matrix44& mat);

	::Effekseer::Matrix44 GetCameraMatrix() const;

	void SetCameraMatrix(const ::Effekseer::Matrix44& mat);

	::Effekseer::Vector3D GetCameraFrontDirection() const;

	::Effekseer::Vector3D GetCameraPosition() const;

	void SetCameraParameter(const ::Effekseer::Vector3D& front, const ::Effekseer::Vector3D& position);

	::Effekseer::Matrix44 GetCameraProjectionMatrix() const;

	void CreateProxyTextures(Renderer* renderer);

	void DeleteProxyTextures(Renderer* renderer);

	::Effekseer::TextureData* GetProxyTexture(EffekseerRenderer::ProxyTextureType type);

	UVStyle GetTextureUVStyle() const;

	void SetTextureUVStyle(UVStyle style);

	UVStyle GetBackgroundTextureUVStyle() const;

	void SetBackgroundTextureUVStyle(UVStyle style);

	int32_t GetDrawCallCount() const;

	int32_t GetDrawVertexCount() const;

	void ResetDrawCallCount();

	void ResetDrawVertexCount();

	float GetTime() const;

	void SetTime(float time);

	Effekseer::RenderMode GetRenderMode() const;

	void SetRenderMode(Effekseer::RenderMode renderMode);
};

} // namespace EffekseerRenderer

#endif

#ifndef	__EFFEKSEERRENDERER_RENDERSTATE_BASE_H__
#define	__EFFEKSEERRENDERER_RENDERSTATE_BASE_H__

//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------
#include "EffekseerNative.h"
#include <assert.h>
#include <string.h>
#include <stack>

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
namespace EffekseerRenderer
{
//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
class RenderStateBase
{
public:
	struct State
	{
		bool								DepthTest				: 1;
		bool								DepthWrite				: 1;
		::Effekseer::AlphaBlendType			AlphaBlend;
		::Effekseer::CullingType			CullingType;
		std::array<::Effekseer::TextureFilterType, Effekseer::TextureSlotMax> TextureFilterTypes;
		std::array<::Effekseer::TextureWrapType, Effekseer::TextureSlotMax> TextureWrapTypes;

		//! for OpenGL
		std::array<uint64_t, Effekseer::TextureSlotMax> TextureIDs;

		State();

		void Reset();

		void CopyTo( State& state );
	};

protected:
	std::stack<State>	m_stateStack;
	State				m_active;
	State				m_next;

public:
	RenderStateBase();
	virtual ~RenderStateBase();

	virtual void Update( bool forced ) = 0;

	State& Push();
	void Pop();
	State& GetActiveState();
};

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
}
//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
#endif	// __EFFEKSEERRENDERER_RENDERSTATE_BASE_H__

#ifndef	__EFFEKSEERRENDERER_VERTEXBUFFER_BASE_H__
#define	__EFFEKSEERRENDERER_VERTEXBUFFER_BASE_H__

//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------
#include "EffekseerNative.h"
#include <assert.h>
#include <string.h>

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
namespace EffekseerRenderer
{
//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
class VertexBufferBase
{
protected:
	bool					m_isDynamic;
	int						m_size;
	int						m_offset;
	uint8_t*				m_resource;
	bool					m_isLock;

public:
	VertexBufferBase( int size, bool isDynamic );
	virtual ~VertexBufferBase();

	virtual void Lock() = 0;
	virtual bool RingBufferLock(int32_t size, int32_t& offset, void*& data, int32_t alignment) = 0;

	/**
		@brief	try lock as a ring buffer. if failed, it does't lock it.
	*/
	virtual bool TryRingBufferLock(int32_t size, int32_t& offset, void*& data, int32_t alignment) = 0;

	virtual void Unlock() = 0;
	virtual void Push( const void* buffer, int size );
	virtual int GetMaxSize() const;
	virtual int GetSize() const;
	virtual void* GetBufferDirect( int size );
};

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
}
//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
#endif	// __EFFEKSEERRENDERER_VERTEXBUFFER_BASE_H__

#ifndef	__EFFEKSEERRENDERER_INDEXBUFFER_BASE_H__
#define	__EFFEKSEERRENDERER_INDEXBUFFER_BASE_H__

//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------
#include "EffekseerNative.h"
#include <assert.h>
#include <string.h>

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
namespace EffekseerRenderer
{
//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
class IndexBufferBase
{
protected:
	int						m_indexMaxCount;
	int						m_indexCount;
	bool					m_isDynamic;
	bool					m_isLock;
	uint8_t*				m_resource;
	int32_t stride_ = 2;

public:
	IndexBufferBase( int maxCount, bool isDynamic );
	virtual ~IndexBufferBase();

	virtual void Lock() = 0;
	virtual void Unlock() = 0;
	void Push( const void* buffer, int count );
	int GetCount() const;
	int GetMaxCount() const;
	void* GetBufferDirect( int count );
};

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
}
//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
#endif	// __EFFEKSEERRENDERER_INDEXBUFFER_BASE_H__

#ifndef __EFFEKSEERRENDERER_STANDARD_RENDERER_BASE_H__
#define __EFFEKSEERRENDERER_STANDARD_RENDERER_BASE_H__

#include "EffekseerNative.h"
#include <vector>
#include <algorithm>
#include <functional>


//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
namespace EffekseerRenderer
{
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------

struct StandardRendererState
{
	bool DepthTest;
	bool DepthWrite;
	bool Distortion;
	float DistortionIntensity;
	bool Wireframe;
	bool Refraction;

	::Effekseer::AlphaBlendType AlphaBlend;
	::Effekseer::CullingType CullingType;
	::Effekseer::TextureFilterType TextureFilter1;
	::Effekseer::TextureWrapType TextureWrap1;
	::Effekseer::TextureFilterType TextureFilter2;
	::Effekseer::TextureWrapType TextureWrap2;
#ifdef __EFFEKSEER_BUILD_VERSION16__
	::Effekseer::TextureFilterType TextureFilter3;
	::Effekseer::TextureWrapType TextureWrap3;
#endif
	::Effekseer::TextureData* TexturePtr;
	::Effekseer::TextureData* NormalTexturePtr;
#ifdef __EFFEKSEER_BUILD_VERSION16__
	::Effekseer::TextureData* AlphaTexturePtr;
#endif

#ifdef __EFFEKSEER_BUILD_VERSION16__
	int32_t EnableInterpolation;
	int32_t UVLoopType;
	int32_t InterpolationType;
	int32_t FlipbookDivideX;
	int32_t FlipbookDivideY;
#endif

	::Effekseer::RendererMaterialType MaterialType;
	::Effekseer::MaterialData* MaterialPtr;
	int32_t MaterialUniformCount = 0;
	std::array<std::array<float, 4>, 16> MaterialUniforms;
	int32_t MaterialTextureCount = 0;
	std::array<::Effekseer::TextureData*, 16> MaterialTextures;
	int32_t CustomData1Count = 0;
	int32_t CustomData2Count = 0;

	StandardRendererState()
	{
		DepthTest = false;
		DepthWrite = false;
		Distortion = false;
		DistortionIntensity = 1.0f;
		Wireframe = true;
		Refraction = false;

		AlphaBlend = ::Effekseer::AlphaBlendType::Blend;
		CullingType = ::Effekseer::CullingType::Front;
		TextureFilter1 = ::Effekseer::TextureFilterType::Nearest;
		TextureWrap1 = ::Effekseer::TextureWrapType::Repeat;
		TextureFilter2 = ::Effekseer::TextureFilterType::Nearest;
		TextureWrap2 = ::Effekseer::TextureWrapType::Repeat;
#ifdef __EFFEKSEER_BUILD_VERSION16__
		TextureFilter3 = ::Effekseer::TextureFilterType::Nearest;
		TextureWrap3 = ::Effekseer::TextureWrapType::Repeat;
#endif
		TexturePtr = nullptr;
		NormalTexturePtr = nullptr;
#ifdef __EFFEKSEER_BUILD_VERSION16__
		AlphaTexturePtr = nullptr;
#endif

#ifdef __EFFEKSEER_BUILD_VERSION16__
		EnableInterpolation = 0;
		UVLoopType = 0;
		InterpolationType = 0;
		FlipbookDivideX = 0;
		FlipbookDivideY = 0;
#endif

		MaterialPtr = nullptr;

		MaterialType = ::Effekseer::RendererMaterialType::Default;
		MaterialPtr = nullptr;
		MaterialUniformCount = 0;
		MaterialTextureCount = 0;
		CustomData1Count = 0;
		CustomData2Count = 0;
	}

	bool operator!=(const StandardRendererState state)
	{
		if (DepthTest != state.DepthTest)
			return true;
		if (DepthWrite != state.DepthWrite)
			return true;
		if (Distortion != state.Distortion)
			return true;
		if (DistortionIntensity != state.DistortionIntensity)
			return true;
		if (AlphaBlend != state.AlphaBlend)
			return true;
		if (CullingType != state.CullingType)
			return true;
		if (TextureFilter1 != state.TextureFilter1)
			return true;
		if (TextureWrap1 != state.TextureWrap1)
			return true;
		if (TextureFilter2 != state.TextureFilter2)
			return true;
		if (TextureWrap2 != state.TextureWrap2)
			return true;
#ifdef __EFFEKSEER_BUILD_VERSION16__
		if (TextureFilter3 != state.TextureFilter3)
			return true;
		if (TextureWrap3 != state.TextureWrap3)
			return true;
#endif
		if (TexturePtr != state.TexturePtr)
			return true;
		if (NormalTexturePtr != state.NormalTexturePtr)
			return true;
#ifdef __EFFEKSEER_BUILD_VERSION16__
		if (AlphaTexturePtr != state.AlphaTexturePtr)
			return true;
#endif
#ifdef __EFFEKSEER_BUILD_VERSION16__
		if (EnableInterpolation != state.EnableInterpolation)
			return true;
		if (UVLoopType != state.UVLoopType)
			return true;
		if (InterpolationType != state.InterpolationType)
			return true;
		if (FlipbookDivideX != state.FlipbookDivideX)
			return true;
		if (FlipbookDivideY != state.FlipbookDivideY)
			return true;
#endif
		if (MaterialType != state.MaterialType)
			return true;
		if (MaterialPtr != state.MaterialPtr)
			return true;
		if (MaterialUniformCount != state.MaterialUniformCount)
			return true;
		if (MaterialTextureCount != state.MaterialTextureCount)
			return true;
		if (Refraction != state.Refraction)
			return true;

		for (int32_t i = 0; i < state.MaterialUniformCount; i++)
		{
			if (MaterialUniforms[i] != state.MaterialUniforms[i])
				return true;
		}

		for (int32_t i = 0; i < state.MaterialTextureCount; i++)
		{
			if (MaterialTextures[i] != state.MaterialTextures[i])
				return true;
		}

		if (CustomData1Count != state.CustomData1Count)
			return true;

		if (CustomData1Count != state.CustomData1Count)
			return true;

		return false;
	}

	void CopyMaterialFromParameterToState(Effekseer::Effect* effect, Effekseer::MaterialParameter* materialParam, int32_t colorTextureIndex, int32_t texture2Index
#ifdef __EFFEKSEER_BUILD_VERSION16__
										  , int32_t texture3Index
#endif
	)
	{
		if (materialParam != nullptr)
		{
			if (materialParam->MaterialIndex >= 0 && effect->GetMaterial(materialParam->MaterialIndex) != nullptr)
			{
				MaterialPtr = effect->GetMaterial(materialParam->MaterialIndex);

				CustomData1Count = MaterialPtr->CustomData1;
				CustomData2Count = MaterialPtr->CustomData2;

				MaterialUniformCount = static_cast<int32_t>(Effekseer::Min(materialParam->MaterialUniforms.size(), MaterialUniforms.size()));
				for (size_t i = 0; i < MaterialUniformCount; i++)
				{
					MaterialUniforms[i] = materialParam->MaterialUniforms[i];
				}

				MaterialTextureCount = static_cast<int32_t>(Effekseer::Min(materialParam->MaterialTextures.size(), MaterialTextures.size()));
				for (size_t i = 0; i < MaterialTextureCount; i++)
				{
					if (materialParam->MaterialTextures[i].Type == 1)
					{
						if (materialParam->MaterialTextures[i].Index >= 0)
						{
							MaterialTextures[i] = effect->GetNormalImage(materialParam->MaterialTextures[i].Index);
						}
						else
						{
							MaterialTextures[i] = nullptr;
						}
					}
					else
					{
						if (materialParam->MaterialTextures[i].Index >= 0)
						{
							MaterialTextures[i] = effect->GetColorImage(materialParam->MaterialTextures[i].Index);
						}
						else
						{
							MaterialTextures[i] = nullptr;
						}
					}
				}
			}
		}
		else
		{
			if (colorTextureIndex >= 0)
			{
				if (Distortion)
				{
					TexturePtr = effect->GetDistortionImage(colorTextureIndex);
				}
				else
				{
					TexturePtr = effect->GetColorImage(colorTextureIndex);
				}
			}
			else
			{
				TexturePtr = nullptr;
			}

			if (texture2Index >= 0)
			{
				NormalTexturePtr = effect->GetNormalImage(texture2Index);
			}
			else
			{
				NormalTexturePtr = nullptr;
			}

#ifdef __EFFEKSEER_BUILD_VERSION16__
			if (texture3Index >= 0)
			{
				if (Distortion)
				{
					AlphaTexturePtr = effect->GetDistortionImage(texture3Index);
				}
				else
				{
					AlphaTexturePtr = effect->GetColorImage(texture3Index);
				}
			}
			else
			{
				AlphaTexturePtr = nullptr;
			}
#endif

			Refraction = false;
			CustomData1Count = 0;
			CustomData2Count = 0;
		}
	}
};

template <typename RENDERER, typename SHADER, typename VERTEX, typename VERTEX_DISTORTION> class StandardRenderer
{
private:
	RENDERER* m_renderer;
	SHADER* m_shader;
	//SHADER* m_shader_no_texture;

	SHADER* m_shader_distortion;
	//SHADER* m_shader_no_texture_distortion;

	Effekseer::TextureData* m_texture;

	StandardRendererState m_state;

	std::vector<uint8_t> vertexCaches;
	int32_t squareMaxSize_ = 0;

	bool isDistortionMode_;
	bool isDynamicVertexMode_ = false;

	struct VertexConstantBuffer
	{
		Effekseer::Matrix44 constantVSBuffer[2];
		float uvInversed[4];

#ifdef __EFFEKSEER_BUILD_VERSION16__
		struct
		{
			union
			{
				float Buffer[4];

				struct
				{
					float enableInterpolation;
					float loopType;
					float divideX;
					float divideY;
				};
			};
		} flipbookParameter;
#endif
	};

#ifdef __EFFEKSEER_BUILD_VERSION16__
	struct PixelConstantBuffer
	{
		struct
		{
			union
			{
				float Buffer[4];

				struct
				{
					float enableInterpolation;
					float interpolationType;
				};
			};
		} flipbookParameter;
	};
#endif

	struct DistortionPixelConstantBuffer
	{
		float scale[4];
		float uvInversed[4];

#ifdef __EFFEKSEER_BUILD_VERSION16__
		struct
		{
			union
			{
				float Buffer[4];

				struct
				{
					float enableInterpolation;
					float interpolationType;
				};
			};
		} flipbookParameter;
#endif
	};

	void ColorToFloat4(::Effekseer::Color color, float fc[4])
	{
		fc[0] = color.R / 255.0f;
		fc[1] = color.G / 255.0f;
		fc[2] = color.B / 255.0f;
		fc[3] = color.A / 255.0f;
	}

	void VectorToFloat4(const ::Effekseer::Vec3f& v, float fc[4])
	{
		::Effekseer::SIMD4f::Store3(fc, v.s);
		fc[3] = 1.0f;
	}

public:
	StandardRenderer(
		RENDERER* renderer, SHADER* shader, SHADER* shader_distortion)
		: squareMaxSize_(renderer->GetSquareMaxCount()), isDistortionMode_(false)
	{
		m_renderer = renderer;
		m_shader = shader;
		m_shader_distortion = shader_distortion;

		vertexCaches.reserve(m_renderer->GetVertexBuffer()->GetMaxSize());
	}

	virtual ~StandardRenderer()
	{
	}

	int32_t CalculateCurrentStride() const
	{
		int32_t stride = 0;
		if (isDynamicVertexMode_)
		{
			stride = (int32_t)sizeof(DynamicVertex);
			stride += (m_state.CustomData1Count + m_state.CustomData2Count) * sizeof(float);
		}
		else if (isDistortionMode_)
		{
			stride = (int32_t)sizeof(VERTEX_DISTORTION);
		}
		else
		{
			stride = (int32_t)sizeof(VERTEX);
		}
		return stride;
	}

	void UpdateStateAndRenderingIfRequired(StandardRendererState state)
	{
		if (m_state != state)
		{
			Rendering();
		}

		m_state = state;

		isDynamicVertexMode_ = (m_state.MaterialPtr != nullptr && !m_state.MaterialPtr->IsSimpleVertex) || 
			m_state.MaterialType == ::Effekseer::RendererMaterialType::Lighting;
		isDistortionMode_ = m_state.Distortion;
	}

	void BeginRenderingAndRenderingIfRequired(int32_t count, int& stride, void*& data)
	{
		stride = CalculateCurrentStride();

		{
			int32_t renderVertexMaxSize = squareMaxSize_ * stride * 4;

			if (count * stride + (int32_t)vertexCaches.size() > renderVertexMaxSize)
			{
				Rendering();
			}

			auto old = vertexCaches.size();
			vertexCaches.resize(count * stride + vertexCaches.size());
			data = (vertexCaches.data() + old);
		}
	}

	void ResetAndRenderingIfRequired()
	{
		Rendering();

		// It is always initialized with the next drawing.
		m_state.TexturePtr = (Effekseer::TextureData*)0x1;
		m_state.NormalTexturePtr = (Effekseer::TextureData*)0x1;
#ifdef __EFFEKSEER_BUILD_VERSION16__
		m_state.AlphaTexturePtr = (Effekseer::TextureData*)0x1;
#endif
	}

	const StandardRendererState& GetState() { return m_state; }

	void Rendering(const Effekseer::Mat44f& mCamera, const Effekseer::Mat44f& mProj)
	{
		if (vertexCaches.size() == 0)
			return;

		int32_t stride = CalculateCurrentStride();

		int32_t passNum = 1;

		if (m_state.MaterialPtr != nullptr)
		{
			if(m_state.MaterialPtr->RefractionUserPtr != nullptr)
			{
				// refraction and standard
				passNum = 2;
			}
		}

		for (int32_t passInd = 0; passInd < passNum; passInd++)
		{
			int32_t offset = 0;

			while (true)
			{
				// only sprite
				int32_t renderBufferSize = (int32_t)vertexCaches.size() - offset;

				int32_t renderVertexMaxSize = squareMaxSize_ * stride * 4;

				if (renderBufferSize > renderVertexMaxSize)
				{
					renderBufferSize =
						(Effekseer::Min(renderVertexMaxSize, (int32_t)vertexCaches.size() - offset) / (stride * 4)) * (stride * 4);
				}

				Rendering_(mCamera, mProj, offset, renderBufferSize, stride, passInd);

				offset += renderBufferSize;

				if (offset == vertexCaches.size())
					break;
			}
		}

		vertexCaches.clear();
	}

	void Rendering_(const Effekseer::Mat44f& mCamera, const Effekseer::Mat44f& mProj, int32_t bufferOffset, int32_t bufferSize, int32_t stride, int32_t renderPass)
	{
		bool isBackgroundRequired = false;
		
		isBackgroundRequired |= m_state.Distortion;
		isBackgroundRequired |=
			(m_state.MaterialPtr != nullptr && m_state.MaterialPtr->IsRefractionRequired && renderPass == 0);

		if (isBackgroundRequired)
		{
			auto callback = m_renderer->GetDistortingCallback();
			if (callback != nullptr)
			{
				if (!callback->OnDistorting())
				{
					return;
				}
			}
		}

		if (isBackgroundRequired && m_renderer->GetBackground() == 0)
		{
			return;
		}

		int32_t vertexSize = bufferSize;
		int32_t vbOffset = 0;
		{
			VertexBufferBase* vb = m_renderer->GetVertexBuffer();

			void* vbData = nullptr;

			if (vb->RingBufferLock(vertexSize, vbOffset, vbData, stride * 4))
			{
				assert(vbData != nullptr);
				memcpy(vbData, vertexCaches.data() + bufferOffset, vertexSize);
				vb->Unlock();
			}
			else
			{
				return;
			}
		}

		SHADER* shader_ = nullptr;

		bool distortion = m_state.Distortion;
		bool renderDistortedBackground = false;

		if (m_state.MaterialPtr != nullptr)
		{
			if (m_state.MaterialPtr->IsRefractionRequired)
			{
				if (renderPass == 0)
				{
					if (m_renderer->GetBackground() == 0)
					{
						return;
					}

					shader_ = (SHADER*)m_state.MaterialPtr->RefractionUserPtr;
					renderDistortedBackground = true;
				}
				else
				{
					shader_ = (SHADER*)m_state.MaterialPtr->UserPtr;
				}
			}
			else
			{
				shader_ = (SHADER*)m_state.MaterialPtr->UserPtr;
			}
			
			// validate
			if (shader_ == nullptr)
				return;
			
			if (m_state.MaterialPtr->UniformCount != m_state.MaterialUniformCount)
				return;

			if (m_state.MaterialPtr->TextureCount != m_state.MaterialTextureCount)
				return;
		}
		else
		{
			shader_ = m_renderer->GetShader(true, m_state.MaterialType);
		}

		RenderStateBase::State& state = m_renderer->GetRenderState()->Push();
		state.DepthTest = m_state.DepthTest;
		state.DepthWrite = m_state.DepthWrite;
		state.CullingType = m_state.CullingType;
		state.AlphaBlend = m_state.AlphaBlend;

		if (renderDistortedBackground)
		{
			state.AlphaBlend = ::Effekseer::AlphaBlendType::Blend;
		}

		m_renderer->BeginShader(shader_);

		if (m_state.MaterialPtr != nullptr)
		{
			std::array<Effekseer::TextureData*, 16> textures;
			textures.fill(nullptr);
			int32_t textureCount = Effekseer::Min(m_state.MaterialTextureCount, textures.size() - 1);

			if (m_state.MaterialTextureCount > 0)
			{
				auto textureSize = static_cast<int32_t>(textures.size());

				for (size_t i = 0; i < Effekseer::Min(m_state.MaterialTextureCount, textureSize); i++)
				{
					textures[i] = m_state.MaterialTextures[i];
					state.TextureFilterTypes[i] = Effekseer::TextureFilterType::Linear;
					state.TextureWrapTypes[i] = m_state.MaterialPtr->TextureWrapTypes[i];
				}
			}

			if (m_renderer->GetBackground() != 0)
			{
				textures[textureCount] = m_renderer->GetBackground();
				state.TextureFilterTypes[textureCount] = Effekseer::TextureFilterType::Linear;
				state.TextureWrapTypes[textureCount] = Effekseer::TextureWrapType::Clamp;
				textureCount += 1;
			}

			m_renderer->SetTextures(shader_, textures.data(), textureCount);
		}
		else
		{
            state.TextureFilterTypes[0] = m_state.TextureFilter1;
            state.TextureWrapTypes[0] = m_state.TextureWrap1;

            if (distortion)
            {
                state.TextureFilterTypes[1] = Effekseer::TextureFilterType::Linear;
                state.TextureWrapTypes[1] = Effekseer::TextureWrapType::Clamp;

#ifdef __EFFEKSEER_BUILD_VERSION16__
                state.TextureFilterTypes[2] = m_state.TextureFilter3;
                state.TextureWrapTypes[2] = m_state.TextureWrap3;
#endif
            }
            else
            {
#ifdef __EFFEKSEER_BUILD_VERSION16__
                if (m_state.MaterialType == ::Effekseer::RendererMaterialType::Lighting)
                {
                    state.TextureFilterTypes[1] = m_state.TextureFilter2;
                    state.TextureWrapTypes[1] = m_state.TextureWrap2;

                    state.TextureFilterTypes[2] = m_state.TextureFilter3;
                    state.TextureWrapTypes[2] = m_state.TextureWrap3;
                }
                else
                {
                    state.TextureFilterTypes[1] = m_state.TextureFilter3;
                    state.TextureWrapTypes[1] = m_state.TextureWrap3;
                }
#else
                state.TextureFilterTypes[1] = m_state.TextureFilter2;
                state.TextureWrapTypes[1] = m_state.TextureWrap2;
#endif
            }
            
#ifdef __EFFEKSEER_BUILD_VERSION16__
			std::array<Effekseer::TextureData*, 3> textures;
#else
			std::array<Effekseer::TextureData*, 2> textures;
#endif
			textures.fill(nullptr);

			if (m_state.TexturePtr != nullptr && m_state.TexturePtr != (Effekseer::TextureData*)0x01 && m_renderer->GetRenderMode() == Effekseer::RenderMode::Normal)
			{
				textures[0] = m_state.TexturePtr;
			}
			else
			{
				textures[0] = m_renderer->GetImpl()->GetProxyTexture(EffekseerRenderer::ProxyTextureType::White);
			}

			if (m_state.MaterialType == ::Effekseer::RendererMaterialType::Lighting)
			{
				textures[1] = m_renderer->GetImpl()->GetProxyTexture(EffekseerRenderer::ProxyTextureType::Normal);

				if (m_state.NormalTexturePtr != nullptr && m_state.NormalTexturePtr != (Effekseer::TextureData*)0x01)
				{
					textures[1] = m_state.NormalTexturePtr;
				}

#ifdef __EFFEKSEER_BUILD_VERSION16__
				if (m_state.AlphaTexturePtr != nullptr && m_state.AlphaTexturePtr != (Effekseer::TextureData*)0x01)
				{
					textures[2] = m_state.AlphaTexturePtr;
				}
				else
				{
					textures[2] = m_renderer->GetImpl()->GetProxyTexture(EffekseerRenderer::ProxyTextureType::White);
				}

				m_renderer->SetTextures(shader_, textures.data(), 3);
#else
				m_renderer->SetTextures(shader_, textures.data(), 2);
#endif
			}
			else if (distortion)
			{
				textures[1] = m_renderer->GetBackground();

#ifdef __EFFEKSEER_BUILD_VERSION16__
				if (m_state.AlphaTexturePtr != nullptr && m_state.AlphaTexturePtr != (Effekseer::TextureData*)0x01)
				{
					textures[2] = m_state.AlphaTexturePtr;
				}
				else
				{
					textures[2] = m_renderer->GetImpl()->GetProxyTexture(EffekseerRenderer::ProxyTextureType::White);
				}

				m_renderer->SetTextures(shader_, textures.data(), 3);
#else
				m_renderer->SetTextures(shader_, textures.data(), 2);
#endif
			}
			else
			{
#ifdef __EFFEKSEER_BUILD_VERSION16__
				if (m_state.AlphaTexturePtr != nullptr && m_state.AlphaTexturePtr != (Effekseer::TextureData*)0x01)
				{
					textures[1] = m_state.AlphaTexturePtr;
				}
				else
				{
					textures[1] = m_renderer->GetImpl()->GetProxyTexture(EffekseerRenderer::ProxyTextureType::White);
				}

				m_renderer->SetTextures(shader_, textures.data(), 2);
#else
				m_renderer->SetTextures(shader_, textures.data(), 1);
#endif
			}
		}

		std::array<float, 4> uvInversed;
		std::array<float, 4> uvInversedBack;
		std::array<float, 4> uvInversedMaterial;
		
		if (m_renderer->GetTextureUVStyle() == UVStyle::VerticalFlipped)
		{
			uvInversed[0] = 1.0f;
			uvInversed[1] = -1.0f;
		}
		else
		{
			uvInversed[0] = 0.0f;
			uvInversed[1] = 1.0f;
		}

		if (m_renderer->GetBackgroundTextureUVStyle() == UVStyle::VerticalFlipped)
		{
			uvInversedBack[0] = 1.0f;
			uvInversedBack[1] = -1.0f;
		}
		else
		{
			uvInversedBack[0] = 0.0f;
			uvInversedBack[1] = 1.0f;
		}

		uvInversedMaterial[0] = uvInversed[0];
		uvInversedMaterial[1] = uvInversed[1];
		uvInversedMaterial[2] = uvInversedBack[0];
		uvInversedMaterial[3] = uvInversedBack[1];

		if (m_state.MaterialPtr != nullptr)
		{
			Effekseer::Matrix44 mstCamera = ToStruct(mCamera);
			Effekseer::Matrix44 mstProj = ToStruct(mProj);

			// camera
			float cameraPosition[4];
			::Effekseer::Vec3f cameraPosition3 = m_renderer->GetCameraPosition();
			VectorToFloat4(cameraPosition3, cameraPosition);
			// time
			std::array<float, 4> predefined_uniforms;
			predefined_uniforms.fill(0.5f);
			predefined_uniforms[0] = m_renderer->GetTime();

			// vs
			int32_t vsOffset = 0;
			m_renderer->SetVertexBufferToShader(&mstCamera, sizeof(Effekseer::Matrix44), vsOffset);
			vsOffset += sizeof(Effekseer::Matrix44);

			m_renderer->SetVertexBufferToShader(&mstProj, sizeof(Effekseer::Matrix44), vsOffset);
			vsOffset += sizeof(Effekseer::Matrix44);

			m_renderer->SetVertexBufferToShader(uvInversedMaterial.data(), sizeof(float) * 4, vsOffset);
			vsOffset += (sizeof(float) * 4);

			m_renderer->SetVertexBufferToShader(predefined_uniforms.data(), sizeof(float) * 4, vsOffset);
			vsOffset += (sizeof(float) * 4);
			
			m_renderer->SetVertexBufferToShader(cameraPosition, sizeof(float) * 4, vsOffset);
			vsOffset += (sizeof(float) * 4);
			
			for (size_t i = 0; i < m_state.MaterialUniformCount; i++)
			{
				m_renderer->SetVertexBufferToShader(m_state.MaterialUniforms[i].data(), sizeof(float) * 4, vsOffset);
				vsOffset += (sizeof(float) * 4);
			}
			
			// ps
			int32_t psOffset = 0;
			m_renderer->SetPixelBufferToShader(uvInversedMaterial.data(), sizeof(float) * 4, psOffset);
			psOffset += (sizeof(float) * 4);

			m_renderer->SetPixelBufferToShader(predefined_uniforms.data(), sizeof(float) * 4, psOffset);
			psOffset += (sizeof(float) * 4);

			m_renderer->SetPixelBufferToShader(cameraPosition, sizeof(float) * 4, psOffset);
			psOffset += (sizeof(float) * 4);

			// shader model
			if (m_state.MaterialPtr->ShadingModel == ::Effekseer::ShadingModelType::Lit)
			{
				
				float lightDirection[4];
				float lightColor[4];
				float lightAmbientColor[4];

				::Effekseer::Vec3f lightDirection3 = m_renderer->GetLightDirection();
				lightDirection3 = lightDirection3.Normalize();
				VectorToFloat4(lightDirection3, lightDirection);
				ColorToFloat4(m_renderer->GetLightColor(), lightColor);
				ColorToFloat4(m_renderer->GetLightAmbientColor(), lightAmbientColor);

				m_renderer->SetPixelBufferToShader(lightDirection, sizeof(float) * 4, psOffset);
				psOffset += (sizeof(float) * 4);

				m_renderer->SetPixelBufferToShader(lightColor, sizeof(float) * 4, psOffset);
				psOffset += (sizeof(float) * 4);

				m_renderer->SetPixelBufferToShader(lightAmbientColor, sizeof(float) * 4, psOffset);
				psOffset += (sizeof(float) * 4);

			}

			// refraction
			if (m_state.MaterialPtr->RefractionUserPtr != nullptr && renderPass == 0)
			{
				auto mat = m_renderer->GetCameraMatrix();
				m_renderer->SetPixelBufferToShader(&mat, sizeof(float) * 16, psOffset);
				psOffset += (sizeof(float) * 16);
			}


			for (size_t i = 0; i < m_state.MaterialUniformCount; i++)
			{
				m_renderer->SetPixelBufferToShader(m_state.MaterialUniforms[i].data(), sizeof(float) * 4, psOffset);
				psOffset += (sizeof(float) * 4);
			}
		}
		else if (m_state.MaterialType == ::Effekseer::RendererMaterialType::Lighting)
		{
			VertexConstantBuffer vcb;
			vcb.constantVSBuffer[0] = ToStruct(mCamera);
			vcb.constantVSBuffer[1] = ToStruct(mProj);
			vcb.uvInversed[0] = uvInversed[0];
			vcb.uvInversed[1] = uvInversed[1];

#ifdef __EFFEKSEER_BUILD_VERSION16__
			vcb.flipbookParameter.enableInterpolation = static_cast<float>(m_state.EnableInterpolation);
			vcb.flipbookParameter.loopType = static_cast<float>(m_state.UVLoopType);
			vcb.flipbookParameter.divideX = static_cast<float>(m_state.FlipbookDivideX);
			vcb.flipbookParameter.divideY = static_cast<float>(m_state.FlipbookDivideY);
#endif

			m_renderer->SetVertexBufferToShader(&vcb, sizeof(VertexConstantBuffer), 0);

			// ps
			int32_t psOffset = 0;
			float lightDirection[4];
			float lightColor[4];
			float lightAmbientColor[4];

			::Effekseer::Vec3f lightDirection3 = m_renderer->GetLightDirection();
			lightDirection3 = lightDirection3.Normalize();
			VectorToFloat4(lightDirection3, lightDirection);
			ColorToFloat4(m_renderer->GetLightColor(), lightColor);
			ColorToFloat4(m_renderer->GetLightAmbientColor(), lightAmbientColor);

			m_renderer->SetPixelBufferToShader(lightDirection, sizeof(float) * 4, psOffset);
			psOffset += (sizeof(float) * 4);

			m_renderer->SetPixelBufferToShader(lightColor, sizeof(float) * 4, psOffset);
			psOffset += (sizeof(float) * 4);

			m_renderer->SetPixelBufferToShader(lightAmbientColor, sizeof(float) * 4, psOffset);
			psOffset += (sizeof(float) * 4);
			
#ifdef __EFFEKSEER_BUILD_VERSION16__
			PixelConstantBuffer pcb;
			pcb.flipbookParameter.enableInterpolation = static_cast<float>(m_state.EnableInterpolation);
			pcb.flipbookParameter.interpolationType = static_cast<float>(m_state.InterpolationType);

			m_renderer->SetPixelBufferToShader(&pcb.flipbookParameter, sizeof(float) * 4, psOffset);
#endif
		}
		else
		{
			VertexConstantBuffer vcb;
			vcb.constantVSBuffer[0] = ToStruct(mCamera);
			vcb.constantVSBuffer[1] = ToStruct(mProj);
			vcb.uvInversed[0] = uvInversed[0];
			vcb.uvInversed[1] = uvInversed[1];
			vcb.uvInversed[2] = 0.0f;
			vcb.uvInversed[3] = 0.0f;

#ifdef __EFFEKSEER_BUILD_VERSION16__
			vcb.flipbookParameter.enableInterpolation = static_cast<float>(m_state.EnableInterpolation);
			vcb.flipbookParameter.loopType = static_cast<float>(m_state.UVLoopType);
			vcb.flipbookParameter.divideX = static_cast<float>(m_state.FlipbookDivideX);
			vcb.flipbookParameter.divideY = static_cast<float>(m_state.FlipbookDivideY);
#endif

			m_renderer->SetVertexBufferToShader(&vcb, sizeof(VertexConstantBuffer), 0);

			if (distortion)
			{
				DistortionPixelConstantBuffer pcb;
				pcb.scale[0] = m_state.DistortionIntensity;
				pcb.uvInversed[0] = uvInversedBack[0];
				pcb.uvInversed[1] = uvInversedBack[1];

#ifdef __EFFEKSEER_BUILD_VERSION16__
				pcb.flipbookParameter.enableInterpolation = static_cast<float>(m_state.EnableInterpolation);
				pcb.flipbookParameter.interpolationType = static_cast<float>(m_state.InterpolationType);
#endif

				m_renderer->SetPixelBufferToShader(&pcb, sizeof(DistortionPixelConstantBuffer), 0);
			}
#ifdef __EFFEKSEER_BUILD_VERSION16__
			else
			{
				PixelConstantBuffer pcb;
				pcb.flipbookParameter.enableInterpolation = static_cast<float>(m_state.EnableInterpolation);
				pcb.flipbookParameter.interpolationType = static_cast<float>(m_state.InterpolationType);

				m_renderer->SetPixelBufferToShader(&pcb, sizeof(PixelConstantBuffer), 0);
			}
#endif
		}

		shader_->SetConstantBuffer();

		m_renderer->GetRenderState()->Update(distortion);

		assert(vbOffset % stride == 0);

		m_renderer->SetVertexBuffer(m_renderer->GetVertexBuffer(), stride);
		m_renderer->SetIndexBuffer(m_renderer->GetIndexBuffer());
		m_renderer->SetLayout(shader_);
		m_renderer->DrawSprites(vertexSize / stride / 4, vbOffset / stride);

		m_renderer->EndShader(shader_);

		m_renderer->GetRenderState()->Pop();
	}

	void Rendering() { Rendering(m_renderer->GetCameraMatrix(), m_renderer->GetProjectionMatrix()); }
};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
} // namespace EffekseerRenderer
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#endif // __EFFEKSEERRENDERER_STANDARD_RENDERER_H__


#ifndef	__EFFEKSEERRENDERER_MODEL_RENDERER_BASE_H__
#define	__EFFEKSEERRENDERER_MODEL_RENDERER_BASE_H__

#include "EffekseerNative.h"

#include <assert.h>
#include <string.h>
#include <algorithm>
#include <vector>


//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
namespace EffekseerRenderer
{
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
typedef ::Effekseer::ModelRenderer::NodeParameter efkModelNodeParam;
typedef ::Effekseer::ModelRenderer::InstanceParameter efkModelInstanceParam;
typedef ::Effekseer::Vec3f efkVector3D;

template<int MODEL_COUNT>
 struct ModelRendererVertexConstantBuffer
{
	Effekseer::Matrix44		CameraMatrix;
	Effekseer::Matrix44		ModelMatrix[MODEL_COUNT];
	float	ModelUV[MODEL_COUNT][4];
#ifdef __EFFEKSEER_BUILD_VERSION16__
	float	ModelAlphaUV[MODEL_COUNT][4];

	struct
	{
		union
		{
			float Buffer[4];

			struct
			{
				float EnableInterpolation;
				float LoopType;
				float DivideX;
				float DivideY;
			};
		};
	} ModelFlipbookParameter;
	
	float	ModelFlipbookIndexAndNextRate[MODEL_COUNT][4];

	float	ModelAlphaThreshold[MODEL_COUNT][4];

#endif
	float	ModelColor[MODEL_COUNT][4];

	float	LightDirection[4];
	float	LightColor[4];
	float	LightAmbientColor[4];
	float	UVInversed[4];
};

struct ModelRendererPixelConstantBuffer
{
	float	LightDirection[4];
	float	LightColor[4];
	float	LightAmbientColor[4];

#ifdef __EFFEKSEER_BUILD_VERSION16__
	struct
	{
		union
		{
			float Buffer[4];

			struct
			{
				float EnableInterpolation;
				float InterpolationType;
			};
		};
	} ModelFlipbookParameter;
#endif
};

class ModelRendererBase
	: public ::Effekseer::ModelRenderer
	, public ::Effekseer::AlignedAllocationPolicy<16>
{
protected:
	struct KeyValue
	{
		float Key;
		int Value;
	};

	std::vector<KeyValue> keyValues_;
	
	std::vector<Effekseer::Matrix44> matrixesSorted_;
	std::vector<Effekseer::RectF> uvSorted_;
#ifdef __EFFEKSEER_BUILD_VERSION16__
	std::vector<Effekseer::RectF> alphaUVSorted_;
	std::vector<float> flipbookIndexAndNextRateSorted_;
	std::vector<float> alphaThresholdSorted_;
#endif
	std::vector<Effekseer::Color> colorsSorted_;
	std::vector<int32_t> timesSorted_;
	std::vector<std::array<float, 4>> customData1Sorted_;
	std::vector<std::array<float, 4>> customData2Sorted_;

	std::vector<Effekseer::Matrix44> m_matrixes;
	std::vector<Effekseer::RectF> m_uv;
#ifdef __EFFEKSEER_BUILD_VERSION16__
	std::vector<Effekseer::RectF> m_alphaUV;
	std::vector<float> m_flipbookIndexAndNextRate;
	std::vector<float> m_alphaThreshold;
#endif
	std::vector<Effekseer::Color> m_colors;
	std::vector<int32_t> m_times;
	std::vector<std::array<float, 4>> customData1_;
	std::vector<std::array<float, 4>> customData2_;

	int32_t customData1Count_ = 0;
	int32_t customData2Count_ = 0;

	void ColorToFloat4(::Effekseer::Color color, float fc[4])
	{
		fc[0] = color.R / 255.0f;
		fc[1] = color.G / 255.0f;
		fc[2] = color.B / 255.0f;
		fc[3] = color.A / 255.0f;
	}
	
	void VectorToFloat4(const ::Effekseer::Vec3f& v, float fc[4])
	{
		::Effekseer::SIMD4f::Store3(fc, v.s);
		fc[3] = 1.0f;
	}

	ModelRendererBase()
	{
	}

	template <typename RENDERER>
	void GetInversedFlags(RENDERER* renderer, std::array<float, 4>& uvInversed, std::array<float, 4>& uvInversedBack)
	{
		if (renderer->GetTextureUVStyle() == UVStyle::VerticalFlipped)
		{
			uvInversed[0] = 1.0f;
			uvInversed[1] = -1.0f;
		}
		else
		{
			uvInversed[0] = 0.0f;
			uvInversed[1] = 1.0f;
		}

		if (renderer->GetBackgroundTextureUVStyle() == UVStyle::VerticalFlipped)
		{
			uvInversedBack[0] = 1.0f;
			uvInversedBack[1] = -1.0f;
		}
		else
		{
			uvInversedBack[0] = 0.0f;
			uvInversedBack[1] = 1.0f;
		}
	}

	template <typename RENDERER>
	void SortTemporaryValues(RENDERER* renderer, const efkModelNodeParam& param)
	{
		if (param.DepthParameterPtr->ZSort != Effekseer::ZSortType::None)
		{
			keyValues_.resize(m_matrixes.size());
			for (size_t i = 0; i < keyValues_.size(); i++)
			{
				efkVector3D t(
					m_matrixes[i].Values[3][0],
					m_matrixes[i].Values[3][1],
					m_matrixes[i].Values[3][2]);

				auto frontDirection = renderer->GetCameraFrontDirection();
				if (!param.IsRightHand)
				{
					frontDirection.Z = -frontDirection.Z;
				}

				keyValues_[i].Key = Effekseer::Vec3f::Dot(t, frontDirection);
				keyValues_[i].Value = static_cast<int32_t>(i);
			}

			if (param.DepthParameterPtr->ZSort == Effekseer::ZSortType::NormalOrder)
			{
				std::sort(keyValues_.begin(), keyValues_.end(), [](const KeyValue& a, const KeyValue& b) -> bool { return a.Key < b.Key; });
			}
			else
			{
				std::sort(keyValues_.begin(), keyValues_.end(), [](const KeyValue& a, const KeyValue& b) -> bool { return a.Key > b.Key; });
			}

			matrixesSorted_.resize(m_matrixes.size());
			uvSorted_.resize(m_matrixes.size());
#ifdef __EFFEKSEER_BUILD_VERSION16__
			alphaUVSorted_.resize(m_matrixes.size());
			flipbookIndexAndNextRateSorted_.resize(m_matrixes.size());
#endif
			colorsSorted_.resize(m_matrixes.size());
			timesSorted_.resize(m_matrixes.size());

			if (customData1Count_ > 0)
			{
				customData1Sorted_.resize(m_matrixes.size());
			}

			if (customData2Count_ > 0)
			{
				customData2Sorted_.resize(m_matrixes.size());
			}

			for (size_t i = 0; i < keyValues_.size(); i++)
			{
				matrixesSorted_[keyValues_[i].Value] = m_matrixes[i];
				uvSorted_[keyValues_[i].Value] = m_uv[i];
#ifdef __EFFEKSEER_BUILD_VERSION16__
				alphaUVSorted_[keyValues_[i].Value] = m_alphaUV[i];
				flipbookIndexAndNextRateSorted_[keyValues_[i].Value] = m_flipbookIndexAndNextRate[i];
				alphaThresholdSorted_[keyValues_[i].Value] = m_alphaThreshold[i];
#endif
				colorsSorted_[keyValues_[i].Value] = m_colors[i];
				timesSorted_[keyValues_[i].Value] = m_times[i];
			}

			if (customData1Count_ > 0)
			{
				for (size_t i = 0; i < keyValues_.size(); i++)
				{
					customData1Sorted_[keyValues_[i].Value] = customData1_[i];
				}
			}

			if (customData2Count_ > 0)
			{
				for (size_t i = 0; i < keyValues_.size(); i++)
				{
					customData2Sorted_[keyValues_[i].Value] = customData2_[i];
				}
			}

			m_matrixes = matrixesSorted_;
			m_uv = uvSorted_;
#ifdef __EFFEKSEER_BUILD_VERSION16__
			m_alphaUV = alphaUVSorted_;
			m_flipbookIndexAndNextRate = flipbookIndexAndNextRateSorted_;
			m_alphaThreshold = alphaThresholdSorted_;
#endif
			m_colors = colorsSorted_;
			m_times = timesSorted_;
			customData1_ = customData1Sorted_;
			customData2_ = customData2Sorted_;
		}
	}

	template <typename RENDERER, typename SHADER, int InstanceCount>
	void StoreFileUniform(RENDERER* renderer,
						  SHADER* shader_,
						  Effekseer::MaterialData* material,
						  Effekseer::MaterialParameter* materialParam,
						  const efkModelNodeParam& param,
						  int32_t renderPassInd,
						  float*& cutomData1Ptr,
						  float*& cutomData2Ptr)
	{
		std::array<float, 4> uvInversed;
		std::array<float, 4> uvInversedBack;
		cutomData1Ptr = nullptr;
		cutomData2Ptr = nullptr;

		GetInversedFlags(renderer, uvInversed, uvInversedBack);

		std::array<float, 4> uvInversedMaterial;
		uvInversedMaterial[0] = uvInversed[0];
		uvInversedMaterial[1] = uvInversed[1];
		uvInversedMaterial[2] = uvInversedBack[0];
		uvInversedMaterial[3] = uvInversedBack[1];

		// camera
		float cameraPosition[4];
		::Effekseer::Vec3f cameraPosition3 = renderer->GetCameraPosition();
		VectorToFloat4(cameraPosition3, cameraPosition);

		// time
		std::array<float, 4> predefined_uniforms;
		predefined_uniforms.fill(0.5f);
		predefined_uniforms[0] = renderer->GetTime();

		// vs
		int32_t vsOffset = sizeof(Effekseer::Matrix44) + (sizeof(Effekseer::Matrix44) + sizeof(float) * 4 * 2) * InstanceCount;

		renderer->SetVertexBufferToShader(uvInversedMaterial.data(), sizeof(float) * 4, vsOffset);
		vsOffset += (sizeof(float) * 4);

		renderer->SetVertexBufferToShader(predefined_uniforms.data(), sizeof(float) * 4, vsOffset);
		vsOffset += (sizeof(float) * 4);

		renderer->SetVertexBufferToShader(cameraPosition, sizeof(float) * 4, vsOffset);
		vsOffset += (sizeof(float) * 4);

		// vs - custom data
		if (customData1Count_ > 0)
		{
			cutomData1Ptr = (float*)((uint8_t*)shader_->GetVertexConstantBuffer() + vsOffset);
			vsOffset += (sizeof(float) * 4) * InstanceCount;
		}

		if (customData2Count_ > 0)
		{
			cutomData2Ptr = (float*)((uint8_t*)shader_->GetVertexConstantBuffer() + vsOffset);
			vsOffset += (sizeof(float) * 4) * InstanceCount;
		}

		for (size_t i = 0; i < materialParam->MaterialUniforms.size(); i++)
		{
			renderer->SetVertexBufferToShader(materialParam->MaterialUniforms[i].data(), sizeof(float) * 4, vsOffset);
			vsOffset += (sizeof(float) * 4);
		}

		// ps
		int32_t psOffset = 0;
		renderer->SetPixelBufferToShader(uvInversedMaterial.data(), sizeof(float) * 4, psOffset);
		psOffset += (sizeof(float) * 4);

		renderer->SetPixelBufferToShader(predefined_uniforms.data(), sizeof(float) * 4, psOffset);
		psOffset += (sizeof(float) * 4);

		renderer->SetPixelBufferToShader(cameraPosition, sizeof(float) * 4, psOffset);
		psOffset += (sizeof(float) * 4);

		// shader model
		material = param.EffectPointer->GetMaterial(materialParam->MaterialIndex);

		if (material->ShadingModel == ::Effekseer::ShadingModelType::Lit)
		{
			float lightDirection[4];
			float lightColor[4];
			float lightAmbientColor[4];

			::Effekseer::Vec3f lightDirection3 = renderer->GetLightDirection();
			lightDirection3 = lightDirection3.Normalize();

			VectorToFloat4(lightDirection3, lightDirection);
			ColorToFloat4(renderer->GetLightColor(), lightColor);
			ColorToFloat4(renderer->GetLightAmbientColor(), lightAmbientColor);

			renderer->SetPixelBufferToShader(lightDirection, sizeof(float) * 4, psOffset);
			psOffset += (sizeof(float) * 4);

			renderer->SetPixelBufferToShader(lightColor, sizeof(float) * 4, psOffset);
			psOffset += (sizeof(float) * 4);

			renderer->SetPixelBufferToShader(lightAmbientColor, sizeof(float) * 4, psOffset);
			psOffset += (sizeof(float) * 4);
		}

		// refraction
		if (material->RefractionModelUserPtr != nullptr && renderPassInd == 0)
		{
			auto mat = renderer->GetCameraMatrix();
			renderer->SetPixelBufferToShader(&mat, sizeof(float) * 16, psOffset);
			psOffset += (sizeof(float) * 16);
		}

		for (size_t i = 0; i < materialParam->MaterialUniforms.size(); i++)
		{
			renderer->SetPixelBufferToShader(materialParam->MaterialUniforms[i].data(), sizeof(float) * 4, psOffset);
			psOffset += (sizeof(float) * 4);
		}
	}

 public:

	virtual ~ModelRendererBase() {}

	template<typename RENDERER>
	void BeginRendering_(RENDERER* renderer, const efkModelNodeParam& parameter, int32_t count, void* userData)
	{
		keyValues_.clear();

		m_matrixes.clear();
		m_uv.clear();
#ifdef __EFFEKSEER_BUILD_VERSION16__
		m_alphaUV.clear();
		m_flipbookIndexAndNextRate.clear();
		m_alphaThreshold.clear();
#endif
		m_colors.clear();
		m_times.clear();
		customData1_.clear();
		customData2_.clear();

		matrixesSorted_.clear();
		uvSorted_.clear();
#ifdef __EFFEKSEER_BUILD_VERSION16__
		alphaUVSorted_.clear();
		flipbookIndexAndNextRateSorted_.clear();
		alphaThresholdSorted_.clear();
#endif
		colorsSorted_.clear();
		timesSorted_.clear();
		customData1Sorted_.clear();
		customData2Sorted_.clear();

		if (parameter.BasicParameterPtr->MaterialType == ::Effekseer::RendererMaterialType::File &&
			parameter.BasicParameterPtr->MaterialParameterPtr != nullptr &&
			parameter.BasicParameterPtr->MaterialParameterPtr->MaterialIndex >= 0 &&
			parameter.EffectPointer->GetMaterial(parameter.BasicParameterPtr->MaterialParameterPtr->MaterialIndex) != nullptr)
		{
			auto material = parameter.EffectPointer->GetMaterial(parameter.BasicParameterPtr->MaterialParameterPtr->MaterialIndex);
			customData1Count_ = material->CustomData1;
			customData2Count_ = material->CustomData2;
		}
		else
		{
			customData1Count_ = 0;
			customData2Count_ = 0;
		}

		renderer->GetStandardRenderer()->ResetAndRenderingIfRequired();
	}

	template<typename RENDERER>
	void Rendering_(RENDERER* renderer, const efkModelNodeParam& parameter, const efkModelInstanceParam& instanceParameter, void* userData)
	{
		::Effekseer::BillboardType btype = parameter.Billboard;
		Effekseer::Mat44f mat44;

		if (btype == ::Effekseer::BillboardType::Fixed)
		{
			mat44 = instanceParameter.SRTMatrix43;
		}
		else
		{
			Effekseer::Mat43f mat43;
			Effekseer::Vec3f s;
			Effekseer::Vec3f R;
			Effekseer::Vec3f F;

			CalcBillboard(btype, mat43, s ,R, F, instanceParameter.SRTMatrix43, renderer->GetCameraFrontDirection());

			mat44 = ::Effekseer::Mat43f::Scaling(s) * mat43;
		}
		
		if (parameter.Magnification != 1.0f)
		{
			mat44 = Effekseer::Mat44f::Scaling(::Effekseer::Vec3f(parameter.Magnification)) * mat44;
		}

		if (!parameter.IsRightHand)
		{
			mat44 = Effekseer::Mat44f::Scaling(1.0f, 1.0f, -1.0f) * mat44;
		}

		m_matrixes.push_back(ToStruct(mat44));
		m_uv.push_back(instanceParameter.UV);
#ifdef __EFFEKSEER_BUILD_VERSION16__
		m_alphaUV.push_back(instanceParameter.AlphaUV);
		m_flipbookIndexAndNextRate.push_back(instanceParameter.FlipbookIndexAndNextRate);
		m_alphaThreshold.push_back(instanceParameter.AlphaThreshold);
#endif
		m_colors.push_back(instanceParameter.AllColor);
		m_times.push_back(instanceParameter.Time);
		
		if (customData1Count_ > 0)
		{
			customData1_.push_back(instanceParameter.CustomData1);
		}

		if (customData2Count_ > 0)
		{
			customData2_.push_back(instanceParameter.CustomData2);
		}
	}

	template <typename RENDERER, typename SHADER, typename MODEL, bool Instancing, int InstanceCount>
	void EndRendering_(RENDERER* renderer,
					   SHADER* shader_lighting_texture_normal,
					   SHADER* shader_texture,
					   SHADER* shader_distortion_texture,
					   const efkModelNodeParam& param)
	{
		if (m_matrixes.size() == 0)
			return;
		if (param.ModelIndex < 0)
			return;
		
		int32_t renderPassCount = 1;

		if (param.BasicParameterPtr->MaterialParameterPtr != nullptr && param.BasicParameterPtr->MaterialParameterPtr->MaterialIndex >= 0)
		{
			auto materialData = param.EffectPointer->GetMaterial(param.BasicParameterPtr->MaterialParameterPtr->MaterialIndex);
			if (materialData != nullptr && materialData->IsRefractionRequired)
			{
				// refraction, standard
				renderPassCount = 2;
			}
		}

		// sort
		SortTemporaryValues(renderer, param);

		for (int32_t renderPassInd = 0; renderPassInd < renderPassCount; renderPassInd++)
		{
			RenderPass<RENDERER, SHADER, MODEL, Instancing, InstanceCount>(renderer,
																		   shader_lighting_texture_normal,
																		   //shader_lighting_normal,
																		   //shader_lighting_texture,
																		   //shader_lighting,
																		   shader_texture,
																		   //shader,
																		   shader_distortion_texture,
																		   //shader_distortion,
																		   param,
																		   renderPassInd);		
		}
	}

	template<typename RENDERER, typename SHADER, typename MODEL, bool Instancing, int InstanceCount>
	void RenderPass(
		RENDERER* renderer, 
		SHADER* shader_lighting_texture_normal,
		//SHADER* shader_lighting_normal,
		//SHADER* shader_lighting_texture,
		//SHADER* shader_lighting,
		SHADER* shader_texture,
		//SHADER* shader,
		SHADER* shader_distortion_texture,
		//SHADER* shader_distortion,
		const efkModelNodeParam& param,
		int32_t renderPassInd)
	{
		if (m_matrixes.size() == 0) return;
		if (param.ModelIndex < 0) return;

		MODEL* model = (MODEL*) param.EffectPointer->GetModel(param.ModelIndex);
		if (model == NULL) return;
		
		bool isBackgroundRequired = false;

		isBackgroundRequired |= (param.BasicParameterPtr->MaterialType == Effekseer::RendererMaterialType::BackDistortion);

		if (param.BasicParameterPtr->MaterialParameterPtr != nullptr && param.BasicParameterPtr->MaterialParameterPtr->MaterialIndex >= 0)
		{
			auto materialData = param.EffectPointer->GetMaterial(param.BasicParameterPtr->MaterialParameterPtr->MaterialIndex);
			if (materialData != nullptr && materialData->IsRefractionRequired && renderPassInd == 0)
			{
				isBackgroundRequired = true;
			}
		}

		if (isBackgroundRequired)
		{
			auto callback = renderer->GetDistortingCallback();
			if (callback != nullptr)
			{
				if (!callback->OnDistorting())
				{
					return;
				}
			}
		}

		auto distortion = param.BasicParameterPtr->MaterialType == Effekseer::RendererMaterialType::BackDistortion;

		if (isBackgroundRequired && renderer->GetBackground() == 0)
			return;

		// select shader
		Effekseer::MaterialParameter* materialParam = param.BasicParameterPtr->MaterialParameterPtr;
		//materialParam = nullptr;
		Effekseer::MaterialData* material = nullptr;
		SHADER* shader_ = nullptr;
		bool renderDistortedBackground = false;

		if (materialParam != nullptr && materialParam->MaterialIndex >= 0 &&
			param.EffectPointer->GetMaterial(materialParam->MaterialIndex) != nullptr)
		{
			material = param.EffectPointer->GetMaterial(materialParam->MaterialIndex);

			if (material != nullptr && material->IsRefractionRequired)
			{
				if (renderPassInd == 0)
				{
					shader_ = (SHADER*)material->RefractionModelUserPtr;
					renderDistortedBackground = true;
				}
				else
				{
					shader_ = (SHADER*)material->ModelUserPtr;
				}
			}
			else
			{
				shader_ = (SHADER*)material->ModelUserPtr;
			}

			// validate
			if (shader_ == nullptr)
			{
				return;
			}

			if (material != nullptr && (material->TextureCount != materialParam->MaterialTextures.size() ||
				material->UniformCount != materialParam->MaterialUniforms.size()))
			{
				return;			
			}
		}
		else
		{
			if (distortion)
			{
				//if (param.BasicParameterPtr->Texture1Index >= 0)
				//{
					shader_ = shader_distortion_texture;
				//}
				//else
				//{
				//	shader_ = shader_distortion;
				//}
			}
			else if (param.BasicParameterPtr->MaterialType == Effekseer::RendererMaterialType::Lighting)
			{
				//if (param.BasicParameterPtr->Texture2Index >= 0)
				//{
				//	if (param.BasicParameterPtr->Texture1Index >= 0)
				//	{
						shader_ = shader_lighting_texture_normal;
				//	}
				//	else
				//	{
				//		shader_ = shader_lighting_normal;
				//	}
				//}
				//else
				//{
				//	if (param.BasicParameterPtr->Texture1Index >= 0)
				//	{
				//		shader_ = shader_lighting_texture;
				//	}
				//	else
				//	{
				//		shader_ = shader_lighting;
				//	}
				//}
			}
			else
			{
				//if (param.BasicParameterPtr->Texture1Index >= 0)
				//{
					shader_ = shader_texture;
				//}
				//else
				//{
				//	shader_ = shader;
				//}
			}
		}

		RenderStateBase::State& state = renderer->GetRenderState()->Push();
		state.DepthTest = param.ZTest;
		state.DepthWrite = param.ZWrite;
		state.AlphaBlend = param.BasicParameterPtr->AlphaBlend;
		state.CullingType = param.Culling;

		if (renderDistortedBackground)
		{
			state.AlphaBlend = ::Effekseer::AlphaBlendType::Blend;
		}

		renderer->BeginShader(shader_);

		// Select texture
		if (materialParam != nullptr && material != nullptr)
		{
			int32_t textureCount = 0;
			std::array<Effekseer::TextureData*, ::Effekseer::TextureSlotMax> textures;

			if (materialParam->MaterialTextures.size() > 0)
			{
				textureCount = Effekseer::Min(materialParam->MaterialTextures.size(), ::Effekseer::UserTextureSlotMax);

				auto effect = param.EffectPointer;

				for (size_t i = 0; i < textureCount; i++)
				{
					if (materialParam->MaterialTextures[i].Type == 1)
					{
						if (materialParam->MaterialTextures[i].Index >= 0)
						{
							textures[i] = effect->GetNormalImage(materialParam->MaterialTextures[i].Index);
						}
						else
						{
							textures[i] = nullptr;
						}
					}
					else
					{
						if (materialParam->MaterialTextures[i].Index >= 0)
						{
							textures[i] = effect->GetColorImage(materialParam->MaterialTextures[i].Index);
						}
						else
						{
							textures[i] = nullptr;
						}
					}

					state.TextureFilterTypes[i] = Effekseer::TextureFilterType::Linear;
					state.TextureWrapTypes[i] = material->TextureWrapTypes[i];
				}
			}

			if (renderer->GetBackground() != 0)
			{
				textures[textureCount] = renderer->GetBackground();
				state.TextureFilterTypes[textureCount] = Effekseer::TextureFilterType::Linear;
				state.TextureWrapTypes[textureCount] = Effekseer::TextureWrapType::Clamp;
				textureCount += 1;
			}

			if (textureCount > 0)
			{
				renderer->SetTextures(shader_, textures.data(), textureCount);			
			}
		}
		else
		{
#ifdef __EFFEKSEER_BUILD_VERSION16__
			Effekseer::TextureData* textures[3];
			textures[0] = nullptr;
			textures[1] = nullptr;
			textures[2] = nullptr;
#else
			Effekseer::TextureData* textures[2];
			textures[0] = nullptr;
			textures[1] = nullptr;
#endif

			if (distortion)
			{
				if (param.BasicParameterPtr->Texture1Index >= 0)
				{
					textures[0] = param.EffectPointer->GetDistortionImage(param.BasicParameterPtr->Texture1Index);
				}
				else
				{
					textures[0] = renderer->GetImpl()->GetProxyTexture(EffekseerRenderer::ProxyTextureType::White);
				}

				textures[1] = renderer->GetBackground();

#ifdef __EFFEKSEER_BUILD_VERSION16__
				if (param.BasicParameterPtr->Texture3Index >= 0)
				{
					textures[2] = param.EffectPointer->GetDistortionImage(param.BasicParameterPtr->Texture3Index);
				}
				else
				{
					textures[2] = renderer->GetImpl()->GetProxyTexture(EffekseerRenderer::ProxyTextureType::White);
				}
#endif
			}
			else
			{
				if (param.BasicParameterPtr->Texture1Index >= 0)
				{
					textures[0] = param.EffectPointer->GetColorImage(param.BasicParameterPtr->Texture1Index);
				}
				
				if (textures[0] == nullptr)
				{
					textures[0] = renderer->GetImpl()->GetProxyTexture(EffekseerRenderer::ProxyTextureType::White);	
				}
				

				if (param.BasicParameterPtr->Texture2Index >= 0)
				{
					textures[1] = param.EffectPointer->GetNormalImage(param.BasicParameterPtr->Texture2Index);
				}
				
				if (textures[1] == nullptr)
				{
					textures[1] = renderer->GetImpl()->GetProxyTexture(EffekseerRenderer::ProxyTextureType::Normal);
				}

#ifdef __EFFEKSEER_BUILD_VERSION16__
				if (param.BasicParameterPtr->Texture3Index >= 0)
				{
					textures[2] = param.EffectPointer->GetColorImage(param.BasicParameterPtr->Texture3Index);
				}

				if (textures[2] == nullptr)
				{
					textures[2] = renderer->GetImpl()->GetProxyTexture(EffekseerRenderer::ProxyTextureType::White);	
				}
#endif
			}

			state.TextureFilterTypes[0] = param.BasicParameterPtr->TextureFilter1;
			state.TextureWrapTypes[0] = param.BasicParameterPtr->TextureWrap1;

			if (distortion)
			{
				state.TextureFilterTypes[1] = Effekseer::TextureFilterType::Linear;
				state.TextureWrapTypes[1] = Effekseer::TextureWrapType::Clamp;
			}
			else
			{
				state.TextureFilterTypes[1] = param.BasicParameterPtr->TextureFilter2;
				state.TextureWrapTypes[1] = param.BasicParameterPtr->TextureWrap2;
			}

#ifdef __EFFEKSEER_BUILD_VERSION16__
			state.TextureFilterTypes[2] = param.BasicParameterPtr->TextureFilter3;
			state.TextureWrapTypes[2] = param.BasicParameterPtr->TextureWrap3;
#endif

#ifdef __EFFEKSEER_BUILD_VERSION16__
			renderer->SetTextures(shader_, textures, 3);
#else
			renderer->SetTextures(shader_, textures, 2);
#endif
		}
		
		renderer->GetRenderState()->Update(distortion);

		ModelRendererVertexConstantBuffer<InstanceCount>* vcb =
			(ModelRendererVertexConstantBuffer<InstanceCount>*)shader_->GetVertexConstantBuffer();

		float* cutomData1Ptr = nullptr;
		float* cutomData2Ptr = nullptr;

		if (materialParam != nullptr && material != nullptr)
		{
			StoreFileUniform<RENDERER, SHADER, InstanceCount>(renderer, shader_, material, materialParam, param, renderPassInd, cutomData1Ptr, cutomData2Ptr);
		}
		else
		{
			std::array<float, 4> uvInversed;
			std::array<float, 4> uvInversedBack;

			GetInversedFlags(renderer, uvInversed, uvInversedBack);

			vcb->UVInversed[0] = uvInversed[0];
			vcb->UVInversed[1] = uvInversed[1];

			if (distortion)
			{
				float* pcb = (float*) shader_->GetPixelConstantBuffer();
				pcb[4 * 0 + 0] = param.BasicParameterPtr->DistortionIntensity;

				pcb[4 * 1 + 0] = uvInversedBack[0];
				pcb[4 * 1 + 1] = uvInversedBack[1];

#ifdef __EFFEKSEER_BUILD_VERSION16__
				pcb[4 * 2 + 0] = param.BasicParameterPtr->EnableInterpolation;
				pcb[4 * 2 + 1] = param.BasicParameterPtr->InterpolationType;
#endif
			}
			else
			{
				ModelRendererPixelConstantBuffer* pcb = (ModelRendererPixelConstantBuffer*) shader_->GetPixelConstantBuffer();

				// specify predefined parameters
				if (param.BasicParameterPtr->MaterialType == Effekseer::RendererMaterialType::Lighting)
				{
					::Effekseer::Vec3f lightDirection = renderer->GetLightDirection();
					lightDirection = lightDirection.Normalize();
					VectorToFloat4(lightDirection, vcb->LightDirection);
					VectorToFloat4(lightDirection, pcb->LightDirection);
				}

				{
					ColorToFloat4(renderer->GetLightColor(), vcb->LightColor);
					ColorToFloat4(renderer->GetLightColor(), pcb->LightColor);
				}

				{
					ColorToFloat4(renderer->GetLightAmbientColor(), vcb->LightAmbientColor);
					ColorToFloat4(renderer->GetLightAmbientColor(), pcb->LightAmbientColor);
				}

#ifdef __EFFEKSEER_BUILD_VERSION16__
				pcb->ModelFlipbookParameter.EnableInterpolation = param.BasicParameterPtr->EnableInterpolation;
				pcb->ModelFlipbookParameter.InterpolationType = param.BasicParameterPtr->InterpolationType;
#endif
			}
		}

		vcb->CameraMatrix = renderer->GetCameraProjectionMatrix();

#ifdef __EFFEKSEER_BUILD_VERSION16__
		vcb->ModelFlipbookParameter.EnableInterpolation = param.BasicParameterPtr->EnableInterpolation;
		vcb->ModelFlipbookParameter.LoopType = param.BasicParameterPtr->UVLoopType;
		vcb->ModelFlipbookParameter.DivideX =  param.BasicParameterPtr->FlipbookDivideX;
		vcb->ModelFlipbookParameter.DivideY =  param.BasicParameterPtr->FlipbookDivideY;
#endif

		// Check time
		auto stTime0 = m_times[0] % model->GetFrameCount();
		auto isTimeSame = true;

		for (auto t : m_times)
		{
			t = t % model->GetFrameCount();
			if(t != stTime0)
			{
				isTimeSame = false;
				break;
			}
		}

		if(Instancing && isTimeSame)
		{
			auto& imodel = model->InternalModels[stTime0];

			// Invalid unless layout is set after buffer
			renderer->SetVertexBuffer(imodel.VertexBuffer, model->GetVertexSize());
			renderer->SetIndexBuffer(imodel.IndexBuffer);
			renderer->SetLayout(shader_);

			for( size_t loop = 0; loop < m_matrixes.size(); )
			{
				int32_t modelCount = Effekseer::Min(static_cast<int32_t>(m_matrixes.size()) - loop, model->ModelCount);
				
				for( int32_t num = 0; num < modelCount; num++ )
				{
					vcb->ModelMatrix[num] = m_matrixes[loop+num];

					// DepthParameter
					::Effekseer::Mat44f modelMatrix = vcb->ModelMatrix[num];
					ApplyDepthParameters(modelMatrix,
										renderer->GetCameraFrontDirection(),
										renderer->GetCameraPosition(),
										param.DepthParameterPtr,
										param.IsRightHand);
					vcb->ModelMatrix[num] = ToStruct(modelMatrix);

					vcb->ModelUV[num][0] = m_uv[loop+num].X;
					vcb->ModelUV[num][1] = m_uv[loop+num].Y;
					vcb->ModelUV[num][2] = m_uv[loop+num].Width;
					vcb->ModelUV[num][3] = m_uv[loop+num].Height;

#ifdef __EFFEKSEER_BUILD_VERSION16__
					vcb->ModelAlphaUV[num][0] = m_alphaUV[loop + num].X;
					vcb->ModelAlphaUV[num][1] = m_alphaUV[loop + num].Y;
					vcb->ModelAlphaUV[num][2] = m_alphaUV[loop + num].Width;
					vcb->ModelAlphaUV[num][3] = m_alphaUV[loop + num].Height;

					vcb->ModelFlipbookIndexAndNextRate[num][0] = m_flipbookIndexAndNextRate[loop + num];

					vcb->ModelAlphaThreshold[num][0] = m_alphaThreshold[loop + num];
#endif

					ColorToFloat4(m_colors[loop+num],vcb->ModelColor[num]);

					if (cutomData1Ptr != nullptr)
					{
						cutomData1Ptr[num * 4 + 0] = customData1_[loop + num][0];
						cutomData1Ptr[num * 4 + 1] = customData1_[loop + num][1];
						cutomData1Ptr[num * 4 + 2] = customData1_[loop + num][2];
						cutomData1Ptr[num * 4 + 3] = customData1_[loop + num][3];
					}

					if (cutomData2Ptr != nullptr)
					{
						cutomData2Ptr[num * 4 + 0] = customData2_[loop + num][0];
						cutomData2Ptr[num * 4 + 1] = customData2_[loop + num][1];
						cutomData2Ptr[num * 4 + 2] = customData2_[loop + num][2];
						cutomData2Ptr[num * 4 + 3] = customData2_[loop + num][3];
					}
				}

				shader_->SetConstantBuffer();

				renderer->DrawPolygon(imodel.VertexCount * modelCount, imodel.IndexCount * modelCount);

				loop += modelCount;
			}
		}
		else
		{
			for( size_t loop = 0; loop < m_matrixes.size(); )
			{
				auto stTime = m_times[loop] % model->GetFrameCount();
				auto& imodel = model->InternalModels[stTime];

				// Invalid unless layout is set after buffer
				renderer->SetVertexBuffer(imodel.VertexBuffer, model->GetVertexSize());
				renderer->SetIndexBuffer(imodel.IndexBuffer);
				renderer->SetLayout(shader_);

				vcb->ModelMatrix[0] = m_matrixes[loop];
				vcb->ModelUV[0][0] = m_uv[loop].X;
				vcb->ModelUV[0][1] = m_uv[loop].Y;
				vcb->ModelUV[0][2] = m_uv[loop].Width;
				vcb->ModelUV[0][3] = m_uv[loop].Height;

#ifdef __EFFEKSEER_BUILD_VERSION16__
				vcb->ModelAlphaUV[0][0] = m_alphaUV[loop].X;
				vcb->ModelAlphaUV[0][1] = m_alphaUV[loop].Y;
				vcb->ModelAlphaUV[0][2] = m_alphaUV[loop].Width;
				vcb->ModelAlphaUV[0][3] = m_alphaUV[loop].Height;

				vcb->ModelFlipbookIndexAndNextRate[0][0] = m_flipbookIndexAndNextRate[loop];

				vcb->ModelAlphaThreshold[0][0] = m_alphaThreshold[loop];
#endif

				// DepthParameters
				::Effekseer::Mat44f modelMatrix = vcb->ModelMatrix[0];
				ApplyDepthParameters(modelMatrix, renderer->GetCameraFrontDirection(), renderer->GetCameraPosition(),
									 param.DepthParameterPtr,
									 param.IsRightHand);
				vcb->ModelMatrix[0] = ToStruct(modelMatrix);
				ColorToFloat4( m_colors[loop], vcb->ModelColor[0] );

				if (cutomData1Ptr != nullptr)
				{
					cutomData1Ptr[0] = customData1_[loop][0];
					cutomData1Ptr[1] = customData1_[loop][1];
					cutomData1Ptr[2] = customData1_[loop][2];
					cutomData1Ptr[3] = customData1_[loop][3];
				}

				if (cutomData2Ptr != nullptr)
				{
					cutomData2Ptr[0] = customData2_[loop][0];
					cutomData2Ptr[1] = customData2_[loop][1];
					cutomData2Ptr[2] = customData2_[loop][2];
					cutomData2Ptr[3] = customData2_[loop][3];
				}

				shader_->SetConstantBuffer();
				renderer->DrawPolygon(imodel.VertexCount, imodel.IndexCount);

				loop += 1;
			}
		}

		renderer->EndShader(shader_);

		renderer->GetRenderState()->Pop();
	}
};
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
}
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#endif	// __EFFEKSEERRENDERER_MODEL_RENDERER_H__


#ifndef __EFFEKSEERRENDERER_SHADER_BASE_H__
#define __EFFEKSEERRENDERER_SHADER_BASE_H__

#include "EffekseerNative.h"
#include <assert.h>
#include <sstream>
#include <string.h>

namespace EffekseerRenderer
{
class ShaderBase
{
public:
	ShaderBase() {}
	virtual ~ShaderBase() {}

	virtual void SetVertexConstantBufferSize(int32_t size) = 0;
	virtual void SetPixelConstantBufferSize(int32_t size) = 0;

	virtual void* GetVertexConstantBuffer() = 0;
	virtual void* GetPixelConstantBuffer() = 0;

	virtual void SetConstantBuffer() = 0;
};

} // namespace EffekseerRenderer

#endif // __EFFEKSEERRENDERER_SHADER_BASE_H__

#ifndef	__EFFEKSEERRENDERER_RIBBON_RENDERER_BASE_H__
#define	__EFFEKSEERRENDERER_RIBBON_RENDERER_BASE_H__

//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------
#include "EffekseerNative.h"

#include <assert.h>
#include <string.h>


//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
namespace EffekseerRenderer
{
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	typedef ::Effekseer::RibbonRenderer::NodeParameter efkRibbonNodeParam;
	typedef ::Effekseer::RibbonRenderer::InstanceParameter efkRibbonInstanceParam;
	typedef ::Effekseer::Vec3f efkVector3D;

	template<typename RENDERER, typename VERTEX_NORMAL, typename VERTEX_DISTORTION>
	class RibbonRendererBase
		: public ::Effekseer::RibbonRenderer
		, public ::Effekseer::AlignedAllocationPolicy<16>
	{
	private:

	protected:
		RENDERER*						m_renderer;
		int32_t							m_ribbonCount;

		int32_t							m_ringBufferOffset;
		uint8_t*						m_ringBufferData;

		efkRibbonNodeParam					innstancesNodeParam;
		Effekseer::CustomAlignedVector<efkRibbonInstanceParam> instances;
		SplineGenerator spline_left;
		SplineGenerator spline_right;

		int32_t vertexCount_ = 0;
		int32_t stride_ = 0;

		int32_t customData1Count_ = 0;
		int32_t customData2Count_ = 0;

		enum class VertexType
		{
			Normal,
			Distortion,
			Dynamic,
		};

		VertexType GetVertexType(const VERTEX_NORMAL* v) { return VertexType::Normal; }

		VertexType GetVertexType(const VERTEX_DISTORTION* v) { return VertexType::Distortion; }

		VertexType GetVertexType(const DynamicVertex* v) { return VertexType::Dynamic; }

		template <typename VERTEX, int TARGET> void AssignUV(StrideView<VERTEX> v, float uvX1, float uvX2, float uvY1, float uvY2)
		{
			if (TARGET == 0)
			{
				v[0].UV[0] = uvX1;
				v[0].UV[1] = uvY1;

				v[1].UV[0] = uvX2;
				v[1].UV[1] = uvY1;

				v[2].UV[0] = uvX1;
				v[2].UV[1] = uvY2;

				v[3].UV[0] = uvX2;
				v[3].UV[1] = uvY2;
			}
#ifdef __EFFEKSEER_BUILD_VERSION16__
			else if (TARGET == 1)
			{
				v[0].UV2[0] = uvX1;
				v[0].UV2[1] = uvY1;

				v[1].UV2[0] = uvX2;
				v[1].UV2[1] = uvY1;

				v[2].UV2[0] = uvX1;
				v[2].UV2[1] = uvY2;

				v[3].UV2[0] = uvX2;
				v[3].UV2[1] = uvY2;
			}
			else if (TARGET == 2)
			{
				v[0].AlphaUV[0] = uvX1;
				v[0].AlphaUV[1] = uvY1;

				v[1].AlphaUV[0] = uvX2;
				v[1].AlphaUV[1] = uvY1;

				v[2].AlphaUV[0] = uvX1;
				v[2].AlphaUV[1] = uvY2;

				v[3].AlphaUV[0] = uvX2;
				v[3].AlphaUV[1] = uvY2;
			}
#else
			else
			{
				v[0].UV2[0] = uvX1;
				v[0].UV2[1] = uvY1;
					   
				v[1].UV2[0] = uvX2;
				v[1].UV2[1] = uvY1;
					   
				v[2].UV2[0] = uvX1;
				v[2].UV2[1] = uvY2;
					   
				v[3].UV2[0] = uvX2;
				v[3].UV2[1] = uvY2;
			}
#endif
		}

		template <typename VERTEX, int TARGET> 
		void AssignUVs(efkRibbonNodeParam& parameter, StrideView<VERTEX> verteies)
		{
			float uvx = 0.0f;
			float uvw = 1.0f;
			float uvy = 0.0f;
			float uvh = 1.0f;

			if (parameter.TextureUVTypeParameterPtr->Type == ::Effekseer::TextureUVType::Strech)
			{
				verteies.Reset();

				for (size_t loop = 0; loop < instances.size() - 1; loop++)
				{
					const auto& param = instances[loop];
					if (TARGET == 0)
					{
						uvx = param.UV.X;
						uvw = param.UV.Width;
						uvy = param.UV.Y;
						uvh = param.UV.Height;
					}
#ifdef __EFFEKSEER_BUILD_VERSION16__
					else if (TARGET == 2)
					{
						uvx = param.AlphaUV.X;
						uvw = param.AlphaUV.Width;
						uvy = param.AlphaUV.Y;
						uvh = param.AlphaUV.Height;
					}
#endif

					for (int32_t sploop = 0; sploop < parameter.SplineDivision; sploop++)
					{
						float percent1 = (float)(param.InstanceIndex * parameter.SplineDivision + sploop) /
										 (float)((param.InstanceCount - 1) * parameter.SplineDivision);

						float percent2 = (float)(param.InstanceIndex * parameter.SplineDivision + sploop + 1) /
										 (float)((param.InstanceCount - 1) * parameter.SplineDivision);

						auto uvX1 = uvx;
						auto uvX2 = uvx + uvw;
						auto uvY1 = uvy + percent1 * uvh;
						auto uvY2 = uvy + percent2 * uvh;

						AssignUV<VERTEX, TARGET>(verteies, uvX1, uvX2, uvY1, uvY2);

						verteies += 4;
					}
				}
			}
			else if (parameter.TextureUVTypeParameterPtr->Type == ::Effekseer::TextureUVType::Tile)
			{
				const auto& uvParam = *parameter.TextureUVTypeParameterPtr;
	
				verteies.Reset();

				for (size_t loop = 0; loop < instances.size() - 1; loop++)
				{
					auto& param = instances[loop];
					if (TARGET == 0)
					{
						uvx = param.UV.X;
						uvw = param.UV.Width;
						uvy = param.UV.Y;
						uvh = param.UV.Height;
					}
#ifdef __EFFEKSEER_BUILD_VERSION16__
					else if (TARGET == 2)
					{
						uvx = param.AlphaUV.X;
						uvw = param.AlphaUV.Width;
						uvy = param.AlphaUV.Y;
						uvh = param.AlphaUV.Height;
					}
#endif

					if (loop < uvParam.TileEdgeTail)
					{
						float uvBegin = uvy;
						float uvEnd = uvy + uvh * uvParam.TileLoopAreaBegin;

						for (int32_t sploop = 0; sploop < parameter.SplineDivision; sploop++)
						{
							float percent1 = (float)(param.InstanceIndex * parameter.SplineDivision + sploop) /
											 (float)((uvParam.TileEdgeTail) * parameter.SplineDivision);

							float percent2 = (float)(param.InstanceIndex * parameter.SplineDivision + sploop + 1) /
											 (float)((uvParam.TileEdgeTail) * parameter.SplineDivision);

							auto uvX1 = uvx;
							auto uvX2 = uvx + uvw;
							auto uvY1 = uvBegin + (uvEnd - uvBegin) * percent1;
							auto uvY2 = uvBegin + (uvEnd - uvBegin) * percent2;

							AssignUV<VERTEX, TARGET>(verteies, uvX1, uvX2, uvY1, uvY2);

							verteies += 4;
						}
					}
					else if (loop >= param.InstanceCount - 1 - uvParam.TileEdgeHead)
					{
						float uvBegin = uvy + uvh * uvParam.TileLoopAreaEnd;
						float uvEnd = uvy + uvh * 1.0f;

						for (int32_t sploop = 0; sploop < parameter.SplineDivision; sploop++)
						{
							float percent1 = (float)((param.InstanceIndex - (param.InstanceCount - 1 - uvParam.TileEdgeHead)) *
														 parameter.SplineDivision +
													 sploop) /
											 (float)((uvParam.TileEdgeHead) * parameter.SplineDivision);

							float percent2 = (float)((param.InstanceIndex - (param.InstanceCount - 1 - uvParam.TileEdgeHead)) *
														 parameter.SplineDivision +
													 sploop + 1) /
											 (float)((uvParam.TileEdgeHead) * parameter.SplineDivision);

							auto uvX1 = uvx;
							auto uvX2 = uvx + uvw;
							auto uvY1 = uvBegin + (uvEnd - uvBegin) * percent1;
							auto uvY2 = uvBegin + (uvEnd - uvBegin) * percent2;

							AssignUV<VERTEX, TARGET>(verteies, uvX1, uvX2, uvY1, uvY2);

							verteies += 4;
						}
					}
					else
					{
						float uvBegin = uvy + uvh * uvParam.TileLoopAreaBegin;
						float uvEnd = uvy + uvh * uvParam.TileLoopAreaEnd;

						for (int32_t sploop = 0; sploop < parameter.SplineDivision; sploop++)
						{
							bool isFirst = param.InstanceIndex == 0 && sploop == 0;
							bool isLast = param.InstanceIndex == (param.InstanceCount - 1);

							float percent1 = (float)(sploop) / (float)(parameter.SplineDivision);

							float percent2 = (float)(sploop + 1) / (float)(parameter.SplineDivision);

							auto uvX1 = uvx;
							auto uvX2 = uvx + uvw;
							auto uvY1 = uvBegin + (uvEnd - uvBegin) * percent1;
							auto uvY2 = uvBegin + (uvEnd - uvBegin) * percent2;

							AssignUV<VERTEX, TARGET>(verteies, uvX1, uvX2, uvY1, uvY2);

							verteies += 4;
						}
					}
				}
			}

		}

		template<typename VERTEX>
		void RenderSplines(const ::Effekseer::Mat44f& camera)
		{
			if (instances.size() == 0)
			{
				return;
			}

			auto& parameter = innstancesNodeParam;

			auto vertexType = GetVertexType((VERTEX*)m_ringBufferData);

			// Calculate spline
			if (parameter.SplineDivision > 1)
			{
				spline_left.Reset();
				spline_right.Reset();

				for (size_t loop = 0; loop < instances.size(); loop++)
				{
					auto& param = instances[loop];

					efkVector3D pl(param.Positions[0], 0.0f, 0.0f);
					efkVector3D pr(param.Positions[1], 0.0f, 0.0f);

					if (parameter.ViewpointDependent)
					{
						::Effekseer::Mat43f mat = param.SRTMatrix43;
						::Effekseer::Vec3f s;
						::Effekseer::Mat43f r;
						::Effekseer::Vec3f t;
						mat.GetSRT(s, r, t);

						ApplyDepthParameters(r,
											 t,
											 s,
											 m_renderer->GetCameraFrontDirection(),
											 m_renderer->GetCameraPosition(),
											 parameter.DepthParameterPtr,
											 parameter.IsRightHand);

						// extend
						pl.SetX(pl.GetX() * s.GetX());
						pr.SetX(pr.GetX() * s.GetX());

						::Effekseer::Vec3f F;
						::Effekseer::Vec3f R;
						::Effekseer::Vec3f U;

						U = ::Effekseer::Vec3f(r.X.GetY(), r.Y.GetY(), r.X.GetY());
						F = ::Effekseer::Vec3f(-m_renderer->GetCameraFrontDirection()).Normalize();
						R = ::Effekseer::Vec3f::Cross(U, F).Normalize();
						F = ::Effekseer::Vec3f::Cross(R, U).Normalize();

						::Effekseer::Mat43f mat_rot(
							-R.GetX(), -R.GetY(), -R.GetZ(),
							 U.GetX(),  U.GetY(),  U.GetZ(),
							 F.GetX(),  F.GetY(),  F.GetZ(),
							 t.GetX(),  t.GetY(),  t.GetZ());

						pl = ::Effekseer::Vec3f::Transform(pl, mat_rot);
						pr = ::Effekseer::Vec3f::Transform(pr, mat_rot);

						spline_left.AddVertex(pl);
						spline_right.AddVertex(pr);
					}
					else
					{
						::Effekseer::Mat43f mat = param.SRTMatrix43;

						ApplyDepthParameters(mat,
											 m_renderer->GetCameraFrontDirection(),
											 m_renderer->GetCameraPosition(),
											 //s,
											 parameter.DepthParameterPtr,
											 parameter.IsRightHand);

						pl = ::Effekseer::Vec3f::Transform(pl, mat);
						pr = ::Effekseer::Vec3f::Transform(pr, mat);

						spline_left.AddVertex(pl);
						spline_right.AddVertex(pr);
					}
				}

				spline_left.Calculate();
				spline_right.Calculate();
			}

			StrideView<VERTEX> verteies(m_ringBufferData, stride_, vertexCount_);
			for (size_t loop = 0; loop < instances.size(); loop++)
			{
				auto& param = instances[loop];

				for (auto sploop = 0; sploop < parameter.SplineDivision; sploop++)
				{
					bool isFirst = param.InstanceIndex == 0 && sploop == 0;
					bool isLast = param.InstanceIndex == (param.InstanceCount - 1);

					float percent_instance = sploop / (float)parameter.SplineDivision;

					if (parameter.SplineDivision > 1)
					{
						verteies[0].Pos = ToStruct(spline_left.GetValue(param.InstanceIndex + sploop / (float)parameter.SplineDivision));
						verteies[1].Pos = ToStruct(spline_right.GetValue(param.InstanceIndex + sploop / (float)parameter.SplineDivision));

						verteies[0].SetColor(Effekseer::Color::Lerp(param.Colors[0], param.Colors[2], percent_instance));
						verteies[1].SetColor(Effekseer::Color::Lerp(param.Colors[1], param.Colors[3], percent_instance));
					}
					else
					{
						for (int i = 0; i < 2; i++)
						{
							verteies[i].Pos.X = param.Positions[i];
							verteies[i].Pos.Y = 0.0f;
							verteies[i].Pos.Z = 0.0f;
							verteies[i].SetColor(param.Colors[i]);

#ifdef __EFFEKSEER_BUILD_VERSION16__
							verteies[i].FlipbookIndexAndNextRate = param.FlipbookIndexAndNextRate;
							verteies[i].AlphaThreshold = param.AlphaThreshold;
#endif
						}
					}


					float percent = (float)(param.InstanceIndex  * parameter.SplineDivision + sploop) / (float)((param.InstanceCount - 1) * parameter.SplineDivision);

					if (parameter.ViewpointDependent)
					{
						::Effekseer::Mat43f mat = param.SRTMatrix43;
						::Effekseer::Vec3f s;
						::Effekseer::Mat43f r;
						::Effekseer::Vec3f t;
						mat.GetSRT(s, r, t);

						ApplyDepthParameters(r,
											 t,
											 s,
											 m_renderer->GetCameraFrontDirection(),
											 m_renderer->GetCameraPosition(),
											 parameter.DepthParameterPtr,
											 parameter.IsRightHand);

						if (parameter.SplineDivision > 1)
						{
						}
						else
						{
							for (int i = 0; i < 2; i++)
							{
								verteies[i].Pos.X = verteies[i].Pos.X * s.GetX();
							}

							::Effekseer::Vec3f F;
							::Effekseer::Vec3f R;
							::Effekseer::Vec3f U;

							U = ::Effekseer::Vec3f(r.X.GetY(), r.Y.GetY(), r.Z.GetY());

							F = ::Effekseer::Vec3f(-m_renderer->GetCameraFrontDirection()).Normalize();
							R = ::Effekseer::Vec3f::Cross(U, F).Normalize();
							F = ::Effekseer::Vec3f::Cross(R, U).Normalize();

							::Effekseer::Mat43f mat_rot(
								-R.GetX(), -R.GetY(), -R.GetZ(),
								 U.GetX(),  U.GetY(),  U.GetZ(),
								 F.GetX(),  F.GetY(),  F.GetZ(),
								 t.GetX(),  t.GetY(),  t.GetZ());

							for (int i = 0; i < 2; i++)
							{
								verteies[i].Pos = ToStruct(::Effekseer::Vec3f::Transform(verteies[i].Pos, mat_rot));
							}
						}
					}
					else
					{
						if (parameter.SplineDivision > 1)
						{
						}
						else
						{
							::Effekseer::Mat43f mat = param.SRTMatrix43;

							ApplyDepthParameters(mat,
												 m_renderer->GetCameraFrontDirection(),
												 m_renderer->GetCameraPosition(),
												 //s,
												 parameter.DepthParameterPtr,
												 parameter.IsRightHand);


							for (int i = 0; i < 2; i++)
							{
								verteies[i].Pos = ToStruct(::Effekseer::Vec3f::Transform(verteies[i].Pos, mat));
							}
						}
					}

					if (isFirst || isLast)
					{
						verteies += 2;
					}
					else
					{
						verteies[2] = verteies[0];
						verteies[3] = verteies[1];
						verteies += 4;
					}

					if (!isFirst)
					{
						m_ribbonCount++;
					}

					if (isLast)
					{
						break;
					}
				}
			}

			// calculate UV
			AssignUVs<VERTEX, 0>(parameter, verteies);

			if (vertexType == VertexType::Dynamic)
			{
				AssignUVs<VERTEX, 1>(parameter, verteies);
			}

#ifdef __EFFEKSEER_BUILD_VERSION16__
			AssignUVs<VERTEX, 2>(parameter, verteies);
#endif

			// Apply distortion
			if (vertexType == VertexType::Distortion)
			{
				StrideView<VERTEX_DISTORTION> vs_(m_ringBufferData, stride_, vertexCount_);
				Effekseer::Vec3f axisBefore;

				for (size_t i = 0; i < (instances.size() - 1) * parameter.SplineDivision + 1; i++)
				{
					bool isFirst_ = (i == 0);
					bool isLast_ = (i == ((instances.size() - 1) * parameter.SplineDivision));

					Effekseer::Vec3f axis;

					if (isFirst_)
					{
						axis = (vs_[3].Pos - vs_[1].Pos);
						axis = SafeNormalize(axis);
						axisBefore = axis;
					}
					else if (isLast_)
					{
						axis = axisBefore;
					}
					else
					{
						Effekseer::Vec3f axisOld = axisBefore;
						axis = (vs_[5].Pos - vs_[3].Pos);
						axis = SafeNormalize(axis);
						axisBefore = axis;

						axis = (axisBefore + axisOld) / 2.0f;
						axis = SafeNormalize(axis);
					}

					Effekseer::Vec3f tangent = vs_[1].Pos - vs_[0].Pos;
					tangent = tangent.Normalize();

					if (isFirst_)
					{
						vs_[0].Binormal = vs_[1].Binormal = ToStruct(axis);
						vs_[0].Tangent = vs_[1].Tangent = ToStruct(tangent);
						vs_ += 2;

					}
					else if (isLast_)
					{
						vs_[0].Binormal = vs_[1].Binormal = ToStruct(axis);
						vs_[0].Tangent = vs_[1].Tangent = ToStruct(tangent);
						vs_ += 2;
					}
					else
					{
						vs_[0].Binormal = vs_[1].Binormal = vs_[2].Binormal = vs_[3].Binormal = ToStruct(axis);
						vs_[0].Tangent = vs_[1].Tangent = vs_[2].Tangent = vs_[3].Tangent = ToStruct(tangent);
						vs_ += 4;
					}
				}
			}
			else if (vertexType == VertexType::Dynamic)
			{
				StrideView<DynamicVertex> vs_(m_ringBufferData, stride_, vertexCount_);
				Effekseer::Vec3f axisBefore;

				for (size_t i = 0; i < (instances.size() - 1) * parameter.SplineDivision + 1; i++)
				{
					bool isFirst_ = (i == 0);
					bool isLast_ = (i == ((instances.size() - 1) * parameter.SplineDivision));

					Effekseer::Vec3f axis;

					if (isFirst_)
					{
						axis = (vs_[3].Pos - vs_[1].Pos);
						axis = SafeNormalize(axis);
						axisBefore = axis;
					}
					else if (isLast_)
					{
						axis = axisBefore;
					}
					else
					{
						Effekseer::Vec3f axisOld = axisBefore;
						axis = (vs_[5].Pos - vs_[3].Pos);
						axis = SafeNormalize(axis);
						axisBefore = axis;

						axis = (axisBefore + axisOld) / 2.0f;
						axis = SafeNormalize(axis);
					}

					Effekseer::Vec3f tangent = vs_[1].Pos - vs_[0].Pos;
					tangent = SafeNormalize(tangent);

					Effekseer::Vec3f normal = Effekseer::Vec3f::Cross(axis, tangent);
					normal = SafeNormalize(normal);

					if (!parameter.IsRightHand)
					{
						normal = -normal;
					}

					if (isFirst_)
					{
						vs_[0].Normal = PackVector3DF(normal);					
						vs_[0].Tangent = PackVector3DF(tangent);
						vs_[1].Tangent = vs_[0].Tangent;
						vs_[1].Normal = vs_[0].Normal;

						vs_ += 2;
					}
					else if (isLast_)
					{
						vs_[0].Normal = PackVector3DF(normal);
						vs_[0].Tangent = PackVector3DF(tangent);
						vs_[1].Tangent = vs_[0].Tangent;
						vs_[1].Normal = vs_[0].Normal;

						vs_ += 2;
					}
					else
					{
						vs_[0].Normal = PackVector3DF(normal);
						vs_[0].Tangent = PackVector3DF(tangent);
						vs_[1].Tangent = vs_[0].Tangent;
						vs_[1].Normal = vs_[0].Normal;
						vs_[2].Tangent = vs_[0].Tangent;
						vs_[2].Normal = vs_[0].Normal;
						vs_[3].Tangent = vs_[0].Tangent;
						vs_[3].Normal = vs_[0].Normal;

						vs_ += 4;
					}
				}
			}

			// custom parameter
			if (customData1Count_ > 0)
			{
				StrideView<float> custom(m_ringBufferData + sizeof(DynamicVertex), stride_, vertexCount_);
				for (size_t loop = 0; loop < instances.size() - 1; loop++)
				{
					auto& param = instances[loop];

					for (int32_t sploop = 0; sploop < parameter.SplineDivision; sploop++)
					{
						for (size_t i = 0; i < 4; i++)
						{
							auto c = (float*)(&custom[0]);
							memcpy(c, param.CustomData1.data(), sizeof(float) * customData1Count_);
							custom += 1;						
						}
					}
				}
			}

			if (customData2Count_ > 0)
			{
				StrideView<float> custom(
					m_ringBufferData + sizeof(DynamicVertex) + sizeof(float) * customData1Count_, stride_, vertexCount_);
				for (size_t loop = 0; loop < instances.size() - 1; loop++)
				{
					auto& param = instances[loop];

					for (int32_t sploop = 0; sploop < parameter.SplineDivision; sploop++)
					{
						for (size_t i = 0; i < 4; i++)
						{
							auto c = (float*)(&custom[0]);
							memcpy(c, param.CustomData2.data(), sizeof(float) * customData2Count_);
							custom += 1;
						}
					}
				}
			}
		}

	public:

		RibbonRendererBase(RENDERER* renderer)
			: m_renderer(renderer)
			, m_ribbonCount(0)
			, m_ringBufferOffset(0)
			, m_ringBufferData(NULL)
		{
		}

		virtual ~RibbonRendererBase()
		{
		}


	protected:

		void Rendering_(const efkRibbonNodeParam& parameter, const efkRibbonInstanceParam& instanceParameter, void* userData, const ::Effekseer::Mat44f& camera)
		{
			const auto& state = m_renderer->GetStandardRenderer()->GetState();

			if ((state.MaterialPtr != nullptr && !state.MaterialPtr->IsSimpleVertex) ||
				parameter.BasicParameterPtr->MaterialType == Effekseer::RendererMaterialType::Lighting)
			{
				Rendering_Internal<DynamicVertex>(parameter, instanceParameter, userData, camera);
			}
			else if (parameter.BasicParameterPtr->MaterialType == Effekseer::RendererMaterialType::BackDistortion)
			{
				Rendering_Internal<VERTEX_DISTORTION>(parameter, instanceParameter, userData, camera);
			}
			else
			{
				Rendering_Internal<VERTEX_NORMAL>(parameter, instanceParameter, userData, camera);
			}
		}

		template<typename VERTEX>
		void Rendering_Internal(const efkRibbonNodeParam& parameter, const efkRibbonInstanceParam& instanceParameter, void* userData, const ::Effekseer::Mat44f& camera)
		{
			if (m_ringBufferData == NULL) return;
			if (instanceParameter.InstanceCount < 2) return;

			bool isFirst = instanceParameter.InstanceIndex == 0;
			bool isLast = instanceParameter.InstanceIndex == (instanceParameter.InstanceCount - 1);

			auto& param = instanceParameter;

			if (isFirst)
			{
				instances.reserve(param.InstanceCount);
				instances.resize(0);
				innstancesNodeParam = parameter;
			}

			instances.push_back(param);

			if (isLast)
			{
				RenderSplines<VERTEX>(camera);
			}
		}

	public:

		void BeginRenderingGroup(const efkRibbonNodeParam& param, int32_t count, void* userData) override
		{
			m_ribbonCount = 0;
			int32_t vertexCount = ((count - 1) * param.SplineDivision) * 4;
			if (vertexCount <= 0) return;

			EffekseerRenderer::StandardRendererState state;
			state.AlphaBlend = param.BasicParameterPtr->AlphaBlend;
			state.CullingType = ::Effekseer::CullingType::Double;
			state.DepthTest = param.ZTest;
			state.DepthWrite = param.ZWrite;
			state.TextureFilter1 = param.BasicParameterPtr->TextureFilter1;
			state.TextureWrap1 = param.BasicParameterPtr->TextureWrap1;
			state.TextureFilter2 = param.BasicParameterPtr->TextureFilter2;
			state.TextureWrap2 = param.BasicParameterPtr->TextureWrap2;
#ifdef __EFFEKSEER_BUILD_VERSION16__
			state.TextureFilter3 = param.BasicParameterPtr->TextureFilter3;
			state.TextureWrap3 = param.BasicParameterPtr->TextureWrap3;

			state.EnableInterpolation = param.BasicParameterPtr->EnableInterpolation;
			state.UVLoopType = param.BasicParameterPtr->UVLoopType;
			state.InterpolationType = param.BasicParameterPtr->InterpolationType;
			state.FlipbookDivideX = param.BasicParameterPtr->FlipbookDivideX;
			state.FlipbookDivideY = param.BasicParameterPtr->FlipbookDivideY;
#endif


			state.Distortion = param.BasicParameterPtr->MaterialType == Effekseer::RendererMaterialType::BackDistortion;
			state.DistortionIntensity = param.BasicParameterPtr->DistortionIntensity;
			state.MaterialType = param.BasicParameterPtr->MaterialType;

			state.CopyMaterialFromParameterToState(param.EffectPointer,
												   param.BasicParameterPtr->MaterialParameterPtr,
												   param.BasicParameterPtr->Texture1Index,
												   param.BasicParameterPtr->Texture2Index
#ifdef __EFFEKSEER_BUILD_VERSION16__
												   , param.BasicParameterPtr->Texture3Index
#endif
			);
			customData1Count_ = state.CustomData1Count;
			customData2Count_ = state.CustomData2Count;

			m_renderer->GetStandardRenderer()->UpdateStateAndRenderingIfRequired(state);

			m_renderer->GetStandardRenderer()->BeginRenderingAndRenderingIfRequired(vertexCount, stride_, (void*&)m_ringBufferData);
			vertexCount_ = vertexCount;
		}

		void Rendering(const efkRibbonNodeParam& parameter, const efkRibbonInstanceParam& instanceParameter, void* userData) override
		{
			Rendering_(parameter, instanceParameter, userData, m_renderer->GetCameraMatrix());
		}
	};
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
}
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#endif	// __EFFEKSEERRENDERER_RIBBON_RENDERER_H__

#ifndef	__EFFEKSEERRENDERER_RING_RENDERER_BASE_H__
#define	__EFFEKSEERRENDERER_RING_RENDERER_BASE_H__

//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------
#include "EffekseerNative.h"

#include <assert.h>
#include <string.h>
#include <math.h>


//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
namespace EffekseerRenderer
{
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
typedef ::Effekseer::RingRenderer::NodeParameter efkRingNodeParam;
typedef ::Effekseer::RingRenderer::InstanceParameter efkRingInstanceParam;
typedef ::Effekseer::Vec3f efkVector3D;

template<typename RENDERER, typename VERTEX_NORMAL, typename VERTEX_DISTORTION>
class RingRendererBase
	: public ::Effekseer::RingRenderer
	, public ::Effekseer::AlignedAllocationPolicy<16>
{
protected:

	struct KeyValue
	{
		float Key;
		efkRingInstanceParam Value;
	};
	std::vector<KeyValue> instances_;

	RENDERER*						m_renderer;
	int32_t							m_ringBufferOffset;
	uint8_t*						m_ringBufferData;

	int32_t							m_spriteCount;
	int32_t							m_instanceCount;
	::Effekseer::Mat44f			m_singleRenderingMatrix;
	::Effekseer::RendererMaterialType materialType_ = ::Effekseer::RendererMaterialType::Default;

	int32_t vertexCount_ = 0;
	int32_t stride_ = 0;
	int32_t customData1Count_ = 0;
	int32_t customData2Count_ = 0;

public:

	RingRendererBase(RENDERER* renderer)
		: m_renderer(renderer)
		, m_ringBufferOffset(0)
		, m_ringBufferData(NULL)
		, m_spriteCount(0)
		, m_instanceCount(0)
	{
	}

	virtual ~RingRendererBase()
	{
	}

protected:

	void RenderingInstance(const efkRingInstanceParam& inst,
						   const efkRingNodeParam& param,
						   const StandardRendererState& state,
						   const ::Effekseer::Mat44f& camera)
	{
		if ((state.MaterialPtr != nullptr && !state.MaterialPtr->IsSimpleVertex) ||
			param.BasicParameterPtr->MaterialType == Effekseer::RendererMaterialType::Lighting)
		{
			Rendering_Internal<DynamicVertex>(param, inst, nullptr, camera);
		}
		else if (param.BasicParameterPtr->MaterialType == Effekseer::RendererMaterialType::BackDistortion)
		{
			Rendering_Internal<VERTEX_DISTORTION>(param, inst, nullptr, camera);
		}
		else
		{
			Rendering_Internal<VERTEX_NORMAL>(param, inst, nullptr, camera);
		}
	}

	void BeginRendering_(RENDERER* renderer, int32_t count, const efkRingNodeParam& param)
	{
		m_spriteCount = 0;
		int32_t singleVertexCount = param.VertexCount * 8;
		m_instanceCount = count;

		instances_.clear();

		if (param.DepthParameterPtr->ZSort != Effekseer::ZSortType::None)
		{
			instances_.reserve(count);
		}

		if (count == 1)
		{
			renderer->GetStandardRenderer()->ResetAndRenderingIfRequired();
		}
		
		EffekseerRenderer::StandardRendererState state;
		state.AlphaBlend = param.BasicParameterPtr->AlphaBlend;
		state.CullingType = ::Effekseer::CullingType::Double;
		state.DepthTest = param.ZTest;
		state.DepthWrite = param.ZWrite;
		state.TextureFilter1 = param.BasicParameterPtr->TextureFilter1;
		state.TextureWrap1 = param.BasicParameterPtr->TextureWrap1;
		state.TextureFilter2 = param.BasicParameterPtr->TextureFilter2;
		state.TextureWrap2 = param.BasicParameterPtr->TextureWrap2;
#ifdef __EFFEKSEER_BUILD_VERSION16__
		state.TextureFilter3 = param.BasicParameterPtr->TextureFilter3;
		state.TextureWrap3 = param.BasicParameterPtr->TextureWrap3;

		state.EnableInterpolation = param.BasicParameterPtr->EnableInterpolation;
		state.UVLoopType = param.BasicParameterPtr->UVLoopType;
		state.InterpolationType = param.BasicParameterPtr->InterpolationType;
		state.FlipbookDivideX = param.BasicParameterPtr->FlipbookDivideX;
		state.FlipbookDivideY = param.BasicParameterPtr->FlipbookDivideY;
#endif

		state.Distortion = param.BasicParameterPtr->MaterialType == Effekseer::RendererMaterialType::BackDistortion;
		state.DistortionIntensity = param.BasicParameterPtr->DistortionIntensity;
		state.MaterialType = param.BasicParameterPtr->MaterialType;

		state.CopyMaterialFromParameterToState(param.EffectPointer,
											   param.BasicParameterPtr->MaterialParameterPtr,
											   param.BasicParameterPtr->Texture1Index,
											   param.BasicParameterPtr->Texture2Index
#ifdef __EFFEKSEER_BUILD_VERSION16__
											   , param.BasicParameterPtr->Texture3Index
#endif
		);

		customData1Count_ = state.CustomData1Count;
		customData2Count_ = state.CustomData2Count;

		materialType_ = param.BasicParameterPtr->MaterialType;

		renderer->GetStandardRenderer()->UpdateStateAndRenderingIfRequired(state);
		renderer->GetStandardRenderer()->BeginRenderingAndRenderingIfRequired(count * singleVertexCount, stride_, (void*&)m_ringBufferData);

		vertexCount_ = count * singleVertexCount;
	}

	void Rendering_(const efkRingNodeParam& parameter, const efkRingInstanceParam& instanceParameter, void* userData, const ::Effekseer::Mat44f& camera)
	{
		if (parameter.DepthParameterPtr->ZSort == Effekseer::ZSortType::None || CanSingleRendering())
		{
			const auto& state = m_renderer->GetStandardRenderer()->GetState();

			RenderingInstance(instanceParameter, parameter, state, camera);
		}
		else
		{
			KeyValue kv;
			kv.Value = instanceParameter;
			instances_.push_back(kv);
		}
	}

	enum class VertexType
	{
		Normal,
		Distortion,
		Dynamic,
	};

	VertexType GetVertexType(const VERTEX_NORMAL* v) { return VertexType::Normal; }

	VertexType GetVertexType(const VERTEX_DISTORTION* v) { return VertexType::Distortion; }

	VertexType GetVertexType(const DynamicVertex* v) { return VertexType::Dynamic; }

	bool CanSingleRendering()
	{ 
		return m_instanceCount <= 1 && materialType_ == ::Effekseer::RendererMaterialType::Default;
	}

	template<typename VERTEX>
	void Rendering_Internal( const efkRingNodeParam& parameter, const efkRingInstanceParam& instanceParameter, void* userData, const ::Effekseer::Mat44f& camera )
	{
		::Effekseer::Mat43f mat43;

		if (parameter.Billboard == ::Effekseer::BillboardType::Billboard ||
			parameter.Billboard == ::Effekseer::BillboardType::RotatedBillboard ||
			parameter.Billboard == ::Effekseer::BillboardType::YAxisFixed)
		{
			Effekseer::Vec3f s;
			Effekseer::Vec3f R;
			Effekseer::Vec3f F;

			CalcBillboard(parameter.Billboard, mat43, s, R, F, instanceParameter.SRTMatrix43, m_renderer->GetCameraFrontDirection());

			ApplyDepthParameters(mat43,
								 m_renderer->GetCameraFrontDirection(),
								 m_renderer->GetCameraPosition(),
								 s,
								 parameter.DepthParameterPtr,
								 parameter.IsRightHand);

			if (CanSingleRendering())
			{
				mat43 = ::Effekseer::Mat43f::Scaling(s) * mat43;
			}
			else
			{
				mat43 = ::Effekseer::Mat43f::Scaling(s) * mat43;
			}
		}
		else if (parameter.Billboard == ::Effekseer::BillboardType::Fixed)
		{
			mat43 = instanceParameter.SRTMatrix43;

			ApplyDepthParameters(mat43,
								 m_renderer->GetCameraFrontDirection(),
								 m_renderer->GetCameraPosition(),
								 parameter.DepthParameterPtr,
								 parameter.IsRightHand);
		}

		int32_t singleVertexCount = parameter.VertexCount * 8;
		//Vertex* verteies = (Vertex*)m_renderer->GetVertexBuffer()->GetBufferDirect( sizeof(Vertex) * vertexCount );
		
		StrideView<VERTEX> verteies(m_ringBufferData, stride_, singleVertexCount);
		auto vertexType = GetVertexType((VERTEX*)m_ringBufferData);

		const float circleAngleDegree = (instanceParameter.ViewingAngleEnd - instanceParameter.ViewingAngleStart);
		const float stepAngleDegree = circleAngleDegree / (parameter.VertexCount);
		const float stepAngle = (stepAngleDegree) / 180.0f * 3.141592f;
		const float beginAngle = (instanceParameter.ViewingAngleStart + 90) / 180.0f * 3.141592f;
		
		const float outerRadius = instanceParameter.OuterLocation.GetX();
		const float innerRadius = instanceParameter.InnerLocation.GetX();
		const float centerRadius = innerRadius + (outerRadius - innerRadius) * instanceParameter.CenterRatio;

		const float outerHeight = instanceParameter.OuterLocation.GetY();
		const float innerHeight = instanceParameter.InnerLocation.GetY();
		const float centerHeight = innerHeight + (outerHeight - innerHeight) * instanceParameter.CenterRatio;
		
		::Effekseer::Color outerColor = instanceParameter.OuterColor;
		::Effekseer::Color innerColor = instanceParameter.InnerColor;
		::Effekseer::Color centerColor = instanceParameter.CenterColor;
		::Effekseer::Color outerColorNext = instanceParameter.OuterColor;
		::Effekseer::Color innerColorNext = instanceParameter.InnerColor;
		::Effekseer::Color centerColorNext = instanceParameter.CenterColor;

		if (parameter.StartingFade > 0)
		{
			outerColor.A = 0;
			innerColor.A = 0;
			centerColor.A = 0;
		}

		const float stepC = cosf(stepAngle);
		const float stepS = sinf(stepAngle);
		float cos_ = cosf(beginAngle);
		float sin_ = sinf(beginAngle);
		::Effekseer::Vec3f outerCurrent( cos_ * outerRadius, sin_ * outerRadius, outerHeight );
		::Effekseer::Vec3f innerCurrent( cos_ * innerRadius, sin_ * innerRadius, innerHeight );
		::Effekseer::Vec3f centerCurrent( cos_ * centerRadius, sin_ * centerRadius, centerHeight );

		float uv0Current = instanceParameter.UV.X;
		const float uv0Step = instanceParameter.UV.Width / parameter.VertexCount;
		const float uv0v1 = instanceParameter.UV.Y;
		const float uv0v2 = uv0v1 + instanceParameter.UV.Height * 0.5f;
		const float uv0v3 = uv0v1 + instanceParameter.UV.Height;
		float uv0texNext = 0.0f;
		
		float uv1Current = 0.0f;
		const float uv1Step = 1.0f / parameter.VertexCount;
		const float uv1v1 = 0.0f;
		const float uv1v2 = uv1v1 + 0.5f;
		const float uv1v3 = uv1v1 + 1.0f;
		float uv1texNext = 0.0f;

#ifdef __EFFEKSEER_BUILD_VERSION16__
		float alphaUVCurrent = instanceParameter.AlphaUV.X;
		const float alphaUVStep = instanceParameter.AlphaUV.Width / parameter.VertexCount;
		const float alphaUVv1 = instanceParameter.AlphaUV.Y;
		const float alphaUVv2 = alphaUVv1 + instanceParameter.AlphaUV.Height * 0.5f;
		const float alphaUVv3 = alphaUVv1 + instanceParameter.AlphaUV.Height;
		float alphaUVtexNext = 0.0f;
#endif

		::Effekseer::Vec3f outerNext, innerNext, centerNext;

		float currentAngleDegree = 0;
		float fadeStartAngle = parameter.StartingFade;
		float fadeEndingAngle = parameter.EndingFade;

		for( int i = 0; i < singleVertexCount; i += 8 )
		{
			float old_c = cos_;
			float old_s = sin_;

			float t;
			t = cos_ * stepC - sin_ * stepS;
			sin_ = sin_ * stepC + cos_ * stepS;
			cos_ = t;

			outerNext = ::Effekseer::Vec3f{
				cos_ * outerRadius,
				sin_ * outerRadius,
				outerHeight};
			innerNext = ::Effekseer::Vec3f{
				cos_ * innerRadius,
				sin_ * innerRadius,
				innerHeight};
			centerNext = ::Effekseer::Vec3f{
				cos_ * centerRadius,
				sin_ * centerRadius,
				centerHeight};

			currentAngleDegree += stepAngleDegree;

			// for floating decimal point error
			currentAngleDegree = Effekseer::Min(currentAngleDegree, circleAngleDegree);
			float alpha = 1.0f;
			if (currentAngleDegree < fadeStartAngle)
			{
				alpha = currentAngleDegree / fadeStartAngle;
			}
			else if (currentAngleDegree > circleAngleDegree - fadeEndingAngle)
			{
				alpha = 1.0f - (currentAngleDegree - (circleAngleDegree - fadeEndingAngle)) / fadeEndingAngle;
			}

			outerColorNext = instanceParameter.OuterColor;
			innerColorNext = instanceParameter.InnerColor;
			centerColorNext = instanceParameter.CenterColor;

			if (alpha != 1.0f)
			{
				outerColorNext.A = static_cast<uint8_t>(outerColorNext.A * alpha);
				innerColorNext.A = static_cast<uint8_t>(innerColorNext.A * alpha);
				centerColorNext.A = static_cast<uint8_t>(centerColorNext.A * alpha);
			}

			uv0texNext = uv0Current + uv0Step;

			StrideView<VERTEX> v(&verteies[i], stride_, 8);
			v[0].Pos = ToStruct(outerCurrent);
			v[0].SetColor( outerColor );
			v[0].UV[0] = uv0Current;
			v[0].UV[1] = uv0v1;

			v[1].Pos = ToStruct(centerCurrent);
			v[1].SetColor( centerColor );
			v[1].UV[0] = uv0Current;
			v[1].UV[1] = uv0v2;

			v[2].Pos = ToStruct(outerNext);
			v[2].SetColor( outerColorNext );
			v[2].UV[0] = uv0texNext;
			v[2].UV[1] = uv0v1;
			
			v[3].Pos = ToStruct(centerNext);
			v[3].SetColor( centerColorNext );
			v[3].UV[0] = uv0texNext;
			v[3].UV[1] = uv0v2;

			v[4] = v[1];

			v[5].Pos = ToStruct(innerCurrent);
			v[5].SetColor( innerColor );
			v[5].UV[0] = uv0Current;
			v[5].UV[1] = uv0v3;

			v[6] = v[3];

			v[7].Pos = ToStruct(innerNext);
			v[7].SetColor( innerColorNext );
			v[7].UV[0] = uv0texNext;
			v[7].UV[1] = uv0v3;

#ifdef __EFFEKSEER_BUILD_VERSION16__
			alphaUVtexNext = alphaUVCurrent + alphaUVStep;

			v[0].AlphaUV[0] = alphaUVCurrent;
			v[0].AlphaUV[1] = alphaUVv1;

			v[1].AlphaUV[0] = alphaUVCurrent;
			v[1].AlphaUV[1] = alphaUVv2;

			v[2].AlphaUV[0] = alphaUVtexNext;
			v[2].AlphaUV[1] = alphaUVv1;

			v[3].AlphaUV[0] = alphaUVtexNext;
			v[3].AlphaUV[1] = alphaUVv2;

			v[4] = v[1];

			v[5].AlphaUV[0] = alphaUVCurrent;
			v[5].AlphaUV[1] = alphaUVv3;

			v[6] = v[3];

			v[7].AlphaUV[0] = alphaUVtexNext;
			v[7].AlphaUV[1] = alphaUVv3;

			for (int32_t i = 0; i < 8; i++)
			{
				v[i].FlipbookIndexAndNextRate = instanceParameter.FlipbookIndexAndNextRate;
				v[i].AlphaThreshold = instanceParameter.AlphaThreshold;
			}
#endif

			// distortion
			if (vertexType == VertexType::Distortion)
			{
				StrideView<VERTEX_DISTORTION> vs(&verteies[i], stride_, 8);
				auto binormalCurrent = v[5].Pos - v[0].Pos;
				auto binormalNext = v[7].Pos - v[2].Pos;

				// return back
				float t_b;
				t_b = old_c * (stepC) - old_s * (-stepS);
				auto s_b = old_s * (stepC) + old_c * (-stepS);
				auto c_b = t_b;

				::Effekseer::Vec3f outerBefore(
					c_b * outerRadius,
					s_b * outerRadius,
					outerHeight);

				// next
				auto t_n = cos_ * stepC - sin_ * stepS;
				auto s_n = sin_ * stepC + cos_ * stepS;
				auto c_n = t_n;

				::Effekseer::Vec3f outerNN(
					c_n * outerRadius,
					s_n * outerRadius,
					outerHeight);

				::Effekseer::Vec3f tangent0 = (outerCurrent - outerBefore).Normalize();
				::Effekseer::Vec3f tangent1 = (outerNext - outerCurrent).Normalize();
				::Effekseer::Vec3f tangent2 = (outerNN - outerNext).Normalize();

				auto tangentCurrent = (tangent0 + tangent1) / 2.0f;
				auto tangentNext = (tangent1 + tangent2) / 2.0f;

				vs[0].Tangent = ToStruct(tangentCurrent);
				vs[0].Binormal = ToStruct(binormalCurrent);
				vs[1].Tangent = ToStruct(tangentCurrent);
				vs[1].Binormal = ToStruct(binormalCurrent);
				vs[2].Tangent = ToStruct(tangentNext);
				vs[2].Binormal = ToStruct(binormalNext);
				vs[3].Tangent = ToStruct(tangentNext);
				vs[3].Binormal = ToStruct(binormalNext);

				vs[4].Tangent = ToStruct(tangentCurrent);
				vs[4].Binormal = ToStruct(binormalCurrent);
				vs[5].Tangent = ToStruct(tangentCurrent);
				vs[5].Binormal = ToStruct(binormalCurrent);
				vs[6].Tangent = ToStruct(tangentNext);
				vs[6].Binormal = ToStruct(binormalNext);
				vs[7].Tangent = ToStruct(tangentNext);
				vs[7].Binormal = ToStruct(binormalNext);
			}
			else if (vertexType == VertexType::Dynamic)
			{
				StrideView<DynamicVertex> vs(&verteies[i], stride_, 8);

				// return back
				float t_b;
				t_b = old_c * (stepC)-old_s * (-stepS);
				auto s_b = old_s * (stepC) + old_c * (-stepS);
				auto c_b = t_b;

				::Effekseer::Vec3f outerBefore{
					c_b * outerRadius,
					s_b * outerRadius,
					outerHeight};

				// next
				auto t_n = cos_ * stepC - sin_ * stepS;
				auto s_n = sin_ * stepC + cos_ * stepS;
				auto c_n = t_n;

				::Effekseer::Vec3f outerNN;
				outerNN.SetX(c_n * outerRadius);
				outerNN.SetY(s_n * outerRadius);
				outerNN.SetZ(outerHeight);

				::Effekseer::Vec3f tangent0 = (outerCurrent - outerBefore).Normalize();
				::Effekseer::Vec3f tangent1 = (outerNext - outerCurrent).Normalize();
				::Effekseer::Vec3f tangent2 = (outerNN - outerNext).Normalize();

				auto tangentCurrent = (tangent0 + tangent1) / 2.0f;
				auto tangentNext = (tangent1 + tangent2) / 2.0f;

				auto binormalCurrent = v[5].Pos - v[0].Pos;
				auto binormalNext = v[7].Pos - v[2].Pos;

				::Effekseer::Vec3f normalCurrent;
				::Effekseer::Vec3f normalNext;

				normalCurrent = ::Effekseer::Vec3f::Cross(tangentCurrent, binormalCurrent);
				normalNext = ::Effekseer::Vec3f::Cross(tangentNext, binormalNext);

				if (!parameter.IsRightHand)
				{
					normalCurrent = -normalCurrent;
					normalNext = -normalNext;
				}

				// rotate directions
				::Effekseer::Mat43f matRot = mat43;
				matRot.SetTranslation({0.0f, 0.0f, 0.0f});

				normalCurrent = ::Effekseer::Vec3f::Transform(normalCurrent, matRot);
				normalNext = ::Effekseer::Vec3f::Transform(normalNext, matRot);
				tangentCurrent = ::Effekseer::Vec3f::Transform(tangentCurrent, matRot);
				tangentNext = ::Effekseer::Vec3f::Transform(tangentNext, matRot);

				normalCurrent = normalCurrent.Normalize();
				normalNext = normalNext.Normalize();
				tangentCurrent = tangentCurrent.Normalize();
				tangentNext = tangentNext.Normalize();

				vs[0].Normal = PackVector3DF(normalCurrent);
				vs[1].Normal = vs[0].Normal;
				vs[2].Normal = PackVector3DF(normalNext);
				vs[3].Normal = vs[2].Normal;

				vs[4].Normal = vs[0].Normal;
				vs[5].Normal = vs[0].Normal;
				vs[6].Normal = vs[2].Normal;
				vs[7].Normal = vs[2].Normal;

				vs[0].Tangent = PackVector3DF(tangentCurrent);
				vs[1].Tangent = vs[0].Tangent;
				vs[2].Tangent = PackVector3DF(tangentNext);
				vs[3].Tangent = vs[2].Tangent;

				vs[4].Tangent = vs[0].Tangent;
				vs[5].Tangent = vs[0].Tangent;
				vs[6].Tangent = vs[2].Tangent;
				vs[7].Tangent = vs[2].Tangent;

				// uv1
				uv1texNext = uv1Current + uv1Step;

				vs[0].UV2[0] = uv1Current;
				vs[0].UV2[1] = uv1v1;
				 
				vs[1].UV2[0] = uv1Current;
				vs[1].UV2[1] = uv1v2;
				 
				vs[2].UV2[0] = uv1texNext;
				vs[2].UV2[1] = uv1v1;
				 
				vs[3].UV2[0] = uv1texNext;
				vs[3].UV2[1] = uv1v2;
				 
				vs[4].UV2[0] = vs[1].UV2[0];
				vs[4].UV2[1] = vs[1].UV2[1];
				 
				vs[5].UV2[0] = uv1Current;
				vs[5].UV2[1] = uv1v3;
				 
				vs[6].UV2[0] = vs[3].UV2[0];
				vs[6].UV2[1] = vs[3].UV2[1];
				 
				vs[7].UV2[0] = uv1texNext;
				vs[7].UV2[1] = uv1v3;
			}

			outerCurrent = outerNext;
			innerCurrent = innerNext;
			centerCurrent = centerNext;
			uv0Current = uv0texNext;
			uv1Current = uv1texNext;
#ifdef __EFFEKSEER_BUILD_VERSION16__
			alphaUVCurrent = alphaUVtexNext;
#endif
			outerColor = outerColorNext;
			innerColor = innerColorNext;
			centerColor = centerColorNext;
		}

		if (CanSingleRendering())
		{
			m_singleRenderingMatrix = mat43;
		}
		else
		{
			TransformVertexes(verteies, singleVertexCount, mat43);
		}

		// custom parameter
		if (customData1Count_ > 0)
		{
			StrideView<float> custom(m_ringBufferData + sizeof(DynamicVertex), stride_, singleVertexCount);
			for (int i = 0; i < singleVertexCount; i++)
			{
				auto c = (float*)(&custom[i]);
				memcpy(c, instanceParameter.CustomData1.data(), sizeof(float) * customData1Count_);
			}
		}

		if (customData2Count_ > 0)
		{
			StrideView<float> custom(
				m_ringBufferData + sizeof(DynamicVertex) + sizeof(float) * customData1Count_, stride_, singleVertexCount);
			for (int i = 0; i < singleVertexCount; i++)
			{
				auto c = (float*)(&custom[i]);
				memcpy(c, instanceParameter.CustomData2.data(), sizeof(float) * customData2Count_);
			}
		}

		m_spriteCount += 2 * parameter.VertexCount;
		m_ringBufferData += stride_ * singleVertexCount;

	}

	void EndRendering_(RENDERER* renderer, const efkRingNodeParam& param, void* userData, const ::Effekseer::Mat44f& camera)
	{
		if (CanSingleRendering())
		{
			::Effekseer::Mat44f mat = m_singleRenderingMatrix * renderer->GetCameraMatrix();

			renderer->GetStandardRenderer()->Rendering(mat, renderer->GetProjectionMatrix());
		}

		if (param.DepthParameterPtr->ZSort != Effekseer::ZSortType::None && !CanSingleRendering())
		{
			for (auto& kv : instances_)
			{
				efkVector3D t = kv.Value.SRTMatrix43.GetTranslation();

				Effekseer::Vec3f frontDirection = m_renderer->GetCameraFrontDirection();
				if (!param.IsRightHand)
				{
					frontDirection.SetZ(-frontDirection.GetZ());
				}

				kv.Key = Effekseer::Vec3f::Dot(t, frontDirection);
			}

			if (param.DepthParameterPtr->ZSort == Effekseer::ZSortType::NormalOrder)
			{
				std::sort(instances_.begin(), instances_.end(), [](const KeyValue& a, const KeyValue& b) -> bool { return a.Key < b.Key; });
			}
			else
			{
				std::sort(instances_.begin(), instances_.end(), [](const KeyValue& a, const KeyValue& b) -> bool { return a.Key > b.Key; });
			}


			const auto& state = m_renderer->GetStandardRenderer()->GetState();

			for (auto& kv : instances_)
			{
				RenderingInstance(kv.Value, param, state, camera);
			}
			
		}
	}

public:
	void BeginRendering(const efkRingNodeParam& parameter, int32_t count, void* userData)
	{
		BeginRendering_(m_renderer, count, parameter);
	}

	void Rendering(const efkRingNodeParam& parameter, const efkRingInstanceParam& instanceParameter, void* userData)
	{
		if (m_spriteCount == m_renderer->GetSquareMaxCount()) return;
		Rendering_(parameter, instanceParameter, userData, m_renderer->GetCameraMatrix());
	}

	void EndRendering(const efkRingNodeParam& parameter, void* userData)
	{
		if (m_ringBufferData == NULL) return;

		if (m_spriteCount == 0 && parameter.DepthParameterPtr->ZSort == Effekseer::ZSortType::None)
			return;

		EndRendering_(m_renderer, parameter, userData, m_renderer->GetCameraMatrix());
	}
};
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
}
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#endif	// __EFFEKSEERRENDERER_RING_RENDERER_H__

#ifndef	__EFFEKSEERRENDERER_SPRITE_RENDERER_BASE_H__
#define	__EFFEKSEERRENDERER_SPRITE_RENDERER_BASE_H__

//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------
#include "EffekseerNative.h"

#include <assert.h>
#include <string.h>
#include <math.h>
#include <algorithm>


//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
namespace EffekseerRenderer
{
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
typedef ::Effekseer::SpriteRenderer::NodeParameter efkSpriteNodeParam;
typedef ::Effekseer::SpriteRenderer::InstanceParameter efkSpriteInstanceParam;
typedef ::Effekseer::Vec3f efkVector3D;

template<typename RENDERER, typename VERTEX_NORMAL, typename VERTEX_DISTORTION>
class SpriteRendererBase
	: public ::Effekseer::SpriteRenderer
	, public ::Effekseer::AlignedAllocationPolicy<16>
{
protected:
	RENDERER*						m_renderer;
	int32_t							m_spriteCount;
	uint8_t*						m_ringBufferData;

	struct KeyValue
	{
		float Key;
		efkSpriteInstanceParam	Value;
	};

	Effekseer::CustomAlignedVector<KeyValue> instances;
	int32_t vertexCount_ = 0;
	int32_t stride_ = 0;
	int32_t customData1Count_ = 0;
	int32_t customData2Count_ = 0;

public:

	SpriteRendererBase(RENDERER* renderer)
		: m_renderer(renderer)
		, m_spriteCount(0)
		, m_ringBufferData(nullptr)
	{
		// reserve buffers
		instances.reserve(m_renderer->GetSquareMaxCount());
	}

	virtual ~SpriteRendererBase()
	{
	}

protected:

	void RenderingInstance(const efkSpriteInstanceParam& inst,
						   const efkSpriteNodeParam& param,
						   const StandardRendererState& state,
						   const ::Effekseer::Mat44f& camera)
	{
		if ((state.MaterialPtr != nullptr && !state.MaterialPtr->IsSimpleVertex) ||
			param.BasicParameterPtr->MaterialType == Effekseer::RendererMaterialType::Lighting)
		{
			Rendering_Internal<DynamicVertex>(param, inst, nullptr, camera);
		}
		else if (param.BasicParameterPtr->MaterialType == Effekseer::RendererMaterialType::BackDistortion)
		{
			Rendering_Internal<VERTEX_DISTORTION>(param, inst, nullptr, camera);
		}
		else
		{
			Rendering_Internal<VERTEX_NORMAL>(param, inst, nullptr, camera);
		}
	}

	void BeginRendering_(RENDERER* renderer, int32_t count, const efkSpriteNodeParam& param)
	{
		EffekseerRenderer::StandardRendererState state;
		state.AlphaBlend = param.BasicParameterPtr->AlphaBlend;
		state.CullingType = ::Effekseer::CullingType::Double;
		state.DepthTest = param.ZTest;
		state.DepthWrite = param.ZWrite;
		state.TextureFilter1 = param.BasicParameterPtr->TextureFilter1;
		state.TextureWrap1 = param.BasicParameterPtr->TextureWrap1;
		state.TextureFilter2 = param.BasicParameterPtr->TextureFilter2;
		state.TextureWrap2 = param.BasicParameterPtr->TextureWrap2;
#ifdef __EFFEKSEER_BUILD_VERSION16__
		state.TextureFilter3 = param.BasicParameterPtr->TextureFilter3;
		state.TextureWrap3 = param.BasicParameterPtr->TextureWrap3;

		state.EnableInterpolation = param.BasicParameterPtr->EnableInterpolation;
		state.UVLoopType = param.BasicParameterPtr->UVLoopType;
		state.InterpolationType = param.BasicParameterPtr->InterpolationType;
		state.FlipbookDivideX = param.BasicParameterPtr->FlipbookDivideX;
		state.FlipbookDivideY = param.BasicParameterPtr->FlipbookDivideY;
#endif


		state.Distortion = param.BasicParameterPtr->MaterialType == Effekseer::RendererMaterialType::BackDistortion;
		state.DistortionIntensity = param.BasicParameterPtr->DistortionIntensity;
		state.MaterialType = param.BasicParameterPtr->MaterialType;

		state.CopyMaterialFromParameterToState(param.EffectPointer, param.BasicParameterPtr->MaterialParameterPtr, param.BasicParameterPtr->Texture1Index, param.BasicParameterPtr->Texture2Index
#ifdef __EFFEKSEER_BUILD_VERSION16__
											   , param.BasicParameterPtr->Texture3Index
#endif
		);
		customData1Count_ = state.CustomData1Count;
		customData2Count_ = state.CustomData2Count;

		renderer->GetStandardRenderer()->UpdateStateAndRenderingIfRequired(state);

		renderer->GetStandardRenderer()->BeginRenderingAndRenderingIfRequired(count * 4, stride_, (void*&)m_ringBufferData);
		m_spriteCount = 0;

		vertexCount_ = count * 4;

		instances.clear();
	}

	void Rendering_(const efkSpriteNodeParam& parameter, const efkSpriteInstanceParam& instanceParameter, void* userData, const ::Effekseer::Mat44f& camera)
	{
		if (parameter.ZSort == Effekseer::ZSortType::None)
		{
			auto camera = m_renderer->GetCameraMatrix();
			const auto& state = m_renderer->GetStandardRenderer()->GetState();

			RenderingInstance(instanceParameter, parameter, state, camera);
		}
		else
		{
			KeyValue kv;
			kv.Value = instanceParameter;
			instances.push_back(kv);
		}
	}

	enum class VertexType
	{
		Normal,
		Distortion,
		Dynamic,
	};

	VertexType GetVertexType(const VERTEX_NORMAL* v) { return VertexType::Normal; }

	VertexType GetVertexType(const VERTEX_DISTORTION* v) { return VertexType::Distortion; }

	VertexType GetVertexType(const DynamicVertex* v) { return VertexType::Dynamic; }

	template<typename VERTEX>
	void Rendering_Internal( const efkSpriteNodeParam& parameter, const efkSpriteInstanceParam& instanceParameter, void* userData, const ::Effekseer::Mat44f& camera )
	{
		if( m_ringBufferData == nullptr ) return;

		StrideView<VERTEX> verteies(m_ringBufferData, stride_, 4);
		
		auto vertexType = GetVertexType((VERTEX*)m_ringBufferData);

		for( int i = 0; i < 4; i++ )
		{
			verteies[i].Pos.X = instanceParameter.Positions[i].GetX();
			verteies[i].Pos.Y = instanceParameter.Positions[i].GetY();
			verteies[i].Pos.Z = 0.0f;
	
			verteies[i].SetColor( instanceParameter.Colors[i] );

#ifdef __EFFEKSEER_BUILD_VERSION16__
			verteies[i].FlipbookIndexAndNextRate = instanceParameter.FlipbookIndexAndNextRate;
			verteies[i].AlphaThreshold = instanceParameter.AlphaThreshold;
#endif
		}
		
		verteies[0].UV[0] = instanceParameter.UV.X;
		verteies[0].UV[1] = instanceParameter.UV.Y + instanceParameter.UV.Height;
	
		verteies[1].UV[0] = instanceParameter.UV.X + instanceParameter.UV.Width;
		verteies[1].UV[1] = instanceParameter.UV.Y + instanceParameter.UV.Height;
		
		verteies[2].UV[0] = instanceParameter.UV.X;
		verteies[2].UV[1] = instanceParameter.UV.Y;
	
		verteies[3].UV[0] = instanceParameter.UV.X + instanceParameter.UV.Width;
		verteies[3].UV[1] = instanceParameter.UV.Y;

#ifdef __EFFEKSEER_BUILD_VERSION16__
		verteies[0].AlphaUV[0] = instanceParameter.AlphaUV.X;
		verteies[0].AlphaUV[1] = instanceParameter.AlphaUV.Y +  instanceParameter.AlphaUV.Height;
		
		verteies[1].AlphaUV[0] = instanceParameter.AlphaUV.X + instanceParameter.AlphaUV.Width;
		verteies[1].AlphaUV[1] = instanceParameter.AlphaUV.Y + instanceParameter.AlphaUV.Height;
		
		verteies[2].AlphaUV[0] = instanceParameter.AlphaUV.X;
		verteies[2].AlphaUV[1] = instanceParameter.AlphaUV.Y;

		verteies[3].AlphaUV[0] = instanceParameter.AlphaUV.X + instanceParameter.AlphaUV.Width;
		verteies[3].AlphaUV[1] = instanceParameter.AlphaUV.Y;
#endif

		// distortion
		if (vertexType == VertexType::Distortion)
		{
			StrideView<VERTEX_DISTORTION> vs(verteies.pointerOrigin_, stride_, 4);
			for (auto i = 0; i < 4; i++)
			{
				vs[i].Tangent.X = 1.0f;
				vs[i].Tangent.Y = 0.0f;
				vs[i].Tangent.Z = 0.0f;
				vs[i].Binormal.X = 0.0f;
				vs[i].Binormal.Y = 1.0f;
				vs[i].Binormal.Z = 0.0f;
			}
		}
		else if (vertexType == VertexType::Dynamic)
		{
			StrideView<DynamicVertex> vs(verteies.pointerOrigin_, stride_, 4);
			vs[0].UV2[0] = 0.0f;
			vs[0].UV2[1] = 1.0f;
			vs[1].UV2[0] = 1.0f;
			vs[1].UV2[1] = 1.0f;
			vs[2].UV2[0] = 0.0f;
			vs[2].UV2[1] = 0.0f;
			vs[3].UV2[0] = 1.0f;
			vs[3].UV2[1] = 0.0f;
		}
		
		if( parameter.Billboard == ::Effekseer::BillboardType::Billboard ||
			parameter.Billboard == ::Effekseer::BillboardType::RotatedBillboard ||
			parameter.Billboard == ::Effekseer::BillboardType::YAxisFixed)
		{
			Effekseer::Mat43f mat_rot = Effekseer::Mat43f::Identity;
			Effekseer::Vec3f s;
			Effekseer::Vec3f R;
			Effekseer::Vec3f F;

			CalcBillboard(parameter.Billboard, mat_rot, s, R, F, instanceParameter.SRTMatrix43, m_renderer->GetCameraFrontDirection());

			for (int i = 0; i < 4; i++)
			{
				verteies[i].Pos.X = verteies[i].Pos.X * s.GetX();
				verteies[i].Pos.Y = verteies[i].Pos.Y * s.GetY();
			}
	
			ApplyDepthParameters(mat_rot,
								 m_renderer->GetCameraFrontDirection(),
								 m_renderer->GetCameraPosition(),
								 s,
								 parameter.DepthParameterPtr,
								 parameter.IsRightHand);

			TransformVertexes( verteies, 4, mat_rot );

			if (vertexType == VertexType::Dynamic)
			{
				if (!parameter.IsRightHand)
				{
					F = -F;
				}

				StrideView<DynamicVertex> vs(verteies.pointerOrigin_, stride_, 4);
				for (auto i = 0; i < 4; i++)
				{
					vs[i].Normal = PackVector3DF(F);
					vs[i].Tangent = PackVector3DF(R);
				}
			}
		}
		else if( parameter.Billboard == ::Effekseer::BillboardType::Fixed )
		{
			auto mat = instanceParameter.SRTMatrix43;

			ApplyDepthParameters(mat,
							 m_renderer->GetCameraFrontDirection(),
							 m_renderer->GetCameraPosition(),
							 parameter.DepthParameterPtr,
							 parameter.IsRightHand);

			for( int i = 0; i < 4; i++ )
			{
				auto Pos = ::Effekseer::Vec3f::Load(&verteies[i].Pos);
				Pos = ::Effekseer::Vec3f::Transform(Pos, mat);
				::Effekseer::Vec3f::Store(&verteies[i].Pos, Pos);

				// distortion
				if (vertexType == VertexType::Distortion)
				{
					auto vs = (VERTEX_DISTORTION*) & verteies[i];

					::Effekseer::Vec3f t = mat.GetTranslation();

					auto Tangent = ::Effekseer::Vec3f::Load(&vs->Tangent);
					Tangent = ::Effekseer::Vec3f::Transform(Tangent, mat) - t;
					::Effekseer::Vec3f::Store(&vs->Tangent, Tangent);

					auto Binormal = ::Effekseer::Vec3f::Load(&vs->Binormal);
					Binormal = ::Effekseer::Vec3f::Transform(Binormal, mat) - t;
					::Effekseer::Vec3f::Store(&vs->Binormal, Binormal);
				}
				else if (vertexType == VertexType::Dynamic)
				{
					StrideView<DynamicVertex> vs(verteies.pointerOrigin_, stride_, 4);
					auto tangentX = efkVector3D(mat.X.GetX(), mat.Y.GetX(), mat.Z.GetX());
					auto tangentZ = efkVector3D(mat.X.GetZ(), mat.Y.GetZ(), mat.Z.GetZ());
					tangentX = tangentX.Normalize();
					tangentZ = tangentZ.Normalize();

					if (!parameter.IsRightHand)
					{
						tangentZ = -tangentZ;
					}

					vs[i].Normal = PackVector3DF(tangentZ);
					vs[i].Tangent = PackVector3DF(tangentX);
				}
			}
		}
		
		// custom parameter
		if (customData1Count_ > 0)
		{
			StrideView<float> custom(m_ringBufferData + sizeof(DynamicVertex), stride_, 4);
			for (int i = 0; i < 4; i++)
			{
				auto c = (float*)(&custom[i]);
				memcpy(c, instanceParameter.CustomData1.data(), sizeof(float) * customData1Count_);
			}
		}

		if (customData2Count_ > 0)
		{
			StrideView<float> custom(m_ringBufferData + sizeof(DynamicVertex) + sizeof(float) * customData1Count_, stride_, 4);
			for (int i = 0; i < 4; i++)
			{
				auto c = (float*)(&custom[i]);
				memcpy(c, instanceParameter.CustomData2.data(), sizeof(float) * customData2Count_);
			}
		}

		m_ringBufferData += (stride_ * 4);
		m_spriteCount++;
	}

	void EndRendering_(RENDERER* renderer, const efkSpriteNodeParam& param)
	{
		if (param.ZSort != Effekseer::ZSortType::None)
		{
			for (auto& kv : instances)
			{
				efkVector3D t = kv.Value.SRTMatrix43.GetTranslation();

				auto frontDirection = m_renderer->GetCameraFrontDirection();
				if (!param.IsRightHand)
				{
					frontDirection.Z = -frontDirection.Z;
				}

				kv.Key = Effekseer::Vec3f::Dot(t, frontDirection);
			}

			if (param.ZSort == Effekseer::ZSortType::NormalOrder)
			{
				std::sort(instances.begin(), instances.end(), [](const KeyValue& a, const KeyValue& b) -> bool { return a.Key < b.Key; });
			}
			else
			{
				std::sort(instances.begin(), instances.end(), [](const KeyValue& a, const KeyValue& b) -> bool { return a.Key > b.Key; });
			}
			

			for (auto& kv : instances)
			{
				auto camera = m_renderer->GetCameraMatrix();
				const auto& state = renderer->GetStandardRenderer()->GetState();

				RenderingInstance(kv.Value, param, state, camera);
			}
		}
	}

public:
	void BeginRendering(const efkSpriteNodeParam& parameter, int32_t count, void* userData) override
	{
		BeginRendering_(m_renderer, count, parameter);
	}

	void Rendering(const efkSpriteNodeParam& parameter, const efkSpriteInstanceParam& instanceParameter, void* userData) override
	{
		if (m_spriteCount == m_renderer->GetSquareMaxCount()) return;
		Rendering_(parameter, instanceParameter, userData, m_renderer->GetCameraMatrix());
	}

	void EndRendering(const efkSpriteNodeParam& parameter, void* userData) override
	{
		EndRendering_(m_renderer, parameter);
	}
};
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
}
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#endif	// __EFFEKSEERRENDERER_SPRITE_RENDERER_H__


#ifndef	__EFFEKSEERRENDERER_TRACK_RENDERER_BASE_H__
#define	__EFFEKSEERRENDERER_TRACK_RENDERER_BASE_H__

//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------
#include "EffekseerNative.h"

#include <assert.h>
#include <string.h>


//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
namespace EffekseerRenderer
{
	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
	typedef ::Effekseer::TrackRenderer::NodeParameter efkTrackNodeParam;
	typedef ::Effekseer::TrackRenderer::InstanceParameter efkTrackInstanceParam;
	typedef ::Effekseer::Vec3f efkVector3D;

	template<typename RENDERER, typename VERTEX_NORMAL, typename VERTEX_DISTORTION>
	class TrackRendererBase
		: public ::Effekseer::TrackRenderer
		, public ::Effekseer::AlignedAllocationPolicy<16>
	{
	protected:
		RENDERER*						m_renderer;
		int32_t							m_ribbonCount;

		int32_t							m_ringBufferOffset;
		uint8_t*						m_ringBufferData;

		efkTrackNodeParam					innstancesNodeParam;
		Effekseer::CustomAlignedVector<efkTrackInstanceParam> instances;
		SplineGenerator spline;

		int32_t vertexCount_ = 0;
		int32_t stride_ = 0;

		int32_t customData1Count_ = 0;
		int32_t customData2Count_ = 0;

		enum class VertexType
		{
			Normal,
			Distortion,
			Dynamic,
		};

		VertexType GetVertexType(const VERTEX_NORMAL* v) { return VertexType::Normal; }

		VertexType GetVertexType(const VERTEX_DISTORTION* v) { return VertexType::Distortion; }

		VertexType GetVertexType(const DynamicVertex* v) { return VertexType::Dynamic; }

		template <typename VERTEX, int TARGET>
		void AssignUV(StrideView<VERTEX>& v, float uvX1, float uvX2, float uvX3, float uvY1, float uvY2)
		{
			if (TARGET == 0)
			{
				v[0].UV[0] = uvX1;
				v[0].UV[1] = uvY1;

				v[1].UV[0] = uvX2;
				v[1].UV[1] = uvY1;

				v[4].UV[0] = uvX2;
				v[4].UV[1] = uvY1;

				v[5].UV[0] = uvX3;
				v[5].UV[1] = uvY1;

				v[2].UV[0] = uvX1;
				v[2].UV[1] = uvY2;

				v[3].UV[0] = uvX2;
				v[3].UV[1] = uvY2;

				v[6].UV[0] = uvX2;
				v[6].UV[1] = uvY2;

				v[7].UV[0] = uvX3;
				v[7].UV[1] = uvY2;
			}
#ifdef __EFFEKSEER_BUILD_VERSION16__
			else if (TARGET == 1)
			{
				v[0].UV2[0] = uvX1;
				v[0].UV2[1] = uvY1;

				v[1].UV2[0] = uvX2;
				v[1].UV2[1] = uvY1;

				v[4].UV2[0] = uvX2;
				v[4].UV2[1] = uvY1;

				v[5].UV2[0] = uvX3;
				v[5].UV2[1] = uvY1;

				v[2].UV2[0] = uvX1;
				v[2].UV2[1] = uvY2;

				v[3].UV2[0] = uvX2;
				v[3].UV2[1] = uvY2;

				v[6].UV2[0] = uvX2;
				v[6].UV2[1] = uvY2;

				v[7].UV2[0] = uvX3;
				v[7].UV2[1] = uvY2;
			}
			else if (TARGET == 2)
			{
				v[0].AlphaUV[0] = uvX1;
				v[0].AlphaUV[1] = uvY1;

				v[1].AlphaUV[0] = uvX2;
				v[1].AlphaUV[1] = uvY1;

				v[4].AlphaUV[0] = uvX2;
				v[4].AlphaUV[1] = uvY1;

				v[5].AlphaUV[0] = uvX3;
				v[5].AlphaUV[1] = uvY1;

				v[2].AlphaUV[0] = uvX1;
				v[2].AlphaUV[1] = uvY2;

				v[3].AlphaUV[0] = uvX2;
				v[3].AlphaUV[1] = uvY2;

				v[6].AlphaUV[0] = uvX2;
				v[6].AlphaUV[1] = uvY2;

				v[7].AlphaUV[0] = uvX3;
				v[7].AlphaUV[1] = uvY2;
			}
#else
			else
			{
				v[0].UV2[0] = uvX1;
				v[0].UV2[1] = uvY1;
					   
				v[1].UV2[0] = uvX2;
				v[1].UV2[1] = uvY1;
					   
				v[4].UV2[0] = uvX2;
				v[4].UV2[1] = uvY1;
					   
				v[5].UV2[0] = uvX3;
				v[5].UV2[1] = uvY1;
					   
				v[2].UV2[0] = uvX1;
				v[2].UV2[1] = uvY2;
					   
				v[3].UV2[0] = uvX2;
				v[3].UV2[1] = uvY2;
					   
				v[6].UV2[0] = uvX2;
				v[6].UV2[1] = uvY2;
					   
				v[7].UV2[0] = uvX3;
				v[7].UV2[1] = uvY2;
			}
#endif
		}

		template <typename VERTEX, int TARGET> void AssignUVs(efkTrackNodeParam& parameter, StrideView<VERTEX> verteies) 
		{
			float uvx = 0.0f;
			float uvw = 1.0f;
			float uvy = 0.0f;
			float uvh = 1.0f;

			if (parameter.TextureUVTypeParameterPtr->Type == ::Effekseer::TextureUVType::Strech)
			{
				verteies.Reset();
				for (size_t loop = 0; loop < instances.size() - 1; loop++)
				{
					const auto& param = instances[loop];
					if (TARGET == 0)
					{
						uvx = param.UV.X;
						uvw = param.UV.Width;
						uvy = param.UV.Y;
						uvh = param.UV.Height;
					}
#ifdef __EFFEKSEER_BUILD_VERSION16__
					else if (TARGET == 2)
					{
						uvx = param.AlphaUV.X;
						uvw = param.AlphaUV.Width;
						uvy = param.AlphaUV.Y;
						uvh = param.AlphaUV.Height;
					}
#endif

					for (int32_t sploop = 0; sploop < parameter.SplineDivision; sploop++)
					{
						float percent1 = (float)(param.InstanceIndex * parameter.SplineDivision + sploop) /
										 (float)((param.InstanceCount - 1) * parameter.SplineDivision);

						float percent2 = (float)(param.InstanceIndex * parameter.SplineDivision + sploop + 1) /
										 (float)((param.InstanceCount - 1) * parameter.SplineDivision);

						auto uvX1 = uvx;
						auto uvX2 = uvx + uvw * 0.5f;
						auto uvX3 = uvx + uvw;
						auto uvY1 = uvy + percent1 * uvh;
						auto uvY2 = uvy + percent2 * uvh;

						AssignUV<VERTEX, TARGET>(verteies, uvX1, uvX2, uvX3, uvY1, uvY2);

						verteies += 8;
					}
				}
			}
			else if (parameter.TextureUVTypeParameterPtr->Type == ::Effekseer::TextureUVType::Tile)
			{
				const auto& uvParam = *parameter.TextureUVTypeParameterPtr;
				verteies.Reset();

				for (size_t loop = 0; loop < instances.size() - 1; loop++)
				{
					auto& param = instances[loop];
					if (TARGET == 0)
					{
						uvx = param.UV.X;
						uvw = param.UV.Width;
						uvy = param.UV.Y;
						uvh = param.UV.Height;
					}
#ifdef __EFFEKSEER_BUILD_VERSION16__
					else if (TARGET == 2)
					{
						uvx = param.AlphaUV.X;
						uvw = param.AlphaUV.Width;
						uvy = param.AlphaUV.Y;
						uvh = param.AlphaUV.Height;
					}
#endif

					if (loop < uvParam.TileEdgeTail)
					{
						float uvBegin = uvy;
						float uvEnd = uvy + uvh * uvParam.TileLoopAreaBegin;

						for (int32_t sploop = 0; sploop < parameter.SplineDivision; sploop++)
						{
							float percent1 = (float)(param.InstanceIndex * parameter.SplineDivision + sploop) /
											 (float)((uvParam.TileEdgeTail) * parameter.SplineDivision);

							float percent2 = (float)(param.InstanceIndex * parameter.SplineDivision + sploop + 1) /
											 (float)((uvParam.TileEdgeTail) * parameter.SplineDivision);

							auto uvX1 = uvx;
							auto uvX2 = uvx + uvw * 0.5f;
							auto uvX3 = uvx + uvw;
							auto uvY1 = uvBegin + (uvEnd - uvBegin) * percent1;
							auto uvY2 = uvBegin + (uvEnd - uvBegin) * percent2;

							AssignUV<VERTEX, TARGET>(verteies, uvX1, uvX2, uvX3, uvY1, uvY2);

							verteies += 8;
						}
					}
					else if (loop >= param.InstanceCount - 1 - uvParam.TileEdgeHead)
					{
						float uvBegin = uvy + uvh * uvParam.TileLoopAreaEnd;
						float uvEnd = uvy + uvh * 1.0f;

						for (int32_t sploop = 0; sploop < parameter.SplineDivision; sploop++)
						{
							float percent1 = (float)((param.InstanceIndex - (param.InstanceCount - 1 - uvParam.TileEdgeHead)) *
														 parameter.SplineDivision +
													 sploop) /
											 (float)((uvParam.TileEdgeHead) * parameter.SplineDivision);

							float percent2 = (float)((param.InstanceIndex - (param.InstanceCount - 1 - uvParam.TileEdgeHead)) *
														 parameter.SplineDivision +
													 sploop + 1) /
											 (float)((uvParam.TileEdgeHead) * parameter.SplineDivision);

							auto uvX1 = uvx;
							auto uvX2 = uvx + uvw * 0.5f;
							auto uvX3 = uvx + uvw;
							auto uvY1 = uvBegin + (uvEnd - uvBegin) * percent1;
							auto uvY2 = uvBegin + (uvEnd - uvBegin) * percent2;

							AssignUV<VERTEX, TARGET>(verteies, uvX1, uvX2, uvX3, uvY1, uvY2);

							verteies += 8;
						}
					}
					else
					{
						float uvBegin = uvy + uvh * uvParam.TileLoopAreaBegin;
						float uvEnd = uvy + uvh * uvParam.TileLoopAreaEnd;

						for (int32_t sploop = 0; sploop < parameter.SplineDivision; sploop++)
						{
							bool isFirst = param.InstanceIndex == 0 && sploop == 0;
							bool isLast = param.InstanceIndex == (param.InstanceCount - 1);

							float percent1 = (float)(sploop) / (float)(parameter.SplineDivision);

							float percent2 = (float)(sploop + 1) / (float)(parameter.SplineDivision);

							auto uvX1 = uvx;
							auto uvX2 = uvx + uvx + uvw * 0.5f;
							auto uvX3 = uvx + uvw;
							auto uvY1 = uvBegin + (uvEnd - uvBegin) * percent1;
							auto uvY2 = uvBegin + (uvEnd - uvBegin) * percent2;

							AssignUV<VERTEX, TARGET>(verteies, uvX1, uvX2, uvX3, uvY1, uvY2);

							verteies += 8;
						}
					}
				}
			}
		}

		template<typename VERTEX>
		void RenderSplines(const ::Effekseer::Mat44f& camera)
		{
			if (instances.size() == 0)
			{
				return;
			}

			auto& parameter = innstancesNodeParam;

			auto vertexType = GetVertexType((VERTEX*)m_ringBufferData);

			// Calculate spline
			if (parameter.SplineDivision > 1)
			{
				spline.Reset();

				for (size_t loop = 0; loop < instances.size(); loop++)
				{
					auto p = efkVector3D();
					auto& param = instances[loop];

					auto mat = param.SRTMatrix43;

					ApplyDepthParameters(mat,
										 m_renderer->GetCameraFrontDirection(),
										 m_renderer->GetCameraPosition(),
										 //s,
										 parameter.DepthParameterPtr,
										 parameter.IsRightHand);

					p = mat.GetTranslation();
					spline.AddVertex(p);
				}

				spline.Calculate();
			}

			StrideView<VERTEX> verteies(m_ringBufferData, stride_, vertexCount_);

			for (size_t loop = 0; loop < instances.size(); loop++)
			{
				auto& param = instances[loop];

				for (int32_t sploop = 0; sploop < parameter.SplineDivision; sploop++)
				{
					auto mat = param.SRTMatrix43;

					::Effekseer::Vec3f s;
					::Effekseer::Mat43f r;
					::Effekseer::Vec3f t;
					mat.GetSRT(s, r, t);

					ApplyDepthParameters(r,
										 t,
										 s,
										 m_renderer->GetCameraFrontDirection(),
										 m_renderer->GetCameraPosition(),
										 parameter.DepthParameterPtr,
										 parameter.IsRightHand);

					bool isFirst = param.InstanceIndex == 0 && sploop == 0;
					bool isLast = param.InstanceIndex == (param.InstanceCount - 1);

					float size = 0.0f;
					::Effekseer::Color leftColor;
					::Effekseer::Color centerColor;
					::Effekseer::Color rightColor;

					float percent = (float)(param.InstanceIndex  * parameter.SplineDivision + sploop) / (float)((param.InstanceCount - 1) * parameter.SplineDivision);

					if (param.InstanceIndex < param.InstanceCount / 2)
					{
						float l = percent;
						l = l * 2.0f;
						size = param.SizeFor + (param.SizeMiddle - param.SizeFor) * l;

						leftColor.R = (uint8_t)Effekseer::Clamp(param.ColorLeft.R + (param.ColorLeftMiddle.R - param.ColorLeft.R) * l, 255, 0);
						leftColor.G = (uint8_t)Effekseer::Clamp(param.ColorLeft.G + (param.ColorLeftMiddle.G - param.ColorLeft.G) * l, 255, 0);
						leftColor.B = (uint8_t)Effekseer::Clamp(param.ColorLeft.B + (param.ColorLeftMiddle.B - param.ColorLeft.B) * l, 255, 0);
						leftColor.A = (uint8_t)Effekseer::Clamp(param.ColorLeft.A + (param.ColorLeftMiddle.A - param.ColorLeft.A) * l, 255, 0);

						centerColor.R = (uint8_t)Effekseer::Clamp(param.ColorCenter.R + (param.ColorCenterMiddle.R - param.ColorCenter.R) * l, 255, 0);
						centerColor.G = (uint8_t)Effekseer::Clamp(param.ColorCenter.G + (param.ColorCenterMiddle.G - param.ColorCenter.G) * l, 255, 0);
						centerColor.B = (uint8_t)Effekseer::Clamp(param.ColorCenter.B + (param.ColorCenterMiddle.B - param.ColorCenter.B) * l, 255, 0);
						centerColor.A = (uint8_t)Effekseer::Clamp(param.ColorCenter.A + (param.ColorCenterMiddle.A - param.ColorCenter.A) * l, 255, 0);

						rightColor.R = (uint8_t)Effekseer::Clamp(param.ColorRight.R + (param.ColorRightMiddle.R - param.ColorRight.R) * l, 255, 0);
						rightColor.G = (uint8_t)Effekseer::Clamp(param.ColorRight.G + (param.ColorRightMiddle.G - param.ColorRight.G) * l, 255, 0);
						rightColor.B = (uint8_t)Effekseer::Clamp(param.ColorRight.B + (param.ColorRightMiddle.B - param.ColorRight.B) * l, 255, 0);
						rightColor.A = (uint8_t)Effekseer::Clamp(param.ColorRight.A + (param.ColorRightMiddle.A - param.ColorRight.A) * l, 255, 0);
					}
					else
					{
						float l = percent;
						l = 1.0f - (l * 2.0f - 1.0f);
						size = param.SizeBack + (param.SizeMiddle - param.SizeBack) * l;

						leftColor.R = (uint8_t)Effekseer::Clamp(param.ColorLeft.R + (param.ColorLeftMiddle.R - param.ColorLeft.R) * l, 255, 0);
						leftColor.G = (uint8_t)Effekseer::Clamp(param.ColorLeft.G + (param.ColorLeftMiddle.G - param.ColorLeft.G) * l, 255, 0);
						leftColor.B = (uint8_t)Effekseer::Clamp(param.ColorLeft.B + (param.ColorLeftMiddle.B - param.ColorLeft.B) * l, 255, 0);
						leftColor.A = (uint8_t)Effekseer::Clamp(param.ColorLeft.A + (param.ColorLeftMiddle.A - param.ColorLeft.A) * l, 255, 0);

						centerColor.R = (uint8_t)Effekseer::Clamp(param.ColorCenter.R + (param.ColorCenterMiddle.R - param.ColorCenter.R) * l, 255, 0);
						centerColor.G = (uint8_t)Effekseer::Clamp(param.ColorCenter.G + (param.ColorCenterMiddle.G - param.ColorCenter.G) * l, 255, 0);
						centerColor.B = (uint8_t)Effekseer::Clamp(param.ColorCenter.B + (param.ColorCenterMiddle.B - param.ColorCenter.B) * l, 255, 0);
						centerColor.A = (uint8_t)Effekseer::Clamp(param.ColorCenter.A + (param.ColorCenterMiddle.A - param.ColorCenter.A) * l, 255, 0);

						rightColor.R = (uint8_t)Effekseer::Clamp(param.ColorRight.R + (param.ColorRightMiddle.R - param.ColorRight.R) * l, 255, 0);
						rightColor.G = (uint8_t)Effekseer::Clamp(param.ColorRight.G + (param.ColorRightMiddle.G - param.ColorRight.G) * l, 255, 0);
						rightColor.B = (uint8_t)Effekseer::Clamp(param.ColorRight.B + (param.ColorRightMiddle.B - param.ColorRight.B) * l, 255, 0);
						rightColor.A = (uint8_t)Effekseer::Clamp(param.ColorRight.A + (param.ColorRightMiddle.A - param.ColorRight.A) * l, 255, 0);
					}

					VERTEX v[3];

					v[0].Pos.X = (-size / 2.0f) * s.GetX();
					v[0].Pos.Y = 0.0f;
					v[0].Pos.Z = 0.0f;
					v[0].SetColor(leftColor);

					v[1].Pos.X = 0.0f;
					v[1].Pos.Y = 0.0f;
					v[1].Pos.Z = 0.0f;
					v[1].SetColor(centerColor);

					v[2].Pos.X = (size / 2.0f) * s.GetX();
					v[2].Pos.Y = 0.0f;
					v[2].Pos.Z = 0.0f;
					v[2].SetColor(rightColor);

#ifdef __EFFEKSEER_BUILD_VERSION16__
					v[0].FlipbookIndexAndNextRate = param.FlipbookIndexAndNextRate;
					v[1].FlipbookIndexAndNextRate = param.FlipbookIndexAndNextRate;
					v[2].FlipbookIndexAndNextRate = param.FlipbookIndexAndNextRate;

					v[0].AlphaThreshold = param.AlphaThreshold;
					v[1].AlphaThreshold = param.AlphaThreshold;
					v[2].AlphaThreshold = param.AlphaThreshold;
#endif

					if (parameter.SplineDivision > 1)
					{
						v[1].Pos = ToStruct(spline.GetValue(param.InstanceIndex + sploop / (float)parameter.SplineDivision));
					}
					else
					{
						v[1].Pos = ToStruct(t);
					}

					if (isFirst)
					{
						verteies[0] = v[0];
						verteies[1] = v[1];
						verteies[4] = v[1];
						verteies[5] = v[2];
						verteies += 2;

					}
					else if (isLast)
					{
						verteies[0] = v[0];
						verteies[1] = v[1];
						verteies[4] = v[1];
						verteies[5] = v[2];
						verteies += 6;
						m_ribbonCount += 2;
					}
					else
					{
						verteies[0] = v[0];
						verteies[1] = v[1];
						verteies[4] = v[1];
						verteies[5] = v[2];

						verteies[6] = v[0];
						verteies[7] = v[1];
						verteies[10] = v[1];
						verteies[11] = v[2];

						verteies += 8;
						m_ribbonCount += 2;
					}

					if (isLast)
					{
						break;
					}
				}
			}


			// transform all vertecies
			{
				StrideView<VERTEX> vs_(m_ringBufferData, stride_, vertexCount_);
				Effekseer::Vec3f axisBefore;

				for (size_t i = 0; i < (instances.size() - 1) * parameter.SplineDivision + 1; i++)
				{
					bool isFirst_ = (i == 0);
					bool isLast_ = (i == ((instances.size() - 1) * parameter.SplineDivision));
					Effekseer::Vec3f axis;
					Effekseer::Vec3f pos;

					if (isFirst_)
					{
						axis = (vs_[3].Pos - vs_[1].Pos);
						axis = SafeNormalize(axis);
						axisBefore = axis;
					}
					else if (isLast_)
					{
						axis = axisBefore;
					}
					else
					{
						Effekseer::Vec3f axisOld = axisBefore;
						axis = vs_[9].Pos - vs_[7].Pos;
						axis = SafeNormalize(axis);
						axisBefore = axis;

						axis = (axisBefore + axisOld) / 2.0f;
					}

					pos = vs_[1].Pos;

					VERTEX vl = vs_[0];
					VERTEX vm = vs_[1];
					VERTEX vr = vs_[5];

					vm.Pos.X = 0.0f;
					vm.Pos.Y = 0.0f;
					vm.Pos.Z = 0.0f;

					::Effekseer::Vec3f F;
					::Effekseer::Vec3f R;
					::Effekseer::Vec3f U;

					// It can be optimized because X is only not zero.
					/*
					U = axis;

					F = ::Effekseer::Vec3f(m_renderer->GetCameraFrontDirection()).Normalize();
					R = ::Effekseer::Vec3f::Cross(U, F).Normalize();
					F = ::Effekseer::Vec3f::Cross(R, U).Normalize();
					
					::Effekseer::Mat43f mat_rot(
						-R.GetX(), -R.GetY(), -R.GetZ(),
						 U.GetX(),  U.GetY(),  U.GetZ(),
						 F.GetX(),  F.GetY(),  F.GetZ(),
						pos.GetX(), pos.GetY(), pos.GetZ());

					vl.Pos = ToStruct(::Effekseer::Vec3f::Transform(vl.Pos, mat_rot));
					vm.Pos = ToStruct(::Effekseer::Vec3f::Transform(vm.Pos, mat_rot));
					vr.Pos = ToStruct(::Effekseer::Vec3f::Transform(vr.Pos,mat_rot));
					*/

					U = axis;
					F = m_renderer->GetCameraFrontDirection();
					R = SafeNormalize(::Effekseer::Vec3f::Cross(U, F));

					assert(vl.Pos.Y == 0.0f);
					assert(vr.Pos.Y == 0.0f);
					assert(vl.Pos.Z == 0.0f);
					assert(vr.Pos.Z == 0.0f);
					assert(vm.Pos.X == 0.0f);
					assert(vm.Pos.Y == 0.0f);
					assert(vm.Pos.Z == 0.0f);

					vl.Pos = ToStruct(-R * vl.Pos.X + pos);
					vm.Pos = ToStruct(pos);
					vr.Pos = ToStruct(-R * vr.Pos.X + pos);

					if (vertexType == VertexType::Distortion)
					{
						auto vl_ = (VERTEX_DISTORTION*)(&vl);
						auto vm_ = (VERTEX_DISTORTION*)(&vm);
						auto vr_ = (VERTEX_DISTORTION*)(&vr);

						vl_->Binormal = vm_->Binormal = vr_->Binormal = ToStruct(axis);

						::Effekseer::Vec3f tangent = vl_->Pos - vr_->Pos;
						tangent.Normalize();

						vl_->Tangent = vm_->Tangent = vr_->Tangent = ToStruct(tangent);
					}
					else if (vertexType == VertexType::Dynamic)
					{
						auto vl_ = (DynamicVertex*)(&vl);
						auto vm_ = (DynamicVertex*)(&vm);
						auto vr_ = (DynamicVertex*)(&vr);

						::Effekseer::Vec3f tangent = SafeNormalize(Effekseer::Vec3f(vl_->Pos - vr_->Pos));
						Effekseer::Vec3f normal = SafeNormalize(Effekseer::Vec3f::Cross(tangent, axis));

						if (!parameter.IsRightHand)
						{
							normal = -normal;
						}

						Effekseer::Color normal_ = PackVector3DF(normal);
						Effekseer::Color tangent_ = PackVector3DF(tangent);

						vl_->Normal = normal_;
						vm_->Normal = normal_;
						vr_->Normal = normal_;

						vl_->Tangent = tangent_;
						vm_->Tangent = tangent_;
						vr_->Tangent = tangent_;
					}

					if (isFirst_)
					{
						vs_[0] = vl;
						vs_[1] = vm;
						vs_[4] = vm;
						vs_[5] = vr;
						vs_ += 2;

					}
					else if (isLast_)
					{
						vs_[0] = vl;
						vs_[1] = vm;
						vs_[4] = vm;
						vs_[5] = vr;
						vs_ += 6;
					}
					else
					{
						vs_[0] = vl;
						vs_[1] = vm;
						vs_[4] = vm;
						vs_[5] = vr;

						vs_[6] = vl;
						vs_[7] = vm;
						vs_[10] = vm;
						vs_[11] = vr;

						vs_ += 8;
					}
				}
			}

			// calculate UV
			AssignUVs<VERTEX, 0>(parameter, verteies);

			if (vertexType == VertexType::Dynamic) 
			{
				AssignUVs<VERTEX, 1>(parameter, verteies);
			}

#ifdef __EFFEKSEER_BUILD_VERSION16__
			AssignUVs<VERTEX, 2>(parameter, verteies);
#endif

			// custom parameter
			if (customData1Count_ > 0)
			{
				StrideView<float> custom(m_ringBufferData + sizeof(DynamicVertex), stride_, vertexCount_);
				for (size_t loop = 0; loop < instances.size() - 1; loop++)
				{
					auto& param = instances[loop];
					
					for (int32_t sploop = 0; sploop < parameter.SplineDivision; sploop++)
					{
						for (size_t i = 0; i < 8; i++)
						{
							auto c = (float*)(&custom[0]);
							memcpy(c, param.CustomData1.data(), sizeof(float) * customData1Count_);
							custom += 1;
						}
					}
				}
			}

			if (customData2Count_ > 0)
			{
				StrideView<float> custom(
					m_ringBufferData + sizeof(DynamicVertex) + sizeof(float) * customData1Count_, stride_, vertexCount_);
				for (size_t loop = 0; loop < instances.size() - 1; loop++)
				{
					auto& param = instances[loop];

					for (int32_t sploop = 0; sploop < parameter.SplineDivision; sploop++)
					{
						for (size_t i = 0; i < 8; i++)
						{
							auto c = (float*)(&custom[0]);
							memcpy(c, param.CustomData2.data(), sizeof(float) * customData2Count_);
							custom += 1;
						}
					}
				}
			}
		}

	public:

		TrackRendererBase(RENDERER* renderer)
			: m_renderer(renderer)
			, m_ribbonCount(0)
			, m_ringBufferOffset(0)
			, m_ringBufferData(NULL)
		{
		}

		virtual ~TrackRendererBase()
		{
		}


	protected:

		void Rendering_(const efkTrackNodeParam& parameter, const efkTrackInstanceParam& instanceParameter, void* userData, const ::Effekseer::Mat44f& camera)
		{
			const auto& state = m_renderer->GetStandardRenderer()->GetState();

			if ((state.MaterialPtr != nullptr && !state.MaterialPtr->IsSimpleVertex) ||	
				parameter.BasicParameterPtr->MaterialType == Effekseer::RendererMaterialType::Lighting)
			{
				Rendering_Internal<DynamicVertex>(parameter, instanceParameter, userData, camera);
			}
			else if (parameter.BasicParameterPtr->MaterialType == Effekseer::RendererMaterialType::BackDistortion)
			{
				Rendering_Internal<VERTEX_DISTORTION>(parameter, instanceParameter, userData, camera);
			}
			else
			{
				Rendering_Internal<VERTEX_NORMAL>(parameter, instanceParameter, userData, camera);
			}
		}

		template<typename VERTEX>
		void Rendering_Internal(const efkTrackNodeParam& parameter, const efkTrackInstanceParam& instanceParameter, void* userData, const ::Effekseer::Mat44f& camera)
		{
			if (m_ringBufferData == NULL) return;
			if (instanceParameter.InstanceCount < 2) return;

			const efkTrackInstanceParam& param = instanceParameter;

			bool isFirst = param.InstanceIndex == 0;
			bool isLast = param.InstanceIndex == (param.InstanceCount - 1);

			if (isFirst)
			{
				instances.reserve(param.InstanceCount);
				instances.resize(0);
				innstancesNodeParam = parameter;
			}

			instances.push_back(param);

			if (isLast)
			{
				RenderSplines<VERTEX>(camera);
			}
		}

	public:

		void Rendering(const efkTrackNodeParam& parameter, const efkTrackInstanceParam& instanceParameter, void* userData) override
		{
			Rendering_(parameter, instanceParameter, userData, m_renderer->GetCameraMatrix());
		}

		void BeginRenderingGroup(const efkTrackNodeParam& param, int32_t count, void* userData) override
		{
			m_ribbonCount = 0;
			int32_t vertexCount = ((count - 1) * param.SplineDivision) * 8;
			if (vertexCount <= 0) return;

			EffekseerRenderer::StandardRendererState state;
			state.AlphaBlend = param.BasicParameterPtr->AlphaBlend;
			state.CullingType = ::Effekseer::CullingType::Double;
			state.DepthTest = param.ZTest;
			state.DepthWrite = param.ZWrite;
			state.TextureFilter1 = param.BasicParameterPtr->TextureFilter1;
			state.TextureWrap1 = param.BasicParameterPtr->TextureWrap1;
			state.TextureFilter2 = param.BasicParameterPtr->TextureFilter2;
			state.TextureWrap2 = param.BasicParameterPtr->TextureWrap2;
#ifdef __EFFEKSEER_BUILD_VERSION16__
			state.TextureFilter3 = param.BasicParameterPtr->TextureFilter3;
			state.TextureWrap3 = param.BasicParameterPtr->TextureWrap3;

			state.EnableInterpolation = param.BasicParameterPtr->EnableInterpolation;
			state.UVLoopType = param.BasicParameterPtr->UVLoopType;
			state.InterpolationType = param.BasicParameterPtr->InterpolationType;
			state.FlipbookDivideX = param.BasicParameterPtr->FlipbookDivideX;
			state.FlipbookDivideY = param.BasicParameterPtr->FlipbookDivideY;
#endif

			state.Distortion = param.BasicParameterPtr->MaterialType == Effekseer::RendererMaterialType::BackDistortion;
			state.DistortionIntensity = param.BasicParameterPtr->DistortionIntensity;
			state.MaterialType = param.BasicParameterPtr->MaterialType;

			state.CopyMaterialFromParameterToState(param.EffectPointer,
												   param.BasicParameterPtr->MaterialParameterPtr,
												   param.BasicParameterPtr->Texture1Index,
												   param.BasicParameterPtr->Texture2Index
#ifdef __EFFEKSEER_BUILD_VERSION16__
												   , param.BasicParameterPtr->Texture3Index
#endif
			);
			customData1Count_ = state.CustomData1Count;
			customData2Count_ = state.CustomData2Count;

			m_renderer->GetStandardRenderer()->UpdateStateAndRenderingIfRequired(state);

			m_renderer->GetStandardRenderer()->BeginRenderingAndRenderingIfRequired(vertexCount, stride_, (void*&)m_ringBufferData);
			vertexCount_ = vertexCount;
		}
	};

	//----------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------
}
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#endif	// __EFFEKSEERRENDERER_RIBBON_RENDERER_H__

#ifndef	__EFFEKSEERRENDERER_VERTEXBUFFER_BASE_H__
#define	__EFFEKSEERRENDERER_VERTEXBUFFER_BASE_H__

//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------
#include "EffekseerNative.h"
#include <assert.h>
#include <string.h>

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
namespace EffekseerRenderer
{
//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
class VertexBufferBase
{
protected:
	bool					m_isDynamic;
	int						m_size;
	int						m_offset;
	uint8_t*				m_resource;
	bool					m_isLock;

public:
	VertexBufferBase( int size, bool isDynamic );
	virtual ~VertexBufferBase();

	virtual void Lock() = 0;
	virtual bool RingBufferLock(int32_t size, int32_t& offset, void*& data, int32_t alignment) = 0;

	/**
		@brief	try lock as a ring buffer. if failed, it does't lock it.
	*/
	virtual bool TryRingBufferLock(int32_t size, int32_t& offset, void*& data, int32_t alignment) = 0;

	virtual void Unlock() = 0;
	virtual void Push( const void* buffer, int size );
	virtual int GetMaxSize() const;
	virtual int GetSize() const;
	virtual void* GetBufferDirect( int size );
};

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
}
//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
#endif	// __EFFEKSEERRENDERER_VERTEXBUFFER_BASE_H__

#ifndef __EFFEKSEERRENDERER_GL_BASE_PRE_H__
#define __EFFEKSEERRENDERER_GL_BASE_PRE_H__

#include "EffekseerNative.h"
#include <vector>

#ifdef _WIN32
#include <Windows.h>
#endif

#if defined(__EFFEKSEER_RENDERER_GLES2__)

#if defined(__APPLE__)
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#else
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#endif

#elif defined(__EFFEKSEER_RENDERER_GLES3__)

#if defined(__APPLE__)
#include <OpenGLES/ES3/gl.h>
#else
#define GL_GLEXT_PROTOTYPES
#include <GLES3/gl3.h>
#endif

#elif defined(__EFFEKSEER_RENDERER_GL2__)

#if _WIN32
#include <GL/gl.h>
#elif defined(__APPLE__)
#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#else

#if _WIN32
#include <GL/gl.h>
#elif defined(__APPLE__)
#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl3.h>
#else
#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#endif

#endif

#if _WIN32
#pragma comment(lib, "gdiplus.lib")
#pragma comment(lib, "opengl32.lib")
#endif

namespace EffekseerRendererGL
{

class Renderer;

enum class OpenGLDeviceType
{
	OpenGL2,
	OpenGL3,
	OpenGLES2,
	OpenGLES3,
	Emscripten,
};

} // namespace EffekseerRendererGL

#endif // __EFFEKSEERRENDERER_GL_BASE_PRE_H__


#ifndef	__EFFEKSEERRENDERER_GL_RENDERER_H__
#define	__EFFEKSEERRENDERER_GL_RENDERER_H__

//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
// Lib
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace EffekseerRendererGL
{

class DeviceObjectCollection;

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------

/**
@brief	テクスチャ読込クラスを生成する。
*/
::Effekseer::TextureLoader* CreateTextureLoader(::Effekseer::FileInterface* fileInterface = nullptr, ::Effekseer::ColorSpaceType colorSpaceType = ::Effekseer::ColorSpaceType::Gamma);

/**
@brief	モデル読込クラスを生成する。
*/
::Effekseer::ModelLoader* CreateModelLoader(::Effekseer::FileInterface* fileInterface = NULL);

/**
	@brief	描画クラス
*/
class Renderer
	: public ::EffekseerRenderer::Renderer
{
protected:
	Renderer() {}
	virtual ~Renderer() {}

public:
	/**
	@brief
	\~english	Create an instance
	\~japanese	インスタンスを生成する。
	@param	squareMaxCount
	\~english	the number of maximum sprites
	\~japanese	最大描画スプライト数
	@param	deviceType
	\~english	device type of opengl
	\~japanese	デバイスの種類
	@param	deviceObjectCollection
	\~english	for a middleware. it should be nullptr.
	\~japanese	ミドルウェア向け。 nullptrにすべきである。
	@return
	\~english	instance
	\~japanese	インスタンス
	*/
	static Renderer* Create(int32_t squareMaxCount,
							OpenGLDeviceType deviceType = OpenGLDeviceType::OpenGL2,
							DeviceObjectCollection* deviceObjectCollection = nullptr);

	/**
		@brief	最大描画スプライト数を取得する。
	*/
	virtual int32_t GetSquareMaxCount() const = 0;

	/**
		@brief	最大描画スプライト数を設定する。
		@note
		描画している時は使用できない。
	*/
	virtual void SetSquareMaxCount(int32_t count) = 0;

	/**
	@brief
	\~english	Get a background.
	\~japanese	背景を取得する。
	*/
	virtual Effekseer::TextureData* GetBackground() = 0;

	/**
	@brief	
	\~english	Specify a background.
	\~japanese	背景を設定する。
	*/
	virtual void SetBackground(GLuint background, bool hasMipmap = false) = 0;

	/**
	@brief	
	\~english get a device type
	\~japanese デバイスの種類を取得する。
	*/
	virtual OpenGLDeviceType GetDeviceType() const = 0;

	/**
	@brief
	\~english get whether VAO is supported
	\~japanese VAOがサポートされているか取得する。
	*/
	virtual bool IsVertexArrayObjectSupported() const = 0;
};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
/**
	@brief	モデル
*/
class Model
	: public Effekseer::Model
{
private:

public:
	struct InternalModel
	{
		GLuint		VertexBuffer;
		GLuint		IndexBuffer;
		int32_t		VertexCount;
		int32_t		IndexCount;

		std::vector<uint8_t> delayVertexBuffer;
		std::vector<uint8_t> delayIndexBuffer;

		InternalModel();

		virtual ~InternalModel();

		bool TryDelayLoad();
	};


	InternalModel*				InternalModels = nullptr;
	int32_t						ModelCount;
	bool IsLoadedOnGPU = false;

	Model(void* data, int32_t size);
	~Model();

	bool LoadToGPU();
};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
}
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#endif	// __EFFEKSEERRENDERER_GL_RENDERER_H__

#ifndef	__EFFEKSEERRENDERER_GL_MODELLOADER_H__
#define	__EFFEKSEERRENDERER_GL_MODELLOADER_H__

//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
namespace EffekseerRendererGL
{
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
class ModelLoader
	: public ::Effekseer::ModelLoader
{
private:
	::Effekseer::FileInterface* m_fileInterface;
	::Effekseer::DefaultFileInterface m_defaultFileInterface;

public:
	ModelLoader( ::Effekseer::FileInterface* fileInterface );
	virtual ~ModelLoader();

public:
	void* Load( const EFK_CHAR* path ) override;

	void* Load(const void* data, int32_t size) override;

	void Unload( void* data ) override;
};
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
}
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#endif	// __EFFEKSEERRENDERER_GL_MODELLOADER_H__


#ifndef __EFFEKSEERRENDERER_GL_MATERIALLOADER_H__
#define __EFFEKSEERRENDERER_GL_MATERIALLOADER_H__


namespace Effekseer
{
class Material;
class CompiledMaterialBinary;
} // namespace Effekseer

namespace EffekseerRendererGL
{

class DeviceObjectCollection;

class MaterialLoader : public ::Effekseer::MaterialLoader
{
private:
	OpenGLDeviceType deviceType_;
	Renderer* renderer_ = nullptr;
	DeviceObjectCollection* deviceObjectCollection_ = nullptr;
	bool canLoadFromCache_ = false;

	::Effekseer::FileInterface* fileInterface_ = nullptr;
	::Effekseer::DefaultFileInterface defaultFileInterface_;

	::Effekseer::MaterialData* LoadAcutually(::Effekseer::Material& material, ::Effekseer::CompiledMaterialBinary* binary);

public:
	MaterialLoader(OpenGLDeviceType deviceType,
				   Renderer* renderer,
				   DeviceObjectCollection* deviceObjectCollection,
				   ::Effekseer::FileInterface* fileInterface,
				   bool canLoadFromCache = true);
	virtual ~MaterialLoader();

	::Effekseer::MaterialData* Load(const EFK_CHAR* path) override;

	::Effekseer::MaterialData* Load(const void* data, int32_t size, Effekseer::MaterialFileType fileType) override;

	void Unload(::Effekseer::MaterialData* data) override;
};

} // namespace EffekseerRendererGL

#endif // __EFFEKSEERRENDERER_GL_MODELLOADER_H__


#ifndef __EFFEKSEERRENDERER_GL_DEVICEOBJECT_COLLECTION_H__
#define __EFFEKSEERRENDERER_GL_DEVICEOBJECT_COLLECTION_H__

#include <set>
#include "EffekseerNative.h"

namespace EffekseerRendererGL
{

class DeviceObject;

class DeviceObjectCollection : public ::Effekseer::ReferenceObject
{
	friend class DeviceObject;

private:
	std::set<DeviceObject*> deviceObjects_;

	/**
		@brief	register an object
	*/
	void Register(DeviceObject* device);

	/**
		@brief	unregister an object
	*/
	void Unregister(DeviceObject* device);

public:
	DeviceObjectCollection() = default;

	~DeviceObjectCollection() = default;

	/**
		@brief
		\~english Call when device lost causes
		\~japanese デバイスロストが発生した時に実行する。
	*/
	void OnLostDevice();

	/**
		@brief
		\~english Call when device reset causes
		\~japanese デバイスがリセットされた時に実行する。
	*/
	void OnResetDevice();
};

} // namespace EffekseerRendererGL

#endif // __EFFEKSEERRENDERER_GL_DEVICEOBJECT_H__

#pragma once

#include <iostream>
#undef min

namespace Effekseer
{
namespace GLSL
{

static const char* material_common_define_450 = R"(
#version 450
#define LAYOUT(ind) layout(location = ind)
#define lowp
#define mediump
#define highp
#define IN in
#define OUT out
#define CENTROID centroid

)";

static const char* material_common_define_not_450 = R"(
#define LAYOUT(ind) 
)";

static const char* material_common_define =
	R"(
#define MOD mod
#define FRAC fract
#define LERP mix

float atan2(in float y, in float x) {
    return x == 0.0 ? sign(y)* 3.141592 / 2.0 : atan(y, x);
}

)";

static const char* material_common_vs_define = R"()"

											   R"(

#define TEX2D textureLod

)";

static const char* material_common_fs_define = R"()"

											   R"(

#define TEX2D texture

)";

static const char g_material_model_vs_src_pre[] =
	R"(

LAYOUT(0) IN vec4 a_Position;
LAYOUT(1) IN vec3 a_Normal;
LAYOUT(2) IN vec3 a_Binormal;
LAYOUT(3) IN vec3 a_Tangent;
LAYOUT(4) IN vec2 a_TexCoord;
LAYOUT(5) IN vec4 a_Color;
)"
#if defined(MODEL_SOFTWARE_INSTANCING)
	R"(
IN float a_InstanceID;
IN vec4 a_UVOffset;
IN vec4 a_ModelColor;
)"
#endif

	R"(

LAYOUT(0) CENTROID OUT lowp vec4 v_VColor;
LAYOUT(1) CENTROID OUT mediump vec2 v_UV1;
LAYOUT(2) CENTROID OUT mediump vec2 v_UV2;
LAYOUT(3) OUT mediump vec3 v_WorldP;
LAYOUT(4) OUT mediump vec3 v_WorldN;
LAYOUT(5) OUT mediump vec3 v_WorldT;
LAYOUT(6) OUT mediump vec3 v_WorldB;
LAYOUT(7) OUT mediump vec2 v_ScreenUV;
//$C_OUT1$
//$C_OUT2$
)";

static const char g_material_model_vs_src_pre_uniform[] =

	R"(
uniform mat4 ProjectionMatrix;
)"
#if defined(MODEL_SOFTWARE_INSTANCING)
	R"(
uniform mat4 ModelMatrix[20];
uniform vec4 UVOffset[20];
uniform vec4 ModelColor[20];
)"
#else
	R"(
uniform mat4 ModelMatrix;
uniform vec4 UVOffset;
uniform vec4 ModelColor;
)"
#endif
	R"(
uniform vec4 mUVInversed;
uniform vec4 predefined_uniform;
uniform vec4 cameraPosition;
)";

static const char g_material_model_vs_src_suf1[] =
	R"(

vec2 GetUV(vec2 uv)
{
	uv.y = mUVInversed.x + mUVInversed.y * uv.y;
	return uv;
}

vec2 GetUVBack(vec2 uv)
{
	uv.y = mUVInversed.z + mUVInversed.w * uv.y;
	return uv;
}


void main()
{
)"
#if defined(MODEL_SOFTWARE_INSTANCING)
	R"(
	mat4 modelMatrix = ModelMatrix[int(a_InstanceID)];
	vec4 uvOffset = a_UVOffset;
	vec4 modelColor = a_ModelColor;
)"
#else
	R"(
	mat4 modelMatrix = ModelMatrix;
	vec4 uvOffset = UVOffset;
	vec4 modelColor = ModelColor * a_Color;
)"
#endif
	R"(
	mat3 modelMatRot = mat3(modelMatrix);
	vec3 worldPos = (modelMatrix * a_Position).xyz;
	vec3 worldNormal = normalize(modelMatRot * a_Normal);
	vec3 worldBinormal = normalize(modelMatRot * a_Binormal);
	vec3 worldTangent = normalize(modelMatRot * a_Tangent);
	vec3 objectScale = vec3(1.0, 1.0, 1.0);

	// Calculate ObjectScale
	objectScale.x = length(modelMatRot * vec3(1.0, 0.0, 0.0));
	objectScale.y = length(modelMatRot * vec3(0.0, 1.0, 0.0));
	objectScale.z = length(modelMatRot * vec3(0.0, 0.0, 1.0));

	// UV
	vec2 uv1 = a_TexCoord.xy * uvOffset.zw + uvOffset.xy;
	vec2 uv2 = uv1;

	//uv1.y = mUVInversed.x + mUVInversed.y * uv1.y;
	//uv1.y = mUVInversed.x + mUVInversed.y * uv1.y;

	vec3 pixelNormalDir = worldNormal;
	
	vec4 vcolor = modelColor;
)";

static const char g_material_model_vs_src_suf2[] =
	R"(
	worldPos = worldPos + worldPositionOffset;

	v_WorldP = worldPos;
	v_WorldN = worldNormal;
	v_WorldB = worldBinormal;
	v_WorldT = worldTangent;
	v_UV1 = uv1;
	v_UV2 = uv2;
	v_VColor = vcolor;
	gl_Position = ProjectionMatrix * vec4(worldPos, 1.0);
	v_ScreenUV.xy = gl_Position.xy / gl_Position.w;
	v_ScreenUV.xy = vec2(v_ScreenUV.x + 1.0, v_ScreenUV.y + 1.0) * 0.5;

	#ifdef _Y_INVERTED_
	gl_Position.y = - gl_Position.y;
	#endif
}
)";

static const char g_material_sprite_vs_src_pre_simple[] =
	R"(

LAYOUT(0) IN vec4 atPosition;
LAYOUT(1) IN vec4 atColor;
LAYOUT(2) IN vec4 atTexCoord;
)"

	R"(
LAYOUT(0) CENTROID OUT lowp vec4 v_VColor;
LAYOUT(1) CENTROID OUT mediump vec2 v_UV1;
LAYOUT(2) CENTROID OUT mediump vec2 v_UV2;
LAYOUT(3) OUT mediump vec3 v_WorldP;
LAYOUT(4) OUT mediump vec3 v_WorldN;
LAYOUT(5) OUT mediump vec3 v_WorldT;
LAYOUT(6) OUT mediump vec3 v_WorldB;
LAYOUT(7) OUT mediump vec2 v_ScreenUV;
)";

static const char g_material_sprite_vs_src_pre_simple_uniform[] =
	R"(
uniform mat4 uMatCamera;
uniform mat4 uMatProjection;
uniform vec4 mUVInversed;
uniform vec4 predefined_uniform;
uniform vec4 cameraPosition;

)";

static const char g_material_sprite_vs_src_pre[] =
	R"(

LAYOUT(0) IN vec4 atPosition;
LAYOUT(1) IN vec4 atColor;
LAYOUT(2) IN vec3 atNormal;
LAYOUT(3) IN vec3 atTangent;
LAYOUT(4) IN vec2 atTexCoord;
LAYOUT(5) IN vec2 atTexCoord2;
//$C_IN1$
//$C_IN2$
)"

	R"(
LAYOUT(0) CENTROID OUT lowp vec4 v_VColor;
LAYOUT(1) CENTROID OUT mediump vec2 v_UV1;
LAYOUT(2) CENTROID OUT mediump vec2 v_UV2;
LAYOUT(3) OUT mediump vec3 v_WorldP;
LAYOUT(4) OUT mediump vec3 v_WorldN;
LAYOUT(5) OUT mediump vec3 v_WorldT;
LAYOUT(6) OUT mediump vec3 v_WorldB;
LAYOUT(7) OUT mediump vec2 v_ScreenUV;
//$C_OUT1$
//$C_OUT2$
)";

static const char g_material_sprite_vs_src_pre_uniform[] =

	R"(
uniform mat4 uMatCamera;
uniform mat4 uMatProjection;
uniform vec4 mUVInversed;
uniform vec4 predefined_uniform;
uniform vec4 cameraPosition;

)";

static const char g_material_sprite_vs_src_suf1_simple[] =

	R"(

vec2 GetUV(vec2 uv)
{
	uv.y = mUVInversed.x + mUVInversed.y * uv.y;
	return uv;
}

vec2 GetUVBack(vec2 uv)
{
	uv.y = mUVInversed.z + mUVInversed.w * uv.y;
	return uv;
}

void main() {
	vec3 worldPos = atPosition.xyz;
	vec3 objectScale = vec3(1.0, 1.0, 1.0);

	// UV
	vec2 uv1 = atTexCoord.xy;
	//uv1.y = mUVInversed.x + mUVInversed.y * uv1.y;
	vec2 uv2 = uv1;

	// NBT
	vec3 worldNormal = vec3(0.0, 0.0, 0.0);
	vec3 worldBinormal = vec3(0.0, 0.0, 0.0);
	vec3 worldTangent = vec3(0.0, 0.0, 0.0);
	v_WorldN = worldNormal;
	v_WorldB = worldBinormal;
	v_WorldT = worldTangent;

	vec3 pixelNormalDir = worldNormal;
	vec4 vcolor = atColor;
)";

static const char g_material_sprite_vs_src_suf1[] =

	R"(

vec2 GetUV(vec2 uv)
{
	uv.y = mUVInversed.x + mUVInversed.y * uv.y;
	return uv;
}

vec2 GetUVBack(vec2 uv)
{
	uv.y = mUVInversed.z + mUVInversed.w * uv.y;
	return uv;
}

void main() {
	vec3 worldPos = atPosition.xyz;
	vec3 objectScale = vec3(1.0, 1.0, 1.0);

	// UV
	vec2 uv1 = atTexCoord.xy;
	//uv1.y = mUVInversed.x + mUVInversed.y * uv1.y;
	vec2 uv2 = atTexCoord2.xy;
	//uv2.y = mUVInversed.x + mUVInversed.y * uv2.y;

	// NBT
	vec3 worldNormal = (atNormal - vec3(0.5, 0.5, 0.5)) * 2.0;
	vec3 worldTangent = (atTangent - vec3(0.5, 0.5, 0.5)) * 2.0;
	vec3 worldBinormal = cross(worldNormal, worldTangent);

	v_WorldN = worldNormal;
	v_WorldB = worldBinormal;
	v_WorldT = worldTangent;
	vec3 pixelNormalDir = worldNormal;
	vec4 vcolor = atColor;
)";

static const char g_material_sprite_vs_src_suf2[] =

	R"(
	worldPos = worldPos + worldPositionOffset;

	vec4 cameraPos = uMatCamera * vec4(worldPos, 1.0);
	cameraPos = cameraPos / cameraPos.w;

	gl_Position = uMatProjection * cameraPos;

	v_WorldP = worldPos;
	v_VColor = vcolor;

	v_UV1 = uv1;
	v_UV2 = uv2;
	v_ScreenUV.xy = gl_Position.xy / gl_Position.w;
	v_ScreenUV.xy = vec2(v_ScreenUV.x + 1.0, v_ScreenUV.y + 1.0) * 0.5;

	#ifdef _Y_INVERTED_
	gl_Position.y = - gl_Position.y;
	#endif
}

)";

static const char g_material_fs_src_pre[] =
	R"(

LAYOUT(0) CENTROID IN lowp vec4 v_VColor;
LAYOUT(1) CENTROID IN mediump vec2 v_UV1;
LAYOUT(2) CENTROID IN mediump vec2 v_UV2;
LAYOUT(3) IN mediump vec3 v_WorldP;
LAYOUT(4) IN mediump vec3 v_WorldN;
LAYOUT(5) IN mediump vec3 v_WorldT;
LAYOUT(6) IN mediump vec3 v_WorldB;
LAYOUT(7) IN mediump vec2 v_ScreenUV;
//$C_PIN1$
//$C_PIN2$

)";

static const char g_material_fs_src_pre_uniform[] =
	R"(

uniform vec4 mUVInversedBack;
uniform vec4 predefined_uniform;
uniform vec4 cameraPosition;

)";

static const char g_material_fs_src_suf1[] =
	R"(

vec2 GetUV(vec2 uv)
{
	uv.y = mUVInversedBack.x + mUVInversedBack.y * uv.y;
	return uv;
}

vec2 GetUVBack(vec2 uv)
{
	uv.y = mUVInversedBack.z + mUVInversedBack.w * uv.y;
	return uv;
}

#ifdef _MATERIAL_LIT_

const float lightScale = 3.14;

float saturate(float v)
{
	return max(min(v, 1.0), 0.0);
}

float calcD_GGX(float roughness, float dotNH)
{
	float alpha = roughness*roughness;
	float alphaSqr = alpha*alpha;
	float pi = 3.14159;
	float denom = dotNH * dotNH *(alphaSqr-1.0) + 1.0;
	return (alpha / denom) * (alpha / denom) / pi;
}

float calcF(float F0, float dotLH)
{
	float dotLH5 = pow(1.0-dotLH,5.0);
	return F0 + (1.0-F0)*(dotLH5);
}

float calcG_Schlick(float roughness, float dotNV, float dotNL)
{
	// UE4
	float k = (roughness + 1.0) * (roughness + 1.0) / 8.0;
	// float k = roughness * roughness / 2.0;

	float gV = dotNV*(1.0 - k) + k;
	float gL = dotNL*(1.0 - k) + k;

	return 1.0 / (gV * gL);
}

float calcLightingGGX(vec3 N, vec3 V, vec3 L, float roughness, float F0)
{
	vec3 H = normalize(V+L);

	float dotNL = saturate( dot(N,L) );
	float dotLH = saturate( dot(L,H) );
	float dotNH = saturate( dot(N,H) ) - 0.001;
	float dotNV = saturate( dot(N,V) ) + 0.001;

	float D = calcD_GGX(roughness, dotNH);
	float F = calcF(F0, dotLH);
	float G = calcG_Schlick(roughness, dotNV, dotNL);

	return dotNL * D * F * G / 4.0;
}

vec3 calcDirectionalLightDiffuseColor(vec3 diffuseColor, vec3 normal, vec3 lightDir, float ao)
{
	vec3 color = vec3(0.0,0.0,0.0);

	float NoL = dot(normal,lightDir);
	color.xyz = lightColor.xyz * lightScale * max(NoL,0.0) * ao / 3.14;
	color.xyz = color.xyz * diffuseColor.xyz;
	return color;
}

#endif

void main()
{
	vec2 uv1 = v_UV1;
	vec2 uv2 = v_UV2;
	vec3 worldPos = v_WorldP;
	vec3 worldNormal = v_WorldN;
	vec3 worldTangent = v_WorldT;
	vec3 worldBinormal = v_WorldB;
	vec3 pixelNormalDir = worldNormal;
	vec4 vcolor = v_VColor;
	vec3 objectScale = vec3(1.0, 1.0, 1.0);
)";

static const char g_material_fs_src_suf2_lit[] =
	R"(

	vec3 viewDir = normalize(cameraPosition.xyz - worldPos);
	vec3 diffuse = calcDirectionalLightDiffuseColor(baseColor, pixelNormalDir, lightDirection.xyz, ambientOcclusion);
	vec3 specular = lightColor.xyz * lightScale * calcLightingGGX(pixelNormalDir, viewDir, lightDirection.xyz, roughness, 0.9);

	vec4 Output =  vec4(metallic * specular + (1.0 - metallic) * diffuse + baseColor * lightAmbientColor.xyz * ambientOcclusion, opacity);
	Output.xyz = Output.xyz + emissive.xyz;

	if(opacityMask <= 0.0) discard;
	if(opacity <= 0.0) discard;

	FRAGCOLOR = Output;
}

)";

static const char g_material_fs_src_suf2_unlit[] =
	R"(

	if(opacityMask <= 0.0) discard;
	if(opacity <= 0.0) discard;

	FRAGCOLOR = vec4(emissive, opacity);
}

)";

static const char g_material_fs_src_suf2_refraction[] =
	R"(
	float airRefraction = 1.0;

	vec3 dir = mat3(cameraMat) * pixelNormalDir;
	vec2 distortUV = dir.xy * (refraction - airRefraction);

	distortUV += v_ScreenUV;
	distortUV = GetUVBack(distortUV);	

	#ifdef _Y_INVERTED_
	distortUV.y = 1.0f - distortUV.y;
	#endif

	vec4 bg = TEX2D(background, distortUV);
	FRAGCOLOR = bg;

	if(opacityMask <= 0.0) discard;
	if(opacity <= 0.0) discard;
}

)";

struct ShaderData
{
	std::string CodeVS;
	std::string CodePS;
};

class ShaderGenerator
{
	bool useUniformBlock_ = false;
	bool useSet_ = false;
	int32_t textureBindingOffset_ = 0;

	std::string Replace(std::string target, std::string from_, std::string to_)
	{
		std::string::size_type Pos(target.find(from_));

		while (Pos != std::string::npos)
		{
			target.replace(Pos, from_.length(), to_);
			Pos = target.find(from_, Pos + to_.length());
		}

		return target;
	}

	std::string GetType(int32_t i)
	{
		if (i == 1)
			return "float";
		if (i == 2)
			return "vec2";
		if (i == 3)
			return "vec3";
		if (i == 4)
			return "vec4";
		if (i == 16)
			return "mat4";
		assert(0);
		return "";
	}

	std::string GetElement(int32_t i)
	{
		if (i == 1)
			return ".x";
		if (i == 2)
			return ".xy";
		if (i == 3)
			return ".xyz";
		if (i == 4)
			return ".xyzw";
		assert(0);
		return "";
	}

	void ExportUniform(std::ostringstream& maincode, int32_t type, const char* name)
	{
		maincode << "uniform " << GetType(type) << " " << name << ";" << std::endl;
	}

	void ExportTexture(std::ostringstream& maincode, const char* name, int bind, int stage)
	{
		if (useUniformBlock_)
		{
			if (useSet_)
			{
				maincode << "layout(set = " << stage << ", binding = " << (bind + textureBindingOffset_) << ") uniform sampler2D " << name
						 << ";" << std::endl;
			}
			else
			{
				maincode << "layout(binding = " << (bind + textureBindingOffset_) << ") uniform sampler2D " << name << ";" << std::endl;
			}
		}
		else
		{
			maincode << "uniform sampler2D " << name << ";" << std::endl;
		}
	}

	void ExportHeader(std::ostringstream& maincode, Material* material, int stage, bool isSprite, bool isOutputDefined, bool is450)
	{
		if (is450)
		{
			maincode << material_common_define_450;
		}
		else
		{
			maincode << material_common_define_not_450;
		}

		maincode << material_common_define;

		// Adhoc
		if (is450)
		{
			if (stage == 0)
			{
				maincode << material_common_vs_define;
			}
			else if (stage == 1)
			{
				maincode << material_common_fs_define;
			}
		}

		if (stage == 0)
		{
			if (isSprite)
			{
				if (material->GetIsSimpleVertex())
				{
					maincode << g_material_sprite_vs_src_pre_simple;
				}
				else
				{
					maincode << g_material_sprite_vs_src_pre;
				}
			}
			else
			{
				maincode << g_material_model_vs_src_pre;
			}
		}
		else
		{
			maincode << g_material_fs_src_pre;
		}

		if (isOutputDefined && stage == 1)
		{
			maincode << "#define FRAGCOLOR out_flagColor" << std::endl;
			maincode << "layout(location = 0) out vec4 out_flagColor;" << std::endl;
			maincode << std::endl;
		}
	}

	void ExportDefaultUniform(std::ostringstream& maincode, Material* material, int stage, bool isSprite)
	{
		if (stage == 0)
		{
			if (isSprite)
			{
				if (material->GetIsSimpleVertex())
				{
					maincode << g_material_sprite_vs_src_pre_simple_uniform;
				}
				else
				{
					maincode << g_material_sprite_vs_src_pre_uniform;
				}
			}
			else
			{
				maincode << g_material_model_vs_src_pre_uniform;
			}
		}
		else
		{
			maincode << g_material_fs_src_pre_uniform;
		}
	}

	void ExportMain(std::ostringstream& maincode,
					Material* material,
					int stage,
					bool isSprite,
					MaterialShaderType shaderType,
					const std::string& baseCode,
					bool useUniformBlock)
	{
		if (stage == 0)
		{
			if (isSprite)
			{
				if (material->GetIsSimpleVertex())
				{
					maincode << g_material_sprite_vs_src_suf1_simple;
				}
				else
				{
					maincode << g_material_sprite_vs_src_suf1;
				}
			}
			else
			{
				maincode << g_material_model_vs_src_suf1;
			}

			if (material->GetCustomData1Count() > 0)
			{
				if (isSprite)
				{
					maincode << GetType(material->GetCustomData1Count()) + " customData1 = atCustomData1;\n";
				}
				maincode << "v_CustomData1 = customData1" + GetElement(material->GetCustomData1Count()) + ";\n";
			}

			if (material->GetCustomData2Count() > 0)
			{
				if (isSprite)
				{
					maincode << GetType(material->GetCustomData2Count()) + " customData2 = atCustomData2;\n";
				}
				maincode << "v_CustomData2 = customData2" + GetElement(material->GetCustomData2Count()) + ";\n";
			}

			maincode << baseCode;

			if (isSprite)
			{
				maincode << g_material_sprite_vs_src_suf2;
			}
			else
			{
				maincode << g_material_model_vs_src_suf2;
			}
		}
		else
		{
			maincode << g_material_fs_src_suf1;

			if (material->GetCustomData1Count() > 0)
			{
				maincode << GetType(material->GetCustomData1Count()) + " customData1 = v_CustomData1;\n";
			}

			if (material->GetCustomData2Count() > 0)
			{
				maincode << GetType(material->GetCustomData2Count()) + " customData2 = v_CustomData2;\n";
			}

			maincode << baseCode;

			if (shaderType == MaterialShaderType::Refraction || shaderType == MaterialShaderType::RefractionModel)
			{
				maincode << g_material_fs_src_suf2_refraction;
			}
			else
			{
				if (material->GetShadingModel() == Effekseer::ShadingModelType::Lit)
				{
					maincode << g_material_fs_src_suf2_lit;
				}
				else if (material->GetShadingModel() == Effekseer::ShadingModelType::Unlit)
				{
					maincode << g_material_fs_src_suf2_unlit;
				}
			}
		}
	}

public:
	ShaderData GenerateShader(Material* material,
							  MaterialShaderType shaderType,
							  int32_t maximumTextureCount,
							  bool useUniformBlock,
							  bool isOutputDefined,
							  bool is450,
							  bool useSet,
							  int textureBindingOffset,
							  bool isYInverted = false)
	{
		useUniformBlock_ = useUniformBlock;
		useSet_ = useSet;
		textureBindingOffset_ = textureBindingOffset;

		bool isSprite = shaderType == MaterialShaderType::Standard || shaderType == MaterialShaderType::Refraction;
		bool isRefrection = material->GetHasRefraction() &&
							(shaderType == MaterialShaderType::Refraction || shaderType == MaterialShaderType::RefractionModel);

		ShaderData shaderData;

		for (int stage = 0; stage < 2; stage++)
		{
			std::ostringstream maincode;

			ExportHeader(maincode, material, stage, isSprite, isOutputDefined, is450);

			if (isYInverted)
			{
				maincode << "#define _Y_INVERTED_ 1" << std::endl;
			}

			int32_t actualTextureCount = std::min(maximumTextureCount, material->GetTextureCount());

			for (size_t i = 0; i < actualTextureCount; i++)
			{
				auto textureIndex = material->GetTextureIndex(i);
				auto textureName = material->GetTextureName(i);

				ExportTexture(maincode, textureName, i, stage);
			}

			for (size_t i = actualTextureCount; i < actualTextureCount + 1; i++)
			{
				ExportTexture(maincode, "background", i, stage);
			}

			// Uniform block begin
			if (useUniformBlock)
			{
				if (useSet_)
				{
					if (stage == 0)
					{
						maincode << "layout(set = 0, binding = 0) uniform Block {" << std::endl;
					}
					else if (stage == 1)
					{
						maincode << "layout(set = 1, binding = 0) uniform Block {" << std::endl;
					}
				}
				else
				{
					if (stage == 0)
					{
						maincode << "layout(binding = 0) uniform Block {" << std::endl;
					}
					else if (stage == 1)
					{
						maincode << "layout(binding = 0) uniform Block {" << std::endl;
					}
				}
			}

			ExportDefaultUniform(maincode, material, stage, isSprite);

			if (material->GetShadingModel() == ::Effekseer::ShadingModelType::Lit && stage == 1)
			{
				ExportUniform(maincode, 4, "lightDirection");
				ExportUniform(maincode, 4, "lightColor");
				ExportUniform(maincode, 4, "lightAmbientColor");

				maincode << "#define _MATERIAL_LIT_ 1" << std::endl;
			}
			else if (material->GetShadingModel() == ::Effekseer::ShadingModelType::Unlit)
			{
			}

			if (isRefrection && stage == 1)
			{
				ExportUniform(maincode, 16, "cameraMat");
			}

			if (!isSprite && stage == 0)
			{
				if (material->GetCustomData1Count() > 0)
				{
					maincode << "uniform vec4 customData1;" << std::endl;
				}
				if (material->GetCustomData2Count() > 0)
				{
					maincode << "uniform vec4 customData2;" << std::endl;
				}
			}

			for (int32_t i = 0; i < material->GetUniformCount(); i++)
			{
				auto uniformIndex = material->GetUniformIndex(i);
				auto uniformName = material->GetUniformName(i);

				ExportUniform(maincode, 4, uniformName);
			}

			// Uniform block end
			if (useUniformBlock)
			{
				maincode << "};" << std::endl;
			}

			auto baseCode = std::string(material->GetGenericCode());
			baseCode = Replace(baseCode, "$F1$", "float");
			baseCode = Replace(baseCode, "$F2$", "vec2");
			baseCode = Replace(baseCode, "$F3$", "vec3");
			baseCode = Replace(baseCode, "$F4$", "vec4");
			baseCode = Replace(baseCode, "$TIME$", "predefined_uniform.x");
			baseCode = Replace(baseCode, "$UV$", "uv");
			baseCode = Replace(baseCode, "$MOD", "mod");

			// replace textures
			for (size_t i = 0; i < actualTextureCount; i++)
			{
				auto textureIndex = material->GetTextureIndex(i);
				auto textureName = std::string(material->GetTextureName(i));

				std::string keyP = "$TEX_P" + std::to_string(textureIndex) + "$";
				std::string keyS = "$TEX_S" + std::to_string(textureIndex) + "$";

				if (stage == 0)
				{
					baseCode = Replace(baseCode, keyP, "TEX2D(" + textureName + ",GetUV(");
					baseCode = Replace(baseCode, keyS, "), 0.0)");
				}
				else
				{
					baseCode = Replace(baseCode, keyP, "TEX2D(" + textureName + ",GetUV(");
					baseCode = Replace(baseCode, keyS, "))");
				}
			}

			// invalid texture
			for (size_t i = actualTextureCount; i < material->GetTextureCount(); i++)
			{
				auto textureIndex = material->GetTextureIndex(i);
				auto textureName = std::string(material->GetTextureName(i));

				std::string keyP = "$TEX_P" + std::to_string(textureIndex) + "$";
				std::string keyS = "$TEX_S" + std::to_string(textureIndex) + "$";

				baseCode = Replace(baseCode, keyP, "vec4(");
				baseCode = Replace(baseCode, keyS, ",0.0,1.0)");
			}

			ExportMain(maincode, material, stage, isSprite, shaderType, baseCode, useUniformBlock);

			if (stage == 0)
			{
				shaderData.CodeVS = maincode.str();
			}
			else
			{
				shaderData.CodePS = maincode.str();
			}
		}

		// custom data
		int32_t layoutOffset = 6;
		int32_t pvLayoutOffset = 8;

		if (material->GetCustomData1Count() > 0)
		{
			if (isSprite)
			{
				shaderData.CodeVS = Replace(shaderData.CodeVS,
											"//$C_IN1$",
											"LAYOUT(" + std::to_string(layoutOffset) + ") " + "IN " +
												GetType(material->GetCustomData1Count()) + " atCustomData1;");
			}
			shaderData.CodeVS = Replace(shaderData.CodeVS,
										"//$C_OUT1$",
										"LAYOUT(" + std::to_string(pvLayoutOffset) + ") " + "OUT mediump " +
											GetType(material->GetCustomData1Count()) + " v_CustomData1;");
			shaderData.CodePS = Replace(shaderData.CodePS,
										"//$C_PIN1$",
										"LAYOUT(" + std::to_string(pvLayoutOffset) + ") " + "IN mediump " +
											GetType(material->GetCustomData1Count()) + " v_CustomData1;");

			layoutOffset += 1;
			pvLayoutOffset += 1;
		}

		if (material->GetCustomData2Count() > 0)
		{
			if (isSprite)
			{
				shaderData.CodeVS = Replace(shaderData.CodeVS,
											"//$C_IN2$",
											"LAYOUT(" + std::to_string(layoutOffset) + ") " + "IN " +
												GetType(material->GetCustomData2Count()) + " atCustomData2;");
			}
			shaderData.CodeVS = Replace(shaderData.CodeVS,
										"//$C_OUT2$",
										"LAYOUT(" + std::to_string(pvLayoutOffset) + ") " + "OUT mediump " +
											GetType(material->GetCustomData2Count()) + " v_CustomData2;");
			shaderData.CodePS = Replace(shaderData.CodePS,
										"//$C_PIN2$",
										"LAYOUT(" + std::to_string(pvLayoutOffset) + ") " + "IN mediump " +
											GetType(material->GetCustomData2Count()) + " v_CustomData2;");
		}

		return shaderData;
	}
};

} // namespace GLSL

} // namespace Effekseer


#pragma once

#include <vector>

namespace Effekseer
{

class MaterialCompilerGL : public MaterialCompiler, ReferenceObject
{
private:
public:
	MaterialCompilerGL() = default;

	virtual ~MaterialCompilerGL() = default;

	CompiledMaterialBinary* Compile(Material* material, int32_t maximumTextureCount);

	CompiledMaterialBinary* Compile(Material* material) override;

	int AddRef() override { return ReferenceObject::AddRef(); }

	int Release() override { return ReferenceObject::Release(); }

	int GetRef() override { return ReferenceObject::GetRef(); }
};

} // namespace Effekseer


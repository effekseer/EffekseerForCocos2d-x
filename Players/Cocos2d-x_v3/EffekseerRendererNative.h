#pragma once
#include "cocos2d.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#define __EFFEKSEER_RENDERER_GLES2__
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#define __EFFEKSEER_RENDERER_GLES2__
#else
#define __EFFEKSEER_RENDERER_GL2__
#endif

#ifndef	__EFFEKSEERRENDERER_COMMON_UTILS_H__
#define	__EFFEKSEERRENDERER_COMMON_UTILS_H__

#include "EffekseerNative.h"
#include <assert.h>
#include <string.h>
#include <math.h>
#include <array>

#if defined(_M_IX86) || defined(__x86__)
#define EFK_SSE2
#include <emmintrin.h>
#elif defined(__ARM_NEON__)
#define EFK_NEON
#include <arm_neon.h>
#endif

#ifdef _MSC_VER
#include <xmmintrin.h>
#endif

namespace EffekseerRenderer
{

struct DynamicVertex
{
	::Effekseer::Vector3D Pos;
	::Effekseer::Color Col;
	//! packed vector
	::Effekseer::Color Normal;
	//! packed vector
	::Effekseer::Color Tangent;

	union {
		//! UV1 (for template)
		float UV[2];
		float UV1[2];
	};

	float UV2[2];

	void SetColor(const ::Effekseer::Color& color) { Col = color; }
};

struct SimpleVertex
{
	::Effekseer::Vector3D Pos;
	uint8_t Col[4];

	union {
		float UV[2];
		//! dummy for template
		float UV2[2];
	};

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
	::Effekseer::Vector3D Pos;
	uint8_t Col[4];

	union {
		float UV[2];
		//! dummy for template
		float UV2[2];
	};

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
	::Effekseer::Vector3D Pos;
	uint8_t Col[4];

	union {
		float UV[2];
		//! dummy for template
		float UV2[2];
	};

	::Effekseer::Vector3D Binormal;
	::Effekseer::Vector3D Tangent;
	
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
	::Effekseer::Vector3D Pos;
	uint8_t Col[4];

	union {
		float UV[2];
		//! dummy for template
		float UV2[2];
	};

	::Effekseer::Vector3D Binormal;
	::Effekseer::Vector3D Tangent;
	
	void SetColor(const ::Effekseer::Color& color)
	{
		Col[0] = color.B;
		Col[1] = color.G;
		Col[2] = color.R;
		Col[3] = color.A;
	}
};

/**
	@brief	a view class to access an array with a stride
*/
template <typename T> struct StrideView
{
	int32_t stride_;
	uint8_t* pointer_;
	uint8_t* pointerOrigin_;
	
#if _DEBUG
	int32_t offset_;
	int32_t elementCount_;
#endif
	
	StrideView(void* pointer, int32_t stride, int32_t elementCount)
		: stride_(stride)
		, pointer_(reinterpret_cast<uint8_t*>(pointer))
		, pointerOrigin_(reinterpret_cast<uint8_t*>(pointer))
#if _DEBUG
		, offset_(0)
		, elementCount_(elementCount)
#endif
	{
	}

	T& operator[](int i) const { 
#if _DEBUG
		assert(i >= 0);
		assert(i + offset_ < elementCount_);
#endif
		return *reinterpret_cast<T*>((pointer_ + stride_ * i)); 
	}

	StrideView& operator+=(const int& rhs)
	{
#if _DEBUG
		offset_ += rhs;
#endif
		pointer_ += stride_ * rhs;
		return *this;
	}

	void Reset() { 
#if _DEBUG
		offset_ = 0;
#endif
		pointer_ = pointerOrigin_; 
	}
};

/**
	@brief	a view class to access an array with a stride
*/
template<> struct StrideView<SimpleVertex>
{
	static const int32_t stride_ = sizeof(SimpleVertex);
	uint8_t* pointer_;
	uint8_t* pointerOrigin_;

#if _DEBUG
	int32_t offset_;
	int32_t elementCount_;
#endif

	StrideView(void* pointer, int32_t stride, int32_t elementCount)
		: pointer_(reinterpret_cast<uint8_t*>(pointer))
		, pointerOrigin_(reinterpret_cast<uint8_t*>(pointer))
#if _DEBUG
		, offset_(0)
		, elementCount_(elementCount)
#endif
	{
		assert(stride_ == stride);
	}

	SimpleVertex& operator[](int i) const
	{
#if _DEBUG
		assert(i >= 0);
		assert(i + offset_ < elementCount_);
#endif
		return *reinterpret_cast<SimpleVertex*>((pointer_ + stride_ * i));
	}

	StrideView& operator+=(const int& rhs)
	{
#if _DEBUG
		offset_ += rhs;
#endif
		pointer_ += stride_ * rhs;
		return *this;
	}

	void Reset()
	{
#if _DEBUG
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

#if _DEBUG
	int32_t offset_;
	int32_t elementCount_;
#endif

	StrideView(void* pointer, int32_t stride, int32_t elementCount)
		: pointer_(reinterpret_cast<uint8_t*>(pointer))
		, pointerOrigin_(reinterpret_cast<uint8_t*>(pointer))
#if _DEBUG
		, offset_(0)
		, elementCount_(elementCount)
#endif
	{
		assert(stride_ == stride);
	}

	SimpleVertexDX9& operator[](int i) const
	{
#if _DEBUG
		assert(i >= 0);
		assert(i + offset_ < elementCount_);
#endif
		return *reinterpret_cast<SimpleVertexDX9*>((pointer_ + stride_ * i));
	}

	StrideView& operator+=(const int& rhs)
	{
#if _DEBUG
		offset_ += rhs;
#endif
		pointer_ += stride_ * rhs;
		return *this;
	}

	void Reset()
	{
#if _DEBUG
		offset_ = 0;
#endif
		pointer_ = pointerOrigin_;
	}
};

void CalcBillboard(::Effekseer::BillboardType billboardType,
				   Effekseer::Matrix43& dst,
				   ::Effekseer::Vector3D& s,
				   ::Effekseer::Vector3D& R,
				   ::Effekseer::Vector3D& F,
				   const ::Effekseer::Matrix43& src,
				   const ::Effekseer::Vector3D& frontDirection);

/**
	@brief Spline generator
	@note
	Reference https://qiita.com/edo_m18/items/f2f0c6bf9032b0ec12d4
*/
class SplineGenerator
{
	std::vector<Effekseer::Vector3D> a;
	std::vector<Effekseer::Vector3D> b;
	std::vector<Effekseer::Vector3D> c;
	std::vector<Effekseer::Vector3D> d;
	std::vector<Effekseer::Vector3D> w;
	std::vector<bool> isSame;

public:
	void AddVertex(const Effekseer::Vector3D& v);

	void Calculate();

	void Reset();

	Effekseer::Vector3D GetValue(float t) const;
};


void ApplyDepthParameters(::Effekseer::Matrix43& mat,
					  const ::Effekseer::Vector3D& cameraFront,
					  const ::Effekseer::Vector3D& cameraPos,
					::Effekseer::NodeRendererDepthParameter* depthParameter,
					  bool isRightHand);

void ApplyDepthParameters(::Effekseer::Matrix43& mat,
					  const ::Effekseer::Vector3D& cameraFront,
					  const ::Effekseer::Vector3D& cameraPos,
					  ::Effekseer::Vector3D& scaleValues,
						  ::Effekseer::NodeRendererDepthParameter* depthParameter,
					  bool isRightHand);

void ApplyDepthParameters(::Effekseer::Matrix43& mat,
						  ::Effekseer::Vector3D& translationValues,
						  ::Effekseer::Vector3D& scaleValues,
						  const ::Effekseer::Vector3D& cameraFront,
						  const ::Effekseer::Vector3D& cameraPos,
						  ::Effekseer::NodeRendererDepthParameter* depthParameter,
						  bool isRightHand);

void ApplyDepthParameters(::Effekseer::Matrix44& mat,
						  const ::Effekseer::Vector3D& cameraFront,
						  const ::Effekseer::Vector3D& cameraPos,
						  ::Effekseer::NodeRendererDepthParameter* depthParameter,
						  bool isRightHand);

template <typename Vertex>
inline void TransformStandardVertexes( Vertex& vertexes, int32_t count, const ::Effekseer::Matrix43& mat )
{
	alignas(16) float Value3[4] = {mat.Value[3][0], mat.Value[3][1], mat.Value[3][2], 0.0f};
#if defined(EFK_SSE2)
	__m128 r0 = _mm_loadu_ps( mat.Value[0] );
	__m128 r1 = _mm_loadu_ps( mat.Value[1] );
	__m128 r2 = _mm_loadu_ps( mat.Value[2] );
	__m128 r3 = _mm_load_ps( Value3 );

	float tmp_out[4];
	::Effekseer::Vector3D* inout_prev;

	// 1st loop
	{
		::Effekseer::Vector3D* inout_cur = &vertexes[0].Pos;
		__m128 v = _mm_loadu_ps( (const float*)inout_cur );

		__m128 x = _mm_shuffle_ps( v, v, _MM_SHUFFLE(0,0,0,0) );
		__m128 a0 = _mm_mul_ps( r0, x );
		__m128 y = _mm_shuffle_ps( v, v, _MM_SHUFFLE(1,1,1,1) );
		__m128 a1 = _mm_mul_ps( r1, y );
		__m128 z = _mm_shuffle_ps( v, v, _MM_SHUFFLE(2,2,2,2) );
		__m128 a2 = _mm_mul_ps( r2, z );

		__m128 a01 = _mm_add_ps( a0, a1 );
		__m128 a23 = _mm_add_ps( a2, r3 );
		__m128 a = _mm_add_ps( a01, a23 );

		// store the result of 1st loop
		_mm_storeu_ps( tmp_out, a );
		inout_prev = inout_cur;
	}

	for( int i = 1; i < count; i++ )
	{
		::Effekseer::Vector3D* inout_cur = &vertexes[i].Pos;
		__m128 v = _mm_loadu_ps( (const float*)inout_cur );

		__m128 x = _mm_shuffle_ps( v, v, _MM_SHUFFLE(0,0,0,0) );
		__m128 a0 = _mm_mul_ps( r0, x );
		__m128 y = _mm_shuffle_ps( v, v, _MM_SHUFFLE(1,1,1,1) );
		__m128 a1 = _mm_mul_ps( r1, y );
		__m128 z = _mm_shuffle_ps( v, v, _MM_SHUFFLE(2,2,2,2) );
		__m128 a2 = _mm_mul_ps( r2, z );

		__m128 a01 = _mm_add_ps( a0, a1 );
		__m128 a23 = _mm_add_ps( a2, r3 );
		__m128 a = _mm_add_ps( a01, a23 );

		// write the result of previous loop
		inout_prev->X = tmp_out[0];
		inout_prev->Y = tmp_out[1];
		inout_prev->Z = tmp_out[2];

		// store the result of current loop
		_mm_storeu_ps( tmp_out, a );
		inout_prev = inout_cur;
	}

	// write the result of last loop
	{
		inout_prev->X = tmp_out[0];
		inout_prev->Y = tmp_out[1];
		inout_prev->Z = tmp_out[2];
	}
#elif defined(EFK_NEON)
	float32x4_t r0 = vld1q_f32( mat.Value[0] );
	float32x4_t r1 = vld1q_f32( mat.Value[1] );
	float32x4_t r2 = vld1q_f32( mat.Value[2] );
	float32x4_t r3 = vld1q_f32( Value3 );

	float tmp_out[4];
	::Effekseer::Vector3D* inout_prev;

	// 1st loop
	{
		::Effekseer::Vector3D* inout_cur = &vertexes[0].Pos;
		float32x4_t v = vld1q_f32( (const float*)inout_cur );

		float32x4_t a = vmlaq_lane_f32( r3, r0, vget_low_f32(v), 0 );
		a = vmlaq_lane_f32( a, r1, vget_low_f32(v), 1 );
		a = vmlaq_lane_f32( a, r2, vget_high_f32(v), 0 );

		// store the result of 1st loop
		vst1q_f32( tmp_out, a );
		inout_prev = inout_cur;
	}

	for( int i = 1; i < count; i++ )
	{
		::Effekseer::Vector3D* inout_cur = &vertexes[i].Pos;
		float32x4_t v = vld1q_f32( (const float*)inout_cur );

		float32x4_t a = vmlaq_lane_f32( r3, r0, vget_low_f32(v), 0 );
		a = vmlaq_lane_f32( a, r1, vget_low_f32(v), 1 );
		a = vmlaq_lane_f32( a, r2, vget_high_f32(v), 0 );

		// write the result of previous loop
		inout_prev->X = tmp_out[0];
		inout_prev->Y = tmp_out[1];
		inout_prev->Z = tmp_out[2];

		// store the result of current loop
		vst1q_f32( tmp_out, a );
		inout_prev = inout_cur;
	}

	// write the result of last loop
	{
		inout_prev->X = tmp_out[0];
		inout_prev->Y = tmp_out[1];
		inout_prev->Z = tmp_out[2];
	}
#else
	for( int i = 0; i < count; i++ )
	{
		::Effekseer::Vector3D::Transform(
			vertexes[i].Pos,
			vertexes[i].Pos,
			mat );
	}
#endif
}

template <typename VertexDistortion>
inline void TransformDistortionVertexes(VertexDistortion& vertexes, int32_t count, const ::Effekseer::Matrix43& mat)
{
	TransformStandardVertexes( vertexes, count, mat );

	for (int i = 0; i < count; i++)
	{
		auto vs = &vertexes[i];

		::Effekseer::Vector3D::Transform(
			vs->Tangent,
			vs->Tangent,
			mat);

		::Effekseer::Vector3D::Transform(
			vs->Binormal,
			vs->Binormal,
			mat);

		Effekseer::Vector3D zero;
		::Effekseer::Vector3D::Transform(
			zero,
			zero,
			mat);

		::Effekseer::Vector3D::Normal(vs->Tangent, vs->Tangent - zero);
		::Effekseer::Vector3D::Normal(vs->Binormal, vs->Binormal - zero);
	}
}

inline void TransformVertexes(StrideView<VertexDistortion>& v, int32_t count, const ::Effekseer::Matrix43& mat)
{
	TransformDistortionVertexes(v, count, mat);
}

inline void TransformVertexes(StrideView<VertexDistortionDX9>& v, int32_t count, const ::Effekseer::Matrix43& mat)
{
	TransformDistortionVertexes(v, count, mat);
}

inline void TransformVertexes(StrideView<SimpleVertex>& v, int32_t count, const ::Effekseer::Matrix43& mat)
{
	TransformStandardVertexes(v, count, mat);
}

inline void TransformVertexes(StrideView<SimpleVertexDX9>& v, int32_t count, const ::Effekseer::Matrix43& mat)
{
	TransformStandardVertexes(v, count, mat);
}

inline void TransformVertexes(StrideView<DynamicVertex>& v, int32_t count, const ::Effekseer::Matrix43& mat)
{
	TransformStandardVertexes(v, count, mat);
}

inline void SwapRGBAToBGRA(Effekseer::Color& color)
{ auto temp = color;
	color.B = temp.R;
	color.R = temp.B;
}

inline Effekseer::Color PackVector3DF(const Effekseer::Vector3D& v)
{
	Effekseer::Color ret;
	ret.R = static_cast<uint8_t>((v.X + 1.0f) / 2.0f * 255.0f);
	ret.G = static_cast<uint8_t>((v.Y + 1.0f) / 2.0f * 255.0f);
	ret.B = static_cast<uint8_t>((v.Z + 1.0f) / 2.0f * 255.0f);
	ret.A = 255;
	return ret;
}

}
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
		@brief	ライトの方向を取得する。
	*/
	virtual const ::Effekseer::Vector3D& GetLightDirection() const = 0;

	/**
		@brief	ライトの方向を設定する。
	*/
	virtual void SetLightDirection( const ::Effekseer::Vector3D& direction ) = 0;

	/**
		@brief	ライトの色を取得する。
	*/
	virtual const ::Effekseer::Color& GetLightColor() const = 0;

	/**
		@brief	ライトの色を設定する。
	*/
	virtual void SetLightColor( const ::Effekseer::Color& color ) = 0;

	/**
		@brief	ライトの環境光の色を取得する。
	*/
	virtual const ::Effekseer::Color& GetLightAmbientColor() const = 0;

	/**
		@brief	ライトの環境光の色を設定する。
	*/
	virtual void SetLightAmbientColor( const ::Effekseer::Color& color ) = 0;

		/**
		@brief	最大描画スプライト数を取得する。
	*/
	virtual int32_t GetSquareMaxCount() const = 0;

	/**
		@brief	投影行列を取得する。
	*/
	virtual const ::Effekseer::Matrix44& GetProjectionMatrix() const = 0;

	/**
		@brief	投影行列を設定する。
	*/
	virtual void SetProjectionMatrix( const ::Effekseer::Matrix44& mat ) = 0;

	/**
		@brief	カメラ行列を取得する。
	*/
	virtual const ::Effekseer::Matrix44& GetCameraMatrix() const = 0;

	/**
		@brief	カメラ行列を設定する。
	*/
	virtual void SetCameraMatrix( const ::Effekseer::Matrix44& mat ) = 0;

	/**
		@brief	カメラプロジェクション行列を取得する。
	*/
	virtual ::Effekseer::Matrix44& GetCameraProjectionMatrix() = 0;

	/**
		@brief	Get a front direction of camera
	*/
	virtual ::Effekseer::Vector3D GetCameraFrontDirection() const = 0;

	/**
		@brief	Get a position of camera
	*/
	virtual ::Effekseer::Vector3D GetCameraPosition() const = 0;

	/**
		@brief	Set a front direction and position of camera manually
		@note
		These are set based on camera matrix automatically.
		It is failed on some platform.
	*/
	virtual void SetCameraParameter(const ::Effekseer::Vector3D& front, const ::Effekseer::Vector3D& position) = 0;

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

class Renderer::Impl
{
private:
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
	::Effekseer::TextureData* TexturePtr;
	::Effekseer::TextureData* NormalTexturePtr;

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
		TexturePtr = nullptr;
		NormalTexturePtr = nullptr;

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
		if (TexturePtr != state.TexturePtr)
			return true;
		if (NormalTexturePtr != state.TexturePtr)
			return true;
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

	void CopyMaterialFromParameterToState(Effekseer::Effect* effect, Effekseer::MaterialParameter* materialParam, int32_t colorTextureIndex, int32_t texture2Index)
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
	int32_t renderVertexMaxSize;

	bool isDistortionMode_;
	bool isDynamicVertexMode_ = false;

	struct VertexConstantBuffer
	{
		Effekseer::Matrix44 constantVSBuffer[2];
		float uvInversed[4];
	};

	struct DistortionPixelConstantBuffer
	{
		float scale[4];
		float uvInversed[4];
	};

	
	void ColorToFloat4(::Effekseer::Color color, float fc[4])
	{
		fc[0] = color.R / 255.0f;
		fc[1] = color.G / 255.0f;
		fc[2] = color.B / 255.0f;
		fc[3] = color.A / 255.0f;
	}

	void VectorToFloat4(::Effekseer::Vector3D v, float fc[4])
	{
		fc[0] = v.X;
		fc[1] = v.Y;
		fc[2] = v.Z;
		fc[3] = 1.0f;
	}

public:
	StandardRenderer(
		RENDERER* renderer, SHADER* shader, SHADER* shader_distortion)
		: renderVertexMaxSize(0), isDistortionMode_(false)
	{
		m_renderer = renderer;
		m_shader = shader;
		//m_shader_no_texture = shader_no_texture;
		m_shader_distortion = shader_distortion;
		//m_shader_no_texture_distortion = shader_no_texture_distortion;

		vertexCaches.reserve(m_renderer->GetVertexBuffer()->GetMaxSize());
		renderVertexMaxSize = m_renderer->GetVertexBuffer()->GetMaxSize();
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
	}

	const StandardRendererState& GetState() { return m_state; }

	void Rendering(const Effekseer::Matrix44& mCamera, const Effekseer::Matrix44& mProj)
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

	void Rendering_(const Effekseer::Matrix44& mCamera, const Effekseer::Matrix44& mProj, int32_t bufferOffset, int32_t bufferSize, int32_t stride, int32_t renderPass)
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
			std::array<Effekseer::TextureData*, 2> textures;
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

				m_renderer->SetTextures(shader_, textures.data(), 2);
			}
			else if (distortion)
			{
				textures[1] = m_renderer->GetBackground();
				m_renderer->SetTextures(shader_, textures.data(), 2);
			}
			else
			{
				m_renderer->SetTextures(shader_, textures.data(), 1);
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
			// time
			std::array<float, 4> predefined_uniforms;
			predefined_uniforms.fill(0.5f);
			predefined_uniforms[0] = m_renderer->GetTime();

			// vs
			int32_t vsOffset = 0;
			m_renderer->SetVertexBufferToShader(&mCamera, sizeof(Effekseer::Matrix44), vsOffset);
			vsOffset += sizeof(Effekseer::Matrix44);

			m_renderer->SetVertexBufferToShader(&mProj, sizeof(Effekseer::Matrix44), vsOffset);
			vsOffset += sizeof(Effekseer::Matrix44);

			m_renderer->SetVertexBufferToShader(uvInversedMaterial.data(), sizeof(float) * 4, vsOffset);
			vsOffset += (sizeof(float) * 4);

			m_renderer->SetVertexBufferToShader(predefined_uniforms.data(), sizeof(float) * 4, vsOffset);
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

			// shader model
			if (m_state.MaterialPtr->ShadingModel == ::Effekseer::ShadingModelType::Lit)
			{
				float cameraPosition[4];
				float lightDirection[4];
				float lightColor[4];
				float lightAmbientColor[4];

				::Effekseer::Vector3D cameraPosition3 = m_renderer->GetCameraPosition();
				::Effekseer::Vector3D lightDirection3 = m_renderer->GetLightDirection();
				::Effekseer::Vector3D::Normal(lightDirection3, lightDirection3);
				VectorToFloat4(cameraPosition3, cameraPosition);
				VectorToFloat4(lightDirection3, lightDirection);
				ColorToFloat4(m_renderer->GetLightColor(), lightColor);
				ColorToFloat4(m_renderer->GetLightAmbientColor(), lightAmbientColor);

				m_renderer->SetPixelBufferToShader(cameraPosition, sizeof(float) * 4, psOffset);
				psOffset += (sizeof(float) * 4);

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
			vcb.constantVSBuffer[0] = mCamera;
			vcb.constantVSBuffer[1] = mProj;
			vcb.uvInversed[0] = uvInversed[0];
			vcb.uvInversed[1] = uvInversed[1];

			m_renderer->SetVertexBufferToShader(&vcb, sizeof(VertexConstantBuffer), 0);

			// ps
			int32_t psOffset = 0;
			float lightDirection[4];
			float lightColor[4];
			float lightAmbientColor[4];

			::Effekseer::Vector3D lightDirection3 = m_renderer->GetLightDirection();
			::Effekseer::Vector3D::Normal(lightDirection3, lightDirection3);
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
		else
		{
			VertexConstantBuffer vcb;
			vcb.constantVSBuffer[0] = mCamera;
			vcb.constantVSBuffer[1] = mProj;
			vcb.uvInversed[0] = uvInversed[0];
			vcb.uvInversed[1] = uvInversed[1];

			m_renderer->SetVertexBufferToShader(&vcb, sizeof(VertexConstantBuffer), 0);

			if (distortion)
			{
				DistortionPixelConstantBuffer pcb;
				pcb.scale[0] = m_state.DistortionIntensity;
				pcb.uvInversed[0] = uvInversedBack[0];
				pcb.uvInversed[1] = uvInversedBack[1];

				m_renderer->SetPixelBufferToShader(&pcb, sizeof(DistortionPixelConstantBuffer), 0);
			}
		}

		shader_->SetConstantBuffer();

		if (m_state.MaterialPtr == nullptr)
		{
			state.TextureFilterTypes[0] = m_state.TextureFilter1;
			state.TextureWrapTypes[0] = m_state.TextureWrap1;

			if (distortion)
			{
				state.TextureFilterTypes[1] = Effekseer::TextureFilterType::Linear;
				state.TextureWrapTypes[1] = Effekseer::TextureWrapType::Clamp;
			}
			else
			{
				state.TextureFilterTypes[1] = m_state.TextureFilter2;
				state.TextureWrapTypes[1] = m_state.TextureWrap2;
			}
		}

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
typedef ::Effekseer::Vector3D efkVector3D;

template<int MODEL_COUNT>
struct ModelRendererVertexConstantBuffer
{
	Effekseer::Matrix44		CameraMatrix;
	Effekseer::Matrix44		ModelMatrix[MODEL_COUNT];
	float	ModelUV[MODEL_COUNT][4];
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
};

class ModelRendererBase
	: public ::Effekseer::ModelRenderer
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
	std::vector<Effekseer::Color> colorsSorted_;
	std::vector<int32_t> timesSorted_;
	std::vector<std::array<float, 4>> customData1Sorted_;
	std::vector<std::array<float, 4>> customData2Sorted_;

	std::vector<Effekseer::Matrix44> m_matrixes;
	std::vector<Effekseer::RectF> m_uv;
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
	
	void VectorToFloat4(::Effekseer::Vector3D v, float fc[4])
	{
		fc[0] = v.X;
		fc[1] = v.Y;
		fc[2] = v.Z;
		fc[3] = 1.0f;
	}

	ModelRendererBase()
	{
	}

public:

	virtual ~ModelRendererBase() {}

	template<typename RENDERER>
	void BeginRendering_(RENDERER* renderer, const efkModelNodeParam& parameter, int32_t count, void* userData)
	{
		keyValues_.clear();

		m_matrixes.clear();
		m_uv.clear();
		m_colors.clear();
		m_times.clear();
		customData1_.clear();
		customData2_.clear();

		matrixesSorted_.clear();
		uvSorted_.clear();
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
		Effekseer::Matrix44 mat44;

		if (btype == ::Effekseer::BillboardType::Fixed)
		{
			for (int32_t r_ = 0; r_ < 4; r_++)
			{
				for (int32_t c_ = 0; c_ < 3; c_++)
				{
					mat44.Values[r_][c_] = instanceParameter.SRTMatrix43.Value[r_][c_];
				}
			}
		}
		else
		{
			Effekseer::Matrix43 mat43;
			Effekseer::Vector3D s;
			Effekseer::Vector3D R;
			Effekseer::Vector3D F;

			CalcBillboard(btype, mat43, s ,R, F, instanceParameter.SRTMatrix43, renderer->GetCameraFrontDirection());

			::Effekseer::Matrix43 mat_scale;
			mat_scale.Scaling(s.X, s.Y, s.Z);
			::Effekseer::Matrix43::Multiple(mat43, mat_scale, mat43);

			for (int32_t r_ = 0; r_ < 4; r_++)
			{
				for (int32_t c_ = 0; c_ < 3; c_++)
				{
					mat44.Values[r_][c_] = mat43.Value[r_][c_];
				}
			}
		}
		
		if (parameter.Magnification != 1.0f)
		{
			Effekseer::Matrix44 mat_scale;
			mat_scale.Values[0][0] = parameter.Magnification;
			mat_scale.Values[1][1] = parameter.Magnification;
			mat_scale.Values[2][2] = parameter.Magnification;

			Effekseer::Matrix44::Mul(mat44, mat_scale, mat44);
		}

		if (!parameter.IsRightHand)
		{
			Effekseer::Matrix44 mat_scale;
			mat_scale.Values[0][0] = 1.0f;
			mat_scale.Values[1][1] = 1.0f;
			mat_scale.Values[2][2] = -1.0f;

			Effekseer::Matrix44::Mul(mat44, mat_scale, mat44);
		}

		m_matrixes.push_back(mat44);
		m_uv.push_back(instanceParameter.UV);
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
		if (param.DepthParameterPtr->ZSort != Effekseer::ZSortType::None)
		{
			keyValues_.resize(m_matrixes.size());
			for (size_t i = 0; i < keyValues_.size(); i++)
			{
				efkVector3D t;
				t.X = m_matrixes[i].Values[3][0];
				t.Y = m_matrixes[i].Values[3][1];
				t.Z = m_matrixes[i].Values[3][2];

				auto frontDirection = renderer->GetCameraFrontDirection();
				if (!param.IsRightHand)
				{
					frontDirection.Z = -frontDirection.Z;
				}

				keyValues_[i].Key = Effekseer::Vector3D::Dot(t, frontDirection);
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
			m_colors = colorsSorted_;
			m_times = timesSorted_;
			customData1_ = customData1Sorted_;
			customData2_ = customData2Sorted_;
		}

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
			Effekseer::TextureData* textures[2];
			textures[0] = nullptr;
			textures[1] = nullptr;

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

			renderer->SetTextures(shader_, textures, 2);
		}
		
		renderer->GetRenderState()->Update(distortion);

		std::array<float, 4> uvInversed;
		std::array<float, 4> uvInversedBack;
		std::array<float, 4> uvInversedMaterial;
		
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

		uvInversedMaterial[0] = uvInversed[0];
		uvInversedMaterial[1] = uvInversed[1];
		uvInversedMaterial[2] = uvInversedBack[0];
		uvInversedMaterial[3] = uvInversedBack[1];

		ModelRendererVertexConstantBuffer<InstanceCount>* vcb =
			(ModelRendererVertexConstantBuffer<InstanceCount>*)shader_->GetVertexConstantBuffer();

		float* cutomData1Ptr = nullptr;
		float* cutomData2Ptr = nullptr;

		if (materialParam != nullptr && material != nullptr)
		{
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

			// shader model
			material = param.EffectPointer->GetMaterial(materialParam->MaterialIndex);

			if (material->ShadingModel == ::Effekseer::ShadingModelType::Lit)
			{
				float cameraPosition[4];
				float lightDirection[4];
				float lightColor[4];
				float lightAmbientColor[4];

				::Effekseer::Vector3D cameraPosition3 = renderer->GetCameraPosition();
				::Effekseer::Vector3D lightDirection3 = renderer->GetLightDirection();
				::Effekseer::Vector3D::Normal(lightDirection3, lightDirection3);
				VectorToFloat4(cameraPosition3, cameraPosition);
				VectorToFloat4(lightDirection3, lightDirection);
				ColorToFloat4(renderer->GetLightColor(), lightColor);
				ColorToFloat4(renderer->GetLightAmbientColor(), lightAmbientColor);

				renderer->SetPixelBufferToShader(cameraPosition, sizeof(float) * 4, psOffset);
				psOffset += (sizeof(float) * 4);

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
		else
		{
			vcb->UVInversed[0] = uvInversed[0];
			vcb->UVInversed[1] = uvInversed[1];

			if (distortion)
			{
				float* pcb = (float*) shader_->GetPixelConstantBuffer();
				pcb[4 * 0 + 0] = param.BasicParameterPtr->DistortionIntensity;

				pcb[4 * 1 + 0] = uvInversedBack[0];
				pcb[4 * 1 + 1] = uvInversedBack[1];
			}
			else
			{
				ModelRendererPixelConstantBuffer* pcb = (ModelRendererPixelConstantBuffer*) shader_->GetPixelConstantBuffer();

				// specify predefined parameters
				if (param.BasicParameterPtr->MaterialType == Effekseer::RendererMaterialType::Lighting)
				{
					::Effekseer::Vector3D lightDirection = renderer->GetLightDirection();
					::Effekseer::Vector3D::Normal(lightDirection, lightDirection);
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
			}
		}

		vcb->CameraMatrix = renderer->GetCameraProjectionMatrix();

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
					ApplyDepthParameters(vcb->ModelMatrix[num],
										renderer->GetCameraFrontDirection(),
										renderer->GetCameraPosition(),
										param.DepthParameterPtr,
										param.IsRightHand);
	
					vcb->ModelUV[num][0] = m_uv[loop+num].X;
					vcb->ModelUV[num][1] = m_uv[loop+num].Y;
					vcb->ModelUV[num][2] = m_uv[loop+num].Width;
					vcb->ModelUV[num][3] = m_uv[loop+num].Height;

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

				// DepthParameters
				ApplyDepthParameters(vcb->ModelMatrix[0], renderer->GetCameraFrontDirection(), renderer->GetCameraPosition(),
									 param.DepthParameterPtr,
									 param.IsRightHand);
				
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
	typedef ::Effekseer::Vector3D efkVector3D;

	template<typename RENDERER, typename VERTEX_NORMAL, typename VERTEX_DISTORTION>
	class RibbonRendererBase
		: public ::Effekseer::RibbonRenderer
	{
	private:

	protected:
		RENDERER*						m_renderer;
		int32_t							m_ribbonCount;

		int32_t							m_ringBufferOffset;
		uint8_t*						m_ringBufferData;

		efkRibbonNodeParam					innstancesNodeParam;
		std::vector<efkRibbonInstanceParam>	instances;
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
		void RenderSplines(const ::Effekseer::Matrix44& camera)
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
					auto pl = efkVector3D();
					auto pr = efkVector3D();

					auto& param = instances[loop];

					pl.X = param.Positions[0];
					pl.Y = 0.0f;
					pl.Z = 0.0f;

					pr.X = param.Positions[1];
					pr.Y = 0.0f;
					pr.Z = 0.0f;

					if (parameter.ViewpointDependent)
					{
						::Effekseer::Matrix43 mat = param.SRTMatrix43;
						::Effekseer::Vector3D s;
						::Effekseer::Matrix43 r;
						::Effekseer::Vector3D t;
						mat.GetSRT(s, r, t);

						ApplyDepthParameters(r,
											 t,
											 s,
											 m_renderer->GetCameraFrontDirection(),
											 m_renderer->GetCameraPosition(),
											 parameter.DepthParameterPtr,
											 parameter.IsRightHand);

						// extend
						pl.X = pl.X * s.X;
						pr.X = pr.X * s.X;

						::Effekseer::Vector3D F;
						::Effekseer::Vector3D R;
						::Effekseer::Vector3D U;

						U = ::Effekseer::Vector3D(r.Value[1][0], r.Value[1][1], r.Value[1][2]);

						::Effekseer::Vector3D::Normal(F, -m_renderer->GetCameraFrontDirection());

						::Effekseer::Vector3D::Normal(R, ::Effekseer::Vector3D::Cross(R, U, F));
						::Effekseer::Vector3D::Normal(F, ::Effekseer::Vector3D::Cross(F, R, U));

						::Effekseer::Matrix43 mat_rot;

						mat_rot.Value[0][0] = -R.X;
						mat_rot.Value[0][1] = -R.Y;
						mat_rot.Value[0][2] = -R.Z;
						mat_rot.Value[1][0] = U.X;
						mat_rot.Value[1][1] = U.Y;
						mat_rot.Value[1][2] = U.Z;
						mat_rot.Value[2][0] = F.X;
						mat_rot.Value[2][1] = F.Y;
						mat_rot.Value[2][2] = F.Z;
						mat_rot.Value[3][0] = t.X;
						mat_rot.Value[3][1] = t.Y;
						mat_rot.Value[3][2] = t.Z;


						::Effekseer::Vector3D::Transform(
							pl,
							pl,
							mat_rot);

						::Effekseer::Vector3D::Transform(
							pr,
							pr,
							mat_rot);

						spline_left.AddVertex(pl);
						spline_right.AddVertex(pr);
					}
					else
					{
						::Effekseer::Matrix43 mat = param.SRTMatrix43;

						::Effekseer::Vector3D s;

						ApplyDepthParameters(mat,
											 m_renderer->GetCameraFrontDirection(),
											 m_renderer->GetCameraPosition(),
											 //s,
											 parameter.DepthParameterPtr,
											 parameter.IsRightHand);

						::Effekseer::Vector3D::Transform(
							pl,
							pl,
							mat);

						::Effekseer::Vector3D::Transform(
							pr,
							pr, 
							mat);

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
						verteies[0].Pos = spline_left.GetValue(param.InstanceIndex + sploop / (float)parameter.SplineDivision);
						verteies[1].Pos = spline_right.GetValue(param.InstanceIndex + sploop / (float)parameter.SplineDivision);

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
						}
					}


					float percent = (float)(param.InstanceIndex  * parameter.SplineDivision + sploop) / (float)((param.InstanceCount - 1) * parameter.SplineDivision);

					if (parameter.ViewpointDependent)
					{
						::Effekseer::Matrix43 mat = param.SRTMatrix43;
						::Effekseer::Vector3D s;
						::Effekseer::Matrix43 r;
						::Effekseer::Vector3D t;
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
								verteies[i].Pos.X = verteies[i].Pos.X * s.X;
							}

							::Effekseer::Vector3D F;
							::Effekseer::Vector3D R;
							::Effekseer::Vector3D U;

							U = ::Effekseer::Vector3D(r.Value[1][0], r.Value[1][1], r.Value[1][2]);

							::Effekseer::Vector3D::Normal(F, -m_renderer->GetCameraFrontDirection());

							::Effekseer::Vector3D::Normal(R, ::Effekseer::Vector3D::Cross(R, U, F));
							::Effekseer::Vector3D::Normal(F, ::Effekseer::Vector3D::Cross(F, R, U));

							::Effekseer::Matrix43 mat_rot;

							mat_rot.Value[0][0] = -R.X;
							mat_rot.Value[0][1] = -R.Y;
							mat_rot.Value[0][2] = -R.Z;
							mat_rot.Value[1][0] = U.X;
							mat_rot.Value[1][1] = U.Y;
							mat_rot.Value[1][2] = U.Z;
							mat_rot.Value[2][0] = F.X;
							mat_rot.Value[2][1] = F.Y;
							mat_rot.Value[2][2] = F.Z;
							mat_rot.Value[3][0] = t.X;
							mat_rot.Value[3][1] = t.Y;
							mat_rot.Value[3][2] = t.Z;

							for (int i = 0; i < 2; i++)
							{
								::Effekseer::Vector3D::Transform(
									verteies[i].Pos,
									verteies[i].Pos,
									mat_rot);
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
							::Effekseer::Matrix43 mat = param.SRTMatrix43;

							::Effekseer::Vector3D s;
	
							ApplyDepthParameters(mat,
												 m_renderer->GetCameraFrontDirection(),
												 m_renderer->GetCameraPosition(),
												 //s,
												 parameter.DepthParameterPtr,
												 parameter.IsRightHand);


							for (int i = 0; i < 2; i++)
							{
								::Effekseer::Vector3D::Transform(
									verteies[i].Pos,
									verteies[i].Pos,
									mat);
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

			// Apply distortion
			if (vertexType == VertexType::Distortion)
			{
				StrideView<VERTEX_DISTORTION> vs_(m_ringBufferData, stride_, vertexCount_);
				Effekseer::Vector3D axisBefore;

				for (size_t i = 0; i < (instances.size() - 1) * parameter.SplineDivision + 1; i++)
				{
					bool isFirst_ = (i == 0);
					bool isLast_ = (i == ((instances.size() - 1) * parameter.SplineDivision));

					Effekseer::Vector3D axis;
					Effekseer::Vector3D pos;

					if (isFirst_)
					{
						axis = (vs_[3].Pos - vs_[1].Pos);
						Effekseer::Vector3D::Normal(axis, axis);
						axisBefore = axis;
					}
					else if (isLast_)
					{
						axis = axisBefore;
					}
					else
					{
						Effekseer::Vector3D axisOld = axisBefore;
						axis = (vs_[5].Pos - vs_[3].Pos);
						Effekseer::Vector3D::Normal(axis, axis);
						axisBefore = axis;

						axis = (axisBefore + axisOld) / 2.0f;
						Effekseer::Vector3D::Normal(axis, axis);
					}

					auto tangent = vs_[1].Pos - vs_[0].Pos;
					Effekseer::Vector3D::Normal(tangent, tangent);

					if (isFirst_)
					{
						vs_[0].Binormal = axis;
						vs_[1].Binormal = axis;

						vs_[0].Tangent = tangent;
						vs_[1].Tangent = tangent;

						vs_ += 2;

					}
					else if (isLast_)
					{
						vs_[0].Binormal = axis;
						vs_[1].Binormal = axis;

						vs_[0].Tangent = tangent;
						vs_[1].Tangent = tangent;

						vs_ += 2;
					}
					else
					{
						vs_[0].Binormal = axis;
						vs_[1].Binormal = axis;
						vs_[2].Binormal = axis;
						vs_[3].Binormal = axis;

						vs_[0].Tangent = tangent;
						vs_[1].Tangent = tangent;
						vs_[2].Tangent = tangent;
						vs_[3].Tangent = tangent;

						vs_ += 4;
					}
				}
			}
			else if (vertexType == VertexType::Dynamic)
			{
				StrideView<DynamicVertex> vs_(m_ringBufferData, stride_, vertexCount_);
				Effekseer::Vector3D axisBefore;

				for (size_t i = 0; i < (instances.size() - 1) * parameter.SplineDivision + 1; i++)
				{
					bool isFirst_ = (i == 0);
					bool isLast_ = (i == ((instances.size() - 1) * parameter.SplineDivision));

					Effekseer::Vector3D axis;
					Effekseer::Vector3D pos;

					if (isFirst_)
					{
						axis = (vs_[3].Pos - vs_[1].Pos);
						Effekseer::Vector3D::Normal(axis, axis);
						axisBefore = axis;
					}
					else if (isLast_)
					{
						axis = axisBefore;
					}
					else
					{
						Effekseer::Vector3D axisOld = axisBefore;
						axis = (vs_[5].Pos - vs_[3].Pos);
						Effekseer::Vector3D::Normal(axis, axis);
						axisBefore = axis;

						axis = (axisBefore + axisOld) / 2.0f;
						Effekseer::Vector3D::Normal(axis, axis);
					}

					auto tangent = vs_[1].Pos - vs_[0].Pos;
					Effekseer::Vector3D::Normal(tangent, tangent);

					Effekseer::Vector3D normal;
					Effekseer::Vector3D::Cross(normal, axis, tangent);
					Effekseer::Vector3D::Normal(normal, normal);

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

		void Rendering_(const efkRibbonNodeParam& parameter, const efkRibbonInstanceParam& instanceParameter, void* userData, const ::Effekseer::Matrix44& camera)
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
		void Rendering_Internal(const efkRibbonNodeParam& parameter, const efkRibbonInstanceParam& instanceParameter, void* userData, const ::Effekseer::Matrix44& camera)
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


			state.Distortion = param.BasicParameterPtr->MaterialType == Effekseer::RendererMaterialType::BackDistortion;
			state.DistortionIntensity = param.BasicParameterPtr->DistortionIntensity;
			state.MaterialType = param.BasicParameterPtr->MaterialType;

			state.CopyMaterialFromParameterToState(param.EffectPointer,
												   param.BasicParameterPtr->MaterialParameterPtr,
												   param.BasicParameterPtr->Texture1Index,
												   param.BasicParameterPtr->Texture2Index);
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
typedef ::Effekseer::Vector3D efkVector3D;

template<typename RENDERER, typename VERTEX_NORMAL, typename VERTEX_DISTORTION>
class RingRendererBase
	: public ::Effekseer::RingRenderer
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
	::Effekseer::Matrix44			m_singleRenderingMatrix;
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
						   const ::Effekseer::Matrix44& camera)
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

		state.Distortion = param.BasicParameterPtr->MaterialType == Effekseer::RendererMaterialType::BackDistortion;
		state.DistortionIntensity = param.BasicParameterPtr->DistortionIntensity;
		state.MaterialType = param.BasicParameterPtr->MaterialType;

		state.CopyMaterialFromParameterToState(param.EffectPointer,
											   param.BasicParameterPtr->MaterialParameterPtr,
											   param.BasicParameterPtr->Texture1Index,
											   param.BasicParameterPtr->Texture2Index);

		customData1Count_ = state.CustomData1Count;
		customData2Count_ = state.CustomData2Count;

		materialType_ = param.BasicParameterPtr->MaterialType;

		renderer->GetStandardRenderer()->UpdateStateAndRenderingIfRequired(state);
		renderer->GetStandardRenderer()->BeginRenderingAndRenderingIfRequired(count * singleVertexCount, stride_, (void*&)m_ringBufferData);

		vertexCount_ = count * singleVertexCount;
	}

	void Rendering_(const efkRingNodeParam& parameter, const efkRingInstanceParam& instanceParameter, void* userData, const ::Effekseer::Matrix44& camera)
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
	void Rendering_Internal( const efkRingNodeParam& parameter, const efkRingInstanceParam& instanceParameter, void* userData, const ::Effekseer::Matrix44& camera )
	{
		::Effekseer::Matrix43 mat43;

		if (parameter.Billboard == ::Effekseer::BillboardType::Billboard ||
			parameter.Billboard == ::Effekseer::BillboardType::RotatedBillboard ||
			parameter.Billboard == ::Effekseer::BillboardType::YAxisFixed)
		{
			Effekseer::Vector3D s;
			Effekseer::Vector3D R;
			Effekseer::Vector3D F;

			CalcBillboard(parameter.Billboard, mat43, s, R, F, instanceParameter.SRTMatrix43, m_renderer->GetCameraFrontDirection());

			ApplyDepthParameters(mat43,
								 m_renderer->GetCameraFrontDirection(),
								 m_renderer->GetCameraPosition(),
								 s,
								 parameter.DepthParameterPtr,
								 parameter.IsRightHand);

			if (CanSingleRendering())
			{
				::Effekseer::Matrix43 mat_scale;
				mat_scale.Scaling(s.X, s.Y, s.Z);
				::Effekseer::Matrix43::Multiple(mat43, mat_scale, mat43);
			}
			else
			{
				::Effekseer::Matrix43 mat_scale;
				mat_scale.Scaling(s.X, s.Y, s.Z);
				::Effekseer::Matrix43::Multiple(mat43, mat_scale, mat43);
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
		
		const float outerRadius = instanceParameter.OuterLocation.X;
		const float innerRadius = instanceParameter.InnerLocation.X;
		const float centerRadius = innerRadius + (outerRadius - innerRadius) * instanceParameter.CenterRatio;

		const float outerHeight = instanceParameter.OuterLocation.Y;
		const float innerHeight = instanceParameter.InnerLocation.Y;
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
		::Effekseer::Vector3D outerCurrent( cos_ * outerRadius, sin_ * outerRadius, outerHeight );
		::Effekseer::Vector3D innerCurrent( cos_ * innerRadius, sin_ * innerRadius, innerHeight );
		::Effekseer::Vector3D centerCurrent( cos_ * centerRadius, sin_ * centerRadius, centerHeight );

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

		::Effekseer::Vector3D outerNext, innerNext, centerNext;

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

			outerNext.X = cos_ * outerRadius;
			outerNext.Y = sin_ * outerRadius;
			outerNext.Z = outerHeight;
			innerNext.X = cos_ * innerRadius;
			innerNext.Y = sin_ * innerRadius;
			innerNext.Z = innerHeight;
			centerNext.X = cos_ * centerRadius;
			centerNext.Y = sin_ * centerRadius;
			centerNext.Z = centerHeight;

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
			v[0].Pos = outerCurrent;
			v[0].SetColor( outerColor );
			v[0].UV[0] = uv0Current;
			v[0].UV[1] = uv0v1;

			v[1].Pos = centerCurrent;
			v[1].SetColor( centerColor );
			v[1].UV[0] = uv0Current;
			v[1].UV[1] = uv0v2;

			v[2].Pos = outerNext;
			v[2].SetColor( outerColorNext );
			v[2].UV[0] = uv0texNext;
			v[2].UV[1] = uv0v1;
			
			v[3].Pos = centerNext;
			v[3].SetColor( centerColorNext );
			v[3].UV[0] = uv0texNext;
			v[3].UV[1] = uv0v2;

			v[4] = v[1];

			v[5].Pos = innerCurrent;
			v[5].SetColor( innerColor );
			v[5].UV[0] = uv0Current;
			v[5].UV[1] = uv0v3;

			v[6] = v[3];

			v[7].Pos = innerNext;
			v[7].SetColor( innerColorNext );
			v[7].UV[0] = uv0texNext;
			v[7].UV[1] = uv0v3;

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

				::Effekseer::Vector3D outerBefore;
				outerBefore.X = c_b * outerRadius;
				outerBefore.Y = s_b * outerRadius;
				outerBefore.Z = outerHeight;

				// next
				auto t_n = cos_ * stepC - sin_ * stepS;
				auto s_n = sin_ * stepC + cos_ * stepS;
				auto c_n = t_n;

				::Effekseer::Vector3D outerNN;
				outerNN.X = c_n * outerRadius;
				outerNN.Y = s_n * outerRadius;
				outerNN.Z = outerHeight;

				::Effekseer::Vector3D tangent0, tangent1, tangent2;
				::Effekseer::Vector3D::Normal(tangent0, outerCurrent - outerBefore);
				::Effekseer::Vector3D::Normal(tangent1, outerNext - outerCurrent);
				::Effekseer::Vector3D::Normal(tangent2, outerNN - outerNext);

				auto tangentCurrent = (tangent0 + tangent1) / 2.0f;
				auto tangentNext = (tangent1 + tangent2) / 2.0f;

				vs[0].Tangent = tangentCurrent;
				vs[0].Binormal = binormalCurrent;
				vs[1].Tangent = tangentCurrent;
				vs[1].Binormal = binormalCurrent;
				vs[2].Tangent = tangentNext;
				vs[2].Binormal = binormalNext;
				vs[3].Tangent = tangentNext;
				vs[3].Binormal = binormalNext;

				vs[4].Tangent = tangentCurrent;
				vs[4].Binormal = binormalCurrent;
				vs[5].Tangent = tangentCurrent;
				vs[5].Binormal = binormalCurrent;
				vs[6].Tangent = tangentNext;
				vs[6].Binormal = binormalNext;
				vs[7].Tangent = tangentNext;
				vs[7].Binormal = binormalNext;
			}
			else if (vertexType == VertexType::Dynamic)
			{
				StrideView<DynamicVertex> vs(&verteies[i], stride_, 8);

				// return back
				float t_b;
				t_b = old_c * (stepC)-old_s * (-stepS);
				auto s_b = old_s * (stepC) + old_c * (-stepS);
				auto c_b = t_b;

				::Effekseer::Vector3D outerBefore;
				outerBefore.X = c_b * outerRadius;
				outerBefore.Y = s_b * outerRadius;
				outerBefore.Z = outerHeight;

				// next
				auto t_n = cos_ * stepC - sin_ * stepS;
				auto s_n = sin_ * stepC + cos_ * stepS;
				auto c_n = t_n;

				::Effekseer::Vector3D outerNN;
				outerNN.X = c_n * outerRadius;
				outerNN.Y = s_n * outerRadius;
				outerNN.Z = outerHeight;

				::Effekseer::Vector3D tangent0, tangent1, tangent2;
				::Effekseer::Vector3D::Normal(tangent0, outerCurrent - outerBefore);
				::Effekseer::Vector3D::Normal(tangent1, outerNext - outerCurrent);
				::Effekseer::Vector3D::Normal(tangent2, outerNN - outerNext);

				auto tangentCurrent = (tangent0 + tangent1) / 2.0f;
				auto tangentNext = (tangent1 + tangent2) / 2.0f;

				auto binormalCurrent = v[5].Pos - v[0].Pos;
				auto binormalNext = v[7].Pos - v[2].Pos;

				::Effekseer::Vector3D normalCurrent;
				::Effekseer::Vector3D normalNext;

				::Effekseer::Vector3D::Cross(normalCurrent, tangentCurrent, binormalCurrent);
				::Effekseer::Vector3D::Cross(normalNext, tangentNext, binormalNext);
				::Effekseer::Vector3D::Normal(normalCurrent, normalCurrent);
				::Effekseer::Vector3D::Normal(normalNext, normalNext);

				// rotate directions
				::Effekseer::Matrix43 matRot = mat43;
				matRot.Value[3][0] = 0.0f;
				matRot.Value[3][1] = 0.0f;
				matRot.Value[3][2] = 0.0f;

				::Effekseer::Vector3D::Transform(normalCurrent, normalCurrent, matRot);
				::Effekseer::Vector3D::Transform(normalNext, normalNext, matRot);
				::Effekseer::Vector3D::Transform(tangentCurrent, tangentCurrent, matRot);
				::Effekseer::Vector3D::Transform(tangentNext, tangentNext, matRot);

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
			outerColor = outerColorNext;
			innerColor = innerColorNext;
			centerColor = centerColorNext;
		}

		if (CanSingleRendering())
		{
			for (int32_t i = 0; i < 4; i++)
			{
				m_singleRenderingMatrix.Values[i][0] = mat43.Value[i][0];
				m_singleRenderingMatrix.Values[i][1] = mat43.Value[i][1];
				m_singleRenderingMatrix.Values[i][2] = mat43.Value[i][2];
			}
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

	void EndRendering_(RENDERER* renderer, const efkRingNodeParam& param, void* userData, const ::Effekseer::Matrix44& camera)
	{
		if (CanSingleRendering())
		{
			::Effekseer::Matrix44 mat;
			::Effekseer::Matrix44::Mul(mat, m_singleRenderingMatrix, renderer->GetCameraMatrix());

			renderer->GetStandardRenderer()->Rendering(mat, renderer->GetProjectionMatrix());
		}

		if (param.DepthParameterPtr->ZSort != Effekseer::ZSortType::None && !CanSingleRendering())
		{
			for (auto& kv : instances_)
			{
				efkVector3D t;
				t.X = kv.Value.SRTMatrix43.Value[3][0];
				t.Y = kv.Value.SRTMatrix43.Value[3][1];
				t.Z = kv.Value.SRTMatrix43.Value[3][2];

				auto frontDirection = m_renderer->GetCameraFrontDirection();
				if (!param.IsRightHand)
				{
					frontDirection.Z = -frontDirection.Z;
				}

				kv.Key = Effekseer::Vector3D::Dot(t, frontDirection);
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
typedef ::Effekseer::Vector3D efkVector3D;

template<typename RENDERER, typename VERTEX_NORMAL, typename VERTEX_DISTORTION>
class SpriteRendererBase
	: public ::Effekseer::SpriteRenderer
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

	std::vector<KeyValue>				instances;
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
						   const ::Effekseer::Matrix44& camera)
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


		state.Distortion = param.BasicParameterPtr->MaterialType == Effekseer::RendererMaterialType::BackDistortion;
		state.DistortionIntensity = param.BasicParameterPtr->DistortionIntensity;
		state.MaterialType = param.BasicParameterPtr->MaterialType;

		state.CopyMaterialFromParameterToState(param.EffectPointer, param.BasicParameterPtr->MaterialParameterPtr, param.BasicParameterPtr->Texture1Index, param.BasicParameterPtr->Texture2Index);
		customData1Count_ = state.CustomData1Count;
		customData2Count_ = state.CustomData2Count;

		renderer->GetStandardRenderer()->UpdateStateAndRenderingIfRequired(state);

		renderer->GetStandardRenderer()->BeginRenderingAndRenderingIfRequired(count * 4, stride_, (void*&)m_ringBufferData);
		m_spriteCount = 0;

		vertexCount_ = count * 4;

		instances.clear();
	}

	void Rendering_(const efkSpriteNodeParam& parameter, const efkSpriteInstanceParam& instanceParameter, void* userData, const ::Effekseer::Matrix44& camera)
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
	void Rendering_Internal( const efkSpriteNodeParam& parameter, const efkSpriteInstanceParam& instanceParameter, void* userData, const ::Effekseer::Matrix44& camera )
	{
		if( m_ringBufferData == nullptr ) return;

		StrideView<VERTEX> verteies(m_ringBufferData, stride_, 4);
		
		auto vertexType = GetVertexType((VERTEX*)m_ringBufferData);

		for( int i = 0; i < 4; i++ )
		{
			verteies[i].Pos.X = instanceParameter.Positions[i].X;
			verteies[i].Pos.Y = instanceParameter.Positions[i].Y;
			verteies[i].Pos.Z = 0.0f;
	
			verteies[i].SetColor( instanceParameter.Colors[i] );
		}
		
		verteies[0].UV[0] = instanceParameter.UV.X;
		verteies[0].UV[1] = instanceParameter.UV.Y + instanceParameter.UV.Height;
	
		verteies[1].UV[0] = instanceParameter.UV.X + instanceParameter.UV.Width;
		verteies[1].UV[1] = instanceParameter.UV.Y + instanceParameter.UV.Height;
		
		verteies[2].UV[0] = instanceParameter.UV.X;
		verteies[2].UV[1] = instanceParameter.UV.Y;
	
		verteies[3].UV[0] = instanceParameter.UV.X + instanceParameter.UV.Width;
		verteies[3].UV[1] = instanceParameter.UV.Y;

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
			Effekseer::Matrix43 mat_rot;
			Effekseer::Vector3D s;
			Effekseer::Vector3D R;
			Effekseer::Vector3D F;

			CalcBillboard(parameter.Billboard, mat_rot, s, R, F, instanceParameter.SRTMatrix43, m_renderer->GetCameraFrontDirection());

			for (int i = 0; i < 4; i++)
			{
				verteies[i].Pos.X = verteies[i].Pos.X * s.X;
				verteies[i].Pos.Y = verteies[i].Pos.Y * s.Y;
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
				::Effekseer::Vector3D::Transform(
					verteies[i].Pos,
					verteies[i].Pos,
					mat);

				// distortion
				if (vertexType == VertexType::Distortion)
				{
					auto vs = (VERTEX_DISTORTION*) & verteies[i];

					::Effekseer::Vector3D::Transform(
						vs->Tangent,
						vs->Tangent,
						mat);

					::Effekseer::Vector3D::Transform(
						vs->Binormal,
						vs->Binormal,
						mat);

					Effekseer::Vector3D zero;
					::Effekseer::Vector3D::Transform(
						zero,
						zero,
						mat);

					::Effekseer::Vector3D::Normal(vs->Tangent, vs->Tangent - zero);
					::Effekseer::Vector3D::Normal(vs->Binormal, vs->Binormal - zero);
				}
				else if (vertexType == VertexType::Dynamic)
				{
					StrideView<DynamicVertex> vs(verteies.pointerOrigin_, stride_, 4);
					auto tangentX = efkVector3D(mat.Value[0][0], mat.Value[0][1], mat.Value[0][2]);
					auto tangentZ = efkVector3D(mat.Value[2][0], mat.Value[2][1], mat.Value[2][2]);
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
				efkVector3D t;
				t.X = kv.Value.SRTMatrix43.Value[3][0];
				t.Y = kv.Value.SRTMatrix43.Value[3][1];
				t.Z = kv.Value.SRTMatrix43.Value[3][2];

				auto frontDirection = m_renderer->GetCameraFrontDirection();
				if (!param.IsRightHand)
				{
					frontDirection.Z = -frontDirection.Z;
				}

				kv.Key = Effekseer::Vector3D::Dot(t, frontDirection);
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
	typedef ::Effekseer::Vector3D efkVector3D;

	template<typename RENDERER, typename VERTEX_NORMAL, typename VERTEX_DISTORTION>
	class TrackRendererBase
		: public ::Effekseer::TrackRenderer
	{
	protected:
		RENDERER*						m_renderer;
		int32_t							m_ribbonCount;

		int32_t							m_ringBufferOffset;
		uint8_t*						m_ringBufferData;

		efkTrackNodeParam					innstancesNodeParam;
		std::vector<efkTrackInstanceParam>	instances;
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
		void RenderSplines(const ::Effekseer::Matrix44& camera)
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

					::Effekseer::Vector3D s;

					ApplyDepthParameters(mat,
										 m_renderer->GetCameraFrontDirection(),
										 m_renderer->GetCameraPosition(),
										 //s,
										 parameter.DepthParameterPtr,
										 parameter.IsRightHand);

					p.X = mat.Value[3][0];
					p.Y = mat.Value[3][1];
					p.Z = mat.Value[3][2];

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

					::Effekseer::Vector3D s;
					::Effekseer::Matrix43 r;
					::Effekseer::Vector3D t;
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

					v[0].Pos.X = (-size / 2.0f) * s.X;
					v[0].Pos.Y = 0.0f;
					v[0].Pos.Z = 0.0f;
					v[0].SetColor(leftColor);

					v[1].Pos.X = 0.0f;
					v[1].Pos.Y = 0.0f;
					v[1].Pos.Z = 0.0f;
					v[1].SetColor(centerColor);

					v[2].Pos.X = (size / 2.0f) * s.X;
					v[2].Pos.Y = 0.0f;
					v[2].Pos.Z = 0.0f;
					v[2].SetColor(rightColor);

					if (parameter.SplineDivision > 1)
					{
						v[1].Pos = spline.GetValue(param.InstanceIndex + sploop / (float)parameter.SplineDivision);
					}
					else
					{
						v[1].Pos.X = t.X;
						v[1].Pos.Y = t.Y;
						v[1].Pos.Z = t.Z;
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
				Effekseer::Vector3D axisBefore;

				for (size_t i = 0; i < (instances.size() - 1) * parameter.SplineDivision + 1; i++)
				{
					bool isFirst_ = (i == 0);
					bool isLast_ = (i == ((instances.size() - 1) * parameter.SplineDivision));
					Effekseer::Vector3D axis;
					Effekseer::Vector3D pos;

					if (isFirst_)
					{
						axis = (vs_[3].Pos - vs_[1].Pos);
						Effekseer::Vector3D::Normal(axis, axis);
						axisBefore = axis;
					}
					else if (isLast_)
					{
						axis = axisBefore;
					}
					else
					{
						Effekseer::Vector3D axisOld = axisBefore;
						axis = vs_[9].Pos - vs_[7].Pos;
						Effekseer::Vector3D::Normal(axis, axis);
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

					::Effekseer::Vector3D F;
					::Effekseer::Vector3D R;
					::Effekseer::Vector3D U;

					U = axis;

					::Effekseer::Vector3D::Normal(F, -m_renderer->GetCameraFrontDirection());

					::Effekseer::Vector3D::Normal(R, ::Effekseer::Vector3D::Cross(R, U, F));
					::Effekseer::Vector3D::Normal(F, ::Effekseer::Vector3D::Cross(F, R, U));

					::Effekseer::Matrix43 mat_rot;

					mat_rot.Value[0][0] = -R.X;
					mat_rot.Value[0][1] = -R.Y;
					mat_rot.Value[0][2] = -R.Z;
					mat_rot.Value[1][0] = U.X;
					mat_rot.Value[1][1] = U.Y;
					mat_rot.Value[1][2] = U.Z;
					mat_rot.Value[2][0] = F.X;
					mat_rot.Value[2][1] = F.Y;
					mat_rot.Value[2][2] = F.Z;
					mat_rot.Value[3][0] = pos.X;
					mat_rot.Value[3][1] = pos.Y;
					mat_rot.Value[3][2] = pos.Z;

					::Effekseer::Vector3D::Transform(
						vl.Pos,
						vl.Pos,
						mat_rot);

					::Effekseer::Vector3D::Transform(
						vm.Pos,
						vm.Pos,
						mat_rot);

					::Effekseer::Vector3D::Transform(
						vr.Pos,
						vr.Pos,
						mat_rot);


					if (vertexType == VertexType::Distortion)
					{
						auto vl_ = (VERTEX_DISTORTION*)(&vl);
						auto vm_ = (VERTEX_DISTORTION*)(&vm);
						auto vr_ = (VERTEX_DISTORTION*)(&vr);

						vl_->Binormal = axis;
						vm_->Binormal = axis;
						vr_->Binormal = axis;

						::Effekseer::Vector3D tangent;
						::Effekseer::Vector3D::Normal(tangent, vr_->Pos - vl_->Pos);

						vl_->Tangent = tangent;
						vm_->Tangent = tangent;
						vr_->Tangent = tangent;
					}
					else if (vertexType == VertexType::Dynamic)
					{
						auto vl_ = (DynamicVertex*)(&vl);
						auto vm_ = (DynamicVertex*)(&vm);
						auto vr_ = (DynamicVertex*)(&vr);

						::Effekseer::Vector3D tangent;
						::Effekseer::Vector3D::Normal(tangent, vr_->Pos - vl_->Pos);

						Effekseer::Vector3D normal;
						Effekseer::Vector3D::Cross(normal, tangent, axis);
						Effekseer::Vector3D::Normal(normal, normal);

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

		void Rendering_(const efkTrackNodeParam& parameter, const efkTrackInstanceParam& instanceParameter, void* userData, const ::Effekseer::Matrix44& camera)
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
		void Rendering_Internal(const efkTrackNodeParam& parameter, const efkTrackInstanceParam& instanceParameter, void* userData, const ::Effekseer::Matrix44& camera)
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

			state.Distortion = param.BasicParameterPtr->MaterialType == Effekseer::RendererMaterialType::BackDistortion;
			state.DistortionIntensity = param.BasicParameterPtr->DistortionIntensity;
			state.MaterialType = param.BasicParameterPtr->MaterialType;

			state.CopyMaterialFromParameterToState(param.EffectPointer,
												   param.BasicParameterPtr->MaterialParameterPtr,
												   param.BasicParameterPtr->Texture1Index,
												   param.BasicParameterPtr->Texture2Index);
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

#ifndef	__EFFEKSEERRENDERER_GL_BASE_PRE_H__
#define	__EFFEKSEERRENDERER_GL_BASE_PRE_H__

//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------
#include "EffekseerNative.h"
#include <vector>

#ifdef _WIN32
#include <Windows.h>
#endif

#if defined(__EFFEKSEER_RENDERER_GL_GLEW__)

#if _WIN32
#include <GL/gl.h>
#elif defined(__APPLE__)
#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#elif defined(__EFFEKSEER_RENDERER_GL_GLEW_S__)

#if _WIN32
#include <GL/gl.h>
#elif defined(__APPLE__)
#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl3.h>
#else
#define GLEW_STATIC
#include <GL/glew.h>
#endif

#elif defined(__EFFEKSEER_RENDERER_GLES2__)

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

#if defined(__APPLE__)
#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#else

#if defined(__APPLE__)
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

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace EffekseerRendererGL
{
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
class Renderer;

enum class OpenGLDeviceType
{
	OpenGL2,
	OpenGL3,
	OpenGLES2,
	OpenGLES3,
	Emscripten,
};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
}
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#endif	// __EFFEKSEERRENDERER_GL_BASE_PRE_H__


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
::Effekseer::TextureLoader* CreateTextureLoader(::Effekseer::FileInterface* fileInterface = NULL);

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
	@brief	背景を取得する。
	*/
	virtual Effekseer::TextureData* GetBackground() = 0;

	/**
	@brief	背景を設定する。
	*/
	virtual void SetBackground(GLuint background) = 0;

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


	Model(void* data, int32_t size);
	~Model();
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


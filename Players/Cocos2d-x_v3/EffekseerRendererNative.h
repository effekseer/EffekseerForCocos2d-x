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

//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------
#include "EffekseerNative.h"
#include <assert.h>
#include <string.h>
#include <math.h>
#include <array>

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
namespace EffekseerRenderer
{
//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------

	static void ApplyDepthOffset(::Effekseer::Matrix43& mat, const ::Effekseer::Matrix44& camera, float depthOffset, bool isDepthOffsetScaledWithCamera, bool isDepthOffsetScaledWithEffect, bool isRightHand)
	{
		if (depthOffset != 0)
		{
			auto f = ::Effekseer::Vector3D(camera.Values[0][2], camera.Values[1][2], camera.Values[2][2]);

			auto offset = depthOffset;

			if (isDepthOffsetScaledWithEffect)
			{
				std::array<float, 3> scales;
				scales.fill(0.0);

				for (auto r = 0; r < 3; r++)
				{
					for (auto c = 0; c < 3; c++)
					{
						scales[c] += mat.Value[c][r] * mat.Value[c][r];
					}
				}

				for (auto c = 0; c < 3; c++)
				{
					scales[c] = sqrt(scales[c]);
				}

				auto scale = (scales[0] + scales[1] + scales[2]) / 3.0f;

				offset *= scale;
			}

			if (isDepthOffsetScaledWithCamera)
			{
				auto cx = mat.Value[3][0] + camera.Values[3][0];
				auto cy = mat.Value[3][1] + camera.Values[3][1];
				auto cz = mat.Value[3][2] + camera.Values[3][2];
				auto cl = sqrt(cx * cx + cy * cy + cz * cz);

				if (cl != 0.0)
				{
					auto scale = (cl - offset) / cl;

					for (auto r = 0; r < 3; r++)
					{
						for (auto c = 0; c < 3; c++)
						{
							mat.Value[c][r] *= scale;
						}
					}
				}
			}

			if (isRightHand)
			{
				mat.Value[3][0] += f.X * offset;
				mat.Value[3][1] += f.Y * offset;
				mat.Value[3][2] += f.Z * offset;
			}
			else
			{
				mat.Value[3][0] -= f.X * offset;
				mat.Value[3][1] -= f.Y * offset;
				mat.Value[3][2] -= f.Z * offset;
			}
		}
	}

	static void ApplyDepthOffset(::Effekseer::Matrix43& mat, const ::Effekseer::Matrix44& camera, ::Effekseer::Vector3D& scaleValues, float depthOffset, bool isDepthOffsetScaledWithCamera, bool isDepthOffsetScaledWithEffect, bool isRightHand)
{
	if (depthOffset != 0)
	{
		auto f = ::Effekseer::Vector3D(camera.Values[0][2], camera.Values[1][2], camera.Values[2][2]);

		auto offset = depthOffset;
		
		if (isDepthOffsetScaledWithEffect)
		{
			auto scale = (scaleValues.X + scaleValues.Y + scaleValues.Z) / 3.0f;

			offset *= scale;
		}

		if (isDepthOffsetScaledWithCamera)
		{
			auto cx = mat.Value[3][0] + camera.Values[3][0];
			auto cy = mat.Value[3][1] + camera.Values[3][1];
			auto cz = mat.Value[3][2] + camera.Values[3][2];
			auto cl = sqrt(cx * cx + cy * cy + cz * cz);

			if (cl != 0.0)
			{
				auto scale = (cl - offset) / cl;

				for (auto r = 0; r < 3; r++)
				{
					for (auto c = 0; c < 3; c++)
					{
						mat.Value[c][r] *= scale;
					}
				}
			}
		}

		if (isRightHand)
		{
			mat.Value[3][0] += f.X * offset;
			mat.Value[3][1] += f.Y * offset;
			mat.Value[3][2] += f.Z * offset;
		}
		else
		{
			mat.Value[3][0] -= f.X * offset;
			mat.Value[3][1] -= f.Y * offset;
			mat.Value[3][2] -= f.Z * offset;
		}

	}
}

	static void ApplyDepthOffset(::Effekseer::Matrix44& mat, const ::Effekseer::Matrix44& camera, float depthOffset, bool isDepthOffsetScaledWithCamera, bool isDepthOffsetScaledWithEffect, bool isRightHand)
{
	if (depthOffset != 0)
	{
		auto f = ::Effekseer::Vector3D(camera.Values[0][2], camera.Values[1][2], camera.Values[2][2]);

		auto offset = depthOffset;

		if (isDepthOffsetScaledWithEffect)
		{
			std::array<float, 3> scales;
			scales.fill(0.0);

			for (auto r = 0; r < 3; r++)
			{
				for (auto c = 0; c < 3; c++)
				{
					scales[c] += mat.Values[c][r] * mat.Values[c][r];
				}
			}

			for (auto c = 0; c < 3; c++)
			{
				scales[c] = sqrt(scales[c]);
			}

			auto scale = (scales[0] + scales[1] + scales[2]) / 3.0f;

			offset *= scale;
		}

		if (isDepthOffsetScaledWithCamera)
		{
			auto cx = mat.Values[3][0] + camera.Values[3][0];
			auto cy = mat.Values[3][1] + camera.Values[3][1];
			auto cz = mat.Values[3][2] + camera.Values[3][2];
			auto cl = sqrt(cx * cx + cy * cy + cz * cz);

			if (cl != 0.0)
			{
				auto scale = (cl - offset) / cl;

				for (auto r = 0; r < 3; r++)
				{
					for (auto c = 0; c < 3; c++)
					{
						mat.Values[c][r] *= scale;
					}
				}
			}
		}

		if (isRightHand)
		{
			mat.Values[3][0] += f.X * offset;
			mat.Values[3][1] += f.Y * offset;
			mat.Values[3][2] += f.Z * offset;
		}
		else
		{
			mat.Values[3][0] -= f.X * offset;
			mat.Values[3][1] -= f.Y * offset;
			mat.Values[3][2] -= f.Z * offset;
		}

	}
}

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
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
//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
class Renderer
	: ::Effekseer::IReference
{
protected:
	Renderer() {}
	virtual ~Renderer() {}

public:
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
	virtual void SetLightDirection( ::Effekseer::Vector3D& direction ) = 0;

	/**
		@brief	ライトの色を取得する。
	*/
	virtual const ::Effekseer::Color& GetLightColor() const = 0;

	/**
		@brief	ライトの色を設定する。
	*/
	virtual void SetLightColor( ::Effekseer::Color& color ) = 0;

	/**
		@brief	ライトの環境光の色を取得する。
	*/
	virtual const ::Effekseer::Color& GetLightAmbientColor() const = 0;

	/**
		@brief	ライトの環境光の色を設定する。
	*/
	virtual void SetLightAmbientColor( ::Effekseer::Color& color ) = 0;

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
};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
}
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#endif	// __EFFEKSEERRENDERER_RENDERER_H__
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
		::Effekseer::TextureFilterType		TextureFilterTypes[4];
		::Effekseer::TextureWrapType		TextureWrapTypes[4];

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
	virtual bool RingBufferLock( int32_t size, int32_t& offset, void*& data ) = 0;

	/**
		@brief	バッファ全体を再確保せずに、試しに一部をロックしてみる。
	*/
	virtual bool TryRingBufferLock(int32_t size, int32_t& offset, void*& data) = 0;

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
#ifndef	__EFFEKSEERRENDERER_STANDARD_RENDERER_BASE_H__
#define	__EFFEKSEERRENDERER_STANDARD_RENDERER_BASE_H__

//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------
#include "EffekseerNative.h"
#include <vector>


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
	bool								DepthTest;
	bool								DepthWrite;
	bool								Distortion;
	float								DistortionIntensity;

	::Effekseer::AlphaBlendType			AlphaBlend;
	::Effekseer::CullingType			CullingType;
	::Effekseer::TextureFilterType		TextureFilterType;
	::Effekseer::TextureWrapType		TextureWrapType;
	::Effekseer::TextureData*			TexturePtr;

	StandardRendererState()
	{
		DepthTest = false;
		DepthWrite = false;
		Distortion = false;
		DistortionIntensity = 1.0f;

		AlphaBlend = ::Effekseer::AlphaBlendType::Blend;
		CullingType = ::Effekseer::CullingType::Front;
		TextureFilterType = ::Effekseer::TextureFilterType::Nearest;
		TextureWrapType = ::Effekseer::TextureWrapType::Repeat;
		TexturePtr = nullptr;
	}

	bool operator != (const StandardRendererState state)
	{
		if (DepthTest != state.DepthTest) return true;
		if (DepthWrite != state.DepthWrite) return true;
		if (Distortion != state.Distortion) return true;
		if (DistortionIntensity != state.DistortionIntensity) return true;
		if (AlphaBlend != state.AlphaBlend) return true;
		if (CullingType != state.CullingType) return true;
		if (TextureFilterType != state.TextureFilterType) return true;
		if (TextureWrapType != state.TextureWrapType) return true;
		if (TexturePtr != state.TexturePtr) return true;
		return false;
	}
};

template<typename RENDERER, typename SHADER, typename VERTEX, typename VERTEX_DISTORTION>
class StandardRenderer
{

private:
	RENDERER*	m_renderer;
	SHADER*		m_shader;
	SHADER*		m_shader_no_texture;

	SHADER*		m_shader_distortion;
	SHADER*		m_shader_no_texture_distortion;

	Effekseer::TextureData*		m_texture;

	StandardRendererState		m_state;

	std::vector<uint8_t>		vertexCaches;
	int32_t						vertexCacheMaxSize;

	bool						m_isDistortionMode;
public:

	StandardRenderer(RENDERER* renderer, SHADER* shader, SHADER* shader_no_texture, SHADER* shader_distortion, SHADER* shader_no_texture_distortion)
		: vertexCacheMaxSize(0)
		, m_isDistortionMode(false)
	{
		m_renderer = renderer;
		m_shader = shader;
		m_shader_no_texture = shader_no_texture;
		m_shader_distortion = shader_distortion;
		m_shader_no_texture_distortion = shader_no_texture_distortion;

		vertexCaches.reserve(m_renderer->GetVertexBuffer()->GetMaxSize());
		vertexCacheMaxSize = m_renderer->GetVertexBuffer()->GetMaxSize();
	}

	void UpdateStateAndRenderingIfRequired(StandardRendererState state)
	{
		if(m_state != state)
		{
			Rendering();
		}

		m_state = state;

		m_isDistortionMode = m_state.Distortion;
	}

	void BeginRenderingAndRenderingIfRequired(int32_t count, int32_t& offset, void*& data)
	{
		if (m_isDistortionMode)
		{
			if (count * sizeof(VERTEX_DISTORTION) + vertexCaches.size() > vertexCacheMaxSize)
			{
				Rendering();
			}

			auto old = vertexCaches.size();
			vertexCaches.resize(count * sizeof(VERTEX_DISTORTION) + vertexCaches.size());
			offset = old;
			data = (vertexCaches.data() + old);
		}
		else
		{
			if (count * sizeof(VERTEX) + vertexCaches.size() > vertexCacheMaxSize)
			{
				Rendering();
			}

			auto old = vertexCaches.size();
			vertexCaches.resize(count * sizeof(VERTEX) + vertexCaches.size());
			offset = old;
			data = (vertexCaches.data() + old);
		}
	}

	void ResetAndRenderingIfRequired()
	{
		Rendering();

		// It is always initialized with the next drawing.
		m_state.TexturePtr = (Effekseer::TextureData*)0x1;
	}

	void Rendering(const Effekseer::Matrix44& mCamera, const Effekseer::Matrix44& mProj)
	{
		if (vertexCaches.size() == 0) return;

		if (m_state.Distortion)
		{
			auto callback = m_renderer->GetDistortingCallback();
			if (callback != nullptr)
			{
				if (!callback->OnDistorting())
				{
					vertexCaches.clear();
					return;
				}
			}
		}

		if (m_state.Distortion && m_renderer->GetBackground() == 0)
		{
			vertexCaches.clear();
			return;
		}

		int32_t vertexSize = vertexCaches.size();
		int32_t offsetSize = 0;
		{
			VertexBufferBase* vb = m_renderer->GetVertexBuffer();

			void* data = nullptr;

			if (m_state.Distortion)
			{
				// For OpenGL ES(Because OpenGL ES 3.2 and later can only realize a vertex layout variable ring buffer)
				vb->Lock();
				data = vb->GetBufferDirect(vertexCaches.size());
				if (data == nullptr)
				{
					vertexCaches.clear();
					return;
				}
				memcpy(data, vertexCaches.data(), vertexCaches.size());
				vb->Unlock();
			}
			else if (vb->RingBufferLock(vertexCaches.size(), offsetSize, data))
			{
				assert(data != nullptr);
				memcpy(data, vertexCaches.data(), vertexCaches.size());
				vb->Unlock();
			}
			else
			{
				// 現状、描画するインスタンス数が多すぎる場合は描画しなくしている
				vertexCaches.clear();
				return;
			}

			vertexCaches.clear();

		}

		RenderStateBase::State& state = m_renderer->GetRenderState()->Push();
		state.DepthTest = m_state.DepthTest;
		state.DepthWrite = m_state.DepthWrite;
		state.CullingType = m_state.CullingType;
		state.AlphaBlend = m_state.AlphaBlend;

		SHADER* shader_ = nullptr;

		bool distortion = m_state.Distortion;

		if (distortion)
		{
			if (m_state.TexturePtr != nullptr)
			{
				shader_ = m_shader_distortion;
			}
			else
			{
				shader_ = m_shader_no_texture_distortion;
			}
		}
		else
		{
			if (m_state.TexturePtr != nullptr)
			{
				shader_ = m_shader;
			}
			else
			{
				shader_ = m_shader_no_texture;
			}
		}

		m_renderer->BeginShader(shader_);

		Effekseer::TextureData* textures[2];

		if (m_state.TexturePtr != nullptr)
		{
			textures[0] = m_state.TexturePtr;
		}
		else
		{
			textures[0] = 0;
		}

		if (distortion)
		{
			textures[1] = m_renderer->GetBackground();
			m_renderer->SetTextures(shader_, textures, 2);
		}
		else
		{
			m_renderer->SetTextures(shader_, textures, 1);
		}

		((Effekseer::Matrix44*)(shader_->GetVertexConstantBuffer()))[0] = mCamera;
		((Effekseer::Matrix44*)(shader_->GetVertexConstantBuffer()))[1] = mProj;

		if (distortion)
		{
			((float*) (shader_->GetPixelConstantBuffer()))[0] = m_state.DistortionIntensity;
		}

		shader_->SetConstantBuffer();

		state.TextureFilterTypes[0] = m_state.TextureFilterType;
		state.TextureWrapTypes[0] = m_state.TextureWrapType;

		m_renderer->GetRenderState()->Update(distortion);

		if (distortion)
		{
			m_renderer->SetVertexBuffer(m_renderer->GetVertexBuffer(), sizeof(VERTEX_DISTORTION));
			m_renderer->SetIndexBuffer(m_renderer->GetIndexBuffer());
			m_renderer->SetLayout(shader_);
			m_renderer->DrawSprites(vertexSize / sizeof(VERTEX_DISTORTION) / 4, offsetSize / sizeof(VERTEX_DISTORTION));
		}
		else
		{
			int32_t spriteCount = vertexSize / sizeof(VERTEX) / 4;
			m_renderer->SetVertexBuffer(m_renderer->GetVertexBuffer(), sizeof(VERTEX));
			m_renderer->SetIndexBuffer(m_renderer->GetIndexBuffer());
			m_renderer->SetLayout(shader_);
			m_renderer->DrawSprites(spriteCount, offsetSize / sizeof(VERTEX));
		}

		m_renderer->EndShader(shader_);

		m_renderer->GetRenderState()->Pop();
	}

	void Rendering()
	{
		Rendering(m_renderer->GetCameraMatrix(), m_renderer->GetProjectionMatrix());
	}
};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
}
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#endif	// __EFFEKSEERRENDERER_STANDARD_RENDERER_H__
#ifndef	__EFFEKSEERRENDERER_MODEL_RENDERER_BASE_H__
#define	__EFFEKSEERRENDERER_MODEL_RENDERER_BASE_H__

//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------
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
	std::vector<Effekseer::Matrix44>	m_matrixes;
	std::vector<Effekseer::RectF>		m_uv;
	std::vector<Effekseer::Color>		m_colors;
	std::vector<int32_t>				m_times;

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
		m_matrixes.clear();
		m_uv.clear();
		m_colors.clear();
		m_times.clear();

		renderer->GetStandardRenderer()->ResetAndRenderingIfRequired();
	}

	template<typename RENDERER>
	void Rendering_(RENDERER* renderer, const efkModelNodeParam& parameter, const efkModelInstanceParam& instanceParameter, void* userData)
	{
		auto camera = renderer->GetCameraMatrix();
		::Effekseer::BillboardType btype = parameter.Billboard;
		Effekseer::Matrix44 mat44;

		if (btype == ::Effekseer::BillboardType::Billboard ||
			btype == ::Effekseer::BillboardType::RotatedBillboard ||
			btype == ::Effekseer::BillboardType::YAxisFixed)
		{
			const ::Effekseer::Matrix43& mat = instanceParameter.SRTMatrix43;
			::Effekseer::Vector3D s;
			::Effekseer::Matrix43 r;
			::Effekseer::Vector3D t;
			mat.GetSRT(s, r, t);

			::Effekseer::Vector3D F;
			::Effekseer::Vector3D R;
			::Effekseer::Vector3D U;

			if (btype == ::Effekseer::BillboardType::Billboard)
			{
				::Effekseer::Vector3D Up(0.0f, 1.0f, 0.0f);

				::Effekseer::Vector3D::Normal(F, ::Effekseer::Vector3D(-camera.Values[0][2], -camera.Values[1][2], -camera.Values[2][2]));

				::Effekseer::Vector3D::Normal(R, ::Effekseer::Vector3D::Cross(R, Up, F));
				::Effekseer::Vector3D::Normal(U, ::Effekseer::Vector3D::Cross(U, F, R));
			}
			else if (btype == ::Effekseer::BillboardType::RotatedBillboard)
			{
				::Effekseer::Vector3D Up(0.0f, 1.0f, 0.0f);

				::Effekseer::Vector3D::Normal(F, ::Effekseer::Vector3D(-camera.Values[0][2], -camera.Values[1][2], -camera.Values[2][2]));

				::Effekseer::Vector3D::Normal(R, ::Effekseer::Vector3D::Cross(R, Up, F));
				::Effekseer::Vector3D::Normal(U, ::Effekseer::Vector3D::Cross(U, F, R));

				float c_zx = sqrt(1.0f - r.Value[2][1] * r.Value[2][1]);
				float s_z = 0.0f;
				float c_z = 0.0f;

				if (fabsf(c_zx) > 0.05f)
				{
					s_z = -r.Value[0][1] / c_zx;
					c_z = sqrt(1.0f - s_z * s_z);
					if (r.Value[1][1] < 0.0f) c_z = -c_z;
				}
				else
				{
					s_z = 0.0f;
					c_z = 1.0f;
				}

				::Effekseer::Vector3D r_temp = R;
				::Effekseer::Vector3D u_temp = U;

				R.X = r_temp.X * c_z + u_temp.X * s_z;
				R.Y = r_temp.Y * c_z + u_temp.Y * s_z;
				R.Z = r_temp.Z * c_z + u_temp.Z * s_z;

				U.X = u_temp.X * c_z - r_temp.X * s_z;
				U.Y = u_temp.Y * c_z - r_temp.Y * s_z;
				U.Z = u_temp.Z * c_z - r_temp.Z * s_z;
			}
			else if (btype == ::Effekseer::BillboardType::YAxisFixed)
			{
				U = ::Effekseer::Vector3D(r.Value[1][0], r.Value[1][1], r.Value[1][2]);

				::Effekseer::Vector3D::Normal(F, ::Effekseer::Vector3D(-camera.Values[0][2], -camera.Values[1][2], -camera.Values[2][2]));

				::Effekseer::Vector3D::Normal(R, ::Effekseer::Vector3D::Cross(R, U, F));
				::Effekseer::Vector3D::Normal(F, ::Effekseer::Vector3D::Cross(F, R, U));
			}

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

			::Effekseer::Matrix43 mat_scale;
			mat_scale.Scaling(s.X, s.Y, s.Z);
			::Effekseer::Matrix43::Multiple(mat_rot, mat_scale, mat_rot);

			for (int32_t r = 0; r < 4; r++)
			{
				for (int32_t c = 0; c < 3; c++)
				{
					mat44.Values[r][c] = mat_rot.Value[r][c];
				}
			}
		}
		else if (btype == ::Effekseer::BillboardType::Fixed)
		{
			for (int32_t r = 0; r < 4; r++)
			{
				for (int32_t c = 0; c < 3; c++)
				{
					mat44.Values[r][c] = instanceParameter.SRTMatrix43.Value[r][c];
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
	}

	template<typename RENDERER, typename SHADER, typename MODEL, bool Instancing, int InstanceCount>
	void EndRendering_(
		RENDERER* renderer, 
		SHADER* shader_lighting_texture_normal,
		SHADER* shader_lighting_normal,
		SHADER* shader_lighting_texture,
		SHADER* shader_lighting,
		SHADER* shader_texture,
		SHADER* shader,
		SHADER* shader_distortion_texture,
		SHADER* shader_distortion,
		const efkModelNodeParam& param)
	{
		if (m_matrixes.size() == 0) return;
		if (param.ModelIndex < 0) return;

		auto camera = renderer->GetCameraMatrix();

		MODEL* model = (MODEL*) param.EffectPointer->GetModel(param.ModelIndex);
		if (model == NULL) return;
		
		auto distortion = param.Distortion;
		if (distortion && renderer->GetBackground() == 0) return;

		if (distortion)
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

		RenderStateBase::State& state = renderer->GetRenderState()->Push();
		state.DepthTest = param.ZTest;
		state.DepthWrite = param.ZWrite;
		state.AlphaBlend = param.AlphaBlend;
		state.CullingType = param.Culling;

		/*シェーダー選択*/
		SHADER* shader_ = NULL;
		if (distortion)
		{
			if (param.ColorTextureIndex >= 0)
			{
				shader_ = shader_distortion_texture;
			}
			else
			{
				shader_ = shader_distortion;
			}
		}
		else if (param.Lighting)
		{
			if (param.NormalTextureIndex >= 0)
			{
				if (param.ColorTextureIndex >= 0)
				{
					shader_ = shader_lighting_texture_normal;
				}
				else
				{
					shader_ = shader_lighting_normal;
				}
			}
			else
			{
				if (param.ColorTextureIndex >= 0)
				{
					shader_ = shader_lighting_texture;
				}
				else
				{
					shader_ = shader_lighting;
				}
			}
		}
		else
		{
			if (param.ColorTextureIndex >= 0)
			{
				shader_ = shader_texture;
			}
			else
			{
				shader_ = shader;
			}
		}

		renderer->BeginShader(shader_);

		// Select texture
		Effekseer::TextureData* textures[2];
		textures[0] = nullptr;
		textures[1] = nullptr;

		if (distortion)
		{
			if (param.ColorTextureIndex >= 0)
			{
				textures[0] = param.EffectPointer->GetDistortionImage(param.ColorTextureIndex);
			}

			textures[1] = renderer->GetBackground();
		}
		else
		{
			if (param.ColorTextureIndex >= 0)
			{
				textures[0] = param.EffectPointer->GetColorImage(param.ColorTextureIndex);
			}

			if (param.NormalTextureIndex >= 0)
			{
				textures[1] = param.EffectPointer->GetNormalImage(param.NormalTextureIndex);
			}
		}
		
		
		renderer->SetTextures(shader_, textures, 2);

		state.TextureFilterTypes[0] = param.TextureFilter;
		state.TextureWrapTypes[0] = param.TextureWrap;
		state.TextureFilterTypes[1] = param.TextureFilter;
		state.TextureWrapTypes[1] = param.TextureWrap;

		renderer->GetRenderState()->Update(distortion);

		ModelRendererVertexConstantBuffer<InstanceCount>* vcb = (ModelRendererVertexConstantBuffer<InstanceCount>*)shader_->GetVertexConstantBuffer();

		if (distortion)
		{
			float* pcb = (float*) shader_->GetPixelConstantBuffer();
			pcb[0] = param.DistortionIntensity;
		}
		else
		{
			ModelRendererPixelConstantBuffer* pcb = (ModelRendererPixelConstantBuffer*) shader_->GetPixelConstantBuffer();

			// 固定値設定
			if (param.Lighting)
			{
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
		auto stTime = m_times[0] % model->GetFrameCount();
		auto isTimeSame = true;

		for (auto t : m_times)
		{
			t = t % model->GetFrameCount();
			if(t != stTime)
			{
				isTimeSame = false;
				break;
			}
		}

		if(Instancing && isTimeSame)
		{
			auto& imodel = model->InternalModels[stTime];

			// Invalid unless layout is set after buffer
			renderer->SetVertexBuffer(imodel.VertexBuffer, sizeof(Effekseer::Model::VertexWithIndex));
			renderer->SetIndexBuffer(imodel.IndexBuffer);
			renderer->SetLayout(shader_);

			for( size_t loop = 0; loop < m_matrixes.size(); )
			{
				int32_t modelCount = Effekseer::Min( m_matrixes.size() - loop, model->ModelCount );
				
				for( int32_t num = 0; num < modelCount; num++ )
				{
					vcb->ModelMatrix[num] = m_matrixes[loop+num];

					// DepthOffset
					ApplyDepthOffset(vcb->ModelMatrix[num], camera, param.DepthOffset, param.IsDepthOffsetScaledWithCamera, param.IsDepthOffsetScaledWithParticleScale, param.IsRightHand);
	
					vcb->ModelUV[num][0] = m_uv[loop+num].X;
					vcb->ModelUV[num][1] = m_uv[loop+num].Y;
					vcb->ModelUV[num][2] = m_uv[loop+num].Width;
					vcb->ModelUV[num][3] = m_uv[loop+num].Height;

					ColorToFloat4(m_colors[loop+num],vcb->ModelColor[num]);
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
				renderer->SetVertexBuffer(imodel.VertexBuffer, sizeof(Effekseer::Model::Vertex));
				renderer->SetIndexBuffer(imodel.IndexBuffer);
				renderer->SetLayout(shader_);

				vcb->ModelMatrix[0] = m_matrixes[loop];
				vcb->ModelUV[0][0] = m_uv[loop].X;
				vcb->ModelUV[0][1] = m_uv[loop].Y;
				vcb->ModelUV[0][2] = m_uv[loop].Width;
				vcb->ModelUV[0][3] = m_uv[loop].Height;

				// DepthOffset
				ApplyDepthOffset(vcb->ModelMatrix[0], camera, param.DepthOffset, param.IsDepthOffsetScaledWithCamera, param.IsDepthOffsetScaledWithParticleScale, param.IsRightHand);
				
				ColorToFloat4( m_colors[loop], vcb->ModelColor[0] );
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
protected:
	RENDERER*						m_renderer;
	int32_t							m_ribbonCount;

	int32_t							m_ringBufferOffset;
	uint8_t*						m_ringBufferData;

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

	void BeginRendering_(RENDERER* renderer, int32_t count, const efkRibbonNodeParam& param)
	{
		m_ribbonCount = 0;
		int32_t vertexCount = (count - 1) * 4;
		if (vertexCount <= 0) return;

		EffekseerRenderer::StandardRendererState state;
		state.AlphaBlend = param.AlphaBlend;
		state.CullingType = ::Effekseer::CullingType::Double;
		state.DepthTest = param.ZTest;
		state.DepthWrite = param.ZWrite;
		state.TextureFilterType = param.TextureFilter;
		state.TextureWrapType = param.TextureWrap;

		state.Distortion = param.Distortion;
		state.DistortionIntensity = param.DistortionIntensity;

		if (param.ColorTextureIndex >= 0)
		{
			if (state.Distortion)
			{
				state.TexturePtr = param.EffectPointer->GetDistortionImage(param.ColorTextureIndex);
			}
			else
			{
				state.TexturePtr = param.EffectPointer->GetColorImage(param.ColorTextureIndex);
			}
		}
		else
		{
			state.TexturePtr = nullptr;
		}

		renderer->GetStandardRenderer()->UpdateStateAndRenderingIfRequired(state);

		renderer->GetStandardRenderer()->BeginRenderingAndRenderingIfRequired(vertexCount, m_ringBufferOffset, (void*&) m_ringBufferData);
	}

	void Rendering_(const efkRibbonNodeParam& parameter, const efkRibbonInstanceParam& instanceParameter, void* userData, const ::Effekseer::Matrix44& camera)
	{
		if (parameter.Distortion)
		{
			Rendering_Internal<VERTEX_DISTORTION>(parameter, instanceParameter, userData, camera);
		}
		else
		{
			Rendering_Internal<VERTEX_NORMAL>(parameter, instanceParameter, userData, camera);
		}
	}

	template<typename VERTEX>
	void Rendering_Internal( const efkRibbonNodeParam& parameter, const efkRibbonInstanceParam& instanceParameter, void* userData, const ::Effekseer::Matrix44& camera )
	{
		if( m_ringBufferData == NULL ) return;
		if( instanceParameter.InstanceCount < 2 ) return;
	
		bool isFirst = instanceParameter.InstanceIndex == 0;
		bool isLast = instanceParameter.InstanceIndex == (instanceParameter.InstanceCount - 1);
	
		VERTEX* verteies = (VERTEX*)m_ringBufferData;
	
		for( int i = 0; i < 2; i++ )
		{
			verteies[i].Pos.X = instanceParameter.Positions[i];
			verteies[i].Pos.Y = 0.0f;
			verteies[i].Pos.Z = 0.0f;
			verteies[i].SetColor( instanceParameter.Colors[i] );
		}

		float percent = (float) instanceParameter.InstanceIndex / (float) (instanceParameter.InstanceCount - 1);
		
		verteies[0].UV[0] = instanceParameter.UV.X;
		verteies[0].UV[1] = instanceParameter.UV.Y + percent * instanceParameter.UV.Height;
	
		verteies[1].UV[0] = instanceParameter.UV.X + instanceParameter.UV.Width;
		verteies[1].UV[1] = instanceParameter.UV.Y + percent * instanceParameter.UV.Height;
		
		if( parameter.ViewpointDependent)
		{
			const ::Effekseer::Matrix43& mat = instanceParameter.SRTMatrix43;
			::Effekseer::Vector3D s;
			::Effekseer::Matrix43 r;
			::Effekseer::Vector3D t;
			mat.GetSRT( s, r, t );
	
			// 拡大
			for( int i = 0; i < 2; i++ )
			{
				verteies[i].Pos.X = verteies[i].Pos.X * s.X;
			}
	
			::Effekseer::Vector3D F;
			::Effekseer::Vector3D R;
			::Effekseer::Vector3D U;
	
			U = ::Effekseer::Vector3D( r.Value[1][0], r.Value[1][1], r.Value[1][2] );
	
			::Effekseer::Vector3D::Normal( F, ::Effekseer::Vector3D( - camera.Values[0][2], - camera.Values[1][2], - camera.Values[2][2] ) );
	
			::Effekseer::Vector3D::Normal( R, ::Effekseer::Vector3D::Cross( R, U, F ) );
			::Effekseer::Vector3D::Normal( F, ::Effekseer::Vector3D::Cross( F, R, U ) );
			
			::Effekseer::Matrix43 mat_rot;
	
			mat_rot.Value[0][0] = - R.X;
			mat_rot.Value[0][1] = - R.Y;
			mat_rot.Value[0][2] = - R.Z;
			mat_rot.Value[1][0] = U.X;
			mat_rot.Value[1][1] = U.Y;
			mat_rot.Value[1][2] = U.Z;
			mat_rot.Value[2][0] = F.X;
			mat_rot.Value[2][1] = F.Y;
			mat_rot.Value[2][2] = F.Z;
			mat_rot.Value[3][0] = t.X;
			mat_rot.Value[3][1] = t.Y;
			mat_rot.Value[3][2] = t.Z;
	
			for( int i = 0; i < 2; i++ )
			{
				::Effekseer::Vector3D::Transform(
					verteies[i].Pos,
					verteies[i].Pos,
					mat_rot );
			}
		}
		else
		{
			for( int i = 0; i < 2; i++ )
			{
				::Effekseer::Vector3D::Transform(
					verteies[i].Pos,
					verteies[i].Pos,
					instanceParameter.SRTMatrix43 );
			}
		}
	
		if( isFirst || isLast )
		{
			m_ringBufferData += sizeof(VERTEX) * 2;
		}
		else
		{
			verteies[2] = verteies[0];
			verteies[3] = verteies[1];
			m_ringBufferData += sizeof(VERTEX) * 4;
		}
	
		if( !isFirst )
		{
			m_ribbonCount++;
		}

		/* 歪みを適用 */
		if (isLast && sizeof(VERTEX) == sizeof(VERTEX_DISTORTION))
		{
			VERTEX_DISTORTION* vs_ = (VERTEX_DISTORTION*) (m_ringBufferData - sizeof(VERTEX_DISTORTION) * (instanceParameter.InstanceCount - 1) * 4);

			Effekseer::Vector3D axisBefore;

			for (int32_t i = 0; i < instanceParameter.InstanceCount; i++)
			{
				bool isFirst_ = (i == 0);
				bool isLast_ = (i == (instanceParameter.InstanceCount - 1));
		
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
	}

	void EndRendering_(RENDERER* renderer, const efkRibbonNodeParam& param)
	{
		/*
		RenderStateBase::State& state = renderer->GetRenderState()->Push();
		state.DepthTest = param.ZTest;
		state.DepthWrite = param.ZWrite;
		state.CullingType = ::Effekseer::CullingType::Double;

		SHADER* shader_ = NULL;
		if (param.ColorTextureIndex >= 0)
		{
			shader_ = shader;
		}
		else
		{
			shader_ = shader_no_texture;
		}

		renderer->BeginShader(shader_);

		if (param.ColorTextureIndex >= 0)
		{
			TEXTURE texture = TexturePointerToTexture<TEXTURE>(param.EffectPointer->GetColorImage(param.ColorTextureIndex));
			renderer->SetTextures(shader_, &texture, 1);
		}
		else
		{
			TEXTURE texture = (TEXTURE)NULL;
			renderer->SetTextures(shader_, &texture, 1);
		}

		((Effekseer::Matrix44*)(shader_->GetVertexConstantBuffer()))[0] = renderer->GetCameraProjectionMatrix();
		shader_->SetConstantBuffer();


		state.AlphaBlend = param.AlphaBlend;
		state.TextureFilterTypes[0] = param.TextureFilter;
		state.TextureWrapTypes[0] = param.TextureWrap;

		renderer->GetRenderState()->Update(false);

		renderer->SetVertexBuffer(renderer->GetVertexBuffer(), sizeof(VERTEX));
		renderer->SetIndexBuffer(renderer->GetIndexBuffer());
		renderer->SetLayout(shader_);
		renderer->DrawSprites(m_ribbonCount, m_ringBufferOffset / sizeof(VERTEX));

		renderer->EndShader(shader_);

		renderer->GetRenderState()->Pop();
		*/
	}

public:

	void BeginRendering(const efkRibbonNodeParam& parameter, int32_t count, void* userData) override
	{
		BeginRendering_(m_renderer, count, parameter);
	}

	void Rendering(const efkRibbonNodeParam& parameter, const efkRibbonInstanceParam& instanceParameter, void* userData) override
	{
		Rendering_(parameter, instanceParameter, userData, m_renderer->GetCameraMatrix());
	}

	void EndRendering(const efkRibbonNodeParam& parameter, void* userData) override
	{
		if (m_ringBufferData == NULL) return;

		if (m_ribbonCount <= 1) return;

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
	RENDERER*						m_renderer;
	int32_t							m_ringBufferOffset;
	uint8_t*						m_ringBufferData;

	int32_t							m_spriteCount;
	int32_t							m_instanceCount;
	::Effekseer::Matrix44			m_singleRenderingMatrix;

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

	void BeginRendering_(RENDERER* renderer, int32_t count, const efkRingNodeParam& param)
	{
		m_spriteCount = 0;
		int32_t vertexCount = param.VertexCount * 8;
		m_instanceCount = count;

		if (count == 1)
		{
			renderer->GetStandardRenderer()->ResetAndRenderingIfRequired();
		}
		
		EffekseerRenderer::StandardRendererState state;
		state.AlphaBlend = param.AlphaBlend;
		state.CullingType = ::Effekseer::CullingType::Double;
		state.DepthTest = param.ZTest;
		state.DepthWrite = param.ZWrite;
		state.TextureFilterType = param.TextureFilter;
		state.TextureWrapType = param.TextureWrap;

		state.Distortion = param.Distortion;
		state.DistortionIntensity = param.DistortionIntensity;

		if (param.ColorTextureIndex >= 0)
		{
			if (state.Distortion)
			{
				state.TexturePtr = param.EffectPointer->GetDistortionImage(param.ColorTextureIndex);
			}
			else
			{
				state.TexturePtr = param.EffectPointer->GetColorImage(param.ColorTextureIndex);
			}
		}
		else
		{
			state.TexturePtr = nullptr;
		}

		renderer->GetStandardRenderer()->UpdateStateAndRenderingIfRequired(state);
		renderer->GetStandardRenderer()->BeginRenderingAndRenderingIfRequired(count * vertexCount, m_ringBufferOffset, (void*&) m_ringBufferData);
	}

	void Rendering_(const efkRingNodeParam& parameter, const efkRingInstanceParam& instanceParameter, void* userData, const ::Effekseer::Matrix44& camera)
	{
		if (parameter.Distortion)
		{
			Rendering_Internal<VERTEX_DISTORTION>(parameter, instanceParameter, userData, camera);
		}
		else
		{
			Rendering_Internal<VERTEX_NORMAL>(parameter, instanceParameter, userData, camera);
		}
	}

	template<typename VERTEX>
	void Rendering_Internal( const efkRingNodeParam& parameter, const efkRingInstanceParam& instanceParameter, void* userData, const ::Effekseer::Matrix44& camera )
	{
		int32_t vertexCount = parameter.VertexCount * 8;
		//Vertex* verteies = (Vertex*)m_renderer->GetVertexBuffer()->GetBufferDirect( sizeof(Vertex) * vertexCount );
		
		VERTEX* verteies = (VERTEX*)m_ringBufferData;
		m_ringBufferData += sizeof(VERTEX) * vertexCount;

		const float radian = instanceParameter.ViewingAngle / 180.0f * 3.141592f;
		const float stepAngle = radian / (parameter.VertexCount);
		const float beginAngle = -radian / 2.0f;
		
		const float outerRadius = instanceParameter.OuterLocation.X;
		const float innerRadius = instanceParameter.InnerLocation.X;
		const float centerRadius = innerRadius + (outerRadius - innerRadius) * instanceParameter.CenterRatio;

		const float outerHeight = instanceParameter.OuterLocation.Y;
		const float innerHeight = instanceParameter.InnerLocation.Y;
		const float centerHeight = innerHeight + (outerHeight - innerHeight) * instanceParameter.CenterRatio;
		
		::Effekseer::Color outerColor = instanceParameter.OuterColor;
		::Effekseer::Color innerColor = instanceParameter.InnerColor;
		::Effekseer::Color centerColor = instanceParameter.CenterColor;
		
		const float stepC = cosf(stepAngle);
		const float stepS = sinf(stepAngle);
		float c = cosf(beginAngle);
		float s = sinf(beginAngle);
		::Effekseer::Vector3D outerCurrent( c * outerRadius, s * outerRadius, outerHeight );
		::Effekseer::Vector3D innerCurrent( c * innerRadius, s * innerRadius, innerHeight );
		::Effekseer::Vector3D centerCurrent( c * centerRadius, s * centerRadius, centerHeight );
		float texCurrent = instanceParameter.UV.X;
		const float texStep = instanceParameter.UV.Width / parameter.VertexCount;
		const float v1 = instanceParameter.UV.Y;
		const float v2 = v1 + instanceParameter.UV.Height * 0.5f;
		const float v3 = v1 + instanceParameter.UV.Height;
		
		::Effekseer::Vector3D outerNext, innerNext, centerNext;
		float texNext;

		for( int i = 0; i < vertexCount; i += 8 )
		{
			float old_c = c;
			float old_s = s;

			float t;
			t = c * stepC - s * stepS;
			s = s * stepC + c * stepS;
			c = t;

			outerNext.X = c * outerRadius;
			outerNext.Y = s * outerRadius;
			outerNext.Z = outerHeight;
			innerNext.X = c * innerRadius;
			innerNext.Y = s * innerRadius;
			innerNext.Z = innerHeight;
			centerNext.X = c * centerRadius;
			centerNext.Y = s * centerRadius;
			centerNext.Z = centerHeight;

			texNext = texCurrent + texStep;
			
			VERTEX* v = &verteies[i];
			v[0].Pos = outerCurrent;
			v[0].SetColor( outerColor );
			v[0].UV[0] = texCurrent;
			v[0].UV[1] = v1;

			v[1].Pos = centerCurrent;
			v[1].SetColor( centerColor );
			v[1].UV[0] = texCurrent;
			v[1].UV[1] = v2;

			v[2].Pos = outerNext;
			v[2].SetColor( outerColor );
			v[2].UV[0] = texNext;
			v[2].UV[1] = v1;
			
			v[3].Pos = centerNext;
			v[3].SetColor( centerColor );
			v[3].UV[0] = texNext;
			v[3].UV[1] = v2;

			v[4] = v[1];

			v[5].Pos = innerCurrent;
			v[5].SetColor( innerColor );
			v[5].UV[0] = texCurrent;
			v[5].UV[1] = v3;

			v[6] = v[3];

			v[7].Pos = innerNext;
			v[7].SetColor( innerColor );
			v[7].UV[0] = texNext;
			v[7].UV[1] = v3;

			// 歪み処理
			if (sizeof(VERTEX) == sizeof(VERTEX_DISTORTION))
			{
				auto vs = (VERTEX_DISTORTION*) &verteies[i];
				auto binormalCurrent = v[5].Pos - v[0].Pos;
				auto binormalNext = v[7].Pos - v[2].Pos;

				// 戻す
				float t_b;
				t_b = old_c * (stepC) - old_s * (-stepS);
				auto s_b = old_s * (stepC) + old_c * (-stepS);
				auto c_b = t_b;

				::Effekseer::Vector3D outerBefore;
				outerBefore.X = c_b * outerRadius;
				outerBefore.Y = s_b * outerRadius;
				outerBefore.Z = outerHeight;

				// 次
				auto t_n = c * stepC - s * stepS;
				auto s_n = s * stepC + c * stepS;
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
			outerCurrent = outerNext;
			innerCurrent = innerNext;
			centerCurrent = centerNext;
			texCurrent = texNext;
		}

		if( parameter.Billboard == ::Effekseer::BillboardType::Billboard ||
			parameter.Billboard == ::Effekseer::BillboardType::RotatedBillboard ||
			parameter.Billboard == ::Effekseer::BillboardType::YAxisFixed)
		{
			const ::Effekseer::Matrix43& mat = instanceParameter.SRTMatrix43;
			::Effekseer::Vector3D s;
			::Effekseer::Matrix43 r;
			::Effekseer::Vector3D t;

			mat.GetSRT( s, r, t );

			::Effekseer::Vector3D F;
			::Effekseer::Vector3D R;
			::Effekseer::Vector3D U;

			
			if( parameter.Billboard == ::Effekseer::BillboardType::Billboard )
			{
				::Effekseer::Vector3D Up( 0.0f, 1.0f, 0.0f );

				::Effekseer::Vector3D::Normal( F, ::Effekseer::Vector3D( - camera.Values[0][2], - camera.Values[1][2], - camera.Values[2][2] ) );

				::Effekseer::Vector3D::Normal( R, ::Effekseer::Vector3D::Cross( R, Up, F ) );
				::Effekseer::Vector3D::Normal( U, ::Effekseer::Vector3D::Cross( U, F, R ) );
			}
			else if( parameter.Billboard == ::Effekseer::BillboardType::RotatedBillboard )
			{
				::Effekseer::Vector3D Up( 0.0f, 1.0f, 0.0f );

				::Effekseer::Vector3D::Normal( F, ::Effekseer::Vector3D( - camera.Values[0][2], - camera.Values[1][2], - camera.Values[2][2] ) );

				::Effekseer::Vector3D::Normal( R, ::Effekseer::Vector3D::Cross( R, Up, F ) );
				::Effekseer::Vector3D::Normal( U, ::Effekseer::Vector3D::Cross( U, F, R ) );

				float c_zx = sqrt( 1.0f - r.Value[2][1] * r.Value[2][1] );
				float s_z = 0.0f;
				float c_z = 0.0f;

				if( fabsf( c_zx ) > 0.05f )
				{
					s_z = - r.Value[0][1] / c_zx;
					c_z = sqrt( 1.0f - s_z * s_z );
					if( r.Value[1][1] < 0.0f ) c_z = - c_z;
				}
				else
				{
					s_z = 0.0f;
					c_z = 1.0f;
				}
				
				::Effekseer::Vector3D r_temp = R;
				::Effekseer::Vector3D u_temp = U;

				R.X = r_temp.X * c_z + u_temp.X * s_z;
				R.Y = r_temp.Y * c_z + u_temp.Y * s_z;
				R.Z = r_temp.Z * c_z + u_temp.Z * s_z;

				U.X = u_temp.X * c_z - r_temp.X * s_z;
				U.Y = u_temp.Y * c_z - r_temp.Y * s_z;
				U.Z = u_temp.Z * c_z - r_temp.Z * s_z;
			}
			else if( parameter.Billboard == ::Effekseer::BillboardType::YAxisFixed )
			{
				U = ::Effekseer::Vector3D( r.Value[1][0], r.Value[1][1], r.Value[1][2] );

				::Effekseer::Vector3D::Normal( F, ::Effekseer::Vector3D( - camera.Values[0][2], - camera.Values[1][2], - camera.Values[2][2] ) );

				::Effekseer::Vector3D::Normal( R, ::Effekseer::Vector3D::Cross( R, U, F ) );
				::Effekseer::Vector3D::Normal( F, ::Effekseer::Vector3D::Cross( F, R, U ) );
			}
			
			::Effekseer::Matrix43 mat_rot;
			
			mat_rot.Value[0][0] = - R.X;
			mat_rot.Value[0][1] = - R.Y;
			mat_rot.Value[0][2] = - R.Z;
			mat_rot.Value[1][0] = U.X;
			mat_rot.Value[1][1] = U.Y;
			mat_rot.Value[1][2] = U.Z;
			mat_rot.Value[2][0] = F.X;
			mat_rot.Value[2][1] = F.Y;
			mat_rot.Value[2][2] = F.Z;
			mat_rot.Value[3][0] = t.X;
			mat_rot.Value[3][1] = t.Y;
			mat_rot.Value[3][2] = t.Z;

			ApplyDepthOffset(mat_rot, camera, s, parameter.DepthOffset, parameter.IsDepthOffsetScaledWithCamera, parameter.IsDepthOffsetScaledWithParticleScale, parameter.IsRightHand);
			
			if( m_instanceCount > 1 )
			{
				::Effekseer::Matrix43 mat_scale;
				mat_scale.Scaling( s.X, s.Y, s.Z );
				::Effekseer::Matrix43::Multiple( mat_rot, mat_scale, mat_rot );

				TransformVertexes( verteies, vertexCount, mat_rot );
			}
			else
			{
				::Effekseer::Matrix43 mat_scale;
				mat_scale.Scaling( s.X, s.Y, s.Z );
				::Effekseer::Matrix43::Multiple( mat_rot, mat_scale, mat_rot );

				for( int32_t i = 0; i < 4; i++ )
				{
					m_singleRenderingMatrix.Values[i][0] = mat_rot.Value[i][0];
					m_singleRenderingMatrix.Values[i][1] = mat_rot.Value[i][1];
					m_singleRenderingMatrix.Values[i][2] = mat_rot.Value[i][2];
				}
			}
		}
		else if( parameter.Billboard == ::Effekseer::BillboardType::Fixed )
		{
			auto mat = instanceParameter.SRTMatrix43;

			ApplyDepthOffset(mat, camera, parameter.DepthOffset, parameter.IsDepthOffsetScaledWithCamera, parameter.IsDepthOffsetScaledWithParticleScale, parameter.IsRightHand);

			if( m_instanceCount > 1 )
			{
				TransformVertexes(verteies, vertexCount, mat);
			}
			else
			{
				for( int32_t i = 0; i < 4; i++ )
				{
					m_singleRenderingMatrix.Values[i][0] = mat.Value[i][0];
					m_singleRenderingMatrix.Values[i][1] = mat.Value[i][1];
					m_singleRenderingMatrix.Values[i][2] = mat.Value[i][2];
				}
			}
		}

		m_spriteCount += 2 * parameter.VertexCount;
	}

	void EndRendering_(RENDERER* renderer, const efkRingNodeParam& param)
	{
		if (m_instanceCount == 1)
		{

			::Effekseer::Matrix44 mat;
			::Effekseer::Matrix44::Mul(mat, m_singleRenderingMatrix, renderer->GetCameraMatrix());

			renderer->GetStandardRenderer()->Rendering(mat, renderer->GetProjectionMatrix());
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

		if (m_spriteCount == 0) return;

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
	int32_t							m_ringBufferOffset;
	uint8_t*						m_ringBufferData;

public:

	SpriteRendererBase(RENDERER* renderer)
		: m_renderer(renderer)
		, m_spriteCount(0)
		, m_ringBufferOffset(0)
		, m_ringBufferData(NULL)
	{
	}

	virtual ~SpriteRendererBase()
	{
	}

protected:

	void BeginRendering_(RENDERER* renderer, int32_t count, const efkSpriteNodeParam& param)
	{
		EffekseerRenderer::StandardRendererState state;
		state.AlphaBlend = param.AlphaBlend;
		state.CullingType = ::Effekseer::CullingType::Double;
		state.DepthTest = param.ZTest;
		state.DepthWrite = param.ZWrite;
		state.TextureFilterType = param.TextureFilter;
		state.TextureWrapType = param.TextureWrap;

		state.Distortion = param.Distortion;
		state.DistortionIntensity = param.DistortionIntensity;

		if (param.ColorTextureIndex >= 0)
		{
			if (state.Distortion)
			{
				state.TexturePtr = param.EffectPointer->GetDistortionImage(param.ColorTextureIndex);
			}
			else
			{
				state.TexturePtr = param.EffectPointer->GetColorImage(param.ColorTextureIndex);
			}
		}
		else
		{
			state.TexturePtr = nullptr;
		}

		renderer->GetStandardRenderer()->UpdateStateAndRenderingIfRequired(state);

		renderer->GetStandardRenderer()->BeginRenderingAndRenderingIfRequired(count * 4, m_ringBufferOffset, (void*&) m_ringBufferData);
		m_spriteCount = 0;
	}

	void Rendering_(const efkSpriteNodeParam& parameter, const efkSpriteInstanceParam& instanceParameter, void* userData, const ::Effekseer::Matrix44& camera)
	{
		if (parameter.Distortion)
		{
			Rendering_Internal<VERTEX_DISTORTION>(parameter, instanceParameter, userData, camera);
		}
		else
		{
			Rendering_Internal<VERTEX_NORMAL>(parameter, instanceParameter, userData, camera);
		}
	}

	template<typename VERTEX>
	void Rendering_Internal( const efkSpriteNodeParam& parameter, const efkSpriteInstanceParam& instanceParameter, void* userData, const ::Effekseer::Matrix44& camera )
	{
		if( m_ringBufferData == NULL ) return;
		
		VERTEX* verteies = (VERTEX*)m_ringBufferData;
		m_ringBufferData += (sizeof(VERTEX) * 4);
	
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

		// 歪み処理
		if (sizeof(VERTEX) == sizeof(VERTEX_DISTORTION))
		{
			auto vs = (VERTEX_DISTORTION*) verteies;
			for (auto i = 0; i < 4; i++)
			{
				vs[i].Tangent.X = 1.0f;
				vs[i].Tangent.Y = 0.0f;
				vs[i].Tangent.Z = 0.0f;
				vs[i].Binormal.X = 1.0f;
				vs[i].Binormal.Y = 0.0f;
				vs[i].Binormal.Z = 0.0f;
			}
		}
		
		if( parameter.Billboard == ::Effekseer::BillboardType::Billboard ||
			parameter.Billboard == ::Effekseer::BillboardType::RotatedBillboard ||
			parameter.Billboard == ::Effekseer::BillboardType::YAxisFixed)
		{
			const ::Effekseer::Matrix43& mat = instanceParameter.SRTMatrix43;
			::Effekseer::Vector3D s;
			::Effekseer::Matrix43 r;
			::Effekseer::Vector3D t;
			mat.GetSRT( s, r, t );
	
			// 拡大
			for( int i = 0; i < 4; i++ )
			{
				verteies[i].Pos.X = verteies[i].Pos.X * s.X;
				verteies[i].Pos.Y = verteies[i].Pos.Y * s.Y;
			}
	
			::Effekseer::Vector3D F;
			::Effekseer::Vector3D R;
			::Effekseer::Vector3D U;
			
			
			if( parameter.Billboard == ::Effekseer::BillboardType::Billboard )
			{
				::Effekseer::Vector3D Up( 0.0f, 1.0f, 0.0f );
	
				::Effekseer::Vector3D::Normal( F, ::Effekseer::Vector3D( - camera.Values[0][2], - camera.Values[1][2], - camera.Values[2][2] ) );
	
				::Effekseer::Vector3D::Normal( R, ::Effekseer::Vector3D::Cross( R, Up, F ) );
				::Effekseer::Vector3D::Normal( U, ::Effekseer::Vector3D::Cross( U, F, R ) );
			}
			else if( parameter.Billboard == ::Effekseer::BillboardType::RotatedBillboard )
			{
				::Effekseer::Vector3D Up( 0.0f, 1.0f, 0.0f );
	
				::Effekseer::Vector3D::Normal( F, ::Effekseer::Vector3D( - camera.Values[0][2], - camera.Values[1][2], - camera.Values[2][2] ) );
	
				::Effekseer::Vector3D::Normal( R, ::Effekseer::Vector3D::Cross( R, Up, F ) );
				::Effekseer::Vector3D::Normal( U, ::Effekseer::Vector3D::Cross( U, F, R ) );
	
				float c_zx = sqrt( 1.0f - r.Value[2][1] * r.Value[2][1] );
				float s_z = 0.0f;
				float c_z = 0.0f;
	
				if( fabsf( c_zx ) > 0.05f )
				{
					s_z = - r.Value[0][1] / c_zx;
					c_z = sqrt( 1.0f - s_z * s_z );
					if( r.Value[1][1] < 0.0f ) c_z = - c_z;
				}
				else
				{
					s_z = 0.0f;
					c_z = 1.0f;
				}
				
				::Effekseer::Vector3D r_temp = R;
				::Effekseer::Vector3D u_temp = U;
	
				R.X = r_temp.X * c_z + u_temp.X * s_z;
				R.Y = r_temp.Y * c_z + u_temp.Y * s_z;
				R.Z = r_temp.Z * c_z + u_temp.Z * s_z;
	
				U.X = u_temp.X * c_z - r_temp.X * s_z;
				U.Y = u_temp.Y * c_z - r_temp.Y * s_z;
				U.Z = u_temp.Z * c_z - r_temp.Z * s_z;
			}
			else if( parameter.Billboard == ::Effekseer::BillboardType::YAxisFixed )
			{
				U = ::Effekseer::Vector3D( r.Value[1][0], r.Value[1][1], r.Value[1][2] );
	
				::Effekseer::Vector3D::Normal( F, ::Effekseer::Vector3D( - camera.Values[0][2], - camera.Values[1][2], - camera.Values[2][2] ) );
	
				::Effekseer::Vector3D::Normal( R, ::Effekseer::Vector3D::Cross( R, U, F ) );
				::Effekseer::Vector3D::Normal( F, ::Effekseer::Vector3D::Cross( F, R, U ) );
			}
			
			::Effekseer::Matrix43 mat_rot;
	
			mat_rot.Value[0][0] = - R.X;
			mat_rot.Value[0][1] = - R.Y;
			mat_rot.Value[0][2] = - R.Z;
			mat_rot.Value[1][0] = U.X;
			mat_rot.Value[1][1] = U.Y;
			mat_rot.Value[1][2] = U.Z;
			mat_rot.Value[2][0] = F.X;
			mat_rot.Value[2][1] = F.Y;
			mat_rot.Value[2][2] = F.Z;
			mat_rot.Value[3][0] = t.X;
			mat_rot.Value[3][1] = t.Y;
			mat_rot.Value[3][2] = t.Z;
	
			ApplyDepthOffset(mat_rot, camera, s, parameter.DepthOffset, parameter.IsDepthOffsetScaledWithCamera, parameter.IsDepthOffsetScaledWithParticleScale, parameter.IsRightHand);

			TransformVertexes( verteies, 4, mat_rot );
		}
		else if( parameter.Billboard == ::Effekseer::BillboardType::Fixed )
		{
			auto mat = instanceParameter.SRTMatrix43;

			ApplyDepthOffset(mat, camera, parameter.DepthOffset, parameter.IsDepthOffsetScaledWithCamera, parameter.IsDepthOffsetScaledWithParticleScale, parameter.IsRightHand);

			for( int i = 0; i < 4; i++ )
			{
				::Effekseer::Vector3D::Transform(
					verteies[i].Pos,
					verteies[i].Pos,
					mat);

				// 歪み処理
				if (sizeof(VERTEX) == sizeof(VERTEX_DISTORTION))
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
			}
		}
		
		m_spriteCount++;
	}

	void EndRendering_(RENDERER* renderer, const efkSpriteNodeParam& param)
	{
		/*
		SHADER* shader_ = NULL;
		if (param.ColorTextureIndex >= 0)
		{
			shader_ = shader;
		}
		else
		{
			shader_ = shader_no_texture;
		}

		renderer->BeginShader(shader_);

		RenderStateBase::State& state = renderer->GetRenderState()->Push();
		state.DepthTest = param.ZTest;
		state.DepthWrite = param.ZWrite;
		state.CullingType = ::Effekseer::CullingType::Double;

		if (param.ColorTextureIndex >= 0)
		{
			TEXTURE texture = TexturePointerToTexture<TEXTURE>(param.EffectPointer->GetColorImage(param.ColorTextureIndex));
			renderer->SetTextures(shader_, &texture, 1);
		}
		else
		{
			TEXTURE texture = 0;
			renderer->SetTextures(shader_, &texture, 1);
		}

		((Effekseer::Matrix44*)(shader_->GetVertexConstantBuffer()))[0] = renderer->GetCameraProjectionMatrix();
		shader_->SetConstantBuffer();

		state.AlphaBlend = param.AlphaBlend;
		state.TextureFilterTypes[0] = param.TextureFilter;
		state.TextureWrapTypes[0] = param.TextureWrap;

		renderer->GetRenderState()->Update(false);

		renderer->SetVertexBuffer(renderer->GetVertexBuffer(), sizeof(VERTEX));
		renderer->SetIndexBuffer(renderer->GetIndexBuffer());
		renderer->SetLayout(shader_);
		renderer->DrawSprites(m_spriteCount, m_ringBufferOffset / sizeof(VERTEX));

		renderer->EndShader(shader_);

		renderer->GetRenderState()->Pop();
		*/
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
		//if( m_ringBufferData == NULL ) return;
		//
		//if( m_spriteCount == 0 ) return;
		//
		//EndRendering_<RendererImplemented, Shader, GLuint, Vertex>(m_renderer, parameter);
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

	void BeginRendering_( RENDERER* renderer, const efkTrackNodeParam& param, int32_t count, void* userData )
	{
		/*
		m_ribbonCount = 0;
		
		int32_t vertexCount = (count - 1) * 8;
	
		if( ! renderer->GetVertexBuffer()->RingBufferLock( sizeof(VERTEX) * vertexCount, m_ringBufferOffset, (void*&)m_ringBufferData ) )
		{
			m_ringBufferOffset = 0;
			m_ringBufferData = NULL;
		}
		*/

		m_ribbonCount = 0;
		int32_t vertexCount = (count - 1) * 8;
		if (vertexCount <= 0) return;

		EffekseerRenderer::StandardRendererState state;
		state.AlphaBlend = param.AlphaBlend;
		state.CullingType = ::Effekseer::CullingType::Double;
		state.DepthTest = param.ZTest;
		state.DepthWrite = param.ZWrite;
		state.TextureFilterType = param.TextureFilter;
		state.TextureWrapType = param.TextureWrap;

		state.Distortion = param.Distortion;
		state.DistortionIntensity = param.DistortionIntensity;

		if (param.ColorTextureIndex >= 0)
		{
			if (state.Distortion)
			{
				state.TexturePtr = param.EffectPointer->GetDistortionImage(param.ColorTextureIndex);
			}
			else
			{
				state.TexturePtr = param.EffectPointer->GetColorImage(param.ColorTextureIndex);
			}
		}
		else
		{
			state.TexturePtr = nullptr;
		}

		renderer->GetStandardRenderer()->UpdateStateAndRenderingIfRequired(state);

		renderer->GetStandardRenderer()->BeginRenderingAndRenderingIfRequired(vertexCount, m_ringBufferOffset, (void*&) m_ringBufferData);
	}

	void Rendering_(const efkTrackNodeParam& parameter, const efkTrackInstanceParam& instanceParameter, void* userData, const ::Effekseer::Matrix44& camera)
	{
		if (parameter.Distortion)
		{
			Rendering_Internal<VERTEX_DISTORTION>(parameter, instanceParameter, userData, camera);
		}
		else
		{
			Rendering_Internal<VERTEX_NORMAL>(parameter, instanceParameter, userData, camera);
		}
	}

	template<typename VERTEX>
	void Rendering_Internal( const efkTrackNodeParam& parameter, const efkTrackInstanceParam& instanceParameter, void* userData, const ::Effekseer::Matrix44& camera )
	{
		if( m_ringBufferData == NULL ) return;
		if( instanceParameter.InstanceCount < 2 ) return;

		const efkTrackInstanceParam& param = instanceParameter;
		
		bool isFirst = param.InstanceIndex == 0;
		bool isLast = param.InstanceIndex == (param.InstanceCount - 1);

		VERTEX* verteies = (VERTEX*)m_ringBufferData;

		float size = 0.0f;
		::Effekseer::Color leftColor;
		::Effekseer::Color centerColor;
		::Effekseer::Color rightColor;

		float percent = (float) param.InstanceIndex / (float) (param.InstanceCount - 1);

		if( param.InstanceIndex < param.InstanceCount / 2 )
		{
			float l = percent;
			l = l * 2.0f;
			size = param.SizeFor + (param.SizeMiddle-param.SizeFor) * l;
			
			leftColor.R = (uint8_t)Effekseer::Clamp( param.ColorLeft.R + (param.ColorLeftMiddle.R-param.ColorLeft.R) * l, 255, 0 );
			leftColor.G = (uint8_t)Effekseer::Clamp( param.ColorLeft.G + (param.ColorLeftMiddle.G-param.ColorLeft.G) * l, 255, 0 );
			leftColor.B = (uint8_t)Effekseer::Clamp( param.ColorLeft.B + (param.ColorLeftMiddle.B-param.ColorLeft.B) * l, 255, 0 );
			leftColor.A = (uint8_t)Effekseer::Clamp( param.ColorLeft.A + (param.ColorLeftMiddle.A-param.ColorLeft.A) * l, 255, 0 );

			centerColor.R = (uint8_t)Effekseer::Clamp( param.ColorCenter.R + (param.ColorCenterMiddle.R-param.ColorCenter.R) * l, 255, 0 );
			centerColor.G = (uint8_t)Effekseer::Clamp( param.ColorCenter.G + (param.ColorCenterMiddle.G-param.ColorCenter.G) * l, 255, 0 );
			centerColor.B = (uint8_t)Effekseer::Clamp( param.ColorCenter.B + (param.ColorCenterMiddle.B-param.ColorCenter.B) * l, 255, 0 );
			centerColor.A = (uint8_t)Effekseer::Clamp( param.ColorCenter.A + (param.ColorCenterMiddle.A-param.ColorCenter.A) * l, 255, 0 );

			rightColor.R = (uint8_t)Effekseer::Clamp( param.ColorRight.R + (param.ColorRightMiddle.R-param.ColorRight.R) * l, 255, 0 );
			rightColor.G = (uint8_t)Effekseer::Clamp( param.ColorRight.G + (param.ColorRightMiddle.G-param.ColorRight.G) * l, 255, 0 );
			rightColor.B = (uint8_t)Effekseer::Clamp( param.ColorRight.B + (param.ColorRightMiddle.B-param.ColorRight.B) * l, 255, 0 );
			rightColor.A = (uint8_t)Effekseer::Clamp( param.ColorRight.A + (param.ColorRightMiddle.A-param.ColorRight.A) * l, 255, 0 );
		}
		else
		{
			float l = percent;
			l = 1.0f - (l * 2.0f - 1.0f);
			size = param.SizeBack + (param.SizeMiddle-param.SizeBack) * l;
			
			leftColor.R = (uint8_t)Effekseer::Clamp( param.ColorLeft.R + (param.ColorLeftMiddle.R-param.ColorLeft.R) * l, 255, 0 );
			leftColor.G = (uint8_t)Effekseer::Clamp( param.ColorLeft.G + (param.ColorLeftMiddle.G-param.ColorLeft.G) * l, 255, 0 );
			leftColor.B = (uint8_t)Effekseer::Clamp( param.ColorLeft.B + (param.ColorLeftMiddle.B-param.ColorLeft.B) * l, 255, 0 );
			leftColor.A = (uint8_t)Effekseer::Clamp( param.ColorLeft.A + (param.ColorLeftMiddle.A-param.ColorLeft.A) * l, 255, 0 );

			centerColor.R = (uint8_t)Effekseer::Clamp( param.ColorCenter.R + (param.ColorCenterMiddle.R-param.ColorCenter.R) * l, 255, 0 );
			centerColor.G = (uint8_t)Effekseer::Clamp( param.ColorCenter.G + (param.ColorCenterMiddle.G-param.ColorCenter.G) * l, 255, 0 );
			centerColor.B = (uint8_t)Effekseer::Clamp( param.ColorCenter.B + (param.ColorCenterMiddle.B-param.ColorCenter.B) * l, 255, 0 );
			centerColor.A = (uint8_t)Effekseer::Clamp( param.ColorCenter.A + (param.ColorCenterMiddle.A-param.ColorCenter.A) * l, 255, 0 );

			rightColor.R = (uint8_t)Effekseer::Clamp( param.ColorRight.R + (param.ColorRightMiddle.R-param.ColorRight.R) * l, 255, 0 );
			rightColor.G = (uint8_t)Effekseer::Clamp( param.ColorRight.G + (param.ColorRightMiddle.G-param.ColorRight.G) * l, 255, 0 );
			rightColor.B = (uint8_t)Effekseer::Clamp( param.ColorRight.B + (param.ColorRightMiddle.B-param.ColorRight.B) * l, 255, 0 );
			rightColor.A = (uint8_t)Effekseer::Clamp( param.ColorRight.A + (param.ColorRightMiddle.A-param.ColorRight.A) * l, 255, 0 );
		}

		const ::Effekseer::Matrix43& mat = instanceParameter.SRTMatrix43;
		::Effekseer::Vector3D s;
		::Effekseer::Matrix43 r;
		::Effekseer::Vector3D t;
		mat.GetSRT(s, r, t);

		VERTEX v[3];

		v[0].Pos.X = (- size / 2.0f) * s.X;
		v[0].Pos.Y = 0.0f;
		v[0].Pos.Z = 0.0f;
		v[0].SetColor( leftColor );

		v[1].Pos.X = 0.0f;
		v[1].Pos.Y = 0.0f;
		v[1].Pos.Z = 0.0f;
		v[1].SetColor( centerColor );

		v[2].Pos.X = (size / 2.0f) * s.X;
		v[2].Pos.Y = 0.0f;
		v[2].Pos.Z = 0.0f;
		v[2].SetColor( rightColor );

		v[0].UV[0] = instanceParameter.UV.X;
		v[0].UV[1] = instanceParameter.UV.Y + percent * instanceParameter.UV.Height;

		v[1].UV[0] = instanceParameter.UV.X + instanceParameter.UV.Width * 0.5f;
		v[1].UV[1] = instanceParameter.UV.Y + percent * instanceParameter.UV.Height;

		v[2].UV[0] = instanceParameter.UV.X + instanceParameter.UV.Width;
		v[2].UV[1] = instanceParameter.UV.Y + percent * instanceParameter.UV.Height;

		v[1].Pos.X = param.SRTMatrix43.Value[3][0];
		v[1].Pos.Y = param.SRTMatrix43.Value[3][1];
		v[1].Pos.Z = param.SRTMatrix43.Value[3][2];

		if( isFirst )
		{
			verteies[0] = v[0];
			verteies[1] = v[1];
			verteies[4] = v[1];
			verteies[5] = v[2];
			m_ringBufferData += sizeof(VERTEX) * 2;
			
		}
		else if( isLast )
		{
			verteies[0] = v[0];
			verteies[1] = v[1];
			verteies[4] = v[1];
			verteies[5] = v[2];
			m_ringBufferData += sizeof(VERTEX) * 6;
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

			m_ringBufferData += sizeof(VERTEX) * 8;
			m_ribbonCount += 2;
		}

		/* 全ての頂点の座標を変換 */
		if( isLast )
		{
			VERTEX* vs_ = (VERTEX*)(m_ringBufferData - sizeof(VERTEX) * 8 * (param.InstanceCount-1) );
			
			Effekseer::Vector3D axisBefore;

			for( int32_t i = 0; i < param.InstanceCount; i++ )
			{
				bool isFirst_ = (i == 0);
				bool isLast_ = (i == (param.InstanceCount-1));
				Effekseer::Vector3D axis;
				Effekseer::Vector3D pos;

				if( isFirst_ )
				{
					axis = (vs_[3].Pos - vs_[1].Pos);
					Effekseer::Vector3D::Normal( axis, axis );
					axisBefore = axis;
				}
				else if( isLast_ )
				{
					 axis = axisBefore;
				}
				else
				{
					Effekseer::Vector3D axisOld = axisBefore;
					axis = vs_[9].Pos - vs_[7].Pos;
					Effekseer::Vector3D::Normal( axis, axis );
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

				::Effekseer::Vector3D::Normal( F, ::Effekseer::Vector3D( - camera.Values[0][2], - camera.Values[1][2], - camera.Values[2][2] ) );

				::Effekseer::Vector3D::Normal( R, ::Effekseer::Vector3D::Cross( R, U, F ) );
				::Effekseer::Vector3D::Normal( F, ::Effekseer::Vector3D::Cross( F, R, U ) );
				
				::Effekseer::Matrix43 mat_rot;

				mat_rot.Value[0][0] = - R.X;
				mat_rot.Value[0][1] = - R.Y;
				mat_rot.Value[0][2] = - R.Z;
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
					mat_rot );

				::Effekseer::Vector3D::Transform(
					vm.Pos,
					vm.Pos,
					mat_rot );

				::Effekseer::Vector3D::Transform(
					vr.Pos,
					vr.Pos,
					mat_rot );


				if (sizeof(VERTEX) == sizeof(VERTEX_DISTORTION))
				{
					auto vl_ = (VERTEX_DISTORTION*) (&vl);
					auto vm_ = (VERTEX_DISTORTION*) (&vm);
					auto vr_ = (VERTEX_DISTORTION*) (&vr);

					vl_->Binormal = axis;
					vm_->Binormal = axis;
					vr_->Binormal = axis;

					::Effekseer::Vector3D tangent;
					::Effekseer::Vector3D::Normal(tangent, vr_->Pos - vl_->Pos);

					vl_->Tangent = tangent;
					vm_->Tangent = tangent;
					vr_->Tangent = tangent;
				}

				if( isFirst_ )
				{
					vs_[0] = vl;
					vs_[1] = vm;
					vs_[4] = vm;
					vs_[5] = vr;
					vs_ += 2;
					
				}
				else if( isLast_ )
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
	}

	void EndRendering_(RENDERER* renderer, const efkTrackNodeParam& param)
	{
		/*
		RenderStateBase::State& state = renderer->GetRenderState()->Push();
		state.DepthTest = param.ZTest;
		state.DepthWrite = param.ZWrite;
		state.CullingType = ::Effekseer::CullingType::Double;

		SHADER* shader_ = NULL;
		if (param.ColorTextureIndex >= 0)
		{
			shader_ = shader;
		}
		else
		{
			shader_ = shader_no_texture;
		}

		renderer->BeginShader(shader_);

		if (param.ColorTextureIndex >= 0)
		{
			TEXTURE texture = TexturePointerToTexture<TEXTURE>(param.EffectPointer->GetColorImage(param.ColorTextureIndex));
			renderer->SetTextures(shader_, &texture, 1);
		}
		else
		{
			TEXTURE texture = (TEXTURE)NULL;
			renderer->SetTextures(shader_, &texture, 1);
		}

		((Effekseer::Matrix44*)(shader_->GetVertexConstantBuffer()))[0] = renderer->GetCameraProjectionMatrix();
		shader_->SetConstantBuffer();


		state.AlphaBlend = param.AlphaBlend;
		state.TextureFilterTypes[0] = param.TextureFilter;
		state.TextureWrapTypes[0] = param.TextureWrap;

		renderer->GetRenderState()->Update(false);

		renderer->SetVertexBuffer(renderer->GetVertexBuffer(), sizeof(VERTEX));
		renderer->SetIndexBuffer(renderer->GetIndexBuffer());
		renderer->SetLayout(shader_);
		renderer->DrawSprites(m_ribbonCount, m_ringBufferOffset / sizeof(VERTEX));

		renderer->EndShader(shader_);

		renderer->GetRenderState()->Pop();
		*/
	}

public:

	void BeginRendering(const efkTrackNodeParam& parameter, int32_t count, void* userData) override
	{
		BeginRendering_(m_renderer, parameter, count, userData);
	}

	void Rendering(const efkTrackNodeParam& parameter, const efkTrackInstanceParam& instanceParameter, void* userData) override
	{
		Rendering_(parameter, instanceParameter, userData, m_renderer->GetCameraMatrix());
	}

	void EndRendering(const efkTrackNodeParam& parameter, void* userData) override
	{
		if (m_ringBufferData == NULL) return;

		if (m_ribbonCount <= 1) return;

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
	virtual bool RingBufferLock( int32_t size, int32_t& offset, void*& data ) = 0;

	/**
		@brief	バッファ全体を再確保せずに、試しに一部をロックしてみる。
	*/
	virtual bool TryRingBufferLock(int32_t size, int32_t& offset, void*& data) = 0;

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

#if defined(__EFFEKSEER_RENDERER_GL_GLEW__)

#if _WIN32
#include <GL/gl.h>
#else
#include <GL/glew.h>
#endif

#elif defined(__EFFEKSEER_RENDERER_GL_GLEW_S__)

#if _WIN32
#include <GL/gl.h>
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
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#else

#if defined(__APPLE__)
#include <OpenGL/gl3.h>
#else
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
		@brief	インスタンスを生成する。
		@param	squareMaxCount		最大描画スプライト数
		@param	OpenGLDeviceType	デバイスの種類
		@return	インスタンス
	*/
	static Renderer* Create(int32_t squareMaxCount, OpenGLDeviceType deviceType = OpenGLDeviceType::OpenGL2);

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

		InternalModel();

		virtual ~InternalModel();
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

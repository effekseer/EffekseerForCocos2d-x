﻿#include "EffekseerRendererNative.h"
#include <memory>
#include <set>
#include <iostream>
#ifdef _WIN32
#include <GL/glu.h>
#else
#endif
#if defined(__ANDROID__) || defined(__EMSCRIPTEN__)
#include <EGL/egl.h>
#endif

#if (_M_IX86_FP >= 2) || defined(__SSE__)
#define EFK_SSE2
#include <emmintrin.h>
#elif defined(__ARM_NEON__)
#define EFK_NEON
#include <arm_neon.h>
#endif



namespace EffekseerRenderer
{

void CalcBillboard(::Effekseer::BillboardType billboardType,
				   Effekseer::Mat43f& dst,
				   ::Effekseer::Vec3f& s,
				   ::Effekseer::Vec3f& R,
				   ::Effekseer::Vec3f& F,
				   const ::Effekseer::Mat43f& src,
				   const ::Effekseer::Vec3f& frontDirection)
{
	auto frontDir = frontDirection;

	if (billboardType == ::Effekseer::BillboardType::Billboard || billboardType == ::Effekseer::BillboardType::RotatedBillboard ||
		billboardType == ::Effekseer::BillboardType::YAxisFixed)
	{
		::Effekseer::Mat43f r;
		::Effekseer::Vec3f t;
		src.GetSRT(s, r, t);

		::Effekseer::Vec3f U;

		if (billboardType == ::Effekseer::BillboardType::Billboard)
		{
			::Effekseer::Vec3f Up(0.0f, 1.0f, 0.0f);

			F = frontDir;
			R = ::Effekseer::Vec3f::Cross(Up, F).Normalize();
			U = ::Effekseer::Vec3f::Cross(F, R).Normalize();
		}
		else if (billboardType == ::Effekseer::BillboardType::RotatedBillboard)
		{
			::Effekseer::Vec3f Up(0.0f, 1.0f, 0.0f);

			F = frontDir;
			R = ::Effekseer::Vec3f::Cross(Up, F).Normalize();
			U = ::Effekseer::Vec3f::Cross(F, R).Normalize();

			float c_zx2 = Effekseer::Vec3f::Dot(r.Y, r.Y) - r.Y.GetZ() * r.Y.GetZ();
			float c_zx = sqrt(std::max(0.0f, c_zx2));
			float s_z = 0.0f;
			float c_z = 0.0f;

			if (fabsf(c_zx) > 0.001f)
			{
				s_z = r.Y.GetX() / c_zx;
				c_z = r.Y.GetY() / c_zx;
			}
			else
			{
				s_z = 0.0f;
				c_z = 1.0f;
			}

			::Effekseer::Vec3f r_temp = R;
			::Effekseer::Vec3f u_temp = U;

			R = r_temp * c_z + u_temp * s_z;
			U = u_temp * c_z - r_temp * s_z;
		}
		else if (billboardType == ::Effekseer::BillboardType::YAxisFixed)
		{
			U = ::Effekseer::Vec3f(r.X.GetY(), r.Y.GetY(), r.Z.GetY());
			F = frontDir;
			R = ::Effekseer::Vec3f::Cross(U, F).Normalize();
			F = ::Effekseer::Vec3f::Cross(R, U).Normalize();
		}

		dst.X = {R.GetX(), U.GetX(), F.GetX(), t.GetX()};
		dst.Y = {R.GetY(), U.GetY(), F.GetY(), t.GetY()};
		dst.Z = {R.GetZ(), U.GetZ(), F.GetZ(), t.GetZ()};
	}
}

void SplineGenerator::AddVertex(const Effekseer::Vec3f& v)
{
	a.push_back(v);
	if (a.size() >= 2)
	{
		isSame.push_back(a[a.size() - 1] == a[a.size() - 2]);
	}
}

void SplineGenerator::Calculate()
{
	b.resize(a.size());
	c.resize(a.size());
	d.resize(a.size());
	w.resize(a.size());

	b[0] = b[b.size() - 1] = ::Effekseer::Vec3f(0.0f, 0.0f, 0.0f);
	c[0] = c[c.size() - 1] = ::Effekseer::Vec3f(0.0f, 0.0f, 0.0f);
	w[0] = ::Effekseer::Vec3f(0.0f, 0.0f, 0.0f);

	for (size_t i = 1; i < a.size() - 1; i++)
	{
		c[i] = (a[i - 1] + a[i] * (-2.0) + a[i + 1]) * 3.0;
	}

	for (size_t i = 1; i < a.size() - 1; i++)
	{
		auto tmp = Effekseer::Vec3f(4.0, 4.0, 4.0) - w[i - 1];
		c[i] = (c[i] - c[i - 1]) / tmp;
		w[i] = Effekseer::Vec3f(1.0, 1.0, 1.0) / tmp;
	}

	for (size_t i = (a.size() - 1) - 1; i > 0; i--)
	{
		c[i] = c[i] - c[i + 1] * w[i];
	}

	for (size_t i = 0; i < a.size() - 1; i++)
	{
		d[i] = (c[i + 1] - c[i]) / 3.0;
		b[i] = a[i + 1] - a[i] - c[i] - d[i];
	}
}

void SplineGenerator::Reset()
{
	a.clear();
	b.clear();
	c.clear();
	d.clear();
	w.clear();
	isSame.clear();
}

Effekseer::Vec3f SplineGenerator::GetValue(float t) const
{
	int32_t j = (int32_t)floorf(t);

	if (j < 0)
	{
		j = 0;
	}

	if (j > (int32_t)a.size())
	{
		j = (int32_t)a.size() - 1;
	}

	auto dt = t - j;

	if (j < (int32_t)isSame.size() && isSame[j])
		return a[j];

	return a[j] + (b[j] + (c[j] + d[j] * dt) * dt) * dt;
}

static void FastScale(::Effekseer::Mat43f& mat, float scale)
{
	float x = mat.X.GetW();
	float y = mat.Y.GetW();
	float z = mat.Z.GetW();

	mat.X *= scale;
	mat.Y *= scale;
	mat.Z *= scale;

	mat.X.SetW(x);
	mat.Y.SetW(y);
	mat.Z.SetW(z);
}

static void FastScale(::Effekseer::Mat44f& mat, float scale)
{
	float x = mat.X.GetW();
	float y = mat.Y.GetW();
	float z = mat.Z.GetW();

	mat.X *= scale;
	mat.Y *= scale;
	mat.Z *= scale;

	mat.X.SetW(x);
	mat.Y.SetW(y);
	mat.Z.SetW(z);
}

void ApplyDepthParameters(::Effekseer::Mat43f& mat,
						  const ::Effekseer::Vec3f& cameraFront,
						  const ::Effekseer::Vec3f& cameraPos,
						  ::Effekseer::NodeRendererDepthParameter* depthParameter,
						  bool isRightHand)
{
	auto depthOffset = depthParameter->DepthOffset;
	auto isDepthOffsetScaledWithCamera = depthParameter->IsDepthOffsetScaledWithCamera;
	auto isDepthOffsetScaledWithEffect = depthParameter->IsDepthOffsetScaledWithParticleScale;

	if (depthOffset != 0)
	{
		auto offset = depthOffset;

		if (isDepthOffsetScaledWithEffect)
		{
			auto scales = mat.GetScale();
			auto scale = (scales.GetX() + scales.GetY() + scales.GetZ()) / 3.0f;

			offset *= scale;
		}

		if (isDepthOffsetScaledWithCamera)
		{
			auto t = mat.GetTranslation();
			auto c = t - cameraPos;
			auto cl = c.GetLength();

			if (cl != 0.0)
			{
				auto scale = (cl - offset) / cl;
				FastScale(mat, scale);
			}
		}

		auto objPos = mat.GetTranslation();
		auto dir = cameraPos - objPos;
		dir = dir.Normalize();

		if (isRightHand)
		{
			mat.SetTranslation(mat.GetTranslation() + dir * offset);
		}
		else
		{
			mat.SetTranslation(mat.GetTranslation() + dir * offset);
		}
	}

	if (depthParameter->SuppressionOfScalingByDepth < 1.0f)
	{
		auto t = mat.GetTranslation();
		auto c = t - cameraPos;
		auto cl = c.GetLength();
		// auto cl = cameraFront.X * cx + cameraFront.Y * cy * cameraFront.Z * cz;

		if (cl != 0.0)
		{
			auto scale = cl / 32.0f * (1.0f - depthParameter->SuppressionOfScalingByDepth) + depthParameter->SuppressionOfScalingByDepth;
			FastScale(mat, scale);
		}
	}
}

void ApplyDepthParameters(::Effekseer::Mat43f& mat,
						  ::Effekseer::Vec3f& translationValues,
						  ::Effekseer::Vec3f& scaleValues,
						  const ::Effekseer::Vec3f& cameraFront,
						  const ::Effekseer::Vec3f& cameraPos,
						  ::Effekseer::NodeRendererDepthParameter* depthParameter,
						  bool isRightHand)
{
	auto depthOffset = depthParameter->DepthOffset;
	auto isDepthOffsetScaledWithCamera = depthParameter->IsDepthOffsetScaledWithCamera;
	auto isDepthOffsetScaledWithEffect = depthParameter->IsDepthOffsetScaledWithParticleScale;

	if (depthOffset != 0)
	{
		auto offset = depthOffset;

		if (isDepthOffsetScaledWithEffect)
		{
			auto scale = (scaleValues.GetX() + scaleValues.GetY() + scaleValues.GetZ()) / 3.0f;
			offset *= scale;
		}

		if (isDepthOffsetScaledWithCamera)
		{
			auto c = translationValues - cameraPos;
			auto cl = c.GetLength();

			if (cl != 0.0)
			{
				auto scale = (cl - offset) / cl;

				scaleValues *= scale;
			}
		}

		auto objPos = translationValues;
		auto dir = cameraPos - objPos;
		dir = dir.Normalize();

		if (isRightHand)
		{
			translationValues += dir * offset;
		}
		else
		{
			translationValues += dir * offset;
		}
	}

	if (depthParameter->SuppressionOfScalingByDepth < 1.0f)
	{
		auto cam2t = translationValues - cameraPos;
		auto cl = cam2t.GetLength();

		if (cl != 0.0)
		{
			auto scale = cl / 32.0f * (1.0f - depthParameter->SuppressionOfScalingByDepth) + depthParameter->SuppressionOfScalingByDepth;
			for (auto r = 0; r < 3; r++)
			{
				for (auto c = 0; c < 3; c++)
				{
					scaleValues *= scale;
				}
			}
		}
	}
}

void ApplyDepthParameters(::Effekseer::Mat43f& mat,
						  const ::Effekseer::Vec3f& cameraFront,
						  const ::Effekseer::Vec3f& cameraPos,
						  ::Effekseer::Vec3f& scaleValues,
						  ::Effekseer::NodeRendererDepthParameter* depthParameter,
						  bool isRightHand)
{
	auto depthOffset = depthParameter->DepthOffset;
	auto isDepthOffsetScaledWithCamera = depthParameter->IsDepthOffsetScaledWithCamera;
	auto isDepthOffsetScaledWithEffect = depthParameter->IsDepthOffsetScaledWithParticleScale;

	if (depthOffset != 0)
	{
		auto offset = depthOffset;

		if (isDepthOffsetScaledWithEffect)
		{
			auto scale = (scaleValues.GetX() + scaleValues.GetY() + scaleValues.GetZ()) / 3.0f;
			offset *= scale;
		}

		if (isDepthOffsetScaledWithCamera)
		{
			auto t = mat.GetTranslation();
			auto c = t - cameraPos;
			auto cl = c.GetLength();

			if (cl != 0.0)
			{
				auto scale = (cl - offset) / cl;
				FastScale(mat, scale);
			}
		}

		auto objPos = mat.GetTranslation();
		auto dir = cameraPos - objPos;
		dir = dir.Normalize();

		if (isRightHand)
		{
			mat.SetTranslation(mat.GetTranslation() + dir * offset);
		}
		else
		{
			mat.SetTranslation(mat.GetTranslation() + dir * offset);
		}
	}

	if (depthParameter->SuppressionOfScalingByDepth < 1.0f)
	{
		auto t = mat.GetTranslation();
		auto c = t - cameraPos;
		auto cl = c.GetLength();

		if (cl != 0.0)
		{
			auto scale = cl / 32.0f * (1.0f - depthParameter->SuppressionOfScalingByDepth) + depthParameter->SuppressionOfScalingByDepth;
			FastScale(mat, scale);
		}
	}
}

void ApplyDepthParameters(::Effekseer::Mat44f& mat,
						  const ::Effekseer::Vec3f& cameraFront,
						  const ::Effekseer::Vec3f& cameraPos,
						  ::Effekseer::NodeRendererDepthParameter* depthParameter,
						  bool isRightHand)
{
	auto depthOffset = depthParameter->DepthOffset;
	auto isDepthOffsetScaledWithCamera = depthParameter->IsDepthOffsetScaledWithCamera;
	auto isDepthOffsetScaledWithEffect = depthParameter->IsDepthOffsetScaledWithParticleScale;

	if (depthOffset != 0)
	{
		auto offset = depthOffset;

		if (isDepthOffsetScaledWithEffect)
		{
			auto scales = mat.GetScale();
			auto scale = (scales.GetX() + scales.GetY() + scales.GetZ()) / 3.0f;

			offset *= scale;
		}

		if (isDepthOffsetScaledWithCamera)
		{
			auto t = mat.GetTranslation();
			auto c = t - cameraPos;
			auto cl = c.GetLength();

			if (cl != 0.0)
			{
				auto scale = (cl - offset) / cl;
				FastScale(mat, scale);
			}
		}

		auto objPos = mat.GetTranslation();
		auto dir = cameraPos - objPos;
		dir = dir.Normalize();

		if (isRightHand)
		{
			mat.SetTranslation(mat.GetTranslation() + dir * offset);
		}
		else
		{
			mat.SetTranslation(mat.GetTranslation() + dir * offset);
		}
	}

	if (depthParameter->SuppressionOfScalingByDepth < 1.0f)
	{
		auto t = mat.GetTranslation();
		auto c = t - cameraPos;
		auto cl = c.GetLength();

		if (cl != 0.0)
		{
			auto scale = cl / 32.0f * (1.0f - depthParameter->SuppressionOfScalingByDepth) + depthParameter->SuppressionOfScalingByDepth;
			FastScale(mat, scale);
		}
	}
}

} // namespace EffekseerRenderer


//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
namespace EffekseerRenderer
{
//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
IndexBufferBase::IndexBufferBase(int maxCount, bool isDynamic)
	: m_indexMaxCount(maxCount)
	, m_indexCount(0)
	, m_isDynamic(false)
	, m_isLock(false)
	, m_resource(NULL)
{
}

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
IndexBufferBase::~IndexBufferBase()
{
}

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
void IndexBufferBase::Push(const void* buffer, int count)
{
	assert(m_isLock);
	memcpy(GetBufferDirect(count), buffer, count * stride_);
}

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
int IndexBufferBase::GetCount() const
{
	return m_indexCount;
}

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
int IndexBufferBase::GetMaxCount() const
{
	return m_indexMaxCount;
}

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
void* IndexBufferBase::GetBufferDirect(int count)
{
	assert(m_isLock);
	assert(m_indexMaxCount >= m_indexCount + count);

	uint8_t* pBuffer = NULL;

	pBuffer = (uint8_t*)m_resource + (m_indexCount * stride_);
	m_indexCount += count;

	return pBuffer;
}

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
} // namespace EffekseerRenderer
//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------


//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
namespace EffekseerRenderer
{

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
}
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------



namespace EffekseerRenderer
{

Renderer::Renderer()
{
	impl = new Impl();
}

Renderer::~Renderer()
{
	ES_SAFE_DELETE(impl);
}

Renderer::Impl* Renderer::GetImpl()
{
	return impl;
}

::Effekseer::Vector3D Renderer::GetLightDirection() const
{
	return impl->GetLightDirection();
}

void Renderer::SetLightDirection(const ::Effekseer::Vector3D& direction)
{
	impl->SetLightDirection(direction);
}

const ::Effekseer::Color& Renderer::GetLightColor() const
{
	return impl->GetLightColor();
}

void Renderer::SetLightColor(const ::Effekseer::Color& color)
{
	impl->SetLightColor(color);
}

const ::Effekseer::Color& Renderer::GetLightAmbientColor() const
{
	return impl->GetLightAmbientColor();
}

void Renderer::SetLightAmbientColor(const ::Effekseer::Color& color)
{
	impl->SetLightAmbientColor(color);
}

::Effekseer::Matrix44 Renderer::GetProjectionMatrix() const
{
	return impl->GetProjectionMatrix();
}

void Renderer::SetProjectionMatrix(const ::Effekseer::Matrix44& mat)
{
	impl->SetProjectionMatrix(mat);
}

::Effekseer::Matrix44 Renderer::GetCameraMatrix() const
{
	return impl->GetCameraMatrix();
}

void Renderer::SetCameraMatrix(const ::Effekseer::Matrix44& mat)
{
	impl->SetCameraMatrix(mat);
}

::Effekseer::Matrix44 Renderer::GetCameraProjectionMatrix() const
{
	return impl->GetCameraProjectionMatrix();
}

::Effekseer::Vector3D Renderer::GetCameraFrontDirection() const
{
	return impl->GetCameraFrontDirection();
}

::Effekseer::Vector3D Renderer::GetCameraPosition() const
{
	return impl->GetCameraPosition();
}

void Renderer::SetCameraParameter(const ::Effekseer::Vector3D& front, const ::Effekseer::Vector3D& position)
{
	impl->SetCameraParameter(front, position);
}

int32_t Renderer::GetDrawCallCount() const
{
	return impl->GetDrawCallCount();
}

int32_t Renderer::GetDrawVertexCount() const
{
	return impl->GetDrawVertexCount();
}

void Renderer::ResetDrawCallCount()
{
	impl->ResetDrawCallCount();
}

void Renderer::ResetDrawVertexCount()
{
	impl->ResetDrawVertexCount();
}

Effekseer::RenderMode Renderer::GetRenderMode() const
{
	return impl->GetRenderMode();
}

void Renderer::SetRenderMode(Effekseer::RenderMode renderMode)
{
	impl->SetRenderMode(renderMode);
}

UVStyle Renderer::GetTextureUVStyle() const
{
	return impl->GetTextureUVStyle();
}

void Renderer::SetTextureUVStyle(UVStyle style)
{
	impl->SetTextureUVStyle(style);
}

UVStyle Renderer::GetBackgroundTextureUVStyle() const
{
	return impl->GetBackgroundTextureUVStyle();
}

void Renderer::SetBackgroundTextureUVStyle(UVStyle style)
{
	impl->SetBackgroundTextureUVStyle(style);
}

float Renderer::GetTime() const
{
	return impl->GetTime();
}

void Renderer::SetTime(float time)
{
	impl->SetTime(time);
}

void Renderer::SetBackgroundTexture(::Effekseer::TextureData* textureData)
{
	// not implemented
	assert(0);
}

} // namespace EffekseerRenderer

namespace EffekseerRenderer
{

void Renderer::Impl::SetCameraParameterInternal(const ::Effekseer::Vec3f& front, const ::Effekseer::Vec3f& position)
{
	cameraPosition_ = position;

	// To optimize particle, cameraFontDirection_ is normalized
	const auto length = front.GetLength();
	const auto eps = 0.0001f;
	if (length > eps)
	{
		cameraFrontDirection_ = front / length;
	}
	else
	{
		std::cout << "Warning : cameraFrontDirection is too small." << std::endl;
		cameraFrontDirection_ = ::Effekseer::Vec3f{0.0f, 0.0f, 1.0f};
	}
}

::Effekseer::Vector3D Renderer::Impl::GetLightDirection() const
{
	return ToStruct(lightDirection_);
}

void Renderer::Impl::SetLightDirection(const ::Effekseer::Vector3D& direction)
{
	lightDirection_ = direction;
}

const ::Effekseer::Color& Renderer::Impl::GetLightColor() const
{
	return lightColor_;
}

void Renderer::Impl::SetLightColor(const ::Effekseer::Color& color)
{
	lightColor_ = color;
}

const ::Effekseer::Color& Renderer::Impl::GetLightAmbientColor() const
{
	return lightAmbient_;
}

void Renderer::Impl::SetLightAmbientColor(const ::Effekseer::Color& color)
{
	lightAmbient_ = color;
}

void Renderer::Impl::CalculateCameraProjectionMatrix()
{
	cameraProjMat_ = cameraMat_ * projectionMat_;
}

::Effekseer::Matrix44 Renderer::Impl::GetProjectionMatrix() const
{
	return ToStruct(projectionMat_);
}

void Renderer::Impl::SetProjectionMatrix(const ::Effekseer::Matrix44& mat)
{
	projectionMat_ = mat;
}

::Effekseer::Matrix44 Renderer::Impl::GetCameraMatrix() const
{
	return ToStruct(cameraMat_);
}

void Renderer::Impl::SetCameraMatrix(const ::Effekseer::Matrix44& mat)
{
	const auto f = ::Effekseer::Vec3f(mat.Values[0][2], mat.Values[1][2], mat.Values[2][2]);
	const auto r = ::Effekseer::Vec3f(mat.Values[0][0], mat.Values[1][0], mat.Values[2][0]);
	const auto u = ::Effekseer::Vec3f(mat.Values[0][1], mat.Values[1][1], mat.Values[2][1]);
	const auto localPos = ::Effekseer::Vec3f(-mat.Values[3][0], -mat.Values[3][1], -mat.Values[3][2]);

	const auto cameraPosition = r * localPos.GetX() + u * localPos.GetY() + f * localPos.GetZ();

	SetCameraParameterInternal(f, cameraPosition);
	cameraMat_ = mat;
}

::Effekseer::Matrix44 Renderer::Impl::GetCameraProjectionMatrix() const
{
	return ToStruct(cameraProjMat_);
}

::Effekseer::Vector3D Renderer::Impl::GetCameraFrontDirection() const
{
	return ToStruct(cameraFrontDirection_);
}

::Effekseer::Vector3D Renderer::Impl::GetCameraPosition() const
{
	return ToStruct(cameraPosition_);
}

void Renderer::Impl::SetCameraParameter(const ::Effekseer::Vector3D& front, const ::Effekseer::Vector3D& position)
{
	SetCameraParameterInternal(front, position);
}

void Renderer::Impl::CreateProxyTextures(Renderer* renderer)
{
	whiteProxyTexture_ = renderer->CreateProxyTexture(::EffekseerRenderer::ProxyTextureType::White);
	normalProxyTexture_ = renderer->CreateProxyTexture(::EffekseerRenderer::ProxyTextureType::Normal);
}

void Renderer::Impl::DeleteProxyTextures(Renderer* renderer)
{
	renderer->DeleteProxyTexture(whiteProxyTexture_);
	renderer->DeleteProxyTexture(normalProxyTexture_);
	whiteProxyTexture_ = nullptr;
	normalProxyTexture_ = nullptr;
}

::Effekseer::TextureData* Renderer::Impl::GetProxyTexture(EffekseerRenderer::ProxyTextureType type)
{
	if (type == EffekseerRenderer::ProxyTextureType::White)
		return whiteProxyTexture_;
	if (type == EffekseerRenderer::ProxyTextureType::Normal)
		return normalProxyTexture_;
	return nullptr;
}

UVStyle Renderer::Impl::GetTextureUVStyle() const
{
	return textureUVStyle;
}

void Renderer::Impl::SetTextureUVStyle(UVStyle style)
{
	textureUVStyle = style;
}

UVStyle Renderer::Impl::GetBackgroundTextureUVStyle() const
{
	return backgroundTextureUVStyle;
}

void Renderer::Impl::SetBackgroundTextureUVStyle(UVStyle style)
{
	backgroundTextureUVStyle = style;
}

int32_t Renderer::Impl::GetDrawCallCount() const
{
	return drawcallCount;
}

int32_t Renderer::Impl::GetDrawVertexCount() const
{
	return drawvertexCount;
}

void Renderer::Impl::ResetDrawCallCount()
{
	drawcallCount = 0;
}

void Renderer::Impl::ResetDrawVertexCount()
{
	drawvertexCount = 0;
}

float Renderer::Impl::GetTime() const
{
	return time_;
}

void Renderer::Impl::SetTime(float time)
{
	time_ = time;
}

Effekseer::RenderMode Renderer::Impl::GetRenderMode() const
{
	if (!isRenderModeValid)
	{
		printf("RenderMode is not implemented.\n");
		return Effekseer::RenderMode::Normal;
	}

	return renderMode_;
}

void Renderer::Impl::SetRenderMode(Effekseer::RenderMode renderMode)
{
	renderMode_ = renderMode;
}

} // namespace EffekseerRenderer

//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
namespace EffekseerRenderer
{
//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
RenderStateBase::State::State()
{
	Reset();
}

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
void RenderStateBase::State::Reset()
{
	DepthTest = false;
	DepthWrite = false;
	AlphaBlend = ::Effekseer::AlphaBlendType::Blend;
	CullingType = ::Effekseer::CullingType::Double;
	TextureFilterTypes.fill(::Effekseer::TextureFilterType::Nearest);
	TextureWrapTypes.fill(::Effekseer::TextureWrapType::Clamp);
	TextureIDs.fill(0);
}

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
void RenderStateBase::State::CopyTo(State& state)
{
	state.DepthTest = DepthTest;
	state.DepthWrite = DepthWrite;
	state.AlphaBlend = AlphaBlend;
	state.CullingType = CullingType;
	state.TextureFilterTypes = TextureFilterTypes;
	state.TextureWrapTypes = TextureWrapTypes;
	state.TextureIDs = TextureIDs;
}

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
RenderStateBase::RenderStateBase()
{
}

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
RenderStateBase::~RenderStateBase()
{
}

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
RenderStateBase::State& RenderStateBase::Push()
{
	State newState;
	m_next.CopyTo(newState);
	m_stateStack.push(m_next);
	m_next = newState;
	return m_next;
}

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
void RenderStateBase::Pop()
{
	assert(!m_stateStack.empty());

	State top = m_stateStack.top();
	m_stateStack.pop();

	m_next = top;
}

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
RenderStateBase::State& RenderStateBase::GetActiveState()
{
	return m_next;
}

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
} // namespace EffekseerRenderer
//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------


//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
namespace EffekseerRenderer
{

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
}
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------


//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
namespace EffekseerRenderer
{
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
} // namespace EffekseerRenderer
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------


//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
namespace EffekseerRenderer
{
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
} // namespace EffekseerRenderer
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------


//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
namespace EffekseerRenderer
{
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
} // namespace EffekseerRenderer
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
namespace EffekseerRenderer
{
//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
VertexBufferBase::VertexBufferBase(int size, bool isDynamic)
	: m_isDynamic(isDynamic)
	, m_size(size)
	, m_offset(0)
	, m_resource(NULL)
	, m_isLock(false)
{
}

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
VertexBufferBase::~VertexBufferBase()
{
}

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
void VertexBufferBase::Push(const void* buffer, int size)
{
	assert(m_isLock);
	memcpy(GetBufferDirect(size), buffer, size);
}

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
int VertexBufferBase::GetMaxSize() const
{
	return m_size;
}

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
int VertexBufferBase::GetSize() const
{
	return m_offset;
}

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
void* VertexBufferBase::GetBufferDirect(int size)
{
	assert(m_isLock);
	if (m_offset + size > m_size)
		return NULL;

	void* pBuffer = NULL;

	// バッファに追記
	pBuffer = m_resource + m_offset;
	m_offset += size;

	return pBuffer;
}

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
} // namespace EffekseerRenderer
  //-----------------------------------------------------------------------------------
  //
  //-----------------------------------------------------------------------------------

#ifndef __EFFEKSEERRENDERER_GL_BASE_H__
#define __EFFEKSEERRENDERER_GL_BASE_H__

//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------





#if _WIN32
#elif EMSCRIPTEN
#endif

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace EffekseerRendererGL
{
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
class RendererImplemented;

class RenderStateBase;

class DeviceObject;
class Texture;
class TargetTexture;
class DepthTexture;
class VertexBuffer;
class IndexBuffer;
class VertexArray;
class Shader;

class SpriteRenderer;
class RibbonRenderer;

class TextureLoader;

#if _WIN32
#pragma comment(lib, "glu32.lib")
#ifndef NDEBUG
#define GLCheckError()                                                                                                          \
	{                                                                                                                           \
		int __code = glGetError();                                                                                              \
		if (__code != GL_NO_ERROR)                                                                                              \
		{                                                                                                                       \
			printf("GLError filename = %s , line = %d, error = %s\n", __FILE__, __LINE__, (const char*)gluErrorString(__code)); \
		}                                                                                                                       \
	}
#else
#define GLCheckError()
#endif
#elif EMSCRIPTEN
#ifndef NDEBUG
#define GLCheckError()                                                                                                            \
	{                                                                                                                             \
		int __code = glGetError();                                                                                                \
		if (__code != GL_NO_ERROR)                                                                                                \
		{                                                                                                                         \
			EM_ASM_ARGS({ console.log("GLError filename = " + Pointer_stringify($0) + " , line = " + $1); }, __FILE__, __LINE__); \
		}                                                                                                                         \
	}
#else
#define GLCheckError()
#endif
#else
#define GLCheckError()
#endif

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
} // namespace EffekseerRendererGL
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#endif // __EFFEKSEERRENDERER_GL_BASE_H__

#ifndef __EFFEKSEERRENDERER_GL_DEVICEOBJECT_H__
#define __EFFEKSEERRENDERER_GL_DEVICEOBJECT_H__


namespace EffekseerRendererGL
{

class DeviceObjectCollection;
class RendererImplemented;

class DeviceObject
{
private:
	RendererImplemented* renderer_;
	DeviceObjectCollection* deviceObjectCollection_ = nullptr;

	//! whether does this instance inc and dec the reference count of renderer
	bool hasRefCount_ = false;

public:
	DeviceObject(RendererImplemented* renderer, DeviceObjectCollection* deviceObjectCollection, bool hasRefCount);
	virtual ~DeviceObject();

public:
	virtual void OnLostDevice() = 0;
	virtual void OnResetDevice() = 0;

	RendererImplemented* GetRenderer() const;
};

} // namespace EffekseerRendererGL

#endif // __EFFEKSEERRENDERER_GL_DEVICEOBJECT_H__

#ifndef __EFFEKSEERRENDERER_GL_GLEXTENSION_H__
#define __EFFEKSEERRENDERER_GL_GLEXTENSION_H__

//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------
#if _WIN32
#endif

#if _WIN32
#endif

#ifdef EMSCRIPTEN
#define GL_GLEXT_PROTOTYPES
#define EGL_EGLEXT_PROTOTYPES
#endif // EMSCRIPTEN


//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
namespace EffekseerRendererGL
{
namespace GLExt
{
//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------

#define GL_ELEMENT_ARRAY_BUFFER 0x8893
#define GL_STREAM_DRAW 0x88E0
#define GL_DYNAMIC_DRAW 0x88E8
#define GL_FUNC_ADD 0x8006
#define GL_MAX 0x8008
#define GL_FUNC_REVERSE_SUBTRACT 0x800B
#define GL_CLAMP_TO_EDGE 0x812F
#define GL_TEXTURE0 0x84C0
#define GL_BLEND_SRC_RGB 0x80C9
#define GL_BLEND_DST_RGB 0x80C8
#define GL_BLEND_EQUATION 0x8009
#define GL_ARRAY_BUFFER 0x8892
#define GL_STATIC_DRAW 0x88E4
#define GL_VERTEX_SHADER 0x8B31
#define GL_FRAGMENT_SHADER 0x8B30
#define GL_COMPILE_STATUS 0x8B81
#define GL_LINK_STATUS 0x8B82

#define GL_VERTEX_ARRAY_BINDING 0x85B5
#define GL_ARRAY_BUFFER_BINDING 0x8894
#define GL_ELEMENT_ARRAY_BUFFER_BINDING 0x8895

#define GL_COMPRESSED_RGBA_S3TC_DXT1_EXT 0x83F1
#define GL_COMPRESSED_RGBA_S3TC_DXT3_EXT 0x83F2
#define GL_COMPRESSED_RGBA_S3TC_DXT5_EXT 0x83F3
#define GL_SRGB8_ALPHA8 0x8C43

#define GL_FRAMEBUFFER_SRGB 0x8DB9

#define GL_MAP_READ_BIT 0x0001
#define GL_MAP_WRITE_BIT 0x0002
#define GL_MAP_INVALIDATE_RANGE_BIT 0x0004
#define GL_MAP_INVALIDATE_BUFFER_BIT 0x0008
#define GL_MAP_FLUSH_EXPLICIT_BIT 0x0010
#define GL_MAP_UNSYNCHRONIZED_BIT 0x0020

//#ifndef GL_WRITE_ONLY
#define GL_WRITE_ONLY 0x000088b9
//#endif

#if defined(__APPLE__) || defined(__ANDROID__)
#else
typedef ptrdiff_t GLsizeiptr;
typedef ptrdiff_t GLintptr;
typedef char GLchar;
#endif

OpenGLDeviceType GetDeviceType();
bool Initialize(OpenGLDeviceType deviceType);
bool IsSupportedVertexArray();
bool IsSupportedBufferRange();
bool IsSupportedMapBuffer();

//! for some devices to avoid a bug
void MakeMapBufferInvalid();

void glDeleteBuffers(GLsizei n, const GLuint* buffers);
GLuint glCreateShader(GLenum type);
void glBindBuffer(GLenum target, GLuint buffer);
void glGenBuffers(GLsizei n, GLuint* buffers);
void glBufferData(GLenum target, GLsizeiptr size, const GLvoid* data, GLenum usage);
void glBlendEquationSeparate(GLenum modeRGB, GLenum modeAlpha);
void glBlendFuncSeparate(GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha);
void glBlendEquation(GLenum mode);
void glActiveTexture(GLenum texture);
void glUniform1i(GLint location, GLint v0);
void glShaderSource(GLuint shader, GLsizei count, const GLchar** strings, const GLint* lengths);
void glCompileShader(GLuint shader);
void glGetShaderiv(GLuint shader, GLenum pname, GLint* param);
GLuint glCreateProgram(void);
void glAttachShader(GLuint program, GLuint shader);
void glDeleteProgram(GLuint program);
void glDeleteShader(GLuint shader);
void glLinkProgram(GLuint program);
void glGetProgramiv(GLuint program, GLenum pname, GLint* param);
void glGetShaderInfoLog(GLuint shader, GLsizei bufSize, GLsizei* length, GLchar* infoLog);
void glGetProgramInfoLog(GLuint program, GLsizei bufSize, GLsizei* length, GLchar* infoLog);
GLint glGetAttribLocation(GLuint program, const GLchar* name);
GLint glGetUniformLocation(GLuint program, const GLchar* name);
void glUseProgram(GLuint program);
void glEnableVertexAttribArray(GLuint index);
void glDisableVertexAttribArray(GLuint index);
void glVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* pointer);
void glUniformMatrix4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
void glUniform4fv(GLint location, GLsizei count, const GLfloat* value);
void glGenerateMipmap(GLenum target);
void glBufferSubData(GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid* data);

void glGenVertexArrays(GLsizei n, GLuint* arrays);
void glDeleteVertexArrays(GLsizei n, const GLuint* arrays);
void glBindVertexArray(GLuint array);

void glGenSamplers(GLsizei n, GLuint* samplers);
void glDeleteSamplers(GLsizei n, const GLuint* samplers);
void glSamplerParameteri(GLuint sampler, GLenum pname, GLint param);
void glBindSampler(GLuint unit, GLuint sampler);

void* glMapBuffer(GLenum target, GLenum access);
void* glMapBufferRange(GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access);
GLboolean glUnmapBuffer(GLenum target);

void glCompressedTexImage2D(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void* data);

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
} // namespace GLExt
} // namespace EffekseerRendererGL
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#endif // __EFFEKSEERRENDERER_GL_GLEXTENSION_H__




//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
namespace EffekseerRendererGL
{
//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
class IndexBuffer
	: public DeviceObject,
	  public ::EffekseerRenderer::IndexBufferBase
{
private:
	GLuint m_buffer;

	IndexBuffer(RendererImplemented* renderer, GLuint buffer, int maxCount, bool isDynamic, int32_t stride, bool hasRefCount);

public:
	virtual ~IndexBuffer();

	static IndexBuffer* Create(RendererImplemented* renderer, int maxCount, bool isDynamic, int32_t stride, bool hasRefCount);

	GLuint GetInterface()
	{
		return m_buffer;
	}

public: // デバイス復旧用
	virtual void OnLostDevice() override;
	virtual void OnResetDevice() override;

public:
	void Lock() override;
	void Unlock() override;

	bool IsValid();

	int32_t GetStride() const
	{
		return stride_;
	}
};

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
} // namespace EffekseerRendererGL
//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------


#ifndef __EFFEKSEERRENDERER_GL_MODEL_RENDERER_H__
#define __EFFEKSEERRENDERER_GL_MODEL_RENDERER_H__

//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
//#define MODEL_SOFTWARE_INSTANCING

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
namespace EffekseerRendererGL
{
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
typedef ::Effekseer::ModelRenderer::NodeParameter efkModelNodeParam;
typedef ::Effekseer::ModelRenderer::InstanceParameter efkModelInstanceParam;
typedef ::Effekseer::Vector3D efkVector3D;

class ModelRenderer
	: public ::EffekseerRenderer::ModelRendererBase
{
public:
	enum eShaderAttrib
	{
		AttribPosition,
		AttribNormal,
		AttribBinormal,
		AttribTangent,
		AttribTexCoord,
		AttribColor,
#if defined(MODEL_SOFTWARE_INSTANCING)
		AttribInstanceID,
		AttribUVOffset,
		AttribModelColor,
#endif
		NumAttribs,
		NumVertexAttribs = AttribTexCoord + 1,
	};
	enum eShaderUniform
	{
		UniformProjectionMatrix,
		UniformModelMatrix,
#if !defined(MODEL_SOFTWARE_INSTANCING)
		UniformUVOffset,
		UniformModelColor,
#endif
		UniformColorTexture,
		UniformNormalTexture,
		UniformLightDirection,
		UniformLightColor,
		UniformLightAmbient,
		Scale,
		UVInversed,
		UVInversedBack,
		NumUniforms,
	};

	static const int32_t MaxInstanced = 20;

private:
	RendererImplemented* m_renderer;

	Shader* m_shader_lighting_texture_normal;
	Shader* m_shader_texture;
	Shader* m_shader_distortion_texture;

	GLint m_uniformLoc[8][NumUniforms];

	VertexArray* m_va[8];

	ModelRenderer(
		RendererImplemented* renderer,
		Shader* shader_lighting_texture_normal,
		Shader* shader_texture,
		Shader* shader_distortion_texture);

public:
	virtual ~ModelRenderer();

	static ModelRenderer* Create(RendererImplemented* renderer);

public:
	void BeginRendering(const efkModelNodeParam& parameter, int32_t count, void* userData) override;

	virtual void Rendering(const efkModelNodeParam& parameter, const InstanceParameter& instanceParameter, void* userData) override;

	void EndRendering(const efkModelNodeParam& parameter, void* userData) override;
};
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
} // namespace EffekseerRendererGL
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#endif // __EFFEKSEERRENDERER_GL_MODEL_RENDERER_H__

#ifndef __EFFEKSEERRENDERER_GL_RENDERER_IMPLEMENTED_H__
#define __EFFEKSEERRENDERER_GL_RENDERER_IMPLEMENTED_H__

//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------

namespace EffekseerRendererGL
{

using Vertex = EffekseerRenderer::SimpleVertex;
using VertexDistortion = EffekseerRenderer::VertexDistortion;

struct RenderStateSet
{
	GLboolean blend;
	GLboolean cullFace;
	GLboolean depthTest;
	GLboolean depthWrite;
	GLboolean texture;
	GLint depthFunc;
	GLint cullFaceMode;
	GLint blendSrc;
	GLint blendDst;
	GLint blendEquation;
	GLint vao;
	GLint arrayBufferBinding;
	GLint elementArrayBufferBinding;
	std::array<GLint, ::Effekseer::TextureSlotMax> boundTextures;
};

/**
	@brief	描画クラス
	@note
	ツール向けの描画機能。
*/
class RendererImplemented
	: public Renderer,
	  public ::Effekseer::ReferenceObject
{
	friend class DeviceObject;

private:
	DeviceObjectCollection* deviceObjectCollection_ = nullptr;

	VertexBuffer* m_vertexBuffer;
	IndexBuffer* m_indexBuffer = nullptr;
	IndexBuffer* m_indexBufferForWireframe = nullptr;
	int32_t m_squareMaxCount;

	Shader* m_shader = nullptr;
	Shader* m_shader_distortion = nullptr;
	Shader* m_shader_lighting = nullptr;
	Shader* currentShader = nullptr;

	EffekseerRenderer::StandardRenderer<RendererImplemented, Shader, Vertex, VertexDistortion>* m_standardRenderer;

	VertexArray* m_vao = nullptr;
	VertexArray* m_vao_distortion = nullptr;
	VertexArray* m_vao_lighting = nullptr;
	VertexArray* m_vao_wire_frame = nullptr;

	//! default vao (alsmot for material)
	GLuint defaultVertexArray_ = 0;

	::EffekseerRenderer::RenderStateBase* m_renderState;

	Effekseer::TextureData m_background;

	OpenGLDeviceType m_deviceType;

	// for restoring states
	RenderStateSet m_originalState;

	bool m_restorationOfStates;

	EffekseerRenderer::DistortingCallback* m_distortingCallback;

	// textures which are specified currently
	std::vector<::Effekseer::TextureData> currentTextures_;

	VertexArray* m_currentVertexArray;

	int32_t indexBufferStride_ = 2;

	int32_t indexBufferCurrentStride_ = 0;

	//! because gleDrawElements has only index offset
	int32_t GetIndexSpriteCount() const;

public:
	RendererImplemented(int32_t squareMaxCount, OpenGLDeviceType deviceType, DeviceObjectCollection* deviceObjectCollection);

	~RendererImplemented();

	void OnLostDevice() override;
	void OnResetDevice() override;

	bool Initialize();

	void Destroy() override;

	void SetRestorationOfStatesFlag(bool flag) override;

	/**
		@brief	描画開始
	*/
	bool BeginRendering() override;

	/**
		@brief	描画終了
	*/
	bool EndRendering() override;

	/**
		@brief	頂点バッファ取得
	*/
	VertexBuffer* GetVertexBuffer();

	/**
		@brief	インデックスバッファ取得
	*/
	IndexBuffer* GetIndexBuffer();

	/**
		@brief	最大描画スプライト数
	*/
	int32_t GetSquareMaxCount() const override;

	void SetSquareMaxCount(int32_t count) override;

	::EffekseerRenderer::RenderStateBase* GetRenderState();

	/**
		@brief	スプライトレンダラーを生成する。
	*/
	::Effekseer::SpriteRenderer* CreateSpriteRenderer() override;

	/**
		@brief	リボンレンダラーを生成する。
	*/
	::Effekseer::RibbonRenderer* CreateRibbonRenderer() override;

	/**
		@brief	リングレンダラーを生成する。
	*/
	::Effekseer::RingRenderer* CreateRingRenderer() override;

	/**
		@brief	モデルレンダラーを生成する。
	*/
	::Effekseer::ModelRenderer* CreateModelRenderer() override;

	/**
		@brief	軌跡レンダラーを生成する。
	*/
	::Effekseer::TrackRenderer* CreateTrackRenderer() override;

	/**
		@brief	テクスチャ読込クラスを生成する。
	*/
	::Effekseer::TextureLoader* CreateTextureLoader(::Effekseer::FileInterface* fileInterface = NULL) override;

	/**
		@brief	モデル読込クラスを生成する。
	*/
	::Effekseer::ModelLoader* CreateModelLoader(::Effekseer::FileInterface* fileInterface = NULL) override;

	::Effekseer::MaterialLoader* CreateMaterialLoader(::Effekseer::FileInterface* fileInterface = nullptr) override;

	/**
	@brief	背景を取得する。
	*/
	Effekseer::TextureData* GetBackground() override
	{
		if (m_background.UserID == 0)
			return nullptr;
		return &m_background;
	}

	void SetBackground(GLuint background, bool hasMipmap) override;

	void SetBackgroundTexture(::Effekseer::TextureData* textureData) override;

	EffekseerRenderer::DistortingCallback* GetDistortingCallback() override;

	void SetDistortingCallback(EffekseerRenderer::DistortingCallback* callback) override;

	EffekseerRenderer::StandardRenderer<RendererImplemented, Shader, Vertex, VertexDistortion>*
	GetStandardRenderer()
	{
		return m_standardRenderer;
	}

	void SetVertexBuffer(VertexBuffer* vertexBuffer, int32_t size);
	void SetVertexBuffer(GLuint vertexBuffer, int32_t size);
	void SetIndexBuffer(IndexBuffer* indexBuffer);
	void SetIndexBuffer(GLuint indexBuffer);
	void SetVertexArray(VertexArray* vertexArray);

	void SetLayout(Shader* shader);
	void DrawSprites(int32_t spriteCount, int32_t vertexOffset);
	void DrawPolygon(int32_t vertexCount, int32_t indexCount);

	Shader* GetShader(bool useTexture, ::Effekseer::RendererMaterialType materialType) const;
	void BeginShader(Shader* shader);
	void EndShader(Shader* shader);

	void SetVertexBufferToShader(const void* data, int32_t size, int32_t dstOffset);

	void SetPixelBufferToShader(const void* data, int32_t size, int32_t dstOffset);

	void SetTextures(Shader* shader, Effekseer::TextureData** textures, int32_t count);

	void ResetRenderState() override;

	Effekseer::TextureData* CreateProxyTexture(EffekseerRenderer::ProxyTextureType type) override;

	void DeleteProxyTexture(Effekseer::TextureData* data) override;

	const std::vector<::Effekseer::TextureData>& GetCurrentTextures() const
	{
		return currentTextures_;
	}

	OpenGLDeviceType GetDeviceType() const override
	{
		return m_deviceType;
	}

	bool IsVertexArrayObjectSupported() const override;

	DeviceObjectCollection* GetDeviceObjectCollection() const
	{
		return deviceObjectCollection_;
	}

	virtual int GetRef() override
	{
		return ::Effekseer::ReferenceObject::GetRef();
	}
	virtual int AddRef() override
	{
		return ::Effekseer::ReferenceObject::AddRef();
	}
	virtual int Release() override
	{
		return ::Effekseer::ReferenceObject::Release();
	}

private:
	void GenerateIndexData();

	template <typename T>
	void GenerateIndexDataStride();
};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
} // namespace EffekseerRendererGL
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#endif // __EFFEKSEERRENDERER_GL_RENDERER_IMPLEMENTED_H__


#ifndef __EFFEKSEERRENDERER_GL_RENDERSTATE_H__
#define __EFFEKSEERRENDERER_GL_RENDERSTATE_H__

//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
namespace EffekseerRendererGL
{
//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
class RenderState
	: public ::EffekseerRenderer::RenderStateBase
{
private:
	RendererImplemented* m_renderer;
	bool m_isCCW = true;

	std::array<GLuint, Effekseer::TextureSlotMax> m_samplers;

public:
	RenderState(RendererImplemented* renderer);
	virtual ~RenderState();

	void Update(bool forced);
};

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
} // namespace EffekseerRendererGL
//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
#endif // __EFFEKSEERRENDERER_GL_RENDERSTATE_H__

#ifndef __EFFEKSEERRENDERER_GL_SHADER_H__
#define __EFFEKSEERRENDERER_GL_SHADER_H__

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

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
struct ShaderAttribInfo
{
	const char* name;
	GLenum type;
	uint16_t count;
	uint16_t offset;
	bool normalized;
};

struct ShaderUniformInfo
{
	const char* name;
};

enum eConstantType
{
	CONSTANT_TYPE_MATRIX44 = 0,
	CONSTANT_TYPE_MATRIX44_ARRAY = 10,
	CONSTANT_TYPE_MATRIX44_ARRAY_END = 99,

	CONSTANT_TYPE_VECTOR4 = 100,
	CONSTANT_TYPE_VECTOR4_ARRAY = 110,
	CONSTANT_TYPE_VECTOR4_ARRAY_END = 199,
};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
class Shader
	: public DeviceObject,
	  public ::EffekseerRenderer::ShaderBase
{
private:
	struct Layout
	{
		GLenum type;
		uint16_t count;
		uint16_t offset;
		bool normalized;
	};

	struct ShaderAttribInfoInternal
	{
		std::string name;
		GLenum type;
		uint16_t count;
		uint16_t offset;
		bool normalized;
	};

	struct ShaderUniformInfoInternal
	{
		std::string name;
	};

	struct ConstantLayout
	{
		eConstantType Type;
		GLint ID;
		int32_t Offset;
	};

	OpenGLDeviceType deviceType_;
	GLuint m_program;

	std::vector<GLint> m_aid;
	std::vector<Layout> m_layout;

	int32_t m_vertexSize;

	uint8_t* m_vertexConstantBuffer;
	uint8_t* m_pixelConstantBuffer;

	std::vector<ConstantLayout> m_vertexConstantLayout;
	std::vector<ConstantLayout> m_pixelConstantLayout;

	std::array<GLuint, Effekseer::TextureSlotMax> m_textureSlots;
	std::array<bool, Effekseer::TextureSlotMax> m_textureSlotEnables;

	std::vector<char> m_vsSrc;
	std::vector<char> m_psSrc;
	std::string m_name;

	std::vector<ShaderAttribInfoInternal> attribs;
	std::vector<ShaderUniformInfoInternal> uniforms;

	static bool CompileShader(
		OpenGLDeviceType deviceType,
		GLuint& program,
		const char* vs_src,
		size_t vertexShaderSize,
		const char* fs_src,
		size_t pixelShaderSize,
		const char* name);

	Shader(OpenGLDeviceType deviceType,
		   Renderer* renderer,
		   DeviceObjectCollection* deviceObjectCollection,
		   GLuint program,
		   const char* vs_src,
		   size_t vertexShaderSize,
		   const char* fs_src,
		   size_t pixelShaderSize,
		   const char* name,
		   bool hasRefCount);

	GLint GetAttribId(const char* name) const;

public:
	GLint GetUniformId(const char* name) const;

public:
	virtual ~Shader();

	static Shader* Create(
		OpenGLDeviceType deviceType,
		DeviceObjectCollection* deviceObjectCollection,
		const char* vs_src,
		size_t vertexShaderSize,
		const char* fs_src,
		size_t pixelShaderSize,
		const char* name,
		bool hasRefCount);

	static Shader* Create(
		Renderer* renderer,
		const char* vs_src,
		size_t vertexShaderSize,
		const char* fs_src,
		size_t pixelShaderSize,
		const char* name);

public:
	virtual void OnLostDevice() override;
	virtual void OnResetDevice() override;
	virtual void OnChangeDevice();

public:
	GLuint GetInterface() const;

	void GetAttribIdList(int count, const ShaderAttribInfo* info);
	void GetUniformIdList(int count, const ShaderUniformInfo* info, GLint* uid_list) const;

	void BeginScene();
	void EndScene();
	void EnableAttribs();
	void DisableAttribs();
	void SetVertex();

	void SetVertexSize(int32_t vertexSize);

	void SetVertexConstantBufferSize(int32_t size) override;
	void SetPixelConstantBufferSize(int32_t size) override;

	void* GetVertexConstantBuffer() override
	{
		return m_vertexConstantBuffer;
	}
	void* GetPixelConstantBuffer() override
	{
		return m_pixelConstantBuffer;
	}

	void AddVertexConstantLayout(eConstantType type, GLint id, int32_t offset);
	void AddPixelConstantLayout(eConstantType type, GLint id, int32_t offset);

	void SetConstantBuffer() override;

	void SetTextureSlot(int32_t index, GLuint value);
	GLuint GetTextureSlot(int32_t index);
	bool GetTextureSlotEnable(int32_t index);

	bool IsValid() const;
};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
} // namespace EffekseerRendererGL

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#endif // __EFFEKSEERRENDERER_GL_SHADER_H__


#ifdef __EFFEKSEER_RENDERER_INTERNAL_LOADER__

#ifndef __EFFEKSEERRENDERER_GL_TEXTURELOADER_H__
#define __EFFEKSEERRENDERER_GL_TEXTURELOADER_H__

//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------

#ifdef __EFFEKSEER_RENDERER_INTERNAL_LOADER__
#endif

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
namespace EffekseerRendererGL
{
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
class TextureLoader
	: public ::Effekseer::TextureLoader
{
private:
	::Effekseer::FileInterface* m_fileInterface;
	::Effekseer::DefaultFileInterface m_defaultFileInterface;
	::Effekseer::ColorSpaceType colorSpaceType_;

#ifdef __EFFEKSEER_RENDERER_INTERNAL_LOADER__
	::EffekseerRenderer::PngTextureLoader pngTextureLoader;
	::EffekseerRenderer::DDSTextureLoader ddsTextureLoader;
#endif

public:
	TextureLoader(::Effekseer::FileInterface* fileInterface = nullptr,
				  ::Effekseer::ColorSpaceType colorSpaceType = ::Effekseer::ColorSpaceType::Gamma);
	virtual ~TextureLoader();

public:
	Effekseer::TextureData* Load(const EFK_CHAR* path, ::Effekseer::TextureType textureType) override;

	Effekseer::TextureData* Load(const void* data, int32_t size, Effekseer::TextureType textureType) override;

	void Unload(Effekseer::TextureData* data) override;
};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
} // namespace EffekseerRendererGL
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#endif // __EFFEKSEERRENDERER_GL_TEXTURELOADER_H__

#endif



//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
namespace EffekseerRendererGL
{
//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
class VertexArray
	: public DeviceObject
{
private:
	GLuint m_vertexArray;

	Shader* m_shader;
	VertexBuffer* m_vertexBuffer;
	IndexBuffer* m_indexBuffer;

	VertexArray(RendererImplemented* renderer, Shader* shader, VertexBuffer* vertexBuffer, IndexBuffer* indexBuffer, bool hasRefCount);

public:
	virtual ~VertexArray();

	static VertexArray*
	Create(RendererImplemented* renderer, Shader* shader, VertexBuffer* vertexBuffer, IndexBuffer* indexBuffer, bool hasRefCount);

	GLuint GetInterface() const
	{
		return m_vertexArray;
	}

	VertexBuffer* GetVertexBuffer()
	{
		return m_vertexBuffer;
	}
	IndexBuffer* GetIndexBuffer()
	{
		return m_indexBuffer;
	}

public: // デバイス復旧用
	virtual void OnLostDevice();
	virtual void OnResetDevice();

private:
	void Init();
	void Release();
};

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
} // namespace EffekseerRendererGL
  //-----------------------------------------------------------------------------------
  //
  //-----------------------------------------------------------------------------------



//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
namespace EffekseerRendererGL
{
//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
class VertexBuffer
	: public DeviceObject,
	  public ::EffekseerRenderer::VertexBufferBase
{
private:
	GLuint m_buffer;

	uint32_t m_vertexRingStart;
	uint32_t m_vertexRingOffset;
	bool m_ringBufferLock;

	VertexBuffer(RendererImplemented* renderer, int size, bool isDynamic, bool hasRefCount);

public:
	virtual ~VertexBuffer();

	static VertexBuffer* Create(RendererImplemented* renderer, int size, bool isDynamic, bool hasRefCount);

	GLuint GetInterface();

public: // デバイス復旧用
	virtual void OnLostDevice();
	virtual void OnResetDevice();

public:
	void Lock();
	bool RingBufferLock(int32_t size, int32_t& offset, void*& data, int32_t alignment) override;
	bool TryRingBufferLock(int32_t size, int32_t& offset, void*& data, int32_t alignment) override;

	void Unlock();

	bool IsValid();
};

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
} // namespace EffekseerRendererGL
  //-----------------------------------------------------------------------------------
  //
  //-----------------------------------------------------------------------------------

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



namespace EffekseerRendererGL
{

DeviceObject::DeviceObject(RendererImplemented* renderer, DeviceObjectCollection* deviceObjectCollection, bool hasRefCount)
	: renderer_(renderer)
	, deviceObjectCollection_(deviceObjectCollection)
	, hasRefCount_(hasRefCount)
{
	if (hasRefCount_)
	{
		ES_SAFE_ADDREF(renderer_);
		ES_SAFE_ADDREF(deviceObjectCollection_);
	}

	deviceObjectCollection_->Register(this);
}

DeviceObject::~DeviceObject()
{
	deviceObjectCollection_->Unregister(this);

	if (hasRefCount_)
	{
		ES_SAFE_RELEASE(renderer_);
		ES_SAFE_RELEASE(deviceObjectCollection_);
	}
}

RendererImplemented* DeviceObject::GetRenderer() const
{
	return renderer_;
}

} // namespace EffekseerRendererGL


namespace EffekseerRendererGL
{

void DeviceObjectCollection::Register(DeviceObject* device)
{
	deviceObjects_.insert(device);
}

void DeviceObjectCollection::Unregister(DeviceObject* device)
{
	deviceObjects_.erase(device);
}

void DeviceObjectCollection::OnLostDevice()
{
	for (auto& device : deviceObjects_)
	{
		device->OnLostDevice();
	}
}

void DeviceObjectCollection::OnResetDevice()
{
	for (auto& device : deviceObjects_)
	{
		device->OnResetDevice();
	}
}

} // namespace EffekseerRendererGL


//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------

#if defined(__ANDROID__) || defined(__EMSCRIPTEN__)
#endif

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
namespace EffekseerRendererGL
{
namespace GLExt
{
//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
#if _WIN32

typedef void(EFK_STDCALL* FP_glDeleteBuffers)(GLsizei n, const GLuint* buffers);
typedef GLuint(EFK_STDCALL* FP_glCreateShader)(GLenum type);
typedef void(EFK_STDCALL* FP_glBindBuffer)(GLenum target, GLuint buffer);
typedef void(EFK_STDCALL* FP_glGenBuffers)(GLsizei n, GLuint* buffers);
typedef void(EFK_STDCALL* FP_glBufferData)(GLenum target, GLsizeiptr size, const GLvoid* data, GLenum usage);

typedef void(EFK_STDCALL* FP_glBlendEquationSeparate)(GLenum modeRGB, GLenum modeAlpha);
typedef void(EFK_STDCALL* FP_glBlendFuncSeparate)(GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha);

typedef void(EFK_STDCALL* FP_glBlendEquation)(GLenum mode);

typedef void(EFK_STDCALL* FP_glActiveTexture)(GLenum texture);

typedef void(EFK_STDCALL* FP_glUniform1i)(GLint location, GLint v0);

typedef void(EFK_STDCALL* FP_glShaderSource)(GLuint shader, GLsizei count, const GLchar** strings, const GLint* lengths);

typedef void(EFK_STDCALL* FP_glCompileShader)(GLuint shader);

typedef void(EFK_STDCALL* FP_glGetShaderiv)(GLuint shader, GLenum pname, GLint* param);

typedef GLuint(EFK_STDCALL* FP_glCreateProgram)(void);

typedef void(EFK_STDCALL* FP_glAttachShader)(GLuint program, GLuint shader);

typedef void(EFK_STDCALL* FP_glDeleteProgram)(GLuint program);
typedef void(EFK_STDCALL* FP_glDeleteShader)(GLuint shader);
typedef void(EFK_STDCALL* FP_glLinkProgram)(GLuint program);

typedef void(EFK_STDCALL* FP_glGetProgramiv)(GLuint program, GLenum pname, GLint* param);

typedef void(EFK_STDCALL* FP_glGetShaderInfoLog)(GLuint shader, GLsizei bufSize, GLsizei* length, GLchar* infoLog);
typedef void(EFK_STDCALL* FP_glGetProgramInfoLog)(GLuint program, GLsizei bufSize, GLsizei* length, GLchar* infoLog);

typedef GLint(EFK_STDCALL* FP_glGetAttribLocation)(GLuint program, const GLchar* name);

typedef GLint(EFK_STDCALL* FP_glGetUniformLocation)(GLuint program, const GLchar* name);

typedef void(EFK_STDCALL* FP_glUseProgram)(GLuint program);

typedef void(EFK_STDCALL* FP_glEnableVertexAttribArray)(GLuint index);
typedef void(EFK_STDCALL* FP_glDisableVertexAttribArray)(GLuint index);
typedef void(EFK_STDCALL* FP_glVertexAttribPointer)(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* pointer);
typedef void(EFK_STDCALL* FP_glUniformMatrix4fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void(EFK_STDCALL* FP_glUniform4fv)(GLint location, GLsizei count, const GLfloat* value);
typedef void(EFK_STDCALL* FP_glGenerateMipmap)(GLenum target);
typedef void(EFK_STDCALL* FP_glBufferSubData)(GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid* data);

typedef void(EFK_STDCALL* FP_glGenVertexArrays)(GLsizei n, GLuint* arrays);
typedef void(EFK_STDCALL* FP_glDeleteVertexArrays)(GLsizei n, const GLuint* arrays);
typedef void(EFK_STDCALL* FP_glBindVertexArray)(GLuint array);

typedef void(EFK_STDCALL* FP_glGenSamplers)(GLsizei n, GLuint* samplers);
typedef void(EFK_STDCALL* FP_glDeleteSamplers)(GLsizei n, const GLuint* samplers);
typedef void(EFK_STDCALL* FP_glSamplerParameteri)(GLuint sampler, GLenum pname, GLint param);
typedef void(EFK_STDCALL* FP_glBindSampler)(GLuint unit, GLuint sampler);

typedef void*(EFK_STDCALL* FP_glMapBuffer)(GLenum target, GLenum access);
typedef void*(EFK_STDCALL* FP_glMapBufferRange)(GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access);
typedef GLboolean(EFK_STDCALL* FP_glUnmapBuffer)(GLenum target);

typedef void(EFK_STDCALL* FP_glCompressedTexImage2D)(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void* data);

static FP_glDeleteBuffers g_glDeleteBuffers = NULL;
static FP_glCreateShader g_glCreateShader = NULL;
static FP_glBindBuffer g_glBindBuffer = NULL;
static FP_glGenBuffers g_glGenBuffers = NULL;
static FP_glBufferData g_glBufferData = NULL;
static FP_glBlendEquationSeparate g_glBlendEquationSeparate = NULL;
static FP_glBlendFuncSeparate g_glBlendFuncSeparate = NULL;
static FP_glBlendEquation g_glBlendEquation = NULL;
static FP_glActiveTexture g_glActiveTexture = NULL;
static FP_glUniform1i g_glUniform1i = NULL;
static FP_glShaderSource g_glShaderSource = NULL;
static FP_glCompileShader g_glCompileShader = NULL;
static FP_glGetShaderiv g_glGetShaderiv = NULL;
static FP_glCreateProgram g_glCreateProgram = NULL;
static FP_glAttachShader g_glAttachShader = NULL;
static FP_glDeleteProgram g_glDeleteProgram = NULL;
static FP_glDeleteShader g_glDeleteShader = NULL;
static FP_glLinkProgram g_glLinkProgram = NULL;
static FP_glGetProgramiv g_glGetProgramiv = NULL;
static FP_glGetShaderInfoLog g_glGetShaderInfoLog = NULL;
static FP_glGetProgramInfoLog g_glGetProgramInfoLog = NULL;
static FP_glGetAttribLocation g_glGetAttribLocation = NULL;
static FP_glGetUniformLocation g_glGetUniformLocation = NULL;
static FP_glUseProgram g_glUseProgram = NULL;
static FP_glEnableVertexAttribArray g_glEnableVertexAttribArray = NULL;
static FP_glDisableVertexAttribArray g_glDisableVertexAttribArray = NULL;
static FP_glVertexAttribPointer g_glVertexAttribPointer = NULL;
static FP_glUniformMatrix4fv g_glUniformMatrix4fv = NULL;
static FP_glUniform4fv g_glUniform4fv = NULL;
static FP_glGenerateMipmap g_glGenerateMipmap = NULL;
static FP_glBufferSubData g_glBufferSubData = NULL;
static FP_glGenVertexArrays g_glGenVertexArrays = NULL;
static FP_glDeleteVertexArrays g_glDeleteVertexArrays = NULL;
static FP_glBindVertexArray g_glBindVertexArray = NULL;
static FP_glGenSamplers g_glGenSamplers = nullptr;
static FP_glDeleteSamplers g_glDeleteSamplers = nullptr;
static FP_glSamplerParameteri g_glSamplerParameteri = nullptr;
static FP_glBindSampler g_glBindSampler = nullptr;

static FP_glMapBuffer g_glMapBuffer = NULL;
static FP_glMapBufferRange g_glMapBufferRange = NULL;
static FP_glUnmapBuffer g_glUnmapBuffer = NULL;

static FP_glCompressedTexImage2D g_glCompressedTexImage2D = nullptr;

#elif defined(__EFFEKSEER_RENDERER_GLES2__)

typedef void (*FP_glGenVertexArraysOES)(GLsizei n, GLuint* arrays);
typedef void (*FP_glDeleteVertexArraysOES)(GLsizei n, const GLuint* arrays);
typedef void (*FP_glBindVertexArrayOES)(GLuint array);

typedef void* (*FP_glMapBufferOES)(GLenum target, GLenum access);
typedef void* (*FP_glMapBufferRangeEXT)(GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access);
typedef GLboolean (*FP_glUnmapBufferOES)(GLenum target);

static FP_glGenVertexArraysOES g_glGenVertexArraysOES = NULL;
static FP_glDeleteVertexArraysOES g_glDeleteVertexArraysOES = NULL;
static FP_glBindVertexArrayOES g_glBindVertexArrayOES = NULL;

static FP_glMapBufferOES g_glMapBufferOES = NULL;
static FP_glMapBufferRangeEXT g_glMapBufferRangeEXT = NULL;
static FP_glUnmapBufferOES g_glUnmapBufferOES = NULL;

#endif

static bool g_isInitialized = false;
static bool g_isSupportedVertexArray = false;
static bool g_isSurrpotedBufferRange = false;
static bool g_isSurrpotedMapBuffer = false;
static OpenGLDeviceType g_deviceType = OpenGLDeviceType::OpenGL2;

#if _WIN32
#define GET_PROC(name)                              \
	g_##name = (FP_##name)wglGetProcAddress(#name); \
	if (g_##name == NULL)                           \
		return false;
#elif defined(__EFFEKSEER_RENDERER_GLES2__) || defined(__EFFEKSEER_RENDERER_GLES3__)
#define GET_PROC(name)                              \
	g_##name = (FP_##name)eglGetProcAddress(#name); \
	if (g_##name == NULL)                           \
		return false;
#endif

OpenGLDeviceType GetDeviceType()
{
	return g_deviceType;
}

bool Initialize(OpenGLDeviceType deviceType)
{
	if (g_isInitialized)
		return true;
	g_deviceType = deviceType;

#if _WIN32
	GET_PROC(glDeleteBuffers);
	GET_PROC(glCreateShader);
	GET_PROC(glBindBuffer);
	GET_PROC(glGenBuffers);
	GET_PROC(glBufferData);
	GET_PROC(glBlendEquationSeparate);
	GET_PROC(glBlendFuncSeparate);
	GET_PROC(glBlendEquation);
	GET_PROC(glActiveTexture);
	GET_PROC(glUniform1i);
	GET_PROC(glShaderSource);

	GET_PROC(glCompileShader);
	GET_PROC(glGetShaderiv);
	GET_PROC(glCreateProgram);

	GET_PROC(glAttachShader);
	GET_PROC(glDeleteProgram);
	GET_PROC(glDeleteShader);

	GET_PROC(glLinkProgram);
	GET_PROC(glGetProgramiv);
	GET_PROC(glGetShaderInfoLog);

	GET_PROC(glGetProgramInfoLog);
	GET_PROC(glGetAttribLocation);
	GET_PROC(glGetUniformLocation);

	GET_PROC(glUseProgram);
	GET_PROC(glEnableVertexAttribArray);
	GET_PROC(glDisableVertexAttribArray);

	GET_PROC(glVertexAttribPointer);
	GET_PROC(glUniformMatrix4fv);
	GET_PROC(glUniform4fv);

	GET_PROC(glGenerateMipmap);
	GET_PROC(glBufferSubData);

	GET_PROC(glGenVertexArrays);
	GET_PROC(glDeleteVertexArrays);
	GET_PROC(glBindVertexArray);

	GET_PROC(glGenSamplers);
	GET_PROC(glDeleteSamplers);
	GET_PROC(glSamplerParameteri);
	GET_PROC(glBindSampler);

	GET_PROC(glMapBuffer);
	GET_PROC(glMapBufferRange);
	GET_PROC(glUnmapBuffer);

	GET_PROC(glCompressedTexImage2D);

	g_isSupportedVertexArray = (g_glGenVertexArrays && g_glDeleteVertexArrays && g_glBindVertexArray);
	g_isSurrpotedBufferRange = (g_glMapBufferRange && g_glUnmapBuffer);
	g_isSurrpotedMapBuffer = (g_glMapBuffer && g_glUnmapBuffer);

#endif

#if defined(__EFFEKSEER_RENDERER_GLES2__)

#if defined(__APPLE__)
	g_glGenVertexArraysOES = ::glGenVertexArraysOES;
	g_glDeleteVertexArraysOES = ::glDeleteVertexArraysOES;
	g_glBindVertexArrayOES = ::glBindVertexArrayOES;
	g_isSupportedVertexArray = true;

	g_glMapBufferRangeEXT = ::glMapBufferRangeEXT;
	g_glMapBufferOES = ::glMapBufferOES;
	g_glUnmapBufferOES = ::glUnmapBufferOES;
	g_isSurrpotedBufferRange = true;
	g_isSurrpotedMapBuffer = true;
#else
	GET_PROC(glGenVertexArraysOES);
	GET_PROC(glDeleteVertexArraysOES);
	GET_PROC(glBindVertexArrayOES);
	char* glExtensions = (char*)glGetString(GL_EXTENSIONS);

#if defined(__EMSCRIPTEN__)
	g_isSupportedVertexArray = (g_glGenVertexArraysOES && g_glDeleteVertexArraysOES && g_glBindVertexArrayOES &&
								((glExtensions && strstr(glExtensions, "OES_vertex_array_object")) ? true : false));
#else
	g_isSupportedVertexArray = (g_glGenVertexArraysOES && g_glDeleteVertexArraysOES && g_glBindVertexArrayOES &&
								((glExtensions && strstr(glExtensions, "GL_OES_vertex_array_object")) ? true : false));
#endif

	// Some smartphone causes segmentation fault.
	//GET_PROC(glMapBufferRangeEXT);

#ifdef EMSCRIPTEN
	g_isSurrpotedBufferRange = false;
	g_isSurrpotedMapBuffer = false;
#else
	GET_PROC(glMapBufferOES);
	GET_PROC(glUnmapBufferOES);
	g_isSurrpotedBufferRange = (g_glMapBufferRangeEXT && g_glUnmapBufferOES);
	g_isSurrpotedMapBuffer = (g_glMapBufferOES && g_glUnmapBufferOES && ((glExtensions && strstr(glExtensions, "GL_OES_mapbuffer")) ? true : false));
#endif

#endif

#else
	if (deviceType == OpenGLDeviceType::OpenGL3 ||
		deviceType == OpenGLDeviceType::OpenGLES3)
	{
		g_isSupportedVertexArray = true;
		g_isSurrpotedBufferRange = true;
	}
	if (deviceType == OpenGLDeviceType::OpenGL3)
	{
		g_isSurrpotedMapBuffer = true;
	}

#endif

	g_isInitialized = true;
	return true;
}

bool IsSupportedVertexArray()
{
	return g_isSupportedVertexArray;
}

bool IsSupportedBufferRange()
{
	return g_isSurrpotedBufferRange;
}

bool IsSupportedMapBuffer()
{
	return g_isSurrpotedMapBuffer;
}

void MakeMapBufferInvalid()
{
	g_isSurrpotedMapBuffer = false;
}

void glDeleteBuffers(GLsizei n, const GLuint* buffers)
{
#if _WIN32
	g_glDeleteBuffers(n, buffers);
#else
	::glDeleteBuffers(n, buffers);
#endif
}

GLuint glCreateShader(GLenum type)
{
#if _WIN32
	return g_glCreateShader(type);
#else
	return ::glCreateShader(type);
#endif
}

void glBindBuffer(GLenum target, GLuint buffer)
{
#if _WIN32
	return g_glBindBuffer(target, buffer);
#else
	return ::glBindBuffer(target, buffer);
#endif
}

void glGenBuffers(GLsizei n, GLuint* buffers)
{
#if _WIN32
	g_glGenBuffers(n, buffers);
#else
	::glGenBuffers(n, buffers);
#endif
}

void glBufferData(GLenum target, GLsizeiptr size, const GLvoid* data, GLenum usage)
{
#if _WIN32
	g_glBufferData(target, size, data, usage);
#else
	::glBufferData(target, size, data, usage);
#endif
}

void glBlendEquationSeparate(GLenum modeRGB, GLenum modeAlpha)
{
#if _WIN32
	g_glBlendEquationSeparate(modeRGB, modeAlpha);
#else
	::glBlendEquationSeparate(modeRGB, modeAlpha);
#endif
}

void glBlendFuncSeparate(GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha)
{
#if _WIN32
	g_glBlendFuncSeparate(sfactorRGB, dfactorRGB, sfactorAlpha, dfactorAlpha);
#else
	::glBlendFuncSeparate(sfactorRGB, dfactorRGB, sfactorAlpha, dfactorAlpha);
#endif
}

void glBlendEquation(GLenum mode)
{
#if _WIN32
	g_glBlendEquation(mode);
#else
	::glBlendEquation(mode);
#endif
}

void glActiveTexture(GLenum texture)
{
#if _WIN32
	g_glActiveTexture(texture);
#else
	::glActiveTexture(texture);
#endif
}

void glUniform1i(GLint location, GLint v0)
{
#if _WIN32
	g_glUniform1i(location, v0);
#else
	::glUniform1i(location, v0);
#endif
}

void glShaderSource(GLuint shader, GLsizei count, const GLchar** strings, const GLint* lengths)
{
#if _WIN32
	g_glShaderSource(shader, count, strings, lengths);
#else
	::glShaderSource(shader, count, strings, lengths);
#endif
}

void glCompileShader(GLuint shader)
{
#if _WIN32
	g_glCompileShader(shader);
#else
	::glCompileShader(shader);
#endif
}

void glGetShaderiv(GLuint shader, GLenum pname, GLint* param)
{
#if _WIN32
	g_glGetShaderiv(shader, pname, param);
#else
	::glGetShaderiv(shader, pname, param);
#endif
}

GLuint glCreateProgram(void)
{
#if _WIN32
	return g_glCreateProgram();
#else
	return ::glCreateProgram();
#endif
}

void glAttachShader(GLuint program, GLuint shader)
{
#if _WIN32
	g_glAttachShader(program, shader);
#else
	::glAttachShader(program, shader);
#endif
}

void glDeleteProgram(GLuint program)
{
#if _WIN32
	g_glDeleteProgram(program);
#else
	::glDeleteProgram(program);
#endif
}

void glDeleteShader(GLuint shader)
{
#if _WIN32
	g_glDeleteShader(shader);
#else
	::glDeleteShader(shader);
#endif
}

void glLinkProgram(GLuint program)
{
#if _WIN32
	g_glLinkProgram(program);
#else
	::glLinkProgram(program);
#endif
}

void glGetProgramiv(GLuint program, GLenum pname, GLint* param)
{
#if _WIN32
	g_glGetProgramiv(program, pname, param);
#else
	::glGetProgramiv(program, pname, param);
#endif
}

void glGetShaderInfoLog(GLuint shader, GLsizei bufSize, GLsizei* length, GLchar* infoLog)
{
#if _WIN32
	g_glGetShaderInfoLog(shader, bufSize, length, infoLog);
#else
	::glGetShaderInfoLog(shader, bufSize, length, infoLog);
#endif
}

void glGetProgramInfoLog(GLuint program, GLsizei bufSize, GLsizei* length, GLchar* infoLog)
{
#if _WIN32
	g_glGetProgramInfoLog(program, bufSize, length, infoLog);
#else
	::glGetProgramInfoLog(program, bufSize, length, infoLog);
#endif
}

GLint glGetAttribLocation(GLuint program, const GLchar* name)
{
#if _WIN32
	return g_glGetAttribLocation(program, name);
#else
	return ::glGetAttribLocation(program, name);
#endif
}

GLint glGetUniformLocation(GLuint program, const GLchar* name)
{
#if _WIN32
	return g_glGetUniformLocation(program, name);
#else
	return ::glGetUniformLocation(program, name);
#endif
}

void glUseProgram(GLuint program)
{
#if _WIN32
	g_glUseProgram(program);
#else
	::glUseProgram(program);
#endif
}

void glEnableVertexAttribArray(GLuint index)
{
#if _WIN32
	g_glEnableVertexAttribArray(index);
#else
	::glEnableVertexAttribArray(index);
#endif
}

void glDisableVertexAttribArray(GLuint index)
{
#if _WIN32
	g_glDisableVertexAttribArray(index);
#else
	::glDisableVertexAttribArray(index);
#endif
}

void glVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* pointer)
{
#if _WIN32
	g_glVertexAttribPointer(index, size, type, normalized, stride, pointer);
#else
	::glVertexAttribPointer(index, size, type, normalized, stride, pointer);
#endif
}

void glUniformMatrix4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value)
{
#if _WIN32
	g_glUniformMatrix4fv(location, count, transpose, value);
#else
	::glUniformMatrix4fv(location, count, transpose, value);
#endif
}

void glUniform4fv(GLint location, GLsizei count, const GLfloat* value)
{
#if _WIN32
	g_glUniform4fv(location, count, value);
#else
	::glUniform4fv(location, count, value);
#endif
}

void glGenerateMipmap(GLenum target)
{
#if _WIN32
	g_glGenerateMipmap(target);
#else
	::glGenerateMipmap(target);
#endif
}

void glBufferSubData(GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid* data)
{
#if _WIN32
	g_glBufferSubData(target, offset, size, data);
#else
	::glBufferSubData(target, offset, size, data);
#endif
}

void glGenVertexArrays(GLsizei n, GLuint* arrays)
{
#if _WIN32
	g_glGenVertexArrays(n, arrays);
#elif defined(__EFFEKSEER_RENDERER_GLES2__)
	g_glGenVertexArraysOES(n, arrays);
#else
	::glGenVertexArrays(n, arrays);
#endif
}

void glDeleteVertexArrays(GLsizei n, const GLuint* arrays)
{
#if _WIN32
	g_glDeleteVertexArrays(n, arrays);
#elif defined(__EFFEKSEER_RENDERER_GLES2__)
	g_glDeleteVertexArraysOES(n, arrays);
#else
	::glDeleteVertexArrays(n, arrays);
#endif
}

void glBindVertexArray(GLuint array)
{
#if _WIN32
	g_glBindVertexArray(array);
#elif defined(__EFFEKSEER_RENDERER_GLES2__)
	g_glBindVertexArrayOES(array);
#else
	::glBindVertexArray(array);
#endif
}

void glGenSamplers(GLsizei n, GLuint* samplers)
{
#if _WIN32
	g_glGenSamplers(n, samplers);
#elif defined(__EFFEKSEER_RENDERER_GLES2__) || defined(__EFFEKSEER_RENDERER_GL2__)
#else
	::glGenSamplers(n, samplers);
#endif
}

void glDeleteSamplers(GLsizei n, const GLuint* samplers)
{
#if _WIN32
	g_glDeleteSamplers(n, samplers);
#elif defined(__EFFEKSEER_RENDERER_GLES2__) || defined(__EFFEKSEER_RENDERER_GL2__)
#else
	::glDeleteSamplers(n, samplers);
#endif
}

void glSamplerParameteri(GLuint sampler, GLenum pname, GLint param)
{
#if _WIN32
	g_glSamplerParameteri(sampler, pname, param);
#elif defined(__EFFEKSEER_RENDERER_GLES2__) || defined(__EFFEKSEER_RENDERER_GL2__)
#else
	::glSamplerParameteri(sampler, pname, param);
#endif
}

void glBindSampler(GLuint unit, GLuint sampler)
{
#if _WIN32
	g_glBindSampler(unit, sampler);
#elif defined(__EFFEKSEER_RENDERER_GLES2__) || defined(__EFFEKSEER_RENDERER_GL2__)
#else
	::glBindSampler(unit, sampler);
#endif
}

void* glMapBuffer(GLenum target, GLenum access)
{
#if _WIN32
	return g_glMapBuffer(target, access);
#elif defined(__EFFEKSEER_RENDERER_GLES2__)
	return g_glMapBufferOES(target, access);
#elif defined(__EFFEKSEER_RENDERER_GLES3__)
	return nullptr;
#else
	return ::glMapBuffer(target, access);
#endif
}

void* glMapBufferRange(GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access)
{
#if _WIN32
	return g_glMapBufferRange(target, offset, length, access);
#elif defined(__EFFEKSEER_RENDERER_GLES2__)
	return g_glMapBufferRangeEXT(target, offset, length, access);
#else

#if defined(__APPLE__)

#if defined(GL_ARB_map_buffer_range)
	return ::glMapBufferRange(target, offset, length, access);
#else
	return nullptr;
#endif

#else
	return ::glMapBufferRange(target, offset, length, access);
#endif

#endif
}

GLboolean glUnmapBuffer(GLenum target)
{
#if _WIN32
	return g_glUnmapBuffer(target);
#elif defined(__EFFEKSEER_RENDERER_GLES2__)
	return g_glUnmapBufferOES(target);
#else
	return ::glUnmapBuffer(target);
#endif
}

void glCompressedTexImage2D(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void* data)
{
#if _WIN32
	g_glCompressedTexImage2D(target, level, internalformat, width, height, border, imageSize, data);
#elif defined(__EFFEKSEER_RENDERER_GLES2__) || defined(__EFFEKSEER_RENDERER_GL2__)
#else
	::glCompressedTexImage2D(target, level, internalformat, width, height, border, imageSize, data);
#endif
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
} // namespace GLExt
} // namespace EffekseerRendererGL


//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
namespace EffekseerRendererGL
{
//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
IndexBuffer::IndexBuffer(RendererImplemented* renderer, GLuint buffer, int maxCount, bool isDynamic, int32_t stride, bool hasRefCount)
	: DeviceObject(renderer, renderer->GetDeviceObjectCollection(), hasRefCount)
	, IndexBufferBase(maxCount, isDynamic)
	, m_buffer(buffer)
{
	stride_ = stride;
	m_resource = new uint8_t[m_indexMaxCount * stride_];
}

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
IndexBuffer::~IndexBuffer()
{
	delete[] m_resource;
	GLExt::glDeleteBuffers(1, &m_buffer);
}

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
IndexBuffer* IndexBuffer::Create(RendererImplemented* renderer, int maxCount, bool isDynamic, int32_t stride, bool hasRefCount)
{
	GLuint ib;
	GLExt::glGenBuffers(1, &ib);
	return new IndexBuffer(renderer, ib, maxCount, isDynamic, stride, hasRefCount);
}

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
void IndexBuffer::OnLostDevice()
{
	GLExt::glDeleteBuffers(1, &m_buffer);
	m_buffer = 0;
}

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
void IndexBuffer::OnResetDevice()
{
	if (IsValid())
		return;
	GLuint ib;
	GLExt::glGenBuffers(1, &ib);
	m_buffer = ib;
}

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
void IndexBuffer::Lock()
{
	assert(!m_isLock);

	m_isLock = true;
	m_indexCount = 0;
}

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
void IndexBuffer::Unlock()
{
	assert(m_isLock);

	GLExt::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffer);
	GLExt::glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indexCount * stride_, m_resource, GL_DYNAMIC_DRAW);
	GLExt::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	m_isLock = false;
}

bool IndexBuffer::IsValid()
{
	return m_buffer != 0;
}

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
} // namespace EffekseerRendererGL
//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------


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
ModelLoader::ModelLoader(::Effekseer::FileInterface* fileInterface)
	: m_fileInterface(fileInterface)
{
	if (m_fileInterface == NULL)
	{
		m_fileInterface = &m_defaultFileInterface;
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
ModelLoader::~ModelLoader()
{
}

void* ModelLoader::Load(const EFK_CHAR* path)
{
	std::unique_ptr<Effekseer::FileReader>
		reader(m_fileInterface->OpenRead(path));

	if (reader.get() != NULL)
	{
		size_t size_model = reader->GetLength();
		char* data_model = new char[size_model];
		reader->Read(data_model, size_model);

		Model* model = (Model*)Load(data_model, (int32_t)size_model);

		delete[] data_model;

		return (void*)model;
	}

	return NULL;
}

void* ModelLoader::Load(const void* data, int32_t size)
{
	Model* model = new Model((uint8_t*)data, size);
	return model;
}

void ModelLoader::Unload(void* data)
{
	if (data != NULL)
	{
		Model* model = (Model*)data;
		delete model;
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
} // namespace EffekseerRendererGL
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------


//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
namespace EffekseerRendererGL
{
//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
static std::string Replace(std::string target, std::string from_, std::string to_)
{
	std::string::size_type Pos(target.find(from_));

	while (Pos != std::string::npos)
	{
		target.replace(Pos, from_.length(), to_);
		Pos = target.find(from_, Pos + to_.length());
	}

	return target;
}

static const char g_model_vs_src[] =
	R"(
IN vec4 a_Position;
IN vec4 a_Normal;
IN vec4 a_Binormal;
IN vec4 a_Tangent;
IN vec4 a_TexCoord;
IN vec4 a_Color;
)"
#if defined(MODEL_SOFTWARE_INSTANCING)
	R"(
IN float a_InstanceID;
IN vec4 a_UVOffset;
IN vec4 a_ModelColor;
)"
#endif
	R"(
OUT mediump vec4 v_Normal;
OUT mediump vec4 v_Binormal;
OUT mediump vec4 v_Tangent;
CENTROID OUT mediump vec4 v_TexCoord;
CENTROID OUT lowp vec4 v_Color;
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
uniform mat4 ProjectionMatrix;
uniform vec4 LightDirection;
uniform vec4 LightColor;
uniform vec4 LightAmbient;
uniform vec4 mUVInversed;

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
	vec4 localPosition = modelMatrix * a_Position;
	gl_Position = ProjectionMatrix * localPosition;

	v_TexCoord.xy = a_TexCoord.xy * uvOffset.zw + uvOffset.xy;

	if (LightingEnable)
	{
		mat3 lightMatrix = mat3(modelMatrix[0].xyz, modelMatrix[1].xyz, modelMatrix[2].xyz);
		vec3 localNormal = normalize( lightMatrix * a_Normal.xyz );

		v_Normal = vec4(localNormal, 1.0);
		v_Binormal = vec4(normalize( lightMatrix * a_Binormal.xyz ), 1.0);
		v_Tangent = vec4(normalize( lightMatrix * a_Tangent.xyz ), 1.0);
	}

	v_Color = modelColor;
	v_TexCoord.y = mUVInversed.x + mUVInversed.y * v_TexCoord.y;
}
)";

static const char g_model_fs_src[] =
	R"(

IN mediump vec4 v_Normal;
IN mediump vec4 v_Binormal;
IN mediump vec4 v_Tangent;
CENTROID IN mediump vec4 v_TexCoord;
CENTROID IN lowp vec4 v_Color;

uniform sampler2D ColorTexture;
uniform sampler2D NormalTexture;
uniform vec4 LightDirection;
uniform vec4 LightColor;
uniform vec4 LightAmbient;

void main()
{
	FRAGCOLOR = v_Color * TEX2D(ColorTexture, v_TexCoord.xy);

	if (LightingEnable)
	{
		vec3 texNormal = (TEX2D(NormalTexture, v_TexCoord.xy).xyz - 0.5) * 2.0;
		mat3 normalMatrix = mat3(v_Tangent.xyz, v_Binormal.xyz, v_Normal.xyz );
		vec3 localNormal = normalize( normalMatrix * texNormal );
		float diffuse = max(0.0, dot(localNormal, LightDirection.xyz));
		FRAGCOLOR.xyz = FRAGCOLOR.xyz * (LightColor.xyz * diffuse + LightAmbient.xyz);
	}

	if(FRAGCOLOR.w <= 0.0) discard;
}

)";

static const char g_model_distortion_vs_src[] =
	"IN vec4 a_Position;\n"
	"IN vec4 a_Normal;\n"
	"IN vec4 a_Binormal;\n"
	"IN vec4 a_Tangent;\n"
	"IN vec4 a_TexCoord;\n"
	"IN vec4 a_Color;\n"
#if defined(MODEL_SOFTWARE_INSTANCING)
	"IN float a_InstanceID;\n"
	"IN vec4 a_UVOffset;\n"
	"IN vec4 a_ModelColor;\n"
#endif

	R"(
OUT mediump vec4 v_Normal;
OUT mediump vec4 v_Binormal;
OUT mediump vec4 v_Tangent;
CENTROID OUT mediump vec4 v_TexCoord;
OUT mediump vec4 v_Pos;
CENTROID OUT lowp vec4 v_Color;
)"

#if defined(MODEL_SOFTWARE_INSTANCING)
	"uniform mat4 ModelMatrix[20];\n"
	"uniform vec4 UVOffset[20];\n"
	"uniform vec4 ModelColor[20];\n"
#else
	"uniform mat4 ModelMatrix;\n"
	"uniform vec4 UVOffset;\n"
	"uniform vec4 ModelColor;\n"
#endif
	"uniform mat4 ProjectionMatrix;\n"
	"uniform vec4 mUVInversed;\n"

	"void main() {\n"
#if defined(MODEL_SOFTWARE_INSTANCING)
	"	mat4 modelMatrix = ModelMatrix[int(a_InstanceID)];\n"
	"	vec4 uvOffset = a_UVOffset;\n"
	"	vec4 modelColor = a_ModelColor;\n"
#else
	"	mat4 modelMatrix = ModelMatrix;\n"
	"	vec4 uvOffset = UVOffset;\n"
	"	vec4 modelColor = ModelColor;\n"
#endif

	R"(
	vec4 localPosition = vec4( a_Position.x, a_Position.y, a_Position.z, 1.0 );
	vec4 localNormal = vec4( a_Position.x + a_Normal.x, a_Position.y + a_Normal.y, a_Position.z + a_Normal.z, 1.0 );
	vec4 localBinormal = vec4( a_Position.x + a_Binormal.x, a_Position.y + a_Binormal.y, a_Position.z + a_Binormal.z, 1.0 );
	vec4 localTangent = vec4( a_Position.x + a_Tangent.x, a_Position.y + a_Tangent.y, a_Position.z + a_Tangent.z, 1.0 );


	localPosition = modelMatrix * localPosition;
	localNormal = modelMatrix * localNormal;
	localBinormal = modelMatrix * localBinormal;
	localTangent = modelMatrix * localTangent;

	localNormal = localPosition + normalize(localNormal - localPosition);
	localBinormal = localPosition + normalize(localBinormal - localPosition);
	localTangent = localPosition + normalize(localTangent - localPosition);

	gl_Position = ProjectionMatrix * localPosition;

	v_TexCoord.xy = a_TexCoord.xy * uvOffset.zw + uvOffset.xy;

	v_Normal = ProjectionMatrix * localNormal;
	v_Binormal = ProjectionMatrix * localBinormal;
	v_Tangent = ProjectionMatrix * localTangent;
	v_Pos = gl_Position;

	v_Color = modelColor * a_Color;

	v_TexCoord.y = mUVInversed.x + mUVInversed.y * v_TexCoord.y;
}
)";

static const char g_model_distortion_fs_src[] =
	"IN mediump vec4 v_Normal;\n"
	"IN mediump vec4 v_Binormal;\n"
	"IN mediump vec4 v_Tangent;\n"
	"CENTROID IN mediump vec4 v_TexCoord;\n"
	"IN mediump vec4 v_Pos;\n"
	"CENTROID IN lowp vec4 v_Color;\n"

	R"(
uniform sampler2D uTexture0;
uniform sampler2D uBackTexture0;

uniform	vec4	g_scale;
uniform	vec4	mUVInversedBack;

void main() {
	if (TextureEnable)
	{
		FRAGCOLOR = TEX2D(uTexture0, v_TexCoord.xy);
	}
	else
	{
		FRAGCOLOR = vec4(1.0, 1.0, 1.0, 1.0);
	}

	FRAGCOLOR.a = FRAGCOLOR.a * v_Color.a;

	vec2 pos = v_Pos.xy / v_Pos.w;
	vec2 posU = v_Tangent.xy / v_Tangent.w;
	vec2 posR = v_Binormal.xy / v_Binormal.w;

	float xscale = (FRAGCOLOR.x * 2.0 - 1.0) * v_Color.x * g_scale.x;
	float yscale = (FRAGCOLOR.y * 2.0 - 1.0) * v_Color.y * g_scale.x;

	vec2 uv = pos + (posR - pos) * xscale + (posU - pos) * yscale;

	uv.x = (uv.x + 1.0) * 0.5;
	uv.y = (uv.y + 1.0) * 0.5;
//	uv.y = 1.0 - (uv.y + 1.0) * 0.5;

	uv.y = mUVInversedBack.x + mUVInversedBack.y * uv.y;

	vec3 color = TEX2D(uBackTexture0, uv).xyz;
	FRAGCOLOR.xyz = color;

	if(FRAGCOLOR.w <= 0.0) discard;
}
)";

static ShaderAttribInfo g_model_attribs[ModelRenderer::NumAttribs] = {
	{"a_Position", GL_FLOAT, 3, 0, false},
	{"a_Normal", GL_FLOAT, 3, 12, false},
	{"a_Binormal", GL_FLOAT, 3, 24, false},
	{"a_Tangent", GL_FLOAT, 3, 36, false},
	{"a_TexCoord", GL_FLOAT, 2, 48, false},
	{"a_Color", GL_UNSIGNED_BYTE, 4, 56, true},
#if defined(MODEL_SOFTWARE_INSTANCING)
	{"a_InstanceID", GL_FLOAT, 1, 0, false},
	{"a_UVOffset", GL_FLOAT, 4, 0, false},
	{"a_ModelColor", GL_FLOAT, 4, 0, false},
#endif
};

static ShaderUniformInfo g_model_uniforms[ModelRenderer::NumUniforms] = {
	{"ProjectionMatrix"},
	{"ModelMatrix"},
#if !defined(MODEL_SOFTWARE_INSTANCING)
	{"UVOffset"},
	{"ModelColor"},
#endif
	{"ColorTexture"},
	{"NormalTexture"},
	{"LightDirection"},
	{"LightColor"},
	{"LightAmbient"},
	{"g_scale"},
	{"mUVInversed"},
	{"mUVInversedBack"},
};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
ModelRenderer::ModelRenderer(
	RendererImplemented* renderer,
	Shader* shader_lighting_texture_normal,
	Shader* shader_texture,
	Shader* shader_distortion_texture)
	: m_renderer(renderer)
	, m_shader_lighting_texture_normal(shader_lighting_texture_normal)
	, m_shader_texture(shader_texture)
	, m_shader_distortion_texture(shader_distortion_texture)
{
	for (size_t i = 0; i < 8; i++)
	{
		m_va[i] = nullptr;
	}

	shader_lighting_texture_normal->GetAttribIdList(NumAttribs, g_model_attribs);
	shader_lighting_texture_normal->GetUniformIdList(NumUniforms, g_model_uniforms, m_uniformLoc[0]);
	shader_lighting_texture_normal->SetTextureSlot(0, shader_lighting_texture_normal->GetUniformId("ColorTexture"));
	shader_lighting_texture_normal->SetTextureSlot(1, shader_lighting_texture_normal->GetUniformId("NormalTexture"));

	shader_texture->GetAttribIdList(NumAttribs, g_model_attribs);
	shader_texture->GetUniformIdList(NumUniforms, g_model_uniforms, m_uniformLoc[4]);
	shader_texture->SetTextureSlot(0, shader_texture->GetUniformId("ColorTexture"));

	shader_distortion_texture->GetAttribIdList(NumAttribs, g_model_attribs);
	shader_distortion_texture->GetUniformIdList(NumUniforms, g_model_uniforms, m_uniformLoc[6]);
	shader_distortion_texture->SetTextureSlot(0, shader_distortion_texture->GetUniformId("uTexture0"));
	shader_distortion_texture->SetTextureSlot(1, shader_distortion_texture->GetUniformId("uBackTexture0"));

	Shader* shaders[2];
	shaders[0] = m_shader_lighting_texture_normal;
	shaders[1] = m_shader_texture;

	for (int32_t i = 0; i < 2; i++)
	{
		shaders[i]->SetVertexSize(sizeof(::Effekseer::Model::Vertex));

		shaders[i]->SetVertexConstantBufferSize(sizeof(::EffekseerRenderer::ModelRendererVertexConstantBuffer<1>));
		shaders[i]->AddVertexConstantLayout(
			CONSTANT_TYPE_MATRIX44,
			shaders[i]->GetUniformId("ProjectionMatrix"),
			0);

		shaders[i]->AddVertexConstantLayout(
			CONSTANT_TYPE_MATRIX44,
			shaders[i]->GetUniformId("ModelMatrix"),
			sizeof(Effekseer::Matrix44));

		shaders[i]->AddVertexConstantLayout(
			CONSTANT_TYPE_VECTOR4,
			shaders[i]->GetUniformId("UVOffset"),
			sizeof(Effekseer::Matrix44) * 2);

		shaders[i]->AddVertexConstantLayout(
			CONSTANT_TYPE_VECTOR4,
			shaders[i]->GetUniformId("ModelColor"),
			sizeof(Effekseer::Matrix44) * 2 + sizeof(float[4]) * 1);

		shaders[i]->AddVertexConstantLayout(
			CONSTANT_TYPE_VECTOR4,
			shaders[i]->GetUniformId("LightDirection"),
			sizeof(Effekseer::Matrix44) * 2 + sizeof(float[4]) * 2 + sizeof(float[4]) * 0);
		shaders[i]->AddVertexConstantLayout(
			CONSTANT_TYPE_VECTOR4,
			shaders[i]->GetUniformId("LightColor"),
			sizeof(Effekseer::Matrix44) * 2 + sizeof(float[4]) * 2 + sizeof(float[4]) * 1);
		shaders[i]->AddVertexConstantLayout(
			CONSTANT_TYPE_VECTOR4,
			shaders[i]->GetUniformId("LightAmbient"),
			sizeof(Effekseer::Matrix44) * 2 + sizeof(float[4]) * 2 + sizeof(float[4]) * 2);
		shaders[i]->AddVertexConstantLayout(
			CONSTANT_TYPE_VECTOR4,
			shaders[i]->GetUniformId("mUVInversed"),
			sizeof(Effekseer::Matrix44) * 2 + sizeof(float[4]) * 2 + sizeof(float[4]) * 3);

		shaders[i]->SetPixelConstantBufferSize(sizeof(::EffekseerRenderer::ModelRendererPixelConstantBuffer));
		shaders[i]->AddPixelConstantLayout(
			CONSTANT_TYPE_VECTOR4,
			shaders[i]->GetUniformId("LightDirection"),
			sizeof(float[4]) * 0);
		shaders[i]->AddPixelConstantLayout(
			CONSTANT_TYPE_VECTOR4,
			shaders[i]->GetUniformId("LightColor"),
			sizeof(float[4]) * 1);
		shaders[i]->AddPixelConstantLayout(
			CONSTANT_TYPE_VECTOR4,
			shaders[i]->GetUniformId("LightAmbient"),
			sizeof(float[4]) * 2);
	}

	Shader* shaders_d[1];
	shaders_d[0] = shader_distortion_texture;

	for (int32_t i = 0; i < 1; i++)
	{
		shaders_d[i]->SetVertexSize(sizeof(::Effekseer::Model::Vertex));

		shaders_d[i]->SetVertexConstantBufferSize(sizeof(::EffekseerRenderer::ModelRendererVertexConstantBuffer<1>));
		shaders_d[i]->AddVertexConstantLayout(
			CONSTANT_TYPE_MATRIX44,
			shaders_d[i]->GetUniformId("ProjectionMatrix"),
			0);

		shaders_d[i]->AddVertexConstantLayout(
			CONSTANT_TYPE_MATRIX44,
			shaders_d[i]->GetUniformId("ModelMatrix"),
			sizeof(Effekseer::Matrix44));

		shaders_d[i]->AddVertexConstantLayout(
			CONSTANT_TYPE_VECTOR4,
			shaders_d[i]->GetUniformId("UVOffset"),
			sizeof(Effekseer::Matrix44) * 2);

		shaders_d[i]->AddVertexConstantLayout(
			CONSTANT_TYPE_VECTOR4,
			shaders_d[i]->GetUniformId("ModelColor"),
			sizeof(Effekseer::Matrix44) * 2 + sizeof(float[4]) * 1);

		shaders_d[i]->AddVertexConstantLayout(
			CONSTANT_TYPE_VECTOR4,
			shaders_d[i]->GetUniformId("LightDirection"),
			sizeof(Effekseer::Matrix44) * 2 + sizeof(float[4]) * 2 + sizeof(float[4]) * 0);

		shaders_d[i]->AddVertexConstantLayout(
			CONSTANT_TYPE_VECTOR4,
			shaders_d[i]->GetUniformId("LightColor"),
			sizeof(Effekseer::Matrix44) * 2 + sizeof(float[4]) * 2 + sizeof(float[4]) * 1);

		shaders_d[i]->AddVertexConstantLayout(
			CONSTANT_TYPE_VECTOR4,
			shaders_d[i]->GetUniformId("LightAmbient"),
			sizeof(Effekseer::Matrix44) * 2 + sizeof(float[4]) * 2 + sizeof(float[4]) * 2);

		shaders_d[i]->AddVertexConstantLayout(
			CONSTANT_TYPE_VECTOR4,
			shaders_d[i]->GetUniformId("mUVInversed"),
			sizeof(Effekseer::Matrix44) * 2 + sizeof(float[4]) * 2 + sizeof(float[4]) * 3);

		shaders_d[i]->SetPixelConstantBufferSize(sizeof(float) * 4 * 2);
		shaders_d[i]->AddPixelConstantLayout(
			CONSTANT_TYPE_VECTOR4,
			shaders_d[i]->GetUniformId("g_scale"),
			sizeof(float[4]) * 0);

		shaders_d[i]->AddPixelConstantLayout(
			CONSTANT_TYPE_VECTOR4,
			shaders_d[i]->GetUniformId("mUVInversedBack"),
			sizeof(float[4]) * 1);
	}

	GLint currentVAO = 0;

	if (GLExt::IsSupportedVertexArray())
	{
		glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &currentVAO);
	}

	m_va[0] = VertexArray::Create(renderer, m_shader_lighting_texture_normal, nullptr, nullptr, true);
	m_va[4] = VertexArray::Create(renderer, m_shader_texture, nullptr, nullptr, true);
	m_va[6] = VertexArray::Create(renderer, m_shader_distortion_texture, nullptr, nullptr, true);

	if (GLExt::IsSupportedVertexArray())
	{
		GLExt::glBindVertexArray(currentVAO);
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
ModelRenderer::~ModelRenderer()
{
	for (size_t i = 0; i < 8; i++)
	{
		ES_SAFE_DELETE(m_va[i]);
	}

	ES_SAFE_DELETE(m_shader_lighting_texture_normal);

	ES_SAFE_DELETE(m_shader_texture);

	ES_SAFE_DELETE(m_shader_distortion_texture);
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
ModelRenderer* ModelRenderer::Create(RendererImplemented* renderer)
{
	assert(renderer != NULL);

	Shader* shader_lighting_texture_normal = NULL;
	Shader* shader_texture = NULL;
	Shader* shader_distortion_texture = NULL;

	std::string vs_ltn_src = g_model_vs_src;
	std::string fs_ltn_src = g_model_fs_src;

	std::string vs_t_src = g_model_vs_src;
	std::string fs_t_src = g_model_fs_src;

	std::string vs_d_t_src = g_model_distortion_vs_src;
	std::string fs_d_t_src = g_model_distortion_fs_src;

	vs_ltn_src = Replace(vs_ltn_src, "TextureEnable", "true");
	fs_ltn_src = Replace(fs_ltn_src, "TextureEnable", "true");
	vs_ltn_src = Replace(vs_ltn_src, "LightingEnable", "true");
	fs_ltn_src = Replace(fs_ltn_src, "LightingEnable", "true");
	vs_ltn_src = Replace(vs_ltn_src, "NormalMapEnable", "true");
	fs_ltn_src = Replace(fs_ltn_src, "NormalMapEnable", "true");

	vs_t_src = Replace(vs_t_src, "TextureEnable", "true");
	fs_t_src = Replace(fs_t_src, "TextureEnable", "true");
	vs_t_src = Replace(vs_t_src, "LightingEnable", "false");
	fs_t_src = Replace(fs_t_src, "LightingEnable", "false");
	vs_t_src = Replace(vs_t_src, "NormalMapEnable", "false");
	fs_t_src = Replace(fs_t_src, "NormalMapEnable", "false");

	vs_d_t_src = Replace(vs_d_t_src, "TextureEnable", "true");
	fs_d_t_src = Replace(fs_d_t_src, "TextureEnable", "true");

	shader_lighting_texture_normal =
		Shader::Create(renderer, vs_ltn_src.c_str(), vs_ltn_src.length(), fs_ltn_src.c_str(), fs_ltn_src.length(), "ModelRenderer1");
	if (shader_lighting_texture_normal == NULL)
		goto End;

	shader_texture = Shader::Create(renderer, vs_t_src.c_str(), vs_t_src.length(), fs_t_src.c_str(), fs_t_src.length(), "ModelRenderer5");
	if (shader_texture == NULL)
		goto End;

	shader_distortion_texture =
		Shader::Create(renderer, vs_d_t_src.c_str(), vs_d_t_src.length(), fs_d_t_src.c_str(), fs_d_t_src.length(), "ModelRenderer7");
	if (shader_distortion_texture == NULL)
		goto End;

	return new ModelRenderer(
		renderer,
		shader_lighting_texture_normal,
		shader_texture,
		shader_distortion_texture);
End:;

	ES_SAFE_DELETE(shader_lighting_texture_normal);
	ES_SAFE_DELETE(shader_texture);
	ES_SAFE_DELETE(shader_distortion_texture);
	return nullptr;
}

void ModelRenderer::BeginRendering(const efkModelNodeParam& parameter, int32_t count, void* userData)
{
	BeginRendering_(m_renderer, parameter, count, userData);
}

void ModelRenderer::Rendering(const efkModelNodeParam& parameter, const InstanceParameter& instanceParameter, void* userData)
{
	Rendering_<
		RendererImplemented>(
		m_renderer,
		parameter,
		instanceParameter,
		userData);
}

void ModelRenderer::EndRendering(const efkModelNodeParam& parameter, void* userData)
{
	if (parameter.BasicParameterPtr->MaterialType == Effekseer::RendererMaterialType::BackDistortion)
	{
		m_renderer->SetVertexArray(m_va[6]);
	}
	else if (parameter.BasicParameterPtr->MaterialType == Effekseer::RendererMaterialType::Lighting)
	{
		m_renderer->SetVertexArray(m_va[0]);
	}
	else
	{
		m_renderer->SetVertexArray(m_va[4]);
	}

	if (parameter.ModelIndex < 0)
	{
		return;
	}

	auto model = (Model*)parameter.EffectPointer->GetModel(parameter.ModelIndex);
	if (model == nullptr)
	{
		return;
	}

	model->LoadToGPU();
	if (!model->IsLoadedOnGPU)
	{
		return;
	}

	m_shader_lighting_texture_normal->SetVertexSize(model->GetVertexSize());
	m_shader_texture->SetVertexSize(model->GetVertexSize());
	m_shader_distortion_texture->SetVertexSize(model->GetVertexSize());

#if defined(MODEL_SOFTWARE_INSTANCING)
	EndRendering_<
		RendererImplemented,
		Shader,
		GLuint,
		Model,
		true,
		20>(
		m_renderer,
		m_shader_lighting_texture_normal,
		m_shader_texture,
		m_shader_distortion_texture,
		parameter);
#else
	EndRendering_<
		RendererImplemented,
		Shader,
		Model,
		false,
		1>(
		m_renderer,
		m_shader_lighting_texture_normal,
		m_shader_texture,
		m_shader_distortion_texture,
		parameter);
#endif
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
} // namespace EffekseerRendererGL
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------




#undef min

namespace EffekseerRendererGL
{

::Effekseer::MaterialData* MaterialLoader::LoadAcutually(::Effekseer::Material& material, ::Effekseer::CompiledMaterialBinary* binary)
{
	auto materialData = new ::Effekseer::MaterialData();
	materialData->IsSimpleVertex = material.GetIsSimpleVertex();
	materialData->IsRefractionRequired = material.GetHasRefraction();

	std::array<Effekseer::MaterialShaderType, 2> shaderTypes;
	std::array<Effekseer::MaterialShaderType, 2> shaderTypesModel;

	shaderTypes[0] = Effekseer::MaterialShaderType::Standard;
	shaderTypes[1] = Effekseer::MaterialShaderType::Refraction;
	shaderTypesModel[0] = Effekseer::MaterialShaderType::Model;
	shaderTypesModel[1] = Effekseer::MaterialShaderType::RefractionModel;
	int32_t shaderTypeCount = 1;

	if (material.GetHasRefraction())
	{
		shaderTypeCount = 2;
	}

	for (int32_t st = 0; st < shaderTypeCount; st++)
	{
		auto parameterGenerator = EffekseerRenderer::MaterialShaderParameterGenerator(material, false, st, 1);

		auto shader = Shader::Create(deviceType_,
									 deviceObjectCollection_,
									 (const char*)binary->GetVertexShaderData(shaderTypes[st]),
									 binary->GetVertexShaderSize(shaderTypes[st]),
									 (const char*)binary->GetPixelShaderData(shaderTypes[st]),
									 binary->GetPixelShaderSize(shaderTypes[st]),
									 "CustomMaterial",
									 true);

		if (shader == nullptr)
		{
			std::cout << "Vertex shader error" << std::endl;
			std::cout << (const char*)binary->GetVertexShaderData(shaderTypesModel[st]) << std::endl;

			std::cout << "Pixel shader error" << std::endl;
			std::cout << (const char*)binary->GetPixelShaderData(shaderTypesModel[st]) << std::endl;

			return nullptr;
		}

		if (materialData->IsSimpleVertex)
		{
			EffekseerRendererGL::ShaderAttribInfo sprite_attribs[3] = {
				{"atPosition", GL_FLOAT, 3, 0, false}, {"atColor", GL_UNSIGNED_BYTE, 4, 12, true}, {"atTexCoord", GL_FLOAT, 2, 16, false}};
			shader->GetAttribIdList(3, sprite_attribs);
			shader->SetVertexSize(sizeof(EffekseerRendererGL::Vertex));
		}
		else
		{
			EffekseerRendererGL::ShaderAttribInfo sprite_attribs[8] = {
				{"atPosition", GL_FLOAT, 3, 0, false},
				{"atColor", GL_UNSIGNED_BYTE, 4, 12, true},
				{"atNormal", GL_UNSIGNED_BYTE, 4, 16, true},
				{"atTangent", GL_UNSIGNED_BYTE, 4, 20, true},
				{"atTexCoord", GL_FLOAT, 2, 24, false},
				{"atTexCoord2", GL_FLOAT, 2, 32, false},
				{"", GL_FLOAT, 0, 0, false},
				{"", GL_FLOAT, 0, 0, false},
			};

			int32_t offset = 40;
			int count = 6;
			const char* customData1Name = "atCustomData1";
			const char* customData2Name = "atCustomData2";

			if (material.GetCustomData1Count() > 0)
			{
				sprite_attribs[count].name = customData1Name;
				sprite_attribs[count].count = material.GetCustomData1Count();
				sprite_attribs[count].offset = offset;
				count++;
				offset += sizeof(float) * material.GetCustomData1Count();
			}

			if (material.GetCustomData2Count() > 0)
			{
				sprite_attribs[count].name = customData2Name;
				sprite_attribs[count].count = material.GetCustomData2Count();
				sprite_attribs[count].offset = offset;
				count++;
				offset += sizeof(float) * material.GetCustomData2Count();
			}

			shader->GetAttribIdList(count, sprite_attribs);
			shader->SetVertexSize(sizeof(EffekseerRenderer::DynamicVertex) +
								  sizeof(float) * (material.GetCustomData1Count() + material.GetCustomData2Count()));
		}

		shader->AddVertexConstantLayout(
			CONSTANT_TYPE_MATRIX44, shader->GetUniformId("uMatCamera"), parameterGenerator.VertexCameraMatrixOffset);

		shader->AddVertexConstantLayout(
			CONSTANT_TYPE_MATRIX44, shader->GetUniformId("uMatProjection"), parameterGenerator.VertexProjectionMatrixOffset);

		shader->AddVertexConstantLayout(
			CONSTANT_TYPE_VECTOR4, shader->GetUniformId("mUVInversed"), parameterGenerator.VertexInversedFlagOffset);

		shader->AddVertexConstantLayout(
			CONSTANT_TYPE_VECTOR4, shader->GetUniformId("predefined_uniform"), parameterGenerator.VertexPredefinedOffset);

		shader->AddVertexConstantLayout(
			CONSTANT_TYPE_VECTOR4, shader->GetUniformId("cameraPosition"), parameterGenerator.VertexCameraPositionOffset);

		for (int32_t ui = 0; ui < material.GetUniformCount(); ui++)
		{
			shader->AddVertexConstantLayout(CONSTANT_TYPE_VECTOR4,
											shader->GetUniformId(material.GetUniformName(ui)),
											parameterGenerator.VertexUserUniformOffset + sizeof(float) * 4 * ui);
		}

		shader->SetVertexConstantBufferSize(parameterGenerator.VertexShaderUniformBufferSize);

		shader->AddPixelConstantLayout(
			CONSTANT_TYPE_VECTOR4, shader->GetUniformId("mUVInversedBack"), parameterGenerator.PixelInversedFlagOffset);

		shader->AddPixelConstantLayout(
			CONSTANT_TYPE_VECTOR4, shader->GetUniformId("predefined_uniform"), parameterGenerator.PixelPredefinedOffset);

		shader->AddPixelConstantLayout(
			CONSTANT_TYPE_VECTOR4, shader->GetUniformId("cameraPosition"), parameterGenerator.PixelCameraPositionOffset);

		// shiding model
		if (material.GetShadingModel() == ::Effekseer::ShadingModelType::Lit)
		{
			shader->AddPixelConstantLayout(
				CONSTANT_TYPE_VECTOR4, shader->GetUniformId("lightDirection"), parameterGenerator.PixelLightDirectionOffset);
			shader->AddPixelConstantLayout(
				CONSTANT_TYPE_VECTOR4, shader->GetUniformId("lightColor"), parameterGenerator.PixelLightColorOffset);
			shader->AddPixelConstantLayout(
				CONSTANT_TYPE_VECTOR4, shader->GetUniformId("lightAmbientColor"), parameterGenerator.PixelLightAmbientColorOffset);
		}
		else if (material.GetShadingModel() == ::Effekseer::ShadingModelType::Unlit)
		{
		}

		if (material.GetHasRefraction() && st == 1)
		{
			shader->AddPixelConstantLayout(
				CONSTANT_TYPE_MATRIX44, shader->GetUniformId("cameraMat"), parameterGenerator.PixelCameraMatrixOffset);
		}

		for (int32_t ui = 0; ui < material.GetUniformCount(); ui++)
		{
			shader->AddPixelConstantLayout(CONSTANT_TYPE_VECTOR4,
										   shader->GetUniformId(material.GetUniformName(ui)),
										   parameterGenerator.PixelUserUniformOffset + sizeof(float) * 4 * ui);
		}

		shader->SetPixelConstantBufferSize(parameterGenerator.PixelShaderUniformBufferSize);

		int32_t lastIndex = -1;
		for (int32_t ti = 0; ti < material.GetTextureCount(); ti++)
		{
			shader->SetTextureSlot(material.GetTextureIndex(ti), shader->GetUniformId(material.GetTextureName(ti)));
			lastIndex = Effekseer::Max(lastIndex, material.GetTextureIndex(ti));
		}

		lastIndex += 1;
		shader->SetTextureSlot(lastIndex, shader->GetUniformId("background"));

		materialData->TextureCount = material.GetTextureCount();
		materialData->UniformCount = material.GetUniformCount();

		if (st == 0)
		{
			materialData->UserPtr = shader;
		}
		else
		{
			materialData->RefractionUserPtr = shader;
		}
	}

	for (int32_t st = 0; st < shaderTypeCount; st++)
	{
		auto parameterGenerator = EffekseerRenderer::MaterialShaderParameterGenerator(material, true, st, 1);

		auto shader = Shader::Create(deviceType_,
									 deviceObjectCollection_,
									 (const char*)binary->GetVertexShaderData(shaderTypesModel[st]),
									 binary->GetVertexShaderSize(shaderTypesModel[st]),
									 (const char*)binary->GetPixelShaderData(shaderTypesModel[st]),
									 binary->GetPixelShaderSize(shaderTypesModel[st]),
									 "CustomMaterial",
									 true);

		if (shader == nullptr)
		{
			std::cout << "Vertex shader error" << std::endl;
			std::cout << (const char*)binary->GetVertexShaderData(shaderTypesModel[st]) << std::endl;

			std::cout << "Pixel shader error" << std::endl;
			std::cout << (const char*)binary->GetPixelShaderData(shaderTypesModel[st]) << std::endl;

			return nullptr;
		}

		static ShaderAttribInfo g_model_attribs[ModelRenderer::NumAttribs] = {
			{"a_Position", GL_FLOAT, 3, 0, false},
			{"a_Normal", GL_FLOAT, 3, 12, false},
			{"a_Binormal", GL_FLOAT, 3, 24, false},
			{"a_Tangent", GL_FLOAT, 3, 36, false},
			{"a_TexCoord", GL_FLOAT, 2, 48, false},
			{"a_Color", GL_UNSIGNED_BYTE, 4, 56, true},
#if defined(MODEL_SOFTWARE_INSTANCING)
			{"a_InstanceID", GL_FLOAT, 1, 0, false},
			{"a_UVOffset", GL_FLOAT, 4, 0, false},
			{"a_ModelColor", GL_FLOAT, 4, 0, false},
#endif
		};

		shader->GetAttribIdList(ModelRenderer::NumAttribs, g_model_attribs);
		shader->SetVertexSize(sizeof(::Effekseer::Model::Vertex));

		shader->AddVertexConstantLayout(
			CONSTANT_TYPE_MATRIX44, shader->GetUniformId("ProjectionMatrix"), parameterGenerator.VertexProjectionMatrixOffset);

		shader->AddVertexConstantLayout(
			CONSTANT_TYPE_MATRIX44, shader->GetUniformId("ModelMatrix"), parameterGenerator.VertexModelMatrixOffset);

		shader->AddVertexConstantLayout(CONSTANT_TYPE_VECTOR4, shader->GetUniformId("UVOffset"), parameterGenerator.VertexModelUVOffset);

		shader->AddVertexConstantLayout(
			CONSTANT_TYPE_VECTOR4, shader->GetUniformId("ModelColor"), parameterGenerator.VertexModelColorOffset);

		shader->AddVertexConstantLayout(
			CONSTANT_TYPE_VECTOR4, shader->GetUniformId("mUVInversed"), parameterGenerator.VertexInversedFlagOffset);

		shader->AddVertexConstantLayout(
			CONSTANT_TYPE_VECTOR4, shader->GetUniformId("predefined_uniform"), parameterGenerator.VertexPredefinedOffset);

		shader->AddVertexConstantLayout(
			CONSTANT_TYPE_VECTOR4, shader->GetUniformId("cameraPosition"), parameterGenerator.VertexCameraPositionOffset);

		if (material.GetCustomData1Count() > 0)
		{
			shader->AddVertexConstantLayout(
				CONSTANT_TYPE_VECTOR4, shader->GetUniformId("customData1"), parameterGenerator.VertexModelCustomData1Offset);
		}

		if (material.GetCustomData2Count() > 0)
		{
			shader->AddVertexConstantLayout(
				CONSTANT_TYPE_VECTOR4, shader->GetUniformId("customData2"), parameterGenerator.VertexModelCustomData2Offset);
		}

		for (int32_t ui = 0; ui < material.GetUniformCount(); ui++)
		{
			shader->AddVertexConstantLayout(CONSTANT_TYPE_VECTOR4,
											shader->GetUniformId(material.GetUniformName(ui)),
											parameterGenerator.VertexUserUniformOffset + sizeof(float) * 4 * ui);
		}

		shader->SetVertexConstantBufferSize(parameterGenerator.VertexShaderUniformBufferSize);

		shader->AddPixelConstantLayout(
			CONSTANT_TYPE_VECTOR4, shader->GetUniformId("mUVInversedBack"), parameterGenerator.PixelInversedFlagOffset);

		shader->AddPixelConstantLayout(
			CONSTANT_TYPE_VECTOR4, shader->GetUniformId("predefined_uniform"), parameterGenerator.PixelPredefinedOffset);

		shader->AddPixelConstantLayout(
			CONSTANT_TYPE_VECTOR4, shader->GetUniformId("cameraPosition"), parameterGenerator.PixelCameraPositionOffset);

		// shiding model
		if (material.GetShadingModel() == ::Effekseer::ShadingModelType::Lit)
		{
			shader->AddPixelConstantLayout(
				CONSTANT_TYPE_VECTOR4, shader->GetUniformId("lightDirection"), parameterGenerator.PixelLightDirectionOffset);
			shader->AddPixelConstantLayout(
				CONSTANT_TYPE_VECTOR4, shader->GetUniformId("lightColor"), parameterGenerator.PixelLightColorOffset);
			shader->AddPixelConstantLayout(
				CONSTANT_TYPE_VECTOR4, shader->GetUniformId("lightAmbientColor"), parameterGenerator.PixelLightAmbientColorOffset);
		}
		else if (material.GetShadingModel() == ::Effekseer::ShadingModelType::Unlit)
		{
		}

		if (material.GetHasRefraction() && st == 1)
		{
			shader->AddPixelConstantLayout(
				CONSTANT_TYPE_MATRIX44, shader->GetUniformId("cameraMat"), parameterGenerator.PixelCameraMatrixOffset);
		}

		for (int32_t ui = 0; ui < material.GetUniformCount(); ui++)
		{
			shader->AddPixelConstantLayout(CONSTANT_TYPE_VECTOR4,
										   shader->GetUniformId(material.GetUniformName(ui)),
										   parameterGenerator.PixelUserUniformOffset + sizeof(float) * 4 * ui);
		}

		shader->SetPixelConstantBufferSize(parameterGenerator.PixelShaderUniformBufferSize);

		int32_t lastIndex = -1;
		for (int32_t ti = 0; ti < material.GetTextureCount(); ti++)
		{
			shader->SetTextureSlot(material.GetTextureIndex(ti), shader->GetUniformId(material.GetTextureName(ti)));
			lastIndex = Effekseer::Max(lastIndex, material.GetTextureIndex(ti));
		}

		lastIndex += 1;
		shader->SetTextureSlot(lastIndex, shader->GetUniformId("background"));

		if (st == 0)
		{
			materialData->ModelUserPtr = shader;
		}
		else
		{
			materialData->RefractionModelUserPtr = shader;
		}
	}

	materialData->CustomData1 = material.GetCustomData1Count();
	materialData->CustomData2 = material.GetCustomData2Count();
	materialData->TextureCount = std::min(material.GetTextureCount(), Effekseer::UserTextureSlotMax);
	materialData->UniformCount = material.GetUniformCount();
	materialData->ShadingModel = material.GetShadingModel();

	for (int32_t i = 0; i < materialData->TextureCount; i++)
	{
		materialData->TextureWrapTypes.at(i) = material.GetTextureWrap(i);
	}

	return materialData;
}

MaterialLoader::MaterialLoader(OpenGLDeviceType deviceType,
							   Renderer* renderer,
							   DeviceObjectCollection* deviceObjectCollection,
							   ::Effekseer::FileInterface* fileInterface,
							   bool canLoadFromCache)
	: fileInterface_(fileInterface)
	, canLoadFromCache_(canLoadFromCache)
{
	if (fileInterface == nullptr)
	{
		fileInterface_ = &defaultFileInterface_;
	}

	deviceType_ = deviceType;

	renderer_ = renderer;
	ES_SAFE_ADDREF(renderer_);

	deviceObjectCollection_ = deviceObjectCollection;
	ES_SAFE_ADDREF(deviceObjectCollection_);
}

MaterialLoader ::~MaterialLoader()
{
	ES_SAFE_RELEASE(renderer_);
	ES_SAFE_RELEASE(deviceObjectCollection_);
}

::Effekseer::MaterialData* MaterialLoader::Load(const EFK_CHAR* path)
{
	// code file
	if (canLoadFromCache_)
	{
		auto binaryPath = std::u16string(path) + u"d";
		std::unique_ptr<Effekseer::FileReader> reader(fileInterface_->TryOpenRead(binaryPath.c_str()));

		if (reader.get() != nullptr)
		{
			size_t size = reader->GetLength();
			std::vector<char> data;
			data.resize(size);
			reader->Read(data.data(), size);

			auto material = Load(data.data(), (int32_t)size, ::Effekseer::MaterialFileType::Compiled);

			if (material != nullptr)
			{
				return material;
			}
		}
	}

	// code file
	{
		std::unique_ptr<Effekseer::FileReader> reader(fileInterface_->OpenRead(path));

		if (reader.get() != nullptr)
		{
			size_t size = reader->GetLength();
			std::vector<char> data;
			data.resize(size);
			reader->Read(data.data(), size);

			auto material = Load(data.data(), (int32_t)size, ::Effekseer::MaterialFileType::Code);

			return material;
		}
	}

	return nullptr;
}

::Effekseer::MaterialData* MaterialLoader::Load(const void* data, int32_t size, Effekseer::MaterialFileType fileType)
{
	if (fileType == Effekseer::MaterialFileType::Compiled)
	{
		auto compiled = Effekseer::CompiledMaterial();
		if (!compiled.Load(static_cast<const uint8_t*>(data), size))
		{
			return nullptr;
		}

		if (!compiled.GetHasValue(::Effekseer::CompiledMaterialPlatformType::OpenGL))
		{
			return nullptr;
		}

		// compiled
		Effekseer::Material material;
		material.Load((const uint8_t*)compiled.GetOriginalData().data(), static_cast<int32_t>(compiled.GetOriginalData().size()));
		auto binary = compiled.GetBinary(::Effekseer::CompiledMaterialPlatformType::OpenGL);

		return LoadAcutually(material, binary);
	}
	else
	{
		Effekseer::Material material;
		if (!material.Load((const uint8_t*)data, size))
		{
			std::cout << "Error : Invalid material is loaded." << std::endl;
		}
		auto compiler = ::Effekseer::CreateUniqueReference(new Effekseer::MaterialCompilerGL());
		auto binary = ::Effekseer::CreateUniqueReference(compiler->Compile(&material));

		return LoadAcutually(material, binary.get());
	}
}

void MaterialLoader::Unload(::Effekseer::MaterialData* data)
{
	if (data == nullptr)
		return;
	auto shader = reinterpret_cast<Shader*>(data->UserPtr);
	auto modelShader = reinterpret_cast<Shader*>(data->ModelUserPtr);
	auto refractionShader = reinterpret_cast<Shader*>(data->RefractionUserPtr);
	auto refractionModelShader = reinterpret_cast<Shader*>(data->RefractionModelUserPtr);

	ES_SAFE_DELETE(shader);
	ES_SAFE_DELETE(modelShader);
	ES_SAFE_DELETE(refractionShader);
	ES_SAFE_DELETE(refractionModelShader);
	ES_SAFE_DELETE(data);
}

} // namespace EffekseerRendererGL

//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------




#ifdef __EFFEKSEER_RENDERER_INTERNAL_LOADER__
#endif

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace EffekseerRendererGL
{

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
static const char g_sprite_vs_src[] =
	R"(
IN vec4 atPosition;
IN vec4 atColor;
IN vec4 atTexCoord;
)"

	R"(
CENTROID OUT vec4 vaColor;
CENTROID OUT vec4 vaTexCoord;
OUT vec4 vaPos;
OUT vec4 vaPosR;
OUT vec4 vaPosU;
)"

	R"(
uniform mat4 uMatCamera;
uniform mat4 uMatProjection;
uniform vec4 mUVInversed;

void main() {
	vec4 cameraPos = uMatCamera * atPosition;
	cameraPos = cameraPos / cameraPos.w;

	gl_Position = uMatProjection * cameraPos;

	vaPos = gl_Position;

	vec4 cameraPosU = cameraPos + vec4(0.0, 1.0, 0.0, 0.0);
	vec4 cameraPosR = cameraPos + vec4(1.0, 0.0, 0.0, 0.0);

	vaPosR = uMatProjection * cameraPosR;
	vaPosU = uMatProjection * cameraPosU;
	
	vaPos = vaPos / vaPos.w;
	vaPosR = vaPosR / vaPosR.w;
	vaPosU = vaPosU / vaPosU.w;

	vaColor = atColor;
	vaTexCoord = atTexCoord;

	vaTexCoord.y = mUVInversed.x + mUVInversed.y * vaTexCoord.y;
}

)";

static const char g_sprite_fs_texture_src[] =
	R"(
CENTROID IN lowp vec4 vaColor;
CENTROID IN mediump vec4 vaTexCoord;

uniform sampler2D uTexture0;

void main()
{
	FRAGCOLOR = vaColor * TEX2D(uTexture0, vaTexCoord.xy);

	if(FRAGCOLOR.w <= 0.0) discard;
}
)";

static const char g_sprite_fs_no_texture_src[] =
	R"(
CENTROID IN lowp vec4 vaColor;
CENTROID IN mediump vec4 vaTexCoord;

void main()
{
	FRAGCOLOR = vaColor;

	if(FRAGCOLOR.w <= 0.0) discard;
}
)";

static const char g_sprite_distortion_vs_src[] =
	R"(
IN vec4 atPosition;
IN vec4 atColor;
IN vec4 atTexCoord;
IN vec4 atBinormal;
IN vec4 atTangent;

)"

	R"(
CENTROID OUT vec4 vaColor;
CENTROID OUT vec4 vaTexCoord;
OUT vec4 vaPos;
OUT vec4 vaPosR;
OUT vec4 vaPosU;
)"
	R"(
uniform mat4 uMatCamera;
uniform mat4 uMatProjection;
uniform vec4 mUVInversed;

void main() {

	vec4 localBinormal = vec4( atPosition.x + atBinormal.x, atPosition.y + atBinormal.y, atPosition.z + atBinormal.z, 1.0 );
	vec4 localTangent = vec4( atPosition.x + atTangent.x, atPosition.y + atTangent.y, atPosition.z + atTangent.z, 1.0 );
	localBinormal = uMatCamera * localBinormal;
	localTangent = uMatCamera * localTangent;

	vec4 cameraPos = uMatCamera * atPosition;
	cameraPos = cameraPos / cameraPos.w;

	localBinormal = localBinormal / localBinormal.w;
	localTangent = localTangent / localTangent.w;

	localBinormal = cameraPos + normalize(localBinormal - cameraPos);
	localTangent = cameraPos + normalize(localTangent - cameraPos);

	gl_Position = uMatProjection * cameraPos;

	vaPos = gl_Position;


	vaPosR = uMatProjection * localTangent;
	vaPosU = uMatProjection * localBinormal;
	
	vaPos = vaPos / vaPos.w;
	vaPosR = vaPosR / vaPosR.w;
	vaPosU = vaPosU / vaPosU.w;

	vaColor = atColor;
	vaTexCoord = atTexCoord;

	vaTexCoord.y = mUVInversed.x + mUVInversed.y * vaTexCoord.y;
}

)";

static const char g_sprite_fs_texture_distortion_src[] =
	R"(
CENTROID IN lowp vec4 vaColor;
CENTROID IN mediump vec4 vaTexCoord;
IN mediump vec4 vaPos;
IN mediump vec4 vaPosR;
IN mediump vec4 vaPosU;
)"

	R"(
uniform sampler2D uTexture0;
uniform sampler2D uBackTexture0;

uniform	vec4	g_scale;
uniform	vec4	mUVInversedBack;

)"

	R"(
void main() {
	vec4 color = TEX2D(uTexture0, vaTexCoord.xy);
	color.w = color.w * vaColor.w;

	vec2 pos = vaPos.xy / vaPos.w;
	vec2 posU = vaPosU.xy / vaPosU.w;
	vec2 posR = vaPosR.xy / vaPosR.w;

	vec2 uv = pos + (posR - pos) * (color.x * 2.0 - 1.0) * vaColor.x * g_scale.x + (posU - pos) * (color.y * 2.0 - 1.0) * vaColor.y * g_scale.x;
	uv.x = (uv.x + 1.0) * 0.5;
	uv.y = (uv.y + 1.0) * 0.5;
	//uv.y = 1.0 - (uv.y + 1.0) * 0.5;

	uv.y = mUVInversedBack.x + mUVInversedBack.y * uv.y;

	color.xyz = TEX2D(uBackTexture0, uv).xyz;
	
	FRAGCOLOR = color;

	if(FRAGCOLOR.w <= 0.0) discard;
}
)";

static const char g_sprite_fs_no_texture_distortion_src[] =
	R"(
CENTROID IN lowp vec4 vaColor;
CENTROID IN mediump vec4 vaTexCoord;
IN mediump vec4 vaPos;
IN mediump vec4 vaPosR;
IN mediump vec4 vaPosU;
)"

	R"(
uniform sampler2D uBackTexture0;

uniform	vec4	g_scale;
uniform	vec4	mUVInversedBack;

)"

	R"(
void main() {
	vec4 color = vaColor;
	color.xyz = vec3(1.0,1.0,1.0);

	vec2 pos = vaPos.xy / vaPos.w;
	vec2 posU = vaPosU.xy / vaPosU.w;
	vec2 posR = vaPosR.xy / vaPosR.w;

	vec2 uv = pos + (posR - pos) * (color.x * 2.0 - 1.0) * vaColor.x * g_scale.x + (posU - pos) * (color.y * 2.0 - 1.0) * vaColor.y * g_scale.x;
	uv.x = (uv.x + 1.0) * 0.5;
	uv.y = (uv.y + 1.0) * 0.5;
	//uv.y = 1.0 - (uv.y + 1.0) * 0.5;

	uv.y = mUVInversedBack.x + mUVInversedBack.y * uv.y;

	color.xyz = TEX2D(uBackTexture0, uv).xyz;
	
	FRAGCOLOR = color;

	if(FRAGCOLOR.w <= 0.0) discard;
}
)";

static const char g_sprite_vs_lighting_src[] =
	R"(
IN vec4 atPosition;
IN vec4 atColor;
IN vec3 atNormal;
IN vec3 atTangent;
IN vec2 atTexCoord;
IN vec2 atTexCoord2;
)"

	R"(
CENTROID OUT lowp vec4 v_VColor;
CENTROID OUT mediump vec2 v_UV1;
OUT mediump vec2 v_UV2;
OUT mediump vec3 v_WorldP;
OUT mediump vec3 v_WorldN;
OUT mediump vec3 v_WorldT;
OUT mediump vec3 v_WorldB;
OUT mediump vec2 v_ScreenUV;
)"

	R"(
uniform mat4 uMatCamera;
uniform mat4 uMatProjection;
uniform vec4 mUVInversed;

)"

	R"(
void main() {
	vec3 worldPos = atPosition.xyz;

	// UV
	vec2 uv1 = atTexCoord.xy;
	uv1.y = mUVInversed.x + mUVInversed.y * uv1.y;
	vec2 uv2 = atTexCoord2.xy;
	uv2.y = mUVInversed.x + mUVInversed.y * uv2.y;

	// NBT
	vec3 worldNormal = (atNormal - vec3(0.5, 0.5, 0.5)) * 2.0;
	vec3 worldTangent = (atTangent - vec3(0.5, 0.5, 0.5)) * 2.0;
	vec3 worldBinormal = cross(worldNormal, worldTangent);

	v_WorldN = worldNormal;
	v_WorldB = worldBinormal;
	v_WorldT = worldTangent;
	vec3 pixelNormalDir = vec3(0.5, 0.5, 1.0);

	vec4 cameraPos = uMatCamera * vec4(worldPos, 1.0);
	cameraPos = cameraPos / cameraPos.w;

	gl_Position = uMatProjection * cameraPos;

	v_WorldP = worldPos;
	v_VColor = atColor;

	v_UV1 = uv1;
	v_UV2 = uv2;
	v_ScreenUV.xy = gl_Position.xy / gl_Position.w;
	v_ScreenUV.xy = vec2(v_ScreenUV.x + 1.0, v_ScreenUV.y + 1.0) * 0.5;
}

)";

static const char g_sprite_fs_lighting_src[] =
	R"(

CENTROID IN lowp vec4 v_VColor;
CENTROID IN mediump vec2 v_UV1;
IN mediump vec2 v_UV2;
IN mediump vec3 v_WorldP;
IN mediump vec3 v_WorldN;
IN mediump vec3 v_WorldT;
IN mediump vec3 v_WorldB;
IN mediump vec2 v_ScreenUV;

uniform sampler2D ColorTexture;
uniform sampler2D NormalTexture;
uniform vec4 LightDirection;
uniform vec4 LightColor;
uniform vec4 LightAmbient;

void main()
{
	vec3 texNormal = (TEX2D(NormalTexture, v_UV1.xy).xyz - 0.5) * 2.0;
	mat3 normalMatrix = mat3(v_WorldT.xyz, v_WorldB.xyz, v_WorldN.xyz );
	vec3 localNormal = normalize( normalMatrix * texNormal );
	float diffuse = max(0.0, dot(localNormal, LightDirection.xyz));
	
	FRAGCOLOR = v_VColor * TEX2D(ColorTexture, v_UV1.xy);
	FRAGCOLOR.xyz = FRAGCOLOR.xyz * (LightColor.xyz * diffuse + LightAmbient.xyz);

	if(FRAGCOLOR.w <= 0.0) discard;
}


)";

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
::Effekseer::TextureLoader* CreateTextureLoader(::Effekseer::FileInterface* fileInterface, ::Effekseer::ColorSpaceType colorSpaceType)
{
#ifdef __EFFEKSEER_RENDERER_INTERNAL_LOADER__
	return new TextureLoader(fileInterface, colorSpaceType);
#else
	return NULL;
#endif
}

::Effekseer::ModelLoader* CreateModelLoader(::Effekseer::FileInterface* fileInterface)
{
#ifdef __EFFEKSEER_RENDERER_INTERNAL_LOADER__
	return new ModelLoader(fileInterface);
#else
	return NULL;
#endif
}

Renderer* Renderer::Create(int32_t squareMaxCount, OpenGLDeviceType deviceType, DeviceObjectCollection* deviceObjectCollection)
{
	GLExt::Initialize(deviceType);

	RendererImplemented* renderer = new RendererImplemented(squareMaxCount, deviceType, deviceObjectCollection);
	if (renderer->Initialize())
	{
		return renderer;
	}
	return NULL;
}

int32_t RendererImplemented::GetIndexSpriteCount() const
{
	int vsSize = EffekseerRenderer::GetMaximumVertexSizeInAllTypes() * m_squareMaxCount * 4;
	return (vsSize / sizeof(Vertex) / 4 + 1);
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
RendererImplemented::RendererImplemented(int32_t squareMaxCount,
										 OpenGLDeviceType deviceType,
										 DeviceObjectCollection* deviceObjectCollection)
	: m_vertexBuffer(NULL)
	, m_indexBuffer(NULL)
	, m_indexBufferForWireframe(NULL)
	, m_squareMaxCount(squareMaxCount)
	, m_renderState(NULL)
	, m_restorationOfStates(true)
	, m_currentVertexArray(NULL)

	, m_shader(nullptr)
	, m_shader_distortion(nullptr)
	, m_standardRenderer(nullptr)

	, m_vao(nullptr)
	, m_vao_distortion(nullptr)
	, m_vao_wire_frame(nullptr)
	, m_distortingCallback(nullptr)

	, m_deviceType(deviceType)
	, deviceObjectCollection_(deviceObjectCollection)
{
	m_background.UserID = 0;
	m_background.HasMipmap = false;

	if (deviceObjectCollection == nullptr)
	{
		deviceObjectCollection_ = new DeviceObjectCollection();
	}
	else
	{
		ES_SAFE_ADDREF(deviceObjectCollection_);
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
RendererImplemented::~RendererImplemented()
{
	GetImpl()->DeleteProxyTextures(this);

	ES_SAFE_DELETE(m_distortingCallback);

	ES_SAFE_DELETE(m_standardRenderer);
	ES_SAFE_DELETE(m_shader);
	ES_SAFE_DELETE(m_shader_distortion);
	ES_SAFE_DELETE(m_shader_lighting);

	auto isVaoEnabled = m_vao != nullptr;

	ES_SAFE_DELETE(m_vao);
	ES_SAFE_DELETE(m_vao_distortion);
	ES_SAFE_DELETE(m_vao_wire_frame);
	ES_SAFE_DELETE(m_vao_lighting);

	ES_SAFE_DELETE(m_renderState);
	ES_SAFE_DELETE(m_vertexBuffer);
	ES_SAFE_DELETE(m_indexBuffer);
	ES_SAFE_DELETE(m_indexBufferForWireframe);

	ES_SAFE_RELEASE(deviceObjectCollection_);

	if (GLExt::IsSupportedVertexArray() && defaultVertexArray_ > 0)
	{
		GLExt::glDeleteVertexArrays(1, &defaultVertexArray_);
		defaultVertexArray_ = 0;
	}
}

void RendererImplemented::OnLostDevice()
{
	if (deviceObjectCollection_ != nullptr)
		deviceObjectCollection_->OnLostDevice();
}

void RendererImplemented::OnResetDevice()
{
	if (deviceObjectCollection_ != nullptr)
		deviceObjectCollection_->OnResetDevice();

	GenerateIndexData();
}

void RendererImplemented::GenerateIndexData()
{
	if (indexBufferStride_ == 2)
	{
		GenerateIndexDataStride<uint16_t>();
	}
	else if (indexBufferStride_ == 4)
	{
		GenerateIndexDataStride<uint32_t>();
	}
}

template <typename T>
void RendererImplemented::GenerateIndexDataStride()
{
	// generate an index buffer
	if (m_indexBuffer != nullptr)
	{
		m_indexBuffer->Lock();

		for (int i = 0; i < GetIndexSpriteCount(); i++)
		{
			std::array<T, 6> buf;
			buf[0] = (T)(3 + 4 * i);
			buf[1] = (T)(1 + 4 * i);
			buf[2] = (T)(0 + 4 * i);
			buf[3] = (T)(3 + 4 * i);
			buf[4] = (T)(0 + 4 * i);
			buf[5] = (T)(2 + 4 * i);
			memcpy(m_indexBuffer->GetBufferDirect(6), buf.data(), sizeof(T) * 6);
		}

		m_indexBuffer->Unlock();
	}

	// generate an index buffer for a wireframe
	if (m_indexBufferForWireframe != nullptr)
	{
		m_indexBufferForWireframe->Lock();

		for (int i = 0; i < GetIndexSpriteCount(); i++)
		{
			std::array<T, 8> buf;
			buf[0] = (T)(0 + 4 * i);
			buf[1] = (T)(1 + 4 * i);
			buf[2] = (T)(2 + 4 * i);
			buf[3] = (T)(3 + 4 * i);
			buf[4] = (T)(0 + 4 * i);
			buf[5] = (T)(2 + 4 * i);
			buf[6] = (T)(1 + 4 * i);
			buf[7] = (T)(3 + 4 * i);
			memcpy(m_indexBufferForWireframe->GetBufferDirect(8), buf.data(), sizeof(T) * 8);
		}

		m_indexBufferForWireframe->Unlock();
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
bool RendererImplemented::Initialize()
{
	GLint currentVAO = 0;

	if (GLExt::IsSupportedVertexArray())
	{
		glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &currentVAO);
	}

	int arrayBufferBinding = 0;
	int elementArrayBufferBinding = 0;
	glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &arrayBufferBinding);
	glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &elementArrayBufferBinding);

	if (GetIndexSpriteCount() * 4 > 65536)
	{
		indexBufferStride_ = 4;
	}

	SetSquareMaxCount(m_squareMaxCount);

	m_renderState = new RenderState(this);

	m_shader = Shader::Create(this->GetDeviceType(),
							  this->GetDeviceObjectCollection(),
							  g_sprite_vs_src,
							  sizeof(g_sprite_vs_src),
							  g_sprite_fs_texture_src,
							  sizeof(g_sprite_fs_texture_src),
							  "Standard Tex",
							  false);
	if (m_shader == nullptr)
		return false;

	m_shader_distortion = Shader::Create(this->GetDeviceType(),
										 this->GetDeviceObjectCollection(),
										 g_sprite_distortion_vs_src,
										 sizeof(g_sprite_distortion_vs_src),
										 g_sprite_fs_texture_distortion_src,
										 sizeof(g_sprite_fs_texture_distortion_src),
										 "Standard Distortion Tex",
										 false);
	if (m_shader_distortion == nullptr)
		return false;

	static ShaderAttribInfo sprite_attribs[3] = {
		{"atPosition", GL_FLOAT, 3, 0, false},
		{"atColor", GL_UNSIGNED_BYTE, 4, 12, true},
		{"atTexCoord", GL_FLOAT, 2, 16, false}};

	static ShaderAttribInfo sprite_attribs_distortion[5] = {
		{"atPosition", GL_FLOAT, 3, 0, false},
		{"atColor", GL_UNSIGNED_BYTE, 4, 12, true},
		{"atTexCoord", GL_FLOAT, 2, 16, false},
		{"atBinormal", GL_FLOAT, 3, 24, false},
		{"atTangent", GL_FLOAT, 3, 36, false},
	};

	// 頂点属性IDを取得
	m_shader->GetAttribIdList(3, sprite_attribs);
	m_shader->SetVertexSize(sizeof(Vertex));
	m_shader->SetVertexConstantBufferSize(sizeof(Effekseer::Matrix44) * 2 + sizeof(float) * 4);

	m_shader->AddVertexConstantLayout(
		CONSTANT_TYPE_MATRIX44,
		m_shader->GetUniformId("uMatCamera"),
		0);

	m_shader->AddVertexConstantLayout(
		CONSTANT_TYPE_MATRIX44,
		m_shader->GetUniformId("uMatProjection"),
		sizeof(Effekseer::Matrix44));

	m_shader->AddVertexConstantLayout(
		CONSTANT_TYPE_VECTOR4,
		m_shader->GetUniformId("mUVInversed"),
		sizeof(Effekseer::Matrix44) * 2);

	m_shader->SetTextureSlot(0, m_shader->GetUniformId("uTexture0"));

	m_vao = VertexArray::Create(this, m_shader, GetVertexBuffer(), GetIndexBuffer(), false);

	// Distortion
	m_shader_distortion->GetAttribIdList(5, sprite_attribs_distortion);
	m_shader_distortion->SetVertexSize(sizeof(VertexDistortion));
	m_shader_distortion->SetVertexConstantBufferSize(sizeof(Effekseer::Matrix44) * 2 + sizeof(float) * 4);
	m_shader_distortion->SetPixelConstantBufferSize(sizeof(float) * 4 + sizeof(float) * 4);

	m_shader_distortion->AddVertexConstantLayout(
		CONSTANT_TYPE_MATRIX44,
		m_shader_distortion->GetUniformId("uMatCamera"),
		0);

	m_shader_distortion->AddVertexConstantLayout(
		CONSTANT_TYPE_MATRIX44,
		m_shader_distortion->GetUniformId("uMatProjection"),
		sizeof(Effekseer::Matrix44));

	m_shader_distortion->AddVertexConstantLayout(
		CONSTANT_TYPE_VECTOR4,
		m_shader_distortion->GetUniformId("mUVInversed"),
		sizeof(Effekseer::Matrix44) * 2);

	m_shader_distortion->AddPixelConstantLayout(
		CONSTANT_TYPE_VECTOR4,
		m_shader_distortion->GetUniformId("g_scale"),
		0);

	m_shader_distortion->AddPixelConstantLayout(
		CONSTANT_TYPE_VECTOR4,
		m_shader_distortion->GetUniformId("mUVInversedBack"),
		sizeof(float) * 4);

	m_shader_distortion->SetTextureSlot(0, m_shader_distortion->GetUniformId("uTexture0"));
	m_shader_distortion->SetTextureSlot(1, m_shader_distortion->GetUniformId("uBackTexture0"));

	m_vao_distortion = VertexArray::Create(this, m_shader_distortion, GetVertexBuffer(), GetIndexBuffer(), false);

	// Lighting
	EffekseerRendererGL::ShaderAttribInfo sprite_attribs_lighting[6] = {
		{"atPosition", GL_FLOAT, 3, 0, false},
		{"atColor", GL_UNSIGNED_BYTE, 4, 12, true},
		{"atNormal", GL_UNSIGNED_BYTE, 4, 16, true},
		{"atTangent", GL_UNSIGNED_BYTE, 4, 20, true},
		{"atTexCoord", GL_FLOAT, 2, 24, false},
		{"atTexCoord2", GL_FLOAT, 2, 32, false},
	};

	m_shader_lighting = Shader::Create(this->GetDeviceType(),
									   this->GetDeviceObjectCollection(),
									   g_sprite_vs_lighting_src,
									   sizeof(g_sprite_vs_lighting_src),
									   g_sprite_fs_lighting_src,
									   sizeof(g_sprite_fs_lighting_src),
									   "Standard Lighting Tex",
									   false);

	m_shader_lighting->GetAttribIdList(5, sprite_attribs_lighting);
	m_shader_lighting->SetVertexSize(sizeof(EffekseerRenderer::DynamicVertex));
	m_shader_lighting->SetVertexConstantBufferSize(sizeof(Effekseer::Matrix44) * 2 + sizeof(float) * 4);
	m_shader_lighting->SetPixelConstantBufferSize(sizeof(float) * 4 * 3);

	m_shader_lighting->AddVertexConstantLayout(CONSTANT_TYPE_MATRIX44, m_shader_lighting->GetUniformId("uMatCamera"), 0);

	m_shader_lighting->AddVertexConstantLayout(
		CONSTANT_TYPE_MATRIX44, m_shader_lighting->GetUniformId("uMatProjection"), sizeof(Effekseer::Matrix44));

	m_shader_lighting->AddVertexConstantLayout(
		CONSTANT_TYPE_VECTOR4, m_shader_lighting->GetUniformId("mUVInversed"), sizeof(Effekseer::Matrix44) * 2);

	m_shader_lighting->AddPixelConstantLayout(
		CONSTANT_TYPE_VECTOR4, m_shader_lighting->GetUniformId("LightDirection"), sizeof(float[4]) * 0);
	m_shader_lighting->AddPixelConstantLayout(CONSTANT_TYPE_VECTOR4, m_shader_lighting->GetUniformId("LightColor"), sizeof(float[4]) * 1);
	m_shader_lighting->AddPixelConstantLayout(CONSTANT_TYPE_VECTOR4, m_shader_lighting->GetUniformId("LightAmbient"), sizeof(float[4]) * 2);

	m_shader_lighting->SetTextureSlot(0, m_shader_lighting->GetUniformId("ColorTexture"));
	m_shader_lighting->SetTextureSlot(1, m_shader_lighting->GetUniformId("NormalTexture"));

	m_vao_lighting = VertexArray::Create(this, m_shader_lighting, GetVertexBuffer(), GetIndexBuffer(), false);

	m_vao_wire_frame = VertexArray::Create(this, m_shader, GetVertexBuffer(), m_indexBufferForWireframe, false);

	m_standardRenderer = new EffekseerRenderer::StandardRenderer<RendererImplemented, Shader, Vertex, VertexDistortion>(this, m_shader, m_shader_distortion);

	GLExt::glBindBuffer(GL_ARRAY_BUFFER, arrayBufferBinding);
	GLExt::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementArrayBufferBinding);

	if (GLExt::IsSupportedVertexArray())
	{
		GLExt::glBindVertexArray(currentVAO);
	}

	GetImpl()->CreateProxyTextures(this);

	if (GLExt::IsSupportedVertexArray())
	{
		GLExt::glGenVertexArrays(1, &defaultVertexArray_);
	}

	return true;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void RendererImplemented::Destroy()
{
	Release();
}

void RendererImplemented::SetRestorationOfStatesFlag(bool flag)
{
	m_restorationOfStates = flag;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
bool RendererImplemented::BeginRendering()
{
	GLCheckError();

	impl->CalculateCameraProjectionMatrix();

	// store state
	if (m_restorationOfStates)
	{
		m_originalState.blend = glIsEnabled(GL_BLEND);
		m_originalState.cullFace = glIsEnabled(GL_CULL_FACE);
		m_originalState.depthTest = glIsEnabled(GL_DEPTH_TEST);

		if (GetDeviceType() == OpenGLDeviceType::OpenGL2)
		{
			m_originalState.texture = glIsEnabled(GL_TEXTURE_2D);
		}

		glGetBooleanv(GL_DEPTH_WRITEMASK, &m_originalState.depthWrite);
		glGetIntegerv(GL_DEPTH_FUNC, &m_originalState.depthFunc);
		glGetIntegerv(GL_CULL_FACE_MODE, &m_originalState.cullFaceMode);
		glGetIntegerv(GL_BLEND_SRC_RGB, &m_originalState.blendSrc);
		glGetIntegerv(GL_BLEND_DST_RGB, &m_originalState.blendDst);
		glGetIntegerv(GL_BLEND_EQUATION, &m_originalState.blendEquation);
		glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &m_originalState.arrayBufferBinding);
		glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &m_originalState.elementArrayBufferBinding);

		for (size_t i = 0; i < m_originalState.boundTextures.size(); i++)
		{
			GLint bound = 0;
			GLExt::glActiveTexture(GL_TEXTURE0 + i);
			glGetIntegerv(GL_TEXTURE_BINDING_2D, &bound);
			m_originalState.boundTextures[i] = bound;
		}

		if (GLExt::IsSupportedVertexArray())
		{
			glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &m_originalState.vao);
		}
	}

	glDepthFunc(GL_LEQUAL);
	glEnable(GL_BLEND);
	glDisable(GL_CULL_FACE);

	currentTextures_.clear();
	m_renderState->GetActiveState().Reset();
	m_renderState->Update(true);

	m_renderState->GetActiveState().TextureIDs.fill(0);

	// reset renderer
	m_standardRenderer->ResetAndRenderingIfRequired();

	GLCheckError();

	return true;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
bool RendererImplemented::EndRendering()
{
	GLCheckError();

	// reset renderer
	m_standardRenderer->ResetAndRenderingIfRequired();

	// restore states
	if (m_restorationOfStates)
	{
		if (GLExt::IsSupportedVertexArray())
		{
			GLExt::glBindVertexArray(m_originalState.vao);
		}

		for (size_t i = 0; i < m_originalState.boundTextures.size(); i++)
		{
			GLExt::glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, m_originalState.boundTextures[i]);
		}
		GLExt::glActiveTexture(GL_TEXTURE0);

		if (m_originalState.blend)
			glEnable(GL_BLEND);
		else
			glDisable(GL_BLEND);
		if (m_originalState.cullFace)
			glEnable(GL_CULL_FACE);
		else
			glDisable(GL_CULL_FACE);
		if (m_originalState.depthTest)
			glEnable(GL_DEPTH_TEST);
		else
			glDisable(GL_DEPTH_TEST);

		if (GetDeviceType() == OpenGLDeviceType::OpenGL2)
		{
			if (m_originalState.texture)
				glEnable(GL_TEXTURE_2D);
			else
				glDisable(GL_TEXTURE_2D);
		}

		glDepthFunc(m_originalState.depthFunc);
		glDepthMask(m_originalState.depthWrite);
		glCullFace(m_originalState.cullFaceMode);
		glBlendFunc(m_originalState.blendSrc, m_originalState.blendDst);
		GLExt::glBlendEquation(m_originalState.blendEquation);

		GLExt::glBindBuffer(GL_ARRAY_BUFFER, m_originalState.arrayBufferBinding);
		GLExt::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_originalState.elementArrayBufferBinding);

		if (GetDeviceType() == OpenGLDeviceType::OpenGL3 || GetDeviceType() == OpenGLDeviceType::OpenGLES3)
		{
			for (int32_t i = 0; i < 4; i++)
			{
				GLExt::glBindSampler(i, 0);
			}
		}
	}

	GLCheckError();

	return true;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
VertexBuffer* RendererImplemented::GetVertexBuffer()
{
	return m_vertexBuffer;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
IndexBuffer* RendererImplemented::GetIndexBuffer()
{
	return m_indexBuffer;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
int32_t RendererImplemented::GetSquareMaxCount() const
{
	return m_squareMaxCount;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void RendererImplemented::SetSquareMaxCount(int32_t count)
{
	int arrayBufferBinding = 0;
	int elementArrayBufferBinding = 0;
	glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &arrayBufferBinding);
	glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &elementArrayBufferBinding);

	m_squareMaxCount = count;

	if (m_vertexBuffer != nullptr)
		AddRef();
	if (m_indexBuffer != nullptr)
		AddRef();
	ES_SAFE_DELETE(m_vertexBuffer);
	ES_SAFE_DELETE(m_indexBuffer);

	// generate a vertex buffer
	{
		m_vertexBuffer =
			VertexBuffer::Create(this, EffekseerRenderer::GetMaximumVertexSizeInAllTypes() * m_squareMaxCount * 4, true, false);
		if (m_vertexBuffer == NULL)
			return;
	}

	// generate an index buffer
	{
		m_indexBuffer = IndexBuffer::Create(this, GetIndexSpriteCount() * 6, false, indexBufferStride_, false);
		if (m_indexBuffer == nullptr)
			return;
	}

	// generate an index buffer for a wireframe
	{
		m_indexBufferForWireframe = IndexBuffer::Create(this, GetIndexSpriteCount() * 8, false, indexBufferStride_, false);
		if (m_indexBufferForWireframe == nullptr)
			return;
	}

	// generate index data
	GenerateIndexData();

	GLExt::glBindBuffer(GL_ARRAY_BUFFER, arrayBufferBinding);
	GLExt::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementArrayBufferBinding);
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
::EffekseerRenderer::RenderStateBase* RendererImplemented::GetRenderState()
{
	return m_renderState;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
::Effekseer::SpriteRenderer* RendererImplemented::CreateSpriteRenderer()
{
	return new ::EffekseerRenderer::SpriteRendererBase<RendererImplemented, Vertex, VertexDistortion>(this);
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
::Effekseer::RibbonRenderer* RendererImplemented::CreateRibbonRenderer()
{
	return new ::EffekseerRenderer::RibbonRendererBase<RendererImplemented, Vertex, VertexDistortion>(this);
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
::Effekseer::RingRenderer* RendererImplemented::CreateRingRenderer()
{
	return new ::EffekseerRenderer::RingRendererBase<RendererImplemented, Vertex, VertexDistortion>(this);
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
::Effekseer::ModelRenderer* RendererImplemented::CreateModelRenderer()
{
	return ModelRenderer::Create(this);
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
::Effekseer::TrackRenderer* RendererImplemented::CreateTrackRenderer()
{
	return new ::EffekseerRenderer::TrackRendererBase<RendererImplemented, Vertex, VertexDistortion>(this);
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
::Effekseer::TextureLoader* RendererImplemented::CreateTextureLoader(::Effekseer::FileInterface* fileInterface)
{
#ifdef __EFFEKSEER_RENDERER_INTERNAL_LOADER__
	return new TextureLoader(fileInterface);
#else
	return NULL;
#endif
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
::Effekseer::ModelLoader* RendererImplemented::CreateModelLoader(::Effekseer::FileInterface* fileInterface)
{
#ifdef __EFFEKSEER_RENDERER_INTERNAL_LOADER__
	return new ModelLoader(fileInterface);
#else
	return NULL;
#endif
}

::Effekseer::MaterialLoader* RendererImplemented::CreateMaterialLoader(::Effekseer::FileInterface* fileInterface)
{
#ifdef __EFFEKSEER_RENDERER_INTERNAL_LOADER__
	return new MaterialLoader(this->GetDeviceType(), this, this->GetDeviceObjectCollection(), fileInterface);
#else
	return nullptr;
#endif
}

void RendererImplemented::SetBackground(GLuint background, bool hasMipmap)
{
	m_background.UserID = background;
	m_background.HasMipmap = hasMipmap;
}

void RendererImplemented::SetBackgroundTexture(::Effekseer::TextureData* textureData)
{
	m_background = *textureData;
}

EffekseerRenderer::DistortingCallback* RendererImplemented::GetDistortingCallback()
{
	return m_distortingCallback;
}

void RendererImplemented::SetDistortingCallback(EffekseerRenderer::DistortingCallback* callback)
{
	ES_SAFE_DELETE(m_distortingCallback);
	m_distortingCallback = callback;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void RendererImplemented::SetVertexBuffer(VertexBuffer* vertexBuffer, int32_t size)
{
	if (m_currentVertexArray == nullptr ||
		m_currentVertexArray->GetVertexBuffer() == nullptr)
	{
		GLExt::glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer->GetInterface());
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void RendererImplemented::SetVertexBuffer(GLuint vertexBuffer, int32_t size)
{
	if (m_currentVertexArray == nullptr || m_currentVertexArray->GetVertexBuffer() == nullptr)
	{
		GLExt::glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void RendererImplemented::SetIndexBuffer(IndexBuffer* indexBuffer)
{
	if (m_currentVertexArray == nullptr || m_currentVertexArray->GetIndexBuffer() == nullptr)
	{
		GLExt::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer->GetInterface());
		indexBufferCurrentStride_ = indexBuffer->GetStride();
	}
	else
	{
		indexBufferCurrentStride_ = m_currentVertexArray->GetIndexBuffer()->GetStride();
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void RendererImplemented::SetIndexBuffer(GLuint indexBuffer)
{
	if (m_currentVertexArray == nullptr || m_currentVertexArray->GetIndexBuffer() == nullptr)
	{
		GLExt::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
		indexBufferCurrentStride_ = 4;
	}
	else
	{
		indexBufferCurrentStride_ = m_currentVertexArray->GetIndexBuffer()->GetStride();
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void RendererImplemented::SetVertexArray(VertexArray* vertexArray)
{
	m_currentVertexArray = vertexArray;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void RendererImplemented::SetLayout(Shader* shader)
{
	GLCheckError();

	if (m_currentVertexArray == nullptr ||
		m_currentVertexArray->GetVertexBuffer() == nullptr)
	{
		shader->EnableAttribs();
		shader->SetVertex();
		GLCheckError();
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void RendererImplemented::DrawSprites(int32_t spriteCount, int32_t vertexOffset)
{
	GLCheckError();

	impl->drawcallCount++;
	impl->drawvertexCount += spriteCount * 4;

	GLsizei stride = GL_UNSIGNED_SHORT;
	if (indexBufferCurrentStride_ == 4)
	{
		stride = GL_UNSIGNED_INT;
	}

	if (GetRenderMode() == ::Effekseer::RenderMode::Normal)
	{
		glDrawElements(GL_TRIANGLES, spriteCount * 6, stride, reinterpret_cast<GLvoid*>(static_cast<size_t>(vertexOffset / 4 * 6 * indexBufferCurrentStride_)));
	}
	else if (GetRenderMode() == ::Effekseer::RenderMode::Wireframe)
	{
		glDrawElements(GL_LINES, spriteCount * 8, stride, reinterpret_cast<GLvoid*>(static_cast<size_t>(vertexOffset / 4 * 8 * indexBufferCurrentStride_)));
	}

	GLCheckError();
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void RendererImplemented::DrawPolygon(int32_t vertexCount, int32_t indexCount)
{
	GLCheckError();

	impl->drawcallCount++;
	impl->drawvertexCount += vertexCount;

	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, NULL);

	GLCheckError();
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
Shader* RendererImplemented::GetShader(bool useTexture, ::Effekseer::RendererMaterialType materialType) const
{
	if (materialType == ::Effekseer::RendererMaterialType::BackDistortion)
	{
		if (useTexture && GetRenderMode() == Effekseer::RenderMode::Normal)
		{
			return m_shader_distortion;
		}
		else
		{
			return m_shader_distortion;
		}
	}
	else if (materialType == ::Effekseer::RendererMaterialType::Lighting)
	{
		if (useTexture && GetRenderMode() == Effekseer::RenderMode::Normal)
		{
			return m_shader_lighting;
		}
		else
		{
			return m_shader_lighting;
		}
	}
	else
	{
		if (useTexture && GetRenderMode() == Effekseer::RenderMode::Normal)
		{
			return m_shader;
		}
		else
		{
			return m_shader;
		}
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void RendererImplemented::BeginShader(Shader* shader)
{
	GLCheckError();

	// change VAO with shader
	if (GetRenderMode() == ::Effekseer::RenderMode::Wireframe)
	{
		SetVertexArray(m_vao_wire_frame);
	}
	else if (shader == m_shader)
	{
		SetVertexArray(m_vao);
	}
	else if (shader == m_shader_distortion)
	{
		SetVertexArray(m_vao_distortion);
	}
	else if (shader == m_shader_lighting)
	{
		SetVertexArray(m_vao_lighting);
	}
	else if (m_currentVertexArray != nullptr)
	{
		SetVertexArray(m_currentVertexArray);
	}
	else
	{
		m_currentVertexArray = nullptr;

		if (defaultVertexArray_ > 0)
		{
			GLExt::glBindVertexArray(defaultVertexArray_);
		}
	}

	shader->BeginScene();

	if (m_currentVertexArray)
	{
		GLExt::glBindVertexArray(m_currentVertexArray->GetInterface());
	}

	assert(currentShader == nullptr);
	currentShader = shader;

	GLCheckError();
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void RendererImplemented::EndShader(Shader* shader)
{
	assert(currentShader == shader);
	currentShader = nullptr;

	GLCheckError();

	if (m_currentVertexArray)
	{
		if (m_currentVertexArray->GetVertexBuffer() == nullptr)
		{
			shader->DisableAttribs();
			GLCheckError();

			GLExt::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			GLCheckError();

			GLExt::glBindBuffer(GL_ARRAY_BUFFER, 0);
			GLCheckError();
		}

		GLExt::glBindVertexArray(0);
		GLCheckError();
		m_currentVertexArray = nullptr;
	}
	else
	{
		shader->DisableAttribs();
		GLCheckError();

		GLExt::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		GLCheckError();

		GLExt::glBindBuffer(GL_ARRAY_BUFFER, 0);
		GLCheckError();

		if (defaultVertexArray_ > 0)
		{
			GLExt::glBindVertexArray(0);
		}
	}

	shader->EndScene();
	GLCheckError();
}

void RendererImplemented::SetVertexBufferToShader(const void* data, int32_t size, int32_t dstOffset)
{
	assert(currentShader != nullptr);
	auto p = static_cast<uint8_t*>(currentShader->GetVertexConstantBuffer()) + dstOffset;
	memcpy(p, data, size);
}

void RendererImplemented::SetPixelBufferToShader(const void* data, int32_t size, int32_t dstOffset)
{
	assert(currentShader != nullptr);
	auto p = static_cast<uint8_t*>(currentShader->GetPixelConstantBuffer()) + dstOffset;
	memcpy(p, data, size);
}

void RendererImplemented::SetTextures(Shader* shader, Effekseer::TextureData** textures, int32_t count)
{
	GLCheckError();

	for (int i = count; i < currentTextures_.size(); i++)
	{
		m_renderState->GetActiveState().TextureIDs[i] = 0;
	}

	currentTextures_.resize(count);

	for (int32_t i = 0; i < count; i++)
	{
		GLuint id = 0;
		if (textures[i] != nullptr)
		{
			id = (GLuint)textures[i]->UserID;
		}

		GLExt::glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, id);

		if (textures[i] != nullptr)
		{
			m_renderState->GetActiveState().TextureIDs[i] = textures[i]->UserID;
			currentTextures_[i] = *textures[i];
		}
		else
		{
			currentTextures_[i].UserID = 0;
			currentTextures_[i].UserPtr = nullptr;
			m_renderState->GetActiveState().TextureIDs[i] = 0;
		}

		if (shader->GetTextureSlotEnable(i))
		{
			GLExt::glUniform1i(shader->GetTextureSlot(i), i);
		}
	}
	GLExt::glActiveTexture(GL_TEXTURE0);

	GLCheckError();
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void RendererImplemented::ResetRenderState()
{
	m_renderState->GetActiveState().Reset();
	m_renderState->Update(true);
}

Effekseer::TextureData* RendererImplemented::CreateProxyTexture(EffekseerRenderer::ProxyTextureType type)
{

	GLint bound = 0;
	GLExt::glActiveTexture(GL_TEXTURE0);
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &bound);

	std::array<uint8_t, 4> buf;

	if (type == EffekseerRenderer::ProxyTextureType::White)
	{
		buf.fill(255);
	}
	else if (type == EffekseerRenderer::ProxyTextureType::Normal)
	{
		buf.fill(127);
		buf[2] = 255;
		buf[3] = 255;
	}
	else
	{
		assert(0);
	}

	GLuint texture = 0;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D,
				 0,
				 GL_RGBA,
				 1,
				 1,
				 0,
				 GL_RGBA,
				 GL_UNSIGNED_BYTE,
				 buf.data());

	// Generate mipmap
	GLExt::glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, bound);

	auto textureData = new Effekseer::TextureData();
	textureData->UserPtr = nullptr;
	textureData->UserID = texture;
	textureData->TextureFormat = Effekseer::TextureFormatType::ABGR8;
	textureData->Width = 1;
	textureData->Height = 1;
	return textureData;
}

void RendererImplemented::DeleteProxyTexture(Effekseer::TextureData* data)
{
	if (data != nullptr)
	{
		GLuint texture = (GLuint)data->UserID;
		glDeleteTextures(1, &texture);
		delete data;
	}
}

bool RendererImplemented::IsVertexArrayObjectSupported() const
{
	return GLExt::IsSupportedVertexArray();
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------

Model::InternalModel::InternalModel()
{
	VertexBuffer = 0;
	IndexBuffer = 0;
}

Model::InternalModel::~InternalModel()
{
	if (VertexBuffer > 0)
	{
		GLExt::glDeleteBuffers(1, &IndexBuffer);
	}

	if (IndexBuffer > 0)
	{
		GLExt::glDeleteBuffers(1, &VertexBuffer);
	}
}

bool Model::InternalModel::TryDelayLoad()
{
	if (VertexBuffer > 0)
		return false;

	int arrayBufferBinding = 0;
	int elementArrayBufferBinding = 0;
	glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &arrayBufferBinding);
	glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &elementArrayBufferBinding);

	GLExt::glGenBuffers(1, &VertexBuffer);
	if (VertexBuffer > 0)
	{
		GLExt::glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
		GLExt::glBufferData(GL_ARRAY_BUFFER, delayVertexBuffer.size(), delayVertexBuffer.data(), GL_STATIC_DRAW);
	}

	GLExt::glGenBuffers(1, &IndexBuffer);
	if (IndexBuffer > 0)
	{
		GLExt::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBuffer);
		GLExt::glBufferData(GL_ELEMENT_ARRAY_BUFFER, delayIndexBuffer.size(), delayIndexBuffer.data(), GL_STATIC_DRAW);
	}

	GLExt::glBindBuffer(GL_ARRAY_BUFFER, arrayBufferBinding);
	GLExt::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementArrayBufferBinding);

	return true;
}

Model::Model(void* data, int32_t size)
	: ::Effekseer::Model(data, size)
	, InternalModels(nullptr)
	, ModelCount(1)
{
	InternalModels = new Model::InternalModel[GetFrameCount()];

	for (int32_t f = 0; f < GetFrameCount(); f++)
	{
		auto vertexData = GetVertexes(f);
		auto vertexCount = GetVertexCount(f);
		auto faceData = GetFaces(f);
		auto faceCount = GetFaceCount(f);

		InternalModels[f].VertexCount = vertexCount;
		InternalModels[f].IndexCount = faceCount * 3;

		//GLExt::glGenBuffers(1, &InternalModels[f].VertexBuffer);
		size_t vertexSize = vertexCount * sizeof(::Effekseer::Model::Vertex);

		//int arrayBufferBinding = 0;
		//int elementArrayBufferBinding = 0;
		//glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &arrayBufferBinding);
		//glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &elementArrayBufferBinding);

		//if (InternalModels[f].VertexBuffer > 0)
		//{
		//	GLExt::glBindBuffer(GL_ARRAY_BUFFER, InternalModels[f].VertexBuffer);
		//	GLExt::glBufferData(GL_ARRAY_BUFFER, vertexSize, vertexData, GL_STATIC_DRAW);
		//}
		InternalModels[f].delayVertexBuffer.resize(vertexSize);
		memcpy(InternalModels[f].delayVertexBuffer.data(), vertexData, vertexSize);

		//GLExt::glGenBuffers(1, &InternalModels[f].IndexBuffer);
		size_t indexSize = faceCount * sizeof(::Effekseer::Model::Face);

		//if (InternalModels[f].IndexBuffer > 0)
		//{
		//	GLExt::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, InternalModels[f].IndexBuffer);
		//	GLExt::glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexSize, faceData, GL_STATIC_DRAW);
		//}

		//GLExt::glBindBuffer(GL_ARRAY_BUFFER, arrayBufferBinding);
		//GLExt::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementArrayBufferBinding);

		InternalModels[f].delayIndexBuffer.resize(indexSize);
		memcpy(InternalModels[f].delayIndexBuffer.data(), faceData, indexSize);
	}
}

Model::~Model()
{
	ES_SAFE_DELETE_ARRAY(InternalModels);
}

bool Model::LoadToGPU()
{
	if (IsLoadedOnGPU)
	{
		return false;
	}

	for (int32_t f = 0; f < GetFrameCount(); f++)
	{
		if (!InternalModels[f].TryDelayLoad())
		{
			return false;
		}
	}

	IsLoadedOnGPU = true;

	return true;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
} // namespace EffekseerRendererGL
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------


//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------



//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
namespace EffekseerRendererGL
{
//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
RenderState::RenderState(RendererImplemented* renderer)
	: m_renderer(renderer)
{
	if (m_renderer->GetDeviceType() == OpenGLDeviceType::OpenGL3 || m_renderer->GetDeviceType() == OpenGLDeviceType::OpenGLES3)
	{
		GLExt::glGenSamplers(Effekseer::TextureSlotMax, m_samplers.data());
	}

	GLint frontFace = 0;
	glGetIntegerv(GL_FRONT_FACE, &frontFace);

	if (GL_CW == frontFace)
	{
		m_isCCW = false;
	}
}

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
RenderState::~RenderState()
{
	if (m_renderer->GetDeviceType() == OpenGLDeviceType::OpenGL3 || m_renderer->GetDeviceType() == OpenGLDeviceType::OpenGLES3)
	{
		GLExt::glDeleteSamplers(Effekseer::TextureSlotMax, m_samplers.data());
	}
}

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
void RenderState::Update(bool forced)
{
	GLCheckError();

	if (m_active.DepthTest != m_next.DepthTest || forced)
	{
		if (m_next.DepthTest)
		{
			glEnable(GL_DEPTH_TEST);
		}
		else
		{
			glDisable(GL_DEPTH_TEST);
		}
	}

	GLCheckError();

	if (m_active.DepthWrite != m_next.DepthWrite || forced)
	{
		glDepthMask(m_next.DepthWrite);
	}

	GLCheckError();

	if (m_active.CullingType != m_next.CullingType || forced)
	{
		if (m_isCCW)
		{
			if (m_next.CullingType == Effekseer::CullingType::Front)
			{
				glEnable(GL_CULL_FACE);
				glCullFace(GL_FRONT);
			}
			else if (m_next.CullingType == Effekseer::CullingType::Back)
			{
				glEnable(GL_CULL_FACE);
				glCullFace(GL_BACK);
			}
			else if (m_next.CullingType == Effekseer::CullingType::Double)
			{
				glDisable(GL_CULL_FACE);
				glCullFace(GL_FRONT_AND_BACK);
			}
		}
		else
		{
			if (m_next.CullingType == Effekseer::CullingType::Front)
			{
				glEnable(GL_CULL_FACE);
				glCullFace(GL_BACK);
			}
			else if (m_next.CullingType == Effekseer::CullingType::Back)
			{
				glEnable(GL_CULL_FACE);
				glCullFace(GL_FRONT);
			}
			else if (m_next.CullingType == Effekseer::CullingType::Double)
			{
				glDisable(GL_CULL_FACE);
				glCullFace(GL_FRONT_AND_BACK);
			}
		}
	}

	GLCheckError();

	if (m_active.AlphaBlend != m_next.AlphaBlend || forced)
	{
		{
			glEnable(GL_BLEND);

			if (m_next.AlphaBlend == ::Effekseer::AlphaBlendType::Opacity ||
				m_renderer->GetRenderMode() == ::Effekseer::RenderMode::Wireframe)
			{
				GLExt::glBlendEquationSeparate(GL_FUNC_ADD, GL_MAX);
				GLExt::glBlendFuncSeparate(GL_ONE, GL_ZERO, GL_ONE, GL_ONE);
			}
			else if (m_next.AlphaBlend == ::Effekseer::AlphaBlendType::Sub)
			{
				GLExt::glBlendEquationSeparate(GL_FUNC_REVERSE_SUBTRACT, GL_FUNC_ADD);
				GLExt::glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE, GL_ZERO, GL_ONE);
			}
			else
			{
				GLExt::glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
				if (m_next.AlphaBlend == ::Effekseer::AlphaBlendType::Blend)
				{
					GLExt::glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE);
				}
				else if (m_next.AlphaBlend == ::Effekseer::AlphaBlendType::Add)
				{
					GLExt::glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE, GL_ONE, GL_ONE);
				}
				else if (m_next.AlphaBlend == ::Effekseer::AlphaBlendType::Mul)
				{
					GLExt::glBlendFuncSeparate(GL_ZERO, GL_SRC_COLOR, GL_ZERO, GL_ONE);
				}
			}
		}
	}

	GLCheckError();

	static const GLint glfilterMin[] = {GL_NEAREST, GL_LINEAR_MIPMAP_LINEAR};
	static const GLint glfilterMin_NoneMipmap[] = {GL_NEAREST, GL_LINEAR};
	static const GLint glfilterMag[] = {GL_NEAREST, GL_LINEAR};
	static const GLint glwrap[] = {GL_REPEAT, GL_CLAMP_TO_EDGE};

	if (m_renderer->GetDeviceType() == OpenGLDeviceType::OpenGL3 || m_renderer->GetDeviceType() == OpenGLDeviceType::OpenGLES3)
	{
		for (int32_t i = 0; i < (int32_t)m_renderer->GetCurrentTextures().size(); i++)
		{
			// If a texture is not assigned, skip it.
			if (m_renderer->GetCurrentTextures()[i].UserID == 0)
				continue;

			if (m_active.TextureFilterTypes[i] != m_next.TextureFilterTypes[i] || forced || m_active.TextureIDs[i] != m_next.TextureIDs[i])
			{
				GLExt::glActiveTexture(GL_TEXTURE0 + i);

				// for webngl
#ifndef NDEBUG
				//GLint bound = 0;
				//glGetIntegerv(GL_TEXTURE_BINDING_2D, &bound);
				//assert(bound > 0);
#endif

				int32_t filter_ = (int32_t)m_next.TextureFilterTypes[i];

				GLExt::glSamplerParameteri(m_samplers[i], GL_TEXTURE_MAG_FILTER, glfilterMag[filter_]);

				if (m_renderer->GetCurrentTextures()[i].HasMipmap)
				{
					GLExt::glSamplerParameteri(m_samplers[i], GL_TEXTURE_MIN_FILTER, glfilterMin[filter_]);
				}
				else
				{
					GLExt::glSamplerParameteri(m_samplers[i], GL_TEXTURE_MIN_FILTER, glfilterMin_NoneMipmap[filter_]);
				}

				//glSamplerParameteri( m_samplers[i],  GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				//glSamplerParameteri( m_samplers[i],  GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

				GLExt::glBindSampler(i, m_samplers[i]);
			}

			if (m_active.TextureWrapTypes[i] != m_next.TextureWrapTypes[i] || forced || m_active.TextureIDs[i] != m_next.TextureIDs[i])
			{
				GLExt::glActiveTexture(GL_TEXTURE0 + i);

				int32_t wrap_ = (int32_t)m_next.TextureWrapTypes[i];
				GLExt::glSamplerParameteri(m_samplers[i], GL_TEXTURE_WRAP_S, glwrap[wrap_]);
				GLExt::glSamplerParameteri(m_samplers[i], GL_TEXTURE_WRAP_T, glwrap[wrap_]);

				GLExt::glBindSampler(i, m_samplers[i]);
			}
		}
	}
	else
	{
		GLCheckError();
		for (int32_t i = 0; i < (int32_t)m_renderer->GetCurrentTextures().size(); i++)
		{
			// If a texture is not assigned, skip it.
			if (m_renderer->GetCurrentTextures()[i].UserID == 0)
				continue;

			// always changes because a flag is assigned into a texture
			// if (m_active.TextureFilterTypes[i] != m_next.TextureFilterTypes[i] || forced)
			{
				GLExt::glActiveTexture(GL_TEXTURE0 + i);
				GLCheckError();

				// for webngl
#ifndef NDEBUG
				GLint bound = 0;
				glGetIntegerv(GL_TEXTURE_BINDING_2D, &bound);
				assert(bound > 0);
#endif

				int32_t filter_ = (int32_t)m_next.TextureFilterTypes[i];

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, glfilterMag[filter_]);
				GLCheckError();

				if (m_renderer->GetCurrentTextures()[i].HasMipmap)
				{
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, glfilterMin[filter_]);
				}
				else
				{
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, glfilterMin_NoneMipmap[filter_]);
				}
			}

			// always changes because a flag is assigned into a texture
			// if (m_active.TextureWrapTypes[i] != m_next.TextureWrapTypes[i] || forced)
			{
				GLExt::glActiveTexture(GL_TEXTURE0 + i);
				GLCheckError();

				int32_t wrap_ = (int32_t)m_next.TextureWrapTypes[i];

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, glwrap[wrap_]);
				GLCheckError();

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, glwrap[wrap_]);
				GLCheckError();
			}
		}
		GLCheckError();
	}

	GLExt::glActiveTexture(GL_TEXTURE0);

	m_active = m_next;

	GLCheckError();
}

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
} // namespace EffekseerRendererGL
//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------

#ifdef __ANDROID__

#ifdef __ANDROID__DEBUG__
#define LOG(s) __android_log_print(ANDROID_LOG_DEBUG, "Tag", "%s", s)
#else
#define LOG(s) printf("%s", s)
#endif

#elif defined(_WIN32)
#define LOG(s) OutputDebugStringA(s)
#else
#define LOG(s) printf("%s", s)
#endif

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
namespace EffekseerRendererGL
{

static const char g_header_vs_gl3_src[] =
	"#version 330\n"
	"#define lowp\n"
	"#define mediump\n"
	"#define highp\n"
	"#define IN in\n"
	"#define CENTROID centroid\n"
	"#define TEX2D textureLod\n"
	"#define OUT out\n";

static const char g_header_fs_gl3_src[] =
	"#version 330\n"
	"#define lowp\n"
	"#define mediump\n"
	"#define highp\n"
	"#define IN in\n"
	"#define CENTROID centroid\n"
	"#define TEX2D texture\n"
	"layout (location = 0) out vec4 FRAGCOLOR;\n";

static const char g_header_vs_gles3_src[] =
	"#version 300 es\n"
	"precision mediump float;\n"
	"#define IN in\n"
	"#define CENTROID\n"
	"#define TEX2D textureLod\n"
	"#define OUT out\n";

static const char g_header_fs_gles3_src[] =
	"#version 300 es\n"
	"precision mediump float;\n"
	"#define IN in\n"
	"#define CENTROID\n"
	"#define TEX2D texture\n"
	"layout (location = 0) out vec4 FRAGCOLOR;\n";

static const char g_header_vs_gles2_src[] =
	"precision mediump float;\n"
	"#define IN attribute\n"
	"#define CENTROID\n"
	"#define TEX2D texture2DLod\n"
	"#define OUT varying\n";

static const char g_header_fs_gles2_src[] =
	"precision mediump float;\n"
	"#define IN varying\n"
	"#define CENTROID\n"
	"#define TEX2D texture2D\n"
	"#define FRAGCOLOR gl_FragColor\n";

static const char g_header_vs_gl2_src[] =
	"#version 120\n"
	"#define lowp\n"
	"#define mediump\n"
	"#define highp\n"
	"#define IN attribute\n"
	"#define CENTROID\n"
	"#define TEX2D texture2DLod\n"
	"#define OUT varying\n";

static const char g_header_fs_gl2_src[] =
	"#version 120\n"
	"#define lowp\n"
	"#define mediump\n"
	"#define highp\n"
	"#define IN varying\n"
	"#define CENTROID\n"
	"#define TEX2D texture2D\n"
	"#define FRAGCOLOR gl_FragColor\n";

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
bool Shader::CompileShader(
	OpenGLDeviceType deviceType,
	GLuint& program,
	const char* vs_src,
	size_t vertexShaderSize,
	const char* fs_src,
	size_t pixelShaderSize,
	const char* name)
{
	const char* src_data[2];
	GLint src_size[2];

	GLuint vert_shader, frag_shader;
	GLint res_vs, res_fs, res_link;

	// compile a vertex shader
	if (deviceType == OpenGLDeviceType::OpenGL3)
		src_data[0] = g_header_vs_gl3_src;
	if (deviceType == OpenGLDeviceType::OpenGL2)
		src_data[0] = g_header_vs_gl2_src;
	if (deviceType == OpenGLDeviceType::OpenGLES3)
		src_data[0] = g_header_vs_gles3_src;
	if (deviceType == OpenGLDeviceType::OpenGLES2 || deviceType == OpenGLDeviceType::Emscripten)
		src_data[0] = g_header_vs_gles2_src;

	src_size[0] = (GLint)strlen(src_data[0]);
	src_data[1] = vs_src;
	src_size[1] = (GLint)strlen(vs_src);

	vert_shader = GLExt::glCreateShader(GL_VERTEX_SHADER);
	GLExt::glShaderSource(vert_shader, 2, src_data, src_size);
	GLExt::glCompileShader(vert_shader);
	GLExt::glGetShaderiv(vert_shader, GL_COMPILE_STATUS, &res_vs);

	// compile a fragment shader
	if (deviceType == OpenGLDeviceType::OpenGL3)
		src_data[0] = g_header_fs_gl3_src;
	if (deviceType == OpenGLDeviceType::OpenGL2)
		src_data[0] = g_header_fs_gl2_src;
	if (deviceType == OpenGLDeviceType::OpenGLES3)
		src_data[0] = g_header_fs_gles3_src;
	if (deviceType == OpenGLDeviceType::OpenGLES2 || deviceType == OpenGLDeviceType::Emscripten)
		src_data[0] = g_header_fs_gles2_src;

	src_size[0] = (GLint)strlen(src_data[0]);
	src_data[1] = fs_src;
	src_size[1] = (GLint)strlen(fs_src);

	frag_shader = GLExt::glCreateShader(GL_FRAGMENT_SHADER);
	GLExt::glShaderSource(frag_shader, 2, src_data, src_size);
	GLExt::glCompileShader(frag_shader);
	GLExt::glGetShaderiv(frag_shader, GL_COMPILE_STATUS, &res_fs);

	// create shader program
	program = GLExt::glCreateProgram();
	GLExt::glAttachShader(program, vert_shader);
	GLExt::glAttachShader(program, frag_shader);

	// link shaders
	GLExt::glLinkProgram(program);
	GLExt::glGetProgramiv(program, GL_LINK_STATUS, &res_link);

#ifndef NDEBUG
	if (res_link == GL_FALSE)
	{
		// output errors
		char log[512];
		int32_t log_size;
		GLExt::glGetShaderInfoLog(vert_shader, sizeof(log), &log_size, log);
		if (log_size > 0)
		{
			LOG(name);
			LOG(": Vertex Shader error.\n");
			LOG(log);
		}
		GLExt::glGetShaderInfoLog(frag_shader, sizeof(log), &log_size, log);
		if (log_size > 0)
		{
			LOG(name);
			LOG(": Fragment Shader error.\n");
			LOG(log);
		}
		GLExt::glGetProgramInfoLog(program, sizeof(log), &log_size, log);
		if (log_size > 0)
		{
			LOG(name);
			LOG(": Shader Link error.\n");
			LOG(log);
		}
	}
#endif
	// dispose shader objects
	GLExt::glDeleteShader(frag_shader);
	GLExt::glDeleteShader(vert_shader);

	if (res_link == GL_FALSE)
	{
		GLExt::glDeleteProgram(program);
		return false;
	}

	return true;
}

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
Shader::Shader(
	OpenGLDeviceType deviceType,
	Renderer* renderer,
	DeviceObjectCollection* deviceObjectCollection,
	GLuint program,
	const char* vs_src,
	size_t vertexShaderSize,
	const char* fs_src,
	size_t pixelShaderSize,
	const char* name,
	bool hasRefCount)
	: DeviceObject(static_cast<RendererImplemented*>(renderer), deviceObjectCollection, hasRefCount)
	, deviceType_(deviceType)
	, m_program(program)
	, m_vertexSize(0)
	, m_vertexConstantBuffer(NULL)
	, m_pixelConstantBuffer(NULL)
{
	m_textureSlots.fill(0);
	m_textureSlotEnables.fill(false);

	m_vsSrc.resize(vertexShaderSize);
	memcpy(m_vsSrc.data(), vs_src, vertexShaderSize);
	m_vsSrc.push_back(0);

	m_psSrc.resize(pixelShaderSize);
	memcpy(m_psSrc.data(), fs_src, pixelShaderSize);
	m_psSrc.push_back(0);

	m_name = name;
}

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
GLint Shader::GetAttribId(const char* name) const
{
	return GLExt::glGetAttribLocation(m_program, name);
}

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
GLint Shader::GetUniformId(const char* name) const
{
	return GLExt::glGetUniformLocation(m_program, name);
}

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
Shader::~Shader()
{
	GLExt::glDeleteProgram(m_program);
	ES_SAFE_DELETE_ARRAY(m_vertexConstantBuffer);
	ES_SAFE_DELETE_ARRAY(m_pixelConstantBuffer);
}

Shader* Shader::Create(OpenGLDeviceType deviceType,

					   DeviceObjectCollection* deviceObjectCollection,
					   const char* vs_src,
					   size_t vertexShaderSize,
					   const char* fs_src,
					   size_t pixelShaderSize,
					   const char* name,
					   bool hasRefCount)
{
	GLuint program;

	if (CompileShader(deviceType, program, vs_src, vertexShaderSize, fs_src, pixelShaderSize, name))
	{
		return new Shader(deviceType, nullptr, deviceObjectCollection, program, vs_src, vertexShaderSize, fs_src, pixelShaderSize, name, hasRefCount);
	}
	else
	{
		return NULL;
	}
}

Shader* Shader::Create(
	Renderer* renderer, const char* vs_src, size_t vertexShaderSize, const char* fs_src, size_t pixelShaderSize, const char* name)
{
	GLuint program;

	auto r = static_cast<RendererImplemented*>(renderer);

	if (CompileShader(r->GetDeviceType(), program, vs_src, vertexShaderSize, fs_src, pixelShaderSize, name))
	{
		return new Shader(r->GetDeviceType(),
						  renderer,
						  r->GetDeviceObjectCollection(),
						  program,
						  vs_src,
						  vertexShaderSize,
						  fs_src,
						  pixelShaderSize,
						  name,
						  true);
	}
	else
	{
		return NULL;
	}
}

void Shader::OnLostDevice()
{
	GLExt::glDeleteProgram(m_program);
	m_program = 0;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void Shader::OnResetDevice()
{
	if (IsValid())
		return;

	GLuint program;

	if (CompileShader(
			deviceType_,
			program,
			(const char*)(m_vsSrc.data()),
			m_vsSrc.size(),
			(const char*)(m_psSrc.data()),
			m_psSrc.size(),
			m_name.c_str()))
	{
		m_program = program;
		GetAttribIdList(0, nullptr);
	}
	else
	{
		printf("Failed to reset device.\n");
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void Shader::OnChangeDevice()
{
	GLExt::glDeleteProgram(m_program);

	GLuint program;

	if (CompileShader(
			deviceType_,
			program,
			(const char*)&(m_vsSrc[0]),
			m_vsSrc.size(),
			(const char*)&(m_psSrc[0]),
			m_psSrc.size(),
			m_name.c_str()))
	{
		m_program = program;
		GetAttribIdList(0, nullptr);
	}
	else
	{
		printf("Failed to change device.\n");
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
GLuint Shader::GetInterface() const
{
	return m_program;
}

void Shader::GetAttribIdList(int count, const ShaderAttribInfo* info)
{
	// TODO : refactoring

	m_aid.clear();

	if (info != nullptr)
	{
		for (int i = 0; i < count; i++)
		{
			m_aid.push_back(GLExt::glGetAttribLocation(m_program, info[i].name));
			Layout layout;

			layout.normalized = info[i].normalized;
			layout.type = info[i].type;
			layout.offset = info[i].offset;
			layout.count = info[i].count;

			m_layout.push_back(layout);
		}

		attribs.resize(count);

		for (int i = 0; i < count; i++)
		{
			attribs[i].name = info[i].name;
			attribs[i].normalized = info[i].normalized;
			attribs[i].type = info[i].type;
			attribs[i].offset = info[i].offset;
			attribs[i].count = info[i].count;
		}
	}
	else
	{
		for (int i = 0; i < (int)attribs.size(); i++)
		{
			m_aid.push_back(GLExt::glGetAttribLocation(m_program, attribs[i].name.c_str()));
			Layout layout;

			layout.normalized = attribs[i].normalized;
			layout.type = attribs[i].type;
			layout.offset = attribs[i].offset;
			layout.count = attribs[i].count;

			m_layout.push_back(layout);
		}
	}
}

void Shader::GetUniformIdList(int count, const ShaderUniformInfo* info, GLint* uid_list) const
{
	for (int i = 0; i < count; i++)
	{
		uid_list[i] = GLExt::glGetUniformLocation(m_program, info[i].name);
	}
}

void Shader::BeginScene()
{
	GLExt::glUseProgram(m_program);
}

void Shader::EndScene()
{
	GLExt::glUseProgram(0);
}

void Shader::EnableAttribs()
{
	GLCheckError();
	for (size_t i = 0; i < m_aid.size(); i++)
	{
		if (m_aid[i] >= 0)
		{
			GLExt::glEnableVertexAttribArray(m_aid[i]);
		}
	}
	GLCheckError();
}

void Shader::DisableAttribs()
{
	GLCheckError();

	for (size_t i = 0; i < m_aid.size(); i++)
	{
		if (m_aid[i] >= 0)
		{
			GLExt::glDisableVertexAttribArray(m_aid[i]);
		}
	}
	GLCheckError();
}

void Shader::SetVertex()
{
	for (size_t i = 0; i < m_aid.size(); i++)
	{
		if (m_aid[i] >= 0)
		{
			GLExt::glVertexAttribPointer(m_aid[i], m_layout[i].count, m_layout[i].type, m_layout[i].normalized, m_vertexSize, reinterpret_cast<GLvoid*>(m_layout[i].offset));
		}
	}
}

void Shader::SetVertexSize(int32_t vertexSize)
{
	m_vertexSize = vertexSize;
}

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
void Shader::SetVertexConstantBufferSize(int32_t size)
{
	ES_SAFE_DELETE_ARRAY(m_vertexConstantBuffer);
	m_vertexConstantBuffer = new uint8_t[size];
}

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
void Shader::SetPixelConstantBufferSize(int32_t size)
{
	ES_SAFE_DELETE_ARRAY(m_pixelConstantBuffer);
	m_pixelConstantBuffer = new uint8_t[size];
}

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
void Shader::AddVertexConstantLayout(eConstantType type, GLint id, int32_t offset)
{
	ConstantLayout l;
	l.Type = type;
	l.ID = id;
	l.Offset = offset;
	m_vertexConstantLayout.push_back(l);
}

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
void Shader::AddPixelConstantLayout(eConstantType type, GLint id, int32_t offset)
{
	ConstantLayout l;
	l.Type = type;
	l.ID = id;
	l.Offset = offset;
	m_pixelConstantLayout.push_back(l);
}

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
void Shader::SetConstantBuffer()
{
	for (size_t i = 0; i < m_vertexConstantLayout.size(); i++)
	{
		if (m_vertexConstantLayout[i].Type == CONSTANT_TYPE_MATRIX44)
		{
			uint8_t* data = (uint8_t*)m_vertexConstantBuffer;
			data += m_vertexConstantLayout[i].Offset;
			GLExt::glUniformMatrix4fv(
				m_vertexConstantLayout[i].ID,
				1,
				GL_FALSE,
				(const GLfloat*)data);
		}
		else if (
			CONSTANT_TYPE_MATRIX44_ARRAY_END > m_vertexConstantLayout[i].Type &&
			m_vertexConstantLayout[i].Type >= CONSTANT_TYPE_MATRIX44_ARRAY)
		{
			uint8_t* data = (uint8_t*)m_vertexConstantBuffer;
			data += m_vertexConstantLayout[i].Offset;
			GLExt::glUniformMatrix4fv(
				m_vertexConstantLayout[i].ID,
				m_vertexConstantLayout[i].Type - CONSTANT_TYPE_MATRIX44_ARRAY,
				GL_FALSE,
				(const GLfloat*)data);
		}

		else if (m_vertexConstantLayout[i].Type == CONSTANT_TYPE_VECTOR4)
		{
			uint8_t* data = (uint8_t*)m_vertexConstantBuffer;
			data += m_vertexConstantLayout[i].Offset;
			GLExt::glUniform4fv(
				m_vertexConstantLayout[i].ID,
				1,
				(const GLfloat*)data);
		}
		else if (
			CONSTANT_TYPE_VECTOR4_ARRAY_END > m_vertexConstantLayout[i].Type &&
			m_vertexConstantLayout[i].Type >= CONSTANT_TYPE_VECTOR4_ARRAY)
		{
			uint8_t* data = (uint8_t*)m_vertexConstantBuffer;
			data += m_vertexConstantLayout[i].Offset;
			GLExt::glUniform4fv(
				m_vertexConstantLayout[i].ID,
				m_vertexConstantLayout[i].Type - CONSTANT_TYPE_VECTOR4_ARRAY,
				(const GLfloat*)data);
		}
	}

	for (size_t i = 0; i < m_pixelConstantLayout.size(); i++)
	{
		if (m_pixelConstantLayout[i].Type == CONSTANT_TYPE_MATRIX44)
		{
			uint8_t* data = (uint8_t*)m_pixelConstantBuffer;
			data += m_pixelConstantLayout[i].Offset;
			GLExt::glUniformMatrix4fv(
				m_pixelConstantLayout[i].ID,
				1,
				GL_FALSE,
				(const GLfloat*)data);
		}
		else if (
			CONSTANT_TYPE_MATRIX44_ARRAY_END > m_pixelConstantLayout[i].Type &&
			m_pixelConstantLayout[i].Type >= CONSTANT_TYPE_MATRIX44_ARRAY)
		{
			uint8_t* data = (uint8_t*)m_pixelConstantBuffer;
			data += m_pixelConstantLayout[i].Offset;
			GLExt::glUniformMatrix4fv(
				m_pixelConstantLayout[i].ID,
				m_pixelConstantLayout[i].Type - CONSTANT_TYPE_MATRIX44_ARRAY,
				GL_FALSE,
				(const GLfloat*)data);
		}

		else if (m_pixelConstantLayout[i].Type == CONSTANT_TYPE_VECTOR4)
		{
			uint8_t* data = (uint8_t*)m_pixelConstantBuffer;
			data += m_pixelConstantLayout[i].Offset;
			GLExt::glUniform4fv(
				m_pixelConstantLayout[i].ID,
				1,
				(const GLfloat*)data);
		}
		else if (
			CONSTANT_TYPE_VECTOR4_ARRAY_END > m_pixelConstantLayout[i].Type &&
			m_pixelConstantLayout[i].Type >= CONSTANT_TYPE_VECTOR4_ARRAY)
		{
			uint8_t* data = (uint8_t*)m_pixelConstantBuffer;
			data += m_pixelConstantLayout[i].Offset;
			GLExt::glUniform4fv(
				m_pixelConstantLayout[i].ID,
				m_pixelConstantLayout[i].Type - CONSTANT_TYPE_VECTOR4_ARRAY,
				(const GLfloat*)data);
		}
	}

	GLCheckError();
}

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
void Shader::SetTextureSlot(int32_t index, GLuint value)
{
	m_textureSlots[index] = value;
	m_textureSlotEnables[index] = true;
}

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
GLuint Shader::GetTextureSlot(int32_t index)
{
	return m_textureSlots[index];
}

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
bool Shader::GetTextureSlotEnable(int32_t index)
{
	return m_textureSlotEnables[index];
}

bool Shader::IsValid() const
{
	return m_program != 0;
}

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
} // namespace EffekseerRendererGL


#ifdef __EFFEKSEER_RENDERER_INTERNAL_LOADER__

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
TextureLoader::TextureLoader(::Effekseer::FileInterface* fileInterface, ::Effekseer::ColorSpaceType colorSpaceType)
	: m_fileInterface(fileInterface)
	, colorSpaceType_(colorSpaceType)
{
	if (m_fileInterface == NULL)
	{
		m_fileInterface = &m_defaultFileInterface;
	}

#ifdef __EFFEKSEER_RENDERER_INTERNAL_LOADER__
	pngTextureLoader.Initialize();
#endif
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
TextureLoader::~TextureLoader()
{
#ifdef __EFFEKSEER_RENDERER_INTERNAL_LOADER__
	pngTextureLoader.Finalize();
#endif
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
Effekseer::TextureData* TextureLoader::Load(const EFK_CHAR* path, ::Effekseer::TextureType textureType)
{
	std::unique_ptr<Effekseer::FileReader>
		reader(m_fileInterface->OpenRead(path));

	if (reader.get() != NULL)
	{
		size_t size_texture = reader->GetLength();
		char* data_texture = new char[size_texture];
		reader->Read(data_texture, size_texture);

		auto textureData = Load(data_texture, size_texture, textureType);

		delete[] data_texture;

		return textureData;
	}

	return nullptr;
}

Effekseer::TextureData* TextureLoader::Load(const void* data, int32_t size, Effekseer::TextureType textureType)
{
	auto size_texture = size;
	auto data_texture = (uint8_t*)data;

	if (size_texture < 4)
	{
	}
	else if (data_texture[1] == 'P' && data_texture[2] == 'N' && data_texture[3] == 'G')
	{
		pngTextureLoader.Load(data_texture, size_texture, false);

		GLuint colorFormat = GL_RGBA;
		if (colorSpaceType_ == ::Effekseer::ColorSpaceType::Linear && textureType == Effekseer::TextureType::Color)
			colorFormat = GL_SRGB8_ALPHA8;

		GLuint texture = 0;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D,
					 0,
					 colorFormat,
					 pngTextureLoader.GetWidth(),
					 pngTextureLoader.GetHeight(),
					 0,
					 GL_RGBA,
					 GL_UNSIGNED_BYTE,
					 pngTextureLoader.GetData().data());

		// Generate mipmap
		GLExt::glGenerateMipmap(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, 0);
		pngTextureLoader.Unload();

		auto textureData = new Effekseer::TextureData();
		textureData->UserPtr = nullptr;
		textureData->UserID = texture;
		textureData->TextureFormat = Effekseer::TextureFormatType::ABGR8;
		textureData->Width = pngTextureLoader.GetWidth();
		textureData->Height = pngTextureLoader.GetHeight();
		return textureData;
	}
	else if (data_texture[0] == 'D' && data_texture[1] == 'D' && data_texture[2] == 'S' && data_texture[3] == ' ')
	{
		if (ddsTextureLoader.Load(data_texture, size_texture))
		{
			if (ddsTextureLoader.GetTextureFormat() == Effekseer::TextureFormatType::ABGR8)
			{
				GLuint texture = 0;

				glGenTextures(1, &texture);
				glBindTexture(GL_TEXTURE_2D, texture);

				glTexImage2D(GL_TEXTURE_2D,
							 0,
							 GL_RGBA,
							 ddsTextureLoader.GetWidth(),
							 ddsTextureLoader.GetHeight(),
							 0,
							 GL_RGBA,
							 GL_UNSIGNED_BYTE,
							 ddsTextureLoader.GetData().data());

				// Generate mipmap
				GLExt::glGenerateMipmap(GL_TEXTURE_2D);

				glBindTexture(GL_TEXTURE_2D, 0);

				auto textureData = new Effekseer::TextureData();
				textureData->UserPtr = nullptr;
				textureData->UserID = texture;
				textureData->TextureFormat = ddsTextureLoader.GetTextureFormat();
				textureData->Width = ddsTextureLoader.GetWidth();
				textureData->Height = ddsTextureLoader.GetHeight();
				return textureData;
			}
			else
			{
				uint32_t format = 0;

				if (ddsTextureLoader.GetTextureFormat() == Effekseer::TextureFormatType::BC1)
				{
					format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
				}
				else if (ddsTextureLoader.GetTextureFormat() == Effekseer::TextureFormatType::BC2)
				{
					format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
				}
				else if (ddsTextureLoader.GetTextureFormat() == Effekseer::TextureFormatType::BC3)
				{
					format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
				}

				GLuint texture = 0;
				glGenTextures(1, &texture);
				glBindTexture(GL_TEXTURE_2D, texture);

				GLExt::glCompressedTexImage2D(GL_TEXTURE_2D,
											  0,
											  format,
											  ddsTextureLoader.GetWidth(),
											  ddsTextureLoader.GetHeight(),
											  0,
											  ddsTextureLoader.GetData().size(),
											  ddsTextureLoader.GetData().data());

				// Generate mipmap
				GLExt::glGenerateMipmap(GL_TEXTURE_2D);

				glBindTexture(GL_TEXTURE_2D, 0);

				auto textureData = new Effekseer::TextureData();
				textureData->UserPtr = nullptr;
				textureData->UserID = texture;
				textureData->TextureFormat = ddsTextureLoader.GetTextureFormat();
				textureData->Width = ddsTextureLoader.GetWidth();
				textureData->Height = ddsTextureLoader.GetHeight();
				return textureData;
			}
		}
	}

	return nullptr;
}

void TextureLoader::Unload(Effekseer::TextureData* data)
{
	if (data != NULL)
	{
		GLuint texture = (GLuint)data->UserID;
		glDeleteTextures(1, &texture);
	}

	if (data != nullptr)
	{
		delete data;
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
} // namespace EffekseerRendererGL
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------

#endif


//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
namespace EffekseerRendererGL
{
//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
VertexArray::VertexArray(RendererImplemented* renderer, Shader* shader, VertexBuffer* vertexBuffer, IndexBuffer* indexBuffer, bool hasRefCount)
	: DeviceObject(renderer, renderer->GetDeviceObjectCollection(), hasRefCount)
	, m_shader(shader)
	, m_vertexBuffer(vertexBuffer)
	, m_indexBuffer(indexBuffer)
{
	Init();
}

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
VertexArray::~VertexArray()
{
	Release();
}

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
VertexArray* VertexArray::Create(RendererImplemented* renderer, Shader* shader, VertexBuffer* vertexBuffer, IndexBuffer* indexBuffer, bool hasRefCount)
{
	if (GLExt::IsSupportedVertexArray())
	{
		return new VertexArray(renderer, shader, vertexBuffer, indexBuffer, hasRefCount);
	}
	return nullptr;
}

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
void VertexArray::OnLostDevice()
{
	Release();
}

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
void VertexArray::OnResetDevice()
{
	Init();
}

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
void VertexArray::Init()
{
	if (!m_shader->IsValid())
	{
		m_shader->OnResetDevice();
	}

	GLCheckError();

	GLExt::glGenVertexArrays(1, &m_vertexArray);

	GLExt::glBindVertexArray(m_vertexArray);

	if (m_vertexBuffer != nullptr)
	{
		if (!m_vertexBuffer->IsValid())
		{
			m_vertexBuffer->OnResetDevice();
		}

		GLExt::glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer->GetInterface());
	}

	if (m_indexBuffer != nullptr)
	{
		if (!m_indexBuffer->IsValid())
		{
			m_indexBuffer->OnResetDevice();
		}

		GLExt::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer->GetInterface());
	}

	if (m_vertexBuffer != nullptr)
	{
		m_shader->EnableAttribs();
		m_shader->SetVertex();
	}

	GLExt::glBindVertexArray(0);

	GLCheckError();
}

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
void VertexArray::Release()
{
	GLCheckError();

	if (m_vertexArray != 0)
	{
		GLExt::glDeleteVertexArrays(1, &m_vertexArray);
		m_vertexArray = 0;
	}

	GLCheckError();
}

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
} // namespace EffekseerRendererGL
  //-----------------------------------------------------------------------------------
  //
  //-----------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
namespace EffekseerRendererGL
{
//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
VertexBuffer::VertexBuffer(RendererImplemented* renderer, int size, bool isDynamic, bool hasRefCount)
	: DeviceObject(renderer, renderer->GetDeviceObjectCollection(), hasRefCount)
	, VertexBufferBase(size, isDynamic)
	, m_vertexRingStart(0)
	, m_vertexRingOffset(0)
	, m_ringBufferLock(false)
{
	m_resource = new uint8_t[m_size];
	memset(m_resource, 0, (size_t)m_size);

	GLExt::glGenBuffers(1, &m_buffer);
	GLExt::glBindBuffer(GL_ARRAY_BUFFER, m_buffer);

#ifndef __ANDROID__
	GLExt::glBufferData(GL_ARRAY_BUFFER, m_size, m_resource, GL_STREAM_DRAW);
#endif // !__ANDROID__

	GLExt::glBindBuffer(GL_ARRAY_BUFFER, 0);
}

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
VertexBuffer::~VertexBuffer()
{
	GLExt::glDeleteBuffers(1, &m_buffer);
	delete[] m_resource;
}

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
VertexBuffer* VertexBuffer::Create(RendererImplemented* renderer, int size, bool isDynamic, bool hasRefCount)
{
	return new VertexBuffer(renderer, size, isDynamic, hasRefCount);
}

GLuint VertexBuffer::GetInterface()
{
	return m_buffer;
}

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
void VertexBuffer::OnLostDevice()
{
	GLExt::glDeleteBuffers(1, &m_buffer);
	m_buffer = 0;
}

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
void VertexBuffer::OnResetDevice()
{
	if (IsValid())
		return;

	GLExt::glGenBuffers(1, &m_buffer);
	GLExt::glBindBuffer(GL_ARRAY_BUFFER, m_buffer);

#ifndef __ANDROID__
	GLExt::glBufferData(GL_ARRAY_BUFFER, m_size, m_resource, GL_STREAM_DRAW);
#endif // !__ANDROID__

	GLExt::glBindBuffer(GL_ARRAY_BUFFER, 0);
}

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
void VertexBuffer::Lock()
{
	assert(!m_isLock);

	m_isLock = true;
	m_offset = 0;
	m_vertexRingStart = 0;
}

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
bool VertexBuffer::RingBufferLock(int32_t size, int32_t& offset, void*& data, int32_t alignment)
{
	assert(!m_isLock);
	assert(!m_ringBufferLock);
	assert(this->m_isDynamic);

	if (size > m_size)
		return false;

	m_vertexRingOffset = (m_vertexRingOffset + alignment - 1) / alignment * alignment;

#ifdef __ANDROID__
	if (true)
#else
	if ((int32_t)m_vertexRingOffset + size > m_size)
#endif
	{
		offset = 0;
		data = m_resource;
		m_vertexRingOffset = size;

		m_vertexRingStart = offset;
		m_offset = size;
	}
	else
	{
		offset = m_vertexRingOffset;
		data = m_resource;
		m_vertexRingOffset += size;

		m_vertexRingStart = offset;
		m_offset = size;
	}

	m_ringBufferLock = true;

	return true;
}

bool VertexBuffer::TryRingBufferLock(int32_t size, int32_t& offset, void*& data, int32_t alignment)
{
	if ((int32_t)m_vertexRingOffset + size > m_size)
		return false;

	return RingBufferLock(size, offset, data, alignment);
}

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
void VertexBuffer::Unlock()
{
	assert(m_isLock || m_ringBufferLock);

	GLExt::glBindBuffer(GL_ARRAY_BUFFER, m_buffer);

	if (GLExt::IsSupportedBufferRange() && m_vertexRingStart > 0)
	{
#ifdef __ANDROID__
		GLExt::glBufferData(GL_ARRAY_BUFFER, m_offset, m_resource, GL_STREAM_DRAW);
#endif // !__ANDROID__

		auto target = GLExt::glMapBufferRange(GL_ARRAY_BUFFER, m_vertexRingStart, m_offset, GL_MAP_WRITE_BIT | GL_MAP_UNSYNCHRONIZED_BIT);
		memcpy(target, m_resource, m_offset);
		GLExt::glUnmapBuffer(GL_ARRAY_BUFFER);
	}
	else
	{
		// giMapBuffer is invalid with OpenGLES3 after iOS12.2?
		bool avoidIOS122 = false;
#if defined(__APPLE__)
		if (GLExt::GetDeviceType() == OpenGLDeviceType::OpenGLES3)
		{
			avoidIOS122 = true;
		}
#endif

		if (GLExt::IsSupportedMapBuffer() && !avoidIOS122)
		{
#ifdef __ANDROID__
			GLExt::glBufferData(GL_ARRAY_BUFFER, m_offset, nullptr, GL_STREAM_DRAW);
#else
			GLExt::glBufferData(GL_ARRAY_BUFFER, m_size, nullptr, GL_STREAM_DRAW);
#endif // !__ANDROID__

			auto target = (uint8_t*)GLExt::glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
			if (target == nullptr)
			{
				GLExt::MakeMapBufferInvalid();

				if (m_vertexRingStart > 0)
				{
					GLExt::glBufferSubData(GL_ARRAY_BUFFER, m_vertexRingStart, m_offset, m_resource);
				}
				else
				{
					GLExt::glBufferData(GL_ARRAY_BUFFER, m_size, m_resource, GL_STREAM_DRAW);
				}
			}
			else
			{
				memcpy(target + m_vertexRingStart, m_resource, m_offset);
				GLExt::glUnmapBuffer(GL_ARRAY_BUFFER);
			}
		}
		else
		{
#ifdef __ANDROID__
			GLExt::glBufferData(GL_ARRAY_BUFFER, m_size, m_resource, GL_STREAM_DRAW);
#else
			if (m_vertexRingStart > 0)
			{
				GLExt::glBufferSubData(GL_ARRAY_BUFFER, m_vertexRingStart, m_offset, m_resource);
			}
			else
			{
				GLExt::glBufferData(GL_ARRAY_BUFFER, m_size, m_resource, GL_STREAM_DRAW);
			}
#endif
		}
	}

	GLExt::glBindBuffer(GL_ARRAY_BUFFER, 0);

	if (m_isLock)
	{
		m_vertexRingOffset += m_offset;
	}

	m_isLock = false;
	m_ringBufferLock = false;
}

bool VertexBuffer::IsValid()
{
	return m_buffer != 0;
}

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
} // namespace EffekseerRendererGL
//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------




namespace Effekseer
{
namespace GL
{
/*
static char* material_common_define = R"(
#define MOD mod
#define FRAC fract
#define LERP mix

float atan2(in float y, in float x) {
    return x == 0.0 ? sign(y)* 3.141592 / 2.0 : atan(y, x);
}

)";

static const char g_material_model_vs_src_pre[] =
	R"(
IN vec4 a_Position;
IN vec3 a_Normal;
IN vec3 a_Binormal;
IN vec3 a_Tangent;
IN vec2 a_TexCoord;
IN vec4 a_Color;
)"
#if defined(MODEL_SOFTWARE_INSTANCING)
	R"(
IN float a_InstanceID;
IN vec4 a_UVOffset;
IN vec4 a_ModelColor;
)"
#endif
	R"(

OUT lowp vec4 v_VColor;
OUT mediump vec2 v_UV1;
OUT mediump vec2 v_UV2;
OUT mediump vec3 v_WorldP;
OUT mediump vec3 v_WorldN;
OUT mediump vec3 v_WorldT;
OUT mediump vec3 v_WorldB;
OUT mediump vec2 v_ScreenUV;
//$C_OUT1$
//$C_OUT2$

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

)";

static const char g_material_model_vs_src_suf1[] =
	R"(

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
}
)";

static const char g_material_sprite_vs_src_pre_simple[] =
	R"(
IN vec4 atPosition;
IN vec4 atColor;
IN vec4 atTexCoord;
)"

	R"(
OUT lowp vec4 v_VColor;
OUT mediump vec2 v_UV1;
OUT mediump vec2 v_UV2;
OUT mediump vec3 v_WorldP;
OUT mediump vec3 v_WorldN;
OUT mediump vec3 v_WorldT;
OUT mediump vec3 v_WorldB;
OUT mediump vec2 v_ScreenUV;
)"

	R"(
uniform mat4 uMatCamera;
uniform mat4 uMatProjection;
uniform vec4 mUVInversed;
uniform vec4 predefined_uniform;
uniform vec4 cameraPosition;

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

)";

static const char g_material_sprite_vs_src_pre[] =
	R"(
IN vec4 atPosition;
IN vec4 atColor;
IN vec3 atNormal;
IN vec3 atTangent;
IN vec2 atTexCoord;
IN vec2 atTexCoord2;
//$C_IN1$
//$C_IN2$
)"

	R"(
OUT lowp vec4 v_VColor;
OUT mediump vec2 v_UV1;
OUT mediump vec2 v_UV2;
OUT mediump vec3 v_WorldP;
OUT mediump vec3 v_WorldN;
OUT mediump vec3 v_WorldT;
OUT mediump vec3 v_WorldB;
OUT mediump vec2 v_ScreenUV;
//$C_OUT1$
//$C_OUT2$
)"

	R"(
uniform mat4 uMatCamera;
uniform mat4 uMatProjection;
uniform vec4 mUVInversed;
uniform vec4 predefined_uniform;
uniform vec4 cameraPosition;

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

)";

static const char g_material_sprite_vs_src_suf1_simple[] =

	R"(

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
}

)";

static const char g_material_fs_src_pre[] =
	R"(

IN lowp vec4 v_VColor;
IN mediump vec2 v_UV1;
IN mediump vec2 v_UV2;
IN mediump vec3 v_WorldP;
IN mediump vec3 v_WorldN;
IN mediump vec3 v_WorldT;
IN mediump vec3 v_WorldB;
IN mediump vec2 v_ScreenUV;
//$C_PIN1$
//$C_PIN2$

uniform vec4 mUVInversedBack;
uniform vec4 predefined_uniform;
uniform vec4 cameraPosition;

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


)";

static const char g_material_fs_src_suf1[] =
	R"(

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
	vec3 specular = lightColor.xyz * lightScale * calcLightingGGX(worldNormal, viewDir, lightDirection.xyz, roughness, 0.9);

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

	vec4 bg = TEX2D(background, distortUV);
	FRAGCOLOR = bg;

	if(opacityMask <= 0.0) discard;
	if(opacity <= 0.0) discard;
}

)";

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

struct ShaderData
{
	std::string CodeVS;
	std::string CodePS;
};

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

void ExportTexture(std::ostringstream& maincode, const char* name) { maincode << "uniform sampler2D " << name << ";" << std::endl; }

void ExportHeader(std::ostringstream& maincode, Material* material, int stage, bool isSprite)
{
	maincode << material_common_define;

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
}

void ExportMain(
	std::ostringstream& maincode, Material* material, int stage, bool isSprite, MaterialShaderType shaderType, const std::string& baseCode)
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

ShaderData GenerateShader(Material* material, MaterialShaderType shaderType, int32_t maximumTextureCount)
{
	bool isSprite = shaderType == MaterialShaderType::Standard || shaderType == MaterialShaderType::Refraction;
	bool isRefrection =
		material->GetHasRefraction() && (shaderType == MaterialShaderType::Refraction || shaderType == MaterialShaderType::RefractionModel);

	ShaderData shaderData;

	for (int stage = 0; stage < 2; stage++)
	{
		std::ostringstream maincode;

		ExportHeader(maincode, material, stage, isSprite);

		int32_t actualTextureCount = std::min(maximumTextureCount, material->GetTextureCount());

		for (size_t i = 0; i < actualTextureCount; i++)
		{
			auto textureIndex = material->GetTextureIndex(i);
			auto textureName = material->GetTextureName(i);

			ExportTexture(maincode, textureName);
		}

		for (size_t i = actualTextureCount; i < actualTextureCount + 1; i++)
		{
			ExportTexture(maincode, "background");
		}

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

		ExportMain(maincode, material, stage, isSprite, shaderType, baseCode);

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
	if (material->GetCustomData1Count() > 0)
	{
		if (isSprite)
		{
			shaderData.CodeVS =
				Replace(shaderData.CodeVS, "//$C_IN1$", "IN " + GetType(material->GetCustomData1Count()) + " atCustomData1;");
		}
		shaderData.CodeVS =
			Replace(shaderData.CodeVS, "//$C_OUT1$", "OUT mediump " + GetType(material->GetCustomData1Count()) + " v_CustomData1;");
		shaderData.CodePS =
			Replace(shaderData.CodePS, "//$C_PIN1$", "IN mediump " + GetType(material->GetCustomData1Count()) + " v_CustomData1;");
	}

	if (material->GetCustomData2Count() > 0)
	{
		if (isSprite)
		{
			shaderData.CodeVS =
				Replace(shaderData.CodeVS, "//$C_IN2$", "IN " + GetType(material->GetCustomData2Count()) + " atCustomData2;");
		}
		shaderData.CodeVS =
			Replace(shaderData.CodeVS, "//$C_OUT2$", "OUT mediump " + GetType(material->GetCustomData2Count()) + " v_CustomData2;");
		shaderData.CodePS =
			Replace(shaderData.CodePS, "//$C_PIN2$", "IN mediump " + GetType(material->GetCustomData2Count()) + " v_CustomData2;");
	}

	return shaderData;
}
*/
} // namespace GL

} // namespace Effekseer

namespace Effekseer
{

class CompiledMaterialBinaryGL : public CompiledMaterialBinary, ReferenceObject
{
private:
	std::array<std::vector<uint8_t>, static_cast<int32_t>(MaterialShaderType::Max)> vertexShaders_;

	std::array<std::vector<uint8_t>, static_cast<int32_t>(MaterialShaderType::Max)> pixelShaders_;

public:
	CompiledMaterialBinaryGL()
	{
	}

	virtual ~CompiledMaterialBinaryGL()
	{
	}

	void SetVertexShaderData(MaterialShaderType type, const std::vector<uint8_t>& data)
	{
		vertexShaders_.at(static_cast<int>(type)) = data;
	}

	void SetPixelShaderData(MaterialShaderType type, const std::vector<uint8_t>& data)
	{
		pixelShaders_.at(static_cast<int>(type)) = data;
	}

	const uint8_t* GetVertexShaderData(MaterialShaderType type) const override
	{
		return vertexShaders_.at(static_cast<int>(type)).data();
	}

	int32_t GetVertexShaderSize(MaterialShaderType type) const override
	{
		return static_cast<int32_t>(vertexShaders_.at(static_cast<int>(type)).size());
	}

	const uint8_t* GetPixelShaderData(MaterialShaderType type) const override
	{
		return pixelShaders_.at(static_cast<int>(type)).data();
	}

	int32_t GetPixelShaderSize(MaterialShaderType type) const override
	{
		return static_cast<int32_t>(pixelShaders_.at(static_cast<int>(type)).size());
	}

	int AddRef() override
	{
		return ReferenceObject::AddRef();
	}

	int Release() override
	{
		return ReferenceObject::Release();
	}

	int GetRef() override
	{
		return ReferenceObject::GetRef();
	}
};

CompiledMaterialBinary* MaterialCompilerGL::Compile(Material* material, int32_t maximumTextureCount)
{
	auto binary = new CompiledMaterialBinaryGL();

	auto convertToVector = [](const std::string& str) -> std::vector<uint8_t> {
		std::vector<uint8_t> ret;
		ret.resize(str.size() + 1);
		memcpy(ret.data(), str.data(), str.size());
		ret[str.size()] = 0;
		return ret;
	};

	auto saveBinary = [&material, &binary, &convertToVector, &maximumTextureCount](MaterialShaderType type) {
		GLSL::ShaderGenerator generator;
		auto shader = generator.GenerateShader(material, type, maximumTextureCount, false, false, false, false, 0);
		binary->SetVertexShaderData(type, convertToVector(shader.CodeVS));
		binary->SetPixelShaderData(type, convertToVector(shader.CodePS));
	};

	if (material->GetHasRefraction())
	{
		saveBinary(MaterialShaderType::Refraction);
		saveBinary(MaterialShaderType::RefractionModel);
	}

	saveBinary(MaterialShaderType::Standard);
	saveBinary(MaterialShaderType::Model);

	return binary;
}

CompiledMaterialBinary* MaterialCompilerGL::Compile(Material* material)
{
	return Compile(material, Effekseer::UserTextureSlotMax);
}

} // namespace Effekseer

#ifdef __SHARED_OBJECT__

extern "C"
{
#ifdef _WIN32
#define EFK_EXPORT __declspec(dllexport)
#else
#define EFK_EXPORT
#endif

	EFK_EXPORT Effekseer::MaterialCompiler* EFK_STDCALL CreateCompiler()
	{
		return new Effekseer::MaterialCompilerGL();
	}
}
#endif

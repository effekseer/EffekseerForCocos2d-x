#include "EffekseerRendererNative.h"
#include <memory>
#include <set>
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
	
void ApplyDepthOffset(::Effekseer::Matrix43& mat, const ::Effekseer::Vector3D& cameraFront, const ::Effekseer::Vector3D& cameraPos, float depthOffset, bool isDepthOffsetScaledWithCamera, bool isDepthOffsetScaledWithEffect, bool isRightHand)
{
	if (depthOffset != 0)
	{
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
			auto cx = mat.Value[3][0] + cameraPos.X;
			auto cy = mat.Value[3][1] + cameraPos.Y;
			auto cz = mat.Value[3][2] + cameraPos.Z;
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

		auto objPos = ::Effekseer::Vector3D(mat.Value[3][0], mat.Value[3][1], mat.Value[3][2]);
		auto dir = cameraPos - objPos;
		Effekseer::Vector3D::Normal(dir, dir);

		if (isRightHand)
		{
			mat.Value[3][0] += dir.X * offset;
			mat.Value[3][1] += dir.Y * offset;
			mat.Value[3][2] += dir.Z * offset;
		}
		else
		{
			mat.Value[3][0] += dir.X * offset;
			mat.Value[3][1] += dir.Y * offset;
			mat.Value[3][2] += dir.Z * offset;
		}
	}
}

void ApplyDepthOffset(::Effekseer::Matrix43& mat, const ::Effekseer::Vector3D& cameraFront, const ::Effekseer::Vector3D& cameraPos, ::Effekseer::Vector3D& scaleValues, float depthOffset, bool isDepthOffsetScaledWithCamera, bool isDepthOffsetScaledWithEffect, bool isRightHand)
{
	if (depthOffset != 0)
	{
		auto offset = depthOffset;

		if (isDepthOffsetScaledWithEffect)
		{
			auto scale = (scaleValues.X + scaleValues.Y + scaleValues.Z) / 3.0f;

			offset *= scale;
		}

		if (isDepthOffsetScaledWithCamera)
		{
			auto cx = mat.Value[3][0] + cameraPos.X;
			auto cy = mat.Value[3][1] + cameraPos.Y;
			auto cz = mat.Value[3][2] + cameraPos.Z;
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

		auto objPos = ::Effekseer::Vector3D(mat.Value[3][0], mat.Value[3][1], mat.Value[3][2]);
		auto dir = cameraPos - objPos;
		Effekseer::Vector3D::Normal(dir, dir);

		if (isRightHand)
		{
			mat.Value[3][0] += dir.X * offset;
			mat.Value[3][1] += dir.Y * offset;
			mat.Value[3][2] += dir.Z * offset;
		}
		else
		{
			mat.Value[3][0] += dir.X * offset;
			mat.Value[3][1] += dir.Y * offset;
			mat.Value[3][2] += dir.Z * offset;
		}
	}
}

void ApplyDepthOffset(::Effekseer::Matrix44& mat, const ::Effekseer::Vector3D& cameraFront, const ::Effekseer::Vector3D& cameraPos, float depthOffset, bool isDepthOffsetScaledWithCamera, bool isDepthOffsetScaledWithEffect, bool isRightHand)
{
	if (depthOffset != 0)
	{
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
			auto cx = mat.Values[3][0] + cameraPos.X;
			auto cy = mat.Values[3][1] + cameraPos.Y;
			auto cz = mat.Values[3][2] + cameraPos.Z;
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

		auto objPos = ::Effekseer::Vector3D(mat.Values[3][0], mat.Values[3][1], mat.Values[3][2]);
		auto dir = cameraPos - objPos;
		Effekseer::Vector3D::Normal(dir, dir);

		if (isRightHand)
		{
			mat.Values[3][0] += dir.X * offset;
			mat.Values[3][1] += dir.Y * offset;
			mat.Values[3][2] += dir.Z * offset;
		}
		else
		{
			mat.Values[3][0] += dir.X * offset;
			mat.Values[3][1] += dir.Y * offset;
			mat.Values[3][2] += dir.Z * offset;
		}
	}
}


}

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
IndexBufferBase::IndexBufferBase( int maxCount, bool isDynamic )
	: m_indexMaxCount	( maxCount )
	, m_indexCount		( 0 )
	, m_isDynamic		( false )
	, m_isLock			( false )
	, m_resource		( NULL )
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
void IndexBufferBase::Push( const void* buffer, int count )
{
	assert( m_isLock );

	memcpy( GetBufferDirect( count ), buffer, count * sizeof(uint16_t) );
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
void* IndexBufferBase::GetBufferDirect( int count )
{
	assert( m_isLock );
	assert( m_indexMaxCount >= m_indexCount + count );

	uint8_t* pBuffer = NULL;

	pBuffer = (uint8_t*)m_resource + ( m_indexCount * sizeof(uint16_t) );
	m_indexCount += count;

	return pBuffer;
}

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
}
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

}
namespace EffekseerRenderer
{

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

}
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

	TextureFilterTypes[0] = ::Effekseer::TextureFilterType::Nearest;
	TextureFilterTypes[1] = ::Effekseer::TextureFilterType::Nearest;
	TextureFilterTypes[2] = ::Effekseer::TextureFilterType::Nearest;
	TextureFilterTypes[3] = ::Effekseer::TextureFilterType::Nearest;
	
	TextureWrapTypes[0] = ::Effekseer::TextureWrapType::Clamp;
	TextureWrapTypes[1] = ::Effekseer::TextureWrapType::Clamp;
	TextureWrapTypes[2] = ::Effekseer::TextureWrapType::Clamp;
	TextureWrapTypes[3] = ::Effekseer::TextureWrapType::Clamp;	
}

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
void RenderStateBase::State::CopyTo( State& state )
{
	state.DepthTest = DepthTest;
	state.DepthWrite = DepthWrite;
	state.AlphaBlend = AlphaBlend;
	state.CullingType = CullingType;
	state.TextureFilterTypes[0] = TextureFilterTypes[0];
	state.TextureFilterTypes[1] = TextureFilterTypes[1];
	state.TextureFilterTypes[2] = TextureFilterTypes[2];
	state.TextureFilterTypes[3] = TextureFilterTypes[3];

	state.TextureWrapTypes[0] = TextureWrapTypes[0];
	state.TextureWrapTypes[1] = TextureWrapTypes[1];
	state.TextureWrapTypes[2] = TextureWrapTypes[2];
	state.TextureWrapTypes[3] = TextureWrapTypes[3];
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
	m_next.CopyTo( newState );
	m_stateStack.push( m_next );
	m_next = newState;
	return m_next;
}

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
void RenderStateBase::Pop()
{
	assert( !m_stateStack.empty() );

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
}
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
}
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
VertexBufferBase::VertexBufferBase( int size, bool isDynamic )
	: m_isDynamic		( isDynamic )
	, m_size			( size )
	, m_offset			( 0 )
	, m_resource		( NULL )
	, m_isLock			( false )
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
void VertexBufferBase::Push( const void* buffer, int size )
{
	assert( m_isLock );
	memcpy( GetBufferDirect( size ), buffer, size );
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
void* VertexBufferBase::GetBufferDirect( int size )
{
	assert( m_isLock );
	if( m_offset + size > m_size ) return NULL;

	void* pBuffer = NULL;

	// バッファに追記
	pBuffer = m_resource + m_offset;
	m_offset += size;
	
	return pBuffer;
}

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
}
//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
#ifndef	__EFFEKSEERRENDERER_GL_BASE_H__
#define	__EFFEKSEERRENDERER_GL_BASE_H__

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
#ifdef _DEBUG
#define GLCheckError()		{ int __code = glGetError(); if(__code != GL_NO_ERROR) { printf("GLError filename = %s , line = %d, error = %s\n", __FILE__, __LINE__, (const char*)gluErrorString(__code) ); }  }
#else
#define GLCheckError()
#endif
#elif EMSCRIPTEN
#ifdef _DEBUG
#define GLCheckError()		{ int __code = glGetError(); if(__code != GL_NO_ERROR) { EM_ASM_ARGS({console.log("GLError filename = " + Pointer_stringify($0) + " , line = " + $1);}, __FILE__, __LINE__); } }
#else
#define GLCheckError()
#endif
#else
#define GLCheckError()
#endif

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
}
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#endif	// __EFFEKSEERRENDERER_GL_BASE_H__
#ifndef	__EFFEKSEERRENDERER_GL_DEVICEOBJECT_H__
#define	__EFFEKSEERRENDERER_GL_DEVICEOBJECT_H__

//----------------------------------------------------------------------------------
// Include
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
	@brief	デバイスによって生成されるオブジェクト
*/
class DeviceObject
{
private:
	RendererImplemented*	m_renderer;

public:
		DeviceObject( RendererImplemented* renderer );
		virtual ~DeviceObject();

	public:
		virtual void OnLostDevice() = 0;
		virtual void OnResetDevice() = 0;

		RendererImplemented* GetRenderer() const;
};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
}
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#endif	// __EFFEKSEERRENDERER_GL_DEVICEOBJECT_H__
#ifndef	__EFFEKSEERRENDERER_GL_GLEXTENSION_H__
#define	__EFFEKSEERRENDERER_GL_GLEXTENSION_H__

//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------
#if _WIN32
#endif

#ifdef EMSCRIPTEN
#define		GL_GLEXT_PROTOTYPES
#define		EGL_EGLEXT_PROTOTYPES
#endif	// EMSCRIPTEN


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

#define GL_COMPRESSED_RGBA_S3TC_DXT1_EXT 0x83F1
#define GL_COMPRESSED_RGBA_S3TC_DXT3_EXT 0x83F2
#define GL_COMPRESSED_RGBA_S3TC_DXT5_EXT 0x83F3

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

void glDeleteBuffers(GLsizei n, const GLuint* buffers);
GLuint glCreateShader(GLenum type);
void glBindBuffer(GLenum target, GLuint buffer);
void glGenBuffers(GLsizei n, GLuint* buffers);
void glBufferData(GLenum target, GLsizeiptr size, const GLvoid* data, GLenum usage);
void glBlendEquationSeparate(GLenum modeRGB, GLenum modeAlpha);
void glBlendFuncSeparate (GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha);
void glBlendEquation (GLenum mode);
void glActiveTexture (GLenum texture);
void glUniform1i(GLint location, GLint v0);
void glShaderSource (GLuint shader, GLsizei count, const GLchar** strings, const GLint* lengths);
void glCompileShader(GLuint shader);
void glGetShaderiv (GLuint shader, GLenum pname, GLint* param);
GLuint glCreateProgram (void);
void glAttachShader(GLuint program, GLuint shader);
void glDeleteProgram(GLuint program);
void glDeleteShader(GLuint shader);
void glLinkProgram (GLuint program);
void glGetProgramiv(GLuint program, GLenum pname, GLint* param);
void glGetShaderInfoLog (GLuint shader, GLsizei bufSize, GLsizei* length, GLchar* infoLog);
void glGetProgramInfoLog (GLuint program, GLsizei bufSize, GLsizei* length, GLchar* infoLog);
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

void glGenVertexArrays(GLsizei n, GLuint *arrays);
void glDeleteVertexArrays(GLsizei n, const GLuint *arrays);
void glBindVertexArray(GLuint array);

void glGenSamplers(GLsizei n, GLuint *samplers);
void glDeleteSamplers(GLsizei n, const GLuint * samplers);
void glSamplerParameteri(GLuint sampler, GLenum pname, GLint param);
void glBindSampler(GLuint unit, GLuint sampler);

void* glMapBuffer(GLenum target, GLenum access);
void* glMapBufferRange(GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access);
GLboolean glUnmapBuffer(GLenum target);

void glCompressedTexImage2D(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void *data);

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
}
}
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#endif	// __EFFEKSEERRENDERER_GL_GLEXTENSION_H__



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
	: public DeviceObject
	, public ::EffekseerRenderer::IndexBufferBase
{
private:
	GLuint					m_buffer;

	IndexBuffer( RendererImplemented* renderer, GLuint buffer, int maxCount, bool isDynamic );

public:
	virtual ~IndexBuffer();

	static IndexBuffer* Create( RendererImplemented* renderer, int maxCount, bool isDynamic );

	GLuint GetInterface() { return m_buffer; }

public:	// デバイス復旧用
	virtual void OnLostDevice() override;
	virtual void OnResetDevice() override;

public:
	void Lock() override;
	void Unlock() override;

	bool IsValid();
};

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
}
//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------

#ifndef	__EFFEKSEERRENDERER_GL_MODEL_RENDERER_H__
#define	__EFFEKSEERRENDERER_GL_MODEL_RENDERER_H__

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
	RendererImplemented*				m_renderer;

	Shader*								m_shader_lighting_texture_normal;
	Shader*								m_shader_lighting_normal;

	Shader*								m_shader_lighting_texture;
	Shader*								m_shader_lighting;

	Shader*								m_shader_texture;
	Shader*								m_shader;

	Shader*								m_shader_distortion_texture;
	Shader*								m_shader_distortion;

	GLint								m_uniformLoc[8][NumUniforms];

	VertexArray*						m_va[8];

	ModelRenderer(
		RendererImplemented* renderer,
		Shader* shader_lighting_texture_normal,
		Shader* shader_lighting_normal,
		Shader* shader_lighting_texture,
		Shader* shader_lighting,
		Shader* shader_texture,
		Shader* shader,
		Shader* shader_distortion_texture,
		Shader* shader_distortion);
public:

	virtual ~ModelRenderer();

	static ModelRenderer* Create( RendererImplemented* renderer );

public:
	void BeginRendering(const efkModelNodeParam& parameter, int32_t count, void* userData) override;

	virtual void Rendering(const efkModelNodeParam& parameter, const InstanceParameter& instanceParameter, void* userData) override;

	void EndRendering( const efkModelNodeParam& parameter, void* userData ) override;
};
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
}
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#endif	// __EFFEKSEERRENDERER_GL_MODEL_RENDERER_H__
#ifndef	__EFFEKSEERRENDERER_GL_RENDERER_IMPLEMENTED_H__
#define	__EFFEKSEERRENDERER_GL_RENDERER_IMPLEMENTED_H__

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
struct Vertex
{
	::Effekseer::Vector3D	Pos;
	::Effekseer::Color		Col;
	float					UV[2];

	void SetColor( const ::Effekseer::Color& color )
	{
		Col = color;
	}
};

struct VertexDistortion
{
	::Effekseer::Vector3D	Pos;
	::Effekseer::Color		Col;
	float					UV[2];
	::Effekseer::Vector3D	Tangent;
	::Effekseer::Vector3D	Binormal;

	void SetColor(const ::Effekseer::Color& color)
	{
		Col = color;
	}
};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
struct RenderStateSet
{
	GLboolean	blend;
	GLboolean	cullFace;
	GLboolean	depthTest;
	GLboolean	depthWrite;
	GLboolean	texture;
	GLint		depthFunc;
	GLint		cullFaceMode;
	GLint		blendSrc;
	GLint		blendDst;
	GLint		blendEquation;
	GLint		vao;
};

/**
	@brief	描画クラス
	@note
	ツール向けの描画機能。
*/
class RendererImplemented
	: public Renderer
	, public ::Effekseer::ReferenceObject
{
friend class DeviceObject;

private:
	VertexBuffer*		m_vertexBuffer;
	IndexBuffer*		m_indexBuffer = nullptr;
	IndexBuffer*		m_indexBufferForWireframe = nullptr;
	int32_t				m_squareMaxCount;
	
	int32_t				drawcallCount = 0;
	int32_t				drawvertexCount = 0;

	Shader*							m_shader;
	Shader*							m_shader_no_texture;

	Shader*							m_shader_distortion;
	Shader*							m_shader_no_texture_distortion;

	Shader*		currentShader = nullptr;

	EffekseerRenderer::StandardRenderer<RendererImplemented, Shader, Vertex, VertexDistortion>*	m_standardRenderer;

	VertexArray*			m_vao;
	VertexArray*			m_vao_no_texture;

	VertexArray*			m_vao_distortion;
	VertexArray*			m_vao_no_texture_distortion;

	VertexArray*			m_vao_wire_frame;

	::Effekseer::Vector3D	m_lightDirection;
	::Effekseer::Color		m_lightColor;
	::Effekseer::Color		m_lightAmbient;

	::Effekseer::Matrix44	m_proj;
	::Effekseer::Matrix44	m_camera;
	::Effekseer::Matrix44	m_cameraProj;

	::Effekseer::Vector3D	m_cameraPosition;
	::Effekseer::Vector3D	m_cameraFrontDirection;

	::EffekseerRenderer::RenderStateBase*		m_renderState;

	Effekseer::TextureData	m_background;

	std::set<DeviceObject*>	m_deviceObjects;

	OpenGLDeviceType		m_deviceType;

	// ステート保存用
	RenderStateSet m_originalState;

	bool	m_restorationOfStates;

	EffekseerRenderer::DistortingCallback* m_distortingCallback;

	/* 現在設定されているテクスチャ */
	std::vector<GLuint>	m_currentTextures;

	VertexArray*	m_currentVertexArray;

	Effekseer::RenderMode m_renderMode = Effekseer::RenderMode::Normal;

public:
	/**
		@brief	コンストラクタ
	*/
	RendererImplemented(int32_t squareMaxCount, OpenGLDeviceType deviceType);

	/**
		@brief	デストラクタ
	*/
	~RendererImplemented();

	void OnLostDevice() override;
	void OnResetDevice() override;

	/**
		@brief	初期化
	*/
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
		@brief	ライトの方向を取得する。
	*/
	const ::Effekseer::Vector3D& GetLightDirection() const override;

	/**
		@brief	ライトの方向を設定する。
	*/
	void SetLightDirection( const ::Effekseer::Vector3D& direction ) override;

	/**
		@brief	ライトの色を取得する。
	*/
	const ::Effekseer::Color& GetLightColor() const override;

	/**
		@brief	ライトの色を設定する。
	*/
	void SetLightColor( const ::Effekseer::Color& color ) override;

	/**
		@brief	ライトの環境光の色を取得する。
	*/
	const ::Effekseer::Color& GetLightAmbientColor() const override;

	/**
		@brief	ライトの環境光の色を設定する。
	*/
	void SetLightAmbientColor( const ::Effekseer::Color& color ) override;

	/**
		@brief	投影行列を取得する。
	*/
	const ::Effekseer::Matrix44& GetProjectionMatrix() const override;

	/**
		@brief	投影行列を設定する。
	*/
	void SetProjectionMatrix( const ::Effekseer::Matrix44& mat ) override;

	/**
		@brief	カメラ行列を取得する。
	*/
	const ::Effekseer::Matrix44& GetCameraMatrix() const override;

	/**
		@brief	カメラ行列を設定する。
	*/
	void SetCameraMatrix( const ::Effekseer::Matrix44& mat ) override;

	/**
		@brief	カメラプロジェクション行列を取得する。
	*/
	::Effekseer::Matrix44& GetCameraProjectionMatrix() override;

	::Effekseer::Vector3D GetCameraFrontDirection() const override;

	::Effekseer::Vector3D GetCameraPosition() const  override;

	void SetCameraParameter(const ::Effekseer::Vector3D& front, const ::Effekseer::Vector3D& position)  override;

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
	::Effekseer::TextureLoader* CreateTextureLoader( ::Effekseer::FileInterface* fileInterface = NULL )override;
	
	/**
		@brief	モデル読込クラスを生成する。
	*/
	::Effekseer::ModelLoader* CreateModelLoader( ::Effekseer::FileInterface* fileInterface = NULL )override;

	/**
	@brief	背景を取得する。
	*/
	Effekseer::TextureData* GetBackground() override 
	{
		if (m_background.UserID == 0) return nullptr;
		return &m_background;
	}

	/**
	@brief	背景を設定する。
	*/
	void SetBackground(GLuint background) override;

	EffekseerRenderer::DistortingCallback* GetDistortingCallback() override;

	void SetDistortingCallback(EffekseerRenderer::DistortingCallback* callback) override;

	/**
	@brief	描画モードを設定する。
	*/
	void SetRenderMode( Effekseer::RenderMode renderMode ) override
	{
		m_renderMode = renderMode;
	}
	Effekseer::RenderMode GetRenderMode() override
	{
		return m_renderMode;
	}

	EffekseerRenderer::StandardRenderer<RendererImplemented, Shader, Vertex, VertexDistortion>* GetStandardRenderer() { return m_standardRenderer; }

	void SetVertexBuffer( VertexBuffer* vertexBuffer, int32_t size );
	void SetVertexBuffer(GLuint vertexBuffer, int32_t size);
	void SetIndexBuffer( IndexBuffer* indexBuffer );
	void SetIndexBuffer(GLuint indexBuffer);
	void SetVertexArray( VertexArray* vertexArray );

	void SetLayout(Shader* shader);
	void DrawSprites( int32_t spriteCount, int32_t vertexOffset );
	void DrawPolygon( int32_t vertexCount, int32_t indexCount);

	Shader* GetShader(bool useTexture, bool useDistortion) const;
	void BeginShader(Shader* shader);
	void EndShader(Shader* shader);

	void SetVertexBufferToShader(const void* data, int32_t size);

	void SetPixelBufferToShader(const void* data, int32_t size);

	void SetTextures(Shader* shader, Effekseer::TextureData** textures, int32_t count);

	void ResetRenderState() override;

	int32_t GetDrawCallCount() const override;

	int32_t GetDrawVertexCount() const override;

	void ResetDrawCallCount() override;

	void ResetDrawVertexCount() override;

	std::vector<GLuint>& GetCurrentTextures() { return m_currentTextures; }

	OpenGLDeviceType GetDeviceType() { return m_deviceType; }

	virtual int GetRef() override { return ::Effekseer::ReferenceObject::GetRef(); }
	virtual int AddRef() override { return ::Effekseer::ReferenceObject::AddRef(); }
	virtual int Release() override { return ::Effekseer::ReferenceObject::Release(); }

private:
	void GenerateIndexData();
};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
}
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#endif	// __EFFEKSEERRENDERER_GL_RENDERER_IMPLEMENTED_H__

#ifndef	__EFFEKSEERRENDERER_GL_RENDERSTATE_H__
#define	__EFFEKSEERRENDERER_GL_RENDERSTATE_H__

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
	RendererImplemented*	m_renderer;
	bool					m_isCCW = true;

	GLuint					m_samplers[4];


public:
	RenderState( RendererImplemented* renderer );
	virtual ~RenderState();

	void Update( bool forced );
};

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
}
//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
#endif	// __EFFEKSEERRENDERER_GL_RENDERSTATE_H__
#ifndef	__EFFEKSEERRENDERER_GL_SHADER_H__
#define	__EFFEKSEERRENDERER_GL_SHADER_H__

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
	const char*	name;
	GLenum		type;
	uint16_t	count;
	uint16_t	offset;
	bool		normalized;
};

struct ShaderUniformInfo
{
	const char*	name;
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
	: public DeviceObject
	, public ::EffekseerRenderer::ShaderBase
{
private:
	struct Layout
	{
		GLenum		type;
		uint16_t	count;
		uint16_t	offset;
		bool		normalized;
	};

	struct ShaderAttribInfoInternal
	{
		std::string	name;
		GLenum		type;
		uint16_t	count;
		uint16_t	offset;
		bool		normalized;
	};

	struct ShaderUniformInfoInternal
	{
		std::string	name;
	};

	struct ConstantLayout
	{
		eConstantType	Type;
		GLint			ID;
		int32_t			Offset;
	};

	GLuint m_program;

	std::vector<GLint>		m_aid;
	std::vector<Layout>		m_layout;

	int32_t					m_vertexSize;

	uint8_t*					m_vertexConstantBuffer;
	uint8_t*					m_pixelConstantBuffer;

	std::vector<ConstantLayout>	m_vertexConstantLayout;
	std::vector<ConstantLayout>	m_pixelConstantLayout;

	GLuint	m_textureSlots[4];
	bool	m_textureSlotEnables[4];

	std::vector<char>	m_vsSrc;
	std::vector<char>	m_psSrc;
	std::string				m_name;

	std::vector<ShaderAttribInfoInternal>	attribs;
	std::vector<ShaderUniformInfoInternal>	uniforms;

	static bool CompileShader(
		RendererImplemented* renderer,
		GLuint& program,
		const char* vs_src,
		size_t vertexShaderSize,
		const char* fs_src,
		size_t pixelShaderSize,
		const char* name);

	Shader(
		RendererImplemented* renderer, 
		GLuint program,
		const char* vs_src,
		size_t vertexShaderSize,
		const char* fs_src,
		size_t pixelShaderSize,
		const char* name);

	GLint GetAttribId(const char* name) const;

public:
	GLint GetUniformId(const char* name) const;

public:
	virtual ~Shader();

	static Shader* Create(
		RendererImplemented* renderer,
		const char* vs_src,
		size_t vertexShaderSize,
		const char* fs_src,
		size_t pixelShaderSize,
		const char* name);

public:	// デバイス復旧用
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

	void* GetVertexConstantBuffer()  override { return m_vertexConstantBuffer; }
	void* GetPixelConstantBuffer()  override { return m_pixelConstantBuffer; }

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
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#endif	// __EFFEKSEERRENDERER_GL_SHADER_H__

#ifdef __EFFEKSEER_RENDERER_INTERNAL_LOADER__

#ifndef	__EFFEKSEERRENDERER_GL_TEXTURELOADER_H__
#define	__EFFEKSEERRENDERER_GL_TEXTURELOADER_H__

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

#ifdef __EFFEKSEER_RENDERER_INTERNAL_LOADER__
	::EffekseerRenderer::PngTextureLoader pngTextureLoader;
	::EffekseerRenderer::DDSTextureLoader ddsTextureLoader;
#endif

public:
	TextureLoader(::Effekseer::FileInterface* fileInterface = NULL);
	virtual ~TextureLoader();

public:
	Effekseer::TextureData* Load(const EFK_CHAR* path, ::Effekseer::TextureType textureType) override;

	void Unload(Effekseer::TextureData* data) override;
};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
}
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
#endif	// __EFFEKSEERRENDERER_GL_TEXTURELOADER_H__

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
	GLuint					m_vertexArray;

	Shader*					m_shader;
	VertexBuffer*			m_vertexBuffer;
	IndexBuffer*			m_indexBuffer;

	VertexArray( RendererImplemented* renderer, Shader* shader, 
		VertexBuffer* vertexBuffer, IndexBuffer* indexBuffer );
public:
	virtual ~VertexArray();

	static VertexArray* Create( RendererImplemented* renderer, Shader* shader, 
		VertexBuffer* vertexBuffer, IndexBuffer* indexBuffer );

	GLuint GetInterface() const { return m_vertexArray; }

	VertexBuffer* GetVertexBuffer() { return m_vertexBuffer; }
	IndexBuffer* GetIndexBuffer() { return m_indexBuffer; }

public:	// デバイス復旧用
	virtual void OnLostDevice();
	virtual void OnResetDevice();

private:
	void Init();
	void Release();
};

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
}
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
	: public DeviceObject
	, public ::EffekseerRenderer::VertexBufferBase
{
private:
	GLuint					m_buffer;

	uint32_t				m_vertexRingStart;
	uint32_t				m_vertexRingOffset;
	bool					m_ringBufferLock;

	VertexBuffer( RendererImplemented* renderer, int size, bool isDynamic );
public:
	virtual ~VertexBuffer();

	static VertexBuffer* Create( RendererImplemented* renderer, int size, bool isDynamic );

	GLuint GetInterface();

public:	// デバイス復旧用
	virtual void OnLostDevice();
	virtual void OnResetDevice();

public:
	void Lock();
	bool RingBufferLock( int32_t size, int32_t& offset, void*& data );
	bool TryRingBufferLock(int32_t size, int32_t& offset, void*& data);

	void Unlock();

	bool IsValid();
};

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
}
//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
// Include
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
namespace EffekseerRendererGL
{
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
DeviceObject::DeviceObject( RendererImplemented* renderer )
	: m_renderer	( renderer )
{
	ES_SAFE_ADDREF( m_renderer );
	m_renderer->m_deviceObjects.insert( this );
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
DeviceObject::~DeviceObject()
{
	m_renderer->m_deviceObjects.erase( this );
	ES_SAFE_RELEASE( m_renderer );
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
RendererImplemented* DeviceObject::GetRenderer() const
{
	return m_renderer;
}

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

typedef void (EFK_STDCALL * FP_glDeleteBuffers) (GLsizei n, const GLuint* buffers);
typedef GLuint (EFK_STDCALL * FP_glCreateShader) (GLenum type);
typedef void (EFK_STDCALL * FP_glBindBuffer) (GLenum target, GLuint buffer);
typedef void (EFK_STDCALL * FP_glGenBuffers) (GLsizei n, GLuint* buffers);
typedef void (EFK_STDCALL * FP_glBufferData) (GLenum target, GLsizeiptr size, const GLvoid* data, GLenum usage);

typedef void (EFK_STDCALL * FP_glBlendEquationSeparate) (GLenum modeRGB, GLenum modeAlpha);
typedef void (EFK_STDCALL * FP_glBlendFuncSeparate) (GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha);

typedef void (EFK_STDCALL * FP_glBlendEquation) (GLenum mode);

typedef void (EFK_STDCALL * FP_glActiveTexture) (GLenum texture);

typedef void (EFK_STDCALL * FP_glUniform1i) (GLint location, GLint v0);

typedef void (EFK_STDCALL * FP_glShaderSource) (GLuint shader, GLsizei count, const GLchar** strings, const GLint* lengths);

typedef void (EFK_STDCALL * FP_glCompileShader) (GLuint shader);

typedef void (EFK_STDCALL * FP_glGetShaderiv) (GLuint shader, GLenum pname, GLint* param);

typedef GLuint (EFK_STDCALL * FP_glCreateProgram) (void);

typedef void (EFK_STDCALL * FP_glAttachShader) (GLuint program, GLuint shader);

typedef void (EFK_STDCALL * FP_glDeleteProgram) (GLuint program);
typedef void (EFK_STDCALL * FP_glDeleteShader) (GLuint shader);
typedef void (EFK_STDCALL * FP_glLinkProgram) (GLuint program);

typedef void (EFK_STDCALL * FP_glGetProgramiv) (GLuint program, GLenum pname, GLint* param);

typedef void (EFK_STDCALL * FP_glGetShaderInfoLog) (GLuint shader, GLsizei bufSize, GLsizei* length, GLchar* infoLog);
typedef void (EFK_STDCALL * FP_glGetProgramInfoLog) (GLuint program, GLsizei bufSize, GLsizei* length, GLchar* infoLog);

typedef GLint (EFK_STDCALL * FP_glGetAttribLocation) (GLuint program, const GLchar* name);

typedef GLint (EFK_STDCALL * FP_glGetUniformLocation) (GLuint program, const GLchar* name);

typedef void (EFK_STDCALL * FP_glUseProgram) (GLuint program);

typedef void (EFK_STDCALL * FP_glEnableVertexAttribArray) (GLuint index);
typedef void (EFK_STDCALL * FP_glDisableVertexAttribArray) (GLuint index);
typedef void (EFK_STDCALL * FP_glVertexAttribPointer) (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* pointer);
typedef void (EFK_STDCALL * FP_glUniformMatrix4fv) (GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void (EFK_STDCALL * FP_glUniform4fv) (GLint location, GLsizei count, const GLfloat* value);
typedef void (EFK_STDCALL * FP_glGenerateMipmap) (GLenum target);
typedef void (EFK_STDCALL * FP_glBufferSubData) (GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid* data);

typedef void (EFK_STDCALL * FP_glGenVertexArrays) (GLsizei n, GLuint *arrays);
typedef void (EFK_STDCALL * FP_glDeleteVertexArrays) (GLsizei n, const GLuint *arrays);
typedef void (EFK_STDCALL * FP_glBindVertexArray) (GLuint array);

typedef void (EFK_STDCALL * FP_glGenSamplers) (GLsizei n, GLuint *samplers);
typedef void (EFK_STDCALL * FP_glDeleteSamplers) (GLsizei n, const GLuint * samplers);
typedef void (EFK_STDCALL * FP_glSamplerParameteri) (GLuint sampler, GLenum pname, GLint param);
typedef void (EFK_STDCALL * FP_glBindSampler) (GLuint unit, GLuint sampler);

typedef void* (EFK_STDCALL *FP_glMapBuffer)(GLenum target, GLenum access);
typedef void* (EFK_STDCALL *FP_glMapBufferRange)(GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access);
typedef GLboolean(EFK_STDCALL *FP_glUnmapBuffer)(GLenum target);

typedef void (EFK_STDCALL * FP_glCompressedTexImage2D) (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void *data);

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

typedef void (* FP_glGenVertexArraysOES) (GLsizei n, GLuint *arrays);
typedef void (* FP_glDeleteVertexArraysOES) (GLsizei n, const GLuint *arrays);
typedef void (* FP_glBindVertexArrayOES) (GLuint array);

typedef void* (* FP_glMapBufferOES)(GLenum target, GLenum access);
typedef void* (* FP_glMapBufferRangeEXT)(GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access);
typedef GLboolean (* FP_glUnmapBufferOES)(GLenum target);

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
#define GET_PROC(name)	g_##name = (FP_##name)wglGetProcAddress( #name ); if(g_##name==NULL) return false;
#elif defined(__EFFEKSEER_RENDERER_GLES2__) || defined(__EFFEKSEER_RENDERER_GLES3__)
#define GET_PROC(name)	g_##name = (FP_##name)eglGetProcAddress( #name ); if(g_##name==NULL) return false;
#endif

OpenGLDeviceType GetDeviceType()
{
    return g_deviceType;
}
    
bool Initialize(OpenGLDeviceType deviceType)
{
	if(g_isInitialized) return true;
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
	char *glExtensions = (char *)glGetString(GL_EXTENSIONS);
	g_isSupportedVertexArray = (g_glGenVertexArraysOES && g_glDeleteVertexArraysOES && g_glBindVertexArrayOES
		&& ((glExtensions && strstr(glExtensions, "GL_OES_vertex_array_object")) ? true : false));

	// Some smartphone causes segmentation fault.
	//GET_PROC(glMapBufferRangeEXT);

	GET_PROC(glMapBufferOES);
	GET_PROC(glUnmapBufferOES);
	g_isSurrpotedBufferRange = (g_glMapBufferRangeEXT && g_glUnmapBufferOES);
	g_isSurrpotedMapBuffer = (g_glMapBufferOES && g_glUnmapBufferOES 
		&& ((glExtensions && strstr(glExtensions, "GL_OES_mapbuffer")) ? true : false));
#endif

#else
	if (deviceType == OpenGLDeviceType::OpenGL3 ||
		deviceType == OpenGLDeviceType::OpenGLES3)
	{
		g_isSupportedVertexArray = true;
		g_isSurrpotedBufferRange = true;
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

void glBufferData (GLenum target, GLsizeiptr size, const GLvoid* data, GLenum usage)
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

void glBlendFuncSeparate (GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha)
{
#if _WIN32
	g_glBlendFuncSeparate (sfactorRGB, dfactorRGB, sfactorAlpha, dfactorAlpha);
#else
	::glBlendFuncSeparate (sfactorRGB, dfactorRGB, sfactorAlpha, dfactorAlpha);
#endif
}

void glBlendEquation (GLenum mode)
{
#if _WIN32
	g_glBlendEquation (mode);
#else
	::glBlendEquation (mode);
#endif
}

void glActiveTexture (GLenum texture)
{
#if _WIN32
	g_glActiveTexture (texture);
#else
	::glActiveTexture (texture);
#endif
}

void glUniform1i(GLint location, GLint v0)
{
#if _WIN32
	g_glUniform1i (location, v0);
#else
	::glUniform1i (location, v0);
#endif
}

void glShaderSource (GLuint shader, GLsizei count, const GLchar** strings, const GLint* lengths)
{
#if _WIN32
	g_glShaderSource (shader, count, strings, lengths);
#else
	::glShaderSource (shader, count, strings, lengths);
#endif
}

void glCompileShader(GLuint shader)
{
#if _WIN32
	g_glCompileShader (shader);
#else
	::glCompileShader (shader);
#endif
}

void glGetShaderiv (GLuint shader, GLenum pname, GLint* param)
{
#if _WIN32
	g_glGetShaderiv (shader, pname, param);
#else
	::glGetShaderiv (shader, pname, param);
#endif
}

GLuint glCreateProgram (void)
{
#if _WIN32
	return g_glCreateProgram ();
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

void glLinkProgram (GLuint program)
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

void glGetShaderInfoLog (GLuint shader, GLsizei bufSize, GLsizei* length, GLchar* infoLog)
{
#if _WIN32
	g_glGetShaderInfoLog(shader, bufSize, length, infoLog);
#else
	::glGetShaderInfoLog(shader, bufSize, length, infoLog);
#endif
}

void glGetProgramInfoLog (GLuint program, GLsizei bufSize, GLsizei* length, GLchar* infoLog)
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
	g_glVertexAttribPointer(index, size, type, normalized ,stride, pointer);
#else
	::glVertexAttribPointer(index, size, type, normalized ,stride, pointer);
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

void glGenVertexArrays(GLsizei n, GLuint *arrays)
{
#if _WIN32
	g_glGenVertexArrays(n, arrays);
#elif defined(__EFFEKSEER_RENDERER_GLES2__)
	g_glGenVertexArraysOES(n, arrays);
#else
	::glGenVertexArrays(n, arrays);
#endif
}

void glDeleteVertexArrays(GLsizei n, const GLuint *arrays)
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

void glGenSamplers(GLsizei n, GLuint *samplers)
{
#if _WIN32
	g_glGenSamplers(n, samplers);
#elif defined(__EFFEKSEER_RENDERER_GLES2__) || defined(__EFFEKSEER_RENDERER_GL2__)
#else
	::glGenSamplers(n, samplers);
#endif
}

void glDeleteSamplers(GLsizei n, const GLuint * samplers)
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

void glCompressedTexImage2D(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void *data)
{
#if _WIN32
	g_glCompressedTexImage2D(target, level,internalformat, width, height, border,imageSize, data);
#elif defined(__EFFEKSEER_RENDERER_GLES2__) || defined(__EFFEKSEER_RENDERER_GL2__)
#else
	::glCompressedTexImage2D(target, level,internalformat, width, height, border,imageSize, data);
#endif
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
}
}

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
IndexBuffer::IndexBuffer( RendererImplemented* renderer, GLuint buffer, int maxCount, bool isDynamic )
	: DeviceObject		( renderer )
	, IndexBufferBase	( maxCount, isDynamic )
	, m_buffer			( buffer )
{
	m_resource = new uint8_t[m_indexMaxCount * sizeof(uint16_t)];
}

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
IndexBuffer::~IndexBuffer()
{
	delete [] m_resource;
	GLExt::glDeleteBuffers(1, &m_buffer);
}

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
IndexBuffer* IndexBuffer::Create( RendererImplemented* renderer, int maxCount, bool isDynamic )
{
	GLuint ib;
	GLExt::glGenBuffers(1, &ib);
	return new IndexBuffer( renderer, ib, maxCount, isDynamic );
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
	if (IsValid()) return;
	GLuint ib;
	GLExt::glGenBuffers(1, &ib);
	m_buffer = ib;
}

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
void IndexBuffer::Lock()
{
	assert( !m_isLock );

	m_isLock = true;
	m_indexCount = 0;
}

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
void IndexBuffer::Unlock()
{
	assert( m_isLock );

	GLExt::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffer);
	GLExt::glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indexCount * sizeof(uint16_t), m_resource, GL_DYNAMIC_DRAW);
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
}
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
ModelLoader::ModelLoader( ::Effekseer::FileInterface* fileInterface )
	: m_fileInterface	( fileInterface )
{
	if( m_fileInterface == NULL )
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

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void* ModelLoader::Load( const EFK_CHAR* path )
{
	std::unique_ptr<Effekseer::FileReader> 
		reader( m_fileInterface->OpenRead( path ) );
	
	if( reader.get() != NULL )
	{
		size_t size_model = reader->GetLength();
		char* data_model = new char[size_model];
		reader->Read( data_model, size_model );

		Model* model = new Model(data_model, (int32_t)size_model);

		delete [] data_model;

		return (void*)model;
	}

	return NULL;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void ModelLoader::Unload( void* data )
{
	if( data != NULL )
	{
		Model* model = (Model*)data;
		delete model;
	}
}

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
namespace EffekseerRendererGL
{
//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
static std::string Replace( std::string target, std::string from_, std::string to_ )
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
OUT mediump vec4 v_TexCoord;
OUT lowp vec4 v_Color;
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
		float diffuse = 1.0;
		if (NormalMapEnable)
		{
			v_Normal = vec4(localNormal, 1.0);
			v_Binormal = vec4(normalize( lightMatrix * a_Binormal.xyz ), 1.0);
			v_Tangent = vec4(normalize( lightMatrix * a_Tangent.xyz ), 1.0);
		}
		else
		{
			diffuse = max(0.0, dot(localNormal, LightDirection.xyz));
		}
		v_Color = modelColor * vec4(diffuse * LightColor.rgb, 1.0);
	}
	else
	{
		v_Color = modelColor;
	}

	v_TexCoord.y = mUVInversed.x + mUVInversed.y * v_TexCoord.y;
}
)";

static const char g_model_fs_src[] = 
R"(

IN mediump vec4 v_Normal;
IN mediump vec4 v_Binormal;
IN mediump vec4 v_Tangent;
IN mediump vec4 v_TexCoord;
IN lowp vec4 v_Color;

uniform sampler2D ColorTexture;
uniform sampler2D NormalTexture;
uniform vec4 LightDirection;
uniform vec4 LightColor;
uniform vec4 LightAmbient;

void main()
{
	vec4 diffuse = vec4(1.0);
	if (LightingEnable && NormalMapEnable)
	{
		vec3 texNormal = (TEX2D(NormalTexture, v_TexCoord.xy).xyz - 0.5) * 2.0;
		mat3 normalMatrix = mat3(v_Tangent.xyz, v_Binormal.xyz, v_Normal.xyz );
		vec3 localNormal = normalize( normalMatrix * texNormal );
		diffuse = vec4(max(0.0, dot(localNormal, LightDirection.xyz)));
	}
	if (TextureEnable)
	{
		FRAGCOLOR = v_Color * TEX2D(ColorTexture, v_TexCoord.xy);
		FRAGCOLOR.xyz = FRAGCOLOR.xyz * diffuse.xyz;
	} else
	{
		FRAGCOLOR = v_Color;
		FRAGCOLOR.xyz = FRAGCOLOR.xyz * diffuse.xyz;
	}
  
	if (LightingEnable)
	{
		FRAGCOLOR.xyz = FRAGCOLOR.xyz + LightAmbient.xyz;
	}
}

)";


static const char g_model_distortion_vs_src [] =
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
OUT mediump vec4 v_TexCoord;
OUT mediump vec4 v_Pos;
OUT lowp vec4 v_Color;
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

static const char g_model_distortion_fs_src [] =
"IN mediump vec4 v_Normal;\n"
"IN mediump vec4 v_Binormal;\n"
"IN mediump vec4 v_Tangent;\n"
"IN mediump vec4 v_TexCoord;\n"
"IN mediump vec4 v_Pos;\n"
"IN lowp vec4 v_Color;\n"

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
}
)";

static ShaderAttribInfo g_model_attribs[ModelRenderer::NumAttribs] = {
	{"a_Position",		GL_FLOAT,			3,  0,	false},
	{"a_Normal",		GL_FLOAT,			3, 12,	false},
	{"a_Binormal",		GL_FLOAT,			3, 24,	false},
	{"a_Tangent",		GL_FLOAT,			3, 36,	false},
	{"a_TexCoord",		GL_FLOAT,			2, 48,	false},
	{"a_Color", GL_UNSIGNED_BYTE,			4, 56,	true },
#if defined(MODEL_SOFTWARE_INSTANCING)
	{"a_InstanceID",	GL_FLOAT,			1,  0,	false},
	{"a_UVOffset",		GL_FLOAT,			4,	0,	false},
	{"a_ModelColor",	GL_FLOAT,			4,  0,	false},
#endif
};

static ShaderUniformInfo g_model_uniforms[ModelRenderer::NumUniforms] = {
	{"ProjectionMatrix"	},
	{"ModelMatrix"		},
#if !defined(MODEL_SOFTWARE_INSTANCING)
	{"UVOffset"			},
	{"ModelColor"		},
#endif
	{"ColorTexture"		},
	{"NormalTexture"	},
	{"LightDirection"	},
	{"LightColor"		},
	{"LightAmbient"		},
	{ "g_scale" },
	{ "mUVInversed" },
	{ "mUVInversedBack" },
};

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
ModelRenderer::ModelRenderer(
	RendererImplemented* renderer,
	Shader* shader_lighting_texture_normal,
	Shader* shader_lighting_normal,
	Shader* shader_lighting_texture,
	Shader* shader_lighting,
	Shader* shader_texture,
	Shader* shader,
	Shader* shader_distortion_texture,
	Shader* shader_distortion)
	: m_renderer(renderer)
	, m_shader_lighting_texture_normal(shader_lighting_texture_normal)
	, m_shader_lighting_normal(shader_lighting_normal)
	, m_shader_lighting_texture(shader_lighting_texture)
	, m_shader_lighting(shader_lighting)
	, m_shader_texture(shader_texture)
	, m_shader(shader)
	, m_shader_distortion_texture(shader_distortion_texture)
	, m_shader_distortion(shader_distortion)
{
	for (size_t i = 0; i < 8; i++)
	{
		m_va[i] = nullptr;
	}

	shader_lighting_texture_normal->GetAttribIdList(NumAttribs, g_model_attribs);
	shader_lighting_texture_normal->GetUniformIdList(NumUniforms, g_model_uniforms, m_uniformLoc[0]);
	shader_lighting_texture_normal->SetTextureSlot(0, shader_lighting_texture_normal->GetUniformId("ColorTexture"));
	shader_lighting_texture_normal->SetTextureSlot(1, shader_lighting_texture_normal->GetUniformId("NormalTexture"));


	shader_lighting_normal->GetAttribIdList(NumAttribs, g_model_attribs);
	shader_lighting_normal->GetUniformIdList(NumUniforms, g_model_uniforms, m_uniformLoc[1]);
	shader_lighting_normal->SetTextureSlot(1, shader_lighting_normal->GetUniformId("NormalTexture"));


	shader_lighting_texture->GetAttribIdList(NumAttribs, g_model_attribs);
	shader_lighting_texture->GetUniformIdList(NumUniforms, g_model_uniforms, m_uniformLoc[2]);
	shader_lighting_texture->SetTextureSlot(0, shader_lighting_texture->GetUniformId("ColorTexture"));

	shader_lighting->GetAttribIdList(NumAttribs, g_model_attribs);
	shader_lighting->GetUniformIdList(NumUniforms, g_model_uniforms, m_uniformLoc[3]);

	shader_texture->GetAttribIdList(NumAttribs, g_model_attribs);
	shader_texture->GetUniformIdList(NumUniforms, g_model_uniforms, m_uniformLoc[4]);
	shader_texture->SetTextureSlot(0, shader_texture->GetUniformId("ColorTexture"));
	
	shader->GetAttribIdList(NumAttribs, g_model_attribs);
	shader->GetUniformIdList(NumUniforms, g_model_uniforms, m_uniformLoc[5]);

	shader_distortion_texture->GetAttribIdList(NumAttribs, g_model_attribs);
	shader_distortion_texture->GetUniformIdList(NumUniforms, g_model_uniforms, m_uniformLoc[6]);
	shader_distortion_texture->SetTextureSlot(0, shader_distortion_texture->GetUniformId("uTexture0"));
	shader_distortion_texture->SetTextureSlot(1, shader_distortion_texture->GetUniformId("uBackTexture0"));

	shader_distortion->GetAttribIdList(NumAttribs, g_model_attribs);
	shader_distortion->GetUniformIdList(NumUniforms, g_model_uniforms, m_uniformLoc[7]);
	shader_distortion->SetTextureSlot(0, shader_distortion->GetUniformId("uTexture0"));
	shader_distortion->SetTextureSlot(1, shader_distortion->GetUniformId("uBackTexture0"));

	Shader* shaders[6];
	shaders[0] = m_shader_lighting_texture_normal;
	shaders[1] = m_shader_lighting_normal;
	shaders[2] = m_shader_lighting_texture;
	shaders[3] = m_shader_lighting;
	shaders[4] = m_shader_texture;
	shaders[5] = m_shader;
	
	for( int32_t i = 0; i < 6; i++ )
	{
		shaders[i]->SetVertexSize(sizeof(::Effekseer::Model::Vertex));

		shaders[i]->SetVertexConstantBufferSize(sizeof(::EffekseerRenderer::ModelRendererVertexConstantBuffer<1>));
		shaders[i]->AddVertexConstantLayout(
			CONSTANT_TYPE_MATRIX44,
			shaders[i]->GetUniformId("ProjectionMatrix"),
			0
			);

		shaders[i]->AddVertexConstantLayout(
			CONSTANT_TYPE_MATRIX44,
			shaders[i]->GetUniformId("ModelMatrix"),
			sizeof(Effekseer::Matrix44)
			);

		shaders[i]->AddVertexConstantLayout(
			CONSTANT_TYPE_VECTOR4,
			shaders[i]->GetUniformId("UVOffset"),
			sizeof(Effekseer::Matrix44) * 2
			);

		shaders[i]->AddVertexConstantLayout(
			CONSTANT_TYPE_VECTOR4,
			shaders[i]->GetUniformId("ModelColor"),
			sizeof(Effekseer::Matrix44) * 2 + sizeof(float[4]) * 1
			);

		shaders[i]->AddVertexConstantLayout(
			CONSTANT_TYPE_VECTOR4,
			shaders[i]->GetUniformId("LightDirection"),
			sizeof(Effekseer::Matrix44) * 2 + sizeof(float[4]) * 2 + sizeof(float[4]) * 0
			);
		shaders[i]->AddVertexConstantLayout(
			CONSTANT_TYPE_VECTOR4,
			shaders[i]->GetUniformId("LightColor"),
			sizeof(Effekseer::Matrix44) * 2 + sizeof(float[4]) * 2 + sizeof(float[4]) * 1
			);
		shaders[i]->AddVertexConstantLayout(
			CONSTANT_TYPE_VECTOR4,
			shaders[i]->GetUniformId("LightAmbient"),
			sizeof(Effekseer::Matrix44) * 2 + sizeof(float[4]) * 2 + sizeof(float[4]) * 2
			);
		shaders[i]->AddVertexConstantLayout(
			CONSTANT_TYPE_VECTOR4,
			shaders[i]->GetUniformId("mUVInversed"),
			sizeof(Effekseer::Matrix44) * 2 + sizeof(float[4]) * 2 + sizeof(float[4]) * 3
		);

		shaders[i]->SetPixelConstantBufferSize(sizeof(::EffekseerRenderer::ModelRendererPixelConstantBuffer));
		shaders[i]->AddPixelConstantLayout(
			CONSTANT_TYPE_VECTOR4,
			shaders[i]->GetUniformId("LightDirection"),
			sizeof(float[4]) * 0
			);
		shaders[i]->AddPixelConstantLayout(
			CONSTANT_TYPE_VECTOR4,
			shaders[i]->GetUniformId("LightColor"),
			sizeof(float[4]) * 1
			);
		shaders[i]->AddPixelConstantLayout(
			CONSTANT_TYPE_VECTOR4,
			shaders[i]->GetUniformId("LightAmbient"),
			sizeof(float[4]) * 2
			);
	}

	Shader* shaders_d[2];
	shaders_d[0] = shader_distortion_texture;
	shaders_d[1] = shader_distortion;

	for (int32_t i = 0; i < 2; i++)
	{
		shaders_d[i]->SetVertexSize(sizeof(::Effekseer::Model::Vertex));

		shaders_d[i]->SetVertexConstantBufferSize(sizeof(::EffekseerRenderer::ModelRendererVertexConstantBuffer<1>));
		shaders_d[i]->AddVertexConstantLayout(
			CONSTANT_TYPE_MATRIX44,
			shaders_d[i]->GetUniformId("ProjectionMatrix"),
			0
			);

		shaders_d[i]->AddVertexConstantLayout(
			CONSTANT_TYPE_MATRIX44,
			shaders_d[i]->GetUniformId("ModelMatrix"),
			sizeof(Effekseer::Matrix44)
			);

		shaders_d[i]->AddVertexConstantLayout(
			CONSTANT_TYPE_VECTOR4,
			shaders_d[i]->GetUniformId("UVOffset"),
			sizeof(Effekseer::Matrix44) * 2
			);

		shaders_d[i]->AddVertexConstantLayout(
			CONSTANT_TYPE_VECTOR4,
			shaders_d[i]->GetUniformId("ModelColor"),
			sizeof(Effekseer::Matrix44) * 2 + sizeof(float[4]) * 1
			);

		shaders_d[i]->AddVertexConstantLayout(
			CONSTANT_TYPE_VECTOR4,
			shaders_d[i]->GetUniformId("LightDirection"),
			sizeof(Effekseer::Matrix44) * 2 + sizeof(float[4]) * 2 + sizeof(float[4]) * 0
			);

		shaders_d[i]->AddVertexConstantLayout(
			CONSTANT_TYPE_VECTOR4,
			shaders_d[i]->GetUniformId("LightColor"),
			sizeof(Effekseer::Matrix44) * 2 + sizeof(float[4]) * 2 + sizeof(float[4]) * 1
			);

		shaders_d[i]->AddVertexConstantLayout(
			CONSTANT_TYPE_VECTOR4,
			shaders_d[i]->GetUniformId("LightAmbient"),
			sizeof(Effekseer::Matrix44) * 2 + sizeof(float[4]) * 2 + sizeof(float[4]) * 2
			);

		shaders_d[i]->AddVertexConstantLayout(
			CONSTANT_TYPE_VECTOR4,
			shaders_d[i]->GetUniformId("mUVInversed"),
			sizeof(Effekseer::Matrix44) * 2 + sizeof(float[4]) * 2 + sizeof(float[4]) * 3
		);


		shaders_d[i]->SetPixelConstantBufferSize(sizeof(float) * 4 * 2);
		shaders_d[i]->AddPixelConstantLayout(
			CONSTANT_TYPE_VECTOR4,
			shaders_d[i]->GetUniformId("g_scale"),
			sizeof(float[4]) * 0
			);


		shaders_d[i]->AddPixelConstantLayout(
			CONSTANT_TYPE_VECTOR4,
			shaders_d[i]->GetUniformId("mUVInversedBack"),
			sizeof(float[4]) * 1
			);
	}

	m_va[0] = VertexArray::Create(renderer, m_shader_lighting_texture_normal, nullptr, nullptr);
	m_va[1] = VertexArray::Create(renderer, m_shader_lighting_normal, nullptr, nullptr);

	m_va[2] = VertexArray::Create(renderer, m_shader_lighting_texture, nullptr, nullptr);
	m_va[3] = VertexArray::Create(renderer, m_shader_lighting, nullptr, nullptr);

	m_va[4] = VertexArray::Create(renderer, m_shader_texture, nullptr, nullptr);
	m_va[5] = VertexArray::Create(renderer, m_shader, nullptr, nullptr);

	m_va[6] = VertexArray::Create(renderer, m_shader_distortion_texture, nullptr, nullptr);
	m_va[7] = VertexArray::Create(renderer, m_shader_distortion, nullptr, nullptr);

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
	ES_SAFE_DELETE(m_shader_lighting_normal);

	ES_SAFE_DELETE(m_shader_lighting_texture);
	ES_SAFE_DELETE(m_shader_lighting);

	ES_SAFE_DELETE(m_shader_texture);
	ES_SAFE_DELETE(m_shader);

	ES_SAFE_DELETE(m_shader_distortion_texture);
	ES_SAFE_DELETE(m_shader_distortion);
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
ModelRenderer* ModelRenderer::Create( RendererImplemented* renderer )
{
	assert( renderer != NULL );

	Shader* shader_lighting_texture_normal = NULL;
	Shader* shader_lighting_normal = NULL;
		    
	Shader* shader_lighting_texture = NULL;
	Shader* shader_lighting = NULL;
		    
	Shader* shader_texture = NULL;
	Shader* shader = NULL;

	Shader* shader_distortion_texture = NULL;
	Shader* shader_distortion = NULL;

	std::string vs_ltn_src = g_model_vs_src;
	std::string fs_ltn_src = g_model_fs_src;

	std::string vs_ln_src = g_model_vs_src;
	std::string fs_ln_src = g_model_fs_src;

	std::string vs_lt_src = g_model_vs_src;
	std::string fs_lt_src = g_model_fs_src;

	std::string vs_l_src = g_model_vs_src;
	std::string fs_l_src = g_model_fs_src;

	std::string vs_t_src = g_model_vs_src;
	std::string fs_t_src = g_model_fs_src;

	std::string vs_src = g_model_vs_src;
	std::string fs_src = g_model_fs_src;

	std::string vs_d_t_src = g_model_distortion_vs_src;
	std::string fs_d_t_src = g_model_distortion_fs_src;

	std::string vs_d_src = g_model_distortion_vs_src;
	std::string fs_d_src = g_model_distortion_fs_src;

	vs_ltn_src = Replace(vs_ltn_src, "TextureEnable", "true");
	fs_ltn_src = Replace(fs_ltn_src, "TextureEnable", "true");
	vs_ltn_src = Replace(vs_ltn_src, "LightingEnable", "true");
	fs_ltn_src = Replace(fs_ltn_src, "LightingEnable", "true");
	vs_ltn_src = Replace(vs_ltn_src, "NormalMapEnable", "true");
	fs_ltn_src = Replace(fs_ltn_src, "NormalMapEnable", "true");

	vs_ln_src = Replace(vs_ln_src, "TextureEnable", "false");
	fs_ln_src = Replace(fs_ln_src, "TextureEnable", "false");
	vs_ln_src = Replace(vs_ln_src, "LightingEnable", "true");
	fs_ln_src = Replace(fs_ln_src, "LightingEnable", "true");
	vs_ln_src = Replace(vs_ln_src, "NormalMapEnable", "true");
	fs_ln_src = Replace(fs_ln_src, "NormalMapEnable", "true");

	vs_lt_src = Replace(vs_lt_src, "TextureEnable", "true");
	fs_lt_src = Replace(fs_lt_src, "TextureEnable", "true");
	vs_lt_src = Replace(vs_lt_src, "LightingEnable", "true");
	fs_lt_src = Replace(fs_lt_src, "LightingEnable", "true");
	vs_lt_src = Replace(vs_lt_src, "NormalMapEnable", "false");
	fs_lt_src = Replace(fs_lt_src, "NormalMapEnable", "false");

	vs_l_src = Replace(vs_l_src, "TextureEnable", "false");
	fs_l_src = Replace(fs_l_src, "TextureEnable", "false");
	vs_l_src = Replace(vs_l_src, "LightingEnable", "true");
	fs_l_src = Replace(fs_l_src, "LightingEnable", "true");
	vs_l_src = Replace(vs_l_src, "NormalMapEnable", "false");
	fs_l_src = Replace(fs_l_src, "NormalMapEnable", "false");

	vs_t_src = Replace(vs_t_src, "TextureEnable", "true");
	fs_t_src = Replace(fs_t_src, "TextureEnable", "true");
	vs_t_src = Replace(vs_t_src, "LightingEnable", "false");
	fs_t_src = Replace(fs_t_src, "LightingEnable", "false");
	vs_t_src = Replace(vs_t_src, "NormalMapEnable", "false");
	fs_t_src = Replace(fs_t_src, "NormalMapEnable", "false");

	vs_src = Replace(vs_src, "TextureEnable", "false");
	fs_src = Replace(fs_src, "TextureEnable", "false");
	vs_src = Replace(vs_src, "LightingEnable", "false");
	fs_src = Replace(fs_src, "LightingEnable", "false");
	vs_src = Replace(vs_src, "NormalMapEnable", "false");
	fs_src = Replace(fs_src, "NormalMapEnable", "false");

	vs_d_t_src = Replace(vs_d_t_src, "TextureEnable", "true");
	fs_d_t_src = Replace(fs_d_t_src, "TextureEnable", "true");

	vs_d_src = Replace(vs_d_src, "TextureEnable", "false");
	fs_d_src = Replace(fs_d_src, "TextureEnable", "false");

	shader_lighting_texture_normal = Shader::Create(renderer,
		vs_ltn_src.c_str(), vs_ltn_src.length(), fs_ltn_src.c_str(), fs_ltn_src.length(), "ModelRenderer1");
	if (shader_lighting_texture_normal == NULL) goto End;

	shader_lighting_normal = Shader::Create(renderer,
		vs_ln_src.c_str(), vs_ln_src.length(), fs_ln_src.c_str(), fs_ln_src.length(), "ModelRenderer2");
	if (shader_lighting_normal == NULL) goto End;

	shader_lighting_texture = Shader::Create(renderer,
		vs_lt_src.c_str(), vs_lt_src.length(), fs_lt_src.c_str(), fs_lt_src.length(), "ModelRenderer3");
	if (shader_lighting_texture == NULL) goto End;

	shader_lighting = Shader::Create(renderer,
		vs_l_src.c_str(), vs_l_src.length(), fs_l_src.c_str(), fs_l_src.length(), "ModelRenderer4");
	if (shader_lighting == NULL) goto End;

	shader_texture = Shader::Create(renderer,
		vs_t_src.c_str(), vs_t_src.length(), fs_t_src.c_str(), fs_t_src.length(), "ModelRenderer5");
	if (shader_texture == NULL) goto End;

	shader = Shader::Create( renderer, 
		vs_src.c_str(), vs_src.length(), fs_src.c_str(), fs_src.length(), "ModelRenderer6");
	if (shader == NULL) goto End;

	shader_distortion_texture = Shader::Create(renderer,
		vs_d_t_src.c_str(), vs_d_t_src.length(), fs_d_t_src.c_str(), fs_d_t_src.length(), "ModelRenderer7");
	if (shader_distortion_texture == NULL) goto End;

	shader_distortion = Shader::Create(renderer,
		vs_d_src.c_str(), vs_d_src.length(), fs_d_src.c_str(), fs_d_src.length(), "ModelRenderer8");
	if (shader_distortion == NULL) goto End;

	return new ModelRenderer( 
		renderer, 
		shader_lighting_texture_normal,
		shader_lighting_normal,
		shader_lighting_texture,
		shader_lighting,
		shader_texture,
		shader,
		shader_distortion_texture,
		shader_distortion);
End:;

	ES_SAFE_DELETE(shader_lighting_texture_normal);
	ES_SAFE_DELETE(shader_lighting_normal);

	ES_SAFE_DELETE(shader_lighting_texture);
	ES_SAFE_DELETE(shader_lighting);

	ES_SAFE_DELETE(shader_texture);
	ES_SAFE_DELETE(shader);

	ES_SAFE_DELETE(shader_distortion_texture);
	ES_SAFE_DELETE(shader_distortion);
	return NULL;

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

void ModelRenderer::EndRendering( const efkModelNodeParam& parameter, void* userData )
{
	if (parameter.Distortion)
	{
		if (parameter.ColorTextureIndex >= 0)
		{
			m_renderer->SetVertexArray(m_va[6]);
		}
		else
		{
			m_renderer->SetVertexArray(m_va[7]);
		}
	}
	else if (parameter.Lighting)
	{
		if (parameter.NormalTextureIndex >= 0)
		{
			if (parameter.ColorTextureIndex >= 0)
			{
				m_renderer->SetVertexArray(m_va[0]);
			}
			else
			{
				m_renderer->SetVertexArray(m_va[1]);
			}
			}
		else
		{
			if (parameter.ColorTextureIndex >= 0)
			{
				m_renderer->SetVertexArray(m_va[2]);
			}
			else
			{
				m_renderer->SetVertexArray(m_va[3]);
			}
		}
	}
	else
	{
		if (parameter.ColorTextureIndex >= 0)
		{
			m_renderer->SetVertexArray(m_va[4]);
		}
		else
		{
			m_renderer->SetVertexArray(m_va[5]);
		}
	}

	auto model = (Model*) parameter.EffectPointer->GetModel(parameter.ModelIndex);
	if(model != nullptr)
	{
		for(auto i = 0; i < model->GetFrameCount(); i++)
		{
			model->InternalModels[i].TryDelayLoad();
		}
	
		m_shader_lighting_texture_normal->SetVertexSize(model->GetVertexSize());
		m_shader_lighting_normal->SetVertexSize(model->GetVertexSize());
		m_shader_lighting_texture->SetVertexSize(model->GetVertexSize());
		m_shader_lighting->SetVertexSize(model->GetVertexSize());
		m_shader_texture->SetVertexSize(model->GetVertexSize());
		m_shader->SetVertexSize(model->GetVertexSize());
		m_shader_distortion_texture->SetVertexSize(model->GetVertexSize());
		m_shader_distortion->SetVertexSize(model->GetVertexSize());
	}



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
		m_shader_lighting_normal,
		m_shader_lighting_texture,
		m_shader_lighting,
		m_shader_texture,
		m_shader,
		m_shader_distortion_texture,
		m_shader_distortion,
		parameter );
#else
	EndRendering_<
		RendererImplemented,
		Shader,
		Model,
		false,
		1>(
		m_renderer,
		m_shader_lighting_texture_normal,
		m_shader_lighting_normal,
		m_shader_lighting_texture,
		m_shader_lighting,
		m_shader_texture,
		m_shader,
		m_shader_distortion_texture,
		m_shader_distortion,
		parameter );
#endif

	
	/*
	return;
	
	if( m_matrixes.size() == 0 ) return;
	if( parameter.ModelIndex < 0 ) return;
	
	Model* model = (Model*)parameter.EffectPointer->GetModel( parameter.ModelIndex );
	if( model == NULL ) return;

	RenderStateBase::State& state = m_renderer->GetRenderState()->Push();
	state.DepthTest = parameter.ZTest;
	state.DepthWrite = parameter.ZWrite;
	state.AlphaBlend = parameter.AlphaBlend;
	state.CullingType = parameter.Culling;

	
	Shader* shader_ = NULL;

	if (parameter.Lighting)
	{
		if (parameter.NormalTextureIndex >= 0)
		{
			if (parameter.ColorTextureIndex >= 0)
			{
				shader_ = m_shader_lighting_texture_normal;
			}
			else
			{
				shader_ = m_shader_lighting_normal;
			}
		}
		else
		{
			if (parameter.ColorTextureIndex >= 0)
			{
				shader_ = m_shader_lighting_texture;
			}
			else
			{
				shader_ = m_shader_lighting;
			}
		}
	}
	else
	{
		if (parameter.ColorTextureIndex >= 0)
		{
			shader_ = m_shader_texture;
		}
		else
		{
			shader_ = m_shader;
		}
	}

	m_renderer->BeginShader(shader_);
	

	GLuint textures[2];
	textures[0] = 0;
	textures[1] = 0;

	if( parameter.ColorTextureIndex >= 0 )
	{
		// テクスチャ有り
		textures[0] = (GLuint) parameter.EffectPointer->GetImage(parameter.ColorTextureIndex);
	}
	
	if( parameter.NormalTextureIndex >= 0 )
	{
		textures[1] = (GLuint) parameter.EffectPointer->GetImage(parameter.NormalTextureIndex);
	}

	m_renderer->SetTextures(shader_, textures, 2);

	state.TextureFilterTypes[0] = parameter.TextureFilter;
	state.TextureWrapTypes[0] = parameter.TextureWrap;
	state.TextureFilterTypes[1] = parameter.TextureFilter;
	state.TextureWrapTypes[1] = parameter.TextureWrap;

	m_renderer->GetRenderState()->Update( false );
	
	// ここから
	ModelRendererVertexConstantBuffer<1>* vcb = (ModelRendererVertexConstantBuffer<1>*)shader_->GetVertexConstantBuffer();
	ModelRendererPixelConstantBuffer* pcb = (ModelRendererPixelConstantBuffer*)shader_->GetPixelConstantBuffer();
	
	if( parameter.Lighting )
	{
		{
			::Effekseer::Vector3D lightDirection = m_renderer->GetLightDirection();
			::Effekseer::Vector3D::Normal( lightDirection, lightDirection );
			VectorToFloat4(lightDirection, vcb->LightDirection);
			VectorToFloat4(lightDirection, pcb->LightDirection);
		}

		{
			ColorToFloat4(m_renderer->GetLightColor(), vcb->LightColor);
			ColorToFloat4(m_renderer->GetLightColor(), pcb->LightColor);
		}

		{
			ColorToFloat4(m_renderer->GetLightAmbientColor(), vcb->LightAmbientColor);
			ColorToFloat4(m_renderer->GetLightAmbientColor(), pcb->LightAmbientColor);
		}
	}
	else
	{
	}

	vcb->CameraMatrix = m_renderer->GetCameraProjectionMatrix();
	
	m_renderer->SetVertexBuffer(model->VertexBuffer, sizeof(Effekseer::Model::VertexWithIndex));
	m_renderer->SetIndexBuffer(model->IndexBuffer);

	m_renderer->SetLayout(shader_);
	
#if defined(MODEL_SOFTWARE_INSTANCING)
	for( size_t loop = 0; loop < m_matrixes.size(); )
	{
		int32_t modelCount = Effekseer::Min( m_matrixes.size() - loop, MaxInstanced );
		
		glUniformMatrix4fv( m_uniformLoc[UniformModelMatrix],
			modelCount, GL_FALSE, &m_matrixes[loop].Values[0][0] );
		
		for( int32_t num = 0; num < modelCount; num++ )
		{
			glVertexAttrib1f( m_attribLoc[AttribInstanceID], (float)num );
			
			float lc[4];
			ColorToFloat4( m_colors[loop + num], lc );
			glVertexAttrib4fv( m_attribLoc[AttribModelColor], lc );

			glDrawElements( GL_TRIANGLES, model->IndexCount, GL_UNSIGNED_INT, NULL );
		}

		loop += modelCount;
	}
#else
	for( size_t loop = 0; loop < m_matrixes.size(); )
	{
		vcb->ModelMatrix[0] = m_matrixes[loop];
		vcb->ModelUV[0][0] = m_uv[loop].X;
		vcb->ModelUV[0][1] = m_uv[loop].Y;
		vcb->ModelUV[0][2] = m_uv[loop].Width;
		vcb->ModelUV[0][3] = m_uv[loop].Height;
		
		ColorToFloat4( m_colors[loop], vcb->ModelColor[0] );
		shader_->SetConstantBuffer();
		
		//glDrawElements( GL_TRIANGLES, model->IndexCount, GL_UNSIGNED_INT, NULL );
		m_renderer->DrawPolygon( model->VertexCount, model->IndexCount );

		loop += 1;
	}
#endif

	m_renderer->EndShader(shader_);

	m_renderer->GetRenderState()->Pop();

	//*/
}

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
static const char g_sprite_vs_src [] =
R"(
IN vec4 atPosition;
IN vec4 atColor;
IN vec4 atTexCoord;
)"

R"(
OUT vec4 vaColor;
OUT vec4 vaTexCoord;
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
IN lowp vec4 vaColor;
IN mediump vec4 vaTexCoord;

uniform sampler2D uTexture0;

void main()
{
	FRAGCOLOR = vaColor * TEX2D(uTexture0, vaTexCoord.xy);
}
)";

static const char g_sprite_fs_no_texture_src[] =
R"(
IN lowp vec4 vaColor;
IN mediump vec4 vaTexCoord;

void main()
{
	FRAGCOLOR = vaColor;
}
)";


static const char g_sprite_distortion_vs_src [] =
R"(
IN vec4 atPosition;
IN vec4 atColor;
IN vec4 atTexCoord;
IN vec4 atBinormal;
IN vec4 atTangent;

)"

R"(
OUT vec4 vaColor;
OUT vec4 vaTexCoord;
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

static const char g_sprite_fs_texture_distortion_src [] =
R"(
IN lowp vec4 vaColor;
IN mediump vec4 vaTexCoord;
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
}
)";

static const char g_sprite_fs_no_texture_distortion_src [] =
R"(
IN lowp vec4 vaColor;
IN mediump vec4 vaTexCoord;
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
}
)";

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
::Effekseer::TextureLoader* CreateTextureLoader(::Effekseer::FileInterface* fileInterface)
{
#ifdef __EFFEKSEER_RENDERER_INTERNAL_LOADER__
	return new TextureLoader(fileInterface);
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


//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
Renderer* Renderer::Create(int32_t squareMaxCount, OpenGLDeviceType deviceType)
{
	GLExt::Initialize(deviceType);

	RendererImplemented* renderer = new RendererImplemented( squareMaxCount, deviceType );
	if( renderer->Initialize() )
	{
		return renderer;
	}
	return NULL;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
RendererImplemented::RendererImplemented(int32_t squareMaxCount, OpenGLDeviceType deviceType)
	: m_vertexBuffer( NULL )
	, m_indexBuffer	( NULL )
	, m_indexBufferForWireframe	( NULL )
	, m_squareMaxCount	( squareMaxCount )
	, m_renderState		( NULL )
	, m_restorationOfStates(true)
	, m_currentVertexArray( NULL )

	, m_shader(nullptr)
	, m_shader_no_texture(nullptr)
	, m_shader_distortion(nullptr)
	, m_shader_no_texture_distortion(nullptr)
	, m_standardRenderer(nullptr)

	, m_vao(nullptr)
	, m_vao_no_texture(nullptr)
	, m_vao_distortion(nullptr)
	, m_vao_no_texture_distortion(nullptr)
	, m_distortingCallback(nullptr)

	, m_deviceType(deviceType)
{
	::Effekseer::Vector3D direction( 1.0f, 1.0f, 1.0f );
	SetLightDirection( direction );
	::Effekseer::Color lightColor( 255, 255, 255, 255 );
	SetLightColor( lightColor );
	::Effekseer::Color lightAmbient( 40, 40, 40, 255 );
	SetLightAmbientColor( lightAmbient );

	m_background.UserID = 0;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
RendererImplemented::~RendererImplemented()
{
	assert( GetRef() == 0 );

	ES_SAFE_DELETE(m_distortingCallback);

	ES_SAFE_DELETE(m_standardRenderer);
	ES_SAFE_DELETE(m_shader);
	ES_SAFE_DELETE(m_shader_no_texture);
	ES_SAFE_DELETE(m_shader_distortion);
	ES_SAFE_DELETE(m_shader_no_texture_distortion);

	auto isVaoEnabled = m_vao != nullptr;

	ES_SAFE_DELETE(m_vao);
	ES_SAFE_DELETE(m_vao_no_texture);
	ES_SAFE_DELETE(m_vao_distortion);
	ES_SAFE_DELETE(m_vao_no_texture_distortion);
	ES_SAFE_DELETE(m_vao_wire_frame);

	ES_SAFE_DELETE( m_renderState );
	ES_SAFE_DELETE( m_vertexBuffer );
	ES_SAFE_DELETE( m_indexBuffer );
	ES_SAFE_DELETE(m_indexBufferForWireframe);

	if (isVaoEnabled)
	{
		assert(GetRef() == -12);
	}
	else
	{
		assert(GetRef() == -7);
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void RendererImplemented::OnLostDevice()
{
	for (auto& device : m_deviceObjects)
	{
		device->OnLostDevice();
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void RendererImplemented::OnResetDevice()
{
	for (auto& device : m_deviceObjects)
	{
		device->OnResetDevice();
	}

	GenerateIndexData();
}

//----------------------------------------------------------------------------------
// インデックスデータの生成
//----------------------------------------------------------------------------------
void RendererImplemented::GenerateIndexData()
{
	// インデックスの生成
	if( m_indexBuffer != NULL )
	{
		m_indexBuffer->Lock();

		// ( 標準設定で　DirectX 時計周りが表, OpenGLは反時計回りが表 )
		for( int i = 0; i < m_squareMaxCount; i++ )
		{
			uint16_t* buf = (uint16_t*) m_indexBuffer->GetBufferDirect(6);
			buf[0] = (uint16_t) (3 + 4 * i);
			buf[1] = (uint16_t) (1 + 4 * i);
			buf[2] = (uint16_t) (0 + 4 * i);
			buf[3] = (uint16_t) (3 + 4 * i);
			buf[4] = (uint16_t) (0 + 4 * i);
			buf[5] = (uint16_t) (2 + 4 * i);
		}

		m_indexBuffer->Unlock();
	}

	// ワイヤーフレーム用インデックスの生成
	if( m_indexBufferForWireframe != NULL )
	{
		m_indexBufferForWireframe->Lock();

		// ( 標準設定で　DirectX 時計周りが表, OpenGLは反時計回りが表 )
		for( int i = 0; i < m_squareMaxCount; i++ )
		{
			uint16_t* buf = (uint16_t*)m_indexBufferForWireframe->GetBufferDirect( 8 );
			buf[0] = (uint16_t)(0 + 4 * i);
			buf[1] = (uint16_t)(1 + 4 * i);
			buf[2] = (uint16_t)(2 + 4 * i);
			buf[3] = (uint16_t)(3 + 4 * i);
			buf[4] = (uint16_t)(0 + 4 * i);
			buf[5] = (uint16_t)(2 + 4 * i);
			buf[6] = (uint16_t)(1 + 4 * i);
			buf[7] = (uint16_t)(3 + 4 * i);
		}

		m_indexBufferForWireframe->Unlock();
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
bool RendererImplemented::Initialize()
{
	SetSquareMaxCount( m_squareMaxCount );

	m_renderState = new RenderState( this );

	m_shader = Shader::Create(this,
		g_sprite_vs_src, sizeof(g_sprite_vs_src), 
		g_sprite_fs_texture_src, sizeof(g_sprite_fs_texture_src), 
		"Standard Tex");
	if (m_shader == nullptr) return false;

	// 参照カウントの調整
	Release();

	m_shader_no_texture = Shader::Create(this,
		g_sprite_vs_src, sizeof(g_sprite_vs_src), 
		g_sprite_fs_no_texture_src, sizeof(g_sprite_fs_no_texture_src), 
		"Standard NoTex");
	if (m_shader_no_texture == nullptr)
	{
		return false;
	}

	// 参照カウントの調整
	Release();

	m_shader_distortion = Shader::Create(this,
		g_sprite_distortion_vs_src, sizeof(g_sprite_distortion_vs_src), 
		g_sprite_fs_texture_distortion_src, sizeof(g_sprite_fs_texture_distortion_src), 
		"Standard Distortion Tex");
	if (m_shader_distortion == nullptr) return false;

	// 参照カウントの調整
	Release();

	m_shader_no_texture_distortion = Shader::Create(this,
		g_sprite_distortion_vs_src, sizeof(g_sprite_distortion_vs_src), 
		g_sprite_fs_no_texture_distortion_src, sizeof(g_sprite_fs_no_texture_distortion_src), 
		"Standard Distortion NoTex");
	if (m_shader_no_texture_distortion == nullptr)
	{
		return false;
	}

	// 参照カウントの調整
	Release();

	static ShaderAttribInfo sprite_attribs[3] = {
		{ "atPosition", GL_FLOAT, 3, 0, false },
		{ "atColor", GL_UNSIGNED_BYTE, 4, 12, true },
		{ "atTexCoord", GL_FLOAT, 2, 16, false }
	};

	static ShaderAttribInfo sprite_attribs_distortion[5] = {
		{ "atPosition", GL_FLOAT, 3, 0, false },
		{ "atColor", GL_UNSIGNED_BYTE, 4, 12, true },
		{ "atTexCoord", GL_FLOAT, 2, 16, false },
		{ "atBinormal", GL_FLOAT, 3, 24, false },
		{ "atTangent", GL_FLOAT, 3, 36, false },
	};

	// 頂点属性IDを取得
	m_shader->GetAttribIdList(3, sprite_attribs);
	m_shader->SetVertexSize(sizeof(Vertex));
	m_shader->SetVertexConstantBufferSize(sizeof(Effekseer::Matrix44) * 2 + sizeof(float) * 4);
	
	m_shader->AddVertexConstantLayout(
		CONSTANT_TYPE_MATRIX44,
		m_shader->GetUniformId("uMatCamera"),
		0
		);

	m_shader->AddVertexConstantLayout(
		CONSTANT_TYPE_MATRIX44,
		m_shader->GetUniformId("uMatProjection"),
		sizeof(Effekseer::Matrix44)
		);

	m_shader->AddVertexConstantLayout(
		CONSTANT_TYPE_VECTOR4,
		m_shader->GetUniformId("mUVInversed"),
		sizeof(Effekseer::Matrix44) * 2
	);

	m_shader->SetTextureSlot(0, m_shader->GetUniformId("uTexture0"));

	m_shader_no_texture->GetAttribIdList(3, sprite_attribs);
	m_shader_no_texture->SetVertexSize(sizeof(Vertex));
	m_shader_no_texture->SetVertexConstantBufferSize(sizeof(Effekseer::Matrix44) * 2 + sizeof(float) * 4);
	
	m_shader_no_texture->AddVertexConstantLayout(
		CONSTANT_TYPE_MATRIX44,
		m_shader_no_texture->GetUniformId("uMatCamera"),
		0
		);

	m_shader_no_texture->AddVertexConstantLayout(
		CONSTANT_TYPE_MATRIX44,
		m_shader_no_texture->GetUniformId("uMatProjection"),
		sizeof(Effekseer::Matrix44)
		);

	m_shader_no_texture->AddVertexConstantLayout(
		CONSTANT_TYPE_VECTOR4,
		m_shader_no_texture->GetUniformId("mUVInversed"),
		sizeof(Effekseer::Matrix44) * 2
	);

	m_vao = VertexArray::Create(this, m_shader, GetVertexBuffer(), GetIndexBuffer());
	// 参照カウントの調整
	if (m_vao != nullptr) Release();

	m_vao_no_texture = VertexArray::Create(this, m_shader_no_texture, GetVertexBuffer(), GetIndexBuffer());
	// 参照カウントの調整
	if (m_vao_no_texture != nullptr) Release();

	// Distortion
	m_shader_distortion->GetAttribIdList(5, sprite_attribs_distortion);
	m_shader_distortion->SetVertexSize(sizeof(VertexDistortion));
	m_shader_distortion->SetVertexConstantBufferSize(sizeof(Effekseer::Matrix44) * 2 + sizeof(float) * 4);
	m_shader_distortion->SetPixelConstantBufferSize(sizeof(float) * 4 + sizeof(float) * 4);

	m_shader_distortion->AddVertexConstantLayout(
		CONSTANT_TYPE_MATRIX44,
		m_shader_distortion->GetUniformId("uMatCamera"),
		0
		);

	m_shader_distortion->AddVertexConstantLayout(
		CONSTANT_TYPE_MATRIX44,
		m_shader_distortion->GetUniformId("uMatProjection"),
		sizeof(Effekseer::Matrix44)
		);

	m_shader_distortion->AddVertexConstantLayout(
		CONSTANT_TYPE_VECTOR4,
		m_shader_distortion->GetUniformId("mUVInversed"),
		sizeof(Effekseer::Matrix44) * 2
	);

	m_shader_distortion->AddPixelConstantLayout(
		CONSTANT_TYPE_VECTOR4,
		m_shader_distortion->GetUniformId("g_scale"),
		0
		);

	m_shader_distortion->AddPixelConstantLayout(
		CONSTANT_TYPE_VECTOR4,
		m_shader_distortion->GetUniformId("mUVInversedBack"),
		sizeof(float) * 4
	);


	m_shader_distortion->SetTextureSlot(0, m_shader_distortion->GetUniformId("uTexture0"));
	m_shader_distortion->SetTextureSlot(1, m_shader_distortion->GetUniformId("uBackTexture0"));

	m_shader_no_texture_distortion->GetAttribIdList(5, sprite_attribs_distortion);
	m_shader_no_texture_distortion->SetVertexSize(sizeof(VertexDistortion));
	m_shader_no_texture_distortion->SetVertexConstantBufferSize(sizeof(Effekseer::Matrix44) * 2 + sizeof(float) * 4);
	m_shader_no_texture_distortion->SetPixelConstantBufferSize(sizeof(float) * 4 + sizeof(float) * 4);

	m_shader_no_texture_distortion->AddVertexConstantLayout(
		CONSTANT_TYPE_MATRIX44,
		m_shader_no_texture_distortion->GetUniformId("uMatCamera"),
		0
		);

	m_shader_no_texture_distortion->AddVertexConstantLayout(
		CONSTANT_TYPE_MATRIX44,
		m_shader_no_texture_distortion->GetUniformId("uMatProjection"),
		sizeof(Effekseer::Matrix44)
		);

	m_shader_no_texture_distortion->AddVertexConstantLayout(
		CONSTANT_TYPE_VECTOR4,
		m_shader_no_texture_distortion->GetUniformId("mUVInversed"),
		sizeof(Effekseer::Matrix44) * 2
	);

	m_shader_no_texture_distortion->AddPixelConstantLayout(
		CONSTANT_TYPE_VECTOR4,
		m_shader_distortion->GetUniformId("g_scale"),
		0
		);

	m_shader_no_texture_distortion->AddPixelConstantLayout(
		CONSTANT_TYPE_VECTOR4,
		m_shader_no_texture_distortion->GetUniformId("mUVInversedBack"),
		sizeof(float) * 4
	);

	m_shader_no_texture_distortion->SetTextureSlot(1, m_shader_no_texture_distortion->GetUniformId("uBackTexture0"));


	m_vao_distortion = VertexArray::Create(this, m_shader_distortion, GetVertexBuffer(), GetIndexBuffer());
	
	// 参照カウントの調整
	if (m_vao_distortion != nullptr) Release();

	m_vao_no_texture_distortion = VertexArray::Create(this, m_shader_no_texture_distortion, GetVertexBuffer(), GetIndexBuffer());
	
	// 参照カウントの調整
	if (m_vao_no_texture_distortion != nullptr) Release();

	m_vao_wire_frame = VertexArray::Create(this, m_shader_no_texture, GetVertexBuffer(), m_indexBufferForWireframe);
	if (m_vao_wire_frame != nullptr) Release();

	m_standardRenderer = new EffekseerRenderer::StandardRenderer<RendererImplemented, Shader, Vertex, VertexDistortion>(this, m_shader, m_shader_no_texture, m_shader_distortion, m_shader_no_texture_distortion);

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

	::Effekseer::Matrix44::Mul( m_cameraProj, m_camera, m_proj );

	// ステートを保存する
	if(m_restorationOfStates)
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

		if (GLExt::IsSupportedVertexArray())
		{
			glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &m_originalState.vao);
		}
	}

	glDepthFunc(GL_LEQUAL);
	glEnable(GL_BLEND);
	glDisable(GL_CULL_FACE);

	m_currentTextures.clear();
	m_renderState->GetActiveState().Reset();
	m_renderState->Update( true );
	
	// レンダラーリセット
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
	if(m_restorationOfStates)
	{
		if (m_originalState.blend) glEnable(GL_BLEND); else glDisable(GL_BLEND);
		if (m_originalState.cullFace) glEnable(GL_CULL_FACE); else glDisable(GL_CULL_FACE);
		if (m_originalState.depthTest) glEnable(GL_DEPTH_TEST); else glDisable(GL_DEPTH_TEST);
		
		if (GetDeviceType() == OpenGLDeviceType::OpenGL2)
		{
			if (m_originalState.texture) glEnable(GL_TEXTURE_2D); else glDisable(GL_TEXTURE_2D);
		}
		
		glDepthFunc(m_originalState.depthFunc);
		glDepthMask(m_originalState.depthWrite);
		glCullFace(m_originalState.cullFaceMode);
		glBlendFunc(m_originalState.blendSrc, m_originalState.blendDst);
		GLExt::glBlendEquation(m_originalState.blendEquation);

		if (GetDeviceType() == OpenGLDeviceType::OpenGL3 || GetDeviceType() == OpenGLDeviceType::OpenGLES3)
		{
			for( int32_t i = 0; i < 4; i++ )
			{
				GLExt::glBindSampler(i, 0);
			}
		}

		if (GLExt::IsSupportedVertexArray())
		{
			GLExt::glBindVertexArray(m_originalState.vao);
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
	m_squareMaxCount = count;

	if (m_vertexBuffer != nullptr) AddRef();
	if (m_indexBuffer != nullptr) AddRef();
	ES_SAFE_DELETE(m_vertexBuffer);
	ES_SAFE_DELETE(m_indexBuffer);

	// 頂点の生成
	{
		// 最大でfloat * 10 と仮定
		m_vertexBuffer = VertexBuffer::Create(this, sizeof(Vertex) * m_squareMaxCount * 4, true);
		if (m_vertexBuffer == NULL) return;
	}

	// 参照カウントの調整
	Release();


	// インデックスの生成
	{
		m_indexBuffer = IndexBuffer::Create(this, m_squareMaxCount * 6, false);
		if (m_indexBuffer == NULL) return;
	}

	// 参照カウントの調整
	Release();

	// ワイヤーフレーム用インデックスの生成
	{
		m_indexBufferForWireframe = IndexBuffer::Create( this, m_squareMaxCount * 8, false );
		if( m_indexBufferForWireframe == NULL ) return;
	}

	// 参照カウントの調整
	Release();

	// インデックスデータの生成
	GenerateIndexData();
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
const ::Effekseer::Vector3D& RendererImplemented::GetLightDirection() const
{
	return m_lightDirection;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void RendererImplemented::SetLightDirection( const ::Effekseer::Vector3D& direction )
{
	m_lightDirection = direction;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
const ::Effekseer::Color& RendererImplemented::GetLightColor() const
{
	return m_lightColor;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void RendererImplemented::SetLightColor( const ::Effekseer::Color& color )
{
	m_lightColor = color;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
const ::Effekseer::Color& RendererImplemented::GetLightAmbientColor() const
{
	return m_lightAmbient;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void RendererImplemented::SetLightAmbientColor( const ::Effekseer::Color& color )
{
	m_lightAmbient = color;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
const ::Effekseer::Matrix44& RendererImplemented::GetProjectionMatrix() const
{
	return m_proj;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void RendererImplemented::SetProjectionMatrix( const ::Effekseer::Matrix44& mat )
{
	m_proj = mat;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
const ::Effekseer::Matrix44& RendererImplemented::GetCameraMatrix() const
{
	return m_camera;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void RendererImplemented::SetCameraMatrix( const ::Effekseer::Matrix44& mat )
{
	m_cameraFrontDirection = ::Effekseer::Vector3D(mat.Values[0][2], mat.Values[1][2], mat.Values[2][2]);

	auto localPos = ::Effekseer::Vector3D(-mat.Values[3][0], -mat.Values[3][1], -mat.Values[3][2]);
	auto f = m_cameraFrontDirection;
	auto r = ::Effekseer::Vector3D(mat.Values[0][0], mat.Values[1][0], mat.Values[2][0]);
	auto u = ::Effekseer::Vector3D(mat.Values[0][1], mat.Values[1][1], mat.Values[2][1]);

	m_cameraPosition = r * localPos.X + u * localPos.Y + f * localPos.Z;

	m_camera = mat;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
::Effekseer::Matrix44& RendererImplemented::GetCameraProjectionMatrix()
{
	return m_cameraProj;
}

::Effekseer::Vector3D RendererImplemented::GetCameraFrontDirection() const
{
	return m_cameraFrontDirection;
}

::Effekseer::Vector3D RendererImplemented::GetCameraPosition() const
{
	return m_cameraPosition;
}

void RendererImplemented::SetCameraParameter(const ::Effekseer::Vector3D& front, const ::Effekseer::Vector3D& position)
{
	m_cameraFrontDirection = front;
	m_cameraPosition = position;
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
	return new ::EffekseerRenderer::RibbonRendererBase<RendererImplemented, Vertex, VertexDistortion>( this );
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
	return ModelRenderer::Create( this );
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
::Effekseer::TextureLoader* RendererImplemented::CreateTextureLoader( ::Effekseer::FileInterface* fileInterface )
{
#ifdef __EFFEKSEER_RENDERER_INTERNAL_LOADER__
	return new TextureLoader( fileInterface );
#else
	return NULL;
#endif
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
::Effekseer::ModelLoader* RendererImplemented::CreateModelLoader( ::Effekseer::FileInterface* fileInterface )
{
#ifdef __EFFEKSEER_RENDERER_INTERNAL_LOADER__
	return new ModelLoader( fileInterface );
#else
	return NULL;
#endif
}

void RendererImplemented::SetBackground(GLuint background)
{
	m_background.UserID = background;
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
void RendererImplemented::SetVertexBuffer( VertexBuffer* vertexBuffer, int32_t size )
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
void RendererImplemented::SetIndexBuffer( IndexBuffer* indexBuffer )
{
	if (m_currentVertexArray == nullptr || m_currentVertexArray->GetIndexBuffer() == nullptr)
	{
		GLExt::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer->GetInterface());
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
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void RendererImplemented::SetVertexArray( VertexArray* vertexArray )
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
void RendererImplemented::DrawSprites( int32_t spriteCount, int32_t vertexOffset )
{
	GLCheckError();

	drawcallCount++;
	drawvertexCount += spriteCount * 4;

	if( m_renderMode == ::Effekseer::RenderMode::Normal )
	{
		glDrawElements(GL_TRIANGLES, spriteCount * 6, GL_UNSIGNED_SHORT, (void*) (vertexOffset / 4 * 6 * sizeof(GLushort)));
	}
	else if( m_renderMode == ::Effekseer::RenderMode::Wireframe )
	{
		glDrawElements(GL_LINES, spriteCount * 8, GL_UNSIGNED_SHORT, (void*) (vertexOffset / 4 * 8 * sizeof(GLushort)));
	}
	
	GLCheckError();
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void RendererImplemented::DrawPolygon( int32_t vertexCount, int32_t indexCount)
{
	GLCheckError();

	drawcallCount++;
	drawvertexCount += vertexCount;

	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, NULL);

	GLCheckError();
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
Shader* RendererImplemented::GetShader(bool useTexture, bool useDistortion) const
{
	if( useDistortion )
	{
		if( useTexture && m_renderMode == Effekseer::RenderMode::Normal )
		{
			return m_shader_distortion;
		}
		else
		{
			return m_shader_no_texture_distortion;
		}
	}
	else
	{
		if( useTexture && m_renderMode == Effekseer::RenderMode::Normal )
		{
			return m_shader;
		}
		else
		{
			return m_shader_no_texture;
		}
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void RendererImplemented::BeginShader(Shader* shader)
{
	GLCheckError();

	// VAOの切り替え
	if (m_renderMode == ::Effekseer::RenderMode::Wireframe)
	{
		SetVertexArray(m_vao_wire_frame);
	}
	else if (shader == m_shader)
	{
		SetVertexArray(m_vao);
	}
	else if (shader == m_shader_no_texture)
	{
		SetVertexArray(m_vao_no_texture);
	}
	else if (shader == m_shader_distortion)
	{
		SetVertexArray(m_vao_distortion);
	}
	else if (shader == m_shader_no_texture_distortion)
	{
		SetVertexArray(m_vao_no_texture_distortion);
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
	}

	shader->EndScene();
	GLCheckError();
}

void RendererImplemented::SetVertexBufferToShader(const void* data, int32_t size)
{
	assert(currentShader != nullptr);
	memcpy(currentShader->GetVertexConstantBuffer(), data, size);
}

void RendererImplemented::SetPixelBufferToShader(const void* data, int32_t size)
{
	assert(currentShader != nullptr);
	memcpy(currentShader->GetPixelConstantBuffer(), data, size);
}

void RendererImplemented::SetTextures(Shader* shader, Effekseer::TextureData** textures, int32_t count)
{
	GLCheckError();

	m_currentTextures.clear();
	m_currentTextures.resize(count);

	for (int32_t i = 0; i < count; i++)
	{
		GLuint id = 0;
		if (textures[i] != nullptr)
		{
			id = (GLuint)textures[i]->UserID;
		}

		GLExt::glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, id);
		
		m_currentTextures[i] = id;

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
	m_renderState->Update( true );
}

int32_t RendererImplemented::GetDrawCallCount() const
{
	return drawcallCount;
}

int32_t RendererImplemented::GetDrawVertexCount() const
{
	return drawvertexCount;
}

void RendererImplemented::ResetDrawCallCount()
{
	drawcallCount = 0;
}

void RendererImplemented::ResetDrawVertexCount()
{
	drawvertexCount = 0;
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
	GLExt::glDeleteBuffers(1, &IndexBuffer);
	GLExt::glDeleteBuffers(1, &VertexBuffer);
}

bool Model::InternalModel::TryDelayLoad()
{
	if(VertexBuffer > 0) return false;
	
	GLExt::glGenBuffers(1, &VertexBuffer);
	if (VertexBuffer > 0)
	{
		GLExt::glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
		GLExt::glBufferData(GL_ARRAY_BUFFER, delayVertexBuffer.size(), delayVertexBuffer.data(), GL_STATIC_DRAW);
		GLExt::glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	
	GLExt::glGenBuffers(1, &IndexBuffer);
	if (IndexBuffer > 0)
	{
		GLExt::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBuffer);
		GLExt::glBufferData(GL_ELEMENT_ARRAY_BUFFER, delayIndexBuffer.size(), delayIndexBuffer.data(), GL_STATIC_DRAW);
		GLExt::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

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
		
		GLExt::glGenBuffers(1, &InternalModels[f].VertexBuffer);
		size_t vertexSize = vertexCount * sizeof(::Effekseer::Model::Vertex);
		if (InternalModels[f].VertexBuffer > 0)
		{
			GLExt::glBindBuffer(GL_ARRAY_BUFFER, InternalModels[f].VertexBuffer);
			GLExt::glBufferData(GL_ARRAY_BUFFER, vertexSize, vertexData, GL_STATIC_DRAW);
			GLExt::glBindBuffer(GL_ARRAY_BUFFER, 0);
		}
		InternalModels[f].delayVertexBuffer.resize(vertexSize);
		memcpy(InternalModels[f].delayVertexBuffer.data(), vertexData, vertexSize);
		
		GLExt::glGenBuffers(1, &InternalModels[f].IndexBuffer);
		size_t indexSize = faceCount * sizeof(::Effekseer::Model::Face);

		if (InternalModels[f].IndexBuffer > 0)
		{
			GLExt::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, InternalModels[f].IndexBuffer);
			GLExt::glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexSize, faceData, GL_STATIC_DRAW);
			GLExt::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		}
		InternalModels[f].delayIndexBuffer.resize(indexSize);
		memcpy(InternalModels[f].delayIndexBuffer.data(), faceData, indexSize);
	}
}

Model::~Model()
{
	ES_SAFE_DELETE_ARRAY(InternalModels);

}

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
namespace EffekseerRendererGL
{
//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
RenderState::RenderState( RendererImplemented* renderer )
	: m_renderer	( renderer )
{
	if (m_renderer->GetDeviceType() == OpenGLDeviceType::OpenGL3 || m_renderer->GetDeviceType() == OpenGLDeviceType::OpenGLES3)
	{
		GLExt::glGenSamplers(4, m_samplers);
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
		GLExt::glDeleteSamplers(4, m_samplers);
	}
}

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
void RenderState::Update( bool forced )
{
	GLCheckError();

	if( m_active.DepthTest != m_next.DepthTest || forced )
	{
		if( m_next.DepthTest )
		{
			glEnable( GL_DEPTH_TEST );
		}
		else
		{
			glDisable( GL_DEPTH_TEST );
		}
	}

	GLCheckError();

	if( m_active.DepthWrite != m_next.DepthWrite || forced )
	{
		glDepthMask( m_next.DepthWrite );
	}

	GLCheckError();

	if( m_active.CullingType != m_next.CullingType || forced )
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

	if( m_active.AlphaBlend != m_next.AlphaBlend || forced )
	{
		if(  m_next.AlphaBlend == ::Effekseer::AlphaBlendType::Opacity ||
			m_renderer->GetRenderMode() == ::Effekseer::RenderMode::Wireframe )
		{
			glDisable( GL_BLEND );
		}
		else
		{
			glEnable( GL_BLEND );

			if( m_next.AlphaBlend == ::Effekseer::AlphaBlendType::Sub )
			{
				GLExt::glBlendEquationSeparate(GL_FUNC_REVERSE_SUBTRACT, GL_FUNC_ADD);
				GLExt::glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE, GL_ZERO, GL_ONE);
			}
			else
			{
				GLExt::glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
				if( m_next.AlphaBlend == ::Effekseer::AlphaBlendType::Blend )
				{
					GLExt::glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE);
				}
				else if( m_next.AlphaBlend == ::Effekseer::AlphaBlendType::Add )
				{
					GLExt::glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE, GL_ONE, GL_ONE);
				}
				else if( m_next.AlphaBlend == ::Effekseer::AlphaBlendType::Mul )
				{
					GLExt::glBlendFuncSeparate(GL_ZERO, GL_SRC_COLOR, GL_ZERO, GL_ONE);
				}
			}
		}
	}

	GLCheckError();
	
	static const GLint glfilterMin[] = { GL_NEAREST, GL_LINEAR_MIPMAP_LINEAR };
	static const GLint glfilterMag[] = { GL_NEAREST, GL_LINEAR };
	static const GLint glwrap[] = { GL_REPEAT, GL_CLAMP_TO_EDGE };

	if (m_renderer->GetDeviceType() == OpenGLDeviceType::OpenGL3 || m_renderer->GetDeviceType() == OpenGLDeviceType::OpenGLES3)
	{
		for (int32_t i = 0; i < 4; i++)
		{
			if (m_active.TextureFilterTypes[i] != m_next.TextureFilterTypes[i] || forced)
			{
				GLExt::glActiveTexture(GL_TEXTURE0 + i);

				int32_t filter_ = (int32_t) m_next.TextureFilterTypes[i];

				GLExt::glSamplerParameteri(m_samplers[i], GL_TEXTURE_MAG_FILTER, glfilterMag[filter_]);
				GLExt::glSamplerParameteri(m_samplers[i], GL_TEXTURE_MIN_FILTER, glfilterMin[filter_]);
				//glSamplerParameteri( m_samplers[i],  GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				//glSamplerParameteri( m_samplers[i],  GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

				GLExt::glBindSampler(i, m_samplers[i]);
			}

			if (m_active.TextureWrapTypes[i] != m_next.TextureWrapTypes[i] || forced)
			{
				GLExt::glActiveTexture(GL_TEXTURE0 + i);

				int32_t wrap_ = (int32_t) m_next.TextureWrapTypes[i];
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
			if (m_renderer->GetCurrentTextures()[i] == 0) continue;

			// always changes because a flag is assigned into a texture
			// if (m_active.TextureFilterTypes[i] != m_next.TextureFilterTypes[i] || forced)
			{
				GLExt::glActiveTexture(GL_TEXTURE0 + i);
				GLCheckError();

				int32_t filter_ = (int32_t) m_next.TextureFilterTypes[i];

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, glfilterMag[filter_]);
				GLCheckError();

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, glfilterMin[filter_]);
				GLCheckError();
			}

			// always changes because a flag is assigned into a texture
			// if (m_active.TextureWrapTypes[i] != m_next.TextureWrapTypes[i] || forced)
			{
				GLExt::glActiveTexture(GL_TEXTURE0 + i);
				GLCheckError();

				int32_t wrap_ = (int32_t) m_next.TextureWrapTypes[i];

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, glwrap[wrap_]);
				GLCheckError();

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, glwrap[wrap_]);
				GLCheckError();
			}
		}
		GLCheckError();
	}

	GLExt::glActiveTexture( GL_TEXTURE0 );
	
	m_active = m_next;

	GLCheckError();
}

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
}
//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------

#if _WIN32
#define LOG(s)	OutputDebugStringA(s)
#else
#define LOG(s)	printf("%s", s)
#endif

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
namespace EffekseerRendererGL
{

static const char g_header_vs_gl3_src [] =
"#version 330\n" \
"#define lowp\n" \
"#define mediump\n" \
"#define highp\n" \
"#define IN in\n" \
"#define OUT out\n";

static const char g_header_fs_gl3_src [] =
"#version 330\n" \
"#define lowp\n" \
"#define mediump\n" \
"#define highp\n" \
"#define IN in\n" \
"#define TEX2D texture\n" \
"layout (location = 0) out vec4 FRAGCOLOR;\n";

static const char g_header_vs_gles3_src [] =
"#version 300 es\n" \
"precision mediump float;\n" \
"#define IN in\n" \
"#define OUT out\n";

static const char g_header_fs_gles3_src [] =
"#version 300 es\n" \
"precision mediump float;\n" \
"#define IN in\n" \
"#define TEX2D texture\n" \
"layout (location = 0) out vec4 FRAGCOLOR;\n";

static const char g_header_vs_gles2_src [] =
"precision mediump float;\n" \
"#define IN attribute\n" \
"#define OUT varying\n";

static const char g_header_fs_gles2_src [] =
"precision mediump float;\n" \
"#define IN varying\n" \
"#define TEX2D texture2D\n" \
"#define FRAGCOLOR gl_FragColor\n";

static const char g_header_vs_gl2_src [] =
"#version 110\n" \
"#define lowp\n" \
"#define mediump\n" \
"#define highp\n" \
"#define IN attribute\n" \
"#define OUT varying\n";

static const char g_header_fs_gl2_src [] =
"#version 110\n" \
"#define lowp\n" \
"#define mediump\n" \
"#define highp\n" \
"#define IN varying\n" \
"#define TEX2D texture2D\n" \
"#define FRAGCOLOR gl_FragColor\n";

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
bool Shader::CompileShader(
	RendererImplemented* renderer,
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
	

	// バーテックスシェーダをコンパイル
	if (renderer->GetDeviceType() == OpenGLDeviceType::OpenGL3) src_data[0] = g_header_vs_gl3_src;
	if (renderer->GetDeviceType() == OpenGLDeviceType::OpenGL2) src_data[0] = g_header_vs_gl2_src;
	if (renderer->GetDeviceType() == OpenGLDeviceType::OpenGLES3) src_data[0] = g_header_vs_gles3_src;
	if (renderer->GetDeviceType() == OpenGLDeviceType::OpenGLES2 || renderer->GetDeviceType() == OpenGLDeviceType::Emscripten) src_data[0] = g_header_vs_gles2_src;

	src_size[0] = (GLint) strlen(src_data[0]);
	src_data[1] = vs_src;
	src_size[1] = (GLint)strlen(vs_src);
	
	vert_shader = GLExt::glCreateShader(GL_VERTEX_SHADER);
	GLExt::glShaderSource(vert_shader, 2, src_data, src_size);
	GLExt::glCompileShader(vert_shader);
	GLExt::glGetShaderiv(vert_shader, GL_COMPILE_STATUS, &res_vs);

	// フラグメントシェーダをコンパイル
	if (renderer->GetDeviceType() == OpenGLDeviceType::OpenGL3) src_data[0] = g_header_fs_gl3_src;
	if (renderer->GetDeviceType() == OpenGLDeviceType::OpenGL2) src_data[0] = g_header_fs_gl2_src;
	if (renderer->GetDeviceType() == OpenGLDeviceType::OpenGLES3) src_data[0] = g_header_fs_gles3_src;
	if (renderer->GetDeviceType() == OpenGLDeviceType::OpenGLES2 || renderer->GetDeviceType() == OpenGLDeviceType::Emscripten) src_data[0] = g_header_fs_gles2_src;

	src_size[0] = (GLint) strlen(src_data[0]);
	src_data[1] = fs_src;
	src_size[1] = (GLint)strlen(fs_src);

	frag_shader = GLExt::glCreateShader(GL_FRAGMENT_SHADER);
	GLExt::glShaderSource(frag_shader, 2, src_data, src_size);
	GLExt::glCompileShader(frag_shader);
	GLExt::glGetShaderiv(frag_shader, GL_COMPILE_STATUS, &res_fs);
	
	// シェーダプログラムの作成
	program = GLExt::glCreateProgram();
	GLExt::glAttachShader(program, vert_shader);
	GLExt::glAttachShader(program, frag_shader);
	
	// シェーダプログラムのリンク
	GLExt::glLinkProgram(program);
	GLExt::glGetProgramiv(program, GL_LINK_STATUS, &res_link);

#ifndef NDEBUG
	{
		// エラー出力
		char log[512];
		int32_t log_size;
		GLExt::glGetShaderInfoLog(vert_shader, sizeof(log), &log_size, log);
		if (log_size > 0) {
			LOG(name);
			LOG(": Vertex Shader error.\n");
			LOG(log);
		}
		GLExt::glGetShaderInfoLog(frag_shader, sizeof(log), &log_size, log);
		if (log_size > 0) {
			LOG(name);
			LOG(": Fragment Shader error.\n");
			LOG(log);
		}
		GLExt::glGetProgramInfoLog(program, sizeof(log), &log_size, log);
		if (log_size > 0) {
			LOG(name);
			LOG(": Shader Link error.\n");
			LOG(log);
		}
	}
#endif
	// シェーダオブジェクトの削除
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
	RendererImplemented* renderer,
	GLuint program,
	const char* vs_src,
	size_t vertexShaderSize,
	const char* fs_src,
	size_t pixelShaderSize,
	const char* name)
	: DeviceObject		( renderer )
	, m_program			( program )
	, m_vertexSize		( 0 )
	, m_vertexConstantBuffer	( NULL )
	, m_pixelConstantBuffer		( NULL )
{
	for (int32_t i = 0; i < 4; i++)
	{
		m_textureSlots[i] = 0;
		m_textureSlotEnables[i] = false;
	}

	m_vsSrc.resize(vertexShaderSize);
	memcpy(m_vsSrc.data(), vs_src, vertexShaderSize );
	m_vsSrc.push_back(0);

	m_psSrc.resize(pixelShaderSize);
	memcpy(m_psSrc.data(), fs_src, pixelShaderSize );
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
	ES_SAFE_DELETE_ARRAY( m_vertexConstantBuffer );
	ES_SAFE_DELETE_ARRAY( m_pixelConstantBuffer );
}

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
Shader* Shader::Create(
	RendererImplemented* renderer,
		const char* vs_src,
		size_t vertexShaderSize,
		const char* fs_src,
		size_t pixelShaderSize,
		const char* name)
{
	GLuint program;
	
	assert( renderer != NULL );

	if(CompileShader(
		renderer,
		program,
		vs_src,
		vertexShaderSize,
		fs_src,
		pixelShaderSize,
		name))
	{
		return new Shader( 
			renderer, 
			program,
			vs_src,
			vertexShaderSize,
			fs_src,
			pixelShaderSize, 
			name);
	}
	else
	{
		return NULL;
	}
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
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
	if (IsValid()) return;

	GLuint program;
	
	if(CompileShader(
		GetRenderer(),
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
	
	if(CompileShader(
		GetRenderer(),
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

void Shader::GetAttribIdList(int count, const ShaderAttribInfo* info )
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
	for( size_t i = 0; i < m_aid.size(); i++ )
	{
		if ( m_aid[i] >= 0 ) 
		{
			GLExt::glEnableVertexAttribArray( m_aid[i] );
		}
	}
	GLCheckError();
}

void Shader::DisableAttribs()
{
	GLCheckError();

	for( size_t i = 0; i < m_aid.size(); i++ )
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
	const void* vertices = NULL;

	for( size_t i = 0; i < m_aid.size(); i++ )
	{
		if ( m_aid[i] >= 0) 
		{
			GLExt::glVertexAttribPointer(m_aid[i], m_layout[i].count, m_layout[i].type, m_layout[i].normalized, m_vertexSize, (uint8_t*) vertices + m_layout[i].offset);
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
			uint8_t* data = (uint8_t*) m_vertexConstantBuffer;
			data += m_vertexConstantLayout[i].Offset;
			GLExt::glUniformMatrix4fv(
				m_vertexConstantLayout[i].ID,
				1,
				GL_FALSE,
				(const GLfloat*)data);
		}
		else if( 
			CONSTANT_TYPE_MATRIX44_ARRAY_END > m_vertexConstantLayout[i].Type &&
			m_vertexConstantLayout[i].Type >= CONSTANT_TYPE_MATRIX44_ARRAY)
		{
			uint8_t* data = (uint8_t*) m_vertexConstantBuffer;
			data += m_vertexConstantLayout[i].Offset;
			GLExt::glUniformMatrix4fv(
				m_vertexConstantLayout[i].ID,
				m_vertexConstantLayout[i].Type - CONSTANT_TYPE_MATRIX44_ARRAY,
				GL_FALSE,
				(const GLfloat*)data);
		}

		else if (m_vertexConstantLayout[i].Type == CONSTANT_TYPE_VECTOR4)
		{
			uint8_t* data = (uint8_t*) m_vertexConstantBuffer;
			data += m_vertexConstantLayout[i].Offset;
			GLExt::glUniform4fv(
				m_vertexConstantLayout[i].ID,
				1,
				(const GLfloat*)data);
		}
		else if( 
			CONSTANT_TYPE_VECTOR4_ARRAY_END > m_vertexConstantLayout[i].Type &&
			m_vertexConstantLayout[i].Type >= CONSTANT_TYPE_VECTOR4_ARRAY)
		{
			uint8_t* data = (uint8_t*) m_vertexConstantBuffer;
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
			uint8_t* data = (uint8_t*) m_pixelConstantBuffer;
			data += m_pixelConstantLayout[i].Offset;
			GLExt::glUniformMatrix4fv(
				m_pixelConstantLayout[i].ID,
				1,
				GL_FALSE,
				(const GLfloat*) data);
		}
		else if( 
			CONSTANT_TYPE_MATRIX44_ARRAY_END > m_pixelConstantLayout[i].Type &&
			m_pixelConstantLayout[i].Type >= CONSTANT_TYPE_MATRIX44_ARRAY)
		{
			uint8_t* data = (uint8_t*) m_pixelConstantBuffer;
			data += m_pixelConstantLayout[i].Offset;
			GLExt::glUniformMatrix4fv(
				m_pixelConstantLayout[i].ID,
				m_pixelConstantLayout[i].Type - CONSTANT_TYPE_MATRIX44_ARRAY,
				GL_FALSE,
				(const GLfloat*)data);
		}

		else if (m_pixelConstantLayout[i].Type == CONSTANT_TYPE_VECTOR4)
		{
			uint8_t* data = (uint8_t*) m_pixelConstantBuffer;
			data += m_pixelConstantLayout[i].Offset;
			GLExt::glUniform4fv(
				m_pixelConstantLayout[i].ID,
				1,
				(const GLfloat*)data);
		}
		else if( 
			CONSTANT_TYPE_VECTOR4_ARRAY_END > m_pixelConstantLayout[i].Type &&
			m_pixelConstantLayout[i].Type >= CONSTANT_TYPE_VECTOR4_ARRAY)
		{
			uint8_t* data = (uint8_t*) m_pixelConstantBuffer;
			data += m_pixelConstantLayout[i].Offset;
			GLExt::glUniform4fv(
				m_pixelConstantLayout[i].ID,
				m_pixelConstantLayout[i].Type - CONSTANT_TYPE_VECTOR4_ARRAY,
				(const GLfloat*)data);
		}
	}
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
}

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
TextureLoader::TextureLoader( ::Effekseer::FileInterface* fileInterface )
	: m_fileInterface	( fileInterface )
{
	if( m_fileInterface == NULL )
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
		reader( m_fileInterface->OpenRead( path ) );
	
	if( reader.get() != NULL )
	{
		size_t size_texture = reader->GetLength();
		char* data_texture = new char[size_texture];
		reader->Read(data_texture, size_texture);

		if (size_texture < 4)
		{
		}
		else if (data_texture[1] == 'P' &&
			data_texture[2] == 'N' &&
			data_texture[3] == 'G')
		{
			pngTextureLoader.Load(data_texture, size_texture, false);
			delete [] data_texture;

			GLuint texture = 0;
			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexImage2D(
				GL_TEXTURE_2D,
				0,
				GL_RGBA,
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
		else if (data_texture[0] == 'D' &&
			data_texture[1] == 'D' &&
			data_texture[2] == 'S' &&
			data_texture[3] == ' ')
		{
			if (ddsTextureLoader.Load(data_texture, size_texture))
			{
				delete[] data_texture;

				if (ddsTextureLoader.GetTextureFormat() == Effekseer::TextureFormatType::ABGR8)
				{
					GLuint texture = 0;

					glGenTextures(1, &texture);
					glBindTexture(GL_TEXTURE_2D, texture);

					glTexImage2D(
						GL_TEXTURE_2D,
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

					delete[] data_texture;

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

					GLExt::glCompressedTexImage2D(
						GL_TEXTURE_2D,
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
			else
			{
				delete[] data_texture;
			}
		}
	}

	return nullptr;
}

//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
void TextureLoader::Unload(Effekseer::TextureData* data )
{
	if( data != NULL )
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
}
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
VertexArray::VertexArray( RendererImplemented* renderer, Shader* shader, 
	VertexBuffer* vertexBuffer, IndexBuffer* indexBuffer )
	: DeviceObject		( renderer )
	, m_shader			( shader )
	, m_vertexBuffer	( vertexBuffer )
	, m_indexBuffer		( indexBuffer )
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
VertexArray* VertexArray::Create( RendererImplemented* renderer, Shader* shader, 
	VertexBuffer* vertexBuffer, IndexBuffer* indexBuffer )
{
	if (GLExt::IsSupportedVertexArray())
	{
		return new VertexArray( renderer, shader, vertexBuffer, indexBuffer );
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

	m_shader->EnableAttribs();
	m_shader->SetVertex();

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
}
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
VertexBuffer::VertexBuffer( RendererImplemented* renderer, int size, bool isDynamic )
	: DeviceObject		( renderer )
	, VertexBufferBase	( size, isDynamic )
	, m_vertexRingStart		( 0 )
	, m_vertexRingOffset	( 0 )
	, m_ringBufferLock		( false )
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
	delete [] m_resource;
}

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
VertexBuffer* VertexBuffer::Create( RendererImplemented* renderer, int size, bool isDynamic )
{
	return new VertexBuffer( renderer, size, isDynamic );
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
	if (IsValid()) return;

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
	assert( !m_isLock );

	m_isLock = true;
	m_offset = 0;
	m_vertexRingStart = 0;
}

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
bool VertexBuffer::RingBufferLock( int32_t size, int32_t& offset, void*& data )
{
	assert( !m_isLock );
	assert( !m_ringBufferLock );
	assert( this->m_isDynamic );

	if( size > m_size ) return false;

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

bool VertexBuffer::TryRingBufferLock(int32_t size, int32_t& offset, void*& data)
{
	if ((int32_t) m_vertexRingOffset + size > m_size) return false;

	return RingBufferLock(size, offset, data);
}

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
void VertexBuffer::Unlock()
{
	assert( m_isLock || m_ringBufferLock );

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
        if(GLExt::GetDeviceType() == OpenGLDeviceType::OpenGLES3)
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
			memcpy(target + m_vertexRingStart, m_resource, m_offset);
			GLExt::glUnmapBuffer(GL_ARRAY_BUFFER);
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
}
//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------

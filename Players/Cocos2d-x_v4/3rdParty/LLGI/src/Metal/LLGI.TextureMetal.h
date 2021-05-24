
#pragma once

#include "../LLGI.Texture.h"
#include "LLGI.GraphicsMetal.h"

namespace LLGI
{

class TextureMetal : public Texture
{
private:
	ReferenceObject* owner_ = nullptr;
	std::vector<uint8_t> data;

	id<MTLTexture> texture_ = nullptr;
	Vec2I size_;
	bool fromExternal_ = false;

	bool
	Initialize(id<MTLDevice> device, const Vec2I& size, TextureFormatType format, int samplingCount, TextureType type, int MipMapCount);
	void Write(const uint8_t* data);

public:
	TextureMetal();
	~TextureMetal() override;

	bool Initialize(GraphicsMetal* owner, const TextureInitializationParameter& parameter);
	bool Initialize(GraphicsMetal* owner, const RenderTextureInitializationParameter& parameter);
	bool Initialize(GraphicsMetal* owner, const DepthTextureInitializationParameter& parameter);
	bool Initialize(GraphicsMetal* owner, id<MTLTexture> externalTexture);
	void Reset(id<MTLTexture> nativeTexture);
	void* Lock() override;
	void Unlock() override;
	Vec2I GetSizeAs2D() const override;

	id<MTLTexture>& GetTexture() { return texture_; }
};

} // namespace LLGI

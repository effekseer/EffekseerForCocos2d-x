
#pragma once

#include "../LLGI.Texture.h"
#include "LLGI.GraphicsMetal.h"

namespace LLGI
{

struct Texture_Impl;

class TextureMetal : public Texture
{
private:
    ReferenceObject* owner_ = nullptr;
	Texture_Impl* impl = nullptr;
	std::vector<uint8_t> data;

public:
	TextureMetal();
	virtual ~TextureMetal();

	bool Initialize(id<MTLDevice> device, ReferenceObject* owner, Vec2I size, TextureType type);
	bool Initialize(GraphicsMetal* owner, const RenderTextureInitializationParameter& parameter);
	void Reset(id<MTLTexture> nativeTexture);
	void* Lock() override;
	void Unlock() override;
	Vec2I GetSizeAs2D() const override;

	Texture_Impl* GetImpl() const;
};

} // namespace LLGI

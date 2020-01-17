
#pragma once

#include "LLGI.Base.h"

namespace LLGI
{

class Texture : public ReferenceObject
{
private:
protected:
	TextureType type_ = TextureType::Unknown;
	TextureFormatType format_ = TextureFormatType::Uknown;

public:
	Texture() = default;
	virtual ~Texture() = default;

	/**
		@brief	get an internal buffer of image
		@note
		CommandList::UpdateTexture must be called in some platforms.
	*/
	virtual void* GetBuffer() const;

	[[deprecated("use CommandList::SetImageData2D.")]] virtual void* Lock();

	[[deprecated("use CommandList::SetImageData2D.")]] virtual void Unlock();

	virtual Vec2I GetSizeAs2D() const;
	[[deprecated("use GetType.")]] virtual bool IsRenderTexture() const;
	[[deprecated("use GetType.")]] virtual bool IsDepthTexture() const;

	TextureType GetType() const { return type_; }

	virtual TextureFormatType GetFormat() const;
};

} // namespace LLGI
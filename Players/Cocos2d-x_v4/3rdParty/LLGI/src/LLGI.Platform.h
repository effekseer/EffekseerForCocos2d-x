
#pragma once

#include "LLGI.Base.h"

#ifdef _WIN32
#undef CreateWindow
#endif

namespace LLGI
{

Window* CreateWindow(const char* title, Vec2I windowSize);

Platform* CreatePlatform(DeviceType platformDeviceType, Window* window);

class Platform : public ReferenceObject
{
private:
public:
	Platform() = default;
	virtual ~Platform() = default;

	virtual bool NewFrame();
	virtual void Present();
	virtual Graphics* CreateGraphics();
	virtual DeviceType GetDeviceType() const { return DeviceType::Default; }

	virtual void SetWindowSize(const Vec2I& windowSize);

	/**
		@brief get render pass of screen to show on a display.
		@note
		Don't release and addref it.
		Don't use it for the many purposes, please input Clear or SetRenderPass immediately.
	*/
	virtual RenderPass* GetCurrentScreen(const Color8& clearColor = Color8(), bool isColorCleared = false, bool isDepthCleared = false);
};

} // namespace LLGI
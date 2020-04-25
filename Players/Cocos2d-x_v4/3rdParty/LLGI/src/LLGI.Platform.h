
#pragma once

#include "LLGI.Base.h"

#ifdef _WIN32
#undef CreateWindow
#endif

namespace LLGI
{

struct PlatformParameter
{
	DeviceType Device;
	bool WaitVSync;
};

Window* CreateWindow(const char* title, Vec2I windowSize);

Platform* CreatePlatform(const PlatformParameter& parameter, Window* window);

class Platform : public ReferenceObject
{
private:
protected:
	bool waitVSync_ = false;

public:
	Platform() = default;
	virtual ~Platform() = default;

	virtual bool NewFrame();
	virtual void Present();
	virtual Graphics* CreateGraphics();
	virtual DeviceType GetDeviceType() const { return DeviceType::Default; }

	virtual void SetWindowSize(const Vec2I& windowSize);

	bool GetWaitVSync() const { return waitVSync_; }

	/**
		@brief get render pass of screen to show on a display.
		@note
		Don't release and addref it.
		Don't use it for the many purposes, please input Clear or SetRenderPass immediately.
	*/
	virtual RenderPass* GetCurrentScreen(const Color8& clearColor = Color8(), bool isColorCleared = false, bool isDepthCleared = false);
};

} // namespace LLGI
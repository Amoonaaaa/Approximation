#pragma once

class IWindowCore
{
public:
	virtual void Initialize() = 0;
	virtual bool WindowSouldClose() = 0;
	virtual void SwapBufferCore() = 0;
	virtual void MainLoop() = 0;
	virtual ~IWindowCore() = default;
};
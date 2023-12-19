#pragma once
#include "../Module.h"

class Watermark : public IModule {
public:
	Watermark();
	~Watermark();

	Vec2 position = {999.f, 999.f};

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onPreRender(MinecraftUIRenderContext* renderCtx) override;
	virtual bool hasPositionSetting() { return true; }
};

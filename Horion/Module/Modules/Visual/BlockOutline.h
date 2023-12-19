#pragma once
#pragma once
#include "../../ModuleManager.h"
#include "../Module.h"
class BlockOutline : public IModule {
public:
	bool selectRainbow = true;
	float thickness = 1.f;
	rgb Color;
	bool faceH = false;
	bool doOutline = true;
	float opacityVal = 1.f;
	float fOpacity = 0.5f;
	BlockOutline();
	~BlockOutline();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onPostRender(MinecraftUIRenderContext* renderCtx) override;
};
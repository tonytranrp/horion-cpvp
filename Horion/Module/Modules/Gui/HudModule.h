#pragma once
#include "../Module.h"

class HudModule : public IModule {
public:
	HudModule();
	~HudModule();
	
	SettingEnum font{this};
	bool serverInfo = false;
	bool coordinates = false;
	bool currentItemDurability = true;
	bool MinecraftTime = false;
	bool fps = true;
	bool cps = true;
	bool alwaysShow = false;

	float scale = 1.f;
	float timeOfDay;

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onPostRender(MinecraftUIRenderContext* renderCtx) override;
};

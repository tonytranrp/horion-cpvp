#pragma once

#include "../../ModuleManager.h"
#include "../Module.h"

class ESP : public IModule {
public:
	bool isMobEsp = false;
	bool doRainbow = true;
	rgb color = {255, 255, 255};
	SettingEnum type{this};
	ESP();
	~ESP();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual std::string getModeText() override;
	virtual void onPreRender(MinecraftUIRenderContext* renderCtx) override;
};

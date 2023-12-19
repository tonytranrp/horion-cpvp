#pragma once
#include "../Module.h"

class TabGuiMod : public IModule {
public:
	TabGuiMod();
	~TabGuiMod();

	SettingEnum font{this};
	SettingEnum style{this};
	float scale = 1.f;
	float opacity = 0.8f;
	bool isSlider = 0;

	// Inherited via IModule
	virtual const char* getModuleName() override;
};

#pragma once
#include "../Module.h"

class Arraylist : public IModule {
public:
	Arraylist();
	~Arraylist();
	SettingEnum mode{this};
	SettingEnum colorMode{this};
	SettingEnum rainbowMode{this};
	SettingEnum font{this};
	static rgb color;
	float scale = 1.f;
	float opacity = 0.65f;
	bool bottom = false;
	bool keybinds = true;
	bool modes = false;
	bool clickToggle = false;
	// Inherited via IModule
	virtual const char* getModuleName() override;
};

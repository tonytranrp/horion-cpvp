#pragma once
#include "../Module.h"
class AutoArmor : public IModule {
public:
	AutoArmor();
	~AutoArmor();
	SettingEnum Mode;
	bool setArmor = false;

	virtual void ScreenController_tick(ScreenController* c);
	virtual void onTick(GameMode* gm) override;
	virtual const char* getModuleName() override;
};

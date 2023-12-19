#pragma once
#include "../Module.h"

class Phase : public IModule {
public:
	SettingEnum mode;
	float distance = 1.1f;
	bool oldmode = false;

	Phase();
	~Phase();

	virtual const char* getModuleName() override;
	virtual void onTick(GameMode* gm) override;
	virtual void onDisable() override;
};

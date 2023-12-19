#pragma once
#include "../Module.h"

class Offhand : public IModule {
public:
	Offhand();
	~Offhand();
	SettingEnum Item{this};
	SettingEnum Mode{this};
	bool setOffhand = false;

	virtual void ScreenController_tick(ScreenController* c);
	virtual const char* getModuleName() override;
	virtual std::string getModeText() override;
	virtual void onTick(GameMode* gm) override;
};

#pragma once
#include "../Module.h"
class Disabler : public IModule {
private:
public:
	Disabler();
	~Disabler();

	SettingEnum Mode{ this };

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual std::string getModeText() override;
	virtual void onTick(GameMode* gm) override;
};
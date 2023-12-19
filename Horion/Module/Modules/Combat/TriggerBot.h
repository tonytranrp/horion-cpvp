#pragma once
#include "../../ModuleManager.h"
#include "../Module.h"

class TriggerBot : public IModule {
private:
	int delay = 0;
	int Odelay = 0;
	bool sword = true;
	bool includeMobs = false;

public:
	TriggerBot();
	~TriggerBot();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(GameMode* gm) override;
};

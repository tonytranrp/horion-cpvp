#pragma once
#include "../../ModuleManager.h"
#include "../Module.h"

class HealthCheck : public IModule {

private:




public:
	HealthCheck();
	~HealthCheck();
	
	int HealthSlider = 10;

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual std::string getModeText() override;
	virtual void onTick(GameMode* gm) override;
};
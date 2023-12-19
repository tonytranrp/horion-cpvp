#pragma once
#include "../Module.h"
class AutoDisconnect : public IModule {
public:
	AutoDisconnect();
	~AutoDisconnect();
	int HealthSlider = 10;
	// Inherited via IModule
	virtual const char* getModuleName() override;
};
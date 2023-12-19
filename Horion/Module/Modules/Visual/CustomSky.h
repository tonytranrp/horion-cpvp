#pragma once
#include "../Module.h"
class CustomSky : public IModule {
private:
	

public:
	float intensity = 0.01f;

	CustomSky();
	~CustomSky();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	static rgb color;
	bool rainbowSky = false;
};

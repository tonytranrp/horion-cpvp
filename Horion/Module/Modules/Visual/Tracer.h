#pragma once
#include "../Module.h"
class Tracer : public IModule {
public:
	Tracer();
	~Tracer();
	bool includeMobs = false;
	rgb color = {255, 255, 255};
	// Inherited via IModule
	virtual const char* getModuleName() override;
	void onLevelRender() override;
};

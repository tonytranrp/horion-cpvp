#pragma once
#include "../Module.h"
class FastThrow : public IModule {
public:
	FastThrow();
	~FastThrow();

	float Odelay = 0.f;
	float delay = 0.f;

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(GameMode* gm) override;
};

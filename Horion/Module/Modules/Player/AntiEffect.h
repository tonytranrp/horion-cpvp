#pragma once
#include "../Module.h"
class AntiEffect : public IModule {
public:
	AntiEffect();
	~AntiEffect();
	bool JumpBoost = false;
	bool Nausea = false;
	bool Blindness = false;
	bool NightVision = false;
	bool Levitation = false;
	bool SlowFalling = false;
	bool Darkness = false;
	// Inherited via IModule
	virtual const char* getModuleName() override;
};
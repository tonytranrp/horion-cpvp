#pragma once
#include "../Module.h"
class AutoFish : public IModule {
public:
	AutoFish();
	~AutoFish();

	bool shouldFish = false;
	bool reelRod = false;
	bool autoRod = false;

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(GameMode* gm) override;
	virtual void onDisable() override;
	virtual void onEnable() override;
};
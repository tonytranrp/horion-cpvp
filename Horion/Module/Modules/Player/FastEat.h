#pragma once
#include "../Module.h"
class FastEat : public IModule {
public:
	FastEat();
	~FastEat();

	int DurationSpeed = 5;

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(GameMode* gm) override;
	virtual void onDisable() override;
};

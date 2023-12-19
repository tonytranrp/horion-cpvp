#pragma once
#include "../../ModuleManager.h"
#include "../Module.h"
class BowSpam : public IModule {
public:
	BowSpam();
	~BowSpam();
	int Pdelay = 8;
	bool PointingAtPlayer = false;
	
	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(GameMode* gm) override;
};
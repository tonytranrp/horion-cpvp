#pragma once
#include "../Module.h"
class DeathPosition : public IModule {
public:

	DeathPosition();
	~DeathPosition();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(GameMode* gm) override;
};

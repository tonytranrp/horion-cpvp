#pragma once
#include "../Module.h"
class AutoGapple : public IModule {
public:
	AutoGapple();
	~AutoGapple();
	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(GameMode* gm) override;
};

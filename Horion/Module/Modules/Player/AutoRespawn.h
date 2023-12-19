#pragma once
#include "../Module.h"
class AutoRespawn : public IModule {
public:
	AutoRespawn();
	~AutoRespawn();
	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(GameMode* gm) override;
};
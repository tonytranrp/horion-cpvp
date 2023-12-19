#pragma once
#include "../../../../Utils/Target.h"
#include "../../ModuleManager.h"
#include "../Module.h"

class Hitbox : public IModule {
public:
	float height = 2;
	float width = 4;
	float range = 8;
	bool includeMobs = false;

	Hitbox();
	~Hitbox();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual std::string getModeText() override;
	virtual void onTick(GameMode* gm) override;
};

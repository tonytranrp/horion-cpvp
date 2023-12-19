#pragma once

#include "../../ModuleManager.h"
#include "../Module.h"

class InfiniteAura : public IModule {
private:
	int delay = 15;
	int Odelay = 0;
	bool isMulti = false;
	bool mob = false;

public:
	float range = 15;
	InfiniteAura();
	~InfiniteAura();
	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual std::string getModeText() override;
	virtual void onTick(GameMode* gm) override;

	inline bool shouldTargetMobs() const { return this->mob; }
};

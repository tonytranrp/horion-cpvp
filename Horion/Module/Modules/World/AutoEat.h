#pragma once
#include "../Module.h"
class AutoEat : public IModule {
public:
	AutoEat();
	~AutoEat();
	int hunger = 19;
	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual std::string getModeText() override;
	virtual void onTick(GameMode* gm) override;
};
#pragma once
#include "../../ModuleManager.h"
#include "../Module.h"
class AutoPot : public IModule {
public:
	AutoPot();
	~AutoPot();
	int pitch = 0;
	int health = 0;
	int potTimer = 20;
	int throwCoolDown;
	bool rotate = false;
	bool shouldThrow = true;
	int prevSlot = 99;
	bool hasPot();
	void throwPot();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual std::string getModeText() override;
	virtual void onTick(GameMode* gm) override;
};
#pragma once
#include "../Module.h"
class AutoMine : public IModule {
public:
	AutoMine();
	~AutoMine();

	ClientInputCallbacks* InputCallback = nullptr;

	// Inherited via IModule
	virtual const char* getModuleName() override;
	
	virtual void onTick(GameMode* gm) override;
	virtual void onDisable() override;
};
#pragma once
#include "../Module.h"
class Swing : public IModule {
public:
	Swing();
	~Swing();

	SettingEnum selectedSwing;

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(GameMode*) override;
	virtual void onDisable() override;
	virtual void onEnable() override;
};

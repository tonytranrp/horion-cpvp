#pragma once
#include "../Module.h"
class NoRender : public IModule {
public:
	NoRender();
	~NoRender();

	bool noFire = true;
	bool noWeather = true;
	bool noNausea = true; 
	bool noEntities = false;
	bool noBlockEntities = false;
	bool noParticles = false;

	bool* noWeatherSetting = nullptr;
	bool* noEntitySetting = nullptr;
	bool* noBlockEntitySetting = nullptr;
	bool* noParticleSetting = nullptr;

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(GameMode* gm) override;
	virtual void onDisable() override;
};
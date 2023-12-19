#pragma once
#include "../../ModuleManager.h"
#include "../Module.h"

class EntityDespawner : public IModule {
public:
	EntityDespawner();
	~EntityDespawner();

	bool PassiveMobs = false;
	bool Projectiles = false;
	bool Players = false;
	bool Boats = false;
	bool Minecarts = false;
	bool FallingBlocks = true;
	bool Items = false;
	bool XP = false;
	bool TNT = false;
	bool ArmorStands = false;
	bool Paintings = false;
	bool EffectClouds = false;
	bool EndCrystals = false;
	bool Misc = false;

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(GameMode* gm) override;
};

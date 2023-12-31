#pragma once
#include "../../../../Utils/Target.h"
#include "../../ModuleManager.h"
#include "../Module.h"

struct CrystalPlacements {
	Vec3 toPlace;
	float enemyDmg = 0.f;
	float selfDmg = 0.f;
};

struct CrystalInfo {
	CrystalPlacements CPlacements;
	Entity* ent;
};

struct lineResults {
	float blockCount = 0;
	Vec3 lastSolidBlock;
};
class CrystalAura : public IModule {
private:
	
	int origSlot;
	int ctr = 0;

	Vec3 toPlace;

	bool currentlySwitchingHotbar = false;
	float dmg_nukkit = false;

public:
	std::vector<CrystalInfo> CJTWDPlaceArr;
	Vec2 rotAngle;
	bool placeArrEmpty = false;
	bool rotUpNow = false;

	int delay = 0;  // Time to wait (in ticks) until to place a new crystal
	int maxProximity = 4;    // What is the maximum distance can a crystal be placed from a person before switching axis
	float range = 8;         // Range for the enemies to be added to target list
	float placeRange = 6.f;  // Range to place endCrystals
	SettingEnum priority;    // Decides how targets are sorted (distance-> lowest to highest
							 //								   health-> lowest to highest)
	float thruWallsR = 6;    // How many blocks you are allowed to place through walls
	float postWallsR = 10;   // Maximum distance to place *after* going through a wall
	bool safetyFirst = false;  // Whether to prioritize (reduce) self-damage over (maximizing) enemy damage
	float minHealth = 5.f;     // What is the minimum health you should have to stop placing crystals
	float maxSelfDmg = 5.f;    // Maximum damage allowable to player
	float minEnemDmg = 10.f;   // How much damage the crystals do to enemies, ON MINIMUM

	SettingEnum facePlaceType;  // Controls faceplacing (none->obvious
								//                       smart->faceplaces only when the enemy is at a certain health)
	float fpThresh = 20.f;      // Only when facePlaceType is set to smart [if this is set to 20, client always faceplaces]
	float dmgAtThresh = 3.f;    // When fpThreshold is reached, how much damage do the crystals need to deliver

	bool renderPlacing = true;  // Whether to render place position
	

	CrystalAura();
	~CrystalAura();
	float computeExplosionDamage(Vec3 crystalPos, Entity* target, BlockSource* reg);
	float getBlastDamageEnchantReduction(ItemStack* armor);
	CrystalPlacements CrystalAuraJTWD(Entity* target);
	virtual const char* getModuleName() override;
	virtual void onTick(GameMode* gm) override;
	virtual void onEnable() override;
	virtual void onDisable() override;
	virtual void onPreRender(MinecraftUIRenderContext* renderCtx) override;
	virtual void onSendPacket(Packet* packet) override;
};

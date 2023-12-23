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
	// int placetimes = 1;     // Number of times the client should keep retrying to place a crystal
	int maxProximity = 4;    // What is the maximum distance can a crystal be placed from a person before switching axis
	float range = 8;         // Range for the enemies to be added to target list
	float placeRange = 6.f;  // Range to place endCrystals
	SettingEnum priority;    // Decides how targets are sorted (distance-> lowest to highest
							 //								   health-> lowest to highest)
	float thruWallsR = 6;    // How many blocks you are allowed to place through walls
	float postWallsR = 10;   // Maximum distance to place *after* going through a wall

	/* SOON
	int lookAhead = 5;		// how many ticks in the future to predict future enemy location (set to 0 to disable predict)
	*/

	

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

	//bool isPlaceValid(Vec3 location, Entity* atkObj);
	//bool tryRaytrace(Vec3 pos, Vec3 end, BlockSource* region);
	// T H E  H O L Y  T R I N I T Y
	float computeExplosionDamage(Vec3 crystalPos, Entity* target, BlockSource* reg);
	float getBlastDamageEnchantReduction(ItemStack* armor);
	// float calculateBlockDensity(C_Entity* targetEntity, Vec3 explosionPosition);
	//std::vector<CrystalPlacements> GetCrystalPlacements(Entity* target, int maxVOff);
	//std::vector<CrystalPlacements> generateValidPlacements(Entity* target, int yOffset);

	//CrystalPlacements bestPlaceOnPlane(Entity* target, int yOffset);

	CrystalPlacements CrystalAuraJTWD(Entity* target);

	//bool getCrystalSpoof();
	//virtual void onLevelRender() override;
	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(GameMode* gm) override;
	//virtual void onWorldTick(GameMode* gm);
	// virtual void onPlayerTick(C_GameMode* gm) override;
	virtual void onEnable() override;
	virtual void onDisable() override;
	virtual void onPreRender(MinecraftUIRenderContext* renderCtx) override;
	virtual void onSendPacket(Packet* packet) override;
};

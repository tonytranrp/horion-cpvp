#pragma once

#include "../Utils/HMath.h"
#include "ClientInstance.h"
#include "Entity.h"

class GameMode {
private:
	virtual __int64 destructorGameMode();
	// Duplicate destructor
public:
	virtual __int64 startDestroyBlock(Vec3i const & pos, unsigned char blockSide, bool & isDestroyedOut);
	virtual bool destroyBlock(const Vec3i &, unsigned char blockSide);
	virtual __int64 continueDestroyBlock(Vec3i const &, unsigned char blockSide, bool & isDestroyedOut);
	virtual __int64 stopDestroyBlock(Vec3i const &);
	virtual __int64 startBuildBlock(Vec3i const &, unsigned char blockSide);
	virtual bool buildBlock(const Vec3i &, unsigned char blockSide, bool unk = false);
	virtual __int64 continueBuildBlock(Vec3i const &, unsigned char blockSide);
	virtual __int64 stopBuildBlock(void);
	virtual __int64 tick(void);

private:
	virtual __int64 getPickRange(__int64 const &, bool);
	virtual __int64 useItem(__int64 &);
	virtual __int64 useItemOn(__int64 &, Vec3i const &, unsigned char, Vec3 const &, __int64 const *);
	virtual __int64 interact(Entity &, Vec3 const &);

public:
	virtual __int64 attack(Entity *);

public:
	virtual __int64 releaseUsingItem(void);

public:
	virtual void setTrialMode(bool);
	virtual bool isInTrialMode(void);

private:
	virtual __int64 registerUpsellScreenCallback(__int64);

public:
	Player *player;

	void survivalDestroyBlockHack(Vec3i const &block, int face, bool &isDestroyedOut, bool isFirst);
	void baseUseItem(ItemStack &a1);
};
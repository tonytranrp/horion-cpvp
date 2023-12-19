#include "EntityDespawner.h"

EntityDespawner::EntityDespawner() : IModule(0x0, Category::VISUAL, "Despawns entities within your render distance, turn off and come back into render distance or rejoin world to see despawned mobs again.") {
	registerBoolSetting("Passive Mobs", &this->PassiveMobs, this->PassiveMobs);
	registerBoolSetting("Falling Blocks", &this->FallingBlocks, this->FallingBlocks);
	registerBoolSetting("Effect Cloud", &this->EffectClouds, this->EffectClouds);
	registerBoolSetting("Paintings", &this->Paintings, this->Paintings);
	registerBoolSetting("Armour Stands", &this->ArmorStands, this->ArmorStands);
	registerBoolSetting("Players", &this->Players, this->Players);
	registerBoolSetting("Projectiles", &this->Projectiles, this->Projectiles);
	registerBoolSetting("Items", &this->Items, this->Items);
	registerBoolSetting("XP", &this->XP, this->XP);
	registerBoolSetting("EndCrystal", &this->EndCrystals, this->EndCrystals);
	registerBoolSetting("PrimedTnt", &this->TNT, this->TNT);
	registerBoolSetting("Boats", &this->Boats, this->Boats);
	registerBoolSetting("Minecarts", &this->Minecarts, this->Minecarts);
	registerBoolSetting("Misc", &this->Misc, this->Misc);
}

EntityDespawner::~EntityDespawner() {
}

const char* EntityDespawner::getModuleName() {
	return ("EntityDespawner");
}

static std::vector<Entity*> targetList;

void EntityToDespawn(Entity* currentEntity) {
	static auto EntityDespawnMod = moduleMgr->getModule<EntityDespawner>("EntityDespawner");
	if (!currentEntity) return;

	if (EntityDespawnMod->PassiveMobs) {
		int PassiveMobs[] = {10, 11, 12, 13, 14, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 74, 75, 108, 109, 111, 112, 113, 121, 122, 125, 128, 129, 130};
		for (int MobIds : PassiveMobs) {
			if (currentEntity->getEntityTypeId() == MobIds)
				targetList.push_back(currentEntity);
		}
	}
	if (EntityDespawnMod->Projectiles) {
		int Projectiles[] = {68, 70, 72, 76, 79, 80, 81, 82, 85, 86, 87, 89, 91, 94, 101, 102};
		for (int MobIds : Projectiles) {
			if (currentEntity->getEntityTypeId() == MobIds)
				targetList.push_back(currentEntity);
		}
	}
	if (EntityDespawnMod->Boats) {
		int Boats[] = {90, 218};
		for (int MobIds : Boats) {
			if (currentEntity->getEntityTypeId() == MobIds)
				targetList.push_back(currentEntity);
		}
	}
	if (EntityDespawnMod->Minecarts) {
		int Minecarts[] = {84, 96, 97, 98, 100};
		for (int MobIds : Minecarts) {
			if (currentEntity->getEntityTypeId() == MobIds)
				targetList.push_back(currentEntity);
		}
	}
	if (EntityDespawnMod->Misc) {
		int Misc[] = {56, 62, 67, 77, 78, 88, 93, 103, 106, 107, 117, 120};
		for (int MobIds : Misc) {
			if (currentEntity->getEntityTypeId() == MobIds)
				targetList.push_back(currentEntity);
		}
	}
	if (EntityDespawnMod->ArmorStands && currentEntity->getEntityTypeId() == 61) {
		targetList.push_back(currentEntity);
	}
	if (EntityDespawnMod->Players && currentEntity != Game.getLocalPlayer() && currentEntity->getEntityTypeId() == 63) {
		targetList.push_back(currentEntity);
	}
	if (EntityDespawnMod->Items && currentEntity->getEntityTypeId() == 64) {
		targetList.push_back(currentEntity);
	}
	if (EntityDespawnMod->TNT && currentEntity->getEntityTypeId() == 65) {
		targetList.push_back(currentEntity);
	}
	if (EntityDespawnMod->FallingBlocks && currentEntity->getEntityTypeId() == 66) {
		targetList.push_back(currentEntity);
	}
	if (EntityDespawnMod->XP && currentEntity->getEntityTypeId() == 69) {
		targetList.push_back(currentEntity);
	}
	if (EntityDespawnMod->EndCrystals && currentEntity->getEntityTypeId() == 71) {
		targetList.push_back(currentEntity);
	}
	if (EntityDespawnMod->Paintings && currentEntity->getEntityTypeId() == 83) {
		targetList.push_back(currentEntity);
	}
	if (EntityDespawnMod->EffectClouds && currentEntity->getEntityTypeId() == 95) {
		targetList.push_back(currentEntity);
	}
}

void EntityDespawner::onTick(GameMode* gm) {
	if (!Game.getLocalPlayer()) return;
	targetList.clear();
	Game.forEachEntity(EntityToDespawn);
	if (targetList.empty()) return;
	for (auto& i : targetList) {
		i->despawn();
	}
}
#pragma once

#include "../Utils/HMath.h"
#include "../Utils/Utils.h"
#include "BlockLegacy.h"

#include "ClientInstance.h"
#include "mce.h"
#include <functional>

enum class LevelSoundEvent : int32_t;
enum class BuildPlatform : int32_t;

class EntityDefinitionIdentifier;
class Dimension;

enum class HitResultType : int32_t {
	Tile = 0,
	Entity = 1,
	EntityOutOfRange = 2,
	Miss = 3,
};

// https://github.com/Sandertv/gophertunnel/blob/a072417a428c601176d9f7d2172eec5bfde38c07/minecraft/protocol/player.go#L48
struct PlayerListEntry {
	ActorUniqueID uniqueId;
	mce::UUID uuid;
	std::string name, xuid, platformOnlineId;
	BuildPlatform buildPlatform;
	SerializedSkin skin;
	bool isTeacher, isHost;
};

struct HitResult {
	Vec3 startPos;
	Vec3 rayDir;
	HitResultType type;
	uint8_t blockFace;
	Vec3i block;
	Vec3 hitpos;
	WeakEntityRef weakEntity;
	bool hitLiquid;
	bool liquidFacing;
	Vec3i liquid;
	Vec3 liquidPos;
	bool indirectHit;
};
static_assert(sizeof(HitResult) == 0x70);

class LevelData {
public:
	CLASS_FIELD(hasAchievementsDisabled, 0x45C, bool); // xref: LevelData::hasAchievementsDisabled

	int64_t get64BitSeed() const;
};

class Level {
	uintptr_t **vtable;

public:
	CLASS_FIELD(playerListEntries, 0x8F8, std::unordered_map<mce::UUID, PlayerListEntry>);  // xref: Level::getPlayerList

	void forEachPlayerListEntry(std::function<bool(PlayerListEntry &entry)> callback) const;
	class LevelData *getLevelData();
	HitResult &getHitResult();
	HitResult &getLiquidHitResult();
	bool isLookingAtEntity();
	Entity *getLookingAtEntity();
	std::vector<Entity *> getRuntimeActorList();
	void forEachPlayer(const std::function<bool __cdecl(Player &)> callback);
};

class Weather {
public:
	CLASS_FIELD(rainingLevel, 0x38, float);
	CLASS_FIELD(lightningLevel, 0x44, float);

	inline bool isRaining() const { return this->rainingLevel > 0.f; }
	inline bool isLightning() const { return this->lightningLevel > 0.f; }
};

// enum class DimensionID : int32_t
namespace DimensionID {
	constexpr inline int32_t OVERWORLD = 0, NETHER = 1, END = 2, UNDEFINED = 3;
};

class Dimension {
public:
	CLASS_FIELD(name, 0x150, std::string); // xref: Dimension::Dimension
	CLASS_FIELD(id, 0x170, int32_t);  // xref: Dimension::getDimensionId, always right after name
	CLASS_FIELD(weather, 0x1A8, std::unique_ptr<Weather>); // xref: Actor::isInRain

	void createGenerator(std::unique_ptr<class WorldGenerator> *generatorOut);
};

enum class StructureFeatureType : int8_t {
	END_CITY = 1,
	NETHER_FORTRESS = 2,
	MINESHAFT = 3,
	OCEAN_MONUMENT = 4,
	STRONGHOLD = 5,
	DESERT_TEMPLE = 6,
	VILLAGE = 7,
	MANSION = 8,
	SHIPWRECK = 9,
	BURIED_TREASURE = 10,
	OCEAN_RUINS = 11,
	PILLAGER_OUTPOST = 12,
	RUINED_PORTAL = 13,
	BASTION_REMNANT = 14,
	ANCIENT_CITY = 15
};

class WorldGenerator {
	uintptr_t **vtable;
public:
	bool findNearestStructureFeature(StructureFeatureType type, const Vec3i &origin, Vec3i *posOut, bool newChunksOnly);
	class BiomeSource *getBiomeSource();
};
#include "Level.h"
#include "../Utils/Utils.h"
#include "../Memory/GameData.h"

int64_t LevelData::get64BitSeed() const {
	using seedFunc_t = int64_t *(__cdecl *)(const LevelData *, int64_t *);
	static auto seedFunc = reinterpret_cast<seedFunc_t>(FindSignature(
		"48 89 5C 24 ? 57 48 83 EC 20 4C 8B 05 ? ? ? ? 48 8B DA 48 8B F9 48 8D 15 ? ? ? ? 48 81 C1 ? ? ? ? E8 ? ? ? ? 48 3B 87"));
	int64_t result;
	seedFunc(this, &result);
	return result;
}

// to not go trough the entity list twice
bool Level::isLookingAtEntity() {
	return this->getHitResult().type == HitResultType::Entity;
}

// returns the entity that the player is looking at
Entity *Level::getLookingAtEntity() {
	if (!this->isLookingAtEntity()) return nullptr;
	const auto &hitRes = this->getHitResult();
	Entity *retval = nullptr;
	Game.forEachEntity([&hitRes, &retval](Entity *ent) {
		if (*(__int64 *)((__int64)ent + 0x10) == hitRes.weakEntity.id.value) {
			retval = ent;
			return;
		}
	});
	return retval;
}

void Level::forEachPlayerListEntry(std::function<bool (PlayerListEntry& entry)> callback) const {
	for (auto &[k, v] : this->playerListEntries) {
		if (!callback(v)) return;
	}
}

LevelData* Level::getLevelData() {
	return Utils::CallVFunc<152, LevelData *>(this);
}

HitResult &Level::getHitResult() {
	return *Utils::CallVFunc<326, HitResult *>(this);
}

HitResult &Level::getLiquidHitResult() {
	return *Utils::CallVFunc<327, HitResult *>(this);
}

std::vector<Entity *> Level::getRuntimeActorList() {
	std::vector<Entity *> listOut;
	Utils::CallVFunc<317, decltype(&listOut)>(this, &listOut);
	return listOut;
}

// the symbol says to take the callback by value but the compiler optimized it to pass the address
void Level::forEachPlayer(const std::function<bool __cdecl(Player &)> callback) {
	Utils::CallVFunc<226, void>(this, &callback);
}

void Dimension::createGenerator(std::unique_ptr<class WorldGenerator> *generatorOut) {
	Utils::CallVFunc<5, void*>(this, generatorOut);
}

bool WorldGenerator::findNearestStructureFeature(StructureFeatureType type, const Vec3i &origin, Vec3i *posOut, bool newChunksOnly) {
	return Utils::CallVFunc<32, bool>(this, type, origin, posOut, newChunksOnly);
}

class BiomeSource *WorldGenerator::getBiomeSource() {
	return Utils::CallVFunc<37, BiomeSource *>(this);
}
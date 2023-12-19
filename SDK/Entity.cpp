#include "Entity.h"
#include "../Utils/Utils.h"
#include "../Memory/GameData.h"
#include "BuildPlatform.h"
#include "ActorFlags.h"
#include "Attribute.h"
#include "Abilities.h"

int32_t Entity::getDimensionId() const {
	auto dim = this->getDimension();
	return dim ? dim->id : DimensionID::UNDEFINED;
}

AABB *Entity::getAABB() {
	return &this->aabbShape->aabb;
}


uint32_t Entity::getNumericalEntityId() const {
	return this->entityContext.id.value;
}

InventoryTransactionManager *LocalPlayer::getTransactionManager() {
	static unsigned int offset = 0;
	if (!offset)
		offset = *reinterpret_cast<int *>(FindSignature("48 8D 8F ? ? ? ? E8 ? ? ? ? 90 48 8D 8D ? ? ? ? E8 ? ? ? ? 48 8D 8D ? ? ? ? E8 ? ? ? ? 90 48 8D 05 ? ? ? ?") + 3);  // 48 8D 8E ? ? ? ? E8 ? ? ? ? 90 48 8D 8D ? ? ? ? E8 ? ? ? ? E9 ? ? ? ? 48 8B 45 ? 8B 80 ? ? ? ? 44 88 75 ? ? ? ? 45 33 C0 // this should work but its being cringe

	return reinterpret_cast<InventoryTransactionManager *>((uintptr_t)(this) + offset);
}

Vec3 LocalPlayer::getEyePos() {
	return Game.getLevel()->getHitResult().startPos;
}

float& Entity::getFallDistance() {
	using get_fall_distance_t = float&(__fastcall*)(Entity*);
	static get_fall_distance_t getFallDistance = reinterpret_cast<get_fall_distance_t>(FindSignature("48 83 EC ? 48 8B 41 ? 48 8B D1 48 8B 08 8B 42 ? 48 8D 54 24 ? 89 44 24 ? E8 ? ? ? ? 48 85 C0 74 ? F3 0F 10 00 48 83 C4 ? C3 E8 ? ? ? ? CC CC CC CC CC CC CC CC CC CC CC CC CC CC 48 83 EC ? 8B 41"));
	return getFallDistance(this);
}

Vec3i Entity::getFeetBlockPos() const {
	auto& vec = *this->getPos();
	return Vec3i{
		static_cast<int32_t>(std::floorf(vec.x)),
		static_cast<int32_t>(std::floorf(this->aabbShape->aabb.lower.y)),
		static_cast<int32_t>(std::floorf(vec.z))
	};
}

Dimension* Entity::getDimension() const {
	auto &dim = this->dimension;
	if (!dim.expired()) {
		auto lock = dim.lock();
		return lock.get();
	}
	return nullptr;
}

bool Entity::getStatusFlag(ActorFlags flag) {
	return Utils::CallVFunc<0, bool>(this, flag);
}

void Entity::setStatusFlag(ActorFlags flag, bool toggle) {
	Utils::CallVFunc<1, void>(this, flag, toggle);
}

std::string const& Entity::getNameTag() {
	return Utils::CallVFunc<64, std::string const &>(this);
}

bool Entity::isSneaking() {
	return this->getStatusFlag(ActorFlags::SNEAKING);
}

bool Entity::isSprinting() {
	return this->getStatusFlag(ActorFlags::SPRINTING);
}

bool Entity::isSwimming() {
	return this->getStatusFlag(ActorFlags::SWIMMING);
}

bool Entity::isPlayer() {
	return Utils::CallVFunc<112, bool>(this);
}

bool Entity::isAlive() {
	return Utils::CallVFunc<104, bool>(this);
}

bool Entity::isInWater() {
	return Utils::CallVFunc<72, bool>(this);
}

bool Entity::hasEnteredWater() {
	return Utils::CallVFunc<73, bool>(this);
}

bool Entity::isOverWater() {
	return Utils::CallVFunc<76, bool>(this);
}

bool Entity::isInWall() {
	return Utils::CallVFunc<59, bool>(this);
}

bool Entity::isCreative() {
	return Utils::CallVFunc<224, bool>(this);
}

bool Entity::isSurvival() {
	return Utils::CallVFunc<226, bool>(this);
}

bool Entity::isAlliedTo(Entity *ent) {
	return Utils::CallVFunc<306, bool>(this, ent);
}

bool Entity::isInvisible() {
	return Utils::CallVFunc<60, bool>(this);
}

bool Entity::isSilent() {
	return Utils::CallVFunc<94, bool>(this);
}

bool Entity::isImmobile() {
	return Utils::CallVFunc<93, bool>(this);
}

bool Entity::canAttack(Entity *ent) {
	return Utils::CallVFunc<114, bool>(this, ent, false);
}

bool Entity::canShowNameTag() {
	return Utils::CallVFunc<61, bool>(this);
}

bool Entity::isInLava() {
	return Utils::CallVFunc<74, bool>(this, Game.getRegion());
}

void Entity::setPos(Vec3 const &pos) {
	return Utils::CallVFunc<20, void>(this, &pos);
}

void Entity::setOffhandSlot(ItemStack *stack) {
	return Utils::CallVFunc<161, void>(this, stack);
}

void Entity::save(CompoundTag &tag) {
	return Utils::CallVFunc<164, void>(this, &tag);
}

ItemStack *Entity::getArmor(int32_t armorSlot) {
	return Utils::CallVFunc<152, ItemStack*>(this);
}

void Entity::swing() {
	return Utils::CallVFunc<216, void>(this);
}

int32_t Entity::getEntityTypeId() {
	return Utils::CallVFunc<168, int32_t>(this);
}

void Entity::setSprinting(bool value) {
	return Utils::CallVFunc<280, void>(this, value);
}

void Entity::jumpFromGround() {
	return Utils::CallVFunc<348, void>(this);
}

void Entity::lerpMotion(Vec3 const& pos) {
	return Utils::CallVFunc<47, void>(this, &pos);
}

void Entity::setSize(float width, float height) {
	return Utils::CallVFunc<234, void>(this);
}

void Entity::resetBlockMovementSlowdownMultiplier() {
	return Utils::CallVFunc<78, void>(this);
}

int32_t Entity::getDeathTime() {
	return Utils::CallVFunc<205, int32_t>(this);
}

void Entity::despawn() {
	return Utils::CallVFunc<148, void>(this);
}

Vec3 *Entity::getPos() const {
	return &this->stateVector->pos;
}

Vec3 *Entity::getPosOld() const {
	return &this->stateVector->oldPos;
}

// xref: Actor::getRuntimeID
// sig to try_get<RuntimeIDComponent>
ActorRuntimeID Entity::getRuntimeId() {
	using get_rid_t = ActorRuntimeID*(__cdecl*)(BloatRegistry *, const EntityId &);
	static auto func = ([]() -> get_rid_t {
		auto sig = FindSignature("E8 ? ? ? ? 48 85 C0 0F 84 ? ? ? ? 4C 8D ? ? ? ? ? 4C");
		return Utils::FuncFromSigOffset<get_rid_t>(sig, 1);
	})();

	auto reg = *(this->entityContext.registry);
	const auto &id = this->entityContext.id;
	auto ret = func(reg, id);
	return (ret ? *ret : ActorRuntimeID{});
}

ActorUniqueID Entity::getUniqueId() {
	using getUniqueId_t = ActorUniqueID*(__cdecl *)(Entity*);
	static auto getUniqueId = ([]() -> getUniqueId_t {
		auto sig = FindSignature("E8 ? ? ? ? 41 B8 FB ? ? ? 48 8B D0");
		return Utils::FuncFromSigOffset<getUniqueId_t>(sig, 1);
	})();
	auto ret = getUniqueId(this);
	return (ret ? *ret : ActorUniqueID{});
}

// xref: ActorCollision::wasOnGround
// sig to EntityContextBase::hasComponent<FlagComponent<WasOnGroundFlag>>
bool Entity::wasOnGround() {
	using has_ground_flag_t = bool(__cdecl *)(EntityContext *);
	static auto func = ([]() -> has_ground_flag_t {
		auto sig = FindSignature("E8 ? ? ? ? 0F B6 D8 48 8D 4E 08");
		return Utils::FuncFromSigOffset<has_ground_flag_t>(sig, 1);
	})();
	auto &ctx = this->entityContext;
	return func(&ctx);
}

// xref: ActorCollision::isOnGround
// sig to EntityContextBase::hasComponent<FlagComponent<OnGroundFlag>>
// sig to this call site is right below the wasOnGround func is called
bool Entity::isOnGround() {
	using has_ground_flag_t = bool(__cdecl*)(EntityContext *);
	static auto func = ([]() -> has_ground_flag_t {
		auto sig = FindSignature("E8 ? ? ? ? 44 0F B6 F8 48 8B CE");
		return Utils::FuncFromSigOffset<has_ground_flag_t>(sig, 1);
	})();
	auto &ctx = this->entityContext;
	return func(&ctx);
}

// ActorCollision::setOnGround
void Entity::setOnGround(bool value) {
	using setOnGround_t = void(__cdecl*)(EntityContext&, bool);
	static auto setOnGround = reinterpret_cast<setOnGround_t>(FindSignature("40 53 48 83 EC ? ? ? ? 48 8B 09 ? ? ? ? 48 8D 54 24"));
	setOnGround(this->entityContext, value);
}

bool Entity::collidedHorizontally() {
	using hasHorizontalCollision_t = bool(__cdecl *)(EntityContext *);
	static auto func = ([]() -> hasHorizontalCollision_t {
		auto sig = FindSignature("E8 ? ? ? ? 48 8B 8F ? ? ? ? 84 C0 48 8B 11");
		return Utils::FuncFromSigOffset<hasHorizontalCollision_t>(sig, 1);
	})();
	auto &ctx = this->entityContext;
	return func(&ctx);
}

bool Entity::collidedVertically() {
	using hasVerticalCollision_t = bool(__cdecl *)(EntityContext *);
	static auto func = ([]() -> hasVerticalCollision_t {
		auto sig = FindSignature("E8 ? ? ? ? 84 C0 74 ? ? ? ? 0F 28 74 ? ? 48 83 C4");
		return Utils::FuncFromSigOffset<hasVerticalCollision_t>(sig, 1);
	})();
	auto &ctx = this->entityContext;
	return func(&ctx);
}

bool Entity::collided() {
	return collidedHorizontally() || collidedVertically();
}

AttributeInstance *Entity::getAttribute(AttributeID id) {
	Attribute attr(id);
	return Utils::CallVFunc<204, AttributeInstance*>(this, &attr);
}

float Entity::getHealth() {
	return this->getAttribute(AttributeID::Health)->currentValue;
}

float Entity::getAbsorption() {
	return this->getAttribute(AttributeID::Health)->currentValue;
}

// ideally we should use an impl of entt getComponent for UserEntityIdentifierComponent
// then we can better map playerlistentries to real players instead of comparing names
std::optional<mce::UUID> Player::getUUIDFromPlayerList() const {
	const auto &worldName = this->playerName;
	const auto &playerListMap = Game.getLevel()->playerListEntries;
	for (const auto &[k, v] : playerListMap) {
		if (v.name == worldName) {
			return k;
		}
	}
	return {};
}

BuildPlatform Player::getBuildPlatformFromPlayerList() const {
	const auto &uuidToFind = this->getUUIDFromPlayerList();
	BuildPlatform out = BuildPlatform::Unknown;
	Game.getLevel()->forEachPlayerListEntry([&](PlayerListEntry &entry) -> bool {
		if ((uuidToFind.has_value() && (entry.uuid == uuidToFind))) {
			out = entry.buildPlatform;
			return false;
		}
		return true;
	});
	return out;
}

MinecraftEventing* Player::getEventing() {
	return Utils::CallVFunc<409, MinecraftEventing*>(this);
}

int64_t Player::getUserId() {
	return Utils::CallVFunc<410, int64_t>(this);
}

void Player::setPlayerGameType(int32_t mode) {
	return Utils::CallVFunc<406, void>(this, mode);
}

void Player::addExperience(int32_t amount) {
	return Utils::CallVFunc<412, void>(this, amount);
}

void Player::addLevels(int32_t amount) {
	return Utils::CallVFunc<413, void>(this, amount);
}

bool Player::canOpenContainerScreen() {
	return Utils::CallVFunc<383, bool>(this);
}

ItemStack *Player::getSelectedItem() {
	auto supplies = this->getSupplies();
	return supplies->inventory->getItemStack(supplies->selectedHotbarSlot);
}

int Player::getSelectedItemId() {
	auto item = getSelectedItem();
	if (!item || !item->item)
		return 0;
	if (!item->getItem())
		return 0;
	return item->getItem()->itemId;
}

PlayerInventoryProxy *Player::getSupplies() {
	static unsigned int offset = 0;
	if (!offset)
		offset = *reinterpret_cast<int *>(FindSignature("4C 8B 82 ? ? ? ? 41 80 B8 ? ? ? ? ? ? ? 49 8B 88 ? ? ? ? 41 8B 50 ? 48 8B 01 48 8B 40 28 ? ? ? ? ? ? 48 8B 53 08") + 3);  // GameMode::startDestroyBlock -> GameMode::_canDestroy -> getSupplies

	return *reinterpret_cast<PlayerInventoryProxy **>(reinterpret_cast<__int64>(this) + offset);
}

// public: virtual void MinecraftEventing::fireEventSetValidForAchievements(class Player *, bool)
// public: static void MinecraftEventing::fireEventAwardAchievement(class Player *, enum MinecraftEventing::AchievementIds)
void LocalPlayer::unlockAchievments(std::optional<int32_t> specificAchievementId) {
	// MinecraftEventing doesn't seem to care about what this flag is
	/*auto &lvlData = *this->level->getLevelData();
	if (lvlData.hasAchievementsDisabled) { // vanilla has it as "is disabled" instead of "is enabled" so i'll do it how they do it
		lvlData.hasAchievementsDisabled = false;
	}*/

	using fireSetCanEarnAchievements_t = void(__cdecl *)(MinecraftEventing *, Player *, bool /*toggle*/);
	static auto fireSetCanEarnAchievementsFunc = reinterpret_cast<fireSetCanEarnAchievements_t>(
		FindSignature("48 85 D2 0F 84 ? ? ? ? 48 89 5C 24 18 57 48 83 EC 40 48 8B 02"));
	fireSetCanEarnAchievementsFunc(this->getEventing(), this, true);

	using fireEventAward_t = void(__cdecl *)(Player *, int32_t);
	static auto fireEventAwardFunc = reinterpret_cast<fireEventAward_t>(
		FindSignature("48 85 C9 0F 84 ? ? ? ? 48 89 5C 24 ? 57 48 81 EC ? ? ? ? 48 8B 01 8B FA 48 8B D9 FF 90 ? ? ? ? 84 C0 74 6F 48 8B 03 48 8B CB FF 90"));

	if (specificAchievementId.has_value()) {
		fireEventAwardFunc(this, specificAchievementId.value());
		return;
	}
	for (int32_t i = 0; i < 118; i++) {
		fireEventAwardFunc(this, i);
	}
}

void LocalPlayer::applyTurnDelta(Vec2 *viewAngleDelta) {
	using applyTurnDelta = void(__thiscall *)(void *, Vec2 *);
	static applyTurnDelta TurnDelta = reinterpret_cast<applyTurnDelta>(FindSignature("48 8B C4 48 89 58 ? 55 56 57 ? 56 41 57 48 8D 68 ? 48 81 EC ? ? ? ? 0F 29 70 ? 0F 29 78 ? 44 0F 29 ? ? ? 0F 29 48 ? 44 0F 29 ? ? 48 8B 05 ? ? ? ? 48 33 C4"));
	TurnDelta(this, viewAngleDelta);
}

GameMode *LocalPlayer::getGameMode() {
	static unsigned int offset = 0;
	if (offset == NULL)
		offset = *reinterpret_cast<int *>(FindSignature("48 8B B5 ? ? ? ? 48 8B 06 48 8B 78 ? 48 8B 01") + 3);
	return *reinterpret_cast<GameMode **>((uintptr_t)(this) + offset);
}

void LocalPlayer::lerpTo(Vec3 const &pos, Vec2 const &rot, int32_t steps) {
	using lerpTo = void(__fastcall *)(Entity *, Vec3 const &, Vec2 const &, int32_t);
	static lerpTo lerp = reinterpret_cast<lerpTo>(FindSignature("48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 48 83 EC ? 48 8B 41 ? 48 8B F9 48 8B DA 41 8B E9"));
	lerp(this, pos, rot, steps);
}

// xref: Player::getAbilities
// sig to try_get<AbilitiesComponent>
LayeredAbilities *LocalPlayer::getAbilities() {
	using get_abilities_t = LayeredAbilities *(__cdecl *)(BloatRegistry *, const EntityId &);
	static auto func = ([]() -> get_abilities_t {
		auto sig = FindSignature("E8 ? ? ? ? 48 85 C0 0F 84 ? ? ? ? 48 8B 4D ? 44");
		return Utils::FuncFromSigOffset<get_abilities_t>(sig, 1);
	})();

	auto reg = *(this->entityContext.registry);
	const auto &id = this->entityContext.id;
	return func(reg, id);
}

void LocalPlayer::setFlying(bool toggle) {
	auto abils = this->getAbilities();
	if (abils) {
		abils->setAbility(AbilitiesIndex::MayFly, toggle);
	}
}
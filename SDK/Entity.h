#pragma once

#include "../Utils/HMath.h"
#include "../Utils/Utils.h"
#include "BlockLegacy.h"
#include "EntityList.h"
#include "Inventory.h"
#include "InventoryTransaction.h"
#include "MinecraftJson.h"
#include "mce.h"

class LayeredAbilities;
class GameMode;
class Player;
class SerializedSkin;
class MinecraftEventing;
class AttributeInstance;
class Attribute;

enum class BuildPlatform : int32_t;
enum class ActorFlags : int32_t;
enum class AttributeID : uint64_t;

class ActorRuntimeID {
public:
	uint64_t value = static_cast<uint64_t>(-1);

	inline bool operator==(const ActorRuntimeID &rhs) const { return this->value == rhs.value; }
	inline bool operator!=(const ActorRuntimeID &rhs) const { return this->value != rhs.value; }
	inline operator uint64_t() const { return this->value; }
};

struct ActorUniqueID {
	int64_t value = static_cast<int64_t>(-1);

	inline bool operator==(const ActorRuntimeID &rhs) const { return this->value == rhs.value; }
	inline bool operator!=(const ActorRuntimeID &rhs) const { return this->value != rhs.value; }
	inline operator int64_t() const { return this->value; }
};

class StateVectorComponent {
public:
	Vec3 pos;
	Vec3 oldPos;
	Vec3 velocity;
};

class AABBShapeComponent {
public:
	AABB aabb;
	float width;
	float height;
};

struct MovementInterpolatorComponent {
	Vec2 viewAngles;
	Vec2 previousViewAngles;
};

#pragma region bloat
struct EntityId {
	uint32_t value;
};

class BloatRegistry;

struct EntityContext {
	BloatRegistry **registry;
	EntityId id;
};

struct WeakEntityRef {
private:
	uint8_t bloat1[0x10]{};

public:
	EntityId id;

private:
	uint8_t bloat2[0x4]{};
};
#pragma endregion

class Entity {
public:
	CLASS_FIELD(entityContext, 0x8, EntityContext);
	CLASS_FIELD(ticksAlive, 0x1AC, int32_t);
	CLASS_FIELD(dimension, 0x200, std::weak_ptr<class Dimension>);
	CLASS_FIELD(stateVector, 0x248, StateVectorComponent *);
	CLASS_FIELD(aabbShape, 0x250, AABBShapeComponent *);
	CLASS_FIELD(viewAnglesPtr, 0x258, MovementInterpolatorComponent *);
	CLASS_FIELD(isRemoved, 0x291, bool);

	bool getStatusFlag(ActorFlags flag);
	void setStatusFlag(ActorFlags flag, bool toggle);
	std::string const &getNameTag();
	bool isSneaking();
	bool isSprinting();
	bool isSwimming();
	bool isPlayer();
	bool isAlive();
	bool isInWater();
	bool hasEnteredWater();
	bool isOverWater();
	bool isInWall();
	bool isCreative();
	bool isSurvival();
	bool isAlliedTo(Entity* ent);
	bool isInvisible();
	bool isSilent();
	bool isImmobile();
	bool canAttack(Entity* ent);
	bool canShowNameTag();
	bool isInLava();
	void setPos(Vec3 const& pos);
	void setOffhandSlot(ItemStack * stack);
	void save(CompoundTag & tag);
	ItemStack* getArmor(int32_t armorSlot);
	void swing();
	int32_t getEntityTypeId();
	void setSprinting(bool value);
	void jumpFromGround();
	void lerpMotion(Vec3 const&);
	void setSize(float, float);
	void resetBlockMovementSlowdownMultiplier();
	int32_t getDeathTime();
	void despawn();

	AttributeInstance *getAttribute(AttributeID id);
	int32_t getDimensionId() const;
	ActorUniqueID getUniqueId();
	float &getFallDistance();
	float getHealth();
	float getAbsorption();
	Vec3i getFeetBlockPos() const;
	class Dimension *getDimension() const;
	AABB *getAABB();
	uint32_t getNumericalEntityId() const;
	Vec3 *getPos() const;
	Vec3 *getPosOld() const;
	ActorRuntimeID getRuntimeId();
	bool wasOnGround();
	bool isOnGround();
	void setOnGround(bool value);
	bool collidedHorizontally();
	bool collidedVertically();
	bool collided();
};

class Player : public Entity {
public:
	CLASS_FIELD(skin, 0x7F8, class SerializedSkin); // xref: Player:getSkin
	CLASS_FIELD(gamemode, 0x1BF4, int32_t);	// xref: Player::Player, a4 function param (the only int param, trace it to `this` + offset assignment)
	CLASS_FIELD(playerName, 0x1ED0, std::string); // can reclass this easily lol
	CLASS_FIELD(deviceId, 0x1F20, std::string); // xref: Player::Player, a9 function param (trace it to `this` + offset assignment)
	CLASS_FIELD(remainingTicksUsingItem, 0xC70, int32_t); // xref: reclass
private:
	CLASS_FIELD(transac, 0x1088, InventoryTransactionManager); // xref: ClearCommand::execute at FullPlayerInventoryWrapper ctor
public:

	MinecraftEventing *getEventing();
	int64_t getUserId();
	void setPlayerGameType(int32_t mode);
	void addExperience(int32_t amount);
	void addLevels(int32_t amount);
	bool canOpenContainerScreen();

	ItemStack *getSelectedItem();
	int getSelectedItemId();
	PlayerInventoryProxy *getSupplies();
	std::optional<mce::UUID> getUUIDFromPlayerList() const;
	BuildPlatform getBuildPlatformFromPlayerList() const;
};

class LocalPlayer : public Player {
public:
	void unlockAchievments(std::optional<int32_t> specificAchievementId = {});  // todo fix this at some point
	void applyTurnDelta(Vec2 *viewAngleDelta);
	GameMode *getGameMode();
	void lerpTo(Vec3 const &pos, Vec2 const &rot, int32_t steps);
	LayeredAbilities *getAbilities();
	void setFlying(bool toggle);
	InventoryTransactionManager *getTransactionManager();
	Vec3 getEyePos();
};

enum class TrustedSkinFlag : int8_t {
	UNSET = 0,
	NO = 1,
	YES = 2,
};

class SerializedSkin {
	uint8_t pad[0x268]{};
public:
	CLASS_FIELD(name, 0x0, std::string);
	CLASS_FIELD(geometryType, 0x80, std::string);
	CLASS_FIELD(skinWidth, 0xA4, int32_t);
	CLASS_FIELD(skinHeight, 0xA8, int32_t);
	CLASS_FIELD(skinData, 0xB8, const uint8_t*);
	CLASS_FIELD(capeWidth, 0xCC, int32_t);
	CLASS_FIELD(capeHeight, 0xD0, int32_t);
	CLASS_FIELD(capeData, 0xE0, const uint8_t*);
	CLASS_FIELD(skinGeometry, 0x108, MinecraftJson::Value);
	CLASS_FIELD(trustedSkinFlag, 0x260, TrustedSkinFlag);
	CLASS_FIELD(isPremiumSkin, 0x261, bool);
	CLASS_FIELD(isPersonaSkin, 0x262, bool);
};

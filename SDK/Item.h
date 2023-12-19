#pragma once

#include "../Utils/HMath.h"
#include <cstdint>

class CompoundTag;
class Tag;
class BlockLegacy;
class BlockSource;
class Block;
class Entity;
class ItemStack;
class HashedString;
struct ItemEnchants;

enum class EnchantType : int8_t {
	Protection = 0x0,
	FireProtection = 0x1,
	FeatherFalling = 0x2,
	BlastProtection = 0x3,
	ProjectileProtection = 0x4,
	Thorns = 0x5,
	Respiration = 0x6,
	DepthStrider = 0x7,
	AquaAffinity = 0x8,
	Sharpness = 0x9,
	Smite = 0xA,
	BaneOfArthropods = 0xB,
	Knockback = 0xC,
	FireAspect = 0xD,
	Looting = 0xE,
	Efficiency = 0xF,
	SilkTouch = 0x10,
	Unbreaking = 0x11,
	Fortune = 0x12,
	Power = 0x13,
	Punch = 0x14,
	Flame = 0x15,
	Infinity = 0x16,
	LuckOfTheSea = 0x17,
	Lure = 0x18,
	FrostWalker = 0x19,
	Mending = 0x1A,
	CurseOfBinding = 0x1B,
	CurseOfVanishing = 0x1C,
	Impaling = 0x1D,
	Riptide = 0x1E,
	Loyalty = 0x1F,
	Channeling = 0x20,
	Multishot = 0x21,
	Piercing = 0x22,
	QuickCharge = 0x23,
	SoulSpeed = 0x24,
	SwiftSneak = 0x25,
	COUNT = 0x26,
	INVALID = 0x27,
};

struct EnchantmentInstance {
	EnchantType enchantType;
	int32_t level;

	EnchantmentInstance() : enchantType(EnchantType::INVALID), level(0) {}
	EnchantmentInstance(EnchantType enchantType, int32_t level) : enchantType(enchantType), level(level) {}
	inline bool operator==(EnchantmentInstance const &rhs) const { return (this->enchantType == rhs.enchantType) && (this->level == rhs.level); }
	inline bool operator!=(EnchantmentInstance const &rhs) const { return !(*this == rhs); }
};

struct ItemEnchants {
	int32_t slot;
	std::vector<EnchantmentInstance> enchants[3];

	ItemEnchants();
	bool addEnchant(EnchantType enchantType, int level, bool allowNonVanilla = true);
};

class Item {
private:
	char pad_0008[0x88];  // 0x0008
public:
	int16_t maxStackSize;  // 0x0090
	int16_t itemId;        // 0x0092
private:
	char pad_0094[4];  // 0x0094
public:
	std::string tileName;  // 0x0098
private:
	char pad_00B8[8];  // 0x00B8
public:
	std::string name;  // 0x00C0
private:
	char pad_00E0[8];  // 0x00E0
public:
	std::string nameSpace;  // 0x00E8
private:
	virtual __int64 destructor();
	virtual __int64 initServer(int &);
	virtual __int64 tearDown(void);
	virtual __int64 setDescriptionId(std::string const &);
	virtual __int64 getDescriptionId(void);
	virtual __int64 getMaxUseDuration(__int64 const *) const;

public:
	virtual __int64 getMaxUseDuration(ItemStack const *);
	virtual bool isMusicDisk(void);

private:
	virtual __int64 hasTag(HashedString const &);
	virtual __int64 hasTag(__int64 const &);
	virtual __int64 executeEvent(ItemStack &, std::string, char const &, int &);
	virtual __int64 isComponentBased(void);

public:
	virtual bool isArmor(void);
	virtual bool isBlockPlanterItem(void);
	virtual bool isBucket(void);
	virtual bool isCamera(void);
	virtual bool isCandle(void);
	virtual bool isDamageable(void);
	virtual bool isDyeable(void);
	virtual bool isDye(void);

private:
	virtual __int64 getItemColor(void);

public:
	virtual bool isFertilizer(void);
	virtual bool isFood(void);
	virtual bool isThrowable(void);
	virtual bool isUseable(void);

private:
	virtual __int64 getComponent(std::string const &);
	virtual __int64 getCamera(void);
	virtual __int64 getFood(void);

public:
	virtual __int64 setMaxStackSize(int);
	virtual __int64 setStackedByData(bool);
	virtual __int64 setMaxDamage(int);
	virtual __int64 setHandEquipped(void);
	virtual __int64 setUseAnimation(int);
	virtual __int64 setMaxUseDuration(int);
	virtual __int64 setRequiresWorldBuilder(bool);
	virtual __int64 setExplodable(bool);
	virtual __int64 setFireResistant(bool);
	virtual __int64 setIsGlint(bool);
	virtual __int64 setShouldDespawn(bool);

private:
	virtual __int64 buildNetworkTag(void);
	virtual __int64 initializeFromNetwork(CompoundTag const &);
	virtual __int64 validateFromNetwork(CompoundTag const &);
	virtual __int64 getBlockShape(void) const;

public:
	virtual bool canBeDepleted(void) const;
	virtual bool canDestroySpecial(Block const &) const;

private:
	virtual __int64 getLevelDataForAuxValue(int);

public:
	virtual bool isStackedByData(void);
	virtual short getMaxDamage(void);
	virtual __int64 getAttackDamage(void);
	virtual bool isHandEquipped(void);
	virtual bool isGlint(ItemStack const &);
	virtual bool isPattern(void);

private:
	virtual __int64 getPatternIndex(void);

public:
	virtual bool showsDurabilityInCreative(void);
	virtual bool isWearableThroughLootTable(CompoundTag const *);
	virtual bool canDestroyInCreative(void);
	virtual bool isDestructive(int);

private:
	virtual bool isLiquidClipItem(int);
	virtual bool shouldInteractionWithBlockBypassLiquid(Block const &);
	virtual bool requiresInteract(void);
	virtual __int64 appendFormattedHovertext(ItemStack const &, __int64 &, std::string &, bool) const;

public:
	virtual bool isValidRepairItem(ItemStack const &, ItemStack const &) const;

public:
	virtual __int64 getEnchantSlot(void);
	virtual __int64 getEnchantValue(void);
	virtual __int64 getArmorValue(void);
	virtual __int64 getToughnessValue(void);

private:
	virtual bool isComplex(void);
	virtual bool isValidAuxValue(int);

public:
	virtual __int64 getDamageChance(int);
	virtual __int64 getViewDamping(void);
	virtual __int64 uniqueAuxValues(void);

private:
	virtual __int64 isActorPlacerItem(void);

public:
	virtual bool isMultiColorTinted(ItemStack const &) const;

private:
	virtual __int64 getColor(CompoundTag const *, ItemStack const &);
	virtual __int64 hasCustomColor(ItemStack const &);
	virtual __int64 hasCustomColor(CompoundTag const *);
	virtual __int64 clearColor(ItemStack &);
	virtual __int64 clearColor(CompoundTag *);
	virtual __int64 SetColor(ItemStack &, int const &);
	virtual __int64 getBaseColor(ItemStack const &);
	virtual __int64 getSecondaryColor(ItemStack const &);
	virtual __int64 getActorIdentifier(ItemStack const &);
	virtual __int64 buildIdAux(short, const CompoundTag *);
	virtual __int64 canUseOnSimTick(void);
	virtual __int64 use(ItemStack &, Entity &);
	virtual __int64 dispense(Block &, __int64 &, int, Vec3 const &, char);
	virtual __int64 useTimeDepleted(ItemStack &, __int64 *, Entity *);
	virtual __int64 releaseUsing(ItemStack &, Entity *, int);
	virtual __int64 getDestroySpeed(ItemStack const &, Block const &);
	virtual __int64 hurtActor(ItemStack &, Entity &, Entity &);
	virtual __int64 hitActor(ItemStack &, Entity &, Entity &);
	virtual __int64 hitBlock(ItemStack &, Block const &, Vec3 const &, Entity &);
	virtual __int64 mineBlock(__int64 &&, Block const &, int, int, int, Entity *);
	virtual __int64 mineBlock(ItemStack &, Block const &, int, int, int, Entity *);
	virtual __int64 buildDescriptionName(ItemStack const &);
	virtual __int64 buildDescriptionId(ItemStack const &) const;
	virtual __int64 buildEffectDescriptionName(ItemStack const &);
	virtual __int64 buildCategoryDescriptionName(void);
	virtual __int64 readUserData(ItemStack &, __int64 &, __int64 &) const;
	virtual __int64 writeUserData(ItemStack const &, __int64 &) const;

public:
	virtual int getMaxStackSize(void *itemDescriptor) const;

private:
	virtual __int64 inventoryTick(ItemStack &, __int64 &, Entity &, int, bool) const;
	virtual __int64 refreshedInContainer(ItemStack &, __int64 &) const;
	virtual __int64 getCooldownType(void);
	virtual __int64 getCooldownTime(void);
	virtual __int64 fixupCommon(ItemStack &);
	virtual __int64 fixupCommon(ItemStack &, __int64 &&);

public:
	virtual short getDamageValue(CompoundTag const &) const;
	virtual void setDamageValue(ItemStack &, short);

private:
	virtual __int64 getInHandUpdateType(Entity const &, __int64 const &, __int64 const &, bool, bool) const;
	virtual __int64 getInHandUpdateType(Entity const &, ItemStack const &, ItemStack const &, bool, bool) const;
	virtual __int64 validFishInteraction(int);
	virtual __int64 enchantProjectile(ItemStack const &, Entity &);

public:
	virtual __int64 getEquipLocation(void);
	virtual __int64 getEquipSound(void);
	virtual bool isSameItem(ItemStack const &, ItemStack const &) const;

private:
	virtual __int64 initClient(__int64 &);
	virtual class Item &setIconInfo(const std::string &, int32_t);
	virtual struct ResolvedItemIconInfo getIconInfo(const class ItemStackBase &, int32_t, bool) const;
	virtual __int64 getInteractText(Entity const &);
	virtual __int64 getAnimationFrameFor(Entity *, bool, ItemStack const *, bool);

public:
	virtual bool isEmissive(int);
	virtual __int64 getLightEmission(int);

private:
	virtual __int64 getIconYOffset(void) const;

public:
	virtual bool canBeCharged(void) const;

private:
	virtual __int64 playSoundIncrementally(__int64 const &, __int64 &) const;
	virtual __int64 playSoundIncrementally(ItemStack const &, __int64 &) const;
	virtual __int64 getFurnaceBurnIntervalMultipler(ItemStack const &);
	virtual __int64 getFurnaceXPmultiplier(ItemStack const &);
	virtual __int64 getAuxValuesDescription(void);
	virtual __int64 _checkUseOnPermissions(Entity &, ItemStack &, unsigned char const &, Vec3i const &) const;
	virtual __int64 _calculatePlacePos(ItemStack &, Entity &, unsigned char &, Vec3i &) const;
	virtual __int64 _useOn(ItemStack &, Entity &, Block, bool, float, float, float);

public:
	bool isTool(void) {
		if (getAttackDamage() > 0) return true;           // Does Attack Damage
		if (itemId == 300 || itemId == 301) return true;  // Bow & Arrows
		if (itemId == 299) return true;                   // Flint n Steel
		return false;
	}
	bool isWeapon(void) {
		if (itemId == 308) return true;  // Wooden Sword
		if (itemId == 312) return true;  // Stone Sword
		if (itemId == 322) return true;  // Gold Sword
		if (itemId == 307) return true;  // Iron Sword
		if (itemId == 316) return true;  // Diamond Sword
		if (itemId == 603) return true;  // Netherite Sword
		if (itemId == 311) return true;  // Wooden Axe
		if (itemId == 315) return true;  // Stone Axe
		if (itemId == 325) return true;  // Gold Axe
		if (itemId == 298) return true;  // Iron Axe
		if (itemId == 319) return true;  // Diamond Axe
		if (itemId == 606) return true;  // Netherite Axe
		if (itemId == 546) return true;  // Trident
		return false;
	}
	bool isBlock(void) {
		auto val = *reinterpret_cast<__int64 ***>(reinterpret_cast<__int64>(this) + 0x1B8);
		return val && *val;
	}
};

class ItemDescriptor {
	uint8_t base_bad[0x10]{};

public:
	void fromStack(ItemStack *item);
};

class NetworkItemStackDescriptor : public ItemDescriptor {
	uint8_t derived_pad[0x60 - sizeof(ItemDescriptor)]{};  // 0x0000
};

class ItemStack {
private:
	uintptr_t **vTable;  // 0x0000
public:
	Item **item;       // 0x08
	CompoundTag *tag;  // 0x10
private:
	char pad_0x8[0x8];  // 0x18
public:
	int16_t aux;  // 0x20
	char count;   // 0x22
private:
	char pad_0x1B[0x7D];  // 0x23
public:
	ItemStack() {
		memset(this, 0x0, sizeof(ItemStack));
	}

	ItemStack(BlockLegacy &legacy, int count) {
		memset(this, 0x0, sizeof(ItemStack));
		reinit(legacy, count);
	}

	ItemStack(Item &item, int count, int itemData) {
		memset(this, 0x0, sizeof(ItemStack));
		reinit(item, count, itemData);
	}

	ItemStack(ItemStack const &src);

	ItemStack(Tag const &tag);

	void fromTag(Tag const &tag);

	void save(CompoundTag **tag);

	void setUserData(std::unique_ptr<Tag> tag);

	void setCustomName(std::string name);

	void setCustomLore(std::string lore);

	void applyEnchant(EnchantType enchantType, int enchantLevel);

	float getDestroySpeed(Block *block);

	ItemEnchants constructEnchantsFromUserData();

	void saveEnchantsToUserData(ItemEnchants *enchants);

	static const ItemStack &getEmptyItemStack();

	bool isValid() {
		return this->item && *this->item && (this->count > 0);
	}

	inline Item *getItem() {
		return *this->item;
	}

	void reinit(BlockLegacy &legacy, int count);

	void reinit(Item &item, int count, int itemData);

	int32_t getEnchantValue(EnchantType enchantType);
	inline int32_t getEnchantValue(int8_t enchantType) { return this->getEnchantValue(static_cast<EnchantType>(enchantType)); }

	bool isEnchanted() {
		int enchantValue = 0;
		for (int8_t i = 0; i < static_cast<int8_t>(EnchantType::COUNT); i++) {
			enchantValue = this->getEnchantValue(i);
		}
		if (enchantValue)
			return true;
		else
			return false;
	}

	float getArmorValueWithEnchants() {
		if (!this->item || !(*this->item)->isArmor())
			return 0;

		return (float)(((*this->item)->getArmorValue() + ((this->getEnchantValue(0) * 1.5f         // Protection
														   + this->getEnchantValue(5) * 0.4f       // Thorns
														   + this->getEnchantValue(3) * 0.4f       // Blast Protection
														   + this->getEnchantValue(1) * 0.4f       // Fire Protection
														   + this->getEnchantValue(4) * 0.4f       // Projectile Protection
														   + this->getItem()->getToughnessValue()  // Prefer Netherite Over Diamond
														   ))));
	}

	float getAttackingDamageWithEnchants() {
		if (!this->item)
			return 0;
		int sharpnessValue = this->getEnchantValue(9);
		return (*this->item)->getAttackDamage() + 1.25f * sharpnessValue;
	}

	void setShowPickUp(bool b) {
		*(bool *)(reinterpret_cast<__int64>(this) + 0x30) = b;
	}

	short getDurability() {
		short max = this->getItem()->getMaxDamage();
		short current = this->getItem()->getDamageValue(*this->tag);
		return max - current;
	}

	short getMaxDurability() {
		short max = this->getItem()->getMaxDamage();
		return max;
	}
	float getArmorPercentDura() {
		if (!this->isValid()) return 0.f;
		float curDur = this->getDurability(), maxDur = this->getMaxDurability();
		return (curDur / maxDur) * 100.f;
	}

	bool isSplashPotionOfInstantHealth(void) {
		return this->isValid() &&
			   (this->getItem()->itemId == 561) &&
			   ((this->aux == 22) || (this->aux == 21));
	}

private:
	inline void setVtable(void);
};

class ArmorItem : public Item {
private:
	char pad_0x108[0x240 - sizeof(Item)];
public:
	int ArmorSlot;  // 0x1B8

	bool isHelmet() {
		return ArmorSlot == 0;
	}
	bool isChestplate() {
		return ArmorSlot == 1;
	}
	bool isLeggings() {
		return ArmorSlot == 2;
	}
	bool isBoots() {
		return ArmorSlot == 3;
	}
};

template <typename T>
class SharedCounter {
public:
	T *value{};
	std::atomic<int32_t> sharedCount{}, weakCount{};
};

template <typename T>
class WeakPtr {
public:
	SharedCounter<T> *counter{};

	inline operator bool() const { return this->counter && this->counter->ptr; }
	inline T *get() const { return this->counter->value; }
};

class ItemRegistry {
public:
	char pad_0x0[0x28];
	std::unordered_map<int32_t, WeakPtr<Item>> idToItemMap;
	std::unordered_map<HashedString, WeakPtr<Item>> nameToItemMap;
	std::unordered_map<HashedString, WeakPtr<Item>> tileNamespaceToItemMap;
	std::unordered_map<HashedString, WeakPtr<Item>> tileItemNameToItemMap;

	Item *getItemFromId(int32_t itemId);
	Item *lookUpByName(std::string_view name);
};

class ItemRegistryManager {
public:
	static ItemRegistry *getItemRegistry();
};

namespace EnchantUtils {

const std::map<std::string, int8_t> IdMap = {
	{"protection",				0},
	{"fire_protection",			1},
	{"feather_falling",			2},
	{"blast_protection",		3},
	{"projectile_protection",	4},
	{"thorns",					5},
	{"respiration",				6},
	{"depth_strider",			7},
	{"aqua_affinity",			8},
	{"sharpness",				9},
	{"smite",					10},
	{"bane_of_arthropods",		11},
	{"knockback",				12},
	{"fire_aspect",				13},
	{"looting",					14},
	{"efficiency",				15},
	{"silk_touch",				16},
	{"unbreaking",				17},
	{"fortune",					18},
	{"power",					19},
	{"punch",					20},
	{"flame",					21},
	{"infinity",				22},
	{"luck_of_the_sea",			23},
	{"lure",					24},
	{"frost_walker",			25},
	{"mending",					26},
	{"curse_of_binding",		27},
	{"curse_of_vanishing",		28},
	{"impaling",				29},
	{"riptide",					30},
	{"loyalty",					31},
	{"channeling",				32},
	{"multishot",				33},
	{"piercing",				34},
	{"quick_charge",			35},
	{"soul_speed",				36},
	{"swift_sneak",				37}
};

int32_t determineActivation(EnchantType enchantType);
bool applyUnfilteredEnchant(ItemStack &out, EnchantmentInstance const &newEnchant, bool overwriteDuplicates);
bool removeEnchant(ItemStack &out, EnchantType enchantTypeToRemove);
void removeAllEnchants(ItemStack &out);
}  // namespace EnchantUtils
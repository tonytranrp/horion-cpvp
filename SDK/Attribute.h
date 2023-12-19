#pragma once

#include <cstdint>
#include "HashedString.h"

// https://github.com/bdsx/bdsx/blob/cda828e91a573e331952389113483fa17b21eaa1/bdsx/bds/attribute.ts#L7
enum class AttributeID : uint64_t {
	PlayerHunger = 4294967552,
	PlayerSaturation = 8589934848,
	PlayerLevel = 17179869440,
	PlayerExperience = 21474836736,
	Health = 25769804032,
	FollowRange = 30064771073,
	KnockbackResistance = 34359738369,
	MovementSpeed = 38654705921,
	UnderwaterMovementSpeed = 42949673217,
	LavaMovementSpeed = 47244640513,
	AttackDamage = 51539607553,
	Absorption = 55834575105,
	Luck = 60129542401,
	JumpStrength = 64424509697,
};

class Attribute {
	uint64_t hash;
	HashedString name;

private:
	Attribute() = delete;
	Attribute(AttributeID id) {
		memset(this, 0x0, sizeof(*this));
		this->hash = static_cast<uint64_t>(id);
	}
	friend class Entity;
};
static_assert(sizeof(Attribute) == 0x38);

class AttributeInstance {
private:
	char filler[0x74];

public:
	float minimumValue;
	float maximumValue;
	float currentValue;

	~AttributeInstance();
	virtual void tick(void);
};
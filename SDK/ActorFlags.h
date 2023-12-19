#pragma once

#include <cstdint>

// https://github.com/pmmp/BedrockProtocol/blob/master/src/types/entity/EntityMetadataFlags.php#L22
enum class ActorFlags : int32_t {
	ONFIRE = 0,
	SNEAKING = 1,
	RIDING = 2,
	SPRINTING = 3,
	ACTION = 4,
	INVISIBLE = 5,
	TEMPTED = 6,
	INLOVE = 7,
	SADDLED = 8,
	POWERED = 9,
	IGNITED = 10,
	BABY = 11,
	CONVERTING = 12,
	CRITICAL = 13,
	CAN_SHOW_NAMETAG = 14,
	ALWAYS_SHOW_NAMETAG = 15,
	IMMOBILE = 16, NO_AI = 16,
	SILENT = 17,
	WALLCLIMBING = 18,
	CAN_CLIMB = 19,
	SWIMMER = 20,
	CAN_FLY = 21,
	WALKER = 22,
	RESTING = 23,
	SITTING = 24,
	ANGRY = 25,
	INTERESTED = 26,
	CHARGED = 27,
	TAMED = 28,
	ORPHANED = 29,
	LEASHED = 30,
	SHEARED = 31,
	GLIDING = 32,
	ELDER = 33,
	MOVING = 34,
	BREATHING = 35,
	CHESTED = 36,
	STACKABLE = 37,
	SHOWBASE = 38,
	REARING = 39,
	VIBRATING = 40,
	IDLING = 41,
	EVOKER_SPELL = 42,
	CHARGE_ATTACK = 43,
	WASD_CONTROLLED = 44,
	CAN_POWER_JUMP = 45,
	CAN_DASH = 46,
	LINGER = 47,
	HAS_COLLISION = 48,
	AFFECTED_BY_GRAVITY = 49,
	FIRE_IMMUNE = 50,
	DANCING = 51,
	ENCHANTED = 52,
	SHOW_TRIDENT_ROPE = 53,
	CONTAINER_PRIVATE = 54,
	TRANSFORMING = 55,
	SPIN_ATTACK = 56,
	SWIMMING = 57,
	BRIBED = 58,
	PREGNANT = 59,
	LAYING_EGG = 60,
	RIDER_CAN_PICK = 61,
	TRANSITION_SITTING = 62,
	EATING = 63,
	LAYING_DOWN = 64,
	SNEEZING = 65,
	TRUSTING = 66,
	ROLLING = 67,
	SCARED = 68,
	IN_SCAFFOLDING = 69,
	OVER_SCAFFOLDING = 70,
	FALL_THROUGH_SCAFFOLDING = 71,
	BLOCKING = 72,
	TRANSITION_BLOCKING = 73,
	BLOCKED_USING_SHIELD = 74,
	BLOCKED_USING_DAMAGED_SHIELD = 75,
	SLEEPING = 76,
	WANTS_TO_WAKE = 77,
	TRADE_INTEREST = 78,
	DOOR_BREAKER = 79,
	BREAKING_OBSTRUCTION = 80,
	DOOR_OPENER = 81,
	ILLAGER_CAPTAIN = 82,
	STUNNED = 83,
	ROARING = 84,
	DELAYED_ATTACKING = 85,
	AVOIDING_MOBS = 86,
	AVOIDING_BLOCK = 87,
	FACING_TARGET_TO_RANGE_ATTACK = 88,
	HIDDEN_WHEN_INVISIBLE = 89,
	IS_IN_UI = 90,
	STALKING = 91,
	EMOTING = 92,
	CELEBRATING = 93,
	ADMIRING = 94,
	CELEBRATING_SPECIAL = 95,
	OUT_OF_CONTROL = 96,
	RAM_ATTACK = 97,
	PLAYING_DEAD = 98,
	IN_ASCENDABLE_BLOCK = 99,
	OVER_DESCENDABLE_BLOCK = 100,
	CROAKING = 101,
	EAT_MOB = 102,
	JUMP_GOAL_JUMP = 103,
	EMERGING = 104,
	SNIFFING = 105,
	DIGGING = 106,
	SONIC_BOOM = 107,
	HAS_DASH_COOLDOWN = 108,
	PUSH_TOWARDS_CLOSEST_SPACE = 109,
};
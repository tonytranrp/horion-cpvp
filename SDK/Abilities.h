#pragma once

#include <cstdint>

enum class AbilitiesIndex : int16_t {
	Build = 0,
	Mine = 1,
	DoorsAndSwitches = 2,
	OpenContainers = 3,
	AttackPlayers = 4,
	AttackMobs = 5,
	OperatorCommands = 6,
	Teleport = 7,
	Invulnerable = 8,
	Flying = 9,
	MayFly = 10,
	Instabuild = 11,
	Lightning = 12,
	FlySpeed = 13,
	WalkSpeed = 14,
	Muted = 15,
	WorldBuilder = 16,
	NoClip = 17,
	COUNT = 18,
};

enum class CommandPermissionLevel : int8_t {  // 1.14.60 linux
	Any = 0,
	GameMasters = 1,
	Admin = 2,
	Host = 3,
	Owner = 4,
	Internal = 5,
};

enum class PlayerPermissionLevel : int8_t {  // 1.14.60 linux
	Visitor = 0,
	Member = 1,
	Operator = 2,
	Custom = 3,
};

class Ability {
public:
	enum class Type : int8_t {
		Invalid = 0,
		Unset = 1,
		BooleanType = 2,
		FloatType = 3,
	};

	enum class Options : int8_t {
		None = 0,
		NoSave = 1,
		CommandExposed = 2,
		PermissionsInterfaceExposed = 4,
		WorldbuilderOverrides = 8,
	};

	union Value {
		bool boolVal;
		float floatVal;
	};

	Type type;
	Value value;
	Options option;
};

// https://github.com/Sandertv/gophertunnel/blob/master/minecraft/protocol/ability.go
class LayeredAbilities {
public:

	enum class LayerType {
		CUSTOM_CACHE = 0,
		BASE = 1,
		SPECTATOR = 2,
		COMMANDS = 3,
		EDITOR = 4,
		COUNT = 5,
	};

	using ability_list_t = std::array<Ability, static_cast<size_t>(AbilitiesIndex::COUNT)>;
	using layer_t = std::array<ability_list_t, static_cast<size_t>(LayerType::COUNT)>;

	CommandPermissionLevel commandPerms;
	PlayerPermissionLevel playerPerms;
	layer_t abilitiesList;

	template <typename T>
	T getAbility(AbilitiesIndex idx) const = delete;

	template <>
	bool getAbility<bool>(AbilitiesIndex idx) const {
		const auto& abil = this->abilitiesList[(size_t)LayerType::BASE][(size_t)idx];
		if (abil.type == Ability::Type::BooleanType) {
			return static_cast<bool>(abil.value.boolVal);
		}
		return false;
	}

	template <>
	float getAbility<float>(AbilitiesIndex idx) const {
		const auto& abil = this->abilitiesList[(size_t)LayerType::BASE][(size_t)idx];
		if (abil.type == Ability::Type::FloatType) {
			return static_cast<float>(abil.value.floatVal);
		}
		return 0.f;
	}

	inline void setAbility(AbilitiesIndex idx, bool flag) {
		/*auto& abil = this->abilitiesList[(size_t)LayerType::BASE][(size_t)idx];
		if (abil.type == Ability::Type::BooleanType) {
			abil.value.boolVal = flag;
		}*/
		for (auto& abil : this->abilitiesList) {
			if (abil[(size_t)idx].type == Ability::Type::BooleanType) {
				abil[(size_t)idx].value.boolVal = flag;
			}
		}
	}

	inline void setAbility(AbilitiesIndex idx, float val) {
		/*auto& abil = this->abilitiesList[(size_t)LayerType::BASE][(size_t)idx];
		if (abil.type == Ability::Type::FloatType) {
			abil.value.floatVal = val;
		}*/
		for (auto& abil : this->abilitiesList) {
			if (abil[(size_t)idx].type == Ability::Type::FloatType) {
				abil[(size_t)idx].value.floatVal = val;
			}
		}
	}
};
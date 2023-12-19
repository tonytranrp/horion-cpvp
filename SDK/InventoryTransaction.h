#pragma once
#include "../Utils/HMath.h"
#include "Item.h"
#include "Inventory.h"

enum class ContainerID {
	Invalid = -1,
	Inventory,
	First,
	Last = 100,
	Offhand = 119,
	Armor,
	Creative,
	Hotbar,
	FixedInventory,
	UI,
	None = 0xFF,
};

class InventorySource {
public:
	enum class Type {
		Invalid = -1,
		Container,
		Global,
		WorldInteraction,
		Creative,
		UntrackedInteractionUI = 100,
		NotImplemented = 99999
	};

	enum class Flags {
		NoFlag = 0,
		WorldInteraction_Random = 1
	};

	Type type = Type::Invalid;
	ContainerID container = ContainerID::Invalid;
	Flags flags = Flags::NoFlag;


	InventorySource() = default;
	InventorySource(ContainerID id) : container(id) {}
	InventorySource(Type type) : type(type) {}
	InventorySource(Type type, Flags flags) : type(type), flags(flags) {}
	InventorySource(Type type, ContainerID id) : type(type), container(id) {}

	inline bool operator==(InventorySource const& rhs) const {
		return ((this->type == rhs.type) &&
				(this->container == rhs.container) &&
				(this->flags == rhs.flags));
	}
	inline bool operator!=(InventorySource const& rhs) const {
		return !(*this == rhs);
	}
};

namespace std {
template <>
struct hash<InventorySource> {
	constexpr uint64_t operator()(InventorySource const& source) const noexcept {
		return (uint64_t)source.type << 16 ^ (uint64_t)source.container;
	}
};
}  // namespace std

class InventoryAction {
public:
	InventoryAction() = default;

	InventoryAction(uint32_t slot, ItemStack* sourceItem, ItemStack* targetItem, ContainerID containerId = ContainerID::Inventory, InventorySource::Type sourceType = InventorySource::Type::Container, InventorySource::Flags flags = InventorySource::Flags::NoFlag) {
		memset(this, 0x0, sizeof(InventoryAction));

		this->slot = slot;
		if (sourceItem) this->sourceItem = *sourceItem;
		if (targetItem) this->targetItem = *targetItem;
		this->sourceItemDescriptor.fromStack(&this->sourceItem);
		this->targetItemDescriptor.fromStack(&this->targetItem);
		this->source.type = sourceType;
		this->source.container = containerId;
		this->source.flags = flags;
	}

public:
	InventorySource source;								// 0x0
	uint32_t slot;										// 0xC
	NetworkItemStackDescriptor sourceItemDescriptor;	// 0x10
	NetworkItemStackDescriptor targetItemDescriptor;	// 0x68
	ItemStack sourceItem;								// 0xC0
	ItemStack targetItem;								// 0x160
};

class InventoryTransactionItemGroup {
public:
	int32_t itemId;                    // 0x0
	int32_t itemAux;                   // 0x4
	int64_t filler;
	//std::unique_ptr<CompoundTag> tag;  // 0x8  someone help thx
	int32_t count;                     // 0x10
	bool overflow;                     // 0x14
};

class InventoryTransaction {
public:
	std::unordered_map<InventorySource, std::vector<InventoryAction>> actions;  // 0x0
	std::vector<InventoryTransactionItemGroup> items;							// 0x40

	void addAction(InventoryAction const & action);
};

class InventoryTransactionManager {
public:
	Player** player;									// 0x0
	std::unique_ptr<InventoryTransaction> transaction;	// 0x8

	void addAction(InventoryAction const & action , bool balanced = false);
};
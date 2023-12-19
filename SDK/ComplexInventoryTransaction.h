#pragma once
#include "../Utils/HMath.h"
#include "Inventory.h"
#include "InventoryTransaction.h"
//#include "BinaryStream.h"

class ComplexInventoryTransaction {
	uintptr_t** vTable;

public:
	enum class Type : int32_t {
		Normal = 0,
		MisMatch = 1,
		ItemUse = 2,
		ItemUseOnActor = 3,
		ReleaseItem = 4,
	};

	Type type{};
	InventoryTransaction data{};

	//void read(ReadOnlyBinaryStream&);
	//void write(BinaryStream*);
};

class ItemUseInventoryTransaction : public ComplexInventoryTransaction {
public:
	enum class ActionType : int32_t {
		Place = 0,
		Use = 1,
		Destroy = 2,
	};

	ActionType actionType{};
	Vec3i blockPos{};
	uint32_t targetBlockRuntimeId{};
	uint8_t face{};
	int32_t slot{};
	NetworkItemStackDescriptor itemInHand{};
	Vec3 playerPos{}, clickPos{};

	ItemUseInventoryTransaction();
	ItemUseInventoryTransaction(ActionType action, Vec3i blockPos, uint32_t blockRid, uint8_t blockFace, int32_t slot, ItemStack* item, Vec3 pos, Vec3 clickPos = {0.f, 0.f, 0.f}) : ItemUseInventoryTransaction() {
		this->actionType = action;
		this->blockPos = blockPos;
		this->targetBlockRuntimeId = blockRid;
		this->face = blockFace;
		this->slot = slot;
		this->itemInHand.fromStack(item);
		this->playerPos = pos;
		this->clickPos = clickPos;
	}
	ItemUseInventoryTransaction(ActionType action, int32_t slot, ItemStack* item, Vec3 pos, Vec3 clickPos = {0.f, 0.f, 0.f}) : ItemUseInventoryTransaction() {
		this->actionType = action;
		this->blockPos = {0, 0, 0};
		this->targetBlockRuntimeId = 0;
		this->face = 0;
		this->slot = slot;
		this->itemInHand.fromStack(item);
		this->playerPos = pos;
		this->clickPos = clickPos;
	}

};

class ItemUseOnActorInventoryTransaction : public ComplexInventoryTransaction {
public:
	enum class ActionType : int32_t {
		Interact = 0,
		Attack = 1,
		ItemInteract = 2,
	};

	ActorRuntimeID actorId{};
	ActionType actionType{};
	int32_t slot{};
	NetworkItemStackDescriptor itemInHand{};
	Vec3 playerPos{}, clickPos{};

	ItemUseOnActorInventoryTransaction();
	ItemUseOnActorInventoryTransaction(ActionType action, ActorRuntimeID rid, int32_t slot, ItemStack* item, Vec3 pos, Vec3 clickPos = {0.f, 0.f, 0.f}) : ItemUseOnActorInventoryTransaction() {
		this->actorId = rid;
		this->actionType = action;
		this->slot = slot;
		this->itemInHand.fromStack(item);
		this->playerPos = pos;
		this->clickPos = clickPos;
	}
};

class ItemReleaseInventoryTransaction : public ComplexInventoryTransaction {
public:
	enum class ActionType : int32_t {
		Release = 0,
		Use = 1,
	};

	ActionType actionType{};
	int32_t slot{};
	NetworkItemStackDescriptor itemInHand{};
	Vec3 headPos{};

	ItemReleaseInventoryTransaction();
	ItemReleaseInventoryTransaction(ActionType action, int32_t slot, ItemStack* item, Vec3 headPos) : ItemReleaseInventoryTransaction() {
		this->actionType = action;
		this->slot = slot;
		this->itemInHand.fromStack(item);
		this->headPos = headPos;
	}
};
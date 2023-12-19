#pragma once

#include "../Utils/HMath.h"
#include "Entity.h"
#include "ComplexInventoryTransaction.h"
#include "Tag.h"
#include "PacketIDs.h"
#include "BinaryStream.h"
#include "mce.h"

class ConnectionRequest;
enum class LevelSoundEvent : int32_t;

struct PacketHandlerDispatcherInstance {
	uintptr_t** vTable;
};

union PacketHeader {
	struct {
		uint32_t packetId : 10;
		uint32_t recipientSubId : 2;
		uint32_t senderSubId : 2;
	};
	uint32_t packed;
};
static_assert(sizeof(PacketHeader) == 0x4);

class Packet {
public:
	uintptr_t** vTable;
	int32_t priority = 2, reliability = 1;
private:
	char pad_0x10[0x10];
public:
	PacketHandlerDispatcherInstance* dispatcher;
private:
	char pad_0x28[0x8];

public:
	std::string getName();

	PacketID getId();

	void read(ReadOnlyBinaryStream&);

	void write(BinaryStream*);

	static uintptr_t** GetPacketVTable(PacketID id);

	static std::shared_ptr<Packet> CreatePacket(PacketID id);
};

class LevelSoundEventPacket : public Packet {
public:
	LevelSoundEvent sound;
	Vec3 pos;
	int32_t extraData;
	std::string entityType;
	bool isBabyMob, isGlobal;

	LevelSoundEventPacket() : sound((LevelSoundEvent)0), pos({0.f, 0.f, 0.f}), extraData(-1), isBabyMob(false), isGlobal(false) {
		static auto vtable = GetPacketVTable(PacketID::LevelSoundEvent);
		this->vTable = vtable;
	}
	LevelSoundEventPacket(LevelSoundEvent sound, const Vec3& pos, int32_t extraData,
		const std::string& entityType, bool isBabyMob, bool isGlobal) : LevelSoundEventPacket() {
		this->sound = sound;
		this->pos = pos;
		this->extraData = extraData;
		this->entityType = entityType;
		this->isBabyMob = isBabyMob;
		this->isGlobal = isGlobal;
	}
};

class PlayerActionPacket : public Packet {
public:
	enum class Action {
		StartBreakBlock = 0,
		AbortBreakBlock,
		StopBreakBlock,
		GetUpdatedBlock,
		DropItem,
		StartSleeping,
		StopSleeping,
		Respawn,
		Jump,
		StartSprint,
		StopSprint,
		StartSneak,
		StopSneak,
		CreativePlayerDestroyBlock,
		DimensionChangeDone,
		StartGlide,
		StopGlide,
		BuildDenied,
		CrackBlock, // ???
		ChangeSkin,
		SetEnchantmentSeed,
		StartSwimming,
		StopSwimming,
		StartSpinAttack,
		StopSpinAttack,
		StartBuildingBlock,
		PredictDestroyBlock,
		ContinueDestroyBlock,
		StartItemUseOn,
		StopItemUseOn,
	};

	PlayerActionPacket() {
		static auto vtable = GetPacketVTable(PacketID::PlayerAction);
		this->vTable = vtable;
	}

	Vec3i blockPosition;
	Vec3i resultPosition;
	int32_t face;
	Action action;
	ActorRuntimeID actorRuntimeId;
};

class ActorEventPacket : public Packet {
public:
	enum class ActorEvent : int8_t {
		None = 0,
		Jump = 1,
		Hurt = 2,
		Death = 3,
		StartAttacking = 4,
		StopAttacking = 5,
		TamingFailed = 6,
		TamingSucceeded = 7,
		ShakeWetness = 8,
		EatGrass = 10,
		FishHookBubble = 11,
		FishHookFishpos = 12,
		FishHookHooktime = 13,
		FishHookTease = 14,
		SquidFleeing = 15,
		ZombieConverting = 16,
		PlayAmbientSound = 17,
		SpawnAlive = 18,
		StartOfferFlower = 19,
		StopOfferFlower = 20,
		LoveHearts = 21,
		VillagerAngry = 22,
		VillagerHappy = 23,
		WitchHatMagic = 24,
		FireworksExplode = 25,
		InLoveHearts = 26,
		SilverfishMergeAnim = 27,
		GuardianAttackSound = 27,
		DrinkPotion = 29,
		ThrowPotion = 30,
		PrimeTntcart = 31,
		PrimeCreeper = 32,
		AirSupply = 33,
		AddPlayerLevels = 34,
		GuardianMiningFatigue = 35,
		Agentswing = 36,
		DragonStartDeathAnim = 37,
		GroundDust = 38,
		Shake = 39,
		Feed = 57,
		BabyEat = 60,
		InstantDeath = 61,
		NotifyTrade = 62,
		LeashDestroyed = 63,
		CaravanUpdated = 64,
		TalismanActivate = 65,
		UpdateStructureFeature = 66,
		PlayerSpawnedMob = 67,
		DragonPuke = 68,
		UpdateStackSize = 69,
		StartSwimming = 70,
		BalloonPop = 71,
		TreasureHunt = 72,
		SummonAgent = 73,
		FinishedChargingCrossbow = 74,
		LandedOnGround = 75,
		GrowUp = 76,
	};

	ActorRuntimeID actorRuntimeId;
	ActorEvent eventId;
	int32_t data;

	ActorEventPacket() {
		static auto vtable = GetPacketVTable(PacketID::ActorEvent);
		this->vTable = vtable;
	}

	ActorEventPacket(const ActorRuntimeID& actorRuntimeId, ActorEvent eventId, int32_t data) : ActorEventPacket() {
		this->actorRuntimeId = actorRuntimeId;
		this->eventId = eventId;
		this->data = data;
	}
};

class InteractPacket : public Packet {
public:
	enum class Action : int8_t {
		StopRiding = 3,
		InteractUpdate = 4,
		NpcOpen = 5,
		OpenInventory = 6,
	};

	InteractPacket() {
		static auto vtable = GetPacketVTable(PacketID::Interact);
		this->vTable = vtable;
	}
	InteractPacket(Action action, ActorRuntimeID actorRuntimeId, Vec3 pos) : InteractPacket() {
		this->action = action;
		this->actorRuntimeId = actorRuntimeId;
		this->pos = pos;
	}

	Action action;
	ActorRuntimeID actorRuntimeId;
	Vec3 pos;
};

class MobEquipmentPacket : public Packet {
public:
	MobEquipmentPacket() {
		static auto vtable = GetPacketVTable(PacketID::MobEquipment);
		this->vTable = vtable;
	}

	ActorRuntimeID actorRuntimeId;
	ItemStack item;
	int32_t inventorySlot;
	int32_t hotbarSlot;
	ContainerID containerId;
};

class InventoryTransactionPacket : public Packet {
public:
	InventoryTransactionPacket() {
		static auto vtable = GetPacketVTable(PacketID::InventoryTransaction);
		this->vTable = vtable;
	}
	InventoryTransactionPacket(ComplexInventoryTransaction* transaction, bool isClientSide = false) : isClientSide(isClientSide) , transaction(transaction) {
		static auto vtable = GetPacketVTable(PacketID::InventoryTransaction);
		this->vTable = vtable;
	}

	char pad[0x28];
	ComplexInventoryTransaction* transaction;
	bool isClientSide = false;
};

class TextPacket : public Packet {
public:
	enum class Type : int8_t {
		Raw = 0,
		Chat = 1,
		Translate = 2,
		Popup = 3,
		JukeboxPopup = 4,
		Tip = 5,
		SystemMessage = 6,
		Whisper = 7,
		Announcement = 8,
		TextObjectWhisper = 9,
		TextObject = 10,
	};

	TextPacket() {
		static auto vtable = GetPacketVTable(PacketID::Text);
		this->vTable = vtable;
	}
	TextPacket(LocalPlayer* player, std::string message, Type messageType = Type::Chat) : TextPacket() {
		this->messageType = messageType;
		this->sourceName = player->getNameTag().c_str();
		this->message = message;
		this->xboxUserId = std::to_string(player->getUserId());
	}

	Type messageType;
	std::string sourceName;
	std::string message;
private:
	char pad_2[0x18];
public:
	bool translationNeeded = false;
	std::string xboxUserId;
	std::string platformChatId;
};

class MovePlayerPacket : public Packet {
public:
	MovePlayerPacket() {
		static auto vtable = GetPacketVTable(PacketID::MovePlayer);
		this->vTable = vtable;
	}

	MovePlayerPacket(LocalPlayer* player, const Vec3& pos) : MovePlayerPacket() {
		this->actorRuntimeId = player->getRuntimeId();
		this->Position = pos;
		this->pitch = player->viewAnglesPtr->viewAngles.x;
		this->yaw = player->viewAnglesPtr->viewAngles.y;
		this->headYaw = player->viewAnglesPtr->viewAngles.y;
		this->mode = 0;
		this->onGround = player->isOnGround();
		this->ridingActorRuntimeId = {0};
		this->teleportCause = 0;
		this->teleportSourceEntityType = 0;
		this->clientTick = 0;  // ???
	}

	ActorRuntimeID actorRuntimeId;
	Vec3 Position;
	float pitch, yaw, headYaw;
	uint8_t mode;
	bool onGround;
	ActorRuntimeID ridingActorRuntimeId;
	int32_t teleportCause, teleportSourceEntityType;
	uint64_t clientTick;
};

class PlayerAuthInputPacket : public Packet {
public:

	enum class InputFlag { // https://github.com/Sandertv/gophertunnel/blob/master/minecraft/protocol/packet/player_auth_input.go
		Ascend,
		Descend,
		NorthJump,
		JumpDown,
		SprintDown,
		ChangeHeight,
		Jumping,
		AutoJumpingInWater,
		Sneaking,
		SneakDown,
		Up,
		Down,
		Left,
		Right,
		UpLeft,
		UpRight,
		WantUp,
		WantDown,
		WantDownSlow,
		WantUpSlow,
		Sprinting,
		AscendBlock,
		DescendBlock,
		SneakToggleDown,
		PersistSneak,
		StartSprinting,
		StopSprinting,
		StartSneaking,
		StopSneaking,
		StartSwimming,
		StopSwimming,
		StartJumping,
		StartGliding,
		StopGliding,
		PerformItemInteraction,
		PerformBlockActions,
		PerformItemStackRequest,
		COUNT,
	};

	struct PlayerBlockAction { // https://github.com/Sandertv/gophertunnel/blob/12a543ec80e6a3f06132a03251e3ea0186505f70/minecraft/protocol/player.go#L122
		PlayerActionPacket::Action action;
		Vec3i pos;
		int32_t blockFace;
	};

	Vec2 rot;
	Vec3 pos;
	float headYaw;
	Vec3 posDelta;
	Vec2 moveVector;
	Vec3 lookDirectionVR;
	std::bitset<(size_t)InputFlag::COUNT> flags;
private:
	uint8_t unk1[0xC]{}; // probably InputMode, PlayMode, InteractionModel
public:
	uint64_t clientTick;
	void *itemInteractionData = nullptr, *itemStackRequestData = nullptr;
	std::vector<PlayerBlockAction> blockActions;

	PlayerAuthInputPacket() {
		static auto vtable = GetPacketVTable(PacketID::PlayerAuthInput);
		this->vTable = vtable;
	}

	inline bool hasFlag(InputFlag flag) const {
		return this->flags.test((size_t)flag);
	}
	inline void setFlag(InputFlag flag, bool toggle = true) {
		this->flags.set((size_t)flag, toggle);
	}
};
static_assert(sizeof(PlayerAuthInputPacket) == 0xB0);

class CommandRequestPacket : public Packet {
public:
	CommandRequestPacket() {
		static auto vtable = GetPacketVTable(PacketID::CommandRequest);
		this->vTable = vtable;
	}
	CommandRequestPacket(std::string command) : CommandRequestPacket() {
		this->command = command;
	}

	std::string command;

private:
	char filler[0x41];
};

class NetworkStackLatencyPacket : public Packet {
public:
	NetworkStackLatencyPacket() {
		static auto vtable = GetPacketVTable(PacketID::NetworkStackLatency);
		this->vTable = vtable;
	}

	int timeStamp;
	bool needResponse;
};

class BlockActorDataPacket : public Packet {
public:
	BlockActorDataPacket() {
		static auto vtable = GetPacketVTable(PacketID::BlockActorData);
		this->vTable = vtable;
	}
	BlockActorDataPacket(Vec3i blockPos, CompoundTag tag) : BlockActorDataPacket() {
		this->blockPos = blockPos;
		this->tag = tag;
	}

	Vec3i blockPos;
	CompoundTag tag;
};

class DisconnectPacket : public Packet {
public:
	DisconnectPacket() {
		static auto vtable = GetPacketVTable(PacketID::Disconnect);
		this->vTable = vtable;
	}
	DisconnectPacket(bool skipMessage, std::string disconnectMsg) : DisconnectPacket() {
		this->skipMessage = skipMessage;
		this->disconnectMsg = disconnectMsg;
	}

	bool skipMessage;
	std::string disconnectMsg;
};

class RespawnPacket : public Packet {
public:
	enum class RespawnState : int8_t {
		SearchingForSpawn = 0,
		ReadToSpawn = 1,
		ClientReadyToSpawn = 2,
	};

	RespawnPacket() {
		static auto vtable = GetPacketVTable(PacketID::Respawn);
		this->vTable = vtable;
	}
	RespawnPacket(Vec3 spawnPos, RespawnState respawnState, ActorRuntimeID actorRuntimeId) : RespawnPacket() {
		this->spawnPos = spawnPos;
		this->respawnState = respawnState;
		this->actorRuntimeId = actorRuntimeId;
	}

	Vec3 spawnPos;
	RespawnState respawnState;
	ActorRuntimeID actorRuntimeId;
};

class LoginPacket : public Packet {
public:
	int32_t protocolVersion;
	std::unique_ptr<ConnectionRequest> req;
};

class EmoteListPacket : public Packet {
public:
	ActorRuntimeID runtimeId;
	std::vector<mce::UUID> emoteUuids;

	EmoteListPacket() {
		static auto vtable = GetPacketVTable(PacketID::EmoteList);
		this->vTable = vtable;
	}
	EmoteListPacket(ActorRuntimeID runtimeId, const std::vector<mce::UUID>& emoteUuids) : EmoteListPacket() {
		this->runtimeId = runtimeId;
		this->emoteUuids = emoteUuids;
	}
};

class EmotePacket : public Packet {
public:
	ActorRuntimeID runtimeId;
	std::string prettifiedEmoteUuid;
	uint8_t flags;

	EmotePacket() {
		static auto vtable = GetPacketVTable(PacketID::Emote);
		this->vTable = vtable;
	}
	EmotePacket(ActorRuntimeID runtimeId, const std::string& prettifiedEmoteUuid) : EmotePacket() {
		this->runtimeId = runtimeId;
		this->prettifiedEmoteUuid = prettifiedEmoteUuid;
		this->flags = 0x0;
	}
};

class PlayerSkinPacket : public Packet {
public:
	mce::UUID playerUuid;
	SerializedSkin skin;
	std::string newSkinNameDeprecated, oldSkinNameDeprecated;

	PlayerSkinPacket() {
		static auto vtable = GetPacketVTable(PacketID::PlayerSkin);
		this->vTable = vtable;
	}
	PlayerSkinPacket(const mce::UUID uuid, const SerializedSkin &skin,
		const std::string newSkinName = {}, const std::string oldSkinName = {}) : PlayerSkinPacket() {
		this->playerUuid = uuid;
		this->skin = skin;
		this->newSkinNameDeprecated = newSkinName;
		this->oldSkinNameDeprecated = oldSkinName;
	}
};

class BinaryPacket {
public:
	template <typename T>
	static std::shared_ptr<Packet> Create(PacketID id, T& writeFunc, bool skipRead = false) {
		auto packet = Packet::CreatePacket(id);
		BinaryStream bs;
		writeFunc(bs);
		if (!skipRead) {
			packet->read(bs);
		}
		return packet;
	}

	template <typename T>
	static void CreateAndSend(PacketID id, T& writeFunc) {
		auto packet = BinaryPacket::Create(id, writeFunc);
		Game.getLoopbackPacketSender()->sendToServer(packet.get());
	}
};
#include "GameMode.h"
#include "../Memory/GameData.h"

void GameMode::survivalDestroyBlockHack(Vec3i const &block, int face, bool &isDestroyedOut, bool isFirst) {
	auto p = this->player;
	p->swing();
	if (isFirst)
		this->startDestroyBlock(block, face, isDestroyedOut);
	else {
		*reinterpret_cast<bool *>(reinterpret_cast<__int64>(p) + 0x1c5a) = 1;
		this->continueDestroyBlock(block, face, isDestroyedOut);
	}

	//p->startDestroying();

	// prevents regular code from aborting block-break
	*reinterpret_cast<bool *>(reinterpret_cast<__int64>(p) + 0x1c5a) = 0;

	PlayerActionPacket action;
	action.action = PlayerActionPacket::Action::CrackBlock;
	action.blockPosition = Vec3i(block.x, block.y, block.z);
	action.face = face;
	action.actorRuntimeId = p->getRuntimeId();
	Game.getLoopbackPacketSender()->sendToServer(&action);
}

void GameMode::baseUseItem(ItemStack & a1) {
	using GM_baseUseItem_t = void (*)(GameMode *, ItemStack &);
	static GM_baseUseItem_t baseUseItem = reinterpret_cast<GM_baseUseItem_t>(FindSignature("48 89 5C 24 ? 48 89 74 24 ? 55 57 41 56 48 8D 6C 24 ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 45 ? 48 8B F2 48 8B F9 45 33 F6 4C 89 75 ? E8 ? ? ? ? 4C 8B 00 BA ? ? ? ? 48 8B C8"));  //"ItemUseInventoryTransaction::handle - destroy block failed, rending blocks around area to player"

	return baseUseItem(this, a1);
}

#include "AutoTool.h"
AutoTool::AutoTool() : IModule(0, Category::PLAYER, "Automatically Equips Best Tool") {
}

AutoTool::~AutoTool() {
}

const char* AutoTool::getModuleName() {
	return "AutoTool";
}

int AutoTool::getBestDestoryItemSlot(Block* block) {

	if (block->blockLegacy->material->isSolid) {
		auto supplies = Game.getLocalPlayer()->getSupplies();
		auto inv = supplies->inventory;
		float cachedSpeed = 0.f;
		int bestSlot = 0;

		for (int i = 0; i < 9 * 4; i++) {
			auto item = inv->getItemStack(i);
			if (!item->isValid()) continue;
			float curSpeed = item->getDestroySpeed(block);
			if (curSpeed > cachedSpeed) {
				cachedSpeed = curSpeed;
				bestSlot = i;
			}
		}
		return bestSlot;
	}
	return -1;
}

void AutoTool::onTick(GameMode* c) {
	auto player = Game.getLocalPlayer();
	if (!player || !Game.canUseMoveKeys() || player->isCreative()) return;
	auto level = Game.getLevel();
	auto supplies = player->getSupplies();
	auto inv = supplies->inventory;

	//static auto AutoMineMod = moduleMgr->getModule<AutoMine>("AutoMine");

	const auto& hitResult = level->getHitResult();
	if (hitResult.type == HitResultType::Tile && Game.isLeftClickDown()) {
		int bestSlot = getBestDestoryItemSlot(Game.getRegion()->getBlock(hitResult.block));

		if (bestSlot != supplies->selectedHotbarSlot && bestSlot > -1) {
			if (bestSlot > 8) {
				int emptySlot = inv->getFirstEmptySlot();
				if (emptySlot > 8) {
					inv->swapSlots(bestSlot, supplies->selectedHotbarSlot);
				} else {
					inv->swapSlots(bestSlot, emptySlot);
				}
			}
			supplies->selectedHotbarSlot = bestSlot;
		}
	}
}
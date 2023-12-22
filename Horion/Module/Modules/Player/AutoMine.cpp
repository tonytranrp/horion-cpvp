#include "AutoMine.h"

AutoMine::AutoMine() : IModule(0, Category::PLAYER, "Automatically mine the block you are facing!") {
}

AutoMine::~AutoMine() {
}

const char* AutoMine::getModuleName() {
	return "AutoMine";
}
bool isDestroying = false;

void AutoMine::onTick(GameMode* gm) {
	if (!Game.getLocalPlayer()) return;
	
	if (Game.getLocalPlayer()->isSurvival()) {
		auto level = Game.getLevel();
		const auto& hitResult = level->getHitResult();
		Vec3i BlockPos = hitResult.startPos;
		int face = static_cast<int32_t>(hitResult.blockFace);

		Block* block = Game.getRegion()->getBlock(hitResult.block);
		int blockId = block->toLegacy()->blockId;

		if (hitResult.type == HitResultType::Tile && (blockId != 7 && blockId != 210 && blockId != 211 && blockId != 217 && blockId != 416)) {  // looking at a block
			isDestroying = true;
			//AutoMineBlocks(InputCallback, &BlockPos, face, isDestroying);
		} else if (isDestroying) {
			isDestroying = false;
			//AutoMineBlocks(InputCallback, &BlockPos, face, isDestroying);
		}
	}
}

void AutoMine::onDisable() {
	if (!Game.getLocalPlayer()) return;
	InputCallback->handleBuildOrDestroyActionButtonRelease();
}
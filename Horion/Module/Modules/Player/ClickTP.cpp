#include "ClickTP.h"

ClickTP::ClickTP() : IModule(0, Category::PLAYER, "Click a block to teleport to it") {
	registerBoolSetting("Only Hand", &this->onlyHand, this->onlyHand);
	registerBoolSetting("Push", &this->bypass, this->bypass);
}

ClickTP::~ClickTP() {
}

const char* ClickTP::getModuleName() {
	return "ClickTP";
}

std::string ClickTP::getModeText() {
	if (bypass)
		return "Push";
	else
		return "Normal";
}

void ClickTP::onTick(GameMode* gm) {
	if (!GameData::canUseMoveKeys())
		return;
	if (onlyHand && Game.getInventory()->getItemStack(Game.getSupplies()->selectedHotbarSlot)->item)
		return;

	if (GameData::isRightClickDown() && !hasClicked) {
		hasClicked = true;
		auto ps = Game.getLevel();
		Vec3i block = ps->getHitResult().block;
		if (block == Vec3i(0, 0, 0)) return;
		Vec3 pos = block.toFloatVector();
		pos.x += 0.5f;
		pos.z += 0.5f;

		tpPos = pos;
		shouldTP = true;

		Game.getGuiData()->displayClientMessageF("%sTeleport position set to %sX: %.1f Y: %.1f Z: %.1f%s. Sneak to teleport!", GREEN, GRAY, pos.x, pos.y, pos.z, GREEN);
	}
	if (!GameData::isRightClickDown())
		hasClicked = false;

	GameSettingsInput* input = Game.getGameSettingsInput();
	if (shouldTP && GameData::isKeyDown(*input->sneak.key)) {
		tpPos.y += (gm->player->getPos()->y - gm->player->getAABB()->lower.y) + 1;  // eye height + 1
		if (bypass) {
			float dist = gm->player->getPos()->dist(tpPos);
			Game.getLocalPlayer()->lerpTo(tpPos, Vec2(1, 1), (int)fmax((int)dist * 0.1, 1));
		} else
			gm->player->setPos(tpPos);
		shouldTP = false;
	}
}
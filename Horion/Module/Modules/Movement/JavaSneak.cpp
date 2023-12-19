#include "JavaSneak.h"

JavaSneak::JavaSneak() : IModule(0, Category::MOVEMENT, "Crouch under 1.5 blocks") {
}

JavaSneak::~JavaSneak() {
}

const char* JavaSneak::getModuleName() {
	return ("JavaSneak");
}

void JavaSneak::onTick(GameMode* gm) {
	auto* input = Game.getMoveInputHandler();
	if (!(!Game.getLocalPlayer())) {
		if (input->isSneakDown) {
			Game.getLocalPlayer()->setSize(0.6f, 1.5f);
		} else {
			Game.getLocalPlayer()->setSize(0.6f, 1.8f);
		}
	}
}

void JavaSneak::onPostRender(MinecraftUIRenderContext* ctx) {
	auto* input = Game.getMoveInputHandler();
	auto plr = Game.getLocalPlayer();
	if (!plr)
		return;
	auto bp = Vec3i((int)plr->getPos()->x, (int)plr->getPos()->y, (int)plr->getPos()->z);
	if (bp.x < 0) bp.x -= 1;
	if (bp.y < 0) bp.y -= 1;
	if (bp.z < 0) bp.z -= 1;
	auto epic = Game.getRegion()->getBlock(bp);
	static auto was = false;
	if (epic->blockLegacy->material->type != 0 /*MaterialType::Air*/ && epic->blockLegacy->material->isSolid && epic->blockLegacy->material->isBlockingMotion) {
		was = true;
		input->isSneakDown = true;
	} else if (was) {
		auto epic = GameData::isKeyDown(*Game.getGameSettingsInput()->sneak.key);
		if (!epic) {
			input->isSneakDown = false;
		}
		was = false;
	}
}
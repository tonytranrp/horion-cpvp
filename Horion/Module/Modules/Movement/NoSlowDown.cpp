#include "NoSlowDown.h"
#include "../../../../Utils/Utils.h"

NoSlowDown::NoSlowDown() : IModule(0, Category::MOVEMENT, "Don't get slowed down when going in webs, bushes, snow or eating") {
}

NoSlowDown::~NoSlowDown() {
}

const char* NoSlowDown::getModuleName() {
	return ("NoSlowDown");
}

void NoSlowDown::onTick(GameMode* gm) {
	gm->player->resetBlockMovementSlowdownMultiplier();
}

void NoSlowDown::onEnable() {
}

void NoSlowDown::onDisable() {
}

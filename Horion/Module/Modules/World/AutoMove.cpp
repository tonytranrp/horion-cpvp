#include "AutoMove.h"

AutoMove::AutoMove() : IModule(0, Category::WORLD, "Automatically move foward!") {
	this->registerBoolSetting("Sprint", &this->sprint, this->sprint);
	this->registerBoolSetting("Jump", &this->jump, this->jump);
}

AutoMove::~AutoMove() {}

const char* AutoMove::getModuleName() {
	return ("AutoMove");
}

void AutoMove::onTick(GameMode* gm) {
	auto player = Game.getLocalPlayer();

	Game.getMoveInputHandler()->forward = true;

	static auto SpeedMod = moduleMgr->getModule<Speed>("Speed");

	static auto AutoSprintMod = moduleMgr->getModule<AutoSprint>("AutoSprint");

	if (!sprint && !SpeedMod->isEnabled()) gm->player->setSprinting(false);

	if (sprint && !AutoSprintMod->isEnabled()) gm->player->setSprinting(true);

	if (player->isOnGround() && jump) 
		player->jumpFromGround();
}

void AutoMove::onDisable() {
	Game.getMoveInputHandler()->forward = false;
}

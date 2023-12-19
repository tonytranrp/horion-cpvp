#include "Jesus.h"

Jesus::Jesus() : IModule(0, Category::MOVEMENT, "Walk over water, like Jesus") {
	this->registerBoolSetting("Solid", &this->solid, this->solid);
}

Jesus::~Jesus() {
}

const char* Jesus::getModuleName() {
	return "Jesus";
}

bool Jesus::isBlockBelowWater(Vec3 blockPos) {
	return Game.getRegion()->getBlock(Vec3i(blockPos.floor()))->blockLegacy->material->isLiquid;
}

void Jesus::onTick(GameMode* gm) {
	if (gm->player->isSneaking()) return;

	if (solid) {
		Vec3 blockBelow = Game.getLocalPlayer()->getEyePos();
		blockBelow.y -= Game.getLocalPlayer()->aabbShape->height;

		if (isBlockBelowWater(blockBelow) && !GameData::isKeyDown(*Game.getGameSettingsInput()->spaceBar.key)) {
			gm->player->stateVector->velocity.y = -0.0000000001f;
			gm->player->setOnGround(true);
			wasInWater = true;
		} else if (gm->player->isInWater() || gm->player->isInLava()) {
			gm->player->stateVector->velocity.y = 0.1f;
			gm->player->setOnGround(true);
			wasInWater = true;
		} else {
			if (wasInWater) {
				wasInWater = false;
				gm->player->stateVector->velocity.x *= 1.2f;
				gm->player->stateVector->velocity.x *= 1.2f;
			}
		}
	} else {
		if (gm->player->hasEnteredWater()) {
			gm->player->stateVector->velocity.y = 0.06f;
			gm->player->setOnGround(true);
			wasInWater = true;
		} else if (gm->player->isInWater() || gm->player->isInLava()) {
			gm->player->stateVector->velocity.y = 0.1f;
			gm->player->setOnGround(true);
			wasInWater = true;
		} else {
			if (wasInWater) {
				wasInWater = false;
				gm->player->stateVector->velocity.x *= 1.2f;
				gm->player->stateVector->velocity.x *= 1.2f;
			}
		}
	}
}
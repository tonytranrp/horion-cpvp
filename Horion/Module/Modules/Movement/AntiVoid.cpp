#include "AntiVoid.h"

AntiVoid::AntiVoid() : IModule(0, Category::MOVEMENT, "Automatically teleports you back up if you fall down more than X blocks") {
	registerIntSetting("distance", &this->distance, this->distance, 1, 20);
}

AntiVoid::~AntiVoid() {
}

const char* AntiVoid::getModuleName() {
	return ("AntiVoid");
}

std::string AntiVoid::getModeText() {
	return "Distance: " + std::to_string(distance);
}

void AntiVoid::onTick(GameMode* gm) {
	LocalPlayer* player = Game.getLocalPlayer();
	Vec3 blockBelow = player->getEyePos();
	blockBelow.y -= player->aabbShape->height;
	blockBelow.y -= 0.5f;

	if (((Game.getRegion()->getBlock(blockBelow)->blockLegacy))->blockId != 0 && ((Game.getRegion()->getBlock(blockBelow)->blockLegacy))->material->isSolid) {
		savepos = blockBelow;
		savepos.y += player->aabbShape->height;
		savepos.y += 0.5f;
	}

	if (player->getFallDistance() >= distance) {
		player->setPos(savepos);
	}
}
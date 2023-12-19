#include "Nuker.h"

Nuker::Nuker() : IModule(VK_NUMPAD5, Category::WORLD, "Break multiple blocks at once") {
	this->registerIntSetting("radius", &this->nukerRadius, this->nukerRadius, 1, 10);
	this->registerBoolSetting("veinminer", &this->veinMiner, this->veinMiner);
	this->registerBoolSetting("auto destroy", &this->autodestroy, this->autodestroy);
}

const char* Nuker::getModuleName() {
	return ("Nuker");
}

std::string Nuker::getModeText() {
	return "Radius: " + std::to_string(nukerRadius);
}

void Nuker::onTick(GameMode* gm) {
	if (!this->autodestroy) return;
	auto feetBlockPos = gm->player->getFeetBlockPos();

	for (int32_t x = (feetBlockPos.x - this->nukerRadius); x < (feetBlockPos.x + this->nukerRadius); x++) {
		for (int32_t y = (feetBlockPos.y - this->nukerRadius); y < (feetBlockPos.y + this->nukerRadius); y++) {
			for (int32_t z = (feetBlockPos.z - this->nukerRadius); z < (feetBlockPos.z + this->nukerRadius); z++) {
				if ((y >= -64) && (y <= 320) &&
					Game.getRegion()->getBlock({x, y, z})->toLegacy()->material->isSolid) {
					gm->destroyBlock({x, y, z}, 0);
				}
			}
		}
	}
}
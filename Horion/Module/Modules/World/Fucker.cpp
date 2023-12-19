#include "Fucker.h"

Fucker::Fucker() : IModule(VK_NUMPAD9, Category::WORLD, "Destroys certain blocks around you") {
	registerIntSetting("Range", &this->range, this->range, 1, 10);
	registerBoolSetting("Beds", &this->beds, this->beds);
	registerBoolSetting("Eggs", &this->eggs, this->eggs);
	registerBoolSetting("Cakes", &this->cakes, this->cakes);
	registerBoolSetting("Treasures", &this->treasures, this->treasures);
	registerBoolSetting("Chests", &this->chests, this->chests);
	registerBoolSetting("Barrels", &this->barrels, this->barrels);
}

Fucker::~Fucker() {
}

const char* Fucker::getModuleName() {
	return ("Fucker");
}

std::string Fucker::getModeText() {
	return "Range: " + std::to_string(range);
}

void Fucker::onTick(GameMode* gm) {
	auto& lp = *Game.getLocalPlayer();
	auto feetBlockPos = lp.getFeetBlockPos();

	for (int x = feetBlockPos.x - range; x < feetBlockPos.x + range; x++) {
		for (int z = feetBlockPos.z - range; z < feetBlockPos.z + range; z++) {
			for (int y = feetBlockPos.y - range; y < feetBlockPos.y + range; y++) {

				Vec3i currBlockPos = Vec3i(x, y, z);
				uint16_t id = Game.getRegion()->getBlock(currBlockPos)->toLegacy()->blockId;

				bool destroy = false;
				bool eat = false;
				switch (id) {
				case 26: {  // Beds
					if (this->beds) { destroy = true; }
					break;
				}
				case 122: {  // Dragon Eggs
					if (this->eggs) { destroy = true; }
					break;
				}
				case 92: {  // Cakes
					if (this->cakes) { eat = true; }
					break;
				}
				case 54: {  // Chests
					if (this->chests) { destroy = true; }
					break;
				}
				case 458: {  // Barrels
					if (this->barrels) { destroy = true; }
					break;
				}
				default: break;
				}

				if (destroy) {
					gm->destroyBlock(currBlockPos, 1);
					lp.swing();
					return;
				}

				if (eat) {
					gm->buildBlock(currBlockPos, 1);
					lp.swing();
					return;
				}
			}
		}
	}

	if (this->treasures) {
		Game.forEachEntity([&lp, gm](Entity* ent) {
			if ((ent->getNameTag().find("Treasure") != std::string::npos) && (lp.getPos()->dist(*ent->getPos()) <= 5.f)) {
				gm->attack(ent);
				lp.swing();
			}
		});
	}
}
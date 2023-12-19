#include "ChestAura.h"

ChestAura::ChestAura() : IModule(0, Category::WORLD, "Aura for opening chests") {
	registerIntSetting("Range", &this->range, this->range, 1, 10);
	registerBoolSetting("EnderChests", &this->enderchests, this->enderchests);
	registerBoolSetting("Trapped Chest", &this->trappedchest, this->trappedchest);
	registerBoolSetting("Shulker Boxes", &this->shulkerbox, this->shulkerbox);
}

ChestAura::~ChestAura() {
}

const char* ChestAura::getModuleName() {
	return ("ChestAura");
}

std::string ChestAura::getModeText() {
	return "Range: " + std::to_string(range);
}

void ChestAura::onTick(GameMode* gm) {
	if (Game.getInventory()->isFull())
		return;

	Vec3* pos = gm->player->getPos();
	for (int x = (int)pos->x - range; x < pos->x + range; x++) {
		for (int z = (int)pos->z - range; z < pos->z + range; z++) {
			for (int y = (int)pos->y - range; y < pos->y + range; y++) {
				Vec3i pos = Vec3i(x, y, z);
				Block* block = Game.getRegion()->getBlock(pos);
				if (block && Game.canUseMoveKeys()) {
					auto id = Game.getRegion()->getBlock(pos)->toLegacy()->blockId;
					bool open = false;

					if (id == 54) open = true;                              // Chests
					if (id == 130 && enderchests) open = true;              // EnderCheats
					if (id == 146 && trappedchest) open = true;             // TrappedChests
					if (id == 205 || id == 218 && shulkerbox) open = true;  // ShulkerBoxes
					if (open)
						if (!(std::find(chestlist.begin(), chestlist.end(), pos) != chestlist.end())) {
							gm->buildBlock(pos, 0);
							chestlist.push_back(pos);
							return;
						}
				}
			}
		}
	}
}

void ChestAura::onDisable() {
	this->chestlist.clear();  // this code should be changed later, the chestlist has to be cleared when loading into a new world
}
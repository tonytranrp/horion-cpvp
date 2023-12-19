#include "Surround.h"

Surround::Surround() : IModule(0, Category::WORLD, "Surrounds the bottom half of you in obsidian") {
	this->registerBoolSetting("Swap", &this->swap, this->swap);
	this->registerBoolSetting("Centre Player", &this->centre, this->centre);
	this->registerBoolSetting("Disable Complete", &this->disableOnComplete, this->disableOnComplete);
}

Surround::~Surround() {
}

const char* Surround::getModuleName() {
	return "Surround";
}
int lastSlot = -1;
bool weFoundABlock = false;
void Surround::onTick(GameMode* gm) {
	auto player = Game.getLocalPlayer();
	if (!player) return;

	if (swap) {
		for (int i = 0; i < 9; i++) {
			auto CurrentStack = player->getSupplies()->inventory->getItemStack(i);
			if (CurrentStack->isValid() && strcmp(CurrentStack->getItem()->name.c_str(), "obsidian") == 0) {
				if (!weFoundABlock) {
					weFoundABlock = true;
					player->getSupplies()->selectedHotbarSlot = i;
					return;//we need a 1 tick delay at least to fully swap
				}
			}
		}
	}

	auto selectedItem = player->getSelectedItem();
	if (selectedItem->isValid() && strcmp(selectedItem->getItem()->name.c_str(), "obsidian") != 0) {  //check to make sure we're holding the item
		clientMessageF("Error, %s", swap ? "couldn't find obsidian" : "please hold obsidian");
		this->setEnabled(false);
		return;
	}

	auto& region = *Game.getRegion();
	for (auto& i : PotentialSurroundBlocks) {
		auto playerPos = Game.getLocalPlayer()->getPos();
		Vec3i flooredPos = playerPos->floor();
		flooredPos.y -= 1;

		Vec3i newBlockPos(flooredPos.add(i));
		if (region.getBlock(newBlockPos)->toLegacy()->canBeBuiltOver(region, newBlockPos)) {
			gm->buildBlock(newBlockPos, 1);
		}
	}
	if (disableOnComplete) this->setEnabled(false);
}

void Surround::onEnable() {
	auto player = Game.getLocalPlayer();
	if (!player) return;

	weFoundABlock = false;
	if (player->isOnGround() && centre) {
		Game.getMoveInputHandler()->clearMovementState();  //we need to stop before we start to place to avoid fucking up
		player->stateVector->velocity = {0, 0, 0};
		player->setPos(player->getPos()->floor().add(0.5f, 0.f, 0.5f));

	} else if (!player->isOnGround()) {
		clientMessageF("Error, please use whilst on ground");
		this->setEnabled(false);
	}
	lastSlot = Game.getSupplies()->selectedHotbarSlot; 
}

void Surround::onDisable() {
	if (swap) Game.getSupplies()->selectedHotbarSlot = lastSlot;
}
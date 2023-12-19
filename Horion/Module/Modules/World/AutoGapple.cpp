#include "AutoGapple.h"

AutoGapple::AutoGapple() : IModule(0, Category::WORLD, "Eat gapples automatically!") {
}

AutoGapple::~AutoGapple() {
}

const char* AutoGapple::getModuleName() {
	return "AutoGapple";
}

void AutoGapple::onTick(GameMode* gm) {
	if (!Game.getLocalPlayer()) return;
	auto Player = Game.getLocalPlayer();
	if (Player->getSelectedItem()->isValid()) {
		if (Player->getSelectedItem()->getItem()->itemId == 258 || Player->getSelectedItem()->getItem()->itemId == 259) {
			if (Player->remainingTicksUsingItem <= 0) {
				Game.getGameMode()->baseUseItem(*Player->getSelectedItem());
			}
		}
	}
}
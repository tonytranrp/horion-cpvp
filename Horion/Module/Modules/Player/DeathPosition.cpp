#include "DeathPosition.h"

DeathPosition::DeathPosition() : IModule(0, Category::PLAYER, "Logs your last known position before you died") {
}

DeathPosition::~DeathPosition() {
}

const char* DeathPosition::getModuleName() {
	return ("DeathPosition");
}

void DeathPosition::onTick(GameMode* gm) {
	auto player = Game.getLocalPlayer();
	if (!player) return;
	int DeathTime = (int)player->getDeathTime();
	Vec3* pos = Game.getLocalPlayer()->getPos(); //pos->y = eye height
	if (DeathTime == 1) {
		GuiData* guiData = Game.getGuiData();
		if (guiData) {
			Utils::setClipboardText(std::string("Death Position:") + " " + std::to_string((int)pos->x) + ", " + std::to_string((int)pos->y - 2) + ", " + std::to_string((int)pos->z));
			guiData->displayClientMessageF("Death Position: %i, %i, %i", (int)pos->x, (int)pos->y - 2, (int)pos->z);
			guiData->displayClientMessageF("Your Current Death Location has been saved to the clipboard!");
		}
	}
}

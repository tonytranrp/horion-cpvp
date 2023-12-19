#include "MidClick.h"

MidClick::MidClick() : IModule(0, Category::PLAYER, "Click a player with your mouse wheel to add as friend") {
}

MidClick::~MidClick() {
}

const char* MidClick::getModuleName() {
	return "MidClick";
}

void MidClick::onTick(GameMode* gm) {
	Entity* entity = Game.getLevel()->getLookingAtEntity();
	if (!entity)
		return;

	std::string name = entity->getNameTag().c_str();

	if (GameData::isWheelDown() && !hasClicked) {
		if (!FriendList::findPlayer(name)) {
			FriendList::addPlayerToList(name);
			Game.getGuiData()->displayClientMessageF("%sSuccessfully added %s %sto your friendlist.", GREEN, name.c_str(), GREEN);
		} else {
			std::transform(name.begin(), name.end(), name.begin(), ::tolower);
			FriendList::removePlayer(name);
			Game.getGuiData()->displayClientMessageF("%sSuccessfully removed %s %sfrom your friendlist.", GREEN, name.c_str(), GREEN);
		}
		hasClicked = true;
	} else if (!GameData::isWheelDown()) {
		hasClicked = false;
	}
}
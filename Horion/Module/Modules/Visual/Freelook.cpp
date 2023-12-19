#include "Freelook.h"

#include "../../../../Utils/Logger.h"

Freelook::Freelook() : IModule(0, Category::VISUAL, "Move straight while moving your camera around") {
	this->setPerspectiveOptionPtr();
}

const char* Freelook::getModuleName() {
	return "Freelook";
}

void Freelook::onEnable() {
	if (!Game.isInGame()) {
		this->setEnabled(false);
		return;
	}

	this->initialViewAngles = Game.getLocalPlayer()->viewAnglesPtr->viewAngles;

	this->oldPerspective = *(this->perspectiveSetting);
	*(this->perspectiveSetting) = Perspective::THIRD_PERSON_BACK;
}
void Freelook::onDisable() {
	if (!Game.isInGame()) {
		return;
	}

	if (this->oldPerspective != Perspective::UNDEFINED) {
		*(this->perspectiveSetting) = this->oldPerspective;
		this->oldPerspective = Perspective::UNDEFINED;
	}
}

void Freelook::setPerspectiveOptionPtr() {
	auto perspectiveOption = Game.getClientInstance()->getOptions()->getOptionByName("game_thirdperson");
	if (perspectiveOption) {
		this->perspectiveSetting = reinterpret_cast<Perspective*>(&perspectiveOption->maxI); // no clue why but valueI doesn't work
	}
	else {
#ifdef HORION_DEBUG
		__debugbreak();
#endif
	}
}
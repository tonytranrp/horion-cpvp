#include "Phase.h"

Phase::Phase() : IModule('I', Category::MOVEMENT, "Walk through walls") {
	mode = (*new SettingEnum(this))
			   .addEntry(EnumEntry("NoClip", 0))
			   .addEntry(EnumEntry("NewClip", 1));
	registerEnumSetting("Mode", &mode, 1);
	registerFloatSetting("Clip Distance", &this->distance, this->distance, .1f, 3.f);
}

Phase::~Phase() {
}

const char* Phase::getModuleName() {
	return ("Phase");
}

void Phase::onTick(GameMode* gm) {
	switch (mode.GetSelectedEntry().GetValue()) {
		case 0:
			gm->player->aabbShape->aabb.upper.y = gm->player->aabbShape->aabb.lower.y;
			if (!oldmode)
				oldmode = true;
			break;
		case 1:
			if (gm->player->collidedHorizontally()) {
				Vec2 move = MoveUtils::GetMove(this->distance, gm->player->viewAnglesPtr->viewAngles.y, Game.getMoveInputHandler());
				gm->player->aabbShape->aabb.offset({move.x, .03f, move.y});
			}

			if (oldmode) {
				Game.getLocalPlayer()->aabbShape->aabb.upper.y += 1.8f;
				oldmode = false;
			}

			// 3itx is gay

			break;
	}
}

void Phase::onDisable() {
	if (Game.getLocalPlayer() && oldmode)
		Game.getLocalPlayer()->aabbShape->aabb.upper.y += 1.8f;
}
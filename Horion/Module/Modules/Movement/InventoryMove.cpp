#include "InventoryMove.h"

InventoryMove::InventoryMove() : IModule(0, Category::MOVEMENT, "Move even though you have your inventory open") {
}

InventoryMove::~InventoryMove() {
}

const char* InventoryMove::getModuleName() {
	return ("InventoryMove");
}

void InventoryMove::onTick(GameMode* gm) {
	if (gm->player->canOpenContainerScreen() == true)
		return;
	GameSettingsInput* input = Game.getGameSettingsInput();

	if (!input)
		return;

	float speed = 0.2805f;
	float yaw = gm->player->viewAnglesPtr->viewAngles.y;

	if (GameData::isKeyDown(*input->spaceBar.key) && gm->player->isOnGround())
		Game.getLocalPlayer()->jumpFromGround();

	if (GameData::isKeyDown(*input->forward.key) && GameData::isKeyDown(*input->back.key))
		return;
	else if (GameData::isKeyDown(*input->forward.key) && GameData::isKeyDown(*input->right.key) && !GameData::isKeyDown(*input->left.key)) {
		yaw += 45.f;
		keyPressed = true;
	} else if (GameData::isKeyDown(*input->forward.key) && GameData::isKeyDown(*input->left.key) && !GameData::isKeyDown(*input->right.key)) {
		yaw -= 45.f;
		keyPressed = true;
	} else if (GameData::isKeyDown(*input->back.key) && GameData::isKeyDown(*input->right.key) && !GameData::isKeyDown(*input->left.key)) {
		yaw += 135.f;
		keyPressed = true;
	} else if (GameData::isKeyDown(*input->back.key) && GameData::isKeyDown(*input->left.key) && !GameData::isKeyDown(*input->right.key)) {
		yaw -= 135.f;
		keyPressed = true;
	} else if (GameData::isKeyDown(*input->forward.key)) {
		keyPressed = true;
	} else if (GameData::isKeyDown(*input->back.key)) {
		yaw += 180.f;
		keyPressed = true;
	} else if (GameData::isKeyDown(*input->right.key) && !GameData::isKeyDown(*input->left.key)) {
		yaw += 90.f;
		keyPressed = true;
	} else if (GameData::isKeyDown(*input->left.key) && !GameData::isKeyDown(*input->right.key)) {
		yaw -= 90.f;
		keyPressed = true;
	}
	if (yaw >= 180)
		yaw -= 360.f;

	float calcYaw = (yaw + 90) * (PI / 180);
	Vec3 moveVec;
	moveVec.x = cos(calcYaw) * speed;
	moveVec.y = gm->player->stateVector->velocity.y;
	moveVec.z = sin(calcYaw) * speed;
	if (!gm->player->isOnGround())
		speed /= 1.5f;
	if (keyPressed) {
		gm->player->lerpMotion(moveVec);
		keyPressed = false;
	}
}
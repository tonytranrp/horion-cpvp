#include "NoClip.h"

NoClip::NoClip() : IModule(0x0, Category::MOVEMENT, "NoClip through walls on all axis") {
	registerFloatSetting("Horizontal Speed", &this->speed, this->speed, 0.1f, 3.f);
	registerFloatSetting("Vertical Speed", &this->upanddown, this->upanddown, 0.1f, 3.f);
	this->registerBoolSetting("Save Old POS", &this->NoPacket, this->NoPacket);
}

NoClip::~NoClip() {
}

const char* NoClip::getModuleName() {
	return ("NoClip");
}

void NoClip::onTick(GameMode* gm) {
	gm->player->aabbShape->aabb.upper.y = gm->player->aabbShape->aabb.lower.y - (float)1.8f;
	gm->player->stateVector->velocity = Vec3(0, 0, 0);
	glideModEffective = glideMod;
	GameSettingsInput* input = Game.getGameSettingsInput();

	if (Game.canUseMoveKeys()) {
		if (GameData::isKeyDown(*input->spaceBar.key))
			glideModEffective += upanddown;
		if (GameData::isKeyDown(*input->sneak.key))
			glideModEffective -= upanddown;
	}
	gm->player->stateVector->velocity.y = glideModEffective;
}

void NoClip::onMove(MoveInputHandler* input) {
	auto player = Game.getLocalPlayer();
	if (!player) return;


	float yaw = player->viewAnglesPtr->viewAngles.y;

	bool pressed = input->forward || input->backward || input->right || input->left;

	if (input->forward && input->backward)
		return;

	
	if (input->right) {
		yaw += 90.f;
		if (input->forward)
			yaw -= 45.f;
		else if (input->backward)
			yaw += 45.f;
	}
	if (input->left) {
		yaw -= 90.f;
		if (input->forward)
			yaw += 45.f;
		else if (input->backward)
			yaw -= 45.f;
	}

	if (input->backward && !input->left && !input->right)
		yaw += 180.f;

	float calcYaw = (yaw + 90) * (PI / 180);
	Vec3 moveVec;
	moveVec.x = cos(calcYaw) * speed;
	moveVec.y = player->stateVector->velocity.y;
	moveVec.z = sin(calcYaw) * speed;
	if(pressed) player->lerpMotion(moveVec);
}

void NoClip::onEnable() {
	if (NoPacket)
	if (Game.getLocalPlayer()) {
		oldPos = *Game.getLocalPlayer()->getPos();
	}
}

void NoClip::onDisable() {
	if (Game.getLocalPlayer())
		Game.getLocalPlayer()->aabbShape->aabb.upper.y = Game.getLocalPlayer()->aabbShape->aabb.lower.y + (float)1.8f;
	if (NoPacket)
		Game.getLocalPlayer()->setPos(oldPos);
}


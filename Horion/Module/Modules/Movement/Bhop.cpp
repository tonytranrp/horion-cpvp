#include "Bhop.h"

Bhop::Bhop() : IModule(0, Category::MOVEMENT, "Hop around like a bunny!") {
	registerFloatSetting("Speed", &this->speed, this->speed, 0.1f, 0.8f);
}

Bhop::~Bhop() {
}

const char* Bhop::getModuleName() {
	return ("Bhop");
}

std::string Bhop::getModeText() {
	char speedChar[50];
	sprintf_s(speedChar, "Speed: %.1f", speed);
	return speedChar;
}

void Bhop::onMove(MoveInputHandler* input) {
	auto player = Game.getLocalPlayer();
	if (!player) return;

	if (player->isInLava() || player->isInWater())
		return;

	if (player->isSneaking())
		return;

	if (moduleMgr->getModule<Fly>("Fly")->isEnabled()) return;  // Makes bhop not overwrite fly

	Vec2 moveVec2d = {input->forwardMovement, -input->sideMovement};
	bool pressed = moveVec2d.magnitude() > 0.01f;

	if (player->isOnGround() && pressed) {
		player->jumpFromGround();
	}
	float calcYaw = (player->viewAnglesPtr->viewAngles.y + 90) * (PI / 180);
	Vec3 moveVec;
	float c = cos(calcYaw);
	float s = sin(calcYaw);
	moveVec2d = {moveVec2d.x * c - moveVec2d.y * s, moveVec2d.x * s + moveVec2d.y * c};
	moveVec.x = moveVec2d.x * speed;
	moveVec.y = player->stateVector->velocity.y;
	moveVec.z = moveVec2d.y * speed;
	if (pressed)
		player->lerpMotion(moveVec);
	else {
		// Stop moving instantly
		player->stateVector->velocity.x = 0;
		player->stateVector->velocity.z = 0;
	}
}
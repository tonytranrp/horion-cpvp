#include "VHop.h"

VHop::VHop() : IModule(0, Category::MOVEMENT, "Hop around like Void!") {
	registerFloatSetting("Speed", &this->theSpeed, this->theSpeed, 1.0f, 3.0f);
	registerBoolSetting("Weird Friction", &this->weirdFriction, this->weirdFriction);
}

VHop::~VHop() {}

const char* VHop::getModuleName() {
	return "VHop";
}

void VHop::onMove(MoveInputHandler* input) {
	auto plr = Game.getLocalPlayer();
	if (!plr)
		return;

	Vec2 moveVec2d = {input->forwardMovement, -input->sideMovement};
	const bool onGround = plr->isOnGround() && plr->collidedVertically();
	constexpr float baseSpeed = .2873f;
	static bool otherTick;
	float vertical = 0.f;

	if (moveVec2d.magnitude() <= 0.01f || plr->isSneaking() || plr->isInWater() || plr->isInLava()) {
		if (stage && !warmups) {
			plr->stateVector->velocity.x = plr->stateVector->velocity.z = 0.f;
			stage = 0;
		}
		//warmups = 2;
		return;
	}

	if (stage <= 2 && !onGround)
		return;

	switch (stage++) {
	case 1:
		moveSpeed = baseSpeed * 1.35f;
		break;
	case 2:
		vertical = .4f;
		moveSpeed = baseSpeed * ((otherTick = !otherTick) ? 1.6f : theSpeed);
		break;
	case 5:
		moveSpeed = lastMoveSpeed - ((weirdFriction ? .99f : .66f) * (lastMoveSpeed - baseSpeed));
		break;
	case 6:
		vertical = -.2f;
		break;
	default:
		moveSpeed = lastMoveSpeed - lastMoveSpeed / 159;
		if (onGround)
			stage = 2;
		break;
	}

	if (stage == 2 && warmups)
		--warmups;

	const float actualMoveSpeed = lastMoveSpeed = warmups ? baseSpeed : std::max(moveSpeed, baseSpeed);

	// pastee
	float calcYaw = (plr->viewAnglesPtr->viewAngles.y + 90) * (PI / 180);
	Vec3 moveVec;
	float c = cos(calcYaw);
	float s = sin(calcYaw);
	moveVec2d = {moveVec2d.x * c - moveVec2d.y * s, moveVec2d.x * s + moveVec2d.y * c};
	moveVec.x = moveVec2d.x * actualMoveSpeed;
	moveVec.y = vertical != 0.f ? vertical : plr->stateVector->velocity.y;
	moveVec.z = moveVec2d.y * actualMoveSpeed;
	plr->lerpMotion(moveVec);
}

void VHop::onDisable() {
	IModule::onDisable();
	stage = 0;
	//warmups = 2;
}

void VHop::onEnable() {
	stage = 0;
	//warmups = 2;
	IModule::onEnable();
}
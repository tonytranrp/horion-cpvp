#include "FastStop.h"

FastStop::FastStop() : IModule(0, Category::MOVEMENT, "Stops Fast") {
}

FastStop::~FastStop() {
}

const char* FastStop::getModuleName() {
	return ("FastStop");
}
void FastStop::onMove(MoveInputHandler* input) {
	auto player = Game.getLocalPlayer();
	if (!player) return;

	if (player->isInLava() || player->isInWater()) 
		return;
	
	if (player->isSneaking()) 
		return;
	Vec2 moveVec2d = {input->forwardMovement, -input->sideMovement};
	bool pressed = moveVec2d.magnitude() > 0.01f;
	if (!pressed) {
		player->stateVector->velocity.x *= 0.f;
		player->stateVector->velocity.z *= 0.f;
	}
}

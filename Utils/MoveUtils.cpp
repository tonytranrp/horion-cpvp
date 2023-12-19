#include "MoveUtils.h"
void MoveUtils::Tp(float speed, bool lerp, float y) {
	auto plr = Game.getLocalPlayer();
	float yaw = plr->viewAnglesPtr->viewAngles.y * (PI / 180);
	float x = -sin(yaw) * speed;
	float z = cos(yaw) * speed;
	Vec3 pos = *Game.getLocalPlayer()->getPos();
	plr->setPos(pos.add(Vec3(x, y, z)));
	if (lerp) {
		Vec3 moveVec;
		float c = cos(yaw);
		float s = sin(yaw);
		moveVec.x = -sin(yaw) * speed;
		moveVec.y = plr->stateVector->velocity.y;
		moveVec.z = cos(yaw) * speed;
		plr->lerpMotion(moveVec);
	}
}
void MoveUtils::SetMotion(float speed, bool lerp, bool onTick) {
	auto player = Game.getLocalPlayer();
	float calcYaw = (player->viewAnglesPtr->viewAngles.y + 90) * (PI / 180);
	if (!onTick) {
		auto input = Game.getMoveInputHandler();
		Vec2 moveVec2d = {input->forwardMovement, -input->sideMovement};
		bool pressed = moveVec2d.magnitude() > 0.01f;
		Vec3 moveVec;
		float c = cos(calcYaw);
		float s = sin(calcYaw);
		moveVec2d = {moveVec2d.x * c - moveVec2d.y * s, moveVec2d.x * s + moveVec2d.y * c};
		moveVec.x = moveVec2d.x * speed;
		moveVec.y = player->stateVector->velocity.y;
		moveVec.z = moveVec2d.y * speed;
		if (pressed) {
			if (lerp) {
				player->lerpMotion(moveVec);
			} else {
				player->stateVector->velocity = (moveVec);
			}
		} else if (onTick) {
			Vec3 moveVec;
			float c = cos(calcYaw);
			float s = sin(calcYaw);
			moveVec.x = c * speed;
			moveVec.y = player->stateVector->velocity.y;
			moveVec.z = s * speed;
			if (lerp) {
				player->lerpMotion(moveVec);
			} else {
				player->stateVector->velocity = (moveVec);
			}
		}
	}
}

Vec2 MoveUtils::GetMove(float speed, float direction, MoveInputHandler *input) {
	direction += 90;
	char strafe = 0, forward = 1;
	if (input) {
		forward = input->forwardMovement > 0.f ? 1 : input->forwardMovement < 0.f ? -1 : 0;
		strafe = input->sideMovement > 0.f ? 1 : input->sideMovement < 0.f ? -1 : 0;
		if (forward && strafe) {
			direction -= strafe * forward * 45;
			strafe = 0;
		}
	}
	float mX = cosf(direction * PI / 180),
			mZ = sinf(direction * PI / 180);
	return { forward * speed * mX + strafe * speed * mZ, forward * speed * mZ - strafe * speed * mX };
}

bool MoveUtils::isOnground(float height) {
	auto player = Game.getLocalPlayer();
	Vec3 blockBelow = (Vec3(player->getPos()->x, (float)(player->aabbShape->aabb.lower.y - height), player->getPos()->z));
	if (Game.getRegion()->getBlock(blockBelow)->blockLegacy->material->isSolid) {
		return true;
	} 
	return false;
}

bool MoveUtils::isOverVoid() {
	for (float posY = Game.getLocalPlayer()->getPos()->y; posY > 0.0f; --posY) {
		if (!(Game.getRegion()->getBlock(Vec3(Game.getLocalPlayer()->getPos()->x, posY, Game.getLocalPlayer()->getPos()->z))->blockLegacy->blockId == 0)) {
			return false;
		}
	}
	return true;
}
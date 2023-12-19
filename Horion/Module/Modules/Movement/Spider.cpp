#include "Spider.h"

Spider::Spider() : IModule(0, Category::MOVEMENT, "Climb walls") {
	registerFloatSetting("Speed", &speed, 0.6f, 0.1f, 1.f);
}

Spider::~Spider() {
}

const char* Spider::getModuleName() {
	return "Spider";
}

std::string Spider::getModeText() {
	char speedChar[50];
	sprintf_s(speedChar, "Speed: %.1f", speed);
	return speedChar;
}

void Spider::onMove(MoveInputHandler* input) {
	LocalPlayer* player = Game.getLocalPlayer();
	if (!player)
		return;

	if (player->isInLava() || player->isInWater())
		return;

	if (player->isSneaking())
		return;

	std::vector<Vec3i> sideBlocks;
	sideBlocks.reserve(8);

	Vec2 moveVec2d = {input->forwardMovement, -input->sideMovement};
	bool pressed = moveVec2d.magnitude() > 0.01f;
	if (!pressed)
		return;
	moveVec2d = moveVec2d.normalized();

	float calcYaw = (player->viewAnglesPtr->viewAngles.y + 90) * (PI / 180);
	Vec3 moveVec;
	float c = cos(calcYaw);
	float s = sin(calcYaw);
	moveVec2d = {moveVec2d.x * c - moveVec2d.y * s, moveVec2d.x * s + moveVec2d.y * c};

	for (int x = -1; x <= 1; x++) {
		for (int z = -1; z <= 1; z++) {
			if (x == 0 && z == 0)
				continue;

			if (moveVec2d.dot(Vec2(x, z).normalized()) < 0.6f)
				continue;
			sideBlocks.push_back(Vec3i(x, 0, z));
		}
	}

	auto pPos = *player->getPos();
	pPos.y = player->aabbShape->aabb.lower.y;
	auto pPosI = Vec3i(pPos.floor());

	auto isObstructed = [&](int yOff, AABB* obstructingBlock = nullptr, bool ignoreYcoll = false) {
		for (const auto& current : sideBlocks) {
			Vec3i side = pPosI.add(0, yOff, 0).add(current);
			if (side.y < -64 || side.y >= 256)
				break;
			auto block = Game.getRegion()->getBlock(side);
			if (!block || !block->blockLegacy)
				continue;
			BlockLegacy* blockLegacy = block->toLegacy();
			if (!blockLegacy)
				continue;
			AABB collisionVec;
			if (!blockLegacy->getCollisionShape(&collisionVec, block, Game.getRegion(), &side))
				continue;
			bool intersects = ignoreYcoll ? collisionVec.intersectsXZ(player->aabbShape->aabb.expandedXZ(0.1f)) : collisionVec.intersects(player->aabbShape->aabb.expandedXZ(0.1f));

			if (intersects) {
				if (obstructingBlock)
					*obstructingBlock = collisionVec;
				return true;
			}
		}
		return false;
	};

	AABB lowerObsVec, upperObsVec;
	bool upperObstructed = isObstructed(1, &upperObsVec);

	bool lowerObstructed = isObstructed(0, &lowerObsVec);
	float targetSpeed = speed;
	if ((lowerObstructed || upperObstructed)) {
		// simulate because im too lazy to do the math
		const auto distanceError = [](float yVel, float distance) {
			int t = 0;
			constexpr int numIter = 60;
			for (; t < numIter; t++) {
				distance -= yVel;
				yVel -= 0.08f;  // gravity
				yVel *= 0.98f;  // drag

				if (yVel <= 0)
					break;
			}
			return std::tuple(distance, yVel, t);
		};

		float getOver = lowerObstructed ? lowerObsVec.upper.y : upperObsVec.upper.y;
		{
			if (upperObstructed)
				getOver = std::max(getOver, upperObsVec.upper.y);
			// max dist that can be reached with our speed

			const int numIterations = (int)ceil(std::max(5.f, -std::get<0>(distanceError(speed, 0)) + 2));

			bool lastObstructed = upperObstructed;
			for (int iter = 2; iter < numIterations; iter++) {
				if (isObstructed(iter, &upperObsVec, true)) {
					getOver = std::max(getOver, upperObsVec.upper.y);
					lastObstructed = true;
				} else {
					if (lastObstructed)
						lastObstructed = false;
					else
						break;
				}
			}
		}

		float targetDist = getOver - pPos.y + 0.02f;
		if (targetDist <= 0)
			return;

		auto [curDist, curYVel, curT] = distanceError(player->stateVector->velocity.y, targetDist);

		//this->clientMessageF("current trajectory error=%.3f t=%i vel=%.3f total=%.2f", curDist, curT, curYVel, targetDist);
		if (curDist <= 0.01f)
			return;  // We will already get on top of the block

		if (player->stateVector->velocity.y < speed) {
			// do another simulation to determine whether we would overshoot on the next iteration
			auto secondTrajectory = distanceError(speed, targetDist);
			//this->clientMessageF("secondTrajectory: error=%.3f t=%i vel=%.2f", std::get<0>(secondTrajectory), std::get<2>(secondTrajectory), std::get<1>(secondTrajectory));
			if (std::get<0>(secondTrajectory) <= 0) {  // we are overshooting if we give the player our target speed
				// use secant method to approximate perfect start speed
				float error = curDist;
				float startSpeed = player->stateVector->velocity.y;

				float error2 = std::get<0>(secondTrajectory);
				float startSpeed2 = speed;
				int i = 0;
				for (; i < 16; i++) {
					if (error > -0.05f && error <= 0.001f)
						break;  // its better to slightly overshoot than to undershoot

					float newSpeed = (startSpeed2 * error - startSpeed * error2) / (error - error2);
					startSpeed2 = startSpeed;
					startSpeed = newSpeed;

					error2 = error;
					error = std::get<0>(distanceError(newSpeed, targetDist));
				}
				//this->clientMessageF("Secant method finished with error=%.3f speed=%.3f at t=%i", error, startSpeed, i);
				targetSpeed = startSpeed;
			}
		}
	}
	if (upperObstructed || lowerObstructed) {
		if (player->stateVector->velocity.y < targetSpeed)
			player->stateVector->velocity.y = targetSpeed;
	}
}